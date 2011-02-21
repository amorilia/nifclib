#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "niff.h"

#define CHECK_THAT(COND, MSG)\
	if (!(COND)) {\
		printf ("FAILED: "MSG"\n");\
	} else printf ("PASSED: "MSG"\n");

int
main(int argc, char **argv)
{
	NifStream *t;
	char *fexists = "test_niff_file";
	char *fnotexists = "";
	int correctbufsize = 1*1024*1024;
	int wrongbufsize1 = 0;
	int wrongbufsize2 = -1;
	int outofmembufsize = 2000000000;// 32 bit machines only
	t = NifStream_create(fexists, wrongbufsize1);
	CHECK_THAT (!t, "zero sized buffer")
	t = NifStream_create(fexists, wrongbufsize2);
	CHECK_THAT (!t, "negative sized buffer")
	t = NifStream_create(fexists, outofmembufsize);
	CHECK_THAT (!t, "too large buffer")
	t = NifStream_create(NULL, correctbufsize);
	CHECK_THAT (!t, "NULL argument - fname")
	t = NifStream_create(fnotexists, correctbufsize);
	CHECK_THAT (!t, "open inaccessable file")

	t = NifStream_create(fexists, correctbufsize);
	CHECK_THAT (t, "valid arguments")
	CHECK_THAT (t->buf_pos == 0, "initial read ptr position")
	CHECK_THAT (t->buf_len == 0, "initial data length")
	CHECK_THAT (t->buf_cap == correctbufsize, "initial capacity")
	CHECK_THAT (t->f, "file handler")
	NifStream_free (t);

	FILE *test_file = fopen (fexists, "w+");
	char *test_file_data = "0123456789\n0123456789";
	int test_dlen = strlen (test_file_data);
	if (!test_file) {
		fprintf (stderr, "can't create test file \"%s\". ", fexists);
		fprintf (stderr, "Test not possible\n.");
		return 1;
	}
	fwrite (test_file_data, 1, test_dlen, test_file);
	fclose (test_file);
	int i, j;
	// read it byte by byte for different buffer sizes: [1;2*dlen]
	for (j = 1; j < 2*test_dlen; j++) {
		printf ("INFO: for buf size %d\n", j);
		t = NifStream_create(fexists, j);
		Stream *t_as_s = (Stream *)t;
		for (i = 0; i < test_dlen; i++) {
			NIFbyte b;
			int r = t->read_byte(t_as_s, &b, 1);
			CHECK_THAT (r == 1, " reading NIFbyte size")
			CHECK_THAT (b == (NIFbyte)test_file_data[i], " reading NIFbyte")
		}
		NifStream_free (t);
	}
	// cond char read
	{
		t = NifStream_create(fexists, j);
		Stream *s = (Stream *)t;
		NIFchar buf[12];
		buf[11] = '\0';
		int r = t->read_char_cond(s, buf, 12, '\n');
		CHECK_THAT (r == 11, " cond char read size");
		CHECK_THAT (buf[10] == '\n', " cond char read");
		CHECK_THAT (buf[11] == '\0', " cond char zero not touched");
		for (i = 0; i < 11; i++)
			CHECK_THAT (buf[i] == test_file_data[i], " cond char")
		
		r = t->read_char_cond(s, buf, 12, '\n');
		CHECK_THAT (r == 10, " cond char (EOF < size and no cond to meet) read size");
		CHECK_THAT (buf[11] == '\0', " cond char zero not touched");
		for (i = 0; i < 10; i++)
			CHECK_THAT (buf[i] == test_file_data[11+i], " cond char")
		NifStream_free (t);
	}
	return 0;
}
