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

static inline int
read_char_cond(register NifStream *s, register NIFchar *ptr, register int size, register NIFchar cond)
{
	int result = 0, i;
	//printf (" cond req size: %d\n", size);
	for (;;) {
		if ((s->buf_pos + size <= s->buf_len)
			// but we don't know that for "cond", so
			|| (s->buf_len > 0 && s->buf_pos < s->buf_len)
			) {
			for (i = 0; i < size; i++)///---
				if (s->buf[s->buf_pos + i] == cond) {
					//printf ("found at %d ", s->buf_pos + i);
					//printf ("size (%d) adj. to %d\n", size, i + 1);
					size = i + 1;
					break;
				}///---
			memcpy (ptr, &(s->buf[s->buf_pos]), size);
			s->buf_pos += size;
			result += size;
			return result;
		}
		int part = s->buf_len - s->buf_pos;
		if (part > 0) {
			for (i = 0; i < part; i++)///---
				if (s->buf[s->buf_pos + i] == cond) {
					part = i + 1;
					break;
				}///---
			memcpy (ptr, &(s->buf[s->buf_pos]), part);
			s->buf_pos += part;
			result += part;
		}
		s->buf_len = fread (&(s->buf[0]), 1, s->buf_cap, s->f);
		//printf ("req: %d, read %d\n", s->buf_cap, s->buf_len);
		if (s->buf_len <= 0)
			return result;// EOF, sorry
		s->buf_pos = 0;
		ptr += part;
		size -= part;
	}
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
	tmp->f = fopen (fname, "r");
	if (!(tmp->f)) {
		free (tmp);
		return NULL;
	}
	tmp->buf = malloc (bufsize);
	if (!(tmp->buf)) {
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