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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include "nif_reader2.h"

typedef void (*actn)(char *fname);

static void
pathwalk (char *dir, actn onFile)
{
    struct stat dirinfo;
    struct dirent * stFiles;
    DIR * stDirIn;
    char path[MAXPATHLEN];
    struct stat finfo;
    if (lstat (dir, &dirinfo) < 0)
    {
        perror (dir);
        return;// 1;
    }
    if (!S_ISDIR(dirinfo.st_mode))
        return;// 1;// not a folder
    if ((stDirIn = opendir (dir)) == NULL)
    {
        perror (dir);
        return;// 1;// failed to open
    }
    while ((stFiles = readdir (stDirIn)) != NULL)
    {
        sprintf (path, "%s/%s", dir, stFiles -> d_name);
        if (lstat (path, &finfo) < 0)
           perror (path);
        if (S_ISDIR(finfo.st_mode)) {
            //printf ("Directory: %s\n", path);
			int pl = strlen (path);
			if (pl > 1) {
				if (path[pl-1] == '.' && path[pl-2] == '.')
					continue;
				if (path[pl-1] == '.')
					continue;
			}
			pathwalk (path, onFile);
		}
        else {
            //printf ("Filename: %s\n", path);
			if (onFile) onFile (path);
		}
    }
    closedir (stDirIn);
}

static void DoReadNif(char *fname);
static int cnt = 0;

/* Returns (b - a) in microseconds */
long
time_interval(struct timeval *a, struct timeval *b)
{
	return ( (b->tv_sec - a->tv_sec) * 1000000 ) + (b->tv_usec - a->tv_usec);
}

static long long total_bytes = 0;
static int total_blocks = 0;
static int total_niobjects = 0;
static int total_malloc_calls = 0;
static int total_reallocs = 0;

int
main(int argc, char **argv)
{
	struct timeval tstart, tstop;
	/*if (argc == 2)
		return !readnif (argv[1]);*/
	/*gettimeofday (&tstart, NULL);
	int r = readnif ("");
	gettimeofday (&tstop, NULL);
	long ttaken = time_interval (&tstart, &tstop);
	printf ("time %.2f milliseconds \n", ttaken/1000.0f);
	return !r;*/

	char *dir = ".";
	gettimeofday (&tstart, NULL);
	pathwalk (dir, DoReadNif);
	gettimeofday (&tstop, NULL);
	long ttaken = time_interval (&tstart, &tstop) / (1000*1000);
	printf ("files done %d (%ld files per second)\n", cnt, cnt / ttaken);
	printf ("time taken: %ld %.2ld:%.2ld\n", ttaken, ttaken / 60, ttaken % 60);
	printf ("total bytes read: %lld\n", total_bytes);
	printf ("major niff blocks parsed: %d\n", total_blocks);
	printf ("total niff objects parsed: %d\n", total_niobjects);
	printf ("total malloc calls: %d\n", total_malloc_calls);
	printf ("total reallocs: %d\n", total_reallocs);
	printf ("bytes/second: %lld\n", (total_bytes/ttaken));
    return 0;
}  // end main

/*static void
simple_read(char *fname)
{
	FILE *fh = fopen (fname, "r");
	const int bufsize = 1*1024*1024;
	byte buf[bufsize];
	int r;
	while ((r = fread (buf, 1, bufsize, fh)));
	fclose (fh);
}*/

static void
DoReadNif(char *fname)
{
	int len = strlen (fname);
	if (len < 4)
		return;
	if (fname[len-1] == 'f' || fname[len-1] == 'F')
		if (fname[len-2] == 'i' || fname[len-2] == 'I')
			if (fname[len-3] == 'n' || fname[len-3] == 'N')
				if (fname[len-4] == '.') {
					//simple_read (fname);
					if (!readnif (fname)) {
							printf ("files ok %d\n", cnt);
							exit (1);
					} else {
						total_bytes += NIFF_FSIZE;
						total_blocks += NIFF_BLOCK_COUNT;
						total_niobjects += NIFF_OBJECT_COUNT;
						total_malloc_calls += NIFF_MALLOCS;
						total_reallocs += NIFF_REALLOCS;
					}
					cnt++;
					printf ("%5d\r", cnt); fflush (stdout);
	}
}
