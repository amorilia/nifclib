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
#include "niff.h"

static inline size_t
read(register NifStream *s, register void *ptr, register size_t size)
{
	int result = 0;
	for (;;) {
		if (s->buf_pos + size <= s->buf_len) {
			memcpy (ptr, &(s->buf[s->buf_pos]), size);
			s->buf_pos += size;
			result += size;
			return result;
		}
		int part = s->buf_len - s->buf_pos;
		if (part > 0) {
			memcpy (ptr, &(s->buf[s->buf_pos]), part);
			s->buf_pos += part;
			result += part;
		}
		s->buf_len = fread (&(s->buf[0]), 1, s->buf_cap, s->f);
		if (s->buf_len <= 0)
			return result;// EOF, sorry
		s->buf_pos = 0;
		ptr += part;
		size -= part;
	}
	//return read(s, &(s->buf[part]), size); - dangerous
}

static inline int
read_byte(register NifStream *s, register NIFbyte *buf, register int count)
{
	return read (s, buf, count);
}

static inline int
read_char(register NifStream *s, register NIFchar *buf, register int count)
{
	return read (s, buf, count);
}

// It always returns bytes read.
// Ptr needs to have space for "cond" as well.
static inline int
read_char_cond(register NifStream *s, register NIFchar *ptr, register int size, register NIFchar cond)
{
	int i, idx = s->buf_pos, rpos = 0, cnt = 0;
	for (i = 0; i < size; i++) {
		if (idx >= s->buf_len) {
			cnt = idx - s->buf_pos;
			memcpy (&ptr[rpos], &(s->buf[s->buf_pos]), cnt);
			s->buf_len = fread (&(s->buf[0]), 1, s->buf_cap, s->f);
			if (s->buf_len <= 0)
				return i;// EOF - no "cond" found
			s->buf_pos = 0;
			idx = 0;
			rpos += cnt;
		}
		else if (s->buf[idx] == cond) {
			idx++;// incl. "cond"
			memcpy (&ptr[rpos], &(s->buf[s->buf_pos]), idx - s->buf_pos);
			s->buf_pos = idx;
			return i + 1;
		}
		idx++;
	}
	// no "cond" found within "size"
	return rpos;
}

static inline int
read_uint(register NifStream *s, register NIFuint *buf, register int count)
{
	return read (s, buf, count*SIZEOFDWORD);
}

static inline int
read_int(register NifStream *s, register NIFint *buf, register int count)
{
	return read (s, buf, count*SIZEOFDWORD);
}

static inline int
read_short(register NifStream *s, register NIFshort *buf, register int count)
{
	return read (s, buf, count*SIZEOFWORD);
}

static inline int
read_ushort(register NifStream *s, register NIFushort *buf, register int count)
{
	return read (s, buf, count*SIZEOFWORD);
}

static inline int
read_float(register NifStream *s, register NIFfloat *buf, register int count)
{
	return read (s, buf, count*SIZEOFDWORD);
}

NifStream *
NifStream_create (char *fname, int bufsize)
{
	if (bufsize <= 0)
		return NULL;
	if (!fname)
		return NULL;
	NifStream *tmp = malloc (sizeof(NifStream));
	if (!tmp)
		return NULL;// not enough free memory
	tmp->f = fopen (fname, "r");
	if (!(tmp->f)) {
		free (tmp);
		return NULL;
	}
	tmp->buf = malloc (bufsize);
	if (!(tmp->buf)) {
		fclose (tmp->f);
		free (tmp);
		return NULL;
	}
	tmp->buf_cap = bufsize;
	tmp->buf_len = 0;
	tmp->buf_pos = 0;
	tmp->read_byte = (PReadByte)read_byte;
	tmp->read_char = (PReadChar)read_char;
	tmp->read_char_cond = (PReadCharCond)read_char_cond;
	tmp->read_uint = (PReadUInt)read_uint;
	tmp->read_int = (PReadInt)read_int;
	tmp->read_ushort = (PReadUShort)read_ushort;
	tmp->read_short = (PReadShort)read_short;
	tmp->read_float = (PReadFloat)read_float;
	return tmp;
}

void
NifStream_free (NifStream *s)
{
	if (s) {
		free (s->buf);
		fclose (s->f);
		free (s);
	}
}