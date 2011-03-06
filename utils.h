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
#ifndef __UTILS_H__
#define __UTILS_H__
int starts_with(const char *buf, int len, const char *text);
int find (const char *what, const char *buf, int maxlen);
char *get_between(const char *buf, int maxlen, const char *f, const char *t);
void replace(char *buf, int maxlen, char a, char b);
char *replacestr(const char *buf, int maxlen, const char *a, const char *b);
char *concat(const char *a, const char *b);
char *strcopy(const char *);

typedef struct {
	char *val;
	char *key;
} kvp;

typedef struct {
	kvp *items;
	int cnt;
	int cap;
} kvp_list;// a.k.a. bucket

typedef struct {
	kvp_list **items;
	int cnt;
} hash;

hash *hashcreate (int cap);
void hashrelease (hash *h, int kvp);
int hashadd(hash *h, char *key, char *val);
void hashprint(hash *h);
char *hashget(hash *h, char *key);
#endif