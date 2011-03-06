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
#ifndef __NIFF_H__
#define __NIFF_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char   NIFbyte;	// 1 B
typedef char   NIFchar;	// 1 B
typedef int             NIFint;		// 4 DW
typedef unsigned int    NIFuint;	// 4 DW
typedef short           NIFshort;	// 2 W
typedef unsigned short  NIFushort;	// 2 W
typedef float           NIFfloat;	// 4 DW

static const size_t SNIFbyte = sizeof(NIFbyte);
static const size_t SNIFchar = sizeof(NIFchar);
static const size_t SNIFint = sizeof(NIFint);
static const size_t SNIFuint = sizeof(NIFuint);
static const size_t SNIFshort = sizeof(NIFshort);
static const size_t SNIFushort = sizeof(NIFushort);
static const size_t SNIFfloat = sizeof(NIFfloat);

#define SIZEOFWORD 2
#define SIZEOFDWORD 4

#define MAKE_SURE(COND,MSG)\
	if(!(COND)) {\
		printf (stderr, MSG);\
		exit (1);\
	}

struct NifStream;
typedef struct NifStream Stream;

typedef int (*PReadByte)(Stream *s, NIFbyte *buf, int count);
typedef int (*PReadChar)(Stream *s, NIFchar *buf, int count);
typedef int (*PReadCharCond)(Stream *s, NIFchar *buf, int count, NIFchar cond);
typedef int (*PReadUInt)(Stream *s, NIFuint *buf, int count);
typedef int (*PReadInt)(Stream *s, NIFint *buf, int count);
typedef int (*PReadUShort)(Stream *s, NIFushort *buf, int count);
typedef int (*PReadShort)(Stream *s, NIFshort *buf, int count);
typedef int (*PReadFloat)(Stream *s, NIFfloat *buf, int count);

typedef struct {
	FILE *f;
	char *buf;
	int buf_len;// length - like 120
	int buf_pos;// position - like 23
	int buf_cap;// cpacity - like 1000
	PReadByte read_byte;
	PReadChar read_char;
	PReadCharCond read_char_cond;
	PReadUInt read_uint;
	PReadInt read_int;
	PReadUShort read_ushort;
	PReadShort read_short;
	PReadFloat read_float;
} NifStream;

NifStream *NifStream_create(char *fname, int bufsize);
void NifStream_free (NifStream *s);

#endif /* __NIFF_H__ */