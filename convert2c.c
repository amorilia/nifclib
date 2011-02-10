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
#define C_ACOND "cond=\""
#define C_AARG "arg=\""
#define C_ADEF "default=\""
#define C_AVERCOND "vercond=\""
#define C_ATEMPL "template=\""
#define C_AUVER "userver=\""

#define NA "<niobject"
#define NB "</niobject>"

static int convert (FILE *fh, const char *fname, long fsize);
static int convert_buf (const char *buf, int buflen);
static int info(const char *fmt, ...);
static int dbg(const char *fmt, ...);
// codegen procs
static int cg_prologue();
static int cg_basic (const char *name, const char *type);
static int cg_enum_begin (const char *name, const char *type);
static int cg_enum_optn (const char *name, const char *value);
static int cg_enum_end ();
static int cg_struct_begin (
	const char *name,
	const char *inh,
	const char *v1,
	const char *v2,
	const char *uv);
static int cg_struct_field (
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
static int cg_struct_end (const char *name);
static int cg_epilogue();

static hash *rch; // read calls hash - maps basic.name to a readcall
				  //                 - maps enum.name to a readcall
static hash *th; // type hash - maps basic.name to a c language type
				 //           - maps enum.name to a c language type

static FILE *newformat;

int
main(int argc, char **argv)
{
	/*
		-replace "name =" with "name=" and " =\"" with "=\""
		-fix <option in StencilAction - missing </option>
		-fix <add -> replace " />" with " ></add>"
		-fix <add -> replace "/>" with "></add>"
		-move SkinTransform above SkinData
	*/
	newformat = fopen ("nif.new.format", "w+");
	char *fname = "latest/nif.xml";
	FILE *fh = fopen (fname, "r");
	TRY (!fh, "file open failed\n", EXIT_FAILURE)
	TRY (fseek (fh, 0, SEEK_END), "seek failed\n", EXIT_FAILURE)
	long fs = ftell (fh);
	TRY (fs == -1, "seek failed\n", EXIT_FAILURE)
	info ("converting \"%s\" - %ld bytes ...\n", fname, fs);
	TRY (convert (fh, fname, fs), "convert failed\n", EXIT_FAILURE)
	TRY (fclose (fh), "close failed\n", EXIT_FAILURE)
	info ("done.\n");
	fclose (newformat);
	return EXIT_SUCCESS;
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

			/*
   		---/   parser   / /---
					---/*/

#define MAX_LBUF 1*1024*1024

static int
convert (FILE *fh, const char *fname, long fsize)
{
	if (fsize > (long)MAX_LBUF) {
		info ("I can not work with that file size yet\n");
		info ("I can only handle up to %d bytes\n", MAX_LBUF);
		return EXIT_FAILURE;
	}
	int buflen = (int)fsize;
	char *buf = malloc (buflen);
	if (!buf) {
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
	// UnionBV and BoundingVolume are in circular reference so one
	// of them not used anywhere elese - UnionBV, becomes a pointer
	hashadd (th, strcopy( "UnionBV"), strcopy ("struct UnionBV *"));
	int result = convert_buf (buf, buflen);
	free (buf);
	hashrelease (rch, 1);
	hashrelease (th, 1);
	return result;
}

static void
update_line_num(char key, int *line)
{
	if (key == '\n')
		(*line)++;
}

static int
skip_comments(const char *buf, int buflen, char **out)
{
	// <!--.*-->
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

static void
fix_symbol(char *buf, int len)
{
	replace (&buf[0], len, ' ', '_');// s/' '/'_'/g
	replace (&buf[0], len, '?', '_');// s/'\?'/'_'/g
	//dbg ("\nsym\"%s\"\n", buf);
}

static int bc = 0, ec = 0, fc = 0, cc = 0, nc = 0;

// returns true if ok, false otherwise
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
	cg_basic (name, type);
	free (name);
	free (type);
	/*char *test = malloc (len+1);
	memcpy (&test[0], &buf[0], len);
	test[len] = '\0';
	printf ("%s\n", test);
	free (test);*/
	return 1;
}

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
	cg_enum_begin (name, type);
	int i;
	for (i = 0; i < len; i++) {
		if (starts_with (&buf[i], len - i, E_OA)) {
			int e = find (E_OB, &buf[i], len - i);
			TRY (e <= 0, MWORD" "E_OA"\n", 0)
			char *val = get_between (&buf[i], e, E_OVALUE, EOA);
			char *nameo = get_between (&buf[i], e, E_ONAME, EOA);
			TRY (!val, MWORD" "E_OVALUE"\n", 0)
			TRY (!nameo, MWORD" "E_ONAME"\n", 0)
			fix_symbol (&nameo[0], strlen (nameo));
			dbg (" optn: %s=%s\n", nameo, val);
			i += e;
			cg_enum_optn (nameo, val);
			free (val);
			free (nameo);
		}
	}
	cg_enum_end ();
	free (name);
	free (type);
	return 1;
}

static int
pbitflags(const char *buf, int len)
{
	fc++;
	ec--;
	dbg ("F %d, ", len);
	return penum (buf, len);
}

static int
pcompound(const char *buf, int len)
{
	cc++;
	dbg ("C %d, ", len);
	int tlen = find (">", &buf[0], len);
	if (tlen <= 0)
		return 0;
	//len -= tlen;
	char *name = get_between (&buf[0], tlen, CNAME, EOA);
	if (starts_with (&name[0], strlen(name), "ns ")) {
		free (name);
		return 1;// skip things what are not for the lib
	}
	char *inh = get_between (&buf[0], tlen, CINH, EOA);
	char *ver1 = get_between (&buf[0], tlen, CVER1, EOA);
	char *ver2 = get_between (&buf[0], tlen, CVER2, EOA);
	char *userver = get_between (&buf[0], tlen, CUVER, EOA);
	TRY (!name, MWORD" "CNAME"\n", 0)
	if (inh)
		fix_symbol (&inh[0], strlen (inh));
	fix_symbol (&name[0], strlen (name));
	dbg ("name=%s, inh=%s, ver1=%s, ver2=%s\n",
		name, !inh ? "" : inh,
		!ver1 ? "0" : ver1, !ver2 ? "0" : ver2);
	cg_struct_begin (name, inh, ver1, ver2, userver);
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
			char *arr2 = get_between (&buf[i], e, C_AARR2, EOA);
			char *cond = get_between (&buf[i], e, C_ACOND, EOA);
			char *arg = get_between (&buf[i], e, C_AARG, EOA);
			char *def = get_between (&buf[i], e, C_ADEF, EOA);
			char *vc = get_between (&buf[i], e, C_AVERCOND, EOA);
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
			cg_struct_field (aname, atype, aver1, aver2, arr1, arr2,
				cond, arg, def, vc, tp, uvr);
			free (aname);// for now
			free (atype);// for now
			free (aver1);// for now
			free (aver2);// for now
			free (arr1);// for now
			free (arr2);// for now
			free (cond);// for now
			free (arg);// for now
			free (def);// for now
			free (vc);// for now
			free (tp);// for now
			free (uvr);// for now
		}
	}
	cg_struct_end (name);
	free (name);// for now
	free (inh);// for now
	free (ver1);// for now
	free (ver2);// for now
	free (userver);// for now
	return 1;
}

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

static int
convert_buf (const char *in_buf, int buflen)
{
	char *clean = malloc (buflen);
	TRY (!clean, "out of memory\n", EXIT_FAILURE)
	int clen = skip_comments (in_buf, buflen, &clean);
	if (!clen) {
		free (clean);
		return EXIT_FAILURE;
	}
	info ("comments cleaned up: %d bytes\n", buflen - clen);
	/*FILE *tmp = fopen("intermediate.xml", "a");
	fwrite (&clean[0], clen, 1, tmp);
	fclose (tmp);*/
	coverage += buflen - clen; // xml comments worked out
	cg_prologue ();
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
	cg_epilogue ();
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

#define SCREATE "malloc"
#define VCHK "VersionCheck"
#define VCHKCALL "VersionCheck ("
#define USER_VERSION "User_Version"
#define USER_VERSION2 "User_Version_2"

static int
cg_prologue()
{
	printf ("#include <stdlib.h>\n\n");
	printf ("#define READER\n");
	printf ("#define byte unsigned char\n");
	printf ("#define T_BYTE 0 \n");
	printf ("#define T_FLOAT 1 \n");
	printf ("#define T_QUATERNION 2 \n");
	printf ("#define T_STRING 3 \n");
	printf ("#define T_BYTECOLOR4 4 \n\n");
	printf ("typedef struct {\nvoid *d;\n int t;\n} TEMPLATE;\n\n");
	printf ("static int i, j, k;");
	printf ("static unsigned int Version;");
	printf ("static unsigned int "USER_VERSION";");
	printf ("static unsigned int "USER_VERSION2";");
	printf ("static int\n");
	printf (VCHK" (unsigned int a, unsigned int b)\n");
	printf ("{\n");
	printf ("\tif (a == 0 && b == 0) return 1;\n");
	printf ("\tif (a != 0 && b == 0) return Version >= a;\n");
	printf ("\tif (a == 0 && b != 0) return Version <= b;\n");
	printf ("\treturn Version >= a && Version <= b;\n");
	printf ("}\n");
	return 1;
}

#define BTMAP_LEN 17
static const char const *btmap[] = {
	"bool", "byte",
	"byte", "byte",
	"uint", "unsigned int",
	"ushort", "unsigned short",
	"int", "int",
	"short", "short",
	"BlockTypeIndex", "unsigned short",
	"char", "char",
	"FileVersion", "unsigned int",
	"Flags", "unsigned short",
	"float", "float",
	"HeaderString", "char *",
	"LineString", "char *",
	"Ptr", "unsigned int",
	"Ref", "unsigned int",
	"StringOffset", "unsigned int",
	"StringIndex", "unsigned int"
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

static int
cg_basic (const char *name, const char *type)
{
	fprintf (newformat, "B;%s;%s\n", name, type);
	// code
	printf ("%s\nread%s(READER)\n{\n\t//code_me\n}\n\n",
		bt2ct (name), name);
	char *tkey = strcopy (name);
	char *tval = strcopy (bt2ct (name));
	int t = hashadd (th, tkey, tval);
	if (t == -1) {
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

static int
cg_enum_begin (const char *name, const char *type)
{
	fprintf (newformat, "E;%s;%s\n", name, type);
	// def
	printf ("enum %s { // %s\n", name, type);
	char *tkey = strcopy (name);
	char *tval = strcopy (bt2ct (type));
	int t = hashadd (th, tkey, tval);
	if (t == -1) {
		free (tkey);
		free (tval);
	}
	// readcall
	char *key = strcopy (name);
	char *val = strcopy (hashget (rch, (char *)type));
	int r = hashadd (rch, key, val);
	if (r == -1) {
		free (key);
		free (val);
	}
	return r;
}

static int
cg_enum_optn (const char *name, const char *value)
{
	fprintf (newformat, " O;%s;%s\n", name, value);
	printf ("\t%s = %s,\n", name, value);
	return 1;
}

static int
cg_enum_end ()
{
	printf ("};\n\n");
	return 1;
}

static char *tfdefs = NULL;// struct %s;
static char *tdefs = NULL; // tyepdef struct {} %s;
static char *srcalls = NULL; // read this, read that - declarations
static char *srcode = NULL; // read this, read that - definitions

static void
append_to(char **target, const char*a)
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

static char *
v2u(const char *version)
{
	if (!version)
		return strcopy ("0");
	//printf ("DBG: tokenizing %s\n", version);
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
cg_struct_begin(const char *name, const char *inh, const char *v1,
				const char *v2, const char *uv)
{
	fprintf (newformat, "S;%s;%s;%s;%s;%s\n",
		name,
		inh ? inh : "",
		v1 ? v1 : "",
		v2 ? v2 : "",
		uv ? uv : "");
	//printf ("DBG: %s %s %s %s %s\n", name, inh, v1, v2, uv);
	if (!fieldsh)
		fieldsh = hashcreate (20);
	// typedef - forward
	append_to (&tfdefs, "struct ");
	append_to (&tfdefs, name);
	append_to (&tfdefs, ";\n");
	// typedef
	append_to (&tdefs, "typedef struct {\n");
	if (inh) {
		append_to (&tdefs, "\t");
		append_to (&tdefs, inh);
		append_to (&tdefs, " *parent;\n");
	}
	// readcall
	//  don't register it with rch, use rch for simple types only -
	//  basic adn enum. The field reader is able to buid the call.
	// void read%s(%s *obj, unsigned int ARG);
	append_to (&srcalls, "void read");
	append_to (&srcalls, name);
	append_to (&srcalls, "(");
	append_to (&srcalls, name);
	append_to (&srcalls, " *obj, unsigned int ARG);\n");
	// readcode
	append_to (&srcode, "void\nread");
	append_to (&srcode, name);
	append_to (&srcode, "(");
	append_to (&srcode, name);
	append_to (&srcode, " *obj, unsigned int ARG)\n");
	append_to (&srcode, "{\n");
	// readcode read parent call
	// VERCHK(v2u(v1), v2u(v2)
	if (/*inh &&*/ (v1 || v2 || uv)) {
		if (v1 || v2) {
			char *ver1 = v2u(v1);
			char *ver2 = v2u(v2);
			append_to (&srcode, "\tif (!"VCHKCALL);
			append_to (&srcode, ver1);
			append_to (&srcode, ", ");
			append_to (&srcode, ver2);
			free (ver1);
			free (ver2);
			if (uv) {// add userver in the condition
				append_to (&srcode, ") || "USER_VERSION" != ");
				append_to (&srcode, uv);
				append_to (&srcode, ")\n");
			}
			else
				append_to (&srcode, "))\n");
			append_to (&srcode, "\t\treturn;\n");
		}
	}
	if (inh) {// init parent
		append_to (&srcode, "\tobj->parent = "SCREATE" (sizeof(");
		append_to (&srcode, inh);
		append_to (&srcode, "));\n");
		// read call
		append_to (&srcode, "\tread");
		append_to (&srcode, inh);
		append_to (&srcode, " (obj->parent, 0);\n");
	}
	return 1;
}

static int
cg_struct_field(const char *name, const char *type, const char *v1,
				const char *v2, const char *a1, const char *a2,
				const char *cond, const char *arg,	const char *def,
				const char *vcond, const char *templ,
				const char *uv)
{
	fprintf (newformat, " F;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
		name, type,
		v1 ? v1 : "",
		v2 ? v2 : "",
		uv ? uv : "",
		a1 ? a1 : "",
		a2 ? a2 : "",
		cond ? cond : "",
		vcond ? vcond : "",
		arg ? arg : "",
		templ ? templ : "",
		def ? def : "");
	// typedef field
	char *btype = hashget (th, (char *)type);
	//append_to (&tdefs, "\t");
	if (fieldsh) {
		if(hashget (fieldsh, (char *)name)) {
			append_to (&tdefs, "\t// DUPLICATE FIELD NAME ");
			append_to (&tdefs, name);
			append_to (&tdefs, "\n");
			return 1;
		}
		else
			hashadd (fieldsh, strcopy (name), strcopy (name));
	}
	char *arr_thing = NULL;
	if (a1 || a2) {// DBG
		append_to (&arr_thing, "\t// [");
		append_to (&arr_thing, a1 ? a1 : "(null)");
		append_to (&arr_thing, "], [");
		append_to (&arr_thing, a2 ? a2 : "(null)");
		append_to (&arr_thing, "]\n");
	}
	append_to (&arr_thing, "\t");
	append_to (&arr_thing, btype ? btype : type);
	append_to (&arr_thing, " ");
	char *cha1 = a1 ? hashget (fieldsh, (char *)a1) : NULL;
	char *cha2 = a2 ? hashget (fieldsh, (char *)a2) : NULL;
	int d1 = a1 ? a1[0] >= '0' && a1[0] <= '9': 0;
	int d2 = a2 ? a2[0] >= '0' && a2[0] <= '9': 0;
	if (a1 && a2) {// 2d array
		// expressions again - crazy people ...
		//  - "MOPP_Data_Size_-_1" in bhkMoppBvTreeShape
		//  - "Num_UV_Sets_&amp;_63" in NiGeometryData
		//	- "Num_UV_Sets_&amp;_63" in NiParticlesData
		//
		// a1 can be, fname, parentfield, expr., digit, ARG
		//            cha1 , !d1        , !d1  , d1   , strcmp
		// a2 can be, fname, parentfield, expr., digit, ARG
		//printf ("DBG:%s %s %s %d %d\n", name, cha1, cha2, d1, d2);
/*no static init		if ((cha1 && cha2) ||
			(cha1 && !d2)  || // field and parent or expr
			(cha2 && !d1)  || // field and parent or expr
			(!d1 && !d2)      // parent or expr and parent or expr
			) { // both are field names or some other crazyness
*/
			append_to (&arr_thing, "**");
			append_to (&arr_thing, name);
/*no static init		}
		else if (cha1 || cha2) { // one of them is a field name
			append_to (&arr_thing, "*");
			append_to (&arr_thing, name);
			append_to (&arr_thing, "[");
			if (cha1)
				append_to (&arr_thing, a2);
			else append_to (&arr_thing, a1);
			append_to (&arr_thing, "]");
		} else { // both are not fields
			append_to (&arr_thing, name);
			append_to (&arr_thing, "[");
			append_to (&arr_thing, a1);
			append_to (&arr_thing, "][");
			append_to (&arr_thing, a2);
			append_to (&arr_thing, "]");
		}
*/
	}
	else if (a1 || a2) { //1d array
		const char *dim = a1 ? a1 : a2;
		int p = dim[0] < '0' || dim[0] > '9';
		if (p && !cha1 && !cha2 && strcmp (dim, "ARG"))
			// not here, but NaN either and not an ARG param
			append_to (&arr_thing, "/*parent-> or expr.*/");
// no static init		if ((a1 && cha1) || (a2 && cha2) || p) {// runtime size
			append_to (&arr_thing, "*");
			append_to (&arr_thing, name);
/*no static init		} else {// fixed size perhaps
			append_to (&arr_thing, name);
			append_to (&arr_thing, "[");
			append_to (&arr_thing, dim);
			append_to (&arr_thing, "]");
		}
*/
	} else
		append_to (&arr_thing, name);
	append_to (&arr_thing, ";\n");
	append_to (&tdefs, arr_thing);
	free (arr_thing);
	// code - fields reader
	//---
	// the read call
	//printf ("DBG: hello nurse!\n");
	char *field = concat ("obj->", name);
	char *type_rc = hashget (rch, (char *)type);
	char *rc = NULL;
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
	int adim = (a1 && a2) ? 2 : (a1 || a2 ? 1 : 0);
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
	free (rc);
	return 1;
}

static int
cg_struct_end (const char *name)
{
	if (fieldsh) {
		hashrelease (fieldsh, 1);
		fieldsh = NULL;
	}
	// typedef fin
	append_to (&tdefs, "} ");
	append_to (&tdefs, name);
	append_to (&tdefs, ";\n\n");
	// code fin
	append_to (&srcode, "}\n\n");
	return 1;
}

static int
cg_epilogue()
{
	printf ("%s\n", tfdefs);
	printf ("%s\n", tdefs);
	printf ("%s\n", srcalls);
	printf ("%s", srcode);
	printf ("\nstatic int iamthelastline;\n");
	free (tdefs);
	return 1;
}
