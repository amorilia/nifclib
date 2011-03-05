/***************************************************************************
 *   Copyright (C) 2011 by corwin @                                       *
 *   http://niftools.sourceforge.net/forum/                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"

#define TRY(COND, MSG, RETC) \
if ((COND)) {\
	info (MSG);\
	return RETC;\
}

#define MWORD "broken"

			/*
		---/   boundary syntax elements of the xml    / /---
												  ---/*/
#define EOA "\""
#define BA "<basic"
#define BB "</basic>"
#define BNAME "name=\""
#define BTYPE "niflibtype=\""

#define EA "<enum"
#define EB "</enum>"
#define ENAME "name=\""
#define ETYPE "storage=\""
#define E_OA "<option"
#define E_OB "</option>"
#define E_OVALUE "value=\""
#define E_ONAME "name=\""

#define FA "<bitflags"
#define FB "</bitflags>"

#define CA "<compound"
#define CB "</compound>"
#define CNAME "name=\""
#define CINH "inherit=\""
#define CVER1 "ver1=\""
#define CVER2 "ver2=\""
#define CUVER "userver=\""
#define C_AA "<add"
#define C_AB "</add>"
#define C_ANAME "name=\""
#define C_ATYPE "type=\""
#define C_AVER1 "ver1=\""
#define C_AVER2 "ver2=\""
#define C_AARR1 "arr1=\""
#define C_AARR2 "arr2=\""
#define C_ACOND " cond=\""
#define C_AARG "arg=\""
#define C_ADEF "default=\""
#define C_AVERCOND "vercond=\""
#define C_ATEMPL "template=\""
#define C_AUVER "userver=\""

#define NA "<niobject"
#define NB "</niobject>"

			/*
		---/   internal procs    / /---
							 ---/*/

static int convert (FILE *fh, const char *fname, long fsize);
static int convert_buf (const char *buf, int buflen);
static int info(const char *fmt, ...);
static int dbg(const char *fmt, ...);
static void usage();
// codegen procs
#define CG 1

#if CG
static int cg_prologue();
static int cg_basic(const char *name, const char *type);
static int cg_enum_begin(const char *name, const char *type);
static int cg_enum_optn(const char *name, const char *value);
static int cg_enum_end();
static int cg_struct_begin(const char *name, const char *inh);
static int cg_struct_field(
	const char *structname,
	const char *name,
	const char *type,
	const char *v1,
	const char *v2,
	const char *a1,
	const char *a2,
	const char *cond,
	const char *arg,
	const char *def,
	const char *vcond,
	const char *templ,
	const char *uv);
static int cg_struct_end(const char *name);
static int cg_epilogue();
#endif /* CG */

static hash *rch; // read calls hash - maps basic.name to a readcall
				  //                 - maps enum.name to a readcall
static hash *th; // type hash - maps basic.name to a c language type
				 //           - maps enum.name to a c language type

static hash *emptyh; // contains empty types - NiObject and its level 1 desc.

static hash *nikeygroups;
static hash *nikeys;

//static FILE *newformat;

int
main(int argc, char **argv)
{
	/*
		-move SkinTransform above SkinData
		TODO: the above is still done manually
	*/
	//newformat = fopen ("nif.new.format", "w+");
	//char *fname = "latest/nif.xml";
	char *fname = NULL;
	if (argc == 2 && argv[1]) {
		fname = argv[1];
		FILE *fh = fopen (fname, "r");
		TRY (!fh, "file open failed\n", EXIT_FAILURE)
		TRY (fseek (fh, 0, SEEK_END), "seek failed\n", EXIT_FAILURE)
		long fs = ftell (fh);
		TRY (fs == -1, "seek failed\n", EXIT_FAILURE)
		info ("converting \"%s\" - %ld bytes ...\n", fname, fs);
		TRY (convert (fh, fname, fs), "convert failed\n", EXIT_FAILURE)
		TRY (fclose (fh), "close failed\n", EXIT_FAILURE)
		info ("done.\n");
	}
	else usage ();
	//fclose (newformat);
	return EXIT_SUCCESS;
}

static void
usage()
{
	printf ("\"convert2c\" v. 0.1a\n");
	printf ("Usage:\tconvert2c filename\n\n");
}

static int
info(const char *format, ...)
{
	return 0;
	va_list arg;
	int done;
	va_start (arg, format);
	done = vprintf (format, arg);
	va_end (arg);
	return done;
}

static int
dbg(const char *format, ...)
{
	return 0;
	va_list arg;
	int done;
	va_start (arg, format);
	done = vprintf (format, arg);
	va_end (arg);
	return done;
}

static int
cgdbg(const char *format, ...)
{
	return 0;
	va_list arg;
	int done;
	va_start (arg, format);
	done = vprintf (format, arg);
	va_end (arg);
	return done;
}

			/*
   		---/   parser   / /---
					---/*/

#define MAX_LBUF 1*1024*1024

static int
convert (FILE *fh, const char *fname, long fsize)
{
	if (fsize > (long)MAX_LBUF) {// Can't handle too large nif.xml
		info ("I can not work with that file size yet\n");
		info ("I can only handle up to %d bytes\n", MAX_LBUF);
		return EXIT_FAILURE;
	}
	int buflen = (int)fsize;
	char *buf = malloc (buflen);
	if (!buf) {// Can't allocate the buffer
		info ("not enough memory to allocate %d bytes\n", buflen);
		return EXIT_FAILURE;
	}
	fseek (fh, 0, SEEK_SET);
	if (fread (&buf[0], buflen, 1, fh) != 1) {
		info ("failed to read the file in memory\n");
		free (buf);
		return EXIT_FAILURE;
	}
	rch = hashcreate (500);
	if (!rch) {
		free (buf);
		printf ("hash create failed\n");
		return EXIT_FAILURE;
	}
	th = hashcreate (500);
	if (!th) {
		free (buf);
		hashrelease (rch, 1);
		printf ("hash create failed\n");
		return EXIT_FAILURE;
	}
	emptyh = hashcreate (500);
	if (!emptyh) {
		free (buf);
		hashrelease (rch, 1);
		hashrelease (th, 1);
		printf ("hash create failed\n");
		return EXIT_FAILURE;
	}
	nikeygroups = hashcreate (20);// TODO: this can not continue forever -
								  // think of a sane way to check and clean up
	nikeys = hashcreate (20);
	// UnionBV and BoundingVolume are in circular reference so one
	// of them not used anywhere elese - UnionBV, becomes a pointer
	hashadd (th, strcopy( "UnionBV"), strcopy ("struct UnionBV *"));
	int result = convert_buf (buf, buflen);
	free (buf);
	hashrelease (rch, 1);
	hashrelease (th, 1);
	hashrelease (emptyh, 1);
	hashrelease (nikeygroups, 1);
	hashrelease (nikeys, 1);
	return result;
}

// keep track of line number
static void
update_line_num(char key, int *line)
{
	if (key == '\n')
		(*line)++;
}

// removes all xml comments from buf and puts the result in *out
static int
skip_comments(const char *buf, int buflen, char **out)
{
	// regex (incl. \r and/or \n) <!--.*-->
	char *o = *out;
	int mp, i = 0, line = 1;
	for (mp = 0; mp < buflen; mp++) {
		update_line_num (buf[mp], &line);
		if (starts_with (&buf[mp], buflen - mp, "<!--")) {
			int cstart = mp;
			int sline = line;
			while (!starts_with (&buf[mp], buflen - mp, "-->")) {
				mp++;
				if (mp == buflen) {
					dbg ("unterminated \"<!--\" at %d\n", sline);
					return 0;
				}
				update_line_num (buf[mp], &line);
			}
			mp += 3;
			dbg (" skipped comment %d bytes,", mp-cstart);
			dbg (" %d lines starting from line %d\n",
				line - sline, sline);
		}
		o[i++] = buf[mp];
	}
	return i;
}

// turns a symbol into a valid 'c' language identifier
static void
fix_symbol(char *buf, int len)
{
	// no spaces and underscores are needed
	// neither question marks
	int src, dst = 0;
	for (src = 0; src < len; src++)
		if (buf[src] != ' ' && buf[src] != '?')
			buf[dst++] = buf[src];
	buf[dst] = '\0';
}

// coverage cunters: <[b]asic, <[e]num, <bit[f]lags, <[c]ompound, <[n]iobject
static int bc = 0, ec = 0, fc = 0, cc = 0, nc = 0;

// parse <basic...</basic>. returns true if ok, false otherwise
static int
pbasic(const char *buf, int len)
{
	bc++;
	dbg ("B %d, ", len);
	char *name = get_between (&buf[0], len, BNAME, EOA);
	char *type = get_between (&buf[0], len, BTYPE, EOA);
	TRY (!name, MWORD" "BNAME"\n", 0)
	TRY (!type, MWORD" "BTYPE"\n", 0)
	fix_symbol (&name[0], strlen (name));
	fix_symbol (&type[0], strlen (type));
	dbg ("name=%s, type=%s\n", name, type);
#if CG
	cg_basic (name, type);
#endif /* CG */
	free (name);
	free (type);
	// display the tag
	/*char *test = malloc (len+1);
	memcpy (&test[0], &buf[0], len);
	test[len] = '\0';
	printf ("%s\n", test);
	free (test);*/
	return 1;
}

