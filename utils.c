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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

// true if "buf" starts with "text\0", false otherwise
int
starts_with(const char *buf, int len, const char *text)
{
	int i,j = 0;
	if (!text[j])
		return 0;
	for (i = 0; i < len; i++) {
		if (text[j++] != buf[i])
			return 0;
		if (!text[j])
			return 1;
	}
	return 0;
}

// returns a index at witch 1st occurance of "what\0" starts,
// -1 otherwise
int
find (const char *what, const char *buf, int maxlen)
{
	int i = 0;
	while (!starts_with (&buf[i], maxlen-i, what)) {
		i++;
		if (i == maxlen)
			return -1;
	}
	return i;
}

// returns a substr between 1st occurance of "f\0" and 1st occurance
// of "t\0" after "f\0", or null if there are no "f\0" and/or "t\0"
char *
get_between(const char *buf, int maxlen, const char *f, const char *t)
{
	int a = find (f, buf, maxlen);
	if (a < 0)
		return NULL;
	// 0 1 2 3
    // . a b t
	// a = 1, strlen = 2
	// new_start = 3
	int new_start = a + strlen (f);
	if (new_start >= maxlen)
		return NULL;
	int b = find (t, &buf[new_start], maxlen - new_start);
	if (b < 0)
		return NULL;
	char *tmp = malloc (b + 1);
	if (!tmp) {
		printf ("out of memory\n");
		return NULL;
	}
	tmp[b] = '\0';
	memcpy (&tmp[0], &buf[new_start], b);
	return tmp;
}

// replace "a" with "b" in "buf"
void
replace(char *buf, int maxlen, char a, char b)
{
	int i;
	for (i = 0; i < maxlen; i++)
		if (buf[i] == a)
			buf[i] = b;
}

// return c\0 = a\0 + b\0
char *
concat(const char *a, const char *b)
{
	int la = strlen (a);
	int lb = strlen (b);
	char *tmp = malloc (la + lb + 1);
	if (!tmp) {
		printf ("out of memory.\n");
		return NULL;
	}
	memcpy (tmp, a, la);
	memcpy (&tmp[la], b, lb);
	tmp[la + lb] = '\0';
	return tmp;
}

char *
strcopy(const char *src)
{
	int len = strlen (src) + 1;
	char *tmp = malloc (len + 1);
	if (!tmp)
		return NULL;
	memcpy (tmp, src, len);
	tmp[len] = '\0';
	return tmp;
}

// the hash

hash *
hashcreate (int cap)
{
	hash *tmp = malloc (sizeof(hash));
	if (!tmp) {
		printf ("out of memory\n");
		return NULL;
	}
	if (cap <=  0) {
		printf ("invalid argument\n");
		return NULL;
	}
	tmp->cnt = cap;
	tmp->items = malloc (cap*sizeof(kvp_list*));
	if (!tmp->items) {
		printf ("out of memory\n");
		return NULL;
	}
	int i;
	for (i = 0; i < cap; i++)
		tmp->items[i] = NULL;
	return tmp;
}

void
hashrelease (hash *h, int kvp)
{
	int i;
	for (i = 0; i < h->cnt; i++) {
		kvp_list *list = h->items[i];
		if (!list)
			continue;
		if (kvp) {
			int j;
			for (j = 0; j < list->cnt; j++) {
				if (list->items[j].val) {
					free (list->items[j].val);
					list->items[j].val = NULL;
				}
				free (list->items[j].key);
			}
		}
		if (list->items)
			free (list->items);
		free (h->items[i]);
	}
	free (h->items);
	free (h);
}

static int
hashf(hash *h, char *key)
{
	int sum = 0;
	while (*key)
		sum += (int)(*(key++));
	return sum % h->cnt;
}

char *
hashget(hash *h, char *key)
{
	int idx = hashf (h, key);
	if (!h->items[idx])
		return NULL;
	kvp_list *l = h->items[idx];
	if (l->cnt <= 0)
		return NULL;
	int i;
	for (i = 0; i < l->cnt; i++)
		if (!strcmp(l->items[i].key, key))
			return l->items[i].val;
	return NULL;
}
// returns -1 when key exists, 1 when ok and 0 if system failure
int
hashadd(hash *h, char *key, char *val)
{
	if ( hashget (h, key) )
		return -1;
	int idx = hashf (h, key);
	//printf ("hashf (%s) returned %d\n", key, idx);
	if (!h->items[idx]) {
		h->items[idx] = malloc (sizeof(kvp_list));
		if (!h->items[idx]) {
			printf ("out of memory\n");
			return 0;
		}
		h->items[idx]->items = NULL;
		h->items[idx]->cap = 0;
		h->items[idx]->cnt = 0;
	}
	kvp_list *l = h->items[idx];
	if (!l->items) {
		l->items = malloc (2*sizeof(kvp));
		if (!l->items) {
			printf ("out of memory\n");
			return 0;
		}
		l->cnt = 0;
		l->cap = 2;
	}
	if (l->cnt == l->cap) {
		l->cap += l->cap / 2;
		l->items = realloc (l->items, l->cap*sizeof(kvp));
		if (!l->items) {
			printf ("out of memory\n");
			return 0;
		}
	}
	l->items[l->cnt].val = val;
	l->items[l->cnt].key = key;
	l->cnt++;
	return 1;
}

void
hashprint(hash *h)
{
	if (!h) {
		printf ("(null)\n");
		return;
	}
	printf ("hash buckets: %d\n", h->cnt);
	int b;
	for (b = 0; b < h->cnt; b++) {
		kvp_list *l = h->items[b];
		if (!l) {
			printf ("b[%3d]=(null)\n", b);
			continue;
		}
		printf ("b[%3d]=lst(cnt: %d, cap: %d)\n", b, l->cnt, l->cap);
		int i;
		for (i = 0; i < l->cnt; i++)
			printf ("\tlst[%3d]=%s, %s\n", i,
				l->items[i].key, l->items[i].val);
	}
}