// parse <enum...</enum>. returns true if ok, false otherwise
static int
penum(const char *buf, int len)
{
	ec++;
	dbg ("E %d, ", len);
	char *name = get_between (&buf[0], len, ENAME, EOA);
	char *type = get_between (&buf[0], len, ETYPE, EOA);
	TRY (!name, MWORD" "ENAME"\n", 0)
	TRY (!type, MWORD" "ETYPE"\n", 0)
	fix_symbol (&name[0], strlen (name));
	fix_symbol (&type[0], strlen (type));
	dbg ("name=%s, type=%s\n", name, type);
#if CG
	cg_enum_begin (name, type);
#endif /* CG */
	int i;
	for (i = 0; i < len; i++) {
		if (starts_with (&buf[i], len - i, E_OA)) {
			int e = find (E_OB, &buf[i], len - i);
			TRY (e <= 0, MWORD" "E_OA"\n", 0)
			char *val = get_between (&buf[i], e, E_OVALUE, EOA);
			char *nameo = get_between (&buf[i], e, E_ONAME, EOA);
			TRY (!val, MWORD" "E_OVALUE"\n", 0)
			TRY (!nameo, MWORD" "E_ONAME"\n", 0)
			//fix_symbol (&nameo[0], strlen (nameo)); - need to keep their _
			// some names do have spaces in them so fix that
			replace (&nameo[0], strlen (nameo), ' ', '_');
			dbg (" optn: %s=%s\n", nameo, val);
			i += e;
#if CG
			cg_enum_optn (nameo, val);
#endif /* CG */
			free (val);
			free (nameo);
		}
	}
#if CG
	cg_enum_end ();
#endif /* CG */
	free (name);
	free (type);
	// display the tag
	/*char *test = malloc (len+1);
	memcpy (&test[0], &buf[0], len);
	test[len] = '\0';
	printf ("%s\n", test);
	free (test);*/
	return 1;
}

// parse <bitflags...</bitflags>. returns true if ok, false otherwise
static int
pbitflags(const char *buf, int len)
{
	fc++;
	ec--;
	dbg ("F %d, ", len);
	return penum (buf, len);
}

static int
isLetter(char t)
{
	return 
		(t >= 'a' && t <= 'z') ||
		(t >= 'A' && t <= 'Z') ||
		(t >= '0' && t <= '9');
}

static void append_to(char **target, const char *a);

// find identifiers and remove spaces in their names
// find &amp; &gt; &lt; and replace with &, >, <
// TODO: too complex perhaps
static char *
fix_cond(char *cond)
{
	if (!cond)
		return NULL;
	int s = 0, i, j;
	int l = 0;
	int length = strlen (cond);
	int len = 2*length;
	char *buf = malloc (len);
	if (!buf) {
		cgdbg ("out of memory\n");
		return NULL;
	}
	memcpy (buf, cond, length);
	buf[length] = '\0';
	//printf ("buf: \"%s\"\n", buf);
	for (i = 0; i < length; i++) {
		if (isLetter (buf[i]) &&
			(buf[i] < '0' || buf[i] > '9')) { // identifier start
			//printf (" idfr start: %d\n", i);
			s = i;// identifier start
			for (l = i; l < length; l++) {
				if (!isLetter(buf[l]) || l == length - 2) {// check next
					if (l + 1 < length) {
						if (!isLetter (buf[l + 1]) || l == length - 2 ||
							buf[l] == '&' /*no space*/) {
							if (buf[l] == '&') l--;
							// l -> identifier end
							/*printf (" idfr stop: %d, \"", l); int q;
							for (q = s; q <= l; q++) printf ("%c", buf[q]);
							printf ("\"\n");*/
							int a1, a2, a3;
							if ((s + 3) < length)
								a1 = starts_with (&buf[s], 3, "ARG");
							else a1 = 0;
							if ((s + 12) < length)
								a2 = starts_with (&buf[s], 12, "User Version");
							else a2 = 0;
							if ((s + 7) < length)
								a3 = starts_with (&buf[s], 7, "Version");
							else a3 = 0;
							/*int a1 = s + 3 < length ? starts_with (&buf[s], 3, "ARG") : 0;
							int a2 = s + 12 < length ? starts_with (&buf[s], 3, "User Version") : 0;*/
							//printf (" a1:%d, a2:%d\n", a1, a2);
							if (!(a1 || a2 || a3)) {
								// it is not an arg
								// make place for r-> (3 symbols)
								if ( length + 3 >= len) {
									cgdbg ("buffer overflow\n");
									free (buf);
									return NULL;
								} else {length += 3; buf[length] = '\0'; }
								// shr
								int k;
								for (k = length - 1; k >= s + 3; k--)
									buf[k] = buf[k-3];
								memcpy (&buf[s], "r->", 3);// insert 'r->'
								l+=3;
							}
							for (j = s; j < l; j++)
								if (buf[j] == ' ') buf[j] = '_';
							i = l;// continue onwards
							break;
						}// else printf ("  -is a letter\n");
					}
				}
			}
		} else if (buf[i] == '&') {// scan for XML "escape sequence"
			while (i+1 < length && buf[i+1] == '&') i++;// skip consequent ones
			s = i;// XML special symbol start
			//printf (" found &\n");
			for (l = i; l < length; l++) {
				if (buf[l] == ';') { // yes it is
					char *tmp = malloc (l - s);
					if (!tmp) {
						cgdbg ("out of memory\n");
						free (buf);
						return NULL;
					}
					tmp[l - s - 1] = '\0';
					memcpy (&tmp[0], &buf[s+1], l - s - 1);
					char r = ' ';
					if (!strcmp (tmp, "amp")) r = '&';
					else if (!strcmp (tmp, "gt")) r = '>';
					else if (!strcmp (tmp, "lt")) r = '<';
					else {
						cgdbg ("unknown XML special symbol\n");
						free (buf);
						return NULL;
					}
					free (tmp);
					buf[s] = r;
					if (l + 1 < length) {
						int cnt = length - (l+1);
						//printf ("copying %d\n", cnt);
						memcpy (&buf[s+1], &buf[l+1], cnt);// shl
						buf[length - (l-s)] = '\0';
					}
					else { buf[s + 1] = '\0'; break; }
					i = s;// continue onwards
					//printf ("buf[i]: %c\n", buf[i]);
					break;
				}
			}
		}// else if (buf[i] == '&')
	}
	//printf ("buf with ids: \"%s\"\n", buf);
	int src, dst = 0;
	for (src = 0; src < length; src++)
		if (buf[src] != '_' && buf[src] != '?')
			buf[dst++] = buf[src];
	buf[dst] = '\0';
	//printf ("buf with real_ids: \"%s\"\n", buf);
	return buf;
}

// parse <compound...</compound>. returns true if ok, false otherwise
static int
pcompound(const char *buf, int len)
{
	cc++;
	dbg ("C %d, ", len);
	int tlen = find (">", &buf[0], len);
	if (tlen <= 0)// TODO: protect the above parsers as well
		return 0;// tag closer not found => bug or malformed XML
	char *name = get_between (&buf[0], tlen, CNAME, EOA);

	// skip things what are not for the lib
	if (starts_with (&name[0], strlen(name), "ns ")) {
		free (name);
		return 1;
	}

	char *inh = get_between (&buf[0], tlen, CINH, EOA);
	TRY (!name, MWORD" "CNAME"\n", 0)
	if (inh)
		fix_symbol (&inh[0], strlen (inh));
	fix_symbol (&name[0], strlen (name));

	// skip empty compound/niobject levels 0 and 1
	if (hashget (emptyh, name) ) {
		free (inh);
		return 1;
	}

	dbg ("name=%s, inh=%s\n",
		name, !inh ? "" : inh);
#if CG
	cg_struct_begin (name, inh);
#endif /* CG */
	int i;
	for (i = tlen; i < len; i++) {
		if (starts_with (&buf[i], len - i, C_AA)) {
			int e = find (C_AB, &buf[i], len - i);
			TRY (e <= 0, MWORD" "C_AA"\n", 0)
			char *aname = get_between (&buf[i], e, C_ANAME, EOA);
			char *atype = get_between (&buf[i], e, C_ATYPE, EOA);
			char *aver1 = get_between (&buf[i], e, C_AVER1, EOA);
			char *aver2 = get_between (&buf[i], e, C_AVER2, EOA);
			char *arr1 = get_between (&buf[i], e, C_AARR1, EOA);
			//char *arr11 = fix_cond (arr1);
			char *arr2 = get_between (&buf[i], e, C_AARR2, EOA);
			//char *arr21 = fix_cond (arr2);
			char *cond = get_between (&buf[i], e, C_ACOND, EOA);
			char *cond1 = fix_cond (cond);
			char *arg = get_between (&buf[i], e, C_AARG, EOA);
			char *arg1 = fix_cond (arg);
			char *def = get_between (&buf[i], e, C_ADEF, EOA);
			char *vc = get_between (&buf[i], e, C_AVERCOND, EOA);
			char *vc1 = fix_cond (vc);
			// in other languages there is perhaps operator overloading,
			// but not here, so
			// sed s/'10.0.1.0'/'0x0A000100'/g
			// sed s/'20.2.0.7'/'0x14020007'/g
			char *vc2 = NULL;
			char *vc3 = NULL;
			if (vc1) {
				//printf ("vc : \"%s\"\n", vc );
				//printf ("vc1: \"%s\"\n", vc1);
				vc2 = replacestr (vc1, strlen (vc1), "20.2.0.7", "0x14020007");
				if (vc2) {
					//printf ("vc2: \"%s\"\n", vc2);
					vc3 = replacestr (vc2, strlen (vc2), "10.0.1.0", "0x0A000100");
					//printf ("vc3: \"%s\"\n", vc3);
					free (vc1);
					vc1 = vc3;
					free (vc2);
				}
			}
			char *tp = get_between (&buf[i], e, C_ATEMPL, EOA);
			char *uvr = get_between (&buf[i], e, C_AUVER, EOA);
			TRY (!aname, MWORD" "C_ANAME"\n", 0)
			TRY (!atype, MWORD" "C_ATYPE"\n", 0)
			fix_symbol (&aname[0], strlen (aname));
			fix_symbol (&atype[0], strlen (atype));
			if (arr1) {
				fix_symbol (&arr1[0], strlen (arr1));
			}
			if (arr2) {
				fix_symbol (&arr2[0], strlen (arr2));
			}
			dbg (" field: name=%s, type=%s\n", aname, atype);
			i += e;

			// a special fix* just for "QuatKey"
			if (!strcmp (name, "QuatKey") && !strcmp (aname, "Value")) {
				free (atype);
				atype = strcopy ("Quaternion");
			}

#if CG
			cg_struct_field (name, aname, atype, aver1, aver2, arr1, arr2,
				cond1, arg1, def, vc1, tp, uvr);
#endif /* CG */
			free (aname);// for now
			free (atype);// for now
			free (aver1);// for now
			free (aver2);// for now
			free (arr1);// for now
			//if (arr11) free (arr11);// for now
			free (arr2);// for now
			//if (arr21) free (arr21);// for now
			free (cond);// for now
			if (cond1) free (cond1);// for now
			free (arg);// for now
			if (arg1) free (arg1);// for now
			free (def);// for now
			free (vc);// for now
			if (vc1) free (vc1);// for now
			free (tp);// for now
			free (uvr);// for now
		}
	}
#if CG
	cg_struct_end (name);
#endif /* CG */
	free (name);// for now
	free (inh);// for now
	return 1;
}

// parse <niobject...</niobject>. returns true if ok, false otherwise
static int
pniobject(const char *buf, int len)
{
	nc++;
	cc--;
	dbg ("N %d, ", len);
	return pcompound (buf, len); 
}

typedef int (*parse_proc)(const char *buf, int len);

static int coverage = 0;

// tend - tag end
static int
parse(parse_proc pp, const char *tend, const char *buf, int maxlen)
{
	int e = find (tend, &buf[0], maxlen);
	if ( e > 0 ) {
		pp (&buf[0], e);
		int result = e + strlen (tend);
		coverage += result;
		return result;
	} else info ("broken %s\n", tend);
	return 0;
}

// parser main loop
static int
convert_buf(const char *in_buf, int buflen)
{
	char *clean = malloc (buflen);
	TRY (!clean, "out of memory\n", EXIT_FAILURE)
	int clen = skip_comments (in_buf, buflen, &clean);
	if (!clen) {
		free (clean);
		return EXIT_FAILURE;
	}
	info ("comments cleaned up: %d bytes\n", buflen - clen);

	// pass1 - find "NiObject" and its empty level 1 descendants
	// pass1 - find "KeyGroup" variants
	int a = 0, b = 0, ofs = 0;
	for (;;ofs += a + b) {
		a = find (NA, &clean[ofs], clen-ofs);// find "<niobject" ofs
		if (a < 0) break;
		b = find (NB, &clean[ofs+a], clen-(ofs+a));// find "</niobject>" ofs
		if (b < 0) break;
		char *name = get_between (&clean[ofs+a], b + 1, CNAME, EOA);
		if (starts_with (&name[0], strlen(name), "ns ")) {
			free (name);
			continue;// ignore non-lib. objects
		}
		char *inh = get_between (&clean[ofs+a], b + 1, CINH, EOA);
		if (inh)
			fix_symbol (&inh[0], strlen (inh));
		fix_symbol (&name[0], strlen (name));
		//printf ("%s %s\n", name, inh ? inh : "");
		int field_search = find (C_AA, &clean[ofs+a], b + 1);
		if (field_search < 0)
			if ((inh && !strcmp (inh, "NiObject")) ||
				(!strcmp (name, "NiObject"))) {
				//printf (" skipped \"%s\" - no fields\n", name);
				hashadd (emptyh, name, strcopy (""));
				free (inh);
				continue;// found an empty lvl 0 or 1 object
			}
		// KeyGroup
		// find <add with type="KeyGroup" and get its template=""
		char *add_buf = &clean[ofs+a];
		int add_buf_len = b + 1;
		int add_ofs = 0;
		int add_pos = 0, add_end = 0;
		for (;;add_ofs += add_pos + add_end) {
			add_pos = find (C_AA, &add_buf[add_ofs], add_buf_len - add_ofs);
			if (add_pos < 0) break;
			add_end = find (C_AB, &add_buf[add_ofs + add_pos],
				add_buf_len - (add_ofs + add_pos));
			if (add_end < 0) break;
			int kg = find ("type=\"KeyGroup\"",
				&add_buf[add_ofs + add_pos], add_end + 1);
			if (kg >= 0) { // found a keygroup field
				/*int gg;
				for (gg = add_ofs + add_pos; gg < add_ofs + add_pos + add_end + 1; gg++)
					printf ("%c", add_buf[gg]);
				printf ("\n");*/
				char *kgtype = get_between (
					&add_buf[add_ofs + add_pos], add_end + 1,
					"template=\"", EOA);
				if (kgtype)
					hashadd (nikeygroups, kgtype, strcopy(""));
				else// TODO: sane cleanup: memory manager
					dbg ("KeyGroup type: %s\n", kgtype ? kgtype : "bug");
			}
		}//

		free (name);
		free (inh);
	}

	// pass2 - find "Key" variants
	// Key
	// find <add with type="Key" and get its template=""
	char *add_buf = &clean[0];// compounds do include Key
	int add_buf_len = clen;
	int add_ofs = 0;
	int add_pos = 0, add_end = 0;
	for (;;add_ofs += add_pos + add_end) {
		add_pos = find (C_AA, &add_buf[add_ofs], add_buf_len - add_ofs);
		if (add_pos < 0) break;
		add_end = find (C_AB, &add_buf[add_ofs + add_pos],
			add_buf_len - (add_ofs + add_pos));
		if (add_end < 0) break;
		int kg = find ("type=\"Key\"",
			&add_buf[add_ofs + add_pos], add_end + 1);
		if (kg >= 0) { // found a keygroup field
			/*int gg;
			for (gg = add_ofs + add_pos; gg < add_ofs + add_pos + add_end + 1; gg++)
				printf ("%c", add_buf[gg]);
			printf ("\n");*/
			char *ktype = get_between (
				&add_buf[add_ofs + add_pos], add_end + 1,
				"template=\"", EOA);
			if (!strcmp (ktype, "TEMPLATE"))
				continue;// skip it
			if (ktype) {
				hashadd (nikeys, ktype, strcopy(""));
				//printf ("%s\n", ktype);
			}
			else// TODO: sane cleanup: memory manager
				dbg ("Key type: %s\n", ktype ? ktype : "bug");
		}
	}//
	// now nikeygroups requires types not present in nikeys
	// so adjust that:
	{
	int i, j;
	for (i = 0; i < nikeygroups->cnt; i++)// TODO: make hash_walk ot something
		if (nikeygroups->items[i])
			for (j = 0; j < nikeygroups->items[i]->cnt; j++) {
				char *k = nikeygroups->items[i]->items[j].key;
				if (!hashget (nikeys, k))
					hashadd (nikeys, strcopy(k), strcopy(""));
			}
	}

	/*FILE *tmp = fopen("intermediate.xml", "a");
	fwrite (&clean[0], clen, 1, tmp);
	fclose (tmp);*/
	coverage += buflen - clen; // xml comments worked out
#if CG
	cg_prologue ();
#endif /* CG */
	int i;
	for (i = 0; i < clen; i++) {
		int len = clen - i;
		if (starts_with (&clean[i], len, BA))
			i += parse (pbasic, BB, &clean[i], len);
		else if (starts_with (&clean[i], len, EA))
			i += parse (penum, EB, &clean[i], len);
		else if (starts_with (&clean[i], len, FA))
			i += parse (pbitflags, FB, &clean[i], len);
		else if (starts_with (&clean[i], len, CA))
			i += parse (pcompound, CB, &clean[i], len);
		else if (starts_with (&clean[i], len, NA))
			i += parse (pniobject, NB, &clean[i], len);
	}
#if CG
	cg_epilogue ();
#endif /* CG */
	info ("coverage: %d/%d bytes (%d%%)\n", coverage, buflen,
		(int)((((float)coverage/buflen)*100.0f)+0.5f) );
	info ("parsed: basic:%d, enum:%d, bitflags:%d, compound:%d, niobject:%d\n",
		bc, ec, fc, cc, nc);
	free (clean);
	return EXIT_SUCCESS;
}

			/*
   		---/   codegen   / /---
					----/*/
#if CG
#define PRINTBASIC 1
#define PRINTENUMS 1

#define SCREATE "malloc"
#define VCHK "VersionCheck"
#define VCHKCALL "VersionCheck ("
#define USER_VERSION "UserVersion"
#define USER_VERSION2 "UserVersion2"

static int
cg_prologue()
{
	printf ("#include <stdlib.h>\n");
	printf ("#include <string.h>\n");
	printf ("#include <stdarg.h>\n");
	printf ("#include <stdio.h>\n");
	//printf ("#include \"nif_reader2.h\"\n");
	printf ("#include \"utils.h\"\n");
	printf ("#include \"niff.h\"\n\n");

	printf ("#define COVERAGEIN\n");
	printf ("#define COVERAGEOUT\n\n");

	printf ("static inline int\n");
	printf ("dbg(const char *format, ...)\n");
	printf ("{\n");
	printf ("	return 0;\n");
	printf ("	va_list arg;\n");
	printf ("	int done;\n");
	printf ("	va_start (arg, format);\n");
	printf ("	done = vprintf (format, arg);\n");
	printf ("	va_end (arg);\n");
	printf ("	return done;\n");
	printf ("}\n\n");

	printf ("static NifStream *R;\n");
	printf ("static Stream *S;\n");
	printf ("static int ERR;\n");
	printf ("static int STOP;\n\n");

	printf ("#define GETMEM(SIZE) malloc (SIZE)\n\n");
	printf ("#define READERARGLST register NifStream *r\n");
	printf ("#define READER R\n\n");

	printf ("#define ENONE 0\n");
	printf ("#define EM 1\n");
	printf ("#define EIO 2\n");
	printf ("#define ENIF 3\n");
	printf ("#define EBUG 4\n\n");

	printf ("// Nif Parsing Failed\n");
	printf ("static void\n");
	printf ("NPF(const char *msg, int ecode)\n");
	printf ("{\n");
	printf ("	dbg (\"*** %%s\\n\", msg);\n");
	printf ("	ERR = ecode;\n");
	printf ("	STOP = 1;\n");
	printf ("}\n\n");

	printf ("static void\n");
	printf ("NPFF(int ecode, const char *msg, ...)\n");
	printf ("{\n");
	printf ("	va_list arg;\n");
	printf ("	int done;\n");
	printf ("	va_start (arg, msg);\n");
	printf ("	done = vprintf (msg, arg);\n");
	printf ("	va_end (arg);\n\n");

	printf ("	ERR = ecode;\n");
	printf ("	STOP = 1;\n");
	printf ("}\n\n");

	printf ("static unsigned int Version;\n");
	printf ("static unsigned int "USER_VERSION";\n");
	printf ("static unsigned int "USER_VERSION2";\n");
	printf ("static int\n");
	printf (VCHK" (unsigned int a, unsigned int b)\n");
	printf ("{\n");
	printf ("\tif (a == 0 && b == 0) return 1;\n");
	printf ("\tif (a != 0 && b == 0) return Version >= a;\n");
	printf ("\tif (a == 0 && b != 0) return Version <= b;\n");
	printf ("\treturn Version >= a && Version <= b;\n");
	printf ("}\n\n");
	return 1;
}

static void
cg_nif_parser_macros()
{
printf ("#define RANGE_CHK(LEN, MAX, PROC)\\\n\n");
/*printf ("	if ((LEN) > (MAX)) {\\\n");
printf ("		NPFF (ENIF,	"*** "PROC" invalid length read:%d\n", LEN);\\\n");
printf ("		return;\\\n");
printf ("	}\n\n");*/

printf ("#define READ(TYPE, BUF, LEN, BYTES, PROC)						\\\n");
printf ("	{															\\\n");
printf ("		int result = R->read_##TYPE (S, BUF, LEN);				\\\n");
printf ("		if (result != (LEN * BYTES))							\\\n");
printf ("			NPFF (EIO, PROC\": EOF reached, len:%%d \\n\", LEN);\\\n");
printf ("	}\n\n");

	// a macro for reading 1d complex data arrays
printf ("#define READ1DC(LEN, RPROC, FEILD, ARGS)\\\n");
printf ("	{									\\\n");
printf ("		int i;							\\\n");
printf ("		for (i = 0; i < (LEN); i++) {	\\\n");
printf ("			RPROC (&(FEILD[i]), ARGS);	\\\n");
printf ("		}								\\\n");
printf ("	}\n\n");

}

#define BTMAP_LEN 17
static const char const *btmap[] = {
	"bool",				"NIFbyte",
	"byte",				"NIFbyte",
	"uint",				"NIFuint",
	"ushort",			"NIFushort",
	"int",				"NIFint",
	"short",			"NIFshort",
	"BlockTypeIndex",	"NIFushort",
	"char",				"NIFchar",
	"FileVersion",		"NIFuint",
	"Flags",			"NIFushort",
	"float",			"NIFfloat",
	"HeaderString",		"NIFchar *",
	"LineString",		"NIFchar *",
	"Ptr",				"NIFuint",
	"Ref",				"NIFuint",
	"StringOffset",		"NIFuint",
	"StringIndex",		"NIFuint"
};

// turns basic type from the xml to a c type, null if unknown
static const char *
bt2ct(const char *bt)
{
	int i;
	for (i = 0; i < 2*BTMAP_LEN; i+=2)
		if (!strcmp (bt, btmap[i]))
			return &btmap[i+1][0];
	return NULL;
}

// *target = *target + a
static void
append_to(char **target, const char *a)
{
	char *dst = *target;
	if (!dst)
		*target = concat ("", a);
	else {
		char *tmp = concat (dst, a);
		free (dst);
		*target = tmp;
	}
}

static char *bdec = NULL;// struct %s;
static char *bdef = NULL;// struct %s;

static int
cg_basic (const char *name, const char *type)
{
	// "type" from the xml is ignored altogether - it is in "btmap"
	// decl.: static inline NIFbyte readbool(READERARGLST);
	// def.: static inline NIFbyte\nreadbool(READERARGLST)\n{\n\t//code_me\n}\n
	append_to (&bdec, "static inline ");
	append_to (&bdec, bt2ct (name));
	append_to (&bdec, " read");
	append_to (&bdec, name);
	append_to (&bdec, "(READERARGLST);\n");
	append_to (&bdef, "static inline ");
	append_to (&bdef, bt2ct (name));
	append_to (&bdef, "\nread");
	append_to (&bdef, name);
	append_to (&bdef, "(READERARGLST)\n{\n\t//code_me\n}\n\n");

	// add to types hash
	char *tkey = strcopy (name);
	char *tval = strcopy (bt2ct (name));
	int t = hashadd (th, tkey, tval);
	if (t == -1) {// key exists, nothing was added
		free (tkey);
		free (tval);
	}

	// read call
	char *a = concat ("read", name);
	char *readcall = concat (a, " (READER);\n");
	free (a);
	char *key = strcopy (name);
	int r = hashadd (rch, key, readcall);
	if (r == -1)
		free (key);
	return r;
}

static char *enums = NULL;

static int
cg_enum_begin (const char *name, const char *type)
{
	char *tkey = strcopy (name);
	char *tval = strcopy (bt2ct (type));

	// def.
	append_to (&enums, "enum ");
	append_to (&enums, tkey);
	append_to (&enums, " { // ");
	append_to (&enums, tval);
	append_to (&enums, "\n");

	// type hash
	int t = hashadd (th, tkey, tval);
	if (t == -1) {
		free (tkey);
		free (tval);
	}

	// readcall
	char *key = strcopy (name);
	char *rproccall = hashget (rch, (char *)type);
	if (!rproccall) {
		cgdbg ("Unknown reader for type \"%s\"\n", type);
		free (key);
		return 0;
	}
	char *val = strcopy (rproccall);
	int r = hashadd (rch, key, val);
	if (r == -1) {
		free (key);
		free (val);
		free (rproccall);
	}
	return r;
}

static int
cg_enum_optn (const char *name, const char *value)
{
	append_to (&enums, "\t");
	append_to (&enums, name);
	append_to (&enums, " = ");
	append_to (&enums, value);
	append_to (&enums, ",\n");
	return 1;
}

static int
cg_enum_end ()
{
	append_to (&enums, "};\n\n");
	return 1;
}

static char *tfdefs = NULL;// struct %s;
static char *tdefs = NULL; // tyepdef struct {} %s;
static char *srcalls = NULL; // read this, read that - declarations
static char *srcode = NULL; // read this, read that - definitions

static char *
v2u(const char *version)
{
	if (!version)
		return strcopy ("0");
	cgdbg ("DBG: tokenizing %s\n", version);
	char *t = strcopy (version);
	char *tokens[4] = {"0", "0", "0", "0"};
	int len = strlen(t);
	int i, p = 0, k = 0;
	for (i = 0; i < len+1; i++) {
		//printf ("DBG: %c\n", t[i]);
		if (t[i] == '.' || t[i] == '\0') {
			if (k == 4)
				return strcopy ("0");
			t[i] = '\0';
			tokens[k++] = &t[p];
			//printf ("DBG: i:%d, k:%d, t:%s\n", i, k, &t[p]);
			p = i + 1;
		}
	}
	unsigned int v = 0;
	for (k = 0; k < 4; k++) {
		//printf ("DBG: token[%d] = %s\n", k, tokens[k]);
		unsigned int f = (unsigned int)atoi (tokens[k]);
		//printf ("DBG: token[%d] = as digit %d\n", k, f);
		v |= f << ((3-k)*8);
	}
	free (t);
	char buf[2+8+1];
	sprintf (&buf[0], "0x%.8X", v);
	buf[10] = '\0';
	return strcopy (&buf[0]);
}

static hash *fieldsh = NULL;// TODO: err reporting

static int
cg_struct_begin(const char *name, const char *inh)
{
	/*typedef struct {
		NIFfloat Time;
		ByteColor4 Value;
		ByteColor4 Forward;
		ByteColor4 Backward;
		TBC TBC;
	} NiByteColor4Key;*/
	if (!strcmp (name, "Key")) {// special handling is required
		int i, j;
		for (i = 0; i < nikeys->cnt; i++) {
			if (nikeys->items[i]) {
				for (j = 0; j < nikeys->items[i]->cnt; j++) {
					char *k = nikeys->items[i]->items[j].key;
					char *fk = NULL;
					append_to (&fk, "Ni");
					append_to (&fk, k);
					append_to (&fk, "Key");
					// TODO: think about a way tho read this as a template
					// directly from the xml instead of hard-coding it
					cg_struct_begin (fk, "NiObject");
				/*const char *name, const char *type,
				const char *v1, const char *v2, const char *a1, const char *a2,
				const char *cond, const char *arg,	const char *def,
				const char *vcond, const char *templ, const char *uv*/
					cg_struct_field (fk, "Time", "float",
						NULL, NULL, NULL, NULL,
						NULL, NULL, NULL,
						NULL, NULL, NULL);
					cg_struct_field (fk, "Value", k,
						NULL, NULL, NULL, NULL,
						NULL, NULL, NULL,
						NULL, NULL, NULL);
					cg_struct_field (fk, "Forward", k,
						NULL, NULL, NULL, NULL,
						"ARG == 2", NULL, NULL,
						NULL, NULL, NULL);
					cg_struct_field (fk, "Backward", k,
						NULL, NULL, NULL, NULL,
						"ARG == 2", NULL, NULL,
						NULL, NULL, NULL);
					cg_struct_field (fk, "TBC", "TBC",
						NULL, NULL, NULL, NULL,
						"ARG == 3", NULL, NULL,
						NULL, NULL, NULL);
					cg_struct_end (fk);
					free (fk);
				}
			}
		}
		return 1;
	}

	/*typedef struct {
		NIFuint NumKeys;
		NIFuint Interpolation;
		// [NumKeys], [(null)]
		Key *Keys;
	} KeyGroup;*/
	if (!strcmp (name, "KeyGroup")) {// special handling is required
		int i, j;
		for (i = 0; i < nikeygroups->cnt; i++) {
			if (nikeygroups->items[i]) {
				for (j = 0; j < nikeygroups->items[i]->cnt; j++) {
					char *k = nikeygroups->items[i]->items[j].key;
					char *fk = NULL;
					append_to (&fk, "Ni");
					append_to (&fk, k);
					append_to (&fk, "KeyGroup");
					// TODO: think about a way tho read this as a template
					// directly from the xml instead of hard-coding it
					cg_struct_begin (fk, "NiObject");
				/*const char *name, const char *type,
				const char *v1, const char *v2, const char *a1, const char *a2,
				const char *cond, const char *arg,	const char *def,
				const char *vcond, const char *templ, const char *uv*/
					cg_struct_field (fk, "NumKeys", "uint",
						NULL, NULL, NULL, NULL,
						NULL, NULL, NULL,
						NULL, NULL, NULL);
					cg_struct_field (fk, "Interpolation", "KeyType",
						NULL, NULL, NULL, NULL,
						"r->NumKeys != 0", NULL, NULL,
						NULL, NULL, NULL);
					char *fk2 = NULL;
					append_to (&fk2, "Ni");
					append_to (&fk2, k);
					append_to (&fk2, "Key");
					cg_struct_field (fk, "Keys", fk2,
						NULL, NULL, "NumKeys", NULL,
						"r->NumKeys != 0", "r->Interpolation", NULL,
						NULL, NULL, NULL);
					cg_struct_end (fk);
					free (fk);
					free (fk2);
				}
			}
		}
		return 1;
	}

	// DONE: "emptyh" may fail on invalid obj. order in the xml, so
	// it is time for a two-pass parser perhaps
	if (!fieldsh)
		fieldsh = hashcreate (20);
	// typedef - forward
	/*append_to (&tfdefs, "struct "); DISABLED for now
	append_to (&tfdefs, name);
	append_to (&tfdefs, ";\n");*/
	// typedef
	append_to (&tdefs, "typedef struct {\n");
	if (inh && !hashget (emptyh, (char *)inh)) {
		append_to (&tdefs, "\t");
		append_to (&tdefs, inh);
		append_to (&tdefs, " *parent;\n");
	}
	// ---------------------------------------- ok

	// readcall
	//  Don't register it with "rch", use "rch" for simple types only -
	//  "basic" and "enum". The field reader is able to buid the call.
	// readcall def.
	// void read%s(%s *r, unsigned int ARG);
	append_to (&srcalls, "void read");// decl.
	append_to (&srcalls, name);
	append_to (&srcalls, "(");
	append_to (&srcalls, name);
	append_to (&srcalls, " *r, unsigned int ARG);\n");
	// readcode
	append_to (&srcode, "void\nread");// def.
	append_to (&srcode, name);
	append_to (&srcode, "(");
	append_to (&srcode, name);
	append_to (&srcode, " *r, unsigned int ARG)\n");
	append_to (&srcode, "{\n\tCOVERAGEIN\n");
	// readcode read parent call
	// VERCHK(v2u(v1), v2u(v2)
	if (inh && !hashget (emptyh, (char *)inh)) {// init parent
		append_to (&srcode, "\tr->parent = "SCREATE" (sizeof(");
		append_to (&srcode, inh);
		append_to (&srcode, "));\n");
		// read call
		append_to (&srcode, "\tread");
		append_to (&srcode, inh);
		append_to (&srcode, " (r->parent, 0);\n");
	}
	// ---------------------------------------- ok
	return 1;
}

static char *major_vcheck;// help var for "group by"
static char *other_vcheck;// help var for "group by"

static int
cg_struct_field(const char *structname,
				const char *name, const char *type, const char *v1,
				const char *v2, const char *a1, const char *a2,
				const char *cond, const char *arg,	const char *def,
				const char *vcond, const char *templ,
				const char *uv)
{
	if (!strcmp (structname, "Key"))// its is specially-handled
		return 1;
	else if (!strcmp (structname, "KeyGroup"))// sit pecially-handled
		return 1;

	char *btype = hashget (th, (char *)type);
	char *ktype = NULL;

	if (!strcmp (type, "Key") && templ) {// special handling is required
		// TODO: getKeyName(...) perhaps?
		append_to (&ktype, "Ni");
		append_to (&ktype, templ);
		append_to (&ktype, "Key");
	}
	else if (!strcmp (type, "KeyGroup")) {// special handling is required
		// TODO: getKeyGroupName(...) perhaps?
		append_to (&ktype, "Ni");
		append_to (&ktype, templ);
		append_to (&ktype, "KeyGroup");
	}

	// if a symbol (field name) is already there it should not be added,
	// but it should be read
	//
	//	It is a call for simple types and sequental fileds based/or not on a
	// simple types - btype != null
	//
	// "simple type" should not be a pointer type:
	//   int *p for example works because 'int' is a simple type
	//   char *p[3] doesn't because 'char *' is a pointer type
	//
	// 1.arrays of simple type ...
	//	uint * for example
	// 2.seq. fileds of simple type ...
	//  uint * f1;
	//  uint * f2;
	//  uint * f3; for example
	// 3.arrays of types composed of seq. fileds of simple type ...
	//  Vector4 * for eaxmple
	// 4.seq. fileds of types composed of seq. fileds of simple type ...
	//  Vector4 *f1;
	//  Vector4 *f1;
	//  Vector4 *f3; foreaxmple
	//
	// RANGE_CHK(LEN, MAX, PROC) - this will be hard - different MAX
	// READ(TYPE, BUF, LEN, BYTES, PROC)

	// typedef struct {} ;
	if (fieldsh && !hashget (fieldsh, (char *)name)) {
		hashadd (fieldsh, strcopy (name), strcopy (
				(!a1 && !a2 ? "" : "array field")
			));
		char *field_def = NULL;
		// add a comment line that tells about array dimentions
		if (a1 || a2) {// DBG
			append_to (&field_def, "\t// [");
			append_to (&field_def, a1 ? a1 : "(null)");
			append_to (&field_def, "], [");
			append_to (&field_def, a2 ? a2 : "(null)");
			append_to (&field_def, "]\n");
		}
		// begin the field
		append_to (&field_def, "\t");
		append_to (&field_def, btype ? btype : (ktype ? ktype : type));
		append_to (&field_def, " ");
		// ---------------------------------------- ok
		// finish it
		if (a1 && a2) {// 2d array
			int p1 = a1[0] < '0' || a1[0] > '9';
			int p2 = a2[0] < '0' || a2[0] > '9';
			if (!p1 && !p2) {// const size
				append_to (&field_def, name);
				append_to (&field_def, "[");
				append_to (&field_def, a1);
				append_to (&field_def, "*");
				append_to (&field_def, a2);
				append_to (&field_def, "]");
			} else {// dynamic size
				// jagged detect
				char *arrf1 = hashget (fieldsh, (char *)a1);
				char *arrf2 = hashget (fieldsh, (char *)a2);
				if ((arrf1 && !strcmp (arrf1, "array field")) ||
					(arrf2 && !strcmp (arrf2, "array field")))
					append_to (&field_def, "/*jagged*/ **");
				else // not jagged
					append_to (&field_def, "*");
				append_to (&field_def, name);
			}// 2d dynamic size
		}// if (a1 && a2)
		else if (a1 || a2) { //1d array
			const char *dim = a1 ? a1 : a2;
			int p = dim[0] < '0' || dim[0] > '9';
			if (!p) {// const size
				append_to (&field_def, name);
				append_to (&field_def, "[");
				append_to (&field_def, dim);
				append_to (&field_def, "]");
			} else {// dynamic size
				if (!hashget (fieldsh, (char *)dim))
					append_to (&field_def, "/*parent-> or expr.*/");
				//if (p && !cha1 && !cha2 && strcmp (dim, "ARG"))
				//	append_to (&field_def, "/*parent-> or expr.*/");
				append_to (&field_def, "*");
				append_to (&field_def, name);
			}
		} else // not an array field
			append_to (&field_def, name);
		append_to (&field_def, ";\n");
		append_to (&tdefs, field_def);
		free (field_def);
	}// if (fieldsh && !hashget (fieldsh, (char *)name))
	// ---------------------------------------- ok

	//
	// codegen state vars
	//
	int _d = 0; // num of dimensions, 0 means not an array
	int _j = 0; // true when jagged
	//char *_s = NULL; // array size
	//char *_is = NULL;
	char *_l1 = NULL; // array len. 1
	char *_l2 = NULL; // array len. 2
	char *_l = NULL; // array len for 1d
	int _c = 0; // not const array

	if (a1 && a2) {// 2d array
		_d = 2;
		int p1 = a1[0] < '0' || a1[0] > '9';
		int p2 = a2[0] < '0' || a2[0] > '9';
		if (!p1 && !p2) {// const size
			_d = 1;
			_c = 1;
			append_to (&_l, "(");
			append_to (&_l, a1);
			append_to (&_l, "*");
			append_to (&_l, a2);
			append_to (&_l, ")");
		} else {// dynamic size
			// jagged detect
			char *arrf1 = hashget (fieldsh, (char *)a1);
			char *arrf2 = hashget (fieldsh, (char *)a2);
			if ((arrf1 && !strcmp (arrf1, "array field")) ||
				(arrf2 && !strcmp (arrf2, "array field"))) {
				_j = 1;
				if (arrf1) {
					append_to (&_l2, "(");
					append_to (&_l2, a2);
					append_to (&_l2, "[i])");
					_l1 = strcopy (a1);
				} else if (arrf1) {
					append_to (&_l1, "(");
					append_to (&_l1, a1);
					append_to (&_l1, "[j])");
					_l2 = strcopy (a2);
				} else { /* TODO: both are jagged ? */ }
			}
			else {// dynamic, not jagged
				_d = 1;
				append_to (&_l, "(");
				append_to (&_l, a1);
				append_to (&_l, ")*");
				append_to (&_l, "(");
				append_to (&_l, a2);
				append_to (&_l, ")");
			}
		}// 2d dynamic size
	}// if (a1 && a2)
	else if (a1 || a2) { //1d array
		_d = 1;
		const char *dim = a1 ? a1 : a2;
		int p = dim[0] < '0' || dim[0] > '9';
		if (!p) {// const size
				_c = 1;
				append_to (&_l, "(");
				append_to (&_l, dim);
				append_to (&_l, ")");
			} else {// dynamic size
				/*if (strcmp (dim, "ARG")) // avoid adding r-> infront of ARG
					append_to (&_l, "(r->");
				else */append_to (&_l, "(");
				append_to (&_l, dim);
				append_to (&_l, ")");
			}
		}

	//
	// code - fields reader
	//
	// the read call
	char *field = concat ("r->", name);
	char *type_rc = hashget (rch, (char *)type);

	// version and other checks
	char *otherchk = NULL;
	char *current_major_vcheck = NULL;
	if (v1 || v2) {
		char *ver1 = v2u (v1);
		char *ver2 = v2u (v2);
		append_to (&current_major_vcheck, VCHKCALL);
		append_to (&current_major_vcheck, ver1);
		append_to (&current_major_vcheck, ", ");
		append_to (&current_major_vcheck, ver2);
		append_to (&current_major_vcheck, ")");
		free (ver1);
		free (ver2);
	}
	int prev_cond = 0;
	if (uv) {// add userver in the condition
		append_to (&otherchk, "(");
		append_to (&otherchk, USER_VERSION" == ");
		append_to (&otherchk, uv);
		append_to (&otherchk, ")");
		prev_cond++;
	}
	if (vcond) {
		if (prev_cond)
			append_to (&otherchk, " && ");
		append_to (&otherchk, "(");
		append_to (&otherchk, vcond);
		append_to (&otherchk, ")");
		prev_cond++;
	}
	if (cond) {
		if (prev_cond)
			append_to (&otherchk, " && ");
		append_to (&otherchk, "(");
		append_to (&otherchk, cond);
		append_to (&otherchk, ")");
		prev_cond++;
	}
	
	// non-array reads
	char *rc = NULL;// read code - the code that reads the data into the structure

	// the version check can stop only if it was started
	// major version check "group by" stop
	if (major_vcheck)
		if (!current_major_vcheck ||
			strcmp (current_major_vcheck, major_vcheck)) {
			// also stop internal grouping
			if (other_vcheck) {
				free (other_vcheck);
				other_vcheck = NULL;
				append_to (&rc, "\t");
				append_to (&rc, "\t}\n");
			}

			free (major_vcheck);
			major_vcheck = NULL;
			append_to (&rc, "\t}\n");
		}
	// major version check "group by" start
	if (current_major_vcheck)// there is a version check
		if (!major_vcheck ||
			strcmp (current_major_vcheck, major_vcheck)) {
			// last one is empty || current is not the same, so start new
			append_to (&rc, "\tif (");
			append_to (&rc, current_major_vcheck);
			append_to (&rc, ") {\n");
			if (major_vcheck) {
				free (major_vcheck);
				major_vcheck = NULL;
			}
			append_to (&major_vcheck, current_major_vcheck);
		}

	// the version check can stop only if it was started
	// other version/ check "group by" stop
	if (other_vcheck)
		if (!otherchk ||
			strcmp (otherchk, other_vcheck)) {
			free (other_vcheck);
			other_vcheck = NULL;
			if (major_vcheck)
				append_to (&rc, "\t");
			append_to (&rc, "\t}\n");
		}
	// other version/ check "group by" start
	if (otherchk)// there is a version check
		if (!other_vcheck ||
			strcmp (otherchk, other_vcheck)) {
			// last one is empty || current is not the same, so start new
			if (major_vcheck)
				append_to (&rc, "\t");
			append_to (&rc, "\tif (");
			append_to (&rc, otherchk);
			append_to (&rc, ") {\n");
			if (other_vcheck) {
				free (other_vcheck);
				other_vcheck = NULL;
			}
			append_to (&other_vcheck, otherchk);
		}

	/*if (otherchk) {
		if (current_major_vcheck)
			append_to (&rc, "\t");
		append_to (&rc, "\tif (");
		append_to (&rc, otherchk);
		append_to (&rc, ") {\n");
	}*/

	if (_d == 0) {
		if (btype && type_rc) { // basic type
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t");
			append_to (&rc, field);
			append_to (&rc, " = ");
			append_to (&rc, type_rc);
		} else { // complex type
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\tread");
			append_to (&rc, ktype ? ktype : type);
			// special handling - just for "UnionBV" 
			if (!strcmp (type, "UnionBV"))
				append_to (&rc, " ((UnionBV *)(&(");
			else append_to (&rc, " (&(");
			append_to (&rc, field);
			// special handling - just for "UnionBV" 
			if (!strcmp (type, "UnionBV"))
				append_to (&rc, ")), ");
			else append_to (&rc, "), ");
			append_to (&rc, arg ? arg : "ARG");
			append_to (&rc, ");\n");
		}
	} else if (_d == 1) {// 1d arrays
		//printf ("before: %s\n", _l);
		char *_la = fix_cond (_l);
		free(_l);
		_l = _la;
		//printf ("after: %s\n", _l);
		if (btype && type_rc &&
			btype[strlen (btype)-1] == '*') { // 1d, basic char * type
			// NIFchar *buf[3] requires special handling
			// it is a basic type, 1d array and NifStream doesn't have
			// char * reader:
			// for (int i = 0; i < _l; i++)
			//	field[i] = type_rc
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t{ int i; for (i = 0; i < ");
			append_to (&rc, _l);
			append_to (&rc, "; i++)\n");
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t");
			append_to (&rc, field);
			append_to (&rc, "[i] = ");
			append_to (&rc, type_rc);
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t}\n");
		}
		else if (btype && type_rc) { // 1d, basic type

			// not all languages do have classes and inheritance
			// for "NiPSysData"  _l = r->parent->parent->NumVertices)
			// for "NiLinesData" _l = r->parent->NumVertices
			if (!strcmp (structname, "NiPSysData")) {
				free (_l);
				// TODO: do something about that "r->" - it is in too many
				// places already
				_l = strcopy ("r->parent->parent->NumVertices");
			}
			else if (!strcmp (structname, "NiLinesData")) {
				free (_l);
				// TODO: do something about that "r->" - it is in too many
				// places already
				_l = strcopy ("r->parent->NumVertices");
			}

			if (!_c) {// the array is not const sized, so GETMEM
				// make it '\0' terminated if char
				if (current_major_vcheck)// formatting
					append_to (&rc, "\t");
				if (otherchk)// formatting
					append_to (&rc, "\t");
				append_to (&rc, "\t");
				append_to (&rc, field);
				append_to (&rc, " = GETMEM (");
				if (!strcmp (type, "char"))
					append_to (&rc, "(");
				append_to (&rc, _l);
				if (!strcmp (type, "char"))// TODO: to many same-checks
					append_to (&rc, " + 1)");
				append_to (&rc, "*sizeof(");
				append_to (&rc, btype);
				append_to (&rc, ")");
				append_to (&rc, ");\n");
				if (!strcmp (type, "char")) {
					// zero - terminate it
					if (current_major_vcheck)// formatting
						append_to (&rc, "\t");
					if (otherchk)// formatting
						append_to (&rc, "\t");
					append_to (&rc, "\t");
					append_to (&rc, field);
					append_to (&rc, "[");
					append_to (&rc, _l);
					append_to (&rc, "] = '\\0';\n");
				}
				// TODO: think of a way to auto-add RANGE_CHK (_l, MAX, PROC)
				// it is not that easy because: 1.one should check the type of
				// "_l"; 2.for bitmap 4 example max ranges are quite lower than
				// other things where valid range is over 1000000 foreaxmple.
			}// end of dynamic size 1d array initialize
			// simple types 1d array blockread:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t");
			//#define READ(TYPE, BUF, LEN, BYTES, PROC)
			append_to (&rc, "READ (");
			append_to (&rc, &btype[3]);// TODO: make it nicer
			append_to (&rc, ", ");
			append_to (&rc, field);
			append_to (&rc, ", ");
			append_to (&rc, _l);
			append_to (&rc, ", sizeof(");
			append_to (&rc, btype);
			append_to (&rc, "), \"read");
			append_to (&rc, structname);
			append_to (&rc, "\")\n");
		} else { // 1d, complex type

			// not all languages do have classes and inheritance
			// NiRotatingParticlesData - _l = r->parent->NumVertices
			// NiPSysData              - _l = r->parent->parent->NumVertices
			// NiTriShapeData          - _l = r->parent->NumTriangles
			if (!strcmp (structname, "NiRotatingParticlesData")) {
				free (_l);
				_l = strcopy ("r->parent->NumVertices");
			}
			else if (!strcmp (structname, "NiPSysData")) {
				free (_l);
				_l = strcopy ("r->parent->parent->NumVertices");
			}
			else if (!strcmp (structname, "NiTriShapeData")) {
				free (_l);
				_l = strcopy ("r->parent->NumTriangles");
			}

			if (!_c) {// the array is not const sized, so GETMEM
				// make it '\0' terminated if char
				if (current_major_vcheck)// formatting
					append_to (&rc, "\t");
				if (otherchk)// formatting
					append_to (&rc, "\t");
				append_to (&rc, "\t");
				append_to (&rc, field);
				append_to (&rc, " = GETMEM (");
				append_to (&rc, _l);
				append_to (&rc, "*sizeof(");
				append_to (&rc, ktype ? ktype : type);// keytype priority
				append_to (&rc, ")");
				append_to (&rc, ");\n");
			}
			// TODO: READ ( &btype[3], &(field[0].FirstField), _l*FieldNum, )
			// complex types 1d array for-loop read
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			// #define READ1DC(LEN, RPROC, FEILD, ARGS)
			append_to (&rc, "\tREAD1DC (");
			append_to (&rc, _l);
			append_to (&rc, ", ");
			append_to (&rc, "read");
			append_to (&rc, ktype ? ktype : type);
			append_to (&rc, ", ");
			append_to (&rc, field);
			append_to (&rc, ", ");
			append_to (&rc, arg ? arg : "ARG");
			append_to (&rc, ")\n");
		}
	} else if (_d == 2) {// 2d arrays

		// prepare arr. dimensions:
		char *_la = fix_cond (_l1);
		free(_l1);
		_l1 = _la;
		_la = fix_cond (_l2);
		free(_l2);
		_l2 = _la;
		// only jagged arrays come here, everything else is 1d
		/*printf ("_j:%d, _l1:%s, _l2:%s, _l:%s, struct:%s, f:%s %s\n",
			_j, _l1 ? _l1 : "-", _l2 ? _l2 : "-", _l ? _l : "-",
			 structname, type, name);*/
		if (btype && type_rc) { // 2d, basic type
			//printf ("foo\n");
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			// getmem array_sizes array:
			append_to (&rc, "\t");
			append_to (&rc, field);
			append_to (&rc, " = GETMEM (");
			append_to (&rc, _l1);
			append_to (&rc, "*sizeof(");
			append_to (&rc, btype);// keytype priority
			append_to (&rc, "*)");// pointer .a.k.a. array
			append_to (&rc, ");\n");
			// begin the for loop:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t{int i; for (i = 0; i <");
			append_to (&rc, _l1);
			append_to (&rc, "; i++) {\n");
			// inside the loop line 1: get mem for nth array
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t");
			append_to (&rc, field);
			append_to (&rc, "[i] = GETMEM (");
			append_to (&rc, _l2);
			append_to (&rc, "*sizeof(");
			append_to (&rc, btype);// keytype priority
			append_to (&rc, ")");// not pointer a.k.a. the elemnt
			append_to (&rc, ");\n");
			// inside the loop line 2: read the nth array:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t");
			append_to (&rc, "READ (");
			append_to (&rc, &btype[3]);// TODO: make it nicer
			append_to (&rc, ", ");
			append_to (&rc, field);
			append_to (&rc, "[i], ");
			append_to (&rc, _l2);
			append_to (&rc, ", sizeof(");
			append_to (&rc, btype);
			append_to (&rc, "), \"read");
			append_to (&rc, structname);
			append_to (&rc, "\")\n");
			// close the for loop:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t}}\n");
		} else {// 2d, complex - its just one actually - "NiTriShapeSkinController"
			//printf ("foo\n");
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			// getmem array_sizes array:
			append_to (&rc, "\t");
			append_to (&rc, field);
			append_to (&rc, " = GETMEM (");
			append_to (&rc, _l1);
			append_to (&rc, "*sizeof(");
			append_to (&rc, ktype ? ktype : type);// keytype priority
			append_to (&rc, "*)");// pointer .a.k.a. array
			append_to (&rc, ");\n");
			// begin the for loop:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t{int i; for (i = 0; i <");
			append_to (&rc, _l1);
			append_to (&rc, "; i++) {\n");
			// inside the loop line 1: get mem for nth array
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t");
			append_to (&rc, field);
			append_to (&rc, "[i] = GETMEM (");
			append_to (&rc, _l2);
			append_to (&rc, "*sizeof(");
			append_to (&rc, ktype ? ktype : type);// keytype priority
			append_to (&rc, ")");// not pointer a.k.a. the elemnt
			append_to (&rc, ");\n");
			// inside the loop line 2: read the nth array:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t{int j; for (j = 0; j <");
			append_to (&rc, _l2);
			append_to (&rc, "; j++)\n");
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t\tread");
			append_to (&rc, ktype ? ktype : type);// keytype priority
			append_to (&rc, " (&(");
			append_to (&rc, field);
			append_to (&rc, "[i][j]), ");
			append_to (&rc, arg ? arg : "ARG");
			append_to (&rc, ");\n");
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t\t}\n");// end of "for j"
			// close the for loop:
			if (current_major_vcheck)// formatting
				append_to (&rc, "\t");
			if (otherchk)// formatting
				append_to (&rc, "\t");
			append_to (&rc, "\t}}\n");
		}
	}

	/*if (otherchk) {
		if (current_major_vcheck)
			append_to (&rc, "\t");
		append_to (&rc, "\t}\n");
	}*/

	// cleanup
	free (field);
	if (rc) {
		append_to (&srcode, rc);
		free (rc);
	}
	if (current_major_vcheck) free (current_major_vcheck);
	if (otherchk) free (otherchk);
	return 1;

	//char *rc = NULL;
	// simple type \t$FIELD = read$TYPE (READER);\n
		/* array handling
		 - if its 2d
		   int i;
		   $FIELD = SCREATE ((d1)*sizeof($type*));
		   for (i = 0; i < (d1); i++)
				$FIELD[i] = SCREATE ((d2)*sizeof($type_rc))
		 - if its 1d
		   $FIELD = d1*sizeof($type*);
		*/
	/*int adim = (a1 && a2) ? 2 : (a1 || a2 ? 1 : 0);
	char *cre_arr = NULL;
	const char *t1 = bt2ct (type); // basic ?
	const char *t2 = hashget (th, (char *)type); // enum ?
	const char *ctype = t1 ? t1 : (t2 ? t2 : type);
	if (a1 && a2){// 2d array (type_rc)
		const char *dim1 = d1 ? strcopy (a1) :
			(cha1 ? concat ("obj->", cha1) :
			 concat ("obj->parent->", a1) );
		const char *dim2 = d2 ? strcopy (a2) :
			(cha2 ? concat ("obj->", cha2) :
			 concat ("obj->parent->", a2) );
		// expressions in arr sh*t
		if (strcmp ("MOPP_Data_Size_-_1", a1) == 0) {
			free ((char *)dim1);
			dim1 = strcopy ("obj->MOPP_Data_Size - 1");
		}
		if (strcmp ("Num_UV_Sets_&amp;_63", a1) == 0) {
			free ((char *)dim1);
			dim1 = strcopy ("obj->Num_UV_Sets & 63");
		}
		if (strcmp ("MOPP_Data_Size_-_1", a2) == 0) {
			free ((char *)dim2);
			dim2 = strcopy ("obj->MOPP_Data_Size - 1");
		}
		if (strcmp ("Num_UV_Sets_&amp;_63", a2) == 0) {
			free ((char *)dim2);
			dim2 = strcopy ("obj->Num_UV_Sets & 63");
		}// EOexpressions in arr sh*t
		append_to (&cre_arr, "\t// init 2d array\n\t");
		append_to (&cre_arr, field);
		append_to (&cre_arr, " = "SCREATE" ((");
		append_to (&cre_arr, dim1);
		append_to (&cre_arr, ")*sizeof(");
		append_to (&cre_arr, ctype);
		append_to (&cre_arr, " *));\n");
		append_to (&cre_arr, "\tfor (i = 0; i < (");
		append_to (&cre_arr, dim1);
		append_to (&cre_arr, "); i++)\n\t\t");
		append_to (&cre_arr, field);
		append_to (&cre_arr, "[i] = "SCREATE" ((");
		append_to (&cre_arr, dim2);
		append_to (&cre_arr, ")*sizeof(");
		append_to (&cre_arr, ctype);
		append_to (&cre_arr, "));\n");
		append_to (&rc, cre_arr);
		free (cre_arr);
		append_to (&rc, "\t// read 2d array\n");
		append_to (&rc, "\tfor (i = 0; i < (");
		append_to (&rc, dim1);
		append_to (&rc, "); i++)\n");
		append_to (&rc, "\t\tfor (j = 0; j < (");
		append_to (&rc, dim2);
		append_to (&rc, "); j++)\n");
		free ((char *)dim1);
		free ((char *)dim2);
	}
	else if (a1 || a2) { //1d array (type_rc)
		const char *dim = a1 ?
			(d1 ? strcopy (a1) :
				(cha1 ? concat ("obj->", cha1) :
						( strcmp (a1, "ARG") ?
					    	concat ("obj->parent->", a1) :
							strcopy (a1)
						)
				)
			) :
			(d2 ? strcopy (a2) :
				(cha2 ? concat ("obj->", cha2) :
						( strcmp (a2, "ARG") ?
					    	concat ("obj->parent->", a2) :
							strcopy (a2)
						)
				)
			);
		const char *amerge = a1 ? a1 : a2;
		// expressions in arr sh*t
		if (strcmp ("MOPP_Data_Size_-_1", amerge) == 0) {
			free ((char *)dim);
			dim = strcopy ("obj->MOPP_Data_Size - 1");
		}
		if (strcmp ("Num_UV_Sets_&amp;_63", amerge) == 0) {
			free ((char *)dim);
			dim = strcopy ("obj->Num_UV_Sets & 63");
		}// EOexpressions in arr sh*t
		//printf ("DBG: %s %s %s\n", a1, a2, dim);
		append_to (&rc, "\t// init 1d array\n\t");
		append_to (&rc, field);
		append_to (&rc, " = "SCREATE" ((");
		append_to (&rc, dim);
		append_to (&rc, ")*sizeof(");
		append_to (&rc, ctype);
		append_to (&rc, "));\n");
		append_to (&rc, "\t// read 1d array\n");
		append_to (&rc, "\tfor (i = 0; i < (");
		append_to (&rc, dim);
		append_to (&rc, "); i++)\n");
		free ((char *)dim);
	}
	char *tabs =
		adim == 0 ? "\t" : (adim == 1 ? "\t\t" : "\t\t\t");
	char *aa =
		adim == 0 ? "" : (adim == 1 ? "[i]" : "[i][j]");
	if (type_rc) {
		append_to (&rc, tabs);
		append_to (&rc, field);
		append_to (&rc, aa);
		append_to (&rc, " = ");
		append_to (&rc, type_rc);
	} else {// not a simple type - \tread$TYPE (&$FIELD, $ARG);\n
		append_to (&rc, tabs);
		append_to (&rc, "read");
		append_to (&rc, type);
		append_to (&rc, " (&");
		append_to (&rc, field);
		append_to (&rc, aa);
		append_to (&rc, ", ");
		if (!arg)
			append_to (&rc, "0");
		else {
			if (hashget (fieldsh, (char *)arg))
				append_to (&rc, "obj->");// if a field
			append_to (&rc, arg);// if not a field
		}
		append_to (&rc, ");\n");
	}
	//printf ("DBG: %s\n", rc);
	// version checks and template info, version check is 1st
	// then uv, vcond
	// then cond
	//
	char *vc = NULL;
	int prev_cond = 0;
	if (v1 || v2) {
		char *ver1 = v2u (v1);
		char *ver2 = v2u (v2);
		append_to (&vc, VCHKCALL);
		append_to (&vc, ver1);
		append_to (&vc, ", ");
		append_to (&vc, ver2);
		append_to (&vc, ")");
		free (ver1);
		free (ver2);
		prev_cond++;
	}
	if (uv) {// add userver in the condition
		if (prev_cond)
			append_to (&vc, " && ");
		append_to (&vc, "(");
		append_to (&vc, USER_VERSION" == ");
		append_to (&vc, uv);
		append_to (&vc, ")");
		prev_cond++;
	}
	if (vcond) {
		if (prev_cond)
			append_to (&vc, " && ");
		append_to (&vc, "(");
		append_to (&vc, vcond);
		append_to (&vc, ")");
		prev_cond++;
	}
	if (cond) {
		if (prev_cond)
			append_to (&vc, " && ");
		append_to (&vc, "(");
		append_to (&vc, cond);
		append_to (&vc, ")");
		prev_cond++;
	}
	if (vc) {
		append_to (&vc, ")\n");
		append_to (&vc, "\t{\n");
		append_to (&vc, rc);
		append_to (&vc, "\t}\n");
	}
	free (field);
	if (vc) {
		append_to (&srcode, "\tif (");
		append_to (&srcode, vc);
		free (vc);
	}
	else
		append_to (&srcode, rc);
	free (rc);*/
	return 1;
}

static int
cg_struct_end (const char *name)
{
	if (!strcmp (name, "Key"))// special handling is required
		return 1;
	else if (!strcmp (name, "KeyGroup"))// special handling is required
		return 1;

	// other version/ check "group by" stop2
	if (other_vcheck) {
		if (major_vcheck)
			append_to (&srcode, "\t");
		append_to (&srcode, "\t}\n");
		free (other_vcheck);
		other_vcheck = NULL;
	}

	// major version check "group by" stop2
	if (major_vcheck) {
		append_to (&srcode, "\t}\n");
		free (major_vcheck);
		major_vcheck = NULL;
	}

	if (fieldsh) {
		hashrelease (fieldsh, 1);
		fieldsh = NULL;
	}
	// typedef fin
	append_to (&tdefs, "} ");
	append_to (&tdefs, name);
	append_to (&tdefs, ";\n\n");
	// code fin
	append_to (&srcode, "\tCOVERAGEOUT\n}\n\n");
	return 1;
}

static int
cg_epilogue()
{
	// c
	if (bdec) {
#if PRINTBASIC
	 	printf ("%s\n", bdec);	// basic types read proc declarations
#endif /* PRINTBASIC */
		free (bdec);
	}
	if (bdef) {
#if PRINTBASIC
		printf ("%s\n", bdef);	// basic types read proc definitions
#endif /* PRINTBASIC */
		free (bdef);
	}
	// h
	if (enums  ) {
#if PRINTENUMS
		printf ("%s\n", enums);	// enumerations
#endif /* PRINTENUMS */
		free (enums);
	}
	if (tfdefs ) {
		printf ("%s\n", tfdefs);	// typedef struct forwards
		free (tfdefs);
	}
	if (tdefs  ) {
		printf ("%s\n", tdefs);	// typedef struct
		free (tdefs);
	}
	if (srcalls) {
		printf ("%s\n", srcalls);	// tyepdef struct readproc declarations
		free (srcalls);
	}
	// c
	cg_nif_parser_macros ();
	if (srcode) {
		printf ("%s", srcode);	// tyepdef struct readproc definitions
		free (srcode);
	}
	printf ("\nstatic int iamthelastline;\n");
	return 1;
}
#endif /* CG */
