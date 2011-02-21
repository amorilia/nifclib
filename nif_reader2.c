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
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "nif_reader2.h"
#include "utils.h"
#include "niff.h"

//int ncalls[512];
#define COVERAGE
/*#define COVERAGE\
	printf ("%d\n", __LINE__);*/

static void *GetMem (size_t size);

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

//static FILE *R;
static NifStream *R;
static Stream *S;
static int ERR;
static int STOP;

#define READERARGLST register NifStream *r
#define READER R
#define GETMEM(SIZE) GetMem (SIZE)

#define ENONE 0
#define EM 1
#define EIO 2
#define ENIF 3
#define EBUG 4

// Nif Parsing Failed
static void
NPF(const char *msg, int ecode)
{
	dbg ("*** %s\n", msg);
	ERR = ecode;
	STOP = 1;
}

static void
NPFF(int ecode, const char *msg, ...)
{
	va_list arg;
	int done;
	va_start (arg, msg);
	done = vprintf (msg, arg);
	va_end (arg);

	ERR = ecode;
	STOP = 1;
}

static inline byte readbool (READERARGLST);
static inline byte readbyte (READERARGLST);
static inline unsigned int readuint (READERARGLST);
static inline unsigned short readushort (READERARGLST);
static inline int readint (READERARGLST);
static inline short readshort (READERARGLST);
static inline unsigned short readBlockTypeIndex (READERARGLST);
static inline char readchar (READERARGLST);
static inline unsigned int readFileVersion (READERARGLST);
static inline unsigned short readFlags (READERARGLST);
static inline float readfloat (READERARGLST);
static inline char *readHeaderString (READERARGLST);
static inline char *readLineString (READERARGLST);
static inline unsigned int readPtr (READERARGLST);
static inline unsigned int readRef (READERARGLST);
static inline unsigned int readStringOffset (READERARGLST);
static inline unsigned int readStringIndex (READERARGLST);

static inline void
readTEMPLATE(TEMPLATE *obj, unsigned int ARG, int t)
{
	COVERAGE
	obj->t = t;
	if (t == T_BYTE) {
		obj->d = GETMEM (sizeof(byte));
		*(byte *)(obj->d) = readbyte (READER);
	} else if (t == T_FLOAT) {
		obj->d = GETMEM (sizeof(float));
		*(float *)(obj->d) = readfloat (READER);
	} else if (t == T_QUATERNION) {
		obj->d = GETMEM (sizeof(Quaternion));
		readQuaternion ( (Quaternion *)(obj->d), 0 );
	} else if (t == T_STRING) {
		obj->d = GETMEM (sizeof(string));
		readstring ( (string *)(obj->d), 0 );
	} else if (t == T_BYTECOLOR4) {
		obj->d = GETMEM (sizeof(ByteColor4));
		readByteColor4 ( (ByteColor4 *)(obj->d), 0 );
	} else if (t == T_COLOR4) {
		obj->d = GETMEM (sizeof(Color4));
		readColor4 ( (Color4 *)(obj->d), 0 );
	} else if (t == T_VECTOR3) {
		obj->d = GETMEM (sizeof(Vector3));
		readVector3 ( (Vector3 *)(obj->d), 0 );
	} else NPFF (ENIF, "*** unknown TEMPLATE: %d\n", t);
}

//static int i, j/*, k*/;
static unsigned int Version = -1;
static unsigned int User_Version = -1;
static unsigned int User_Version_2 = -1;

// converts a version string into int
static int
v2u(char *version)
{
	if (!version)
		return 0;
	//printf ("DBG: tokenizing %s\n", version);
	char *t = version;
	char *tokens[4] = {"0", "0", "0", "0"};
	int len = strlen (t);
	int i, p = 0, k = 0;
	for (i = 0; i < len + 1; i++) {
		if (t[i] == '.' || t[i] == '\0') {
			if (k == 4)
				return 0;
			t[i] = '\0';
			tokens[k++] = &t[p];
			p = i + 1;
		}
	}
	unsigned int v = 0;
	for (k = 0; k < 4; k++) {
		unsigned int f = (unsigned int)atoi (tokens[k]);
		v |= f << ((3-k)*8);
	}
	return v;
}

// try to init "Version" from the header string
static void
InitVersion(const char *hs)
{
	Version = 0;
	User_Version = 0;
	User_Version_2 = 0;
	if (!hs)
		NPF ("header string can not be null\n", ENIF);
	int l = strlen (hs);
	if (l < 3)// 2.3 4example
		NPFF (ENIF, "*** unknown header string \"%s\"\n", hs);
	dbg ("InitVersion from \"%s\"\n", hs);
	char *hs_c = malloc (l + 1);
	if (!hs_c)
		NPF ("malloc failed\n", EM);
	memcpy (&hs_c[0], &hs[0], l);
	hs_c[l] = '\0';
	// find last ' ' - this should be the version if any
	char *e = &hs_c[l];
	do {
		if (*(--e) == ' ') {
			Version = v2u (e);
			break;
		}
	} while (e != hs_c);
	dbg (" Version is 0x%.8X\n", Version);
	free (hs_c);
}

static void **MM;
static int MM_cnt = 0;
static int MM_cap = 0;
static void
InitMM ()
{
	MM = malloc (1024*sizeof(void *));
	MM_cap = 1024;
	MM_cnt = 0;
}
static void
QuitMM ()
{
	if (MM_cnt > 0)
		dbg ("releasing %d memory locations ...", MM_cnt);
	int i;
	for (i = 0; i < MM_cnt; i++) {
		free (MM[i]);
		MM[i] = NULL;
	}
	free (MM);
	MM = NULL;
	if (MM_cnt > 0)
		dbg ("done. Have a nice day!\n");
	MM_cnt = 0;
	MM_cap = -1;
}
static void
*GetMem (size_t size)
{
	if (MM_cnt == MM_cap) {
		MM_cap += 1024;
		MM = realloc (MM, MM_cap*sizeof(void *));
		if (!MM) {// TODO: exit_proc a.k.a. catch (E) {}
			NPF ("realloc failed.\n", EM);
			return NULL;
		}
		NIFF_REALLOCS++;
	}
	MM[MM_cnt] = malloc (size);
	if (!MM[MM_cnt]) {// TODO: exit_proc a.k.a. catch (E) {}
		NPF ("malloc failed.\n", EM);
		return NULL;
	}
	NIFF_MALLOCS++;
	MM[MM_cnt] = memset (MM[MM_cnt], 0, size);
	if (!MM[MM_cnt]) {// TODO: exit_proc a.k.a. catch (E) {}
		NPF ("memset failed.\n", EM);
		return NULL;
	}
	//dbg ("GETMEM[%5d]:%5d, %p - OK\n", MM_cnt, size, MM[MM_cnt]);
	return MM[MM_cnt++];
}

static inline int VersionCheck(unsigned int a, unsigned int b);
static void ReadNifObject(char *name);
static void TellFilePos(READERARGLST);
static void CreateRpHash();
static void ReleaseRpHash();

#define DO(A)\
	A \
	if (STOP) break;

			/*
   		---/   main loop   / /---
					   ---/*/

#define READ1(TYPE, BUF, LEN, BYTES, PROC)\
	{\
		int r = R->read_##TYPE (S, BUF, LEN);\
		if (r != (LEN * BYTES))\
			NPF (PROC": EOF reached\n", EIO);\
	}

int
readnif(char *fname)
{
	R = NifStream_create (fname, 1*1024*1024);
	dbg ("%s\n", fname);
	if (!R) {
		dbg ("*** Can't open \"%s\"\n", fname);
		return 0;
	}
	S = (Stream *)R;
	NIFF_FSIZE = 0;
	NIFF_BLOCK_COUNT = 0;
	NIFF_OBJECT_COUNT = 0;
	NIFF_MALLOCS = 0;
	NIFF_REALLOCS = 0;
	fseek (R->f, 0, SEEK_END);
	long fsize = ftell (R->f);
	fseek (R->f, 0, SEEK_SET);

	// just read the entire file in memory
	/*byte *buf = malloc (fsize);
	READ1 (byte, buf, fsize, 1, "speed")
	free (buf);
	NifStream_free (R);
	return 1;*/

	ERR = ENONE;
	STOP = 0;
	InitMM ();
	Header *h = GETMEM (sizeof(Header));
	int i = 0;
	//
	// header
	//
	dbg ("Getting header ...\n");
	readHeader (h, 0);
	dbg (" Num_Blocks: %d\n", h->Num_Blocks);
	dbg (" Num_Block_Types: %d\n", h->Num_Block_Types);
	dbg (" User Version: %d\n", User_Version);
	dbg (" User Version 2: %d\n", User_Version_2);
	NIFF_BLOCK_COUNT = h->Num_Blocks;
	if (STOP) {
		NifStream_free (R);
		QuitMM ();
		return 0;
	}
	TellFilePos (READER);
	//
	// nif objects
	//
	CreateRpHash ();
	// header read and parsed ok - at least physically :)
	// two (perhaps more) major ways of reading
	if (Version < 0x030300D) {
		NPF (" see how to deal with version < 3.3.0.13\n", EBUG);
	}
	else if (Version < 0x05000001) {
		for (i = 0; i < h->Num_Blocks; i++) {
			dbg ("reading zblock #%d", i);
			DO (SizedString *n = GETMEM (sizeof(SizedString));)
			DO (readSizedString (n, 0);)
			dbg (", name = \"%s\",", (char *)(n->Value));
			char *tname = (char *)(n->Value);
			DO (ReadNifObject(tname);)
			dbg (" done.");
			TellFilePos (READER);
		}
	}
	else {
		/*for (i = 0; i < h->Num_Blocks; i++) {
			int tidx = h->Block_Type_Index[i];
			dbg ("i=%d, tidx:%d\n", i, tidx);
		}*/
		for (i = 0; i < h->Num_Blocks; i++) {
			dbg ("reading block #%2d ", i);
			if (VersionCheck(0x05000001, 0x0A01006A)) {
				unsigned int z = readuint (READER);// 0
				if (z) {
					NPFF (ENIF, "*** leading 0 is %d\n", z);
					break;
				}
			}
			int tidx = h->Block_Type_Index[i];
			//dbg ("i=%d, tidx:%d, nbt:%d\n", i, tidx, h->Num_Block_Types);
			if (tidx < 0 || tidx >= h->Num_Block_Types) {
				NPFF (EBUG, "*** tidx(%d) out of range\n", tidx);
				break;
			}
			char *tname = (char *)(h->Block_Types[tidx].Value);
			//dbg ("%p\n", tname);
			//dbg ("%s\n", tname);
			dbg ("reading NIFOBJECT \"%s\"", tname);
			DO (ReadNifObject(tname);)
			TellFilePos (READER);
		}
	}
	//
	// footer
	//
	if (!STOP) {
		Footer *f = GETMEM (sizeof(Footer));
		if (!STOP)
			readFooter (f, 0);
	}
	int result = 0;
	if (ftell(R->f) != fsize)// {
	// according to nifskope it is ok for a nif to contain
	// extra bytes: morrowind/DoD/DameCecilia2Hair.nif
	// I have truncated the useless bytes and it loads it just
	// fine.
		dbg ("WRN extra bytes: %ld/%ld [bytes] were read.\n",
			ftell(R->f), fsize);
	//} else {
		if (!STOP) {
			dbg ("read ok\n");
			NIFF_FSIZE = fsize;
			result = 1;
		}
	//}
	NifStream_free (R);
	QuitMM ();
	ReleaseRpHash ();
	return result;
}

static inline int
VersionCheck(register unsigned int a, register unsigned int b)
{
	if (a == 0 && b == 0)
		return 1;
	if (a != 0 && b == 0)
		return Version >= a;
	if (a == 0 && b != 0)
		return Version <= b;
	return Version >= a && Version <= b;
}

			/*
   		---/   489 read procs and their hash init / /---
					                          ---/*/
/*
****
*  There are a lot of functions here what never get
* called so a cleanup is in order, yet it will happen
* slowly because of the large amount of useless objects
* and the uncertainity which ones are from the nif files and
* which ones are from the authors of the original niflib     *
*                                                         ****/
typedef void (*NifObjRProc)();
static void
RPSizedString()
{
	SizedString *t = GETMEM (sizeof(SizedString));
	if (STOP) return;
	readSizedString (t, 0);
}
static void
RPstring()
{
	string *t = GETMEM (sizeof(string));
	if (STOP) return;
	readstring (t, 0);
}
static void
RPByteArray()
{
	ByteArray *t = GETMEM (sizeof(ByteArray));
	if (STOP) return;
	readByteArray (t, 0);
}
static void
RPByteMatrix()
{
	ByteMatrix *t = GETMEM (sizeof(ByteMatrix));
	if (STOP) return;
	readByteMatrix (t, 0);
}
static void
RPColor3()
{
	Color3 *t = GETMEM (sizeof(Color3));
	if (STOP) return;
	readColor3 (t, 0);
}
static void
RPByteColor3()
{
	ByteColor3 *t = GETMEM (sizeof(ByteColor3));
	if (STOP) return;
	readByteColor3 (t, 0);
}
static void
RPColor4()
{
	Color4 *t = GETMEM (sizeof(Color4));
	if (STOP) return;
	readColor4 (t, 0);
}
static void
RPByteColor4()
{
	ByteColor4 *t = GETMEM (sizeof(ByteColor4));
	if (STOP) return;
	readByteColor4 (t, 0);
}
static void
RPFilePath()
{
	FilePath *t = GETMEM (sizeof(FilePath));
	if (STOP) return;
	readFilePath (t, 0);
}
static void
RPFooter()
{
	Footer *t = GETMEM (sizeof(Footer));
	if (STOP) return;
	readFooter (t, 0);
}
static void
RPLODRange()
{
	LODRange *t = GETMEM (sizeof(LODRange));
	if (STOP) return;
	readLODRange (t, 0);
}
static void
RPMatchGroup()
{
	MatchGroup *t = GETMEM (sizeof(MatchGroup));
	if (STOP) return;
	readMatchGroup (t, 0);
}
static void
RPVector3()
{
	Vector3 *t = GETMEM (sizeof(Vector3));
	if (STOP) return;
	readVector3 (t, 0);
}
static void
RPVector4()
{
	Vector4 *t = GETMEM (sizeof(Vector4));
	if (STOP) return;
	readVector4 (t, 0);
}
static void
RPQuaternion()
{
	Quaternion *t = GETMEM (sizeof(Quaternion));
	if (STOP) return;
	readQuaternion (t, 0);
}
static void
RPQuaternionXYZW()
{
	QuaternionXYZW *t = GETMEM (sizeof(QuaternionXYZW));
	if (STOP) return;
	readQuaternionXYZW (t, 0);
}
static void
RPMatrix22()
{
	Matrix22 *t = GETMEM (sizeof(Matrix22));
	if (STOP) return;
	readMatrix22 (t, 0);
}
static void
RPMatrix33()
{
	Matrix33 *t = GETMEM (sizeof(Matrix33));
	if (STOP) return;
	readMatrix33 (t, 0);
}
static void
RPMatrix44()
{
	Matrix44 *t = GETMEM (sizeof(Matrix44));
	if (STOP) return;
	readMatrix44 (t, 0);
}
static void
RPMipMap()
{
	MipMap *t = GETMEM (sizeof(MipMap));
	if (STOP) return;
	readMipMap (t, 0);
}
static void
RPNodeGroup()
{
	NodeGroup *t = GETMEM (sizeof(NodeGroup));
	if (STOP) return;
	readNodeGroup (t, 0);
}
static void
RPShortString()
{
	ShortString *t = GETMEM (sizeof(ShortString));
	if (STOP) return;
	readShortString (t, 0);
}
static void
RPSkinShape()
{
	SkinShape *t = GETMEM (sizeof(SkinShape));
	if (STOP) return;
	readSkinShape (t, 0);
}
static void
RPSkinShapeGroup()
{
	SkinShapeGroup *t = GETMEM (sizeof(SkinShapeGroup));
	if (STOP) return;
	readSkinShapeGroup (t, 0);
}
static void
RPSkinWeight()
{
	SkinWeight *t = GETMEM (sizeof(SkinWeight));
	if (STOP) return;
	readSkinWeight (t, 0);
}
static void
RPAVObject()
{
	AVObject *t = GETMEM (sizeof(AVObject));
	if (STOP) return;
	readAVObject (t, 0);
}
static void
RPControllerLink()
{
	ControllerLink *t = GETMEM (sizeof(ControllerLink));
	if (STOP) return;
	readControllerLink (t, 0);
}
static void
RPExportInfo()
{
	ExportInfo *t = GETMEM (sizeof(ExportInfo));
	if (STOP) return;
	readExportInfo (t, 0);
}
static void
RPHeader()
{
	Header *t = GETMEM (sizeof(Header));
	if (STOP) return;
	readHeader (t, 0);
}
static void
RPStringPalette()
{
	StringPalette *t = GETMEM (sizeof(StringPalette));
	if (STOP) return;
	readStringPalette (t, 0);
}
static void
RPTBC()
{
	TBC *t = GETMEM (sizeof(TBC));
	if (STOP) return;
	readTBC (t, 0);
}
static void
RPQuatKey()
{
	QuatKey *t = GETMEM (sizeof(QuatKey));
	if (STOP) return;
	readQuatKey (t, 0);
}
static void
RPTexCoord()
{
	TexCoord *t = GETMEM (sizeof(TexCoord));
	if (STOP) return;
	readTexCoord (t, 0);
}
static void
RPTexDesc()
{
	TexDesc *t = GETMEM (sizeof(TexDesc));
	if (STOP) return;
	readTexDesc (t, 0);
}
static void
RPShaderTexDesc()
{
	ShaderTexDesc *t = GETMEM (sizeof(ShaderTexDesc));
	if (STOP) return;
	readShaderTexDesc (t, 0);
}
static void
RPTexSource()
{
	TexSource *t = GETMEM (sizeof(TexSource));
	if (STOP) return;
	readTexSource (t, 0);
}
static void
RPTriangle()
{
	Triangle *t = GETMEM (sizeof(Triangle));
	if (STOP) return;
	readTriangle (t, 0);
}
static void
RPSkinPartition()
{
	SkinPartition *t = GETMEM (sizeof(SkinPartition));
	if (STOP) return;
	readSkinPartition (t, 0);
}
static void
RPBoundingBox()
{
	BoundingBox *t = GETMEM (sizeof(BoundingBox));
	if (STOP) return;
	readBoundingBox (t, 0);
}
static void
RPFurniturePosition()
{
	FurniturePosition *t = GETMEM (sizeof(FurniturePosition));
	if (STOP) return;
	readFurniturePosition (t, 0);
}
static void
RPhkTriangle()
{
	hkTriangle *t = GETMEM (sizeof(hkTriangle));
	if (STOP) return;
	readhkTriangle (t, 0);
}
static void
RPMorph()
{
	Morph *t = GETMEM (sizeof(Morph));
	if (STOP) return;
	readMorph (t, 0);
}
static void
RPParticle()
{
	Particle *t = GETMEM (sizeof(Particle));
	if (STOP) return;
	readParticle (t, 0);
}
static void
RPSkinTransform()
{
	SkinTransform *t = GETMEM (sizeof(SkinTransform));
	if (STOP) return;
	readSkinTransform (t, 0);
}
static void
RPSkinData()
{
	SkinData *t = GETMEM (sizeof(SkinData));
	if (STOP) return;
	readSkinData (t, 0);
}
static void
RPSphereBV()
{
	SphereBV *t = GETMEM (sizeof(SphereBV));
	if (STOP) return;
	readSphereBV (t, 0);
}
static void
RPOblivionColFilter()
{
	OblivionColFilter *t = GETMEM (sizeof(OblivionColFilter));
	if (STOP) return;
	readOblivionColFilter (t, 0);
}
static void
RPOblivionSubShape()
{
	OblivionSubShape *t = GETMEM (sizeof(OblivionSubShape));
	if (STOP) return;
	readOblivionSubShape (t, 0);
}
static void
RPMotorDescriptor()
{
	MotorDescriptor *t = GETMEM (sizeof(MotorDescriptor));
	if (STOP) return;
	readMotorDescriptor (t, 0);
}
static void
RPRagdollDescriptor()
{
	RagdollDescriptor *t = GETMEM (sizeof(RagdollDescriptor));
	if (STOP) return;
	readRagdollDescriptor (t, 0);
}
static void
RPLimitedHingeDescriptor()
{
	LimitedHingeDescriptor *t = GETMEM (sizeof(LimitedHingeDescriptor));
	if (STOP) return;
	readLimitedHingeDescriptor (t, 0);
}
static void
RPHingeDescriptor()
{
	HingeDescriptor *t = GETMEM (sizeof(HingeDescriptor));
	if (STOP) return;
	readHingeDescriptor (t, 0);
}
static void
RPOldSkinData()
{
	OldSkinData *t = GETMEM (sizeof(OldSkinData));
	if (STOP) return;
	readOldSkinData (t, 0);
}
static void
RPMultiTextureElement()
{
	MultiTextureElement *t = GETMEM (sizeof(MultiTextureElement));
	if (STOP) return;
	readMultiTextureElement (t, 0);
}
static void
RPBoxBV()
{
	BoxBV *t = GETMEM (sizeof(BoxBV));
	if (STOP) return;
	readBoxBV (t, 0);
}
static void
RPCapsuleBV()
{
	CapsuleBV *t = GETMEM (sizeof(CapsuleBV));
	if (STOP) return;
	readCapsuleBV (t, 0);
}
static void
RPHalfSpaceBV()
{
	HalfSpaceBV *t = GETMEM (sizeof(HalfSpaceBV));
	if (STOP) return;
	readHalfSpaceBV (t, 0);
}
static void
RPBoundingVolume()
{
	BoundingVolume *t = GETMEM (sizeof(BoundingVolume));
	if (STOP) return;
	readBoundingVolume (t, 0);
}
static void
RPUnionBV()
{
	UnionBV *t = GETMEM (sizeof(UnionBV));
	if (STOP) return;
	readUnionBV (t, 0);
}
static void
RPMorphWeight()
{
	MorphWeight *t = GETMEM (sizeof(MorphWeight));
	if (STOP) return;
	readMorphWeight (t, 0);
}
static void
RPArkTexture()
{
	ArkTexture *t = GETMEM (sizeof(ArkTexture));
	if (STOP) return;
	readArkTexture (t, 0);
}
static void
RPInertiaMatrix()
{
	InertiaMatrix *t = GETMEM (sizeof(InertiaMatrix));
	if (STOP) return;
	readInertiaMatrix (t, 0);
}
static void
RPDecalVectorArray()
{
	DecalVectorArray *t = GETMEM (sizeof(DecalVectorArray));
	if (STOP) return;
	readDecalVectorArray (t, 0);
}
static void
RPBodyPartList()
{
	BodyPartList *t = GETMEM (sizeof(BodyPartList));
	if (STOP) return;
	readBodyPartList (t, 0);
}
/*static void
RPNiObject()
{
	NiObject *t = GETMEM (sizeof(NiObject));
	if (STOP) return;
	readNiObject (t, 0);
}*/
static void
RPNi3dsAlphaAnimator()
{
	Ni3dsAlphaAnimator *t = GETMEM (sizeof(Ni3dsAlphaAnimator));
	if (STOP) return;
	readNi3dsAlphaAnimator (t, 0);
}
static void
RPNi3dsAnimationNode()
{
	Ni3dsAnimationNode *t = GETMEM (sizeof(Ni3dsAnimationNode));
	if (STOP) return;
	readNi3dsAnimationNode (t, 0);
}
static void
RPNi3dsColorAnimator()
{
	Ni3dsColorAnimator *t = GETMEM (sizeof(Ni3dsColorAnimator));
	if (STOP) return;
	readNi3dsColorAnimator (t, 0);
}
static void
RPNi3dsMorphShape()
{
	Ni3dsMorphShape *t = GETMEM (sizeof(Ni3dsMorphShape));
	if (STOP) return;
	readNi3dsMorphShape (t, 0);
}
static void
RPNi3dsParticleSystem()
{
	Ni3dsParticleSystem *t = GETMEM (sizeof(Ni3dsParticleSystem));
	if (STOP) return;
	readNi3dsParticleSystem (t, 0);
}
static void
RPNi3dsPathController()
{
	Ni3dsPathController *t = GETMEM (sizeof(Ni3dsPathController));
	if (STOP) return;
	readNi3dsPathController (t, 0);
}
static void
RPNiParticleModifier()
{
	NiParticleModifier *t = GETMEM (sizeof(NiParticleModifier));
	if (STOP) return;
	readNiParticleModifier (t, 0);
}
static void
RPNiPSysCollider()
{
	NiPSysCollider *t = GETMEM (sizeof(NiPSysCollider));
	if (STOP) return;
	readNiPSysCollider (t, 0);
}
static void
RPbhkRefObject()
{
	bhkRefObject *t = GETMEM (sizeof(bhkRefObject));
	if (STOP) return;
	readbhkRefObject (t, 0);
}
static void
RPbhkSerializable()
{
	bhkSerializable *t = GETMEM (sizeof(bhkSerializable));
	if (STOP) return;
	readbhkSerializable (t, 0);
}
static void
RPbhkWorldObject()
{
	bhkWorldObject *t = GETMEM (sizeof(bhkWorldObject));
	if (STOP) return;
	readbhkWorldObject (t, 0);
}
static void
RPbhkPhantom()
{
	bhkPhantom *t = GETMEM (sizeof(bhkPhantom));
	if (STOP) return;
	readbhkPhantom (t, 0);
}
static void
RPbhkShapePhantom()
{
	bhkShapePhantom *t = GETMEM (sizeof(bhkShapePhantom));
	if (STOP) return;
	readbhkShapePhantom (t, 0);
}
static void
RPbhkSimpleShapePhantom()
{
	bhkSimpleShapePhantom *t = GETMEM (sizeof(bhkSimpleShapePhantom));
	if (STOP) return;
	readbhkSimpleShapePhantom (t, 0);
}
static void
RPbhkEntity()
{
	bhkEntity *t = GETMEM (sizeof(bhkEntity));
	if (STOP) return;
	readbhkEntity (t, 0);
}
static void
RPbhkRigidBody()
{
	bhkRigidBody *t = GETMEM (sizeof(bhkRigidBody));
	if (STOP) return;
	readbhkRigidBody (t, 0);
}
static void
RPbhkRigidBodyT()
{
	bhkRigidBodyT *t = GETMEM (sizeof(bhkRigidBodyT));
	if (STOP) return;
	readbhkRigidBodyT (t, 0);
}
static void
RPbhkConstraint()
{
	bhkConstraint *t = GETMEM (sizeof(bhkConstraint));
	if (STOP) return;
	readbhkConstraint (t, 0);
}
static void
RPbhkLimitedHingeConstraint()
{
	bhkLimitedHingeConstraint *t = GETMEM (sizeof(bhkLimitedHingeConstraint));
	if (STOP) return;
	readbhkLimitedHingeConstraint (t, 0);
}
static void
RPbhkMalleableConstraint()
{
	bhkMalleableConstraint *t = GETMEM (sizeof(bhkMalleableConstraint));
	if (STOP) return;
	readbhkMalleableConstraint (t, 0);
}
static void
RPbhkStiffSpringConstraint()
{
	bhkStiffSpringConstraint *t = GETMEM (sizeof(bhkStiffSpringConstraint));
	if (STOP) return;
	readbhkStiffSpringConstraint (t, 0);
}
static void
RPbhkRagdollConstraint()
{
	bhkRagdollConstraint *t = GETMEM (sizeof(bhkRagdollConstraint));
	if (STOP) return;
	readbhkRagdollConstraint (t, 0);
}
static void
RPbhkPrismaticConstraint()
{
	bhkPrismaticConstraint *t = GETMEM (sizeof(bhkPrismaticConstraint));
	if (STOP) return;
	readbhkPrismaticConstraint (t, 0);
}
static void
RPbhkHingeConstraint()
{
	bhkHingeConstraint *t = GETMEM (sizeof(bhkHingeConstraint));
	if (STOP) return;
	readbhkHingeConstraint (t, 0);
}
static void
RPbhkShape()
{
	bhkShape *t = GETMEM (sizeof(bhkShape));
	if (STOP) return;
	readbhkShape (t, 0);
}
static void
RPbhkTransformShape()
{
	bhkTransformShape *t = GETMEM (sizeof(bhkTransformShape));
	if (STOP) return;
	readbhkTransformShape (t, 0);
}
static void
RPbhkSphereRepShape()
{
	bhkSphereRepShape *t = GETMEM (sizeof(bhkSphereRepShape));
	if (STOP) return;
	readbhkSphereRepShape (t, 0);
}
static void
RPbhkConvexShape()
{
	bhkConvexShape *t = GETMEM (sizeof(bhkConvexShape));
	if (STOP) return;
	readbhkConvexShape (t, 0);
}
static void
RPbhkSphereShape()
{
	bhkSphereShape *t = GETMEM (sizeof(bhkSphereShape));
	if (STOP) return;
	readbhkSphereShape (t, 0);
}
static void
RPbhkCapsuleShape()
{
	bhkCapsuleShape *t = GETMEM (sizeof(bhkCapsuleShape));
	if (STOP) return;
	readbhkCapsuleShape (t, 0);
}
static void
RPbhkBoxShape()
{
	bhkBoxShape *t = GETMEM (sizeof(bhkBoxShape));
	if (STOP) return;
	readbhkBoxShape (t, 0);
}
static void
RPbhkConvexVerticesShape()
{
	bhkConvexVerticesShape *t = GETMEM (sizeof(bhkConvexVerticesShape));
	if (STOP) return;
	readbhkConvexVerticesShape (t, 0);
}
static void
RPbhkConvexTransformShape()
{
	bhkConvexTransformShape *t = GETMEM (sizeof(bhkConvexTransformShape));
	if (STOP) return;
	readbhkConvexTransformShape (t, 0);
}
static void
RPbhkMultiSphereShape()
{
	bhkMultiSphereShape *t = GETMEM (sizeof(bhkMultiSphereShape));
	if (STOP) return;
	readbhkMultiSphereShape (t, 0);
}
static void
RPbhkBvTreeShape()
{
	bhkBvTreeShape *t = GETMEM (sizeof(bhkBvTreeShape));
	if (STOP) return;
	readbhkBvTreeShape (t, 0);
}
static void
RPbhkMoppBvTreeShape()
{
	bhkMoppBvTreeShape *t = GETMEM (sizeof(bhkMoppBvTreeShape));
	if (STOP) return;
	readbhkMoppBvTreeShape (t, 0);
}
static void
RPbhkShapeCollection()
{
	bhkShapeCollection *t = GETMEM (sizeof(bhkShapeCollection));
	if (STOP) return;
	readbhkShapeCollection (t, 0);
}
static void
RPbhkListShape()
{
	bhkListShape *t = GETMEM (sizeof(bhkListShape));
	if (STOP) return;
	readbhkListShape (t, 0);
}
static void
RPbhkMeshShape()
{
	bhkMeshShape *t = GETMEM (sizeof(bhkMeshShape));
	if (STOP) return;
	readbhkMeshShape (t, 0);
}
static void
RPbhkPackedNiTriStripsShape()
{
	bhkPackedNiTriStripsShape *t = GETMEM (sizeof(bhkPackedNiTriStripsShape));
	if (STOP) return;
	readbhkPackedNiTriStripsShape (t, 0);
}
static void
RPbhkNiTriStripsShape()
{
	bhkNiTriStripsShape *t = GETMEM (sizeof(bhkNiTriStripsShape));
	if (STOP) return;
	readbhkNiTriStripsShape (t, 0);
}
static void
RPNiExtraData()
{
	NiExtraData *t = GETMEM (sizeof(NiExtraData));
	if (STOP) return;
	readNiExtraData (t, 0);
}
static void
RPNiInterpolator()
{
	NiInterpolator *t = GETMEM (sizeof(NiInterpolator));
	if (STOP) return;
	readNiInterpolator (t, 0);
}
static void
RPNiKeyBasedInterpolator()
{
	NiKeyBasedInterpolator *t = GETMEM (sizeof(NiKeyBasedInterpolator));
	if (STOP) return;
	readNiKeyBasedInterpolator (t, 0);
}
static void
RPNiFloatInterpolator()
{
	NiFloatInterpolator *t = GETMEM (sizeof(NiFloatInterpolator));
	if (STOP) return;
	readNiFloatInterpolator (t, 0);
}
static void
RPNiTransformInterpolator()
{
	NiTransformInterpolator *t = GETMEM (sizeof(NiTransformInterpolator));
	if (STOP) return;
	readNiTransformInterpolator (t, 0);
}
static void
RPNiPoint3Interpolator()
{
	NiPoint3Interpolator *t = GETMEM (sizeof(NiPoint3Interpolator));
	if (STOP) return;
	readNiPoint3Interpolator (t, 0);
}
static void
RPNiPathInterpolator()
{
	NiPathInterpolator *t = GETMEM (sizeof(NiPathInterpolator));
	if (STOP) return;
	readNiPathInterpolator (t, 0);
}
static void
RPNiBoolInterpolator()
{
	NiBoolInterpolator *t = GETMEM (sizeof(NiBoolInterpolator));
	if (STOP) return;
	readNiBoolInterpolator (t, 0);
}
static void
RPNiBoolTimelineInterpolator()
{
	NiBoolTimelineInterpolator *t = GETMEM (sizeof(NiBoolTimelineInterpolator));
	if (STOP) return;
	readNiBoolTimelineInterpolator (t, 0);
}
static void
RPNiBlendInterpolator()
{
	NiBlendInterpolator *t = GETMEM (sizeof(NiBlendInterpolator));
	if (STOP) return;
	readNiBlendInterpolator (t, 0);
}
static void
RPNiBSplineInterpolator()
{
	NiBSplineInterpolator *t = GETMEM (sizeof(NiBSplineInterpolator));
	if (STOP) return;
	readNiBSplineInterpolator (t, 0);
}
static void
RPNiObjectNET()
{
	NiObjectNET *t = GETMEM (sizeof(NiObjectNET));
	if (STOP) return;
	readNiObjectNET (t, 0);
}
static void
RPNiCollisionObject()
{
	NiCollisionObject *t = GETMEM (sizeof(NiCollisionObject));
	if (STOP) return;
	readNiCollisionObject (t, 0);
}
static void
RPNiCollisionData()
{
	NiCollisionData *t = GETMEM (sizeof(NiCollisionData));
	if (STOP) return;
	readNiCollisionData (t, 0);
}
static void
RPbhkNiCollisionObject()
{
	bhkNiCollisionObject *t = GETMEM (sizeof(bhkNiCollisionObject));
	if (STOP) return;
	readbhkNiCollisionObject (t, 0);
}
static void
RPbhkCollisionObject()
{
	bhkCollisionObject *t = GETMEM (sizeof(bhkCollisionObject));
	if (STOP) return;
	readbhkCollisionObject (t, 0);
}
static void
RPbhkBlendCollisionObject()
{
	bhkBlendCollisionObject *t = GETMEM (sizeof(bhkBlendCollisionObject));
	if (STOP) return;
	readbhkBlendCollisionObject (t, 0);
}
static void
RPbhkPCollisionObject()
{
	bhkPCollisionObject *t = GETMEM (sizeof(bhkPCollisionObject));
	if (STOP) return;
	readbhkPCollisionObject (t, 0);
}
static void
RPbhkSPCollisionObject()
{
	bhkSPCollisionObject *t = GETMEM (sizeof(bhkSPCollisionObject));
	if (STOP) return;
	readbhkSPCollisionObject (t, 0);
}
static void
RPNiAVObject()
{
	NiAVObject *t = GETMEM (sizeof(NiAVObject));
	if (STOP) return;
	readNiAVObject (t, 0);
}
static void
RPNiDynamicEffect()
{
	NiDynamicEffect *t = GETMEM (sizeof(NiDynamicEffect));
	if (STOP) return;
	readNiDynamicEffect (t, 0);
}
static void
RPNiLight()
{
	NiLight *t = GETMEM (sizeof(NiLight));
	if (STOP) return;
	readNiLight (t, 0);
}
static void
RPNiProperty()
{
	NiProperty *t = GETMEM (sizeof(NiProperty));
	if (STOP) return;
	readNiProperty (t, 0);
}
static void
RPNiTransparentProperty()
{
	NiTransparentProperty *t = GETMEM (sizeof(NiTransparentProperty));
	if (STOP) return;
	readNiTransparentProperty (t, 0);
}
static void
RPNiPSysModifier()
{
	NiPSysModifier *t = GETMEM (sizeof(NiPSysModifier));
	if (STOP) return;
	readNiPSysModifier (t, 0);
}
static void
RPNiPSysEmitter()
{
	NiPSysEmitter *t = GETMEM (sizeof(NiPSysEmitter));
	if (STOP) return;
	readNiPSysEmitter (t, 0);
}
static void
RPNiPSysVolumeEmitter()
{
	NiPSysVolumeEmitter *t = GETMEM (sizeof(NiPSysVolumeEmitter));
	if (STOP) return;
	readNiPSysVolumeEmitter (t, 0);
}
static void
RPNiTimeController()
{
	NiTimeController *t = GETMEM (sizeof(NiTimeController));
	if (STOP) return;
	readNiTimeController (t, 0);
}
static void
RPNiInterpController()
{
	NiInterpController *t = GETMEM (sizeof(NiInterpController));
	if (STOP) return;
	readNiInterpController (t, 0);
}
static void
RPNiMultiTargetTransformController()
{
	NiMultiTargetTransformController *t = GETMEM (sizeof(NiMultiTargetTransformController));
	if (STOP) return;
	readNiMultiTargetTransformController (t, 0);
}
static void
RPNiGeomMorpherController()
{
	NiGeomMorpherController *t = GETMEM (sizeof(NiGeomMorpherController));
	if (STOP) return;
	readNiGeomMorpherController (t, 0);
}
static void
RPNiMorphController()
{
	NiMorphController *t = GETMEM (sizeof(NiMorphController));
	if (STOP) return;
	readNiMorphController (t, 0);
}
static void
RPNiMorpherController()
{
	NiMorpherController *t = GETMEM (sizeof(NiMorpherController));
	if (STOP) return;
	readNiMorpherController (t, 0);
}
static void
RPNiSingleInterpController()
{
	NiSingleInterpController *t = GETMEM (sizeof(NiSingleInterpController));
	if (STOP) return;
	readNiSingleInterpController (t, 0);
}
static void
RPNiKeyframeController()
{
	NiKeyframeController *t = GETMEM (sizeof(NiKeyframeController));
	if (STOP) return;
	readNiKeyframeController (t, 0);
}
static void
RPNiTransformController()
{
	NiTransformController *t = GETMEM (sizeof(NiTransformController));
	if (STOP) return;
	readNiTransformController (t, 0);
}
static void
RPNiPSysModifierCtlr()
{
	NiPSysModifierCtlr *t = GETMEM (sizeof(NiPSysModifierCtlr));
	if (STOP) return;
	readNiPSysModifierCtlr (t, 0);
}
static void
RPNiPSysEmitterCtlr()
{
	NiPSysEmitterCtlr *t = GETMEM (sizeof(NiPSysEmitterCtlr));
	if (STOP) return;
	readNiPSysEmitterCtlr (t, 0);
}
static void
RPNiPSysModifierBoolCtlr()
{
	NiPSysModifierBoolCtlr *t = GETMEM (sizeof(NiPSysModifierBoolCtlr));
	if (STOP) return;
	readNiPSysModifierBoolCtlr (t, 0);
}
static void
RPNiPSysModifierActiveCtlr()
{
	NiPSysModifierActiveCtlr *t = GETMEM (sizeof(NiPSysModifierActiveCtlr));
	if (STOP) return;
	readNiPSysModifierActiveCtlr (t, 0);
}
static void
RPNiPSysModifierFloatCtlr()
{
	NiPSysModifierFloatCtlr *t = GETMEM (sizeof(NiPSysModifierFloatCtlr));
	if (STOP) return;
	readNiPSysModifierFloatCtlr (t, 0);
}
static void
RPNiPSysEmitterDeclinationCtlr()
{
	NiPSysEmitterDeclinationCtlr *t = GETMEM (sizeof(NiPSysEmitterDeclinationCtlr));
	if (STOP) return;
	readNiPSysEmitterDeclinationCtlr (t, 0);
}
static void
RPNiPSysEmitterDeclinationVarCtlr()
{
	NiPSysEmitterDeclinationVarCtlr *t = GETMEM (sizeof(NiPSysEmitterDeclinationVarCtlr));
	if (STOP) return;
	readNiPSysEmitterDeclinationVarCtlr (t, 0);
}
static void
RPNiPSysEmitterInitialRadiusCtlr()
{
	NiPSysEmitterInitialRadiusCtlr *t = GETMEM (sizeof(NiPSysEmitterInitialRadiusCtlr));
	if (STOP) return;
	readNiPSysEmitterInitialRadiusCtlr (t, 0);
}
static void
RPNiPSysEmitterLifeSpanCtlr()
{
	NiPSysEmitterLifeSpanCtlr *t = GETMEM (sizeof(NiPSysEmitterLifeSpanCtlr));
	if (STOP) return;
	readNiPSysEmitterLifeSpanCtlr (t, 0);
}
static void
RPNiPSysEmitterSpeedCtlr()
{
	NiPSysEmitterSpeedCtlr *t = GETMEM (sizeof(NiPSysEmitterSpeedCtlr));
	if (STOP) return;
	readNiPSysEmitterSpeedCtlr (t, 0);
}
static void
RPNiPSysGravityStrengthCtlr()
{
	NiPSysGravityStrengthCtlr *t = GETMEM (sizeof(NiPSysGravityStrengthCtlr));
	if (STOP) return;
	readNiPSysGravityStrengthCtlr (t, 0);
}
static void
RPNiFloatInterpController()
{
	NiFloatInterpController *t = GETMEM (sizeof(NiFloatInterpController));
	if (STOP) return;
	readNiFloatInterpController (t, 0);
}
static void
RPNiFlipController()
{
	NiFlipController *t = GETMEM (sizeof(NiFlipController));
	if (STOP) return;
	readNiFlipController (t, 0);
}
static void
RPNiAlphaController()
{
	NiAlphaController *t = GETMEM (sizeof(NiAlphaController));
	if (STOP) return;
	readNiAlphaController (t, 0);
}
static void
RPNiTextureTransformController()
{
	NiTextureTransformController *t = GETMEM (sizeof(NiTextureTransformController));
	if (STOP) return;
	readNiTextureTransformController (t, 0);
}
static void
RPNiLightDimmerController()
{
	NiLightDimmerController *t = GETMEM (sizeof(NiLightDimmerController));
	if (STOP) return;
	readNiLightDimmerController (t, 0);
}
static void
RPNiBoolInterpController()
{
	NiBoolInterpController *t = GETMEM (sizeof(NiBoolInterpController));
	if (STOP) return;
	readNiBoolInterpController (t, 0);
}
static void
RPNiVisController()
{
	NiVisController *t = GETMEM (sizeof(NiVisController));
	if (STOP) return;
	readNiVisController (t, 0);
}
static void
RPNiPoint3InterpController()
{
	NiPoint3InterpController *t = GETMEM (sizeof(NiPoint3InterpController));
	if (STOP) return;
	readNiPoint3InterpController (t, 0);
}
static void
RPNiMaterialColorController()
{
	NiMaterialColorController *t = GETMEM (sizeof(NiMaterialColorController));
	if (STOP) return;
	readNiMaterialColorController (t, 0);
}
static void
RPNiLightColorController()
{
	NiLightColorController *t = GETMEM (sizeof(NiLightColorController));
	if (STOP) return;
	readNiLightColorController (t, 0);
}
static void
RPNiExtraDataController()
{
	NiExtraDataController *t = GETMEM (sizeof(NiExtraDataController));
	if (STOP) return;
	readNiExtraDataController (t, 0);
}
static void
RPNiFloatExtraDataController()
{
	NiFloatExtraDataController *t = GETMEM (sizeof(NiFloatExtraDataController));
	if (STOP) return;
	readNiFloatExtraDataController (t, 0);
}
static void
RPNiBoneLODController()
{
	NiBoneLODController *t = GETMEM (sizeof(NiBoneLODController));
	if (STOP) return;
	readNiBoneLODController (t, 0);
}
static void
RPNiBSBoneLODController()
{
	NiBSBoneLODController *t = GETMEM (sizeof(NiBSBoneLODController));
	if (STOP) return;
	readNiBSBoneLODController (t, 0);
}
static void
RPNiGeometry()
{
	NiGeometry *t = GETMEM (sizeof(NiGeometry));
	if (STOP) return;
	readNiGeometry (t, 0);
}
static void
RPNiTriBasedGeom()
{
	NiTriBasedGeom *t = GETMEM (sizeof(NiTriBasedGeom));
	if (STOP) return;
	readNiTriBasedGeom (t, 0);
}
static void
RPNiGeometryData()
{
	NiGeometryData *t = GETMEM (sizeof(NiGeometryData));
	if (STOP) return;
	readNiGeometryData (t, 0);
}
static void
RPNiTriBasedGeomData()
{
	NiTriBasedGeomData *t = GETMEM (sizeof(NiTriBasedGeomData));
	if (STOP) return;
	readNiTriBasedGeomData (t, 0);
}
static void
RPbhkBlendController()
{
	bhkBlendController *t = GETMEM (sizeof(bhkBlendController));
	if (STOP) return;
	readbhkBlendController (t, 0);
}
static void
RPBSBound()
{
	BSBound *t = GETMEM (sizeof(BSBound));
	if (STOP) return;
	readBSBound (t, 0);
}
static void
RPBSFurnitureMarker()
{
	BSFurnitureMarker *t = GETMEM (sizeof(BSFurnitureMarker));
	if (STOP) return;
	readBSFurnitureMarker (t, 0);
}
static void
RPBSParentVelocityModifier()
{
	BSParentVelocityModifier *t = GETMEM (sizeof(BSParentVelocityModifier));
	if (STOP) return;
	readBSParentVelocityModifier (t, 0);
}
static void
RPBSPSysArrayEmitter()
{
	BSPSysArrayEmitter *t = GETMEM (sizeof(BSPSysArrayEmitter));
	if (STOP) return;
	readBSPSysArrayEmitter (t, 0);
}
static void
RPBSWindModifier()
{
	BSWindModifier *t = GETMEM (sizeof(BSWindModifier));
	if (STOP) return;
	readBSWindModifier (t, 0);
}
static void
RPhkPackedNiTriStripsData()
{
	hkPackedNiTriStripsData *t = GETMEM (sizeof(hkPackedNiTriStripsData));
	if (STOP) return;
	readhkPackedNiTriStripsData (t, 0);
}
static void
RPNiAlphaProperty()
{
	NiAlphaProperty *t = GETMEM (sizeof(NiAlphaProperty));
	if (STOP) return;
	readNiAlphaProperty (t, 0);
}
static void
RPNiAmbientLight()
{
	NiAmbientLight *t = GETMEM (sizeof(NiAmbientLight));
	if (STOP) return;
	readNiAmbientLight (t, 0);
}
static void
RPNiParticlesData()
{
	NiParticlesData *t = GETMEM (sizeof(NiParticlesData));
	if (STOP) return;
	readNiParticlesData (t, 0);
}
static void
RPNiRotatingParticlesData()
{
	NiRotatingParticlesData *t = GETMEM (sizeof(NiRotatingParticlesData));
	if (STOP) return;
	readNiRotatingParticlesData (t, 0);
}
static void
RPNiAutoNormalParticlesData()
{
	NiAutoNormalParticlesData *t = GETMEM (sizeof(NiAutoNormalParticlesData));
	if (STOP) return;
	readNiAutoNormalParticlesData (t, 0);
}
static void
RPParticleDesc()
{
	ParticleDesc *t = GETMEM (sizeof(ParticleDesc));
	if (STOP) return;
	readParticleDesc (t, 0);
}
static void
RPNiPSysData()
{
	NiPSysData *t = GETMEM (sizeof(NiPSysData));
	if (STOP) return;
	readNiPSysData (t, 0);
}
static void
RPNiMeshPSysData()
{
	NiMeshPSysData *t = GETMEM (sizeof(NiMeshPSysData));
	if (STOP) return;
	readNiMeshPSysData (t, 0);
}
static void
RPNiBinaryExtraData()
{
	NiBinaryExtraData *t = GETMEM (sizeof(NiBinaryExtraData));
	if (STOP) return;
	readNiBinaryExtraData (t, 0);
}
static void
RPNiBinaryVoxelExtraData()
{
	NiBinaryVoxelExtraData *t = GETMEM (sizeof(NiBinaryVoxelExtraData));
	if (STOP) return;
	readNiBinaryVoxelExtraData (t, 0);
}
static void
RPNiBinaryVoxelData()
{
	NiBinaryVoxelData *t = GETMEM (sizeof(NiBinaryVoxelData));
	if (STOP) return;
	readNiBinaryVoxelData (t, 0);
}
static void
RPNiBlendBoolInterpolator()
{
	NiBlendBoolInterpolator *t = GETMEM (sizeof(NiBlendBoolInterpolator));
	if (STOP) return;
	readNiBlendBoolInterpolator (t, 0);
}
static void
RPNiBlendFloatInterpolator()
{
	NiBlendFloatInterpolator *t = GETMEM (sizeof(NiBlendFloatInterpolator));
	if (STOP) return;
	readNiBlendFloatInterpolator (t, 0);
}
static void
RPNiBlendPoint3Interpolator()
{
	NiBlendPoint3Interpolator *t = GETMEM (sizeof(NiBlendPoint3Interpolator));
	if (STOP) return;
	readNiBlendPoint3Interpolator (t, 0);
}
static void
RPNiBlendTransformInterpolator()
{
	NiBlendTransformInterpolator *t = GETMEM (sizeof(NiBlendTransformInterpolator));
	if (STOP) return;
	readNiBlendTransformInterpolator (t, 0);
}
static void
RPNiBoolData()
{
	NiBoolData *t = GETMEM (sizeof(NiBoolData));
	if (STOP) return;
	readNiBoolData (t, 0);
}
static void
RPNiBooleanExtraData()
{
	NiBooleanExtraData *t = GETMEM (sizeof(NiBooleanExtraData));
	if (STOP) return;
	readNiBooleanExtraData (t, 0);
}
static void
RPNiBSplineBasisData()
{
	NiBSplineBasisData *t = GETMEM (sizeof(NiBSplineBasisData));
	if (STOP) return;
	readNiBSplineBasisData (t, 0);
}
static void
RPNiBSplineFloatInterpolator()
{
	NiBSplineFloatInterpolator *t = GETMEM (sizeof(NiBSplineFloatInterpolator));
	if (STOP) return;
	readNiBSplineFloatInterpolator (t, 0);
}
static void
RPNiBSplineCompFloatInterpolator()
{
	NiBSplineCompFloatInterpolator *t = GETMEM (sizeof(NiBSplineCompFloatInterpolator));
	if (STOP) return;
	readNiBSplineCompFloatInterpolator (t, 0);
}
static void
RPNiBSplinePoint3Interpolator()
{
	NiBSplinePoint3Interpolator *t = GETMEM (sizeof(NiBSplinePoint3Interpolator));
	if (STOP) return;
	readNiBSplinePoint3Interpolator (t, 0);
}
static void
RPNiBSplineCompPoint3Interpolator()
{
	NiBSplineCompPoint3Interpolator *t = GETMEM (sizeof(NiBSplineCompPoint3Interpolator));
	if (STOP) return;
	readNiBSplineCompPoint3Interpolator (t, 0);
}
static void
RPNiBSplineTransformInterpolator()
{
	NiBSplineTransformInterpolator *t = GETMEM (sizeof(NiBSplineTransformInterpolator));
	if (STOP) return;
	readNiBSplineTransformInterpolator (t, 0);
}
static void
RPNiBSplineCompTransformInterpolator()
{
	NiBSplineCompTransformInterpolator *t = GETMEM (sizeof(NiBSplineCompTransformInterpolator));
	if (STOP) return;
	readNiBSplineCompTransformInterpolator (t, 0);
}
static void
RPBSRotAccumTransfInterpolator()
{
	BSRotAccumTransfInterpolator *t = GETMEM (sizeof(BSRotAccumTransfInterpolator));
	if (STOP) return;
	readBSRotAccumTransfInterpolator (t, 0);
}
static void
RPNiBSplineData()
{
	NiBSplineData *t = GETMEM (sizeof(NiBSplineData));
	if (STOP) return;
	readNiBSplineData (t, 0);
}
static void
RPNiCamera()
{
	NiCamera *t = GETMEM (sizeof(NiCamera));
	if (STOP) return;
	readNiCamera (t, 0);
}
static void
RPNiColorData()
{
	NiColorData *t = GETMEM (sizeof(NiColorData));
	if (STOP) return;
	readNiColorData (t, 0);
}
static void
RPNiColorExtraData()
{
	NiColorExtraData *t = GETMEM (sizeof(NiColorExtraData));
	if (STOP) return;
	readNiColorExtraData (t, 0);
}
static void
RPNiControllerManager()
{
	NiControllerManager *t = GETMEM (sizeof(NiControllerManager));
	if (STOP) return;
	readNiControllerManager (t, 0);
}
static void
RPNiSequence()
{
	NiSequence *t = GETMEM (sizeof(NiSequence));
	if (STOP) return;
	readNiSequence (t, 0);
}
static void
RPNiControllerSequence()
{
	NiControllerSequence *t = GETMEM (sizeof(NiControllerSequence));
	if (STOP) return;
	readNiControllerSequence (t, 0);
}
static void
RPNiAVObjectPalette()
{
	NiAVObjectPalette *t = GETMEM (sizeof(NiAVObjectPalette));
	if (STOP) return;
	readNiAVObjectPalette (t, 0);
}
static void
RPNiDefaultAVObjectPalette()
{
	NiDefaultAVObjectPalette *t = GETMEM (sizeof(NiDefaultAVObjectPalette));
	if (STOP) return;
	readNiDefaultAVObjectPalette (t, 0);
}
static void
RPNiDirectionalLight()
{
	NiDirectionalLight *t = GETMEM (sizeof(NiDirectionalLight));
	if (STOP) return;
	readNiDirectionalLight (t, 0);
}
static void
RPNiDitherProperty()
{
	NiDitherProperty *t = GETMEM (sizeof(NiDitherProperty));
	if (STOP) return;
	readNiDitherProperty (t, 0);
}
static void
RPNiRollController()
{
	NiRollController *t = GETMEM (sizeof(NiRollController));
	if (STOP) return;
	readNiRollController (t, 0);
}
static void
RPNiFloatData()
{
	NiFloatData *t = GETMEM (sizeof(NiFloatData));
	if (STOP) return;
	readNiFloatData (t, 0);
}
static void
RPNiFloatExtraData()
{
	NiFloatExtraData *t = GETMEM (sizeof(NiFloatExtraData));
	if (STOP) return;
	readNiFloatExtraData (t, 0);
}
static void
RPNiFloatsExtraData()
{
	NiFloatsExtraData *t = GETMEM (sizeof(NiFloatsExtraData));
	if (STOP) return;
	readNiFloatsExtraData (t, 0);
}
static void
RPNiFogProperty()
{
	NiFogProperty *t = GETMEM (sizeof(NiFogProperty));
	if (STOP) return;
	readNiFogProperty (t, 0);
}
static void
RPNiGravity()
{
	NiGravity *t = GETMEM (sizeof(NiGravity));
	if (STOP) return;
	readNiGravity (t, 0);
}
static void
RPNiIntegerExtraData()
{
	NiIntegerExtraData *t = GETMEM (sizeof(NiIntegerExtraData));
	if (STOP) return;
	readNiIntegerExtraData (t, 0);
}
static void
RPBSXFlags()
{
	BSXFlags *t = GETMEM (sizeof(BSXFlags));
	if (STOP) return;
	readBSXFlags (t, 0);
}
static void
RPNiIntegersExtraData()
{
	NiIntegersExtraData *t = GETMEM (sizeof(NiIntegersExtraData));
	if (STOP) return;
	readNiIntegersExtraData (t, 0);
}
static void
RPBSKeyframeController()
{
	BSKeyframeController *t = GETMEM (sizeof(BSKeyframeController));
	if (STOP) return;
	readBSKeyframeController (t, 0);
}
static void
RPNiKeyframeData()
{
	NiKeyframeData *t = GETMEM (sizeof(NiKeyframeData));
	if (STOP) return;
	readNiKeyframeData (t, 0);
}
static void
RPNiLookAtController()
{
	NiLookAtController *t = GETMEM (sizeof(NiLookAtController));
	if (STOP) return;
	readNiLookAtController (t, 0);
}
static void
RPNiLookAtInterpolator()
{
	NiLookAtInterpolator *t = GETMEM (sizeof(NiLookAtInterpolator));
	if (STOP) return;
	readNiLookAtInterpolator (t, 0);
}
static void
RPNiMaterialProperty()
{
	NiMaterialProperty *t = GETMEM (sizeof(NiMaterialProperty));
	if (STOP) return;
	readNiMaterialProperty (t, 0);
}
static void
RPNiMorphData()
{
	NiMorphData *t = GETMEM (sizeof(NiMorphData));
	if (STOP) return;
	readNiMorphData (t, 0);
}
static void
RPNiNode()
{
	NiNode *t = GETMEM (sizeof(NiNode));
	if (STOP) return;
	readNiNode (t, 0);
}
static void
RPNiBone()
{
	NiBone *t = GETMEM (sizeof(NiBone));
	if (STOP) return;
	readNiBone (t, 0);
}
static void
RPAvoidNode()
{
	AvoidNode *t = GETMEM (sizeof(AvoidNode));
	if (STOP) return;
	readAvoidNode (t, 0);
}
static void
RPFxWidget()
{
	FxWidget *t = GETMEM (sizeof(FxWidget));
	if (STOP) return;
	readFxWidget (t, 0);
}
static void
RPFxButton()
{
	FxButton *t = GETMEM (sizeof(FxButton));
	if (STOP) return;
	readFxButton (t, 0);
}
static void
RPFxRadioButton()
{
	FxRadioButton *t = GETMEM (sizeof(FxRadioButton));
	if (STOP) return;
	readFxRadioButton (t, 0);
}
static void
RPNiBillboardNode()
{
	NiBillboardNode *t = GETMEM (sizeof(NiBillboardNode));
	if (STOP) return;
	readNiBillboardNode (t, 0);
}
static void
RPNiBSAnimationNode()
{
	NiBSAnimationNode *t = GETMEM (sizeof(NiBSAnimationNode));
	if (STOP) return;
	readNiBSAnimationNode (t, 0);
}
static void
RPNiBSParticleNode()
{
	NiBSParticleNode *t = GETMEM (sizeof(NiBSParticleNode));
	if (STOP) return;
	readNiBSParticleNode (t, 0);
}
static void
RPNiSwitchNode()
{
	NiSwitchNode *t = GETMEM (sizeof(NiSwitchNode));
	if (STOP) return;
	readNiSwitchNode (t, 0);
}
static void
RPNiLODNode()
{
	NiLODNode *t = GETMEM (sizeof(NiLODNode));
	if (STOP) return;
	readNiLODNode (t, 0);
}
static void
RPNiPalette()
{
	NiPalette *t = GETMEM (sizeof(NiPalette));
	if (STOP) return;
	readNiPalette (t, 0);
}
static void
RPNiParticleBomb()
{
	NiParticleBomb *t = GETMEM (sizeof(NiParticleBomb));
	if (STOP) return;
	readNiParticleBomb (t, 0);
}
static void
RPNiParticleColorModifier()
{
	NiParticleColorModifier *t = GETMEM (sizeof(NiParticleColorModifier));
	if (STOP) return;
	readNiParticleColorModifier (t, 0);
}
static void
RPNiParticleGrowFade()
{
	NiParticleGrowFade *t = GETMEM (sizeof(NiParticleGrowFade));
	if (STOP) return;
	readNiParticleGrowFade (t, 0);
}
static void
RPNiParticleMeshModifier()
{
	NiParticleMeshModifier *t = GETMEM (sizeof(NiParticleMeshModifier));
	if (STOP) return;
	readNiParticleMeshModifier (t, 0);
}
static void
RPNiParticleRotation()
{
	NiParticleRotation *t = GETMEM (sizeof(NiParticleRotation));
	if (STOP) return;
	readNiParticleRotation (t, 0);
}
static void
RPNiParticles()
{
	NiParticles *t = GETMEM (sizeof(NiParticles));
	if (STOP) return;
	readNiParticles (t, 0);
}
static void
RPNiAutoNormalParticles()
{
	NiAutoNormalParticles *t = GETMEM (sizeof(NiAutoNormalParticles));
	if (STOP) return;
	readNiAutoNormalParticles (t, 0);
}
static void
RPNiParticleMeshes()
{
	NiParticleMeshes *t = GETMEM (sizeof(NiParticleMeshes));
	if (STOP) return;
	readNiParticleMeshes (t, 0);
}
static void
RPNiParticleMeshesData()
{
	NiParticleMeshesData *t = GETMEM (sizeof(NiParticleMeshesData));
	if (STOP) return;
	readNiParticleMeshesData (t, 0);
}
static void
RPNiParticleSystem()
{
	NiParticleSystem *t = GETMEM (sizeof(NiParticleSystem));
	if (STOP) return;
	readNiParticleSystem (t, 0);
}
static void
RPNiMeshParticleSystem()
{
	NiMeshParticleSystem *t = GETMEM (sizeof(NiMeshParticleSystem));
	if (STOP) return;
	readNiMeshParticleSystem (t, 0);
}
static void
RPNiParticleSystemController()
{
	NiParticleSystemController *t = GETMEM (sizeof(NiParticleSystemController));
	if (STOP) return;
	readNiParticleSystemController (t, 0);
}
static void
RPNiBSPArrayController()
{
	NiBSPArrayController *t = GETMEM (sizeof(NiBSPArrayController));
	if (STOP) return;
	readNiBSPArrayController (t, 0);
}
static void
RPNiPathController()
{
	NiPathController *t = GETMEM (sizeof(NiPathController));
	if (STOP) return;
	readNiPathController (t, 0);
}
static void
RPChannelData()
{
	ChannelData *t = GETMEM (sizeof(ChannelData));
	if (STOP) return;
	readChannelData (t, 0);
}
static void
RPATextureRenderData()
{
	ATextureRenderData *t = GETMEM (sizeof(ATextureRenderData));
	if (STOP) return;
	readATextureRenderData (t, 0);
}
static void
RPNiPersistentSrcTextureRendererData()
{
	NiPersistentSrcTextureRendererData *t = GETMEM (sizeof(NiPersistentSrcTextureRendererData));
	if (STOP) return;
	readNiPersistentSrcTextureRendererData (t, 0);
}
static void
RPNiPixelData()
{
	NiPixelData *t = GETMEM (sizeof(NiPixelData));
	if (STOP) return;
	readNiPixelData (t, 0);
}
static void
RPNiPlanarCollider()
{
	NiPlanarCollider *t = GETMEM (sizeof(NiPlanarCollider));
	if (STOP) return;
	readNiPlanarCollider (t, 0);
}
static void
RPNiPointLight()
{
	NiPointLight *t = GETMEM (sizeof(NiPointLight));
	if (STOP) return;
	readNiPointLight (t, 0);
}
static void
RPNiPosData()
{
	NiPosData *t = GETMEM (sizeof(NiPosData));
	if (STOP) return;
	readNiPosData (t, 0);
}
static void
RPNiPSysAgeDeathModifier()
{
	NiPSysAgeDeathModifier *t = GETMEM (sizeof(NiPSysAgeDeathModifier));
	if (STOP) return;
	readNiPSysAgeDeathModifier (t, 0);
}
static void
RPNiPSysBombModifier()
{
	NiPSysBombModifier *t = GETMEM (sizeof(NiPSysBombModifier));
	if (STOP) return;
	readNiPSysBombModifier (t, 0);
}
static void
RPNiPSysBoundUpdateModifier()
{
	NiPSysBoundUpdateModifier *t = GETMEM (sizeof(NiPSysBoundUpdateModifier));
	if (STOP) return;
	readNiPSysBoundUpdateModifier (t, 0);
}
static void
RPNiPSysBoxEmitter()
{
	NiPSysBoxEmitter *t = GETMEM (sizeof(NiPSysBoxEmitter));
	if (STOP) return;
	readNiPSysBoxEmitter (t, 0);
}
static void
RPNiPSysColliderManager()
{
	NiPSysColliderManager *t = GETMEM (sizeof(NiPSysColliderManager));
	if (STOP) return;
	readNiPSysColliderManager (t, 0);
}
static void
RPNiPSysColorModifier()
{
	NiPSysColorModifier *t = GETMEM (sizeof(NiPSysColorModifier));
	if (STOP) return;
	readNiPSysColorModifier (t, 0);
}
static void
RPNiPSysCylinderEmitter()
{
	NiPSysCylinderEmitter *t = GETMEM (sizeof(NiPSysCylinderEmitter));
	if (STOP) return;
	readNiPSysCylinderEmitter (t, 0);
}
static void
RPNiPSysDragModifier()
{
	NiPSysDragModifier *t = GETMEM (sizeof(NiPSysDragModifier));
	if (STOP) return;
	readNiPSysDragModifier (t, 0);
}
static void
RPNiPSysEmitterCtlrData()
{
	NiPSysEmitterCtlrData *t = GETMEM (sizeof(NiPSysEmitterCtlrData));
	if (STOP) return;
	readNiPSysEmitterCtlrData (t, 0);
}
static void
RPNiPSysGravityModifier()
{
	NiPSysGravityModifier *t = GETMEM (sizeof(NiPSysGravityModifier));
	if (STOP) return;
	readNiPSysGravityModifier (t, 0);
}
static void
RPNiPSysGrowFadeModifier()
{
	NiPSysGrowFadeModifier *t = GETMEM (sizeof(NiPSysGrowFadeModifier));
	if (STOP) return;
	readNiPSysGrowFadeModifier (t, 0);
}
static void
RPNiPSysMeshEmitter()
{
	NiPSysMeshEmitter *t = GETMEM (sizeof(NiPSysMeshEmitter));
	if (STOP) return;
	readNiPSysMeshEmitter (t, 0);
}
static void
RPNiPSysMeshUpdateModifier()
{
	NiPSysMeshUpdateModifier *t = GETMEM (sizeof(NiPSysMeshUpdateModifier));
	if (STOP) return;
	readNiPSysMeshUpdateModifier (t, 0);
}
static void
RPNiPSysPlanarCollider()
{
	NiPSysPlanarCollider *t = GETMEM (sizeof(NiPSysPlanarCollider));
	if (STOP) return;
	readNiPSysPlanarCollider (t, 0);
}
static void
RPNiPSysSphericalCollider()
{
	NiPSysSphericalCollider *t = GETMEM (sizeof(NiPSysSphericalCollider));
	if (STOP) return;
	readNiPSysSphericalCollider (t, 0);
}
static void
RPNiPSysPositionModifier()
{
	NiPSysPositionModifier *t = GETMEM (sizeof(NiPSysPositionModifier));
	if (STOP) return;
	readNiPSysPositionModifier (t, 0);
}
static void
RPNiPSysResetOnLoopCtlr()
{
	NiPSysResetOnLoopCtlr *t = GETMEM (sizeof(NiPSysResetOnLoopCtlr));
	if (STOP) return;
	readNiPSysResetOnLoopCtlr (t, 0);
}
static void
RPNiPSysRotationModifier()
{
	NiPSysRotationModifier *t = GETMEM (sizeof(NiPSysRotationModifier));
	if (STOP) return;
	readNiPSysRotationModifier (t, 0);
}
static void
RPNiPSysSpawnModifier()
{
	NiPSysSpawnModifier *t = GETMEM (sizeof(NiPSysSpawnModifier));
	if (STOP) return;
	readNiPSysSpawnModifier (t, 0);
}
static void
RPNiPSysSphereEmitter()
{
	NiPSysSphereEmitter *t = GETMEM (sizeof(NiPSysSphereEmitter));
	if (STOP) return;
	readNiPSysSphereEmitter (t, 0);
}
static void
RPNiPSysUpdateCtlr()
{
	NiPSysUpdateCtlr *t = GETMEM (sizeof(NiPSysUpdateCtlr));
	if (STOP) return;
	readNiPSysUpdateCtlr (t, 0);
}
static void
RPNiPSysFieldModifier()
{
	NiPSysFieldModifier *t = GETMEM (sizeof(NiPSysFieldModifier));
	if (STOP) return;
	readNiPSysFieldModifier (t, 0);
}
static void
RPNiPSysVortexFieldModifier()
{
	NiPSysVortexFieldModifier *t = GETMEM (sizeof(NiPSysVortexFieldModifier));
	if (STOP) return;
	readNiPSysVortexFieldModifier (t, 0);
}
static void
RPNiPSysGravityFieldModifier()
{
	NiPSysGravityFieldModifier *t = GETMEM (sizeof(NiPSysGravityFieldModifier));
	if (STOP) return;
	readNiPSysGravityFieldModifier (t, 0);
}
static void
RPNiPSysDragFieldModifier()
{
	NiPSysDragFieldModifier *t = GETMEM (sizeof(NiPSysDragFieldModifier));
	if (STOP) return;
	readNiPSysDragFieldModifier (t, 0);
}
static void
RPNiPSysTurbulenceFieldModifier()
{
	NiPSysTurbulenceFieldModifier *t = GETMEM (sizeof(NiPSysTurbulenceFieldModifier));
	if (STOP) return;
	readNiPSysTurbulenceFieldModifier (t, 0);
}
static void
RPNiPSysFieldMagnitudeCtlr()
{
	NiPSysFieldMagnitudeCtlr *t = GETMEM (sizeof(NiPSysFieldMagnitudeCtlr));
	if (STOP) return;
	readNiPSysFieldMagnitudeCtlr (t, 0);
}
static void
RPNiPSysFieldAttenuationCtlr()
{
	NiPSysFieldAttenuationCtlr *t = GETMEM (sizeof(NiPSysFieldAttenuationCtlr));
	if (STOP) return;
	readNiPSysFieldAttenuationCtlr (t, 0);
}
static void
RPNiPSysFieldMaxDistanceCtlr()
{
	NiPSysFieldMaxDistanceCtlr *t = GETMEM (sizeof(NiPSysFieldMaxDistanceCtlr));
	if (STOP) return;
	readNiPSysFieldMaxDistanceCtlr (t, 0);
}
static void
RPNiPSysAirFieldAirFrictionCtlr()
{
	NiPSysAirFieldAirFrictionCtlr *t = GETMEM (sizeof(NiPSysAirFieldAirFrictionCtlr));
	if (STOP) return;
	readNiPSysAirFieldAirFrictionCtlr (t, 0);
}
static void
RPNiPSysAirFieldInheritVelocityCtlr()
{
	NiPSysAirFieldInheritVelocityCtlr *t = GETMEM (sizeof(NiPSysAirFieldInheritVelocityCtlr));
	if (STOP) return;
	readNiPSysAirFieldInheritVelocityCtlr (t, 0);
}
static void
RPNiPSysAirFieldSpreadCtlr()
{
	NiPSysAirFieldSpreadCtlr *t = GETMEM (sizeof(NiPSysAirFieldSpreadCtlr));
	if (STOP) return;
	readNiPSysAirFieldSpreadCtlr (t, 0);
}
static void
RPNiPSysInitialRotSpeedCtlr()
{
	NiPSysInitialRotSpeedCtlr *t = GETMEM (sizeof(NiPSysInitialRotSpeedCtlr));
	if (STOP) return;
	readNiPSysInitialRotSpeedCtlr (t, 0);
}
static void
RPNiPSysInitialRotSpeedVarCtlr()
{
	NiPSysInitialRotSpeedVarCtlr *t = GETMEM (sizeof(NiPSysInitialRotSpeedVarCtlr));
	if (STOP) return;
	readNiPSysInitialRotSpeedVarCtlr (t, 0);
}
static void
RPNiPSysInitialRotAngleCtlr()
{
	NiPSysInitialRotAngleCtlr *t = GETMEM (sizeof(NiPSysInitialRotAngleCtlr));
	if (STOP) return;
	readNiPSysInitialRotAngleCtlr (t, 0);
}
static void
RPNiPSysInitialRotAngleVarCtlr()
{
	NiPSysInitialRotAngleVarCtlr *t = GETMEM (sizeof(NiPSysInitialRotAngleVarCtlr));
	if (STOP) return;
	readNiPSysInitialRotAngleVarCtlr (t, 0);
}
static void
RPNiPSysEmitterPlanarAngleCtlr()
{
	NiPSysEmitterPlanarAngleCtlr *t = GETMEM (sizeof(NiPSysEmitterPlanarAngleCtlr));
	if (STOP) return;
	readNiPSysEmitterPlanarAngleCtlr (t, 0);
}
static void
RPNiPSysEmitterPlanarAngleVarCtlr()
{
	NiPSysEmitterPlanarAngleVarCtlr *t = GETMEM (sizeof(NiPSysEmitterPlanarAngleVarCtlr));
	if (STOP) return;
	readNiPSysEmitterPlanarAngleVarCtlr (t, 0);
}
static void
RPNiPSysAirFieldModifier()
{
	NiPSysAirFieldModifier *t = GETMEM (sizeof(NiPSysAirFieldModifier));
	if (STOP) return;
	readNiPSysAirFieldModifier (t, 0);
}
static void
RPNiPSysTrailEmitter()
{
	NiPSysTrailEmitter *t = GETMEM (sizeof(NiPSysTrailEmitter));
	if (STOP) return;
	readNiPSysTrailEmitter (t, 0);
}
static void
RPNiLightIntensityController()
{
	NiLightIntensityController *t = GETMEM (sizeof(NiLightIntensityController));
	if (STOP) return;
	readNiLightIntensityController (t, 0);
}
static void
RPNiPSysRadialFieldModifier()
{
	NiPSysRadialFieldModifier *t = GETMEM (sizeof(NiPSysRadialFieldModifier));
	if (STOP) return;
	readNiPSysRadialFieldModifier (t, 0);
}
static void
RPNiLODData()
{
	NiLODData *t = GETMEM (sizeof(NiLODData));
	if (STOP) return;
	readNiLODData (t, 0);
}
static void
RPNiRangeLODData()
{
	NiRangeLODData *t = GETMEM (sizeof(NiRangeLODData));
	if (STOP) return;
	readNiRangeLODData (t, 0);
}
static void
RPNiScreenLODData()
{
	NiScreenLODData *t = GETMEM (sizeof(NiScreenLODData));
	if (STOP) return;
	readNiScreenLODData (t, 0);
}
static void
RPNiRotatingParticles()
{
	NiRotatingParticles *t = GETMEM (sizeof(NiRotatingParticles));
	if (STOP) return;
	readNiRotatingParticles (t, 0);
}
static void
RPNiSequenceStreamHelper()
{
	NiSequenceStreamHelper *t = GETMEM (sizeof(NiSequenceStreamHelper));
	if (STOP) return;
	readNiSequenceStreamHelper (t, 0);
}
static void
RPNiShadeProperty()
{
	NiShadeProperty *t = GETMEM (sizeof(NiShadeProperty));
	if (STOP) return;
	readNiShadeProperty (t, 0);
}
static void
RPNiSkinData()
{
	NiSkinData *t = GETMEM (sizeof(NiSkinData));
	if (STOP) return;
	readNiSkinData (t, 0);
}
static void
RPNiSkinInstance()
{
	NiSkinInstance *t = GETMEM (sizeof(NiSkinInstance));
	if (STOP) return;
	readNiSkinInstance (t, 0);
}
static void
RPNiTriShapeSkinController()
{
	NiTriShapeSkinController *t = GETMEM (sizeof(NiTriShapeSkinController));
	if (STOP) return;
	readNiTriShapeSkinController (t, 0);
}
static void
RPNiClodSkinInstance()
{
	NiClodSkinInstance *t = GETMEM (sizeof(NiClodSkinInstance));
	if (STOP) return;
	readNiClodSkinInstance (t, 0);
}
static void
RPNiSkinPartition()
{
	NiSkinPartition *t = GETMEM (sizeof(NiSkinPartition));
	if (STOP) return;
	readNiSkinPartition (t, 0);
}
static void
RPNiTexture()
{
	NiTexture *t = GETMEM (sizeof(NiTexture));
	if (STOP) return;
	readNiTexture (t, 0);
}
static void
RPNiSourceTexture()
{
	NiSourceTexture *t = GETMEM (sizeof(NiSourceTexture));
	if (STOP) return;
	readNiSourceTexture (t, 0);
}
static void
RPNiSpecularProperty()
{
	NiSpecularProperty *t = GETMEM (sizeof(NiSpecularProperty));
	if (STOP) return;
	readNiSpecularProperty (t, 0);
}
static void
RPNiSphericalCollider()
{
	NiSphericalCollider *t = GETMEM (sizeof(NiSphericalCollider));
	if (STOP) return;
	readNiSphericalCollider (t, 0);
}
static void
RPNiSpotLight()
{
	NiSpotLight *t = GETMEM (sizeof(NiSpotLight));
	if (STOP) return;
	readNiSpotLight (t, 0);
}
static void
RPNiStencilProperty()
{
	NiStencilProperty *t = GETMEM (sizeof(NiStencilProperty));
	if (STOP) return;
	readNiStencilProperty (t, 0);
}
static void
RPNiStringExtraData()
{
	NiStringExtraData *t = GETMEM (sizeof(NiStringExtraData));
	if (STOP) return;
	readNiStringExtraData (t, 0);
}
static void
RPNiStringPalette()
{
	NiStringPalette *t = GETMEM (sizeof(NiStringPalette));
	if (STOP) return;
	readNiStringPalette (t, 0);
}
static void
RPNiStringsExtraData()
{
	NiStringsExtraData *t = GETMEM (sizeof(NiStringsExtraData));
	if (STOP) return;
	readNiStringsExtraData (t, 0);
}
static void
RPNiTextKeyExtraData()
{
	NiTextKeyExtraData *t = GETMEM (sizeof(NiTextKeyExtraData));
	if (STOP) return;
	readNiTextKeyExtraData (t, 0);
}
static void
RPNiTextureEffect()
{
	NiTextureEffect *t = GETMEM (sizeof(NiTextureEffect));
	if (STOP) return;
	readNiTextureEffect (t, 0);
}
static void
RPNiTextureModeProperty()
{
	NiTextureModeProperty *t = GETMEM (sizeof(NiTextureModeProperty));
	if (STOP) return;
	readNiTextureModeProperty (t, 0);
}
static void
RPNiImage()
{
	NiImage *t = GETMEM (sizeof(NiImage));
	if (STOP) return;
	readNiImage (t, 0);
}
static void
RPNiTextureProperty()
{
	NiTextureProperty *t = GETMEM (sizeof(NiTextureProperty));
	if (STOP) return;
	readNiTextureProperty (t, 0);
}
static void
RPNiMultiTextureProperty()
{
	NiMultiTextureProperty *t = GETMEM (sizeof(NiMultiTextureProperty));
	if (STOP) return;
	readNiMultiTextureProperty (t, 0);
}
static void
RPNiTexturingProperty()
{
	NiTexturingProperty *t = GETMEM (sizeof(NiTexturingProperty));
	if (STOP) return;
	readNiTexturingProperty (t, 0);
}
static void
RPNiTransformData()
{
	NiTransformData *t = GETMEM (sizeof(NiTransformData));
	if (STOP) return;
	readNiTransformData (t, 0);
}
static void
RPNiTriShape()
{
	NiTriShape *t = GETMEM (sizeof(NiTriShape));
	if (STOP) return;
	readNiTriShape (t, 0);
}
static void
RPNiTriShapeData()
{
	NiTriShapeData *t = GETMEM (sizeof(NiTriShapeData));
	if (STOP) return;
	readNiTriShapeData (t, 0);
}
static void
RPNiTriStrips()
{
	NiTriStrips *t = GETMEM (sizeof(NiTriStrips));
	if (STOP) return;
	readNiTriStrips (t, 0);
}
static void
RPNiTriStripsData()
{
	NiTriStripsData *t = GETMEM (sizeof(NiTriStripsData));
	if (STOP) return;
	readNiTriStripsData (t, 0);
}
static void
RPNiEnvMappedTriShape()
{
	NiEnvMappedTriShape *t = GETMEM (sizeof(NiEnvMappedTriShape));
	if (STOP) return;
	readNiEnvMappedTriShape (t, 0);
}
static void
RPNiEnvMappedTriShapeData()
{
	NiEnvMappedTriShapeData *t = GETMEM (sizeof(NiEnvMappedTriShapeData));
	if (STOP) return;
	readNiEnvMappedTriShapeData (t, 0);
}
static void
RPNiBezierTriangle4()
{
	NiBezierTriangle4 *t = GETMEM (sizeof(NiBezierTriangle4));
	if (STOP) return;
	readNiBezierTriangle4 (t, 0);
}
static void
RPNiBezierMesh()
{
	NiBezierMesh *t = GETMEM (sizeof(NiBezierMesh));
	if (STOP) return;
	readNiBezierMesh (t, 0);
}
static void
RPNiClod()
{
	NiClod *t = GETMEM (sizeof(NiClod));
	if (STOP) return;
	readNiClod (t, 0);
}
static void
RPNiClodData()
{
	NiClodData *t = GETMEM (sizeof(NiClodData));
	if (STOP) return;
	readNiClodData (t, 0);
}
static void
RPNiUVController()
{
	NiUVController *t = GETMEM (sizeof(NiUVController));
	if (STOP) return;
	readNiUVController (t, 0);
}
static void
RPNiUVData()
{
	NiUVData *t = GETMEM (sizeof(NiUVData));
	if (STOP) return;
	readNiUVData (t, 0);
}
static void
RPNiVectorExtraData()
{
	NiVectorExtraData *t = GETMEM (sizeof(NiVectorExtraData));
	if (STOP) return;
	readNiVectorExtraData (t, 0);
}
static void
RPNiVertexColorProperty()
{
	NiVertexColorProperty *t = GETMEM (sizeof(NiVertexColorProperty));
	if (STOP) return;
	readNiVertexColorProperty (t, 0);
}
static void
RPNiVertWeightsExtraData()
{
	NiVertWeightsExtraData *t = GETMEM (sizeof(NiVertWeightsExtraData));
	if (STOP) return;
	readNiVertWeightsExtraData (t, 0);
}
static void
RPNiVisData()
{
	NiVisData *t = GETMEM (sizeof(NiVisData));
	if (STOP) return;
	readNiVisData (t, 0);
}
static void
RPNiWireframeProperty()
{
	NiWireframeProperty *t = GETMEM (sizeof(NiWireframeProperty));
	if (STOP) return;
	readNiWireframeProperty (t, 0);
}
static void
RPNiZBufferProperty()
{
	NiZBufferProperty *t = GETMEM (sizeof(NiZBufferProperty));
	if (STOP) return;
	readNiZBufferProperty (t, 0);
}
static void
RPRootCollisionNode()
{
	RootCollisionNode *t = GETMEM (sizeof(RootCollisionNode));
	if (STOP) return;
	readRootCollisionNode (t, 0);
}
static void
RPNiRawImageData()
{
	NiRawImageData *t = GETMEM (sizeof(NiRawImageData));
	if (STOP) return;
	readNiRawImageData (t, 0);
}
static void
RPNiSortAdjustNode()
{
	NiSortAdjustNode *t = GETMEM (sizeof(NiSortAdjustNode));
	if (STOP) return;
	readNiSortAdjustNode (t, 0);
}
static void
RPNiSourceCubeMap()
{
	NiSourceCubeMap *t = GETMEM (sizeof(NiSourceCubeMap));
	if (STOP) return;
	readNiSourceCubeMap (t, 0);
}
static void
RPNiPhysXProp()
{
	NiPhysXProp *t = GETMEM (sizeof(NiPhysXProp));
	if (STOP) return;
	readNiPhysXProp (t, 0);
}
static void
RPphysXMaterialRef()
{
	physXMaterialRef *t = GETMEM (sizeof(physXMaterialRef));
	if (STOP) return;
	readphysXMaterialRef (t, 0);
}
static void
RPNiPhysXPropDesc()
{
	NiPhysXPropDesc *t = GETMEM (sizeof(NiPhysXPropDesc));
	if (STOP) return;
	readNiPhysXPropDesc (t, 0);
}
static void
RPNiPhysXActorDesc()
{
	NiPhysXActorDesc *t = GETMEM (sizeof(NiPhysXActorDesc));
	if (STOP) return;
	readNiPhysXActorDesc (t, 0);
}
static void
RPNiPhysXBodyDesc()
{
	NiPhysXBodyDesc *t = GETMEM (sizeof(NiPhysXBodyDesc));
	if (STOP) return;
	readNiPhysXBodyDesc (t, 0);
}
static void
RPNiPhysXD6JointDesc()
{
	NiPhysXD6JointDesc *t = GETMEM (sizeof(NiPhysXD6JointDesc));
	if (STOP) return;
	readNiPhysXD6JointDesc (t, 0);
}
static void
RPNiPhysXShapeDesc()
{
	NiPhysXShapeDesc *t = GETMEM (sizeof(NiPhysXShapeDesc));
	if (STOP) return;
	readNiPhysXShapeDesc (t, 0);
}
static void
RPNiPhysXMeshDesc()
{
	NiPhysXMeshDesc *t = GETMEM (sizeof(NiPhysXMeshDesc));
	if (STOP) return;
	readNiPhysXMeshDesc (t, 0);
}
static void
RPNiPhysXMaterialDesc()
{
	NiPhysXMaterialDesc *t = GETMEM (sizeof(NiPhysXMaterialDesc));
	if (STOP) return;
	readNiPhysXMaterialDesc (t, 0);
}
static void
RPNiPhysXKinematicSrc()
{
	NiPhysXKinematicSrc *t = GETMEM (sizeof(NiPhysXKinematicSrc));
	if (STOP) return;
	readNiPhysXKinematicSrc (t, 0);
}
static void
RPNiPhysXTransformDest()
{
	NiPhysXTransformDest *t = GETMEM (sizeof(NiPhysXTransformDest));
	if (STOP) return;
	readNiPhysXTransformDest (t, 0);
}
static void
RPNiArkAnimationExtraData()
{
	NiArkAnimationExtraData *t = GETMEM (sizeof(NiArkAnimationExtraData));
	if (STOP) return;
	readNiArkAnimationExtraData (t, 0);
}
static void
RPNiArkImporterExtraData()
{
	NiArkImporterExtraData *t = GETMEM (sizeof(NiArkImporterExtraData));
	if (STOP) return;
	readNiArkImporterExtraData (t, 0);
}
static void
RPNiArkTextureExtraData()
{
	NiArkTextureExtraData *t = GETMEM (sizeof(NiArkTextureExtraData));
	if (STOP) return;
	readNiArkTextureExtraData (t, 0);
}
static void
RPNiArkViewportInfoExtraData()
{
	NiArkViewportInfoExtraData *t = GETMEM (sizeof(NiArkViewportInfoExtraData));
	if (STOP) return;
	readNiArkViewportInfoExtraData (t, 0);
}
static void
RPNiArkShaderExtraData()
{
	NiArkShaderExtraData *t = GETMEM (sizeof(NiArkShaderExtraData));
	if (STOP) return;
	readNiArkShaderExtraData (t, 0);
}
static void
RPNiLines()
{
	NiLines *t = GETMEM (sizeof(NiLines));
	if (STOP) return;
	readNiLines (t, 0);
}
static void
RPNiLinesData()
{
	NiLinesData *t = GETMEM (sizeof(NiLinesData));
	if (STOP) return;
	readNiLinesData (t, 0);
}
static void
RPPolygon()
{
	Polygon *t = GETMEM (sizeof(Polygon));
	if (STOP) return;
	readPolygon (t, 0);
}
static void
RPNiScreenElementsData()
{
	NiScreenElementsData *t = GETMEM (sizeof(NiScreenElementsData));
	if (STOP) return;
	readNiScreenElementsData (t, 0);
}
static void
RPNiScreenElements()
{
	NiScreenElements *t = GETMEM (sizeof(NiScreenElements));
	if (STOP) return;
	readNiScreenElements (t, 0);
}
static void
RPNiRoomGroup()
{
	NiRoomGroup *t = GETMEM (sizeof(NiRoomGroup));
	if (STOP) return;
	readNiRoomGroup (t, 0);
}
static void
RPNiRoom()
{
	NiRoom *t = GETMEM (sizeof(NiRoom));
	if (STOP) return;
	readNiRoom (t, 0);
}
static void
RPNiPortal()
{
	NiPortal *t = GETMEM (sizeof(NiPortal));
	if (STOP) return;
	readNiPortal (t, 0);
}
static void
RPBSFadeNode()
{
	BSFadeNode *t = GETMEM (sizeof(BSFadeNode));
	if (STOP) return;
	readBSFadeNode (t, 0);
}
static void
RPBSShaderProperty()
{
	BSShaderProperty *t = GETMEM (sizeof(BSShaderProperty));
	if (STOP) return;
	readBSShaderProperty (t, 0);
}
static void
RPBSShaderLightingProperty()
{
	BSShaderLightingProperty *t = GETMEM (sizeof(BSShaderLightingProperty));
	if (STOP) return;
	readBSShaderLightingProperty (t, 0);
}
static void
RPBSShaderNoLightingProperty()
{
	BSShaderNoLightingProperty *t = GETMEM (sizeof(BSShaderNoLightingProperty));
	if (STOP) return;
	readBSShaderNoLightingProperty (t, 0);
}
static void
RPBSShaderPPLightingProperty()
{
	BSShaderPPLightingProperty *t = GETMEM (sizeof(BSShaderPPLightingProperty));
	if (STOP) return;
	readBSShaderPPLightingProperty (t, 0);
}
static void
RPBSShaderTextureSet()
{
	BSShaderTextureSet *t = GETMEM (sizeof(BSShaderTextureSet));
	if (STOP) return;
	readBSShaderTextureSet (t, 0);
}
static void
RPWaterShaderProperty()
{
	WaterShaderProperty *t = GETMEM (sizeof(WaterShaderProperty));
	if (STOP) return;
	readWaterShaderProperty (t, 0);
}
static void
RPSkyShaderProperty()
{
	SkyShaderProperty *t = GETMEM (sizeof(SkyShaderProperty));
	if (STOP) return;
	readSkyShaderProperty (t, 0);
}
static void
RPTileShaderProperty()
{
	TileShaderProperty *t = GETMEM (sizeof(TileShaderProperty));
	if (STOP) return;
	readTileShaderProperty (t, 0);
}
static void
RPDistantLODShaderProperty()
{
	DistantLODShaderProperty *t = GETMEM (sizeof(DistantLODShaderProperty));
	if (STOP) return;
	readDistantLODShaderProperty (t, 0);
}
static void
RPBSDistantTreeShaderProperty()
{
	BSDistantTreeShaderProperty *t = GETMEM (sizeof(BSDistantTreeShaderProperty));
	if (STOP) return;
	readBSDistantTreeShaderProperty (t, 0);
}
static void
RPTallGrassShaderProperty()
{
	TallGrassShaderProperty *t = GETMEM (sizeof(TallGrassShaderProperty));
	if (STOP) return;
	readTallGrassShaderProperty (t, 0);
}
static void
RPVolumetricFogShaderProperty()
{
	VolumetricFogShaderProperty *t = GETMEM (sizeof(VolumetricFogShaderProperty));
	if (STOP) return;
	readVolumetricFogShaderProperty (t, 0);
}
static void
RPHairShaderProperty()
{
	HairShaderProperty *t = GETMEM (sizeof(HairShaderProperty));
	if (STOP) return;
	readHairShaderProperty (t, 0);
}
static void
RPLighting30ShaderProperty()
{
	Lighting30ShaderProperty *t = GETMEM (sizeof(Lighting30ShaderProperty));
	if (STOP) return;
	readLighting30ShaderProperty (t, 0);
}
static void
RPBSDismemberSkinInstance()
{
	BSDismemberSkinInstance *t = GETMEM (sizeof(BSDismemberSkinInstance));
	if (STOP) return;
	readBSDismemberSkinInstance (t, 0);
}
static void
RPBSDecalPlacementVectorExtraData()
{
	BSDecalPlacementVectorExtraData *t = GETMEM (sizeof(BSDecalPlacementVectorExtraData));
	if (STOP) return;
	readBSDecalPlacementVectorExtraData (t, 0);
}
static void
RPBSPSysSimpleColorModifier()
{
	BSPSysSimpleColorModifier *t = GETMEM (sizeof(BSPSysSimpleColorModifier));
	if (STOP) return;
	readBSPSysSimpleColorModifier (t, 0);
}
static void
RPBSValueNode()
{
	BSValueNode *t = GETMEM (sizeof(BSValueNode));
	if (STOP) return;
	readBSValueNode (t, 0);
}
static void
RPBSStripParticleSystem()
{
	BSStripParticleSystem *t = GETMEM (sizeof(BSStripParticleSystem));
	if (STOP) return;
	readBSStripParticleSystem (t, 0);
}
static void
RPBSStripPSysData()
{
	BSStripPSysData *t = GETMEM (sizeof(BSStripPSysData));
	if (STOP) return;
	readBSStripPSysData (t, 0);
}
static void
RPBSPSysStripUpdateModifier()
{
	BSPSysStripUpdateModifier *t = GETMEM (sizeof(BSPSysStripUpdateModifier));
	if (STOP) return;
	readBSPSysStripUpdateModifier (t, 0);
}
static void
RPBSMaterialEmittanceMultController()
{
	BSMaterialEmittanceMultController *t = GETMEM (sizeof(BSMaterialEmittanceMultController));
	if (STOP) return;
	readBSMaterialEmittanceMultController (t, 0);
}
static void
RPBSMasterParticleSystem()
{
	BSMasterParticleSystem *t = GETMEM (sizeof(BSMasterParticleSystem));
	if (STOP) return;
	readBSMasterParticleSystem (t, 0);
}
static void
RPBSPSysMultiTargetEmitterCtlr()
{
	BSPSysMultiTargetEmitterCtlr *t = GETMEM (sizeof(BSPSysMultiTargetEmitterCtlr));
	if (STOP) return;
	readBSPSysMultiTargetEmitterCtlr (t, 0);
}
static void
RPBSRefractionStrengthController()
{
	BSRefractionStrengthController *t = GETMEM (sizeof(BSRefractionStrengthController));
	if (STOP) return;
	readBSRefractionStrengthController (t, 0);
}
static void
RPBSOrderedNode()
{
	BSOrderedNode *t = GETMEM (sizeof(BSOrderedNode));
	if (STOP) return;
	readBSOrderedNode (t, 0);
}
static void
RPBSBlastNode()
{
	BSBlastNode *t = GETMEM (sizeof(BSBlastNode));
	if (STOP) return;
	readBSBlastNode (t, 0);
}
static void
RPBSDamageStage()
{
	BSDamageStage *t = GETMEM (sizeof(BSDamageStage));
	if (STOP) return;
	readBSDamageStage (t, 0);
}
static void
RPBSRefractionFirePeriodController()
{
	BSRefractionFirePeriodController *t = GETMEM (sizeof(BSRefractionFirePeriodController));
	if (STOP) return;
	readBSRefractionFirePeriodController (t, 0);
}
static void
RPbhkConvexListShape()
{
	bhkConvexListShape *t = GETMEM (sizeof(bhkConvexListShape));
	if (STOP) return;
	readbhkConvexListShape (t, 0);
}
static void
RPBSTreadTransfSubInfo()
{
	BSTreadTransfSubInfo *t = GETMEM (sizeof(BSTreadTransfSubInfo));
	if (STOP) return;
	readBSTreadTransfSubInfo (t, 0);
}
static void
RPBSTreadTransfInfo()
{
	BSTreadTransfInfo *t = GETMEM (sizeof(BSTreadTransfInfo));
	if (STOP) return;
	readBSTreadTransfInfo (t, 0);
}
static void
RPBSTreadTransfInterpolator()
{
	BSTreadTransfInterpolator *t = GETMEM (sizeof(BSTreadTransfInterpolator));
	if (STOP) return;
	readBSTreadTransfInterpolator (t, 0);
}
static void
RPBSAnimNotes()
{
	BSAnimNotes *t = GETMEM (sizeof(BSAnimNotes));
	if (STOP) return;
	readBSAnimNotes (t, 0);
}
static void
RPbhkLiquidAction()
{
	bhkLiquidAction *t = GETMEM (sizeof(bhkLiquidAction));
	if (STOP) return;
	readbhkLiquidAction (t, 0);
}
static void
RPBSMultiBoundNode()
{
	BSMultiBoundNode *t = GETMEM (sizeof(BSMultiBoundNode));
	if (STOP) return;
	readBSMultiBoundNode (t, 0);
}
static void
RPBSMultiBound()
{
	BSMultiBound *t = GETMEM (sizeof(BSMultiBound));
	if (STOP) return;
	readBSMultiBound (t, 0);
}
static void
RPBSMultiBoundData()
{
	BSMultiBoundData *t = GETMEM (sizeof(BSMultiBoundData));
	if (STOP) return;
	readBSMultiBoundData (t, 0);
}
static void
RPBSMultiBoundSphere()
{
	BSMultiBoundSphere *t = GETMEM (sizeof(BSMultiBoundSphere));
	if (STOP) return;
	readBSMultiBoundSphere (t, 0);
}
static void
RPBSSegmentedTriangle()
{
	BSSegmentedTriangle *t = GETMEM (sizeof(BSSegmentedTriangle));
	if (STOP) return;
	readBSSegmentedTriangle (t, 0);
}
static void
RPBSSegmentedTriShape()
{
	BSSegmentedTriShape *t = GETMEM (sizeof(BSSegmentedTriShape));
	if (STOP) return;
	readBSSegmentedTriShape (t, 0);
}
static void
RPBSMultiBoundAABB()
{
	BSMultiBoundAABB *t = GETMEM (sizeof(BSMultiBoundAABB));
	if (STOP) return;
	readBSMultiBoundAABB (t, 0);
}
static void
RPAdditionalDataInfo()
{
	AdditionalDataInfo *t = GETMEM (sizeof(AdditionalDataInfo));
	if (STOP) return;
	readAdditionalDataInfo (t, 0);
}
static void
RPAdditionalDataBlock()
{
	AdditionalDataBlock *t = GETMEM (sizeof(AdditionalDataBlock));
	if (STOP) return;
	readAdditionalDataBlock (t, 0);
}
static void
RPNiAdditionalGeometryData()
{
	NiAdditionalGeometryData *t = GETMEM (sizeof(NiAdditionalGeometryData));
	if (STOP) return;
	readNiAdditionalGeometryData (t, 0);
}
static void
RPBSWArray()
{
	BSWArray *t = GETMEM (sizeof(BSWArray));
	if (STOP) return;
	readBSWArray (t, 0);
}
static void
RPbhkAabbPhantom()
{
	bhkAabbPhantom *t = GETMEM (sizeof(bhkAabbPhantom));
	if (STOP) return;
	readbhkAabbPhantom (t, 0);
}
static void
RPBSFrustumFOVController()
{
	BSFrustumFOVController *t = GETMEM (sizeof(BSFrustumFOVController));
	if (STOP) return;
	readBSFrustumFOVController (t, 0);
}
static void
RPBSDebrisNode()
{
	BSDebrisNode *t = GETMEM (sizeof(BSDebrisNode));
	if (STOP) return;
	readBSDebrisNode (t, 0);
}
static void
RPbhkBreakableConstraint()
{
	bhkBreakableConstraint *t = GETMEM (sizeof(bhkBreakableConstraint));
	if (STOP) return;
	readbhkBreakableConstraint (t, 0);
}
static void
RPbhkOrientHingedBodyAction()
{
	bhkOrientHingedBodyAction *t = GETMEM (sizeof(bhkOrientHingedBodyAction));
	if (STOP) return;
	readbhkOrientHingedBodyAction (t, 0);
}
static void
RPRegion()
{
	Region *t = GETMEM (sizeof(Region));
	if (STOP) return;
	readRegion (t, 0);
}
static void
RPNiDataStream()
{
	NiDataStream *t = GETMEM (sizeof(NiDataStream));
	if (STOP) return;
	readNiDataStream (t, 0);
}
static void
RPSemanticData()
{
	SemanticData *t = GETMEM (sizeof(SemanticData));
	if (STOP) return;
	readSemanticData (t, 0);
}
static void
RPMeshData()
{
	MeshData *t = GETMEM (sizeof(MeshData));
	if (STOP) return;
	readMeshData (t, 0);
}
static void
RPMaterialData()
{
	MaterialData *t = GETMEM (sizeof(MaterialData));
	if (STOP) return;
	readMaterialData (t, 0);
}
static void
RPNiRenderObject()
{
	NiRenderObject *t = GETMEM (sizeof(NiRenderObject));
	if (STOP) return;
	readNiRenderObject (t, 0);
}
static void
RPNiMeshModifier()
{
	NiMeshModifier *t = GETMEM (sizeof(NiMeshModifier));
	if (STOP) return;
	readNiMeshModifier (t, 0);
}
static void
RPNiMesh()
{
	NiMesh *t = GETMEM (sizeof(NiMesh));
	if (STOP) return;
	readNiMesh (t, 0);
}
static void
RPNiMorphWeightsController()
{
	NiMorphWeightsController *t = GETMEM (sizeof(NiMorphWeightsController));
	if (STOP) return;
	readNiMorphWeightsController (t, 0);
}
static void
RPElementReference()
{
	ElementReference *t = GETMEM (sizeof(ElementReference));
	if (STOP) return;
	readElementReference (t, 0);
}
static void
RPNiMorphMeshModifier()
{
	NiMorphMeshModifier *t = GETMEM (sizeof(NiMorphMeshModifier));
	if (STOP) return;
	readNiMorphMeshModifier (t, 0);
}
static void
RPNiSkinningMeshModifier()
{
	NiSkinningMeshModifier *t = GETMEM (sizeof(NiSkinningMeshModifier));
	if (STOP) return;
	readNiSkinningMeshModifier (t, 0);
}
static void
RPNiInstancingMeshModifier()
{
	NiInstancingMeshModifier *t = GETMEM (sizeof(NiInstancingMeshModifier));
	if (STOP) return;
	readNiInstancingMeshModifier (t, 0);
}
static void
RPNiSkinningLODController()
{
	NiSkinningLODController *t = GETMEM (sizeof(NiSkinningLODController));
	if (STOP) return;
	readNiSkinningLODController (t, 0);
}
static void
RPNiPSParticleSystem()
{
	NiPSParticleSystem *t = GETMEM (sizeof(NiPSParticleSystem));
	if (STOP) return;
	readNiPSParticleSystem (t, 0);
}
static void
RPNiPSMeshParticleSystem()
{
	NiPSMeshParticleSystem *t = GETMEM (sizeof(NiPSMeshParticleSystem));
	if (STOP) return;
	readNiPSMeshParticleSystem (t, 0);
}
static void
RPNiPSEmitParticlesCtlr()
{
	NiPSEmitParticlesCtlr *t = GETMEM (sizeof(NiPSEmitParticlesCtlr));
	if (STOP) return;
	readNiPSEmitParticlesCtlr (t, 0);
}
static void
RPNiPSForceActiveCtlr()
{
	NiPSForceActiveCtlr *t = GETMEM (sizeof(NiPSForceActiveCtlr));
	if (STOP) return;
	readNiPSForceActiveCtlr (t, 0);
}
static void
RPNiPSSimulator()
{
	NiPSSimulator *t = GETMEM (sizeof(NiPSSimulator));
	if (STOP) return;
	readNiPSSimulator (t, 0);
}
static void
RPNiPSSimulatorStep()
{
	NiPSSimulatorStep *t = GETMEM (sizeof(NiPSSimulatorStep));
	if (STOP) return;
	readNiPSSimulatorStep (t, 0);
}
static void
RPNiPSSimulatorGeneralStep()
{
	NiPSSimulatorGeneralStep *t = GETMEM (sizeof(NiPSSimulatorGeneralStep));
	if (STOP) return;
	readNiPSSimulatorGeneralStep (t, 0);
}
static void
RPNiPSSimulatorForcesStep()
{
	NiPSSimulatorForcesStep *t = GETMEM (sizeof(NiPSSimulatorForcesStep));
	if (STOP) return;
	readNiPSSimulatorForcesStep (t, 0);
}
static void
RPNiPSSimulatorCollidersStep()
{
	NiPSSimulatorCollidersStep *t = GETMEM (sizeof(NiPSSimulatorCollidersStep));
	if (STOP) return;
	readNiPSSimulatorCollidersStep (t, 0);
}
static void
RPNiPSSimulatorMeshAlignStep()
{
	NiPSSimulatorMeshAlignStep *t = GETMEM (sizeof(NiPSSimulatorMeshAlignStep));
	if (STOP) return;
	readNiPSSimulatorMeshAlignStep (t, 0);
}
static void
RPNiPSSimulatorFinalStep()
{
	NiPSSimulatorFinalStep *t = GETMEM (sizeof(NiPSSimulatorFinalStep));
	if (STOP) return;
	readNiPSSimulatorFinalStep (t, 0);
}
static void
RPNiPSFacingQuadGenerator()
{
	NiPSFacingQuadGenerator *t = GETMEM (sizeof(NiPSFacingQuadGenerator));
	if (STOP) return;
	readNiPSFacingQuadGenerator (t, 0);
}
static void
RPNiShadowGenerator()
{
	NiShadowGenerator *t = GETMEM (sizeof(NiShadowGenerator));
	if (STOP) return;
	readNiShadowGenerator (t, 0);
}
static void
RPNiPSBoundUpdater()
{
	NiPSBoundUpdater *t = GETMEM (sizeof(NiPSBoundUpdater));
	if (STOP) return;
	readNiPSBoundUpdater (t, 0);
}
static void
RPNiPSDragForce()
{
	NiPSDragForce *t = GETMEM (sizeof(NiPSDragForce));
	if (STOP) return;
	readNiPSDragForce (t, 0);
}
static void
RPNiPSGravityForce()
{
	NiPSGravityForce *t = GETMEM (sizeof(NiPSGravityForce));
	if (STOP) return;
	readNiPSGravityForce (t, 0);
}
static void
RPNiPSBoxEmitter()
{
	NiPSBoxEmitter *t = GETMEM (sizeof(NiPSBoxEmitter));
	if (STOP) return;
	readNiPSBoxEmitter (t, 0);
}
static void
RPNiPSMeshEmitter()
{
	NiPSMeshEmitter *t = GETMEM (sizeof(NiPSMeshEmitter));
	if (STOP) return;
	readNiPSMeshEmitter (t, 0);
}
static void
RPNiPSGravityStrengthCtlr()
{
	NiPSGravityStrengthCtlr *t = GETMEM (sizeof(NiPSGravityStrengthCtlr));
	if (STOP) return;
	readNiPSGravityStrengthCtlr (t, 0);
}
static void
RPNiPSPlanarCollider()
{
	NiPSPlanarCollider *t = GETMEM (sizeof(NiPSPlanarCollider));
	if (STOP) return;
	readNiPSPlanarCollider (t, 0);
}
static void
RPNiPSEmitterSpeedCtlr()
{
	NiPSEmitterSpeedCtlr *t = GETMEM (sizeof(NiPSEmitterSpeedCtlr));
	if (STOP) return;
	readNiPSEmitterSpeedCtlr (t, 0);
}
static void
RPNiPSEmitterRadiusCtlr()
{
	NiPSEmitterRadiusCtlr *t = GETMEM (sizeof(NiPSEmitterRadiusCtlr));
	if (STOP) return;
	readNiPSEmitterRadiusCtlr (t, 0);
}
static void
RPNiPSResetOnLoopCtlr()
{
	NiPSResetOnLoopCtlr *t = GETMEM (sizeof(NiPSResetOnLoopCtlr));
	if (STOP) return;
	readNiPSResetOnLoopCtlr (t, 0);
}
static void
RPNiPSSphereEmitter()
{
	NiPSSphereEmitter *t = GETMEM (sizeof(NiPSSphereEmitter));
	if (STOP) return;
	readNiPSSphereEmitter (t, 0);
}
static void
RPNiPSCylinderEmitter()
{
	NiPSCylinderEmitter *t = GETMEM (sizeof(NiPSCylinderEmitter));
	if (STOP) return;
	readNiPSCylinderEmitter (t, 0);
}
static void
RPNiPSEmitterDeclinationCtlr()
{
	NiPSEmitterDeclinationCtlr *t = GETMEM (sizeof(NiPSEmitterDeclinationCtlr));
	if (STOP) return;
	readNiPSEmitterDeclinationCtlr (t, 0);
}
static void
RPNiPSEmitterDeclinationVarCtlr()
{
	NiPSEmitterDeclinationVarCtlr *t = GETMEM (sizeof(NiPSEmitterDeclinationVarCtlr));
	if (STOP) return;
	readNiPSEmitterDeclinationVarCtlr (t, 0);
}
static void
RPNiPSEmitterPlanarAngleCtlr()
{
	NiPSEmitterPlanarAngleCtlr *t = GETMEM (sizeof(NiPSEmitterPlanarAngleCtlr));
	if (STOP) return;
	readNiPSEmitterPlanarAngleCtlr (t, 0);
}
static void
RPNiPSEmitterPlanarAngleVarCtlr()
{
	NiPSEmitterPlanarAngleVarCtlr *t = GETMEM (sizeof(NiPSEmitterPlanarAngleVarCtlr));
	if (STOP) return;
	readNiPSEmitterPlanarAngleVarCtlr (t, 0);
}
static void
RPNiPSEmitterRotAngleCtlr()
{
	NiPSEmitterRotAngleCtlr *t = GETMEM (sizeof(NiPSEmitterRotAngleCtlr));
	if (STOP) return;
	readNiPSEmitterRotAngleCtlr (t, 0);
}
static void
RPNiPSEmitterRotAngleVarCtlr()
{
	NiPSEmitterRotAngleVarCtlr *t = GETMEM (sizeof(NiPSEmitterRotAngleVarCtlr));
	if (STOP) return;
	readNiPSEmitterRotAngleVarCtlr (t, 0);
}
static void
RPNiPSEmitterRotSpeedCtlr()
{
	NiPSEmitterRotSpeedCtlr *t = GETMEM (sizeof(NiPSEmitterRotSpeedCtlr));
	if (STOP) return;
	readNiPSEmitterRotSpeedCtlr (t, 0);
}
static void
RPNiPSEmitterRotSpeedVarCtlr()
{
	NiPSEmitterRotSpeedVarCtlr *t = GETMEM (sizeof(NiPSEmitterRotSpeedVarCtlr));
	if (STOP) return;
	readNiPSEmitterRotSpeedVarCtlr (t, 0);
}
static void
RPNiPSEmitterLifeSpanCtlr()
{
	NiPSEmitterLifeSpanCtlr *t = GETMEM (sizeof(NiPSEmitterLifeSpanCtlr));
	if (STOP) return;
	readNiPSEmitterLifeSpanCtlr (t, 0);
}
static void
RPNiPSBombForce()
{
	NiPSBombForce *t = GETMEM (sizeof(NiPSBombForce));
	if (STOP) return;
	readNiPSBombForce (t, 0);
}
static void
RPNiPSSphericalCollider()
{
	NiPSSphericalCollider *t = GETMEM (sizeof(NiPSSphericalCollider));
	if (STOP) return;
	readNiPSSphericalCollider (t, 0);
}
static void
RPNiPSSpawner()
{
	NiPSSpawner *t = GETMEM (sizeof(NiPSSpawner));
	if (STOP) return;
	readNiPSSpawner (t, 0);
}
static void
RPNiSequenceData()
{
	NiSequenceData *t = GETMEM (sizeof(NiSequenceData));
	if (STOP) return;
	readNiSequenceData (t, 0);
}
static void
RPNiTransformEvaluator()
{
	NiTransformEvaluator *t = GETMEM (sizeof(NiTransformEvaluator));
	if (STOP) return;
	readNiTransformEvaluator (t, 0);
}
static void
RPNiBSplineCompTransformEvaluator()
{
	NiBSplineCompTransformEvaluator *t = GETMEM (sizeof(NiBSplineCompTransformEvaluator));
	if (STOP) return;
	readNiBSplineCompTransformEvaluator (t, 0);
}
static void
RPNiMeshHWInstance()
{
	NiMeshHWInstance *t = GETMEM (sizeof(NiMeshHWInstance));
	if (STOP) return;
	readNiMeshHWInstance (t, 0);
}
static void
RPNiFurSpringController()
{
	NiFurSpringController *t = GETMEM (sizeof(NiFurSpringController));
	if (STOP) return;
	readNiFurSpringController (t, 0);
}

static hash *RPHash;
static void
CreateRpHash()
{
	RPHash = hashcreate (500);
	hashadd (RPHash, "SizedString", (char*)RPSizedString);
	hashadd (RPHash, "string", (char*)RPstring);
	hashadd (RPHash, "ByteArray", (char*)RPByteArray);
	hashadd (RPHash, "ByteMatrix", (char*)RPByteMatrix);
	hashadd (RPHash, "Color3", (char*)RPColor3);
	hashadd (RPHash, "ByteColor3", (char*)RPByteColor3);
	hashadd (RPHash, "Color4", (char*)RPColor4);
	hashadd (RPHash, "ByteColor4", (char*)RPByteColor4);
	hashadd (RPHash, "FilePath", (char*)RPFilePath);
	hashadd (RPHash, "Footer", (char*)RPFooter);
	hashadd (RPHash, "LODRange", (char*)RPLODRange);
	hashadd (RPHash, "MatchGroup", (char*)RPMatchGroup);
	hashadd (RPHash, "Vector3", (char*)RPVector3);
	hashadd (RPHash, "Vector4", (char*)RPVector4);
	hashadd (RPHash, "Quaternion", (char*)RPQuaternion);
	hashadd (RPHash, "QuaternionXYZW", (char*)RPQuaternionXYZW);
	hashadd (RPHash, "Matrix22", (char*)RPMatrix22);
	hashadd (RPHash, "Matrix33", (char*)RPMatrix33);
	hashadd (RPHash, "Matrix44", (char*)RPMatrix44);
	hashadd (RPHash, "MipMap", (char*)RPMipMap);
	hashadd (RPHash, "NodeGroup", (char*)RPNodeGroup);
	hashadd (RPHash, "ShortString", (char*)RPShortString);
	hashadd (RPHash, "SkinShape", (char*)RPSkinShape);
	hashadd (RPHash, "SkinShapeGroup", (char*)RPSkinShapeGroup);
	hashadd (RPHash, "SkinWeight", (char*)RPSkinWeight);
	hashadd (RPHash, "AVObject", (char*)RPAVObject);
	hashadd (RPHash, "ControllerLink", (char*)RPControllerLink);
	hashadd (RPHash, "ExportInfo", (char*)RPExportInfo);
	hashadd (RPHash, "Header", (char*)RPHeader);
	hashadd (RPHash, "StringPalette", (char*)RPStringPalette);
	hashadd (RPHash, "TBC", (char*)RPTBC);
	hashadd (RPHash, "QuatKey", (char*)RPQuatKey);
	hashadd (RPHash, "TexCoord", (char*)RPTexCoord);
	hashadd (RPHash, "TexDesc", (char*)RPTexDesc);
	hashadd (RPHash, "ShaderTexDesc", (char*)RPShaderTexDesc);
	hashadd (RPHash, "TexSource", (char*)RPTexSource);
	hashadd (RPHash, "Triangle", (char*)RPTriangle);
	hashadd (RPHash, "SkinPartition", (char*)RPSkinPartition);
	hashadd (RPHash, "BoundingBox", (char*)RPBoundingBox);
	hashadd (RPHash, "FurniturePosition", (char*)RPFurniturePosition);
	hashadd (RPHash, "hkTriangle", (char*)RPhkTriangle);
	hashadd (RPHash, "Morph", (char*)RPMorph);
	hashadd (RPHash, "Particle", (char*)RPParticle);
	hashadd (RPHash, "SkinTransform", (char*)RPSkinTransform);
	hashadd (RPHash, "SkinData", (char*)RPSkinData);
	hashadd (RPHash, "SphereBV", (char*)RPSphereBV);
	hashadd (RPHash, "OblivionColFilter", (char*)RPOblivionColFilter);
	hashadd (RPHash, "OblivionSubShape", (char*)RPOblivionSubShape);
	hashadd (RPHash, "MotorDescriptor", (char*)RPMotorDescriptor);
	hashadd (RPHash, "RagdollDescriptor", (char*)RPRagdollDescriptor);
	hashadd (RPHash, "LimitedHingeDescriptor", (char*)RPLimitedHingeDescriptor);
	hashadd (RPHash, "HingeDescriptor", (char*)RPHingeDescriptor);
	hashadd (RPHash, "OldSkinData", (char*)RPOldSkinData);
	hashadd (RPHash, "MultiTextureElement", (char*)RPMultiTextureElement);
	hashadd (RPHash, "BoxBV", (char*)RPBoxBV);
	hashadd (RPHash, "CapsuleBV", (char*)RPCapsuleBV);
	hashadd (RPHash, "HalfSpaceBV", (char*)RPHalfSpaceBV);
	hashadd (RPHash, "BoundingVolume", (char*)RPBoundingVolume);
	hashadd (RPHash, "UnionBV", (char*)RPUnionBV);
	hashadd (RPHash, "MorphWeight", (char*)RPMorphWeight);
	hashadd (RPHash, "ArkTexture", (char*)RPArkTexture);
	hashadd (RPHash, "InertiaMatrix", (char*)RPInertiaMatrix);
	hashadd (RPHash, "DecalVectorArray", (char*)RPDecalVectorArray);
	hashadd (RPHash, "BodyPartList", (char*)RPBodyPartList);
	//hashadd (RPHash, "NiObject", (char*)RPNiObject);
	hashadd (RPHash, "Ni3dsAlphaAnimator", (char*)RPNi3dsAlphaAnimator);
	hashadd (RPHash, "Ni3dsAnimationNode", (char*)RPNi3dsAnimationNode);
	hashadd (RPHash, "Ni3dsColorAnimator", (char*)RPNi3dsColorAnimator);
	hashadd (RPHash, "Ni3dsMorphShape", (char*)RPNi3dsMorphShape);
	hashadd (RPHash, "Ni3dsParticleSystem", (char*)RPNi3dsParticleSystem);
	hashadd (RPHash, "Ni3dsPathController", (char*)RPNi3dsPathController);
	hashadd (RPHash, "NiParticleModifier", (char*)RPNiParticleModifier);
	hashadd (RPHash, "NiPSysCollider", (char*)RPNiPSysCollider);
	hashadd (RPHash, "bhkRefObject", (char*)RPbhkRefObject);
	hashadd (RPHash, "bhkSerializable", (char*)RPbhkSerializable);
	hashadd (RPHash, "bhkWorldObject", (char*)RPbhkWorldObject);
	hashadd (RPHash, "bhkPhantom", (char*)RPbhkPhantom);
	hashadd (RPHash, "bhkShapePhantom", (char*)RPbhkShapePhantom);
	hashadd (RPHash, "bhkSimpleShapePhantom", (char*)RPbhkSimpleShapePhantom);
	hashadd (RPHash, "bhkEntity", (char*)RPbhkEntity);
	hashadd (RPHash, "bhkRigidBody", (char*)RPbhkRigidBody);
	hashadd (RPHash, "bhkRigidBodyT", (char*)RPbhkRigidBodyT);
	hashadd (RPHash, "bhkConstraint", (char*)RPbhkConstraint);
	hashadd (RPHash, "bhkLimitedHingeConstraint", (char*)RPbhkLimitedHingeConstraint);
	hashadd (RPHash, "bhkMalleableConstraint", (char*)RPbhkMalleableConstraint);
	hashadd (RPHash, "bhkStiffSpringConstraint", (char*)RPbhkStiffSpringConstraint);
	hashadd (RPHash, "bhkRagdollConstraint", (char*)RPbhkRagdollConstraint);
	hashadd (RPHash, "bhkPrismaticConstraint", (char*)RPbhkPrismaticConstraint);
	hashadd (RPHash, "bhkHingeConstraint", (char*)RPbhkHingeConstraint);
	hashadd (RPHash, "bhkShape", (char*)RPbhkShape);
	hashadd (RPHash, "bhkTransformShape", (char*)RPbhkTransformShape);
	hashadd (RPHash, "bhkSphereRepShape", (char*)RPbhkSphereRepShape);
	hashadd (RPHash, "bhkConvexShape", (char*)RPbhkConvexShape);
	hashadd (RPHash, "bhkSphereShape", (char*)RPbhkSphereShape);
	hashadd (RPHash, "bhkCapsuleShape", (char*)RPbhkCapsuleShape);
	hashadd (RPHash, "bhkBoxShape", (char*)RPbhkBoxShape);
	hashadd (RPHash, "bhkConvexVerticesShape", (char*)RPbhkConvexVerticesShape);
	hashadd (RPHash, "bhkConvexTransformShape", (char*)RPbhkConvexTransformShape);
	hashadd (RPHash, "bhkMultiSphereShape", (char*)RPbhkMultiSphereShape);
	hashadd (RPHash, "bhkBvTreeShape", (char*)RPbhkBvTreeShape);
	hashadd (RPHash, "bhkMoppBvTreeShape", (char*)RPbhkMoppBvTreeShape);
	hashadd (RPHash, "bhkShapeCollection", (char*)RPbhkShapeCollection);
	hashadd (RPHash, "bhkListShape", (char*)RPbhkListShape);
	hashadd (RPHash, "bhkMeshShape", (char*)RPbhkMeshShape);
	hashadd (RPHash, "bhkPackedNiTriStripsShape", (char*)RPbhkPackedNiTriStripsShape);
	hashadd (RPHash, "bhkNiTriStripsShape", (char*)RPbhkNiTriStripsShape);
	hashadd (RPHash, "NiExtraData", (char*)RPNiExtraData);
	hashadd (RPHash, "NiInterpolator", (char*)RPNiInterpolator);
	hashadd (RPHash, "NiKeyBasedInterpolator", (char*)RPNiKeyBasedInterpolator);
	hashadd (RPHash, "NiFloatInterpolator", (char*)RPNiFloatInterpolator);
	hashadd (RPHash, "NiTransformInterpolator", (char*)RPNiTransformInterpolator);
	hashadd (RPHash, "NiPoint3Interpolator", (char*)RPNiPoint3Interpolator);
	hashadd (RPHash, "NiPathInterpolator", (char*)RPNiPathInterpolator);
	hashadd (RPHash, "NiBoolInterpolator", (char*)RPNiBoolInterpolator);
	hashadd (RPHash, "NiBoolTimelineInterpolator", (char*)RPNiBoolTimelineInterpolator);
	hashadd (RPHash, "NiBlendInterpolator", (char*)RPNiBlendInterpolator);
	hashadd (RPHash, "NiBSplineInterpolator", (char*)RPNiBSplineInterpolator);
	hashadd (RPHash, "NiObjectNET", (char*)RPNiObjectNET);
	hashadd (RPHash, "NiCollisionObject", (char*)RPNiCollisionObject);
	hashadd (RPHash, "NiCollisionData", (char*)RPNiCollisionData);
	hashadd (RPHash, "bhkNiCollisionObject", (char*)RPbhkNiCollisionObject);
	hashadd (RPHash, "bhkCollisionObject", (char*)RPbhkCollisionObject);
	hashadd (RPHash, "bhkBlendCollisionObject", (char*)RPbhkBlendCollisionObject);
	hashadd (RPHash, "bhkPCollisionObject", (char*)RPbhkPCollisionObject);
	hashadd (RPHash, "bhkSPCollisionObject", (char*)RPbhkSPCollisionObject);
	hashadd (RPHash, "NiAVObject", (char*)RPNiAVObject);
	hashadd (RPHash, "NiDynamicEffect", (char*)RPNiDynamicEffect);
	hashadd (RPHash, "NiLight", (char*)RPNiLight);
	hashadd (RPHash, "NiProperty", (char*)RPNiProperty);
	hashadd (RPHash, "NiTransparentProperty", (char*)RPNiTransparentProperty);
	hashadd (RPHash, "NiPSysModifier", (char*)RPNiPSysModifier);
	hashadd (RPHash, "NiPSysEmitter", (char*)RPNiPSysEmitter);
	hashadd (RPHash, "NiPSysVolumeEmitter", (char*)RPNiPSysVolumeEmitter);
	hashadd (RPHash, "NiTimeController", (char*)RPNiTimeController);
	hashadd (RPHash, "NiInterpController", (char*)RPNiInterpController);
	hashadd (RPHash, "NiMultiTargetTransformController", (char*)RPNiMultiTargetTransformController);
	hashadd (RPHash, "NiGeomMorpherController", (char*)RPNiGeomMorpherController);
	hashadd (RPHash, "NiMorphController", (char*)RPNiMorphController);
	hashadd (RPHash, "NiMorpherController", (char*)RPNiMorpherController);
	hashadd (RPHash, "NiSingleInterpController", (char*)RPNiSingleInterpController);
	hashadd (RPHash, "NiKeyframeController", (char*)RPNiKeyframeController);
	hashadd (RPHash, "NiTransformController", (char*)RPNiTransformController);
	hashadd (RPHash, "NiPSysModifierCtlr", (char*)RPNiPSysModifierCtlr);
	hashadd (RPHash, "NiPSysEmitterCtlr", (char*)RPNiPSysEmitterCtlr);
	hashadd (RPHash, "NiPSysModifierBoolCtlr", (char*)RPNiPSysModifierBoolCtlr);
	hashadd (RPHash, "NiPSysModifierActiveCtlr", (char*)RPNiPSysModifierActiveCtlr);
	hashadd (RPHash, "NiPSysModifierFloatCtlr", (char*)RPNiPSysModifierFloatCtlr);
	hashadd (RPHash, "NiPSysEmitterDeclinationCtlr", (char*)RPNiPSysEmitterDeclinationCtlr);
	hashadd (RPHash, "NiPSysEmitterDeclinationVarCtlr", (char*)RPNiPSysEmitterDeclinationVarCtlr);
	hashadd (RPHash, "NiPSysEmitterInitialRadiusCtlr", (char*)RPNiPSysEmitterInitialRadiusCtlr);
	hashadd (RPHash, "NiPSysEmitterLifeSpanCtlr", (char*)RPNiPSysEmitterLifeSpanCtlr);
	hashadd (RPHash, "NiPSysEmitterSpeedCtlr", (char*)RPNiPSysEmitterSpeedCtlr);
	hashadd (RPHash, "NiPSysGravityStrengthCtlr", (char*)RPNiPSysGravityStrengthCtlr);
	hashadd (RPHash, "NiFloatInterpController", (char*)RPNiFloatInterpController);
	hashadd (RPHash, "NiFlipController", (char*)RPNiFlipController);
	hashadd (RPHash, "NiAlphaController", (char*)RPNiAlphaController);
	hashadd (RPHash, "NiTextureTransformController", (char*)RPNiTextureTransformController);
	hashadd (RPHash, "NiLightDimmerController", (char*)RPNiLightDimmerController);
	hashadd (RPHash, "NiBoolInterpController", (char*)RPNiBoolInterpController);
	hashadd (RPHash, "NiVisController", (char*)RPNiVisController);
	hashadd (RPHash, "NiPoint3InterpController", (char*)RPNiPoint3InterpController);
	hashadd (RPHash, "NiMaterialColorController", (char*)RPNiMaterialColorController);
	hashadd (RPHash, "NiLightColorController", (char*)RPNiLightColorController);
	hashadd (RPHash, "NiExtraDataController", (char*)RPNiExtraDataController);
	hashadd (RPHash, "NiFloatExtraDataController", (char*)RPNiFloatExtraDataController);
	hashadd (RPHash, "NiBoneLODController", (char*)RPNiBoneLODController);
	hashadd (RPHash, "NiBSBoneLODController", (char*)RPNiBSBoneLODController);
	hashadd (RPHash, "NiGeometry", (char*)RPNiGeometry);
	hashadd (RPHash, "NiTriBasedGeom", (char*)RPNiTriBasedGeom);
	hashadd (RPHash, "NiGeometryData", (char*)RPNiGeometryData);
	hashadd (RPHash, "NiTriBasedGeomData", (char*)RPNiTriBasedGeomData);
	hashadd (RPHash, "bhkBlendController", (char*)RPbhkBlendController);
	hashadd (RPHash, "BSBound", (char*)RPBSBound);
	hashadd (RPHash, "BSFurnitureMarker", (char*)RPBSFurnitureMarker);
	hashadd (RPHash, "BSParentVelocityModifier", (char*)RPBSParentVelocityModifier);
	hashadd (RPHash, "BSPSysArrayEmitter", (char*)RPBSPSysArrayEmitter);
	hashadd (RPHash, "BSWindModifier", (char*)RPBSWindModifier);
	hashadd (RPHash, "hkPackedNiTriStripsData", (char*)RPhkPackedNiTriStripsData);
	hashadd (RPHash, "NiAlphaProperty", (char*)RPNiAlphaProperty);
	hashadd (RPHash, "NiAmbientLight", (char*)RPNiAmbientLight);
	hashadd (RPHash, "NiParticlesData", (char*)RPNiParticlesData);
	hashadd (RPHash, "NiRotatingParticlesData", (char*)RPNiRotatingParticlesData);
	hashadd (RPHash, "NiAutoNormalParticlesData", (char*)RPNiAutoNormalParticlesData);
	hashadd (RPHash, "ParticleDesc", (char*)RPParticleDesc);
	hashadd (RPHash, "NiPSysData", (char*)RPNiPSysData);
	hashadd (RPHash, "NiMeshPSysData", (char*)RPNiMeshPSysData);
	hashadd (RPHash, "NiBinaryExtraData", (char*)RPNiBinaryExtraData);
	hashadd (RPHash, "NiBinaryVoxelExtraData", (char*)RPNiBinaryVoxelExtraData);
	hashadd (RPHash, "NiBinaryVoxelData", (char*)RPNiBinaryVoxelData);
	hashadd (RPHash, "NiBlendBoolInterpolator", (char*)RPNiBlendBoolInterpolator);
	hashadd (RPHash, "NiBlendFloatInterpolator", (char*)RPNiBlendFloatInterpolator);
	hashadd (RPHash, "NiBlendPoint3Interpolator", (char*)RPNiBlendPoint3Interpolator);
	hashadd (RPHash, "NiBlendTransformInterpolator", (char*)RPNiBlendTransformInterpolator);
	hashadd (RPHash, "NiBoolData", (char*)RPNiBoolData);
	hashadd (RPHash, "NiBooleanExtraData", (char*)RPNiBooleanExtraData);
	hashadd (RPHash, "NiBSplineBasisData", (char*)RPNiBSplineBasisData);
	hashadd (RPHash, "NiBSplineFloatInterpolator", (char*)RPNiBSplineFloatInterpolator);
	hashadd (RPHash, "NiBSplineCompFloatInterpolator", (char*)RPNiBSplineCompFloatInterpolator);
	hashadd (RPHash, "NiBSplinePoint3Interpolator", (char*)RPNiBSplinePoint3Interpolator);
	hashadd (RPHash, "NiBSplineCompPoint3Interpolator", (char*)RPNiBSplineCompPoint3Interpolator);
	hashadd (RPHash, "NiBSplineTransformInterpolator", (char*)RPNiBSplineTransformInterpolator);
	hashadd (RPHash, "NiBSplineCompTransformInterpolator", (char*)RPNiBSplineCompTransformInterpolator);
	hashadd (RPHash, "BSRotAccumTransfInterpolator", (char*)RPBSRotAccumTransfInterpolator);
	hashadd (RPHash, "NiBSplineData", (char*)RPNiBSplineData);
	hashadd (RPHash, "NiCamera", (char*)RPNiCamera);
	hashadd (RPHash, "NiColorData", (char*)RPNiColorData);
	hashadd (RPHash, "NiColorExtraData", (char*)RPNiColorExtraData);
	hashadd (RPHash, "NiControllerManager", (char*)RPNiControllerManager);
	hashadd (RPHash, "NiSequence", (char*)RPNiSequence);
	hashadd (RPHash, "NiControllerSequence", (char*)RPNiControllerSequence);
	hashadd (RPHash, "NiAVObjectPalette", (char*)RPNiAVObjectPalette);
	hashadd (RPHash, "NiDefaultAVObjectPalette", (char*)RPNiDefaultAVObjectPalette);
	hashadd (RPHash, "NiDirectionalLight", (char*)RPNiDirectionalLight);
	hashadd (RPHash, "NiDitherProperty", (char*)RPNiDitherProperty);
	hashadd (RPHash, "NiRollController", (char*)RPNiRollController);
	hashadd (RPHash, "NiFloatData", (char*)RPNiFloatData);
	hashadd (RPHash, "NiFloatExtraData", (char*)RPNiFloatExtraData);
	hashadd (RPHash, "NiFloatsExtraData", (char*)RPNiFloatsExtraData);
	hashadd (RPHash, "NiFogProperty", (char*)RPNiFogProperty);
	hashadd (RPHash, "NiGravity", (char*)RPNiGravity);
	hashadd (RPHash, "NiIntegerExtraData", (char*)RPNiIntegerExtraData);
	hashadd (RPHash, "BSXFlags", (char*)RPBSXFlags);
	hashadd (RPHash, "NiIntegersExtraData", (char*)RPNiIntegersExtraData);
	hashadd (RPHash, "BSKeyframeController", (char*)RPBSKeyframeController);
	hashadd (RPHash, "NiKeyframeData", (char*)RPNiKeyframeData);
	hashadd (RPHash, "NiLookAtController", (char*)RPNiLookAtController);
	hashadd (RPHash, "NiLookAtInterpolator", (char*)RPNiLookAtInterpolator);
	hashadd (RPHash, "NiMaterialProperty", (char*)RPNiMaterialProperty);
	hashadd (RPHash, "NiMorphData", (char*)RPNiMorphData);
	hashadd (RPHash, "NiNode", (char*)RPNiNode);
	hashadd (RPHash, "NiBone", (char*)RPNiBone);
	hashadd (RPHash, "AvoidNode", (char*)RPAvoidNode);
	hashadd (RPHash, "FxWidget", (char*)RPFxWidget);
	hashadd (RPHash, "FxButton", (char*)RPFxButton);
	hashadd (RPHash, "FxRadioButton", (char*)RPFxRadioButton);
	hashadd (RPHash, "NiBillboardNode", (char*)RPNiBillboardNode);
	hashadd (RPHash, "NiBSAnimationNode", (char*)RPNiBSAnimationNode);
	hashadd (RPHash, "NiBSParticleNode", (char*)RPNiBSParticleNode);
	hashadd (RPHash, "NiSwitchNode", (char*)RPNiSwitchNode);
	hashadd (RPHash, "NiLODNode", (char*)RPNiLODNode);
	hashadd (RPHash, "NiPalette", (char*)RPNiPalette);
	hashadd (RPHash, "NiParticleBomb", (char*)RPNiParticleBomb);
	hashadd (RPHash, "NiParticleColorModifier", (char*)RPNiParticleColorModifier);
	hashadd (RPHash, "NiParticleGrowFade", (char*)RPNiParticleGrowFade);
	hashadd (RPHash, "NiParticleMeshModifier", (char*)RPNiParticleMeshModifier);
	hashadd (RPHash, "NiParticleRotation", (char*)RPNiParticleRotation);
	hashadd (RPHash, "NiParticles", (char*)RPNiParticles);
	hashadd (RPHash, "NiAutoNormalParticles", (char*)RPNiAutoNormalParticles);
	hashadd (RPHash, "NiParticleMeshes", (char*)RPNiParticleMeshes);
	hashadd (RPHash, "NiParticleMeshesData", (char*)RPNiParticleMeshesData);
	hashadd (RPHash, "NiParticleSystem", (char*)RPNiParticleSystem);
	hashadd (RPHash, "NiMeshParticleSystem", (char*)RPNiMeshParticleSystem);
	hashadd (RPHash, "NiParticleSystemController", (char*)RPNiParticleSystemController);
	hashadd (RPHash, "NiBSPArrayController", (char*)RPNiBSPArrayController);
	hashadd (RPHash, "NiPathController", (char*)RPNiPathController);
	hashadd (RPHash, "ChannelData", (char*)RPChannelData);
	hashadd (RPHash, "ATextureRenderData", (char*)RPATextureRenderData);
	hashadd (RPHash, "NiPersistentSrcTextureRendererData", (char*)RPNiPersistentSrcTextureRendererData);
	hashadd (RPHash, "NiPixelData", (char*)RPNiPixelData);
	hashadd (RPHash, "NiPlanarCollider", (char*)RPNiPlanarCollider);
	hashadd (RPHash, "NiPointLight", (char*)RPNiPointLight);
	hashadd (RPHash, "NiPosData", (char*)RPNiPosData);
	hashadd (RPHash, "NiPSysAgeDeathModifier", (char*)RPNiPSysAgeDeathModifier);
	hashadd (RPHash, "NiPSysBombModifier", (char*)RPNiPSysBombModifier);
	hashadd (RPHash, "NiPSysBoundUpdateModifier", (char*)RPNiPSysBoundUpdateModifier);
	hashadd (RPHash, "NiPSysBoxEmitter", (char*)RPNiPSysBoxEmitter);
	hashadd (RPHash, "NiPSysColliderManager", (char*)RPNiPSysColliderManager);
	hashadd (RPHash, "NiPSysColorModifier", (char*)RPNiPSysColorModifier);
	hashadd (RPHash, "NiPSysCylinderEmitter", (char*)RPNiPSysCylinderEmitter);
	hashadd (RPHash, "NiPSysDragModifier", (char*)RPNiPSysDragModifier);
	hashadd (RPHash, "NiPSysEmitterCtlrData", (char*)RPNiPSysEmitterCtlrData);
	hashadd (RPHash, "NiPSysGravityModifier", (char*)RPNiPSysGravityModifier);
	hashadd (RPHash, "NiPSysGrowFadeModifier", (char*)RPNiPSysGrowFadeModifier);
	hashadd (RPHash, "NiPSysMeshEmitter", (char*)RPNiPSysMeshEmitter);
	hashadd (RPHash, "NiPSysMeshUpdateModifier", (char*)RPNiPSysMeshUpdateModifier);
	hashadd (RPHash, "NiPSysPlanarCollider", (char*)RPNiPSysPlanarCollider);
	hashadd (RPHash, "NiPSysSphericalCollider", (char*)RPNiPSysSphericalCollider);
	hashadd (RPHash, "NiPSysPositionModifier", (char*)RPNiPSysPositionModifier);
	hashadd (RPHash, "NiPSysResetOnLoopCtlr", (char*)RPNiPSysResetOnLoopCtlr);
	hashadd (RPHash, "NiPSysRotationModifier", (char*)RPNiPSysRotationModifier);
	hashadd (RPHash, "NiPSysSpawnModifier", (char*)RPNiPSysSpawnModifier);
	hashadd (RPHash, "NiPSysSphereEmitter", (char*)RPNiPSysSphereEmitter);
	hashadd (RPHash, "NiPSysUpdateCtlr", (char*)RPNiPSysUpdateCtlr);
	hashadd (RPHash, "NiPSysFieldModifier", (char*)RPNiPSysFieldModifier);
	hashadd (RPHash, "NiPSysVortexFieldModifier", (char*)RPNiPSysVortexFieldModifier);
	hashadd (RPHash, "NiPSysGravityFieldModifier", (char*)RPNiPSysGravityFieldModifier);
	hashadd (RPHash, "NiPSysDragFieldModifier", (char*)RPNiPSysDragFieldModifier);
	hashadd (RPHash, "NiPSysTurbulenceFieldModifier", (char*)RPNiPSysTurbulenceFieldModifier);
	hashadd (RPHash, "NiPSysFieldMagnitudeCtlr", (char*)RPNiPSysFieldMagnitudeCtlr);
	hashadd (RPHash, "NiPSysFieldAttenuationCtlr", (char*)RPNiPSysFieldAttenuationCtlr);
	hashadd (RPHash, "NiPSysFieldMaxDistanceCtlr", (char*)RPNiPSysFieldMaxDistanceCtlr);
	hashadd (RPHash, "NiPSysAirFieldAirFrictionCtlr", (char*)RPNiPSysAirFieldAirFrictionCtlr);
	hashadd (RPHash, "NiPSysAirFieldInheritVelocityCtlr", (char*)RPNiPSysAirFieldInheritVelocityCtlr);
	hashadd (RPHash, "NiPSysAirFieldSpreadCtlr", (char*)RPNiPSysAirFieldSpreadCtlr);
	hashadd (RPHash, "NiPSysInitialRotSpeedCtlr", (char*)RPNiPSysInitialRotSpeedCtlr);
	hashadd (RPHash, "NiPSysInitialRotSpeedVarCtlr", (char*)RPNiPSysInitialRotSpeedVarCtlr);
	hashadd (RPHash, "NiPSysInitialRotAngleCtlr", (char*)RPNiPSysInitialRotAngleCtlr);
	hashadd (RPHash, "NiPSysInitialRotAngleVarCtlr", (char*)RPNiPSysInitialRotAngleVarCtlr);
	hashadd (RPHash, "NiPSysEmitterPlanarAngleCtlr", (char*)RPNiPSysEmitterPlanarAngleCtlr);
	hashadd (RPHash, "NiPSysEmitterPlanarAngleVarCtlr", (char*)RPNiPSysEmitterPlanarAngleVarCtlr);
	hashadd (RPHash, "NiPSysAirFieldModifier", (char*)RPNiPSysAirFieldModifier);
	hashadd (RPHash, "NiPSysTrailEmitter", (char*)RPNiPSysTrailEmitter);
	hashadd (RPHash, "NiLightIntensityController", (char*)RPNiLightIntensityController);
	hashadd (RPHash, "NiPSysRadialFieldModifier", (char*)RPNiPSysRadialFieldModifier);
	hashadd (RPHash, "NiLODData", (char*)RPNiLODData);
	hashadd (RPHash, "NiRangeLODData", (char*)RPNiRangeLODData);
	hashadd (RPHash, "NiScreenLODData", (char*)RPNiScreenLODData);
	hashadd (RPHash, "NiRotatingParticles", (char*)RPNiRotatingParticles);
	hashadd (RPHash, "NiSequenceStreamHelper", (char*)RPNiSequenceStreamHelper);
	hashadd (RPHash, "NiShadeProperty", (char*)RPNiShadeProperty);
	hashadd (RPHash, "NiSkinData", (char*)RPNiSkinData);
	hashadd (RPHash, "NiSkinInstance", (char*)RPNiSkinInstance);
	hashadd (RPHash, "NiTriShapeSkinController", (char*)RPNiTriShapeSkinController);
	hashadd (RPHash, "NiClodSkinInstance", (char*)RPNiClodSkinInstance);
	hashadd (RPHash, "NiSkinPartition", (char*)RPNiSkinPartition);
	hashadd (RPHash, "NiTexture", (char*)RPNiTexture);
	hashadd (RPHash, "NiSourceTexture", (char*)RPNiSourceTexture);
	hashadd (RPHash, "NiSpecularProperty", (char*)RPNiSpecularProperty);
	hashadd (RPHash, "NiSphericalCollider", (char*)RPNiSphericalCollider);
	hashadd (RPHash, "NiSpotLight", (char*)RPNiSpotLight);
	hashadd (RPHash, "NiStencilProperty", (char*)RPNiStencilProperty);
	hashadd (RPHash, "NiStringExtraData", (char*)RPNiStringExtraData);
	hashadd (RPHash, "NiStringPalette", (char*)RPNiStringPalette);
	hashadd (RPHash, "NiStringsExtraData", (char*)RPNiStringsExtraData);
	hashadd (RPHash, "NiTextKeyExtraData", (char*)RPNiTextKeyExtraData);
	hashadd (RPHash, "NiTextureEffect", (char*)RPNiTextureEffect);
	hashadd (RPHash, "NiTextureModeProperty", (char*)RPNiTextureModeProperty);
	hashadd (RPHash, "NiImage", (char*)RPNiImage);
	hashadd (RPHash, "NiTextureProperty", (char*)RPNiTextureProperty);
	hashadd (RPHash, "NiMultiTextureProperty", (char*)RPNiMultiTextureProperty);
	hashadd (RPHash, "NiTexturingProperty", (char*)RPNiTexturingProperty);
	hashadd (RPHash, "NiTransformData", (char*)RPNiTransformData);
	hashadd (RPHash, "NiTriShape", (char*)RPNiTriShape);
	hashadd (RPHash, "NiTriShapeData", (char*)RPNiTriShapeData);
	hashadd (RPHash, "NiTriStrips", (char*)RPNiTriStrips);
	hashadd (RPHash, "NiTriStripsData", (char*)RPNiTriStripsData);
	hashadd (RPHash, "NiEnvMappedTriShape", (char*)RPNiEnvMappedTriShape);
	hashadd (RPHash, "NiEnvMappedTriShapeData", (char*)RPNiEnvMappedTriShapeData);
	hashadd (RPHash, "NiBezierTriangle4", (char*)RPNiBezierTriangle4);
	hashadd (RPHash, "NiBezierMesh", (char*)RPNiBezierMesh);
	hashadd (RPHash, "NiClod", (char*)RPNiClod);
	hashadd (RPHash, "NiClodData", (char*)RPNiClodData);
	hashadd (RPHash, "NiUVController", (char*)RPNiUVController);
	hashadd (RPHash, "NiUVData", (char*)RPNiUVData);
	hashadd (RPHash, "NiVectorExtraData", (char*)RPNiVectorExtraData);
	hashadd (RPHash, "NiVertexColorProperty", (char*)RPNiVertexColorProperty);
	hashadd (RPHash, "NiVertWeightsExtraData", (char*)RPNiVertWeightsExtraData);
	hashadd (RPHash, "NiVisData", (char*)RPNiVisData);
	hashadd (RPHash, "NiWireframeProperty", (char*)RPNiWireframeProperty);
	hashadd (RPHash, "NiZBufferProperty", (char*)RPNiZBufferProperty);
	hashadd (RPHash, "RootCollisionNode", (char*)RPRootCollisionNode);
	hashadd (RPHash, "NiRawImageData", (char*)RPNiRawImageData);
	hashadd (RPHash, "NiSortAdjustNode", (char*)RPNiSortAdjustNode);
	hashadd (RPHash, "NiSourceCubeMap", (char*)RPNiSourceCubeMap);
	hashadd (RPHash, "NiPhysXProp", (char*)RPNiPhysXProp);
	hashadd (RPHash, "physXMaterialRef", (char*)RPphysXMaterialRef);
	hashadd (RPHash, "NiPhysXPropDesc", (char*)RPNiPhysXPropDesc);
	hashadd (RPHash, "NiPhysXActorDesc", (char*)RPNiPhysXActorDesc);
	hashadd (RPHash, "NiPhysXBodyDesc", (char*)RPNiPhysXBodyDesc);
	hashadd (RPHash, "NiPhysXD6JointDesc", (char*)RPNiPhysXD6JointDesc);
	hashadd (RPHash, "NiPhysXShapeDesc", (char*)RPNiPhysXShapeDesc);
	hashadd (RPHash, "NiPhysXMeshDesc", (char*)RPNiPhysXMeshDesc);
	hashadd (RPHash, "NiPhysXMaterialDesc", (char*)RPNiPhysXMaterialDesc);
	hashadd (RPHash, "NiPhysXKinematicSrc", (char*)RPNiPhysXKinematicSrc);
	hashadd (RPHash, "NiPhysXTransformDest", (char*)RPNiPhysXTransformDest);
	hashadd (RPHash, "NiArkAnimationExtraData", (char*)RPNiArkAnimationExtraData);
	hashadd (RPHash, "NiArkImporterExtraData", (char*)RPNiArkImporterExtraData);
	hashadd (RPHash, "NiArkTextureExtraData", (char*)RPNiArkTextureExtraData);
	hashadd (RPHash, "NiArkViewportInfoExtraData", (char*)RPNiArkViewportInfoExtraData);
	hashadd (RPHash, "NiArkShaderExtraData", (char*)RPNiArkShaderExtraData);
	hashadd (RPHash, "NiLines", (char*)RPNiLines);
	hashadd (RPHash, "NiLinesData", (char*)RPNiLinesData);
	hashadd (RPHash, "Polygon", (char*)RPPolygon);
	hashadd (RPHash, "NiScreenElementsData", (char*)RPNiScreenElementsData);
	hashadd (RPHash, "NiScreenElements", (char*)RPNiScreenElements);
	hashadd (RPHash, "NiRoomGroup", (char*)RPNiRoomGroup);
	hashadd (RPHash, "NiRoom", (char*)RPNiRoom);
	hashadd (RPHash, "NiPortal", (char*)RPNiPortal);
	hashadd (RPHash, "BSFadeNode", (char*)RPBSFadeNode);
	hashadd (RPHash, "BSShaderProperty", (char*)RPBSShaderProperty);
	hashadd (RPHash, "BSShaderLightingProperty", (char*)RPBSShaderLightingProperty);
	hashadd (RPHash, "BSShaderNoLightingProperty", (char*)RPBSShaderNoLightingProperty);
	hashadd (RPHash, "BSShaderPPLightingProperty", (char*)RPBSShaderPPLightingProperty);
	hashadd (RPHash, "BSShaderTextureSet", (char*)RPBSShaderTextureSet);
	hashadd (RPHash, "WaterShaderProperty", (char*)RPWaterShaderProperty);
	hashadd (RPHash, "SkyShaderProperty", (char*)RPSkyShaderProperty);
	hashadd (RPHash, "TileShaderProperty", (char*)RPTileShaderProperty);
	hashadd (RPHash, "DistantLODShaderProperty", (char*)RPDistantLODShaderProperty);
	hashadd (RPHash, "BSDistantTreeShaderProperty", (char*)RPBSDistantTreeShaderProperty);
	hashadd (RPHash, "TallGrassShaderProperty", (char*)RPTallGrassShaderProperty);
	hashadd (RPHash, "VolumetricFogShaderProperty", (char*)RPVolumetricFogShaderProperty);
	hashadd (RPHash, "HairShaderProperty", (char*)RPHairShaderProperty);
	hashadd (RPHash, "Lighting30ShaderProperty", (char*)RPLighting30ShaderProperty);
	hashadd (RPHash, "BSDismemberSkinInstance", (char*)RPBSDismemberSkinInstance);
	hashadd (RPHash, "BSDecalPlacementVectorExtraData", (char*)RPBSDecalPlacementVectorExtraData);
	hashadd (RPHash, "BSPSysSimpleColorModifier", (char*)RPBSPSysSimpleColorModifier);
	hashadd (RPHash, "BSValueNode", (char*)RPBSValueNode);
	hashadd (RPHash, "BSStripParticleSystem", (char*)RPBSStripParticleSystem);
	hashadd (RPHash, "BSStripPSysData", (char*)RPBSStripPSysData);
	hashadd (RPHash, "BSPSysStripUpdateModifier", (char*)RPBSPSysStripUpdateModifier);
	hashadd (RPHash, "BSMaterialEmittanceMultController", (char*)RPBSMaterialEmittanceMultController);
	hashadd (RPHash, "BSMasterParticleSystem", (char*)RPBSMasterParticleSystem);
	hashadd (RPHash, "BSPSysMultiTargetEmitterCtlr", (char*)RPBSPSysMultiTargetEmitterCtlr);
	hashadd (RPHash, "BSRefractionStrengthController", (char*)RPBSRefractionStrengthController);
	hashadd (RPHash, "BSOrderedNode", (char*)RPBSOrderedNode);
	hashadd (RPHash, "BSBlastNode", (char*)RPBSBlastNode);
	hashadd (RPHash, "BSDamageStage", (char*)RPBSDamageStage);
	hashadd (RPHash, "BSRefractionFirePeriodController", (char*)RPBSRefractionFirePeriodController);
	hashadd (RPHash, "bhkConvexListShape", (char*)RPbhkConvexListShape);
	hashadd (RPHash, "BSTreadTransfSubInfo", (char*)RPBSTreadTransfSubInfo);
	hashadd (RPHash, "BSTreadTransfInfo", (char*)RPBSTreadTransfInfo);
	hashadd (RPHash, "BSTreadTransfInterpolator", (char*)RPBSTreadTransfInterpolator);
	hashadd (RPHash, "BSAnimNotes", (char*)RPBSAnimNotes);
	hashadd (RPHash, "bhkLiquidAction", (char*)RPbhkLiquidAction);
	hashadd (RPHash, "BSMultiBoundNode", (char*)RPBSMultiBoundNode);
	hashadd (RPHash, "BSMultiBound", (char*)RPBSMultiBound);
	hashadd (RPHash, "BSMultiBoundData", (char*)RPBSMultiBoundData);
	hashadd (RPHash, "BSMultiBoundSphere", (char*)RPBSMultiBoundSphere);
	hashadd (RPHash, "BSSegmentedTriangle", (char*)RPBSSegmentedTriangle);
	hashadd (RPHash, "BSSegmentedTriShape", (char*)RPBSSegmentedTriShape);
	hashadd (RPHash, "BSMultiBoundAABB", (char*)RPBSMultiBoundAABB);
	hashadd (RPHash, "AdditionalDataInfo", (char*)RPAdditionalDataInfo);
	hashadd (RPHash, "AdditionalDataBlock", (char*)RPAdditionalDataBlock);
	hashadd (RPHash, "NiAdditionalGeometryData", (char*)RPNiAdditionalGeometryData);
	hashadd (RPHash, "BSWArray", (char*)RPBSWArray);
	hashadd (RPHash, "bhkAabbPhantom", (char*)RPbhkAabbPhantom);
	hashadd (RPHash, "BSFrustumFOVController", (char*)RPBSFrustumFOVController);
	hashadd (RPHash, "BSDebrisNode", (char*)RPBSDebrisNode);
	hashadd (RPHash, "bhkBreakableConstraint", (char*)RPbhkBreakableConstraint);
	hashadd (RPHash, "bhkOrientHingedBodyAction", (char*)RPbhkOrientHingedBodyAction);
	hashadd (RPHash, "Region", (char*)RPRegion);
	hashadd (RPHash, "NiDataStream", (char*)RPNiDataStream);
	hashadd (RPHash, "SemanticData", (char*)RPSemanticData);
	hashadd (RPHash, "MeshData", (char*)RPMeshData);
	hashadd (RPHash, "MaterialData", (char*)RPMaterialData);
	hashadd (RPHash, "NiRenderObject", (char*)RPNiRenderObject);
	hashadd (RPHash, "NiMeshModifier", (char*)RPNiMeshModifier);
	hashadd (RPHash, "NiMesh", (char*)RPNiMesh);
	hashadd (RPHash, "NiMorphWeightsController", (char*)RPNiMorphWeightsController);
	hashadd (RPHash, "ElementReference", (char*)RPElementReference);
	hashadd (RPHash, "NiMorphMeshModifier", (char*)RPNiMorphMeshModifier);
	hashadd (RPHash, "NiSkinningMeshModifier", (char*)RPNiSkinningMeshModifier);
	hashadd (RPHash, "NiInstancingMeshModifier", (char*)RPNiInstancingMeshModifier);
	hashadd (RPHash, "NiSkinningLODController", (char*)RPNiSkinningLODController);
	hashadd (RPHash, "NiPSParticleSystem", (char*)RPNiPSParticleSystem);
	hashadd (RPHash, "NiPSMeshParticleSystem", (char*)RPNiPSMeshParticleSystem);
	hashadd (RPHash, "NiPSEmitParticlesCtlr", (char*)RPNiPSEmitParticlesCtlr);
	hashadd (RPHash, "NiPSForceActiveCtlr", (char*)RPNiPSForceActiveCtlr);
	hashadd (RPHash, "NiPSSimulator", (char*)RPNiPSSimulator);
	hashadd (RPHash, "NiPSSimulatorStep", (char*)RPNiPSSimulatorStep);
	hashadd (RPHash, "NiPSSimulatorGeneralStep", (char*)RPNiPSSimulatorGeneralStep);
	hashadd (RPHash, "NiPSSimulatorForcesStep", (char*)RPNiPSSimulatorForcesStep);
	hashadd (RPHash, "NiPSSimulatorCollidersStep", (char*)RPNiPSSimulatorCollidersStep);
	hashadd (RPHash, "NiPSSimulatorMeshAlignStep", (char*)RPNiPSSimulatorMeshAlignStep);
	hashadd (RPHash, "NiPSSimulatorFinalStep", (char*)RPNiPSSimulatorFinalStep);
	hashadd (RPHash, "NiPSFacingQuadGenerator", (char*)RPNiPSFacingQuadGenerator);
	hashadd (RPHash, "NiShadowGenerator", (char*)RPNiShadowGenerator);
	hashadd (RPHash, "NiPSBoundUpdater", (char*)RPNiPSBoundUpdater);
	hashadd (RPHash, "NiPSDragForce", (char*)RPNiPSDragForce);
	hashadd (RPHash, "NiPSGravityForce", (char*)RPNiPSGravityForce);
	hashadd (RPHash, "NiPSBoxEmitter", (char*)RPNiPSBoxEmitter);
	hashadd (RPHash, "NiPSMeshEmitter", (char*)RPNiPSMeshEmitter);
	hashadd (RPHash, "NiPSGravityStrengthCtlr", (char*)RPNiPSGravityStrengthCtlr);
	hashadd (RPHash, "NiPSPlanarCollider", (char*)RPNiPSPlanarCollider);
	hashadd (RPHash, "NiPSEmitterSpeedCtlr", (char*)RPNiPSEmitterSpeedCtlr);
	hashadd (RPHash, "NiPSEmitterRadiusCtlr", (char*)RPNiPSEmitterRadiusCtlr);
	hashadd (RPHash, "NiPSResetOnLoopCtlr", (char*)RPNiPSResetOnLoopCtlr);
	hashadd (RPHash, "NiPSSphereEmitter", (char*)RPNiPSSphereEmitter);
	hashadd (RPHash, "NiPSCylinderEmitter", (char*)RPNiPSCylinderEmitter);
	hashadd (RPHash, "NiPSEmitterDeclinationCtlr", (char*)RPNiPSEmitterDeclinationCtlr);
	hashadd (RPHash, "NiPSEmitterDeclinationVarCtlr", (char*)RPNiPSEmitterDeclinationVarCtlr);
	hashadd (RPHash, "NiPSEmitterPlanarAngleCtlr", (char*)RPNiPSEmitterPlanarAngleCtlr);
	hashadd (RPHash, "NiPSEmitterPlanarAngleVarCtlr", (char*)RPNiPSEmitterPlanarAngleVarCtlr);
	hashadd (RPHash, "NiPSEmitterRotAngleCtlr", (char*)RPNiPSEmitterRotAngleCtlr);
	hashadd (RPHash, "NiPSEmitterRotAngleVarCtlr", (char*)RPNiPSEmitterRotAngleVarCtlr);
	hashadd (RPHash, "NiPSEmitterRotSpeedCtlr", (char*)RPNiPSEmitterRotSpeedCtlr);
	hashadd (RPHash, "NiPSEmitterRotSpeedVarCtlr", (char*)RPNiPSEmitterRotSpeedVarCtlr);
	hashadd (RPHash, "NiPSEmitterLifeSpanCtlr", (char*)RPNiPSEmitterLifeSpanCtlr);
	hashadd (RPHash, "NiPSBombForce", (char*)RPNiPSBombForce);
	hashadd (RPHash, "NiPSSphericalCollider", (char*)RPNiPSSphericalCollider);
	hashadd (RPHash, "NiPSSpawner", (char*)RPNiPSSpawner);
	hashadd (RPHash, "NiSequenceData", (char*)RPNiSequenceData);
	hashadd (RPHash, "NiTransformEvaluator", (char*)RPNiTransformEvaluator);
	hashadd (RPHash, "NiBSplineCompTransformEvaluator", (char*)RPNiBSplineCompTransformEvaluator);
	hashadd (RPHash, "NiMeshHWInstance", (char*)RPNiMeshHWInstance);
	hashadd (RPHash, "NiFurSpringController", (char*)RPNiFurSpringController);
}
static void
ReleaseRpHash()
{
	hashrelease (RPHash, 0);
}

			/*
   		---/  call read proc by name / /---
					             ---/*/

static void
ReadNifObject(char *name)
{
	//dbg ("reading NIFOBJECT \"%s\"\n", name);
	NifObjRProc rproc = (NifObjRProc)hashget (RPHash, name );
	if (!rproc) {
		NPFF (ENIF, "*** unknown nif block \"%s\"", name);
		return;
	}
	rproc ();
	NIFF_OBJECT_COUNT++;
	//TellFilePos (READER);
}

		 /*|
   		---|  end of hashed reader  | \---
					             ---|*/

static void
TellFilePos(READERARGLST)
{
	long pos = ftell (r->f) - r->buf_len + r->buf_pos;
	dbg (" fptr is at 0x%.8X, buf_len:%d, buf_pos:%d \n",
		pos, r->buf_len, r->buf_pos);
}
			/*
   		---/   binary reader   / /---
					       ---/*/

static int BDBG = 0;
#define INFO(m,f,val) if (BDBG) dbg ("  "m": "f"\n", val)

static inline byte
readbool(READERARGLST)
{
	// A boolean; 32-bit from 4.0.0.2, and 8-bit from 4.1.0.1 on.
	if (Version > 0x04010001) {
		byte b = readbyte (READER);
		INFO (" readbool", "%d", b);
		return b;
	}
	else {
		unsigned int b;
		int rr = r->read_uint (S, &b, 1);
		if (rr != 4) {
			NPF ("readbool: EOF", EIO);
			return 0;
		}
		INFO ("readbool[]", "%d", b);
		return (byte)(b != 0);
	}
}

static inline byte
readbyte(READERARGLST)
{
	byte b;
	int rr = r->read_byte (S, &b, 1);
	if (rr != 1) {
		NPF ("readbyte: EOF", EIO);
		return 0;
	}
	INFO ("readbyte", "%d", b);
	return b;
}

static inline unsigned int
readuint(READERARGLST)
{
	unsigned int b;
	int rr = r->read_uint (S, &b, 1);
	if (rr != 4) {
		NPF ("readuint: EOF", EIO);
		return 0;
	}
	INFO ("readuint", "%d", b);
	return b;
}

static inline unsigned short
readushort(READERARGLST)
{
	unsigned short b;
	int rr = r->read_ushort (S, &b, 1);
	if (rr != 2) {
		NPF ("readushort: EOF", EIO);
		return 0;
	}
	INFO ("readushort", "%d", b);
	return b;
}

static inline int
readint(READERARGLST)
{
	int b;
	int rr = r->read_int (S, &b, 1);
	if (rr != 4) {
		NPF ("readint: EOF", EIO);
		return 0;
	}
	INFO ("readint", "%d", b);
	return b;
}

static inline short
readshort(READERARGLST)
{
	short b;
	int rr = r->read_short (S, &b, 1);
	if (rr != 2) {
		NPF ("readshort: EOF", EIO);
		return 0;
	}
	INFO ("readshort", "%d", b);
	return b;
}

static inline unsigned short
readBlockTypeIndex(READERARGLST)
{
	unsigned short b = readushort (READER);
	INFO (" readBlockTypeIndex", "%d", b);
	return b;
}

static inline char
readchar(READERARGLST)
{
	unsigned char b;
	// TODO: fix the entire thing (change to NIFchar)
	int rr = r->read_char (S, &b, 1);
	if (rr != 1) {
		NPF ("readchar: EOF", EIO);
		return '\0';
	}
	INFO ("readchar", "%c", b);
	return (char)b;
}

static inline unsigned int
readFileVersion(READERARGLST)
{
	unsigned int b = readuint (READER);
	INFO (" readFileVersion", "%d", b);
	return b;
}

static inline unsigned short
readFlags(READERARGLST)
{
	unsigned short b = readushort (READER);
	INFO (" readFlags", "%d", b);
	return b;
}

static inline float
readfloat(READERARGLST)
{
	float b;
	int rr = r->read_float (S, &b, 1);
	if (rr != 4) {
		NPF ("readfloat: EOF", EIO);
		return 0;
	}
	INFO ("readfloat", "%.2f", b);
	return b;
}

static inline char *
readHeaderString(READERARGLST)
{
	//'\n' terminated string
	const int BS = 1024;
	unsigned char buf[BS];
	int rr = r->read_char_cond (S, &buf[0], BS, '\n');
	if (rr <= 0 || buf[rr-1] != '\n') {
		NPF ("readHeaderString: EOF reached\n", EIO);
		return NULL;
	}
	if (rr == BS - 1) {
		NPF ("readHeaderString: buffer overflow\n", ENIF);
		return NULL;
	}
	char *result = GETMEM (rr + 1);// chars\n\0
	if (!result) {
		NPF ("readHeaderString: malloc failed\n", EM);
		return NULL;
	}
	memcpy (&result[0], &buf[0], rr);
	result[rr] = '\0';
	INFO ("readHeaderString", "%s", result);
	return result;
}

static inline char *
readLineString(READERARGLST)
{
	char *result = readHeaderString (READER);
	INFO ("readLineString", "%s", result);
	return result;
}

static inline unsigned int
readPtr(READERARGLST)
{
	unsigned int b = readuint (READER);
	INFO (" readPtr", "%d", b);
	return b;
}

static inline unsigned int
readRef(READERARGLST)
{
	unsigned int b = readuint (READER);
	INFO (" readRef", "%d", b);
	return b;
}

static inline unsigned int
readStringOffset(READERARGLST)
{
	unsigned int b = readuint (READER);
	INFO (" readStringOffset", "%d", b);
	return b;
}

static inline unsigned int
readStringIndex(READERARGLST)
{
	unsigned int b = readuint (READER);
	INFO (" readStringIndex", "%d", b);
	return b;
}

			/*/-
   		---/   nif types reader   / /---
					       ---/*//*/*/

#define PROTECT_LEN(LEN, MAX, PROC)\
	if ((LEN) > (MAX)) {\
		NPFF (ENIF,	"*** "PROC" invalid length read:%d\n", LEN);\
		return;\
	}

#define READ(TYPE, BUF, LEN, BYTES, PROC)\
	{\
		int r = R->read_##TYPE (S, BUF, LEN);\
		if (r != (LEN * BYTES))\
			NPF (PROC": EOF reached\n", EIO);\
	}

inline void
readSizedString(SizedString * obj, unsigned int ARG)
{
	COVERAGE
	obj->Length = readuint (READER);
	PROTECT_LEN (obj->Length, 0xffff, "SizedString")
	obj->Value = GETMEM (obj->Length + 1);
	READ (char, obj->Value, obj->Length, 1, "SizedString")
	obj->Value[obj->Length] = '\0';// terminate it !
}

inline void
readstring(string * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x14000005)) {
		readSizedString (&obj->String, 0);
	}
	if (VersionCheck (0x14010003, 0)) {
		obj->Index = readStringIndex (READER);
	}
}

void
readByteArray(ByteArray * obj, unsigned int ARG)
{
	COVERAGE
	obj->Data_Size = readuint (READER);
	PROTECT_LEN (obj->Data_Size, 2000000, "readByteArray")
	obj->Data = GETMEM (obj->Data_Size);
	READ (byte, obj->Data, obj->Data_Size, 1, "readByteArray")
}

void
readByteMatrix(ByteMatrix * obj, unsigned int ARG)
{
	COVERAGE
	obj->Data_Size_1 = readuint (READER);
	PROTECT_LEN (obj->Data_Size_1, 0xff, "readByteMatrix")
	obj->Data_Size_2 = readuint (READER);
	PROTECT_LEN (obj->Data_Size_2, 0xff, "readByteMatrix")
	int len = obj->Data_Size_1 * obj->Data_Size_2;
	obj->Data = GETMEM (len);
	READ (byte, obj->Data, len, 1, "readByteMatrix")
}

inline void
readColor3(Color3 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->r), 3, SIZEOFDWORD, "readColor3")
}

inline void
readByteColor3(ByteColor3 * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->r), 3, 1, "readByteColor3")
}

inline void
readColor4(Color4 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->r), 4, SIZEOFDWORD, "readColor4")
}

inline void
readByteColor4(ByteColor4 * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->r), 4, 1, "readByteColor4")
}

void
readFilePath(FilePath * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x14000005)) {
		readSizedString (&obj->String, 0);
	}
	if (VersionCheck (0x14010003, 0)) {
		obj->Index = readStringIndex (READER);
	}
}

void
readFooter(Footer * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x0303000D, 0)) {
		obj->Num_Roots = readuint (READER);
		PROTECT_LEN (obj->Num_Roots, 0xff, "readFooter")
		obj->Roots = GETMEM ((obj->Num_Roots) * sizeof (NIFuint));
		READ (uint, obj->Roots, obj->Num_Roots, SIZEOFDWORD, "readFooter")
	}
}

void
readLODRange(LODRange * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->Near_Extent), 2, SIZEOFDWORD, "readLODRange")
	/*obj->Near_Extent = readfloat (READER);
	obj->Far_Extent = readfloat (READER);*/
	if (VersionCheck (0, 0x03010000)) {
		READ (uint, &(obj->Unknown_Ints[0]), 3, SIZEOFDWORD, "readLODRange")
	}
}

inline void
readMatchGroup(MatchGroup * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Vertices = readushort (READER);
	obj->Vertex_Indices =
	    GETMEM ((obj->Num_Vertices) * sizeof (NIFushort));
	READ (ushort, obj->Vertex_Indices, obj->Num_Vertices, SIZEOFWORD, "readMatchGroup")
}

inline void
readVector3(Vector3 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->x), 3, SIZEOFDWORD, "readVector3")
}

inline void
readVector4(Vector4 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->x), 4, SIZEOFDWORD, "readVector4")
}

inline void
readQuaternion(Quaternion * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->x), 4, SIZEOFDWORD, "readQuaternion")
}

inline void
readQuaternionXYZW(QuaternionXYZW * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->x), 4, SIZEOFDWORD, "readQuaternionXYZW")
}

inline void
readMatrix22(Matrix22 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->m11), 4, SIZEOFDWORD, "readMatrix22")
}

inline void
readMatrix33(Matrix33 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->m11), 9, SIZEOFDWORD, "readMatrix33")
}

inline void
readMatrix44(Matrix44 * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->m11), 16, SIZEOFDWORD, "readMatrix44")
}

void
readMipMap(MipMap * obj, unsigned int ARG)
{
	COVERAGE
	READ (uint, &(obj->Width), 3, SIZEOFDWORD, "readMipMap")
}

void
readNodeGroup(NodeGroup * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Nodes = readuint (READER);
	PROTECT_LEN (obj->Num_Nodes, 2000000, "readNodeGroup")
	obj->Nodes = GETMEM ((obj->Num_Nodes) * sizeof (unsigned int));
	READ (uint, obj->Nodes, obj->Num_Nodes, SIZEOFDWORD, "readNodeGroup")
}

void
readShortString(ShortString * obj, unsigned int ARG)
{
	COVERAGE
	obj->Length = readbyte (READER);
	obj->Value = GETMEM (obj->Length);
	READ (char, obj->Value, obj->Length, 1, "readShortString")
}

inline void
readSkinShape(SkinShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->Shape = readPtr (READER);
	obj->Skin_Instance = readRef (READER);
}

void
readSkinShapeGroup(SkinShapeGroup * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Link_Pairs = readuint (READER);
	PROTECT_LEN (obj->Num_Link_Pairs, 2000000, "readSkinShapeGroup")
	// init 1d array
	obj->Link_Pairs = GETMEM ((obj->Num_Link_Pairs) * sizeof (SkinShape));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Link_Pairs); i++)
		readSkinShape (&obj->Link_Pairs[i], 0);
}

inline void
readSkinWeight(SkinWeight * obj, unsigned int ARG)
{
	COVERAGE
	obj->Index = readushort (READER);
	obj->Weight = readfloat (READER);
}

void
readAVObject(AVObject * obj, unsigned int ARG)
{
	COVERAGE
	readSizedString (&obj->Name, 0);
	obj->AV_Object = readPtr (READER);
}

void
readControllerLink(ControllerLink * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x0A010000)) {
		readstring (&obj->Target_Name, 0);
		obj->Controller = readRef (READER);
	}
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Interpolator = readRef (READER);
		obj->Controller = readRef (READER);
	}
	if (VersionCheck (0x0A01006A, 0x0A01006A))
	{
		obj->Unknown_Link_2 = readRef (READER);
		obj->Unknown_Short_0 = readushort (READER);
	}
	if (VersionCheck (0x0A01006A, 0)
	    && ((User_Version == 10) || (User_Version == 11))) {
		obj->Priority = readbyte (READER);
	}
	if (VersionCheck (0x0A020000, 0x14000005)) {
		obj->String_Palette = readRef (READER);
		obj->Node_Name_Offset = readStringOffset (READER);
		obj->Property_Type_Offset = readStringOffset (READER);
		obj->Controller_Type_Offset = readStringOffset (READER);
		obj->Variable_1_Offset = readStringOffset (READER);
		obj->Variable_2_Offset = readStringOffset (READER);
	}
	if (VersionCheck (0x0A01006A, 0x0A01006A) ||
		VersionCheck (0x14010003, 0) ) {
		readstring (&obj->Node_Name, 0);
		readstring (&obj->Property_Type, 0);
		readstring (&obj->Controller_Type, 0);
		readstring (&obj->Variable_1, 0);
		readstring (&obj->Variable_2, 0);
	}
}

void
readExportInfo(ExportInfo * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x0A000102))
		obj->Unknown = readuint (READER);
	readShortString (&obj->Creator, 0);
	readShortString (&obj->Export_Info_1, 0);
	readShortString (&obj->Export_Info_2, 0);
}

void
readHeader(Header *obj, unsigned int ARG)
{
	COVERAGE
	int i;
	obj->Header_String = readHeaderString (READER);
	InitVersion (obj->Header_String);
	if (VersionCheck (0, 0x03010000)) {
		for (i = 0; i < (3); i++)
			obj->Copyright[i] = readLineString (READER);
	}
	if (VersionCheck (0x0303000D, 0))
		obj->Version = readFileVersion (READER);
	if (VersionCheck (0x14000004, 0))
		obj->Endian_Type = readbyte (READER);
	if (VersionCheck (0x0A010000, 0)) {
		obj->User_Version = readuint (READER);
		User_Version = obj->User_Version;
	}
	if (VersionCheck (0x0303000D, 0)) {
		obj->Num_Blocks = readuint (READER);
		PROTECT_LEN (obj->Num_Blocks, 2000000, "readHeader")
	}
	if (VersionCheck (0x0A010000, 0)
	    && ((User_Version == 10) || (User_Version == 11) ||
		(User_Version == 1))) {
		obj->User_Version_2 = readuint (READER);
		User_Version_2 = obj->User_Version_2;
	}
	if (VersionCheck (0x1E000002, 0))
		obj->Unknown_Int_3 = readuint (READER);
	if (VersionCheck (0x0A000102, 0x0A000102))
		readExportInfo (&obj->Export_Info, 0);
	else if (VersionCheck (0x0A010000, 0) &&
		((User_Version == 10) || (User_Version == 11) ||
		(User_Version == 1)))
		readExportInfo (&obj->Export_Info, 0);
	if (VersionCheck (0x0A000100, 0)) {
		obj->Num_Block_Types = readushort (READER);
		// init 1d array
		obj->Block_Types =
		    GETMEM ((obj->Num_Block_Types) * sizeof (SizedString));
		// read 1d array
		for (i = 0; i < (obj->Num_Block_Types); i++)
			readSizedString (&obj->Block_Types[i], 0);
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Block_Type_Index =
		    GETMEM ((obj->Num_Blocks) * sizeof (unsigned short));
		READ (ushort, obj->Block_Type_Index, obj->Num_Blocks, SIZEOFWORD, "readHeader")
	}
	if (VersionCheck (0x14020007, 0)) {
		obj->Block_Size =
		    GETMEM ((obj->Num_Blocks) * sizeof (unsigned int));
		READ (uint, obj->Block_Size, obj->Num_Blocks, SIZEOFDWORD, "readHeader")
	}
	if (VersionCheck (0x14010003, 0)) {
		obj->Num_Strings = readuint (READER);
		PROTECT_LEN (obj->Num_Strings, 0xffff, "readHeader")
		obj->Max_String_Length = readuint (READER);
		PROTECT_LEN (obj->Max_String_Length, 0xffff, "readHeader")
		// init 1d array
		obj->Strings =
		    GETMEM ((obj->Num_Strings) * sizeof (SizedString));
		// read 1d array
		for (i = 0; i < (obj->Num_Strings); i++)
			readSizedString (&obj->Strings[i], 0);
	}
	if (VersionCheck (0x0A000100, 0))
		obj->Unknown_Int_2 = readuint (READER);
}

void
readStringPalette(StringPalette *obj, unsigned int ARG)
{
	COVERAGE
	readSizedString (&obj->Palette, 0);
	obj->Length = readuint (READER);
}

void
readTBC(TBC *obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->t), 3, SIZEOFDWORD, "readTBC")
}

inline void
readKey(Key *obj, unsigned int ARG, int t)
{
	COVERAGE
	obj->Time = readfloat (READER);
	readTEMPLATE (&obj->Value, 0, t);
	if (ARG == 2) {
		readTEMPLATE (&obj->Forward, 0, t);
		readTEMPLATE (&obj->Backward, 0, t);
	}
	else if (ARG == 3) {
		readTBC (&obj->TBC, 0);
	}
}

inline void
readKeyGroup(KeyGroup *obj, unsigned int ARG, int t)
{
	COVERAGE
	obj->Num_Keys = readuint (READER);
	PROTECT_LEN (obj->Num_Keys, 2000000, "readKeyGroup")
	if ((obj->Num_Keys != 0)) {
		obj->Interpolation = readuint (READER);
	}
	// init 1d array
	obj->Keys = GETMEM ((obj->Num_Keys) * sizeof (Key));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Keys); i++)
		readKey (&obj->Keys[i], obj->Interpolation, t);
}

inline void
readQuatKey(QuatKey *obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x0A010000)) {
		obj->Time = readfloat (READER);
	}
	if (ARG != 4) {
		if (VersionCheck(0x0A01006A, 0))
			obj->Time = readfloat (READER);
		readTEMPLATE (&obj->Value, 0, T_QUATERNION);
	}
	if (ARG == 3) {
		readTBC (&obj->TBC, 0);
	}
}

inline void
readTexCoord(TexCoord *obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->u), 2, SIZEOFDWORD, "readTexCoord")
}

void
readTexDesc(TexDesc *obj, unsigned int ARG)
{
	COVERAGE
	obj->Source = readRef (READER);
	if (VersionCheck (0, 0x14000005)) {
		obj->Clamp_Mode = readuint (READER);
		obj->Filter_Mode = readuint (READER);
	}
	if (VersionCheck (0x14010003, 0)) {
		obj->Flags = readFlags (READER);
	}
	if (VersionCheck (0x14060000, 0)) {
		obj->Unknown_short = readshort (READER);
	}
	if (VersionCheck (0, 0x14000005)) {
		obj->UV_Set = readuint (READER);
	}
	if (VersionCheck (0, 0x0A040001)) {
		obj->PS2_L = readshort (READER);
		obj->PS2_K = readshort (READER);
	}
	if (VersionCheck (0, 0x0401000C)) {
		obj->Unknown1 = readushort (READER);
	}
	if (VersionCheck (0x0A010000, 0)) {
		obj->Has_Texture_Transform = readbool (READER);
		if (obj->Has_Texture_Transform) {
			readTexCoord (&obj->Translation, 0);
			readTexCoord (&obj->Tiling, 0);
			obj->W_Rotation = readfloat (READER);
			obj->Transform_Type_ = readuint (READER);
			readTexCoord (&obj->Center_Offset, 0);
		}
	}
}

void
readShaderTexDesc(ShaderTexDesc *obj, unsigned int ARG)
{
	COVERAGE
	obj->Is_Used = readbool (READER);
	if (obj->Is_Used) {
		readTexDesc (&obj->Texture_Data, 0);
		obj->Map_Index = readuint (READER);
	}
}

void
readTexSource(TexSource * obj, unsigned int ARG)
{
	COVERAGE
	obj->Use_External = readbyte (READER);
	if ((obj->Use_External == 1)) {
		readFilePath (&obj->File_Name, 0);
	}
	if (VersionCheck (0x0A010000, 0) && (obj->Use_External == 1)) {
		obj->Unknown_Link = readRef (READER);
	}
	if (VersionCheck (0, 0x0A000100) && (obj->Use_External == 0)) {
		obj->Unknown_Byte = readbyte (READER);
	}
	if ((obj->Use_External == 0)) {
		obj->Pixel_Data = readRef (READER);
	}
}

inline void
readTriangle(Triangle * obj, unsigned int ARG)
{
	COVERAGE
	READ (ushort, &(obj->v1), 3, SIZEOFWORD, "readTriangle")
}

void
readSkinPartition(SkinPartition * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Vertices = readushort (READER);
	obj->Num_Triangles = readushort (READER);
	obj->Num_Bones = readushort (READER);
	obj->Num_Strips = readushort (READER);
	obj->Num_Weights_Per_Vertex = readushort (READER);
	obj->Bones = GETMEM ((obj->Num_Bones) * sizeof (unsigned short));
	READ (ushort, obj->Bones, obj->Num_Bones, SIZEOFWORD, "readSkinPartition")
	if (VersionCheck (0x0A010000, 0)) {
		obj->Has_Vertex_Map = readbyte (READER);
	}
	if (VersionCheck (0, 0x0A000102) ||
		(obj->Has_Vertex_Map != 0 && VersionCheck (0x0A010000, 0)) ) {
		obj->Vertex_Map =
		    GETMEM ((obj->Num_Vertices) * sizeof (unsigned short));
		READ (ushort, obj->Vertex_Map, obj->Num_Vertices, SIZEOFWORD, "readSkinPartition")
	}
	if (VersionCheck (0x0A010000, 0)) {
		obj->Has_Vertex_Weights = readbool (READER);
	}
	if (VersionCheck (0, 0x0A000102) || 
		(obj->Has_Vertex_Weights != 0 && VersionCheck (0x0A010000, 0)) ) {
		int cnt = (obj->Num_Vertices * obj->Num_Weights_Per_Vertex);
		PROTECT_LEN (cnt, 2000000, "readSkinPartition")
		obj->Vertex_Weights = GETMEM (cnt * sizeof (NIFfloat));
		READ (float, obj->Vertex_Weights, cnt, SIZEOFDWORD, "readSkinPartition")
	}
	obj->Strip_Lengths =
	    GETMEM ((obj->Num_Strips) * sizeof (unsigned short));
	READ (ushort, obj->Strip_Lengths, obj->Num_Strips, SIZEOFWORD, "readSkinPartition")
	if (VersionCheck (0x0A010000, 0)) {
		obj->Has_Faces = readbyte (READER);
	}
	if ((VersionCheck (0, 0x0A000102) && (obj->Num_Strips != 0)) ||
		((obj->Has_Faces != 0) && (obj->Num_Strips != 0) && VersionCheck (0x0A010000, 0))) {
		// for loop can not be avoided for now - jagged array
		obj->Strips =
		    GETMEM ((obj->Num_Strips) * sizeof (unsigned short *));
		int i;
		for (i = 0; i < (obj->Num_Strips); i++) {
			obj->Strips[i] =
			    GETMEM ((obj->Strip_Lengths[i]) * sizeof (unsigned short));
			READ (ushort, obj->Strips[i], obj->Strip_Lengths[i], SIZEOFWORD, "readSkinPartition")
		}
	}
	if ((VersionCheck (0, 0x0A000102) && (obj->Num_Strips == 0)) ||
		((obj->Has_Faces != 0) && (obj->Num_Strips == 0) && VersionCheck (0x0A010000, 0))) {
		obj->Triangles =
		    GETMEM ((obj->Num_Triangles) * sizeof (Triangle));
		if (obj->Num_Triangles > 0)
			READ(ushort, &(obj->Triangles[0].v1), obj->Num_Triangles*3, SIZEOFWORD, "readSkinPartition");
	}
	obj->Has_Bone_Indices = readbool (READER);
	if ((obj->Has_Bone_Indices != 0)) {
		int cnt = (obj->Num_Vertices * obj->Num_Weights_Per_Vertex);
		PROTECT_LEN (cnt, 2000000, "readSkinPartition")
		obj->Bone_Indices = GETMEM (cnt * sizeof (NIFbyte));
		READ (byte, obj->Bone_Indices, cnt, 1, "readSkinPartition")
	}
}

void
readBoundingBox(BoundingBox * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Int = readuint (READER);
	// TODO: REPLACE with 3*3 + 3 + 3 floats read
	// tradeof with future changes in Vector3 and/or Matrix33
	readVector3 (&obj->Translation, 0);
	readMatrix33 (&obj->Rotation, 0);
	readVector3 (&obj->Radius, 0);
}

void
readFurniturePosition(FurniturePosition * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Offset, 0);
	obj->Orientation = readushort (READER);
	obj->Position_Ref_1 = readbyte (READER);
	obj->Position_Ref_2 = readbyte (READER);
}

inline void
readhkTriangle(hkTriangle * obj, unsigned int ARG)
{
	COVERAGE
	readTriangle (&obj->Triangle, 0);
	obj->Welding_Info = readushort (READER);
	if (VersionCheck (0, 0x14000005)) {
		readVector3 (&obj->Normal, 0);
	}
}

void
readMorph(Morph * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x0A01006A, 0)) {
		readstring (&obj->Frame_Name, 0);
	}
	if (VersionCheck (0, 0x0A010000)) {
		obj->Num_Keys = readuint (READER);
		PROTECT_LEN (obj->Num_Keys, 2000000, "readMorph")
		obj->Interpolation = readuint (READER);
		// init 1d array
		obj->Keys = GETMEM ((obj->Num_Keys) * sizeof (Key));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_Keys); i++)
			readKey (&obj->Keys[i], obj->Interpolation, T_FLOAT);
	}
	if (VersionCheck (0x0A01006A, 0x0A020000)) {
		obj->Unknown_Int = readuint (READER);
	}
	PROTECT_LEN (ARG, 2000000, "readMorph")
	obj->Vectors = GETMEM ((ARG) * sizeof (Vector3));
	if (ARG > 0)
		READ(float, &(obj->Vectors[0].x), ARG*3, SIZEOFDWORD, "readMorph");
}

void
readParticle(Particle * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Velocity, 0);
	readVector3 (&obj->Unknown_Vector, 0);
	READ (float, &(obj->Lifetime), 3, SIZEOFDWORD, "readParticle")
	/*obj->Lifetime = readfloat (READER);
	obj->Lifespan = readfloat (READER);
	obj->Timestamp = readfloat (READER);*/
	obj->Unknown_Short = readushort (READER);
	obj->Vertex_ID = readushort (READER);
}

void
readSkinTransform(SkinTransform * obj, unsigned int ARG)
{
	COVERAGE
	readMatrix33 (&obj->Rotation, 0);
	readVector3 (&obj->Translation, 0);
	obj->Scale = readfloat (READER);
}

void
readSkinData(SkinData * obj, unsigned int ARG)
{
	COVERAGE
	readSkinTransform (&obj->Skin_Transform, 0);
	readVector3 (&obj->Bounding_Sphere_Offset, 0);
	obj->Bounding_Sphere_Radius = readfloat (READER);
	if (VersionCheck (0x14030009, 0x14030009) && (User_Version == 131072)) {
		READ (short, &(obj->Unknown_13_Shorts[0]), 13, SIZEOFWORD, "readSkinData")
	}
	obj->Num_Vertices = readushort (READER);
	if (VersionCheck (0, 0x04020100) || 
		(VersionCheck (0x04020200, 0) && ARG != 0) ) {
		// init 1d array
		obj->Vertex_Weights =
		    GETMEM ((obj->Num_Vertices) * sizeof (SkinWeight));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_Vertices); i++)
			readSkinWeight (&obj->Vertex_Weights[i], 0);
	}
}

void
readSphereBV(SphereBV * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Center, 0);
	obj->Radius = readfloat (READER);
}

void
readOblivionColFilter(OblivionColFilter * obj, unsigned int ARG)
{
	COVERAGE
	obj->Layer = readbyte (READER);
	obj->Col_Filter = readbyte (READER);
	obj->Unknown_Short = readushort (READER);
}

void
readOblivionSubShape(OblivionSubShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->Layer = readbyte (READER);
	obj->Col_Filter = readbyte (READER);
	obj->Unknown_Short = readushort (READER);
	obj->Num_Vertices = readuint (READER);
	obj->Material = readuint (READER);
}

void
readMotorDescriptor(MotorDescriptor * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->Unknown_Float_1), 6, SIZEOFDWORD, "readMotorDescriptor")
	obj->Unknown_Byte_1 = readbyte (READER);
}

void
readRagdollDescriptor(RagdollDescriptor * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x14000005)) {
		readVector4 (&obj->Pivot_A, 0);
		readVector4 (&obj->Plane_A, 0);
		readVector4 (&obj->Twist_A, 0);
		readVector4 (&obj->Pivot_B, 0);
		readVector4 (&obj->Plane_B, 0);
		readVector4 (&obj->Twist_B, 0);
	}
	if (VersionCheck (0x14020007, 0)) {
		readVector4 (&obj->Motor_A, 0);
		readVector4 (&obj->Motor_B, 0);
	}
	READ (float, &(obj->Cone_Max_Angle), 6, SIZEOFDWORD, "readRagdollDescriptor")
	/*obj->Cone_Max_Angle = readfloat (READER);
	obj->Plane_Min_Angle = readfloat (READER);
	obj->Plane_Max_Angle = readfloat (READER);
	obj->Twist_Min_Angle = readfloat (READER);
	obj->Twist_Max_Angle = readfloat (READER);
	obj->Max_Friction = readfloat (READER);*/
	if (VersionCheck (0x14020007, 0)) {
		obj->Enable_Motor = readbyte (READER);
		if (obj->Enable_Motor)
			readMotorDescriptor (&obj->Motor, 0);
	}
}

void
readLimitedHingeDescriptor(LimitedHingeDescriptor * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x14000005)) {
		readVector4 (&obj->Pivot_A, 0);
		readVector4 (&obj->Axle_A, 0);
		readVector4 (&obj->Perp2_Axle_In_A1, 0);
		readVector4 (&obj->Perp2_Axle_In_A2, 0);
		readVector4 (&obj->Pivot_B, 0);
		readVector4 (&obj->Axle_B, 0);
		readVector4 (&obj->Perp2_Axle_In_B2, 0);
	}
	else if (VersionCheck (0x14020007, 0)) {
		readVector4 (&obj->Axle_A, 0);
		readVector4 (&obj->Perp2_Axle_In_A1, 0);
		readVector4 (&obj->Perp2_Axle_In_A2, 0);
		readVector4 (&obj->Pivot_A, 0);
		readVector4 (&obj->Axle_B, 0);
		readVector4 (&obj->Perp2_Axle_In_B1, 0);
		readVector4 (&obj->Perp2_Axle_In_B2, 0);
		readVector4 (&obj->Pivot_B, 0);
	}
	READ (float, &(obj->Min_Angle), 3, SIZEOFDWORD, "readLimitedHingeDescriptor")
	/*obj->Min_Angle = readfloat (READER);
	obj->Max_Angle = readfloat (READER);
	obj->Max_Friction = readfloat (READER);*/
	if (VersionCheck (0x14020007, 0)) {
		obj->Enable_Motor = readbyte (READER);
		if (obj->Enable_Motor)
			readMotorDescriptor (&obj->Motor, 0);
	}
}

void
readHingeDescriptor(HingeDescriptor * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0, 0x14000005)) {
		readVector4 (&obj->Pivot_A, 0);
		readVector4 (&obj->Perp2_Axle_In_A1, 0);
		readVector4 (&obj->Perp2_Axle_In_A2, 0);
		readVector4 (&obj->Pivot_B, 0);
		readVector4 (&obj->Axle_B, 0);
	}
	else if (VersionCheck (0x14020007, 0)) {
		readVector4 (&obj->Axle_A, 0);
		readVector4 (&obj->Perp2_Axle_In_B1, 0);
		readVector4 (&obj->Perp2_Axle_In_B2, 0);
	}
}

void
readOldSkinData(OldSkinData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Vertex_Weight = readfloat (READER);
	obj->Vertex_Index = readushort (READER);
	readVector3 (&obj->Unknown_Vector, 0);
}

void
readMultiTextureElement(MultiTextureElement * obj, unsigned int ARG)
{
	COVERAGE
	obj->Has_Image = readbool (READER);
	if ((obj->Has_Image)) {
		obj->Image = readRef (READER);
		obj->Clamp_ = readuint (READER);
		obj->Filter_ = readuint (READER);
		obj->UV_Set_ = readuint (READER);
		if (VersionCheck (0x03030000, 0x0A020000)) {
			obj->PS2_L = readshort (READER);
			obj->PS2_K = readshort (READER);
		}
		if (VersionCheck (0x03030000, 0))
			obj->Unknown_Short_3 = readshort (READER);
	}
}

void
readBoxBV(BoxBV * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Center, 0);
	READ (float, &(obj->Axis[0].x), 3*3, SIZEOFDWORD, "readBoxBV");
	READ (float, &(obj->Extent[0]), 3, SIZEOFDWORD, "readBoxBV")
}

void
readCapsuleBV(CapsuleBV * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Center, 0);
	readVector3 (&obj->Origin, 0);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
}

void
readHalfSpaceBV(HalfSpaceBV * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Normal, 0);
	readVector3 (&obj->Center, 0);
}

void
readBoundingVolume(BoundingVolume * obj, unsigned int ARG)
{
	COVERAGE
	obj->Collision_Type = readuint (READER);
	if ((obj->Collision_Type == 0)) {
		readSphereBV (&obj->Sphere, 0);
	}
	else if ((obj->Collision_Type == 1)) {
		readBoxBV (&obj->Box, 0);
	}
	else if ((obj->Collision_Type == 2)) {
		readCapsuleBV (&obj->Capsule, 0);
	}
	else if ((obj->Collision_Type == 4)) {
		readUnionBV ((UnionBV *)obj->Union, 0);
	}
	else if ((obj->Collision_Type == 5)) {
		readHalfSpaceBV (&obj->HalfSpace, 0);
	} else NPFF (ENIF, "unknown BoundingVolume: %d\n", obj->Collision_Type);
}

void
readUnionBV(UnionBV * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_BV = readuint (READER);
	// init 1d array
	obj->Bounding_Volumes =
	    GETMEM ((obj->Num_BV) * sizeof (BoundingVolume));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_BV); i++)
		readBoundingVolume (&obj->Bounding_Volumes[i], 0);
}

void
readMorphWeight(MorphWeight * obj, unsigned int ARG)
{
	COVERAGE
	obj->Interpolator = readRef (READER);
	obj->Weight_ = readfloat (READER);
}

void
readArkTexture(ArkTexture * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Texture_Name, 0);
	obj->Unknown_Int_3 = readint (READER);
	obj->Unknown_Int_4 = readint (READER);
	obj->Texturing_Property = readRef (READER);
	READ (byte, &(obj->Unknown_Bytes[0]), 9, 1, "readArkTexture")
}

void
readInertiaMatrix(InertiaMatrix * obj, unsigned int ARG)
{
	COVERAGE
	READ (float, &(obj->m11), 12, SIZEOFDWORD, "readArkTexture")
}

void
readDecalVectorArray(DecalVectorArray * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Vectors = readshort (READER);
	obj->Points = GETMEM ((obj->Num_Vectors) * sizeof (Vector3));
	if (obj->Num_Vectors > 0)
		READ (float, &(obj->Points[0].x), obj->Num_Vectors*3, SIZEOFDWORD, "readDecalVectorArray");
	obj->Normals = GETMEM ((obj->Num_Vectors) * sizeof (Vector3));
	if (obj->Num_Vectors > 0)
		READ (float, &(obj->Normals[0].x), obj->Num_Vectors*3, SIZEOFDWORD, "readDecalVectorArray");
}

void readBodyPartList (BodyPartList * obj, unsigned int ARG)
{
	COVERAGE
	obj->Part_Flag = readushort (READER);
	obj->Body_Part = readushort (READER);
}

void
readNi3dsAlphaAnimator(Ni3dsAlphaAnimator * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1[0]), 40, 1, "readNi3dsAlphaAnimator")
	obj->Parent = readRef (READER);
	obj->Num_1 = readuint (READER);
	obj->Num_2 = readuint (READER);
	NIFuint len = obj->Num_1 * obj->Num_2;
	PROTECT_LEN (len, 2000000, "readNi3dsAlphaAnimator")
	obj->Unknown_2 = GETMEM (len * sizeof (NIFuint));
	READ (uint, obj->Unknown_2, len, SIZEOFDWORD, "readNi3dsAlphaAnimator")
}

void
readNi3dsAnimationNode(Ni3dsAnimationNode * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	obj->Has_Data = readuint (READER);
	if ((obj->Has_Data)) {
		READ (float, &(obj->Unknown_Floats_1[0]), 21, SIZEOFDWORD, "readNi3dsAnimationNode")
		obj->Unknown_Short = readushort (READER);
		obj->Child = readRef (READER);
		READ (float, &(obj->Unknown_Floats_2[0]), 12, SIZEOFDWORD, "readNi3dsAnimationNode")
		obj->Count = readuint (READER);
		NIFuint len = obj->Count * 5;
		PROTECT_LEN (len, 2000000, "readNi3dsAnimationNode")
		obj->Unknown_Array = GETMEM (len);
		READ (byte, obj->Unknown_Array, len, 1, "readNi3dsAnimationNode")
	}
}

void
readNi3dsColorAnimator(Ni3dsColorAnimator * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1[0]), 184, 1, "readNi3dsColorAnimator")
}

void
readNi3dsMorphShape(Ni3dsMorphShape * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1[0]), 14, 1, "readNi3dsMorphShape")
}

void
readNi3dsParticleSystem(Ni3dsParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1[0]), 14, 1, "readNi3dsParticleSystem")
}

void
readNi3dsPathController(Ni3dsPathController * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1[0]), 20, 1, "readNi3dsPathController")
}

void
readNiParticleModifier(NiParticleModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->Next_Modifier = readRef (READER);
	if (VersionCheck (0x04000002, 0)) {
		obj->Controller = readPtr (READER);
	}
}

void
readNiPSysCollider(NiPSysCollider * obj, unsigned int ARG)
{
	COVERAGE
	obj->Bounce = readfloat (READER);
	obj->Spawn_on_Collide = readbool (READER);
	obj->Die_on_Collide = readbool (READER);
	obj->Spawn_Modifier = readRef (READER);
	obj->Parent = readPtr (READER);
	obj->Next_Collider = readRef (READER);
	obj->Collider_Object = readPtr (READER);
}

inline void
readbhkRefObject(bhkRefObject * obj, unsigned int ARG)
{
	COVERAGE
}

void
readbhkSerializable(bhkSerializable * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkRefObject));
	readbhkRefObject (obj->parent, 0);
}

void
readbhkWorldObject(bhkWorldObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSerializable));
	readbhkSerializable (obj->parent, 0);
	obj->Shape = readRef (READER);
	obj->Layer = readbyte (READER);
	obj->Col_Filter = readbyte (READER);
	obj->Unknown_Short = readushort (READER);
}

void
readbhkPhantom(bhkPhantom * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkWorldObject));
	readbhkWorldObject (obj->parent, 0);
}

void
readbhkShapePhantom(bhkShapePhantom * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkPhantom));
	readbhkPhantom (obj->parent, 0);
}

void
readbhkSimpleShapePhantom(bhkSimpleShapePhantom * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShapePhantom));
	readbhkShapePhantom (obj->parent, 0);
	READ (float, &(obj->Unkown_Floats[0]), 7, SIZEOFDWORD, "readbhkSimpleShapePhantom")
	READ (float, &(obj->Unknown_Floats_2[0]), 3*5, SIZEOFDWORD, "readbhkSimpleShapePhantom")
	obj->Unknown_Float = readfloat (READER);
}

void
readbhkEntity(bhkEntity * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkWorldObject));
	readbhkWorldObject (obj->parent, 0);
}

void
readbhkRigidBody(bhkRigidBody * obj, unsigned int ARG)
{
	COVERAGE
	//int i;
	obj->parent = GETMEM (sizeof (bhkEntity));
	readbhkEntity (obj->parent, 0);
	obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	READ (int, &(obj->Unknown_3_Ints[0]), 3, SIZEOFDWORD, "readbhkRigidBody")
	obj->Collision_Response_ = readbyte (READER);
	obj->Unknown_Byte = readbyte (READER);
	obj->Process_Contact_Callback_Delay_ = readushort (READER);
	READ (ushort, &(obj->Unknown_2_Shorts[0]), 2, SIZEOFWORD, "readbhkRigidBody")
	obj->Layer_Copy = readbyte (READER);
	obj->Col_Filter_Copy = readbyte (READER);
	READ (ushort, &(obj->Unknown_7_Shorts[0]), 7, SIZEOFWORD, "readbhkRigidBody")
	readVector4 (&obj->Translation, 0);
	readQuaternionXYZW (&obj->Rotation, 0);
	readVector4 (&obj->Linear_Velocity, 0);
	readVector4 (&obj->Angular_Velocity, 0);
	readInertiaMatrix (&obj->Inertia, 0);
	readVector4 (&obj->Center, 0);
	READ (float, &(obj->Mass), 8, SIZEOFDWORD, "readbhkRigidBody")
	/*obj->Mass = readfloat (READER);
	obj->Linear_Damping = readfloat (READER);
	obj->Angular_Damping = readfloat (READER);
	obj->Friction = readfloat (READER);
	obj->Restitution = readfloat (READER);
	obj->Max_Linear_Velocity = readfloat (READER);
	obj->Max_Angular_Velocity = readfloat (READER);
	obj->Penetration_Depth = readfloat (READER);*/
	READ (byte, &(obj->Motion_System), 4, 1, "readbhkRigidBody")
	/*obj->Motion_System = readbyte (READER);
	obj->Deactivator_Type = readbyte (READER);
	obj->Solver_Deactivation = readbyte (READER);
	obj->Quality_Type = readbyte (READER);*/
	READ (uint, &(obj->Auto_Remove_Level), 4, SIZEOFDWORD, "readbhkRigidBody")
	/*obj->Auto_Remove_Level = readuint (READER);
	obj->User_Datas_In_Contact_Point_Properties_ = readuint (READER);
	obj->Force_Collide_Onto_Ppu_ = readuint (READER);
	obj->Num_Constraints = readuint (READER);*/
	PROTECT_LEN (obj->Num_Constraints, 2000000, "readbhkRigidBody")
	obj->Constraints =
	    GETMEM ((obj->Num_Constraints) * sizeof (NIFuint));
	READ (uint, obj->Constraints, obj->Num_Constraints, SIZEOFDWORD, "readbhkRigidBody")
	obj->Unknown_Int_9 = readuint (READER);
}

void
readbhkRigidBodyT(bhkRigidBodyT * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkRigidBody));
	readbhkRigidBody (obj->parent, 0);
}

void
readbhkConstraint(bhkConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSerializable));
	readbhkSerializable (obj->parent, 0);
	obj->Num_Entities = readuint (READER);
	PROTECT_LEN (obj->Num_Entities, 2000000, "readbhkConstraint")
	obj->Entities = GETMEM ((obj->Num_Entities) * sizeof (unsigned int));
	READ (uint, obj->Entities, obj->Num_Entities, SIZEOFDWORD, "readbhkConstraint")
	obj->Priority = readuint (READER);
}

void
readbhkLimitedHingeConstraint(bhkLimitedHingeConstraint * obj,
			       unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	readLimitedHingeDescriptor (&obj->Limited_Hinge, 0);
}

void
readbhkMalleableConstraint(bhkMalleableConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	obj->Type = readuint (READER);
	obj->Unknown_Int_2 = readuint (READER);
	obj->Unknown_Link_1 = readRef (READER);
	obj->Unknown_Link_2 = readRef (READER);
	obj->Unknown_Int_3 = readuint (READER);
	if ((obj->Type == 1)) {
		readHingeDescriptor (&obj->Hinge, 0);
	}
	else if ((obj->Type == 7)) {
		readRagdollDescriptor (&obj->Ragdoll, 0);
	}
	else if ((obj->Type == 2)) {
		readLimitedHingeDescriptor (&obj->Limited_Hinge, 0);
	}
	if (VersionCheck (0, 0x14000005)) {
		obj->Tau = readfloat (READER);
	}
	obj->Damping = readfloat (READER);
}

void
readbhkStiffSpringConstraint(bhkStiffSpringConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	readVector4 (&obj->Pivot_A, 0);
	readVector4 (&obj->Pivot_B, 0);
	obj->Length = readfloat (READER);
}

void
readbhkRagdollConstraint(bhkRagdollConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	readRagdollDescriptor (&obj->Ragdoll, 0);
}

void
readbhkPrismaticConstraint(bhkPrismaticConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	if (VersionCheck (0, 0x14000005)) {
		readVector4 (&obj->Pivot_A, 0);
		READ (float, &(obj->Rotation_Matrix_A[0].x), 4*4, SIZEOFDWORD, "readbhkPrismaticConstraint")
		readVector4 (&obj->Pivot_B, 0);
		readVector4 (&obj->Sliding_B, 0);
		readVector4 (&obj->Plane_B, 0);
	}
	if (VersionCheck (0x14020007, 0)) {
		readVector4 (&obj->Sliding_A, 0);
		readVector4 (&obj->Rotation_A, 0);
		readVector4 (&obj->Plane_A, 0);
		readVector4 (&obj->Pivot_A, 0);
		readVector4 (&obj->Sliding_B, 0);
		readVector4 (&obj->Rotation_B, 0);
		readVector4 (&obj->Plane_B, 0);
		readVector4 (&obj->Pivot_B, 0);
	}
	obj->Min_Distance = readfloat (READER);
	obj->Max_Distance = readfloat (READER);
	obj->Friction = readfloat (READER);
	if (VersionCheck (0x14020007, 0)) {
		obj->Unknown_Byte_1 = readbyte (READER);
	}
}

void
readbhkHingeConstraint(bhkHingeConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	readHingeDescriptor (&obj->Hinge, 0);
}

void
readbhkShape(bhkShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSerializable));
	readbhkSerializable (obj->parent, 0);
}

void
readbhkTransformShape(bhkTransformShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShape));
	readbhkShape (obj->parent, 0);
	obj->Shape = readRef (READER);
	obj->Material = readuint (READER);
	obj->Unknown_Float_1 = readfloat (READER);
	READ (byte, &(obj->Unknown_8_Bytes[0]), 8, 1, "readbhkTransformShape")
	readMatrix44 (&obj->Transform, 0);
}

void
readbhkSphereRepShape(bhkSphereRepShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShape));
	readbhkShape (obj->parent, 0);
	obj->Material = readuint (READER);
	obj->Radius = readfloat (READER);
}

void
readbhkConvexShape(bhkConvexShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSphereRepShape));
	readbhkSphereRepShape (obj->parent, 0);
}

void
readbhkSphereShape(bhkSphereShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConvexShape));
	readbhkConvexShape (obj->parent, 0);
}

void
readbhkCapsuleShape(bhkCapsuleShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConvexShape));
	readbhkConvexShape (obj->parent, 0);
	READ (byte, &(obj->Unknown_8_Bytes[0]), 8, 1, "readbhkCapsuleShape")
	readVector3 (&obj->First_Point, 0);
	obj->Radius_1 = readfloat (READER);
	readVector3 (&obj->Second_Point, 0);
	obj->Radius_2 = readfloat (READER);
}

void
readbhkBoxShape(bhkBoxShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConvexShape));
	readbhkConvexShape (obj->parent, 0);
	READ (byte, &(obj->Unknown_8_Bytes[0]), 8, 1, "readbhkBoxShape")
	readVector3 (&obj->Dimensions, 0);
	obj->Minimum_Size = readfloat (READER);
}

void
readbhkConvexVerticesShape(bhkConvexVerticesShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConvexShape));
	readbhkConvexShape (obj->parent, 0);
	READ (float, &(obj->Unknown_6_Floats[0]), 6, SIZEOFDWORD, "readbhkConvexVerticesShape")
	obj->Num_Vertices = readuint (READER);
	PROTECT_LEN (obj->Num_Vertices, 2000000, "readbhkConvexVerticesShape")
	obj->Vertices = GETMEM ((obj->Num_Vertices) * sizeof (Vector4));
	if (obj->Num_Vertices > 0)
		READ (float, &(obj->Vertices[0].x), obj->Num_Vertices*4, SIZEOFDWORD, "readbhkConvexVerticesShape")
	obj->Num_Normals = readuint (READER);
	PROTECT_LEN (obj->Num_Normals, 2000000, "readbhkConvexVerticesShape")
	obj->Normals = GETMEM ((obj->Num_Normals) * sizeof (Vector4));
	if (obj->Num_Normals > 0)
		READ (float, &(obj->Normals[0].x), obj->Num_Normals*4, SIZEOFDWORD, "readbhkConvexVerticesShape")
}

void
readbhkConvexTransformShape(bhkConvexTransformShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkTransformShape));
	readbhkTransformShape (obj->parent, 0);
}

void
readbhkMultiSphereShape(bhkMultiSphereShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSphereRepShape));
	readbhkSphereRepShape (obj->parent, 0);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Num_Spheres = readuint (READER);
	PROTECT_LEN (obj->Num_Spheres, 2000000, "readbhkMultiSphereShape")
	// init 1d array
	obj->Spheres = GETMEM ((obj->Num_Spheres) * sizeof (SphereBV));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Spheres); i++)
		readSphereBV (&obj->Spheres[i], 0);
}

void
readbhkBvTreeShape(bhkBvTreeShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShape));
	readbhkShape (obj->parent, 0);
}

void
readbhkMoppBvTreeShape(bhkMoppBvTreeShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkBvTreeShape));
	readbhkBvTreeShape (obj->parent, 0);
	obj->Shape = readRef (READER);
	obj->Material = readuint (READER);
	obj->Unknown_Int_1 = readuint (READER);
	obj->Unknown_Int_2 = readuint (READER);
	obj->Unknown_Float = readfloat (READER);
	obj->MOPP_Data_Size = readuint (READER);
	PROTECT_LEN (obj->MOPP_Data_Size, 2000000, "readbhkMoppBvTreeShape")
	readVector3 (&obj->Origin, 0);
	obj->Scale = readfloat (READER);
	if (VersionCheck (0, 0x0A000100)) {
		obj->Old_MOPP_Data =
		    GETMEM ((obj->MOPP_Data_Size - 1));
		READ (byte, obj->Old_MOPP_Data, obj->MOPP_Data_Size - 1, 1, "readbhkMoppBvTreeShape")
	}
	if (VersionCheck (0x0A000102, 0)) {
		obj->MOPP_Data = GETMEM ((obj->MOPP_Data_Size));
		READ (byte, obj->MOPP_Data, obj->MOPP_Data_Size, 1, "readbhkMoppBvTreeShape")
	}
}

void
readbhkShapeCollection(bhkShapeCollection * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShape));
	readbhkShape (obj->parent, 0);
}

void
readbhkListShape(bhkListShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShapeCollection));
	readbhkShapeCollection (obj->parent, 0);
	obj->Num_Sub_Shapes = readuint (READER);
	PROTECT_LEN (obj->Num_Sub_Shapes, 2000000, "readbhkListShape")
	obj->Sub_Shapes =
	    GETMEM ((obj->Num_Sub_Shapes) * sizeof (unsigned int));
	READ (uint, obj->Sub_Shapes, obj->Num_Sub_Shapes, SIZEOFDWORD, "readbhkListShape")
	obj->Material = readuint (READER);
	READ (float, &(obj->Unknown_Floats[0]), 6, SIZEOFDWORD, "readbhkListShape")
	obj->Num_Unknown_Ints = readuint (READER);
	PROTECT_LEN (obj->Num_Unknown_Ints, 2000000, "readbhkListShape")
	obj->Unknown_Ints =
	    GETMEM ((obj->Num_Unknown_Ints) * sizeof (unsigned int));
	READ (uint, obj->Unknown_Ints, obj->Num_Unknown_Ints, SIZEOFDWORD, "readbhkListShape")
}

void
readbhkMeshShape(bhkMeshShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShape));
	readbhkShape (obj->parent, 0);
	READ (float, &(obj->Unknown_1[0]), 9, SIZEOFDWORD, "readbhkMeshShape")
	obj->Num_Unknown_Floats = readint (READER);
	NIFint len = obj->Num_Unknown_Floats * 3;
	PROTECT_LEN (len, 2000000, "readbhkMeshShape")
	obj->Unknown_Floats = GETMEM (len * sizeof (NIFfloat));
	READ (float, obj->Unknown_Floats, len, SIZEOFDWORD, "readbhkMeshShape")
	READ (int, &(obj->Unknown_2[0]), 3, SIZEOFDWORD, "readbhkMeshShape")
	if (VersionCheck (0, 0x0A000100)) {
		obj->Num_Strips_Data = readuint (READER);
		PROTECT_LEN (obj->Num_Strips_Data, 2000000, "readbhkMeshShape")
		obj->Strips_Data =
		    GETMEM ((obj->Num_Strips_Data) * sizeof (unsigned int));
		READ (uint, obj->Strips_Data, obj->Num_Strips_Data, SIZEOFDWORD, "readbhkMeshShape")
	}
}

void
readbhkPackedNiTriStripsShape(bhkPackedNiTriStripsShape * obj, unsigned int ARG)
{
	COVERAGE
	int i;
	obj->parent = GETMEM (sizeof (bhkShapeCollection));
	readbhkShapeCollection (obj->parent, 0);
	if (VersionCheck (0, 0x14000005)) {
		obj->Num_Sub_Shapes = readushort (READER);
		// init 1d array
		obj->Sub_Shapes =
		    GETMEM ((obj->Num_Sub_Shapes) * sizeof (OblivionSubShape));
		// read 1d array
		for (i = 0; i < (obj->Num_Sub_Shapes); i++)
			readOblivionSubShape (&obj->Sub_Shapes[i], 0);
	}
	obj->Unknown_Int_1 = readuint (READER);
	obj->Unknown_Int_2 = readuint (READER);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Int_3 = readuint (READER);
	readVector3 (&obj->Scale_Copy_, 0);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);
	readVector3 (&obj->Scale, 0);
	obj->Unknown_Float_4 = readfloat (READER);
	obj->Data = readRef (READER);
}

void
readbhkNiTriStripsShape(bhkNiTriStripsShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShapeCollection));
	readbhkShapeCollection (obj->parent, 0);
	obj->Material = readuint (READER);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Int_1 = readuint (READER);
	READ (uint, &(obj->Unknown_Ints_1[0]), 4, SIZEOFDWORD, "readbhkNiTriStripsShape")
	obj->Unknown_Int_2 = readuint (READER);
	readVector3 (&obj->Scale, 0);
	obj->Unknown_Int_3 = readuint (READER);
	obj->Num_Strips_Data = readuint (READER);
	PROTECT_LEN (obj->Num_Strips_Data, 2000000, "readbhkNiTriStripsShape")
	obj->Strips_Data =
	    GETMEM ((obj->Num_Strips_Data) * sizeof (unsigned int));
	READ (uint, obj->Strips_Data, obj->Num_Strips_Data, SIZEOFDWORD, "readbhkNiTriStripsShape")
	obj->Num_Data_Layers = readuint (READER);
	PROTECT_LEN (obj->Num_Data_Layers, 2000000, "readbhkNiTriStripsShape")
	// init 1d array
	obj->Data_Layers =
	    GETMEM ((obj->Num_Data_Layers) * sizeof (OblivionColFilter));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Data_Layers); i++)
		readOblivionColFilter (&obj->Data_Layers[i], 0);
}

void
readNiExtraData(NiExtraData * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x0A000100, 0)) {
		readstring (&obj->Name, 0);
	}
	if (VersionCheck (0, 0x04020200)) {
		obj->Next_Extra_Data = readRef (READER);
	}
}

inline void
readNiInterpolator(NiInterpolator * obj, unsigned int ARG)
{
	COVERAGE
}

void
readNiKeyBasedInterpolator(NiKeyBasedInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpolator));
	readNiInterpolator (obj->parent, 0);
}

void
readNiFloatInterpolator(NiFloatInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyBasedInterpolator));
	readNiKeyBasedInterpolator (obj->parent, 0);
	obj->Float_Value = readfloat (READER);
	obj->Data = readRef (READER);
}

void
readNiTransformInterpolator(NiTransformInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyBasedInterpolator));
	readNiKeyBasedInterpolator (obj->parent, 0);
	readVector3 (&obj->Translation, 0);
	readQuaternion (&obj->Rotation, 0);
	obj->Scale = readfloat (READER);
	if (VersionCheck (0x0A01006A, 0x0A01006A)) {
		READ (byte, &(obj->Unknown_Bytes[0]), 3, 1, "readNiTransformInterpolator")
	}
	obj->Data = readRef (READER);
}

void
readNiPoint3Interpolator(NiPoint3Interpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyBasedInterpolator));
	readNiKeyBasedInterpolator (obj->parent, 0);
	readVector3 (&obj->Point_3_Value, 0);
	obj->Data = readRef (READER);
}

void
readNiPathInterpolator(NiPathInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyBasedInterpolator));
	readNiKeyBasedInterpolator (obj->parent, 0);
	obj->Unknown_Short = readushort (READER);
	obj->Unknown_Int = readuint (READER);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Short_2 = readushort (READER);
	obj->Pos_Data = readRef (READER);
	obj->Float_Data = readRef (READER);
}

void
readNiBoolInterpolator(NiBoolInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyBasedInterpolator));
	readNiKeyBasedInterpolator (obj->parent, 0);
	obj->Bool_Value = readbool (READER);
	obj->Data = readRef (READER);
}

void
readNiBoolTimelineInterpolator(NiBoolTimelineInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBoolInterpolator));
	readNiBoolInterpolator (obj->parent, 0);
}

void
readNiBlendInterpolator(NiBlendInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpolator));
	readNiInterpolator (obj->parent, 0);
	obj->Unknown_Short = readushort (READER);
	obj->Unknown_Int = readuint (READER);
}

void
readNiBSplineInterpolator(NiBSplineInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpolator));
	readNiInterpolator (obj->parent, 0);
	obj->Start_Time = readfloat (READER);
	obj->Stop_Time = readfloat (READER);
	obj->Spline_Data = readRef (READER);
	obj->Basis_Data = readRef (READER);
}

inline void
readNiObjectNET(NiObjectNET * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	if (VersionCheck (0, 0x02030000)) {
		obj->Has_Old_Extra_Data = readuint (READER);
		if (obj->Has_Old_Extra_Data) {
			readstring (&obj->Old_Extra_Prop_Name, 0);
			obj->Old_Extra_Internal_Id = readuint (READER);
			readstring (&obj->Old_Extra_String, 0);
		}
		obj->Unknown_Byte = readbyte (READER);
	}
	if (VersionCheck (0x03000000, 0x04020200)) {
		obj->Extra_Data = readRef (READER);
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Num_Extra_Data_List = readuint (READER);
		PROTECT_LEN (obj->Num_Extra_Data_List, 2000000, "readNiObjectNET")
		if (obj->Num_Extra_Data_List > 0) {
			obj->Extra_Data_List =
		    	GETMEM ((obj->Num_Extra_Data_List) * sizeof (unsigned int));
			READ (uint, obj->Extra_Data_List, obj->Num_Extra_Data_List, SIZEOFDWORD, "readNiObjectNET")
		}
	}
	if (VersionCheck (0x03000000, 0)) {
		obj->Controller = readRef (READER);
	}
}

void
readNiCollisionObject(NiCollisionObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->Target = readPtr (READER);
}

void
readNiCollisionData(NiCollisionData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiCollisionObject));
	readNiCollisionObject (obj->parent, 0);
	obj->Propagation_Mode = readuint (READER);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Collision_Mode = readuint (READER);
	}
	obj->Use_ABV = readbyte (READER);
	if ((obj->Use_ABV == 1)) {
		readBoundingVolume (&obj->Bounding_Volume, 0);
	}
}

void
readbhkNiCollisionObject(bhkNiCollisionObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiCollisionObject));
	readNiCollisionObject (obj->parent, 0);
	obj->Flags = readFlags (READER);
	obj->Body = readRef (READER);
}

void
readbhkCollisionObject(bhkCollisionObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkNiCollisionObject));
	readbhkNiCollisionObject (obj->parent, 0);
}

void
readbhkBlendCollisionObject(bhkBlendCollisionObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkCollisionObject));
	readbhkCollisionObject (obj->parent, 0);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
}

void
readbhkPCollisionObject(bhkPCollisionObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkNiCollisionObject));
	readbhkNiCollisionObject (obj->parent, 0);
}

void
readbhkSPCollisionObject(bhkSPCollisionObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkPCollisionObject));
	readbhkPCollisionObject (obj->parent, 0);
}

inline void
readNiAVObject(NiAVObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiObjectNET));
	readNiObjectNET (obj->parent, 0);
	if (VersionCheck (0x03000000, 0)) {
		obj->Flags = readFlags (READER);
	}
	if (VersionCheck (0x14020007, 0)
	    && ((User_Version == 11) && (User_Version_2 > 26))) {
		obj->Unknown_Short_1 = readushort (READER);
	}
	readVector3 (&obj->Translation, 0);
	readMatrix33 (&obj->Rotation, 0);
	obj->Scale = readfloat (READER);
	if (VersionCheck (0, 0x04020200)) {
		readVector3 (&obj->Velocity, 0);
	}
	obj->Num_Properties = readuint (READER);
	PROTECT_LEN (obj->Num_Properties, 2000000, "readNiAVObject")
	obj->Properties =
	    GETMEM ((obj->Num_Properties) * sizeof (unsigned int));
	READ (uint, obj->Properties, obj->Num_Properties, SIZEOFDWORD, "readNiAVObject")
	if (VersionCheck (0, 0x02030000)) {
		READ (uint, &(obj->Unknown_1[0]), 4, SIZEOFDWORD, "readNiAVObject")
	}
	if (VersionCheck (0, 0x02030000)) {
		obj->Unknown_2 = readbyte (READER);
	}
	if (VersionCheck (0x03000000, 0x04020200)) {
		obj->Has_Bounding_Box = readbool (READER);
		if (obj->Has_Bounding_Box != 0)
			readBoundingBox (&obj->Bounding_Box, 0);
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Collision_Object = readRef (READER);
	}
}

void
readNiDynamicEffect(NiDynamicEffect * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Switch_State = readbool (READER);
	}
	if (VersionCheck (0, 0x04000002)) {
		obj->Num_Affected_Node_List_Pointers = readuint (READER);
		PROTECT_LEN (obj->Num_Affected_Node_List_Pointers, 2000000, "readNiDynamicEffect")
	}
	if (VersionCheck (0x0A010000, 0)) {
		obj->Num_Affected_Nodes = readuint (READER);
		PROTECT_LEN (obj->Num_Affected_Nodes, 2000000, "readNiDynamicEffect")
	}
	if (VersionCheck (0, 0x04000002)) {
		obj->Affected_Node_List_Pointers =
		    GETMEM ((obj->Num_Affected_Node_List_Pointers) *
			    sizeof (unsigned int));
		READ (uint, obj->Affected_Node_List_Pointers, obj->Num_Affected_Node_List_Pointers, SIZEOFDWORD, "readNiDynamicEffect")
	}
	if (VersionCheck (0x0A010000, 0)) {
		obj->Affected_Nodes =
		    GETMEM ((obj->Num_Affected_Nodes) * sizeof (unsigned int));
		READ (uint, obj->Affected_Nodes, obj->Num_Affected_Nodes, SIZEOFDWORD, "readNiDynamicEffect")
	}
}

void
readNiLight(NiLight * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiDynamicEffect));
	readNiDynamicEffect (obj->parent, 0);
	obj->Dimmer = readfloat (READER);
	readColor3 (&obj->Ambient_Color, 0);
	readColor3 (&obj->Diffuse_Color, 0);
	readColor3 (&obj->Specular_Color, 0);
}

void
readNiProperty(NiProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiObjectNET));
	readNiObjectNET (obj->parent, 0);
}

void
readNiTransparentProperty(NiTransparentProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	READ (byte, &(obj->Unknown[0]), 6, 1, "readNiTransparentProperty")
}

void
readNiPSysModifier(NiPSysModifier * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	obj->Order = readuint (READER);
	obj->Target = readPtr (READER);
	obj->Active = readbool (READER);
}

void
readNiPSysEmitter(NiPSysEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	READ (float, &(obj->Speed), 6, SIZEOFDWORD, "readNiPSysEmitter")
	/*obj->Speed = readfloat (READER);
	obj->Speed_Variation = readfloat (READER);
	obj->Declination = readfloat (READER);
	obj->Declination_Variation = readfloat (READER);
	obj->Planar_Angle = readfloat (READER);
	obj->Planar_Angle_Variation = readfloat (READER);*/
	readColor4 (&obj->Initial_Color, 0);
	obj->Initial_Radius = readfloat (READER);
	if (VersionCheck (0x0A040001, 0)) {
		obj->Radius_Variation = readfloat (READER);
	}
	obj->Life_Span = readfloat (READER);
	obj->Life_Span_Variation = readfloat (READER);
}

void
readNiPSysVolumeEmitter(NiPSysVolumeEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysEmitter));
	readNiPSysEmitter (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Emitter_Object = readPtr (READER);
	}
}

void
readNiTimeController(NiTimeController * obj, unsigned int ARG)
{
	COVERAGE
	obj->Next_Controller = readRef (READER);
	obj->Flags = readFlags (READER);
	obj->Frequency = readfloat (READER);
	obj->Phase = readfloat (READER);
	obj->Start_Time = readfloat (READER);
	obj->Stop_Time = readfloat (READER);
	if (VersionCheck (0x0303000D, 0)) {
		obj->Target = readPtr (READER);
	}
	if (VersionCheck (0, 0x03010000)) {
		obj->Unknown_Integer = readuint (READER);
	}
}

void
readNiInterpController(NiInterpController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
}

void
readNiMultiTargetTransformController(NiMultiTargetTransformController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpController));
	readNiInterpController (obj->parent, 0);
	obj->Num_Extra_Targets = readushort (READER);
	obj->Extra_Targets =
	    GETMEM ((obj->Num_Extra_Targets) * sizeof (unsigned int));
	READ (uint, obj->Extra_Targets, obj->Num_Extra_Targets, SIZEOFDWORD, "readNiMultiTargetTransformController")
}

void
readNiGeomMorpherController(NiGeomMorpherController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpController));
	readNiInterpController (obj->parent, 0);
	if (VersionCheck (0x0A000102, 0)) {
		obj->Extra_Flags = readFlags (READER);
	}
	if (VersionCheck (0x0A01006A, 0x0A01006A)) {
		obj->Unknown_2 = readbyte (READER);
	}
	obj->Data = readRef (READER);
	obj->Always_Update = readbyte (READER);
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Num_Interpolators = readuint (READER);
		PROTECT_LEN (obj->Num_Interpolators, 2000000, "readNiGeomMorpherController")
	}
	if (VersionCheck (0x0A01006A, 0x14000005)) {
		obj->Interpolators =
		    GETMEM ((obj->Num_Interpolators) * sizeof (unsigned int));
		READ (uint, obj->Interpolators, obj->Num_Interpolators, SIZEOFDWORD, "readNiGeomMorpherController")
	}
	if (VersionCheck (0x14010003, 0)) {
		// init 1d array
		obj->Interpolator_Weights =
		    GETMEM ((obj->Num_Interpolators) * sizeof (MorphWeight));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_Interpolators); i++)
			readMorphWeight (&obj->Interpolator_Weights[i], 0);
	}
	if (VersionCheck (0x14000004, 0x14000005)
	    && ((User_Version == 10) || (User_Version == 11))) {
		obj->Num_Unknown_Ints = readuint (READER);
		PROTECT_LEN (obj->Num_Unknown_Ints, 2000000, "readNiGeomMorpherController")
		obj->Unknown_Ints =
		    GETMEM ((obj->Num_Unknown_Ints) * sizeof (unsigned int));
		READ (uint, obj->Unknown_Ints, obj->Num_Unknown_Ints, SIZEOFDWORD, "readNiGeomMorpherController")
	}
}

void
readNiMorphController(NiMorphController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpController));
	readNiInterpController (obj->parent, 0);
}

void
readNiMorpherController(NiMorpherController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpController));
	readNiInterpController (obj->parent, 0);
	obj->Data = readRef (READER);
}

void
readNiSingleInterpController(NiSingleInterpController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpController));
	readNiInterpController (obj->parent, 0);
	if (VersionCheck (0x0A020000, 0)) {
		obj->Interpolator = readRef (READER);
	}
}

void
readNiKeyframeController(NiKeyframeController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiTransformController(NiTransformController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyframeController));
	readNiKeyframeController (obj->parent, 0);
}

void
readNiPSysModifierCtlr(NiPSysModifierCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
	readstring (&obj->Modifier_Name, 0);
}

void
readNiPSysEmitterCtlr(NiPSysEmitterCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
	if (VersionCheck (0x0A020000, 0)) {
		obj->Visibility_Interpolator = readRef (READER);
	}
}

void
readNiPSysModifierBoolCtlr(NiPSysModifierBoolCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
}

void
readNiPSysModifierActiveCtlr(NiPSysModifierActiveCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierBoolCtlr));
	readNiPSysModifierBoolCtlr (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiPSysModifierFloatCtlr(NiPSysModifierFloatCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiPSysEmitterDeclinationCtlr(NiPSysEmitterDeclinationCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysEmitterDeclinationVarCtlr(NiPSysEmitterDeclinationVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysEmitterInitialRadiusCtlr(NiPSysEmitterInitialRadiusCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysEmitterLifeSpanCtlr(NiPSysEmitterLifeSpanCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void readNiPSysEmitterSpeedCtlr(NiPSysEmitterSpeedCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysGravityStrengthCtlr(NiPSysGravityStrengthCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiFloatInterpController(NiFloatInterpController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
}

void
readNiFlipController(NiFlipController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
	obj->Texture_Slot = readuint (READER);
	if (VersionCheck (0x04000000, 0x0A010000)) {
		obj->Unknown_Int_2 = readuint (READER);
	}
	if (VersionCheck (0, 0x0A010000)) {
		obj->Delta = readfloat (READER);
	}
	obj->Num_Sources = readuint (READER);
	PROTECT_LEN (obj->Num_Sources, 2000000, "readNiFlipController")
	if (VersionCheck (0x04000000, 0)) {
		obj->Sources =
		    GETMEM ((obj->Num_Sources) * sizeof (unsigned int));
		READ (uint, obj->Sources, obj->Num_Sources, SIZEOFDWORD, "readNiFlipController")
	}
	if (VersionCheck (0, 0x03010000)) {
		obj->Images =
		    GETMEM ((obj->Num_Sources) * sizeof (unsigned int));
		READ (uint, obj->Images, obj->Num_Sources, SIZEOFDWORD, "readNiFlipController")
	}
}

void
readNiAlphaController(NiAlphaController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiTextureTransformController(NiTextureTransformController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
	obj->Unknown2 = readbyte (READER);
	obj->Texture_Slot = readuint (READER);
	obj->Operation = readuint (READER);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiLightDimmerController(NiLightDimmerController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
}

void
readNiBoolInterpController(NiBoolInterpController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
}

void
readNiVisController(NiVisController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBoolInterpController));
	readNiBoolInterpController (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiPoint3InterpController(NiPoint3InterpController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Target_Color = readushort (READER);
	}
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
}

void
readNiMaterialColorController(NiMaterialColorController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPoint3InterpController));
	readNiPoint3InterpController (obj->parent, 0);
}

void
readNiLightColorController(NiLightColorController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPoint3InterpController));
	readNiPoint3InterpController (obj->parent, 0);
}

void
readNiExtraDataController(NiExtraDataController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
}

void
readNiFloatExtraDataController(NiFloatExtraDataController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraDataController));
	readNiExtraDataController (obj->parent, 0);
	if (VersionCheck (0x0A020000, 0)) {
		readstring (&obj->Controller_Data, 0);
	}
	if (VersionCheck (0, 0x0A010000)) {
		obj->Num_Extra_Bytes = readbyte (READER);
		READ (byte, &(obj->Unknown_Bytes[0]), 7, 1, "readNiFloatExtraDataController")
		obj->Unknown_Extra_Bytes =
		    GETMEM ((obj->Num_Extra_Bytes) * sizeof (byte));
		READ (byte, obj->Unknown_Extra_Bytes, obj->Num_Extra_Bytes, 1, "readNiFloatExtraDataController")
	}
}

void
readNiBoneLODController(NiBoneLODController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	READ (uint, &(obj->Unknown_Int_1), 3, SIZEOFDWORD, "readNiBoneLODController")
	/*obj->Unknown_Int_1 = readuint (READER);
	obj->Num_Node_Groups = readuint (READER);
	obj->Num_Node_Groups_2 = readuint (READER);*/
	PROTECT_LEN (obj->Num_Node_Groups, 2000000, "readNiBoneLODController")
	PROTECT_LEN (obj->Num_Node_Groups_2, 2000000, "readNiBoneLODController")
	// init 1d array
	obj->Node_Groups = GETMEM ((obj->Num_Node_Groups) * sizeof (NodeGroup));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Node_Groups); i++)
		readNodeGroup (&obj->Node_Groups[i], 0);
	if (VersionCheck (0x04020200, 0) && (User_Version == 0)) {
		obj->Num_Shape_Groups = readuint (READER);
		PROTECT_LEN (obj->Num_Shape_Groups, 2000000, "readNiBoneLODController")
		// init 1d array
		obj->Shape_Groups_1 =
		    GETMEM ((obj->Num_Shape_Groups) * sizeof (SkinShapeGroup));
		// read 1d array
		for (i = 0; i < (obj->Num_Shape_Groups); i++)
			readSkinShapeGroup (&obj->Shape_Groups_1[i], 0);
		obj->Num_Shape_Groups_2 = readuint (READER);
		PROTECT_LEN (obj->Num_Shape_Groups_2, 2000000, "readNiBoneLODController")
		obj->Shape_Groups_2 =
		    GETMEM ((obj->Num_Shape_Groups_2) * sizeof (unsigned int));
		READ (uint, obj->Shape_Groups_2, obj->Num_Shape_Groups_2, SIZEOFDWORD, "readNiBoneLODController")
	}
	if (VersionCheck (0x14030009, 0x14030009) && (User_Version == 131072)) {
		obj->Unknown_Int_2 = readint (READER);
		obj->Unknown_Int_3 = readint (READER);
	}
}

void readNiBSBoneLODController (NiBSBoneLODController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBoneLODController));
	readNiBoneLODController (obj->parent, 0);
}

void
readNiGeometry(NiGeometry * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	obj->Data = readRef (READER);
	if (VersionCheck (0x0303000D, 0)) {
		obj->Skin_Instance = readRef (READER);
	}
	if (VersionCheck (0x14020007, 0)) {
		obj->Num_Materials = readuint (READER);
		PROTECT_LEN (obj->Num_Materials, 2000000, "readNiGeometry")
		// init 1d array
		obj->Material_Name =
		    GETMEM ((obj->Num_Materials) * sizeof (string));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_Materials); i++)
			readstring (&obj->Material_Name[i], 0);
		obj->Material_Extra_Data =
		    GETMEM ((obj->Num_Materials) * sizeof (int));
		READ (int, obj->Material_Extra_Data, obj->Num_Materials, SIZEOFDWORD, "readNiGeometry")
		obj->Active_Material = readint (READER);
	}
	if (VersionCheck (0x0A000100, 0x14010003)) {
		obj->Has_Shader = readbool (READER);
		if (obj->Has_Shader != 0) {
			readstring (&obj->Shader_Name, 0);
			obj->Unknown_Integer = readint (READER);
		}
	}
	if ((User_Version == 1) && Version == 0x0A020000) {
		obj->Unknown_Byte = readbyte (READER);
	}
	if (VersionCheck (0x0A040001, 0x0A040001)) {
		obj->Unknown_Integer_2 = readint (READER);
	}
	if (VersionCheck (0x14020007, 0)) {
		obj->Dirty_Flag = readbool (READER);
	}
}

void
readNiTriBasedGeom(NiTriBasedGeom * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiGeometry));
	readNiGeometry (obj->parent, 0);
}

void
readNiGeometryData(NiGeometryData * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x0A020000, 0))
		obj->Unknown_Int = readint (READER);
	obj->Num_Vertices = readushort (READER);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Keep_Flags = readbyte (READER);
		obj->Compress_Flags = readbyte (READER);
	}
	obj->Has_Vertices = readbool (READER);
	if (obj->Has_Vertices != 0) {
		obj->Vertices = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Vertices[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiGeometryData");
	}
	if ( (Version >= 0x0A000100) &&
	      !((Version >= 0x14020007) && (User_Version == 11)))
	    {
		obj->Num_UV_Sets = readushort (READER);
	}
	if ((((Version >= 0x14020007) && (User_Version == 11)))) {
		obj->BS_Num_UV_Sets = readushort (READER);
	}
	obj->Has_Normals = readbool (READER);
	if (obj->Has_Normals != 0) {
		obj->Normals = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Normals[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiGeometryData");
	}
	if (VersionCheck (0x0A010000, 0)
	    && ((obj->Has_Normals != 0)
		&& ((obj->Num_UV_Sets & 61440)
		    || (obj->BS_Num_UV_Sets & 61440)))) {
		obj->Tangents = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Tangents[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiGeometryData");
	}
	if (VersionCheck (0x0A010000, 0)
	    && ((obj->Has_Normals != 0)
		&& ((obj->Num_UV_Sets & 61440)
		    || (obj->BS_Num_UV_Sets & 61440)))) {
		obj->Binormals =
		    GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Binormals[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiGeometryData");
	}
	readVector3 (&obj->Center, 0);
	obj->Radius = readfloat (READER);
	if (VersionCheck (0x14030009, 0x14030009) && (User_Version == 131072)) {
		READ (short, &(obj->Unknown_13_shorts[0]), 13, SIZEOFWORD, "readNiGeometryData")
	}
	obj->Has_Vertex_Colors = readbool (READER);
	if ((obj->Has_Vertex_Colors != 0)) {
		obj->Vertex_Colors =
		    GETMEM ((obj->Num_Vertices) * sizeof (Color4));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Vertex_Colors[0].r), obj->Num_Vertices*4, SIZEOFDWORD, "readNiGeometryData");
	}
	if (VersionCheck (0, 0x04020200)) {
		obj->Num_UV_Sets = readushort (READER);
	}
	if (VersionCheck (0, 0x04000002)) {
		obj->Has_UV = readbool (READER);
	}
	if (!((Version >= 0x14020007) && (User_Version == 11))) {
		NIFuint len = (obj->Num_UV_Sets & 63) * obj->Num_Vertices;
		PROTECT_LEN (len, 2000000, "readNiGeometryData");
		obj->UV_Sets = GETMEM (len * sizeof (TexCoord));
		if (len > 0)
			READ (float, &(obj->UV_Sets[0].u), len*2, SIZEOFDWORD, "readNiGeometryData");
	}
	else {
		NIFuint len = (obj->BS_Num_UV_Sets & 1) * obj->Num_Vertices;
		dbg ("len: %d\n", len);
		PROTECT_LEN (len, 2000000, "readNiGeometryData");
		obj->UV_Sets = GETMEM (len * sizeof (TexCoord));
		if (len > 0)
			READ (float, &(obj->UV_Sets[0].u), len*2, SIZEOFDWORD, "readNiGeometryData");
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Consistency_Flags = readushort (READER);
	}
	if (VersionCheck (0x14000004, 0)) {
		obj->Additional_Data = readRef (READER);
	}
}

void
readNiTriBasedGeomData(NiTriBasedGeomData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiGeometryData));
	readNiGeometryData (obj->parent, 0);
	obj->Num_Triangles = readushort (READER);
}

void
readbhkBlendController(bhkBlendController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Unknown_Int = readuint (READER);
}

void
readBSBound(BSBound * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	readVector3 (&obj->Center, 0);
	readVector3 (&obj->Dimensions, 0);
}

void
readBSFurnitureMarker(BSFurnitureMarker * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Num_Positions = readuint (READER);
	PROTECT_LEN (obj->Num_Positions, 2000000, "readBSFurnitureMarker");
	// init 1d array
	obj->Positions =
	    GETMEM ((obj->Num_Positions) * sizeof (FurniturePosition));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Positions); i++)
		readFurniturePosition (&obj->Positions[i], 0);
}

void
readBSParentVelocityModifier(BSParentVelocityModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Damping = readfloat (READER);
}

void
readBSPSysArrayEmitter(BSPSysArrayEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysVolumeEmitter));
	readNiPSysVolumeEmitter (obj->parent, 0);
}

void
readBSWindModifier(BSWindModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Strength = readfloat (READER);
}

void
readhkPackedNiTriStripsData(hkPackedNiTriStripsData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShapeCollection));
	readbhkShapeCollection (obj->parent, 0);
	obj->Num_Triangles = readuint (READER);
	PROTECT_LEN (obj->Num_Triangles, 2000000, "readhkPackedNiTriStripsData")
	// init 1d array
	obj->Triangles = GETMEM ((obj->Num_Triangles) * sizeof (hkTriangle));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Triangles); i++)
		readhkTriangle (&obj->Triangles[i], 0);
	obj->Num_Vertices = readuint (READER);
	PROTECT_LEN (obj->Num_Vertices, 2000000, "readhkPackedNiTriStripsData")
	if (VersionCheck (0x14020007, 0)) {
		obj->Unknown_Byte_1 = readbyte (READER);
	}
	obj->Vertices = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
	if (obj->Num_Vertices > 0)
		READ (float, &(obj->Vertices[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readhkPackedNiTriStripsData")
	if (VersionCheck (0x14020007, 0)) {
		obj->Num_Sub_Shapes = readushort (READER);
		// init 1d array
		obj->Sub_Shapes =
		    GETMEM ((obj->Num_Sub_Shapes) * sizeof (OblivionSubShape));
		// read 1d array
		for (i = 0; i < (obj->Num_Sub_Shapes); i++)
			readOblivionSubShape (&obj->Sub_Shapes[i], 0);
	}
}

void
readNiAlphaProperty(NiAlphaProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	obj->Threshold = readbyte (READER);
}

void
readNiAmbientLight(NiAmbientLight * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiLight));
	readNiLight (obj->parent, 0);
}

void
readNiParticlesData(NiParticlesData * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x0A020000, 0)) {
		readstring (&obj->Name, 0);
	}
	obj->Num_Vertices = readushort (READER);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Keep_Flags = readbyte (READER);
		obj->Compress_Flags = readbyte (READER);
	}
	obj->Has_Vertices = readbool (READER);
	if ((!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Vertices != 0)) {
		obj->Vertices = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Vertices[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Num_UV_Sets = readbyte (READER);
		obj->TSpace_Flag = readbyte (READER);
	}
	obj->Has_Normals = readbool (READER);
	if ((!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Normals != 0)) {
		obj->Normals = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Normals[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0x0A010000, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))
	    && ((obj->Has_Normals != 0) && (obj->TSpace_Flag & 240))) {
		obj->Tangents = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Tangents[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0x0A010000, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))
	    && ((obj->Has_Normals != 0) && (obj->TSpace_Flag & 240))) {
		obj->Binormals =
		    GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Binormals[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiParticlesData")
	}
	readVector3 (&obj->Center, 0);
	obj->Radius = readfloat (READER);
	obj->Has_Vertex_Colors = readbool (READER);
	if ((!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Vertex_Colors != 0)) {
		obj->Vertex_Colors =
		    GETMEM ((obj->Num_Vertices) * sizeof (Color4));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Vertex_Colors[0].r), obj->Num_Vertices*4, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0, 0x04020200)) {
		obj->Num_UV_Sets = readbyte (READER);
		obj->TSpace_Flag = readbyte (READER);
	}
	if (VersionCheck (0, 0x04000002)) {
		obj->Has_UV = readbool (READER);
	}
	if ((!((Version >= 0x14020007) && (User_Version == 11)))) {
		NIFuint len = (obj->Num_UV_Sets & 63) * obj->Num_Vertices;
		PROTECT_LEN (len, 2000000, "readNiParticlesData");
		obj->UV_Sets = GETMEM (len * sizeof (TexCoord));
		if (len > 0)
			READ (float, &(obj->UV_Sets[0].u), len*2, SIZEOFDWORD, "readNiParticlesData");
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Consistency_Flags = readushort (READER);
	}
	if (VersionCheck (0x14000004, 0)) {
		obj->Additional_Data = readRef (READER);
	}
	if (VersionCheck (0, 0x04000002)) {
		obj->Num_Particles = readushort (READER);
	}
	if (VersionCheck (0, 0x0A000100)) {
		obj->Particle_Radius = readfloat (READER);
	}
	if (VersionCheck (0x0A010000, 0)) {
		obj->Has_Radii = readbool (READER);
	}
	if (VersionCheck (0x0A010000, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Radii != 0)) {
		obj->Radii = GETMEM ((obj->Num_Vertices) * sizeof (float));
		READ (float, obj->Radii, obj->Num_Vertices, SIZEOFDWORD, "readNiParticlesData")
	}
	obj->Num_Active = readushort (READER);
	obj->Has_Sizes = readbool (READER);
	if ((!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Sizes != 0)) {
		obj->Sizes = GETMEM ((obj->Num_Vertices) * sizeof (float));
		READ (float, obj->Sizes, obj->Num_Vertices, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Has_Rotations = readbool (READER);
	}
	if (VersionCheck (0x0A000100, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Rotations != 0)) {
		obj->Rotations =
		    GETMEM ((obj->Num_Vertices) * sizeof (Quaternion));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Rotations[0].w), obj->Num_Vertices*4, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0x14000004, 0)) {
		obj->Has_Rotation_Angles = readbool (READER);
	}
	if ((!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Rotation_Angles != 0)) {
		obj->Rotation_Angles =
		    GETMEM ((obj->Num_Vertices) * sizeof (float));
		READ (float, obj->Rotation_Angles, obj->Num_Vertices, SIZEOFDWORD, "readNiParticlesData")
	}
	if (VersionCheck (0x14000004, 0)) {
		obj->Has_Rotation_Axes = readbool (READER);
	}
	if (VersionCheck (0x14000004, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Rotation_Axes != 0)) {
		obj->Rotation_Axes =
		    GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
		if (obj->Num_Vertices > 0)
			READ (float, &(obj->Rotation_Axes[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiParticlesData")
	}
	if (((Version >= 0x14020007) && (User_Version == 11))) {
		obj->Has_Unknown_Stuff_1 = readbool (READER);
		obj->Num_Unknown_Stuff_1 = readshort (READER);
	    if ((obj->Has_Unknown_Stuff_1 != 0)) {
			obj->Unknown_Stuff_1 =
		    	GETMEM ((obj->Num_Unknown_Stuff_1) * sizeof (Vector4));
			if (obj->Num_Unknown_Stuff_1 > 0)
				READ (float, &(obj->Unknown_Stuff_1[0].x), obj->Num_Unknown_Stuff_1*4, SIZEOFDWORD, "readNiParticlesData")
		}
	}
}

void
readNiRotatingParticlesData(NiRotatingParticlesData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticlesData));
	readNiParticlesData (obj->parent, 0);
	if (VersionCheck (0, 0x04020200)) {
		obj->Has_Rotations_2 = readbool (READER);
		if ((obj->Has_Rotations_2 != 0)) {
			obj->Rotations_2 =
		    	GETMEM ((obj->parent->Num_Vertices) * sizeof (Quaternion));
			if (obj->parent->Num_Vertices > 0)
				READ (float, &(obj->Rotations_2[0].w), obj->parent->Num_Vertices*4, SIZEOFDWORD, "readNiParticlesData")
		}
	}
}

void
readNiAutoNormalParticlesData(NiAutoNormalParticlesData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticlesData));
	readNiParticlesData (obj->parent, 0);
}

void
readParticleDesc(ParticleDesc * obj, unsigned int ARG)
{
	COVERAGE
	readVector3 (&obj->Translation, 0);
	if (VersionCheck (0, 0x0A040001)) {
		READ (float, &(obj->Unknown_Floats_1[0]), 3, SIZEOFDWORD, "readParticleDesc")
	}
	READ (float, &(obj->Unknown_Float_1), 3, SIZEOFDWORD, "readParticleDesc")
	/*obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);*/
	obj->Unknown_Int_1 = readint (READER);
}

void
readNiPSysData(NiPSysData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiRotatingParticlesData));
	readNiRotatingParticlesData (obj->parent, 0);
	if ((!((Version >= 0x14020007) && (User_Version == 11)))) {
		// init 1d array
		obj->Particle_Descriptions =
		    GETMEM ((obj->parent->parent->Num_Vertices) *
			    sizeof (ParticleDesc));
		// read 1d array
		int i;
		for (i = 0; i < (obj->parent->parent->Num_Vertices); i++)
			readParticleDesc (&obj->Particle_Descriptions[i], 0);
	}
	if (VersionCheck (0x14000004, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))) {
		obj->Has_Unknown_Floats_3 = readbool (READER);
	}
	if (VersionCheck (0x14000004, 0)
	    && (!((Version >= 0x14020007) && (User_Version == 11)))
	    && (obj->Has_Unknown_Floats_3 != 0)) {
		obj->Unknown_Floats_3 =
		    GETMEM ((obj->parent->parent->Num_Vertices) * sizeof (float));
		READ (float, obj->Unknown_Floats_3, obj->parent->parent->Num_Vertices, SIZEOFDWORD, "readNiPSysData")
	}
	if ((!((Version >= 0x14020007) && (User_Version == 11)))) {
		obj->Unknown_Short_1 = readushort (READER);
		obj->Unknown_Short_2 = readushort (READER);
	}
}

void
readNiMeshPSysData(NiMeshPSysData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysData));
	readNiPSysData (obj->parent, 0);
	if (VersionCheck (0x0A020000, 0)) {
		obj->Unknown_Int_2 = readuint (READER);
		obj->Unknown_Byte_3 = readbyte (READER);
		obj->Num_Unknown_Ints_1 = readuint (READER);
		PROTECT_LEN (obj->Num_Unknown_Ints_1, 2000000, "readNiMeshPSysData")
		obj->Unknown_Ints_1 =
		    GETMEM ((obj->Num_Unknown_Ints_1) * sizeof (unsigned int));
		READ (uint, obj->Unknown_Ints_1, obj->Num_Unknown_Ints_1, SIZEOFDWORD, "readNiMeshPSysData")
	}
	obj->Unknown_Node = readRef (READER);
}

void
readNiBinaryExtraData(NiBinaryExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	readByteArray (&obj->Binary_Data, 0);
}

void
readNiBinaryVoxelExtraData(NiBinaryVoxelExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Unknown_Int = readuint (READER);
	obj->Data = readRef (READER);
}

void
readNiBinaryVoxelData(NiBinaryVoxelData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Short_1 = readushort (READER);
	obj->Unknown_Short_2 = readushort (READER);
	obj->Unknown_Short_3 = readushort (READER);
	READ (float, &(obj->Unknown_7_Floats[0]), 7, SIZEOFDWORD, "readNiBinaryVoxelData")
	READ (byte, &(obj->Unknown_Bytes_1[0]), 7*12, 1, "readNiBinaryVoxelData")
	obj->Num_Unknown_Vectors = readuint (READER);
	PROTECT_LEN (obj->Num_Unknown_Vectors, 2000000, "readNiBinaryVoxelData");
	obj->Unknown_Vectors =
	    GETMEM ((obj->Num_Unknown_Vectors) * sizeof (Vector4));
	if (obj->Num_Unknown_Vectors > 0)
		READ (float, &(obj->Unknown_Vectors[0].x), obj->Num_Unknown_Vectors*4, SIZEOFDWORD, "readNiBinaryVoxelData")
	obj->Num_Unknown_Bytes_2 = readuint (READER);
	PROTECT_LEN (obj->Num_Unknown_Bytes_2, 2000000, "readNiBinaryVoxelData");
	obj->Unknown_Bytes_2 = GETMEM (obj->Num_Unknown_Bytes_2);
	READ (byte, obj->Unknown_Bytes_2, obj->Num_Unknown_Bytes_2, 1, "readNiBinaryVoxelData")
	READ (uint, &(obj->Unknown_5_Ints[0]), 5, SIZEOFDWORD, "readNiBinaryVoxelData")
}

void
readNiBlendBoolInterpolator(NiBlendBoolInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBlendInterpolator));
	readNiBlendInterpolator (obj->parent, 0);
	obj->Bool_Value = readbyte (READER);
}

void
readNiBlendFloatInterpolator(NiBlendFloatInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBlendInterpolator));
	readNiBlendInterpolator (obj->parent, 0);
	obj->Float_Value = readfloat (READER);
}

void
readNiBlendPoint3Interpolator(NiBlendPoint3Interpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBlendInterpolator));
	readNiBlendInterpolator (obj->parent, 0);
	readVector3 (&obj->Point_Value, 0);
}

void
readNiBlendTransformInterpolator(NiBlendTransformInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBlendInterpolator));
	readNiBlendInterpolator (obj->parent, 0);
}

void
readNiBoolData(NiBoolData * obj, unsigned int ARG)
{
	COVERAGE
	readKeyGroup (&obj->Data, 0, T_BYTE);
}

void
readNiBooleanExtraData(NiBooleanExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Boolean_Data = readbyte (READER);
}

void readNiBSplineBasisData (NiBSplineBasisData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Control_Points = readuint (READER);
}

void
readNiBSplineFloatInterpolator(NiBSplineFloatInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBSplineInterpolator));
	readNiBSplineInterpolator (obj->parent, 0);
}

void
readNiBSplineCompFloatInterpolator(NiBSplineCompFloatInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBSplineFloatInterpolator));
	readNiBSplineFloatInterpolator (obj->parent, 0);
	obj->Base = readfloat (READER);
	obj->Offset = readuint (READER);
	obj->Bias = readfloat (READER);
	obj->Multiplier = readfloat (READER);
}

void
readNiBSplinePoint3Interpolator(NiBSplinePoint3Interpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBSplineInterpolator));
	readNiBSplineInterpolator (obj->parent, 0);
	READ (float, &(obj->Unknown_Floats[0]), 6, SIZEOFDWORD, "readNiBSplinePoint3Interpolator")
}

void
readNiBSplineCompPoint3Interpolator(NiBSplineCompPoint3Interpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBSplinePoint3Interpolator));
	readNiBSplinePoint3Interpolator (obj->parent, 0);
}

void
readNiBSplineTransformInterpolator(NiBSplineTransformInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBSplineInterpolator));
	readNiBSplineInterpolator (obj->parent, 0);
	readVector3 (&obj->Translation, 0);
	readQuaternion (&obj->Rotation, 0);
	obj->Scale = readfloat (READER);
	obj->Translation_Offset = readuint (READER);
	obj->Rotation_Offset = readuint (READER);
	obj->Scale_Offset = readuint (READER);
}

void
readNiBSplineCompTransformInterpolator(NiBSplineCompTransformInterpolator *obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiBSplineTransformInterpolator));
	readNiBSplineTransformInterpolator (obj->parent, 0);
	obj->Translation_Bias = readfloat (READER);
	obj->Translation_Multiplier = readfloat (READER);
	obj->Rotation_Bias = readfloat (READER);
	obj->Rotation_Multiplier = readfloat (READER);
	obj->Scale_Bias = readfloat (READER);
	obj->Scale_Multiplier = readfloat (READER);
}

void
readBSRotAccumTransfInterpolator(BSRotAccumTransfInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTransformInterpolator));
	readNiTransformInterpolator (obj->parent, 0);
}

void
readNiBSplineData(NiBSplineData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Float_Control_Points = readuint (READER);
	PROTECT_LEN (obj->Num_Float_Control_Points, 2000000, "readNiBSplineData")
	obj->Float_Control_Points =
	    GETMEM ((obj->Num_Float_Control_Points) * sizeof (float));
	READ (float, obj->Float_Control_Points, obj->Num_Float_Control_Points, SIZEOFDWORD, "readNiBSplineData")
	obj->Num_Short_Control_Points = readuint (READER);
	PROTECT_LEN (obj->Num_Short_Control_Points, 2000000, "readNiBSplineData")
	obj->Short_Control_Points =
	    GETMEM ((obj->Num_Short_Control_Points) * sizeof (short));
	READ (short, obj->Short_Control_Points, obj->Num_Short_Control_Points, SIZEOFWORD, "readNiBSplineData")
}

void
readNiCamera(NiCamera * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Unknown_Short = readushort (READER);
	}
	READ (float, &(obj->Frustum_Left), 6, SIZEOFDWORD, "readNiCamera")
	/*obj->Frustum_Left = readfloat (READER);
	obj->Frustum_Right = readfloat (READER);
	obj->Frustum_Top = readfloat (READER);
	obj->Frustum_Bottom = readfloat (READER);
	obj->Frustum_Near = readfloat (READER);
	obj->Frustum_Far = readfloat (READER);*/
	if (VersionCheck (0x0A010000, 0)) {
		obj->Use_Orthographic_Projection = readbool (READER);
	}
	READ (float, &(obj->Viewport_Left), 5, SIZEOFDWORD, "readNiCamera")
	/*obj->Viewport_Left = readfloat (READER);
	obj->Viewport_Right = readfloat (READER);
	obj->Viewport_Top = readfloat (READER);
	obj->Viewport_Bottom = readfloat (READER);
	obj->LOD_Adjust = readfloat (READER);*/
	obj->Unknown_Link = readRef (READER);
	obj->Unknown_Int = readuint (READER);
	if (VersionCheck (0x04020100, 0)) {
		obj->Unknown_Int_2 = readuint (READER);
	}
	if (VersionCheck (0, 0x03010000)) {
		obj->Unknown_Int_3 = readuint (READER);
	}
}

void
readNiColorData(NiColorData * obj, unsigned int ARG)
{
	COVERAGE
	readKeyGroup (&obj->Data, 0, T_COLOR4);
}

void
readNiColorExtraData(NiColorExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	readColor4 (&obj->Data, 0);
}

void
readNiControllerManager(NiControllerManager * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Cumulative = readbool (READER);
	obj->Num_Controller_Sequences = readuint (READER);
	PROTECT_LEN (obj->Num_Controller_Sequences, 2000000, "readNiControllerManager")
	obj->Controller_Sequences =
	    GETMEM ((obj->Num_Controller_Sequences) * sizeof (unsigned int));
	READ (uint, obj->Controller_Sequences, obj->Num_Controller_Sequences, SIZEOFDWORD, "readNiControllerManager")
	obj->Object_Palette = readRef (READER);
}

void
readNiSequence(NiSequence * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	if (VersionCheck (0, 0x0A010000)) {
		readstring (&obj->Text_Keys_Name, 0);
		obj->Text_Keys = readRef (READER);
	}
	if (VersionCheck (0x14030009, 0x14030009) && (User_Version == 131072)) {
		obj->Unknown_Int_4 = readint (READER);
		obj->Unknown_Int_5 = readint (READER);
	}
	obj->Num_Controlled_Blocks = readuint (READER);
	PROTECT_LEN (obj->Num_Controlled_Blocks, 2000000, "readNiSequence")
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Unknown_Int_1 = readuint (READER);
	}
	// init 1d array
	int i;
	obj->Controlled_Blocks =
	    GETMEM ((obj->Num_Controlled_Blocks) * sizeof (ControllerLink));
	// read 1d array
	for (i = 0; i < (obj->Num_Controlled_Blocks); i++)
		readControllerLink (&obj->Controlled_Blocks[i], 0);
}

void
readNiControllerSequence(NiControllerSequence * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSequence));
	readNiSequence (obj->parent, 0);
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Weight = readfloat (READER);
		obj->Text_Keys = readRef (READER);
		obj->Cycle_Type = readuint (READER);
	}
	if (VersionCheck (0x0A01006A, 0x0A01006A)) {
		obj->Unknown_Int_0 = readuint (READER);
	}
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Frequency = readfloat (READER);
		obj->Start_Time = readfloat (READER);
	}
	if (VersionCheck (0x0A020000, 0x0A040001)) {
		obj->Unknown_Float_2 = readfloat (READER);
	}
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Stop_Time = readfloat (READER);
	}
	if (VersionCheck (0x0A01006A, 0x0A01006A)) {
		obj->Unknown_Byte = readbyte (READER);
	}
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Manager = readPtr (READER);
	}
	if (VersionCheck (0x0A01006A, 0)) {
		readstring (&obj->Target_Name, 0);
	}
	if (VersionCheck (0x0A020000, 0x14000005)) {
		obj->String_Palette = readRef (READER);
	}
	if (VersionCheck (0x14020007, 0)
	    && ((User_Version == 11) && (User_Version_2 >= 24))) {
		obj->Unknown_Short_1 = readshort (READER);
		if (User_Version_2 <= 28)
			obj->Unknown_Short_2 = readshort (READER);
	}
	if (VersionCheck (0x14030009, 0)) {
		obj->Unknown_Int_3 = readuint (READER);
	}
}

inline void
readNiAVObjectPalette (NiAVObjectPalette * obj, unsigned int ARG)
{
	COVERAGE
}

void
readNiDefaultAVObjectPalette(NiDefaultAVObjectPalette * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObjectPalette));
	readNiAVObjectPalette (obj->parent, 0);
	obj->Unknown_Int = readuint (READER);
	obj->Num_Objs = readuint (READER);
	PROTECT_LEN (obj->Num_Objs, 2000000, "readNiDefaultAVObjectPalette")
	// init 1d array
	obj->Objs = GETMEM ((obj->Num_Objs) * sizeof (AVObject));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Objs); i++)
		readAVObject (&obj->Objs[i], 0);
}

void
readNiDirectionalLight(NiDirectionalLight * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiLight));
	readNiLight (obj->parent, 0);
}

void
readNiDitherProperty(NiDitherProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
}

void
readNiRollController(NiRollController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSingleInterpController));
	readNiSingleInterpController (obj->parent, 0);
	obj->Data = readRef (READER);
}

void
readNiFloatData(NiFloatData * obj, unsigned int ARG)
{
	COVERAGE
	readKeyGroup (&obj->Data, 0, T_FLOAT);
}

void
readNiFloatExtraData(NiFloatExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Float_Data = readfloat (READER);
}

void
readNiFloatsExtraData(NiFloatsExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Num_Floats = readuint (READER);
	PROTECT_LEN (obj->Num_Floats, 2000000, "readNiFloatsExtraData")
	obj->Data = GETMEM ((obj->Num_Floats) * sizeof (float));
	READ (float, obj->Data, obj->Num_Floats, SIZEOFDWORD, "readNiFloatsExtraData")
}

void
readNiFogProperty(NiFogProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	obj->Fog_Depth = readfloat (READER);
	readColor3 (&obj->Fog_Color, 0);
}

void 
readNiGravity(NiGravity * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	if (VersionCheck (0x04000002, 0)) {
		obj->Unknown_Float_1 = readfloat (READER);
	}
	obj->Force = readfloat (READER);
	obj->Type = readuint (READER);
	readVector3 (&obj->Position, 0);
	readVector3 (&obj->Direction, 0);
}

void
readNiIntegerExtraData(NiIntegerExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Integer_Data = readuint (READER);
}

void
readBSXFlags(BSXFlags * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiIntegerExtraData));
	readNiIntegerExtraData (obj->parent, 0);
}

void
readNiIntegersExtraData(NiIntegersExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Num_Integers = readuint (READER);
	PROTECT_LEN (obj->Num_Integers, 2000000, "readNiIntegersExtraData")
	obj->Data = GETMEM ((obj->Num_Integers) * sizeof (unsigned int));
	READ (uint, obj->Data, obj->Num_Integers, SIZEOFDWORD, "readNiIntegersExtraData")
}

void
readBSKeyframeController(BSKeyframeController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyframeController));
	readNiKeyframeController (obj->parent, 0);
	obj->Data_2 = readRef (READER);
}

void
readNiKeyframeData(NiKeyframeData * obj, unsigned int ARG)
{
	COVERAGE
	int i;
	obj->Num_Rotation_Keys = readuint (READER);
	PROTECT_LEN (obj->Num_Rotation_Keys, 2000000, "readNiKeyframeData")
	if ((obj->Num_Rotation_Keys != 0)) {
		obj->Rotation_Type = readuint (READER);
	}
	if ((obj->Rotation_Type != 4)) {
		// init 1d array
		obj->Quaternion_Keys =
		    GETMEM ((obj->Num_Rotation_Keys) * sizeof (QuatKey));
		// read 1d array
		for (i = 0; i < (obj->Num_Rotation_Keys); i++)
			readQuatKey (&obj->Quaternion_Keys[i], obj->Rotation_Type);
	}
	if (VersionCheck (0, 0x0A010000) && (obj->Rotation_Type == 4)) {
		obj->Unknown_Float = readfloat (READER);
	}
	if ((obj->Rotation_Type == 4)) {
		// read 1d array
		for (i = 0; i < (3); i++)
			readKeyGroup (&obj->XYZ_Rotations[i], 0, T_FLOAT);
	}
	readKeyGroup (&obj->Translations, 0, T_VECTOR3);
	readKeyGroup (&obj->Scales, 0, T_FLOAT);
}

void
readNiLookAtController(NiLookAtController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Unknown1 = readushort (READER);
	}
	obj->Look_At_Node = readRef (READER);
}

void
readNiLookAtInterpolator(NiLookAtInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpolator));
	readNiInterpolator (obj->parent, 0);
	obj->Unknown_Short = readushort (READER);
	obj->Look_At = readPtr (READER);
	readstring (&obj->Target, 0);
	if (VersionCheck (0, 0x14050000)) {
		readVector3 (&obj->Translation, 0);
		readQuaternion (&obj->Rotation, 0);
		obj->Scale = readfloat (READER);
	}
	READ (uint, &(obj->Unknown_Link_1), 3, SIZEOFDWORD, "readNiLookAtInterpolator")
	/*obj->Unknown_Link_1 = readRef (READER);
	obj->Unknown_Link_2 = readRef (READER);
	obj->Unknown_Link_3 = readRef (READER);*/
}

void
readNiMaterialProperty(NiMaterialProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	if (VersionCheck (0, 0x0A000102)) {
		obj->Flags = readFlags (READER);
	}
	if ((!((Version == 0x14020007)
		&& (User_Version == 11) && (User_Version_2 > 21)))) {
		readColor3 (&obj->Ambient_Color, 0);
		readColor3 (&obj->Diffuse_Color, 0);
	}
	readColor3 (&obj->Specular_Color, 0);
	readColor3 (&obj->Emissive_Color, 0);
	obj->Glossiness = readfloat (READER);
	obj->Alpha = readfloat (READER);
	if (((Version == 0x14020007)
		&& (User_Version == 11) && (User_Version_2 > 21)) ) {
		obj->Emit_Multi = readfloat (READER);
	}
}

void
readNiMorphData(NiMorphData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Morphs = readuint (READER);
	obj->Num_Vertices = readuint (READER);
	obj->Relative_Targets = readbyte (READER);
	// init 1d array
	obj->Morphs = GETMEM ((obj->Num_Morphs) * sizeof (Morph));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Morphs); i++)
		readMorph (&obj->Morphs[i], obj->Num_Vertices);
}

inline void
readNiNode(NiNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	obj->Num_Children = readuint (READER);
	PROTECT_LEN (obj->Num_Children, 2000000, "readNiNode")
	obj->Children = GETMEM ((obj->Num_Children) * sizeof (unsigned int));
	READ (uint, obj->Children, obj->Num_Children, SIZEOFDWORD, "readNiNode")
	obj->Num_Effects = readuint (READER);
	PROTECT_LEN (obj->Num_Effects, 2000000, "readNiNode")
	obj->Effects = GETMEM ((obj->Num_Effects) * sizeof (unsigned int));
	READ (uint, obj->Effects, obj->Num_Effects, SIZEOFDWORD, "readNiNode")
}

void
readNiBone(NiBone * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
}

void
readAvoidNode(AvoidNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
}

void
readFxWidget(FxWidget * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Unknown1 = readbyte (READER);
	READ (byte, &(obj->Unknown_292_Bytes[0]), 292, 1, "readFxWidget")
}

void
readFxButton(FxButton * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (FxWidget));
	readFxWidget (obj->parent, 0);
}

void
readFxRadioButton(FxRadioButton * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (FxWidget));
	readFxWidget (obj->parent, 0);
	READ (uint, &(obj->Unknown_Int_1), 4, SIZEOFDWORD, "readFxRadioButton")
	/*obj->Unknown_Int_1 = readuint (READER);
	obj->Unknown_Int__2 = readuint (READER);
	obj->Unknown_Int_3 = readuint (READER);
	obj->Num_Buttons = readuint (READER);*/
	PROTECT_LEN (obj->Num_Buttons, 2000000, "readFxRadioButton")
	obj->Buttons = GETMEM ((obj->Num_Buttons) * sizeof (unsigned int));
	READ (uint, obj->Buttons, obj->Num_Buttons, SIZEOFDWORD, "readFxRadioButton")
}

void
readNiBillboardNode(NiBillboardNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Billboard_Mode = readushort (READER);
	}
}

void
readNiBSAnimationNode(NiBSAnimationNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
}

void
readNiBSParticleNode(NiBSParticleNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
}

void
readNiSwitchNode(NiSwitchNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Unknown_Flags_1 = readushort (READER);
	}
	obj->Unknown_Int_1 = readint (READER);
}

void
readNiLODNode(NiLODNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSwitchNode));
	readNiSwitchNode (obj->parent, 0);
	if (VersionCheck (0x04000002, 0x0A000100)) {
		readVector3 (&obj->LOD_Center, 0);
	}
	if (VersionCheck (0, 0x0A000100)) {
		obj->Num_LOD_Levels = readuint (READER);
		PROTECT_LEN (obj->Num_LOD_Levels, 2000000, "readNiLODNode")
		// init 1d array
		obj->LOD_Levels =
		    GETMEM ((obj->Num_LOD_Levels) * sizeof (LODRange));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_LOD_Levels); i++)
			readLODRange (&obj->LOD_Levels[i], 0);
	}
	if (VersionCheck (0x0A010000, 0)) {
		obj->LOD_Level_Data = readRef (READER);
	}
}

void
readNiPalette(NiPalette * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Byte = readbyte (READER);
	obj->Num_Entries = readuint (READER);
	PROTECT_LEN (obj->Num_Entries, 2000000, "readNiPalette")
	READ (byte, &(obj->Palette[0].r), 256*4, 1, "readNiPalette")
}

void
readNiParticleBomb(NiParticleBomb * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	READ (float, &(obj->Decay_), 4, SIZEOFDWORD, "readNiParticleBomb")
	/*obj->Decay_ = readfloat (READER);
	obj->Duration_ = readfloat (READER);
	obj->DeltaV_ = readfloat (READER);
	obj->Start_ = readfloat (READER);*/
	obj->Decay_Type_ = readuint (READER);
	if (VersionCheck (0x0401000C, 0)) {
		obj->Symmetry_Type_ = readuint (READER);
	}
	readVector3 (&obj->Position_, 0);
	readVector3 (&obj->Direction_, 0);
}

void
readNiParticleColorModifier(NiParticleColorModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	obj->Color_Data = readRef (READER);
}

void
readNiParticleGrowFade(NiParticleGrowFade * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	obj->Grow = readfloat (READER);
	obj->Fade = readfloat (READER);
}

void
readNiParticleMeshModifier(NiParticleMeshModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	obj->Num_Particle_Meshes = readuint (READER);
	PROTECT_LEN (obj->Num_Particle_Meshes, 2000000, "readNiParticleMeshModifier")
	obj->Particle_Meshes =
	    GETMEM ((obj->Num_Particle_Meshes) * sizeof (unsigned int));
	READ (uint, obj->Particle_Meshes, obj->Num_Particle_Meshes, SIZEOFDWORD, "readNiParticleMeshModifier")
}

void
readNiParticleRotation(NiParticleRotation * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	obj->Random_Initial_Axis_ = readbyte (READER);
	readVector3 (&obj->Initial_Axis_, 0);
	obj->Rotation_Speed_ = readfloat (READER);
}

void
readNiParticles(NiParticles * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiGeometry));
	readNiGeometry (obj->parent, 0);
}

void
readNiAutoNormalParticles(NiAutoNormalParticles * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticles));
	readNiParticles (obj->parent, 0);
}

void
readNiParticleMeshes(NiParticleMeshes * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticles));
	readNiParticles (obj->parent, 0);
}

void
readNiParticleMeshesData(NiParticleMeshesData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiRotatingParticlesData));
	readNiRotatingParticlesData (obj->parent, 0);
	obj->Unknown_Link_2 = readRef (READER);
}

void
readNiParticleSystem(NiParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticles));
	readNiParticles (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->World_Space = readbool (READER);
		obj->Num_Modifiers = readuint (READER);
		PROTECT_LEN (obj->Num_Modifiers, 2000000, "readNiParticleSystem")
		obj->Modifiers =
		    GETMEM ((obj->Num_Modifiers) * sizeof (unsigned int));
		READ (uint, obj->Modifiers, obj->Num_Modifiers, SIZEOFDWORD, "readNiParticleSystem")
	}
}

void
readNiMeshParticleSystem(NiMeshParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleSystem));
	readNiParticleSystem (obj->parent, 0);
}

void
readNiParticleSystemController(NiParticleSystemController * obj, unsigned int ARG)
{
	COVERAGE
	int i;
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	if (VersionCheck (0, 0x03010000)) {
		obj->Old_Speed = readuint (READER);
	}
	if (VersionCheck (0x0303000D, 0)) {
		obj->Speed = readfloat (READER);
	}
	READ (float, &(obj->Speed_Random), 5, SIZEOFDWORD, 	"readNiParticleSystemController")
	/*obj->Speed_Random = readfloat (READER);
	obj->Vertical_Direction = readfloat (READER);
	obj->Vertical_Angle = readfloat (READER);
	obj->Horizontal_Direction = readfloat (READER);
	obj->Horizontal_Angle = readfloat (READER);*/
	readVector3 (&obj->Unknown_Normal_, 0);
	readColor4 (&obj->Unknown_Color_, 0);
	obj->Size = readfloat (READER);
	obj->Emit_Start_Time = readfloat (READER);
	obj->Emit_Stop_Time = readfloat (READER);
	if (VersionCheck (0x04000002, 0)) {
		obj->Unknown_Byte = readbyte (READER);
	}
	if (VersionCheck (0, 0x03010000)) {
		obj->Old_Emit_Rate = readuint (READER);
	}
	if (VersionCheck (0x0303000D, 0)) {
		obj->Emit_Rate = readfloat (READER);
	}
	obj->Lifetime = readfloat (READER);
	obj->Lifetime_Random = readfloat (READER);
	if (VersionCheck (0x04000002, 0)) {
		obj->Emit_Flags = readushort (READER);
	}
	readVector3 (&obj->Start_Random, 0);
	obj->Emitter = readPtr (READER);
	if (VersionCheck (0x04000002, 0)) {
		obj->Unknown_Short_2_ = readushort (READER);
		obj->Unknown_Float_13_ = readfloat (READER);
		obj->Unknown_Int_1_ = readuint (READER);
		obj->Unknown_Int_2_ = readuint (READER);
		obj->Unknown_Short_3_ = readushort (READER);
	}
	if (VersionCheck (0, 0x03010000)) {
		readVector3 (&obj->Particle_Velocity, 0);
		readVector3 (&obj->Particle_Unknown_Vector, 0);
		obj->Particle_Lifetime = readfloat (READER);
		obj->Particle_Link = readRef (READER);
		obj->Particle_Timestamp = readuint (READER);
		obj->Particle_Unknown_Short = readushort (READER);
		obj->Particle_Vertex_Id = readushort (READER);
	}
	if (VersionCheck (0x04000002, 0)) {
		obj->Num_Particles = readushort (READER);
		obj->Num_Valid = readushort (READER);
		// init 1d array
		obj->Particles =
		    GETMEM ((obj->Num_Particles) * sizeof (Particle));
		// read 1d array
		for (i = 0; i < (obj->Num_Particles); i++)
			readParticle (&obj->Particles[i], 0);
	}
	if (VersionCheck (0x04000002, 0)) {
		obj->Unknown_Link = readRef (READER);
	}
	obj->Particle_Extra = readRef (READER);
	obj->Unknown_Link_2 = readRef (READER);
	if (VersionCheck (0x04000002, 0)) {
		obj->Trailer = readbyte (READER);
	}
	if (VersionCheck (0, 0x03010000)) {
		obj->Color_Data = readRef (READER);
		obj->Unknown_Float_1 = readfloat (READER);
		obj->Unknown_Floats_2 =
		    GETMEM ((obj->Particle_Unknown_Short) * sizeof (float));
		READ (float, obj->Unknown_Floats_2, obj->Particle_Unknown_Short, SIZEOFDWORD, "readNiParticleSystemController")
	}
}

void
readNiBSPArrayController(NiBSPArrayController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleSystemController));
	readNiParticleSystemController (obj->parent, 0);
}

void
readNiPathController(NiPathController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	if (VersionCheck (0x0A010000, 0)) {
		obj->Unknown_Short_2 = readushort (READER);
	}
	obj->Unknown_Int_1 = readuint (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);
	obj->Unknown_Short = readushort (READER);
	obj->Pos_Data = readRef (READER);
	obj->Float_Data = readRef (READER);
}

void
readChannelData(ChannelData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Type = readuint (READER);
	obj->Convention = readuint (READER);
	obj->Bits_Per_Channel = readbyte (READER);
	obj->Unknown_Byte_1 = readbyte (READER);
}

void
readATextureRenderData(ATextureRenderData * obj, unsigned int ARG)
{
	COVERAGE
	int i;
	obj->Pixel_Format = readuint (READER);
	if (VersionCheck (0, 0x0A020000)) {
		obj->Red_Mask = readuint (READER);
		obj->Green_Mask = readuint (READER);
		obj->Blue_Mask = readuint (READER);
		obj->Alpha_Mask = readuint (READER);
		obj->Bits_Per_Pixel = readbyte (READER);
		// TODO: odd
		READ (byte, &(obj->Unknown_3_Bytes[0]), 3, 1, "readATextureRenderData")
		READ (byte, &(obj->Unknown_8_Bytes[0]), 8, 1, "readATextureRenderData")
	}
	if (VersionCheck (0x0A010000, 0x0A020000)) {
		obj->Unknown_Int = readuint (READER);
	}
	if (VersionCheck (0x14000004, 0)) {
		obj->Bits_Per_Pixel = readbyte (READER);
		obj->Unknown_Int_2 = readint (READER);
		obj->Unknown_Int_3 = readuint (READER);
		obj->Flags = readbyte (READER);
		obj->Unknown_Int_4 = readuint (READER);
	}
	if (VersionCheck (0x14030006, 0)) {
		obj->Unknown_Byte_1 = readbyte (READER);
	}
	if (VersionCheck (0x14000004, 0)) {
		for (i = 0; i < (4); i++)
			readChannelData (&obj->Channels[i], 0);
	}
	obj->Palette = readRef (READER);
	obj->Num_Mipmaps = readuint (READER);
	PROTECT_LEN (obj->Num_Mipmaps, 2000000, "readATextureRenderData")
	obj->Bytes_Per_Pixel = readuint (READER);
	obj->Mipmaps = GETMEM ((obj->Num_Mipmaps) * sizeof (MipMap));
	READ (uint, &(obj->Mipmaps[0].Width), obj->Num_Mipmaps*3, SIZEOFDWORD, "readATextureRenderData")
}

void
readNiPersistentSrcTextureRendererData(NiPersistentSrcTextureRendererData *obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (ATextureRenderData));
	readATextureRenderData (obj->parent, 0);
	obj->Num_Pixels = readuint (READER);
	PROTECT_LEN (obj->Num_Pixels, 2000000, "readNiPersistentSrcTextureRendererData")
	obj->Unknown_Int_6 = readuint (READER);
	obj->Num_Faces = readuint (READER);
	PROTECT_LEN (obj->Num_Faces, 2000000, "readNiPersistentSrcTextureRendererData")
	obj->Unknown_Int_7 = readuint (READER);
	NIFuint len = obj->Num_Faces * obj->Num_Pixels;
	PROTECT_LEN (len, 2000000, "readNiPersistentSrcTextureRendererData")
	obj->Pixel_Data = GETMEM (len);
	READ (byte, obj->Pixel_Data, len, 1, "readNiPersistentSrcTextureRendererData")
}

void
readNiPixelData(NiPixelData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (ATextureRenderData));
	readATextureRenderData (obj->parent, 0);
	obj->Num_Pixels = readuint (READER);
	PROTECT_LEN (obj->Num_Pixels, 5000000, "readNiPixelData")
	if (VersionCheck (0x14000004, 0)) {
		obj->Num_Faces = readuint (READER);
		PROTECT_LEN (obj->Num_Faces, 64, "readNiPixelData")
		NIFuint len = obj->Num_Faces * obj->Num_Pixels;
		PROTECT_LEN (len, 64*5000000, "readNiPixelData")
		obj->Pixel_Data = GETMEM (len);
		READ (byte, obj->Pixel_Data, len, 1, "readNiPixelData")
	}
	else if (VersionCheck (0, 0x0A020000)) {
		NIFuint len = obj->Num_Pixels;
		PROTECT_LEN (len, 5000000, "readNiPixelData")
		obj->Pixel_Data = GETMEM (len);
		READ (byte, obj->Pixel_Data, len, 1, "readNiPixelData")
	}
}

void
readNiPlanarCollider(NiPlanarCollider * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	if (VersionCheck (0x0A000100, 0)) {
		obj->Unknown_Short = readushort (READER);
	}
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	if (VersionCheck (0x04020200, 0x04020200)) {
		obj->Unknown_Short_2 = readushort (READER);
	}
	READ (float, &(obj->Unknown_Float_3), 14, SIZEOFDWORD, "readNiPlanarCollider")
	/*obj->Unknown_Float_3 = readfloat (READER);
	obj->Unknown_Float_4 = readfloat (READER);
	obj->Unknown_Float_5 = readfloat (READER);
	obj->Unknown_Float_6 = readfloat (READER);
	obj->Unknown_Float_7 = readfloat (READER);
	obj->Unknown_Float_8 = readfloat (READER);
	obj->Unknown_Float_9 = readfloat (READER);
	obj->Unknown_Float_10 = readfloat (READER);
	obj->Unknown_Float_11 = readfloat (READER);
	obj->Unknown_Float_12 = readfloat (READER);
	obj->Unknown_Float_13 = readfloat (READER);
	obj->Unknown_Float_14 = readfloat (READER);
	obj->Unknown_Float_15 = readfloat (READER);
	obj->Unknown_Float_16 = readfloat (READER);*/
}

void
readNiPointLight(NiPointLight * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiLight));
	readNiLight (obj->parent, 0);
	obj->Constant_Attenuation = readfloat (READER);
	obj->Linear_Attenuation = readfloat (READER);
	obj->Quadratic_Attenuation = readfloat (READER);
}

void
readNiPosData(NiPosData * obj, unsigned int ARG)
{
	COVERAGE
	readKeyGroup (&obj->Data, 0, T_VECTOR3);
}

void
readNiPSysAgeDeathModifier(NiPSysAgeDeathModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Spawn_on_Death = readbool (READER);
	obj->Spawn_Modifier = readRef (READER);
}

void
readNiPSysBombModifier(NiPSysBombModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Bomb_Object = readPtr (READER);
	readVector3 (&obj->Bomb_Axis, 0);
	obj->Decay = readfloat (READER);
	obj->Delta_V = readfloat (READER);
	obj->Decay_Type = readuint (READER);
	obj->Symmetry_Type = readuint (READER);
}

void
readNiPSysBoundUpdateModifier(NiPSysBoundUpdateModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Update_Skip = readushort (READER);
}

void
readNiPSysBoxEmitter(NiPSysBoxEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysVolumeEmitter));
	readNiPSysVolumeEmitter (obj->parent, 0);
	obj->Width = readfloat (READER);
	obj->Height = readfloat (READER);
	obj->Depth = readfloat (READER);
}

void
readNiPSysColliderManager(NiPSysColliderManager * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Collider = readRef (READER);
}

void
readNiPSysColorModifier(NiPSysColorModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Data = readRef (READER);
}

void
readNiPSysCylinderEmitter(NiPSysCylinderEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysVolumeEmitter));
	readNiPSysVolumeEmitter (obj->parent, 0);
	obj->Radius = readfloat (READER);
	obj->Height = readfloat (READER);
}

void
readNiPSysDragModifier(NiPSysDragModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Parent = readPtr (READER);
	readVector3 (&obj->Drag_Axis, 0);
	obj->Percentage = readfloat (READER);
	obj->Range = readfloat (READER);
	obj->Range_Falloff = readfloat (READER);
}

void
readNiPSysEmitterCtlrData(NiPSysEmitterCtlrData * obj, unsigned int ARG)
{
	COVERAGE
	readKeyGroup (&obj->Float_Keys_, 0, T_FLOAT);
	obj->Num_Visibility_Keys_ = readuint (READER);
	PROTECT_LEN (obj->Num_Visibility_Keys_, 2000000, "readNiPSysEmitterCtlrData")
	// init 1d array
	obj->Visibility_Keys_ =
	    GETMEM ((obj->Num_Visibility_Keys_) * sizeof (Key));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Visibility_Keys_); i++)
		readKey (&obj->Visibility_Keys_[i], 1, T_BYTE);
}

void
readNiPSysGravityModifier(NiPSysGravityModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Gravity_Object = readPtr (READER);
	readVector3 (&obj->Gravity_Axis, 0);
	obj->Decay = readfloat (READER);
	obj->Strength = readfloat (READER);
	obj->Force_Type = readuint (READER);
	obj->Turbulence = readfloat (READER);
	obj->Turbulence_Scale = readfloat (READER);
	if (VersionCheck (0x14020007, 0) && (User_Version == 11)) {
		obj->Unknown_Byte = readbyte (READER);
	}
}

void
readNiPSysGrowFadeModifier(NiPSysGrowFadeModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Grow_Time = readfloat (READER);
	obj->Grow_Generation = readushort (READER);
	obj->Fade_Time = readfloat (READER);
	obj->Fade_Generation = readushort (READER);
	if (VersionCheck (0x14020007, 0) && (User_Version == 11)) {
		obj->Base_Scale = readfloat (READER);
	}
}

void
readNiPSysMeshEmitter(NiPSysMeshEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysEmitter));
	readNiPSysEmitter (obj->parent, 0);
	obj->Num_Emitter_Meshes = readuint (READER);
	PROTECT_LEN (obj->Num_Emitter_Meshes, 2000000, "readNiPSysMeshEmitter")
	obj->Emitter_Meshes =
	    GETMEM ((obj->Num_Emitter_Meshes) * sizeof (unsigned int));
	READ (uint, obj->Emitter_Meshes, obj->Num_Emitter_Meshes, SIZEOFDWORD, "readNiPSysMeshEmitter")
	obj->Initial_Velocity_Type = readuint (READER);
	obj->Emission_Type = readuint (READER);
	readVector3 (&obj->Emission_Axis, 0);
}

void
readNiPSysMeshUpdateModifier(NiPSysMeshUpdateModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Num_Meshes = readuint (READER);
	PROTECT_LEN (obj->Num_Meshes, 2000000, "readNiPSysMeshUpdateModifier")
	obj->Meshes = GETMEM ((obj->Num_Meshes) * sizeof (unsigned int));
	READ (uint, obj->Meshes, obj->Num_Meshes, SIZEOFDWORD, "readNiPSysMeshUpdateModifier")
}

void
readNiPSysPlanarCollider(NiPSysPlanarCollider * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysCollider));
	readNiPSysCollider (obj->parent, 0);
	obj->Width = readfloat (READER);
	obj->Height = readfloat (READER);
	readVector3 (&obj->X_Axis, 0);
	readVector3 (&obj->Y_Axis, 0);
}

void
readNiPSysSphericalCollider(NiPSysSphericalCollider * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysCollider));
	readNiPSysCollider (obj->parent, 0);
	obj->Radius = readfloat (READER);
}

void
readNiPSysPositionModifier(NiPSysPositionModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
}

void
readNiPSysResetOnLoopCtlr(NiPSysResetOnLoopCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
}

void
readNiPSysRotationModifier(NiPSysRotationModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Initial_Rotation_Speed = readfloat (READER);
	if (VersionCheck (0x14000004, 0)) {
		obj->Initial_Rotation_Speed_Variation = readfloat (READER);
		obj->Initial_Rotation_Angle = readfloat (READER);
		obj->Initial_Rotation_Angle_Variation = readfloat (READER);
		obj->Random_Rot_Speed_Sign = readbool (READER);
	}
	obj->Random_Initial_Axis = readbool (READER);
	readVector3 (&obj->Initial_Axis, 0);
}

void
readNiPSysSpawnModifier(NiPSysSpawnModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Num_Spawn_Generations = readushort (READER);
	obj->Percentage_Spawned = readfloat (READER);
	obj->Min_Num_to_Spawn = readushort (READER);
	obj->Max_Num_to_Spawn = readushort (READER);
	obj->Spawn_Speed_Chaos = readfloat (READER);
	obj->Spawn_Dir_Chaos = readfloat (READER);
	obj->Life_Span = readfloat (READER);
	obj->Life_Span_Variation = readfloat (READER);
	if (VersionCheck (0x0A040001, 0x0A040001)) {
		obj->Unknown_int = readint (READER);
	}
}

void
readNiPSysSphereEmitter(NiPSysSphereEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysVolumeEmitter));
	readNiPSysVolumeEmitter (obj->parent, 0);
	obj->Radius = readfloat (READER);
}

void
readNiPSysUpdateCtlr(NiPSysUpdateCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
}

void
readNiPSysFieldModifier(NiPSysFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Field_Object = readRef (READER);
	obj->Magnitude = readfloat (READER);
	obj->Attenuation = readfloat (READER);
	obj->Use_Max_Distance = readbool (READER);
	obj->Max_Distance = readfloat (READER);
}

void
readNiPSysVortexFieldModifier(NiPSysVortexFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysFieldModifier));
	readNiPSysFieldModifier (obj->parent, 0);
	readVector3 (&obj->Direction, 0);
}

void
readNiPSysGravityFieldModifier(NiPSysGravityFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysFieldModifier));
	readNiPSysFieldModifier (obj->parent, 0);
	readVector3 (&obj->Direction, 0);
}

void
readNiPSysDragFieldModifier(NiPSysDragFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysFieldModifier));
	readNiPSysFieldModifier (obj->parent, 0);
	obj->Use_Direction_ = readbool (READER);
	readVector3 (&obj->Direction, 0);
}

void
readNiPSysTurbulenceFieldModifier(NiPSysTurbulenceFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysFieldModifier));
	readNiPSysFieldModifier (obj->parent, 0);
	obj->Frequency = readfloat (READER);
}

void
readNiPSysFieldMagnitudeCtlr(NiPSysFieldMagnitudeCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysFieldAttenuationCtlr(NiPSysFieldAttenuationCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysFieldMaxDistanceCtlr(NiPSysFieldMaxDistanceCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysAirFieldAirFrictionCtlr(NiPSysAirFieldAirFrictionCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysAirFieldInheritVelocityCtlr(NiPSysAirFieldInheritVelocityCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysAirFieldSpreadCtlr(NiPSysAirFieldSpreadCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysInitialRotSpeedCtlr(NiPSysInitialRotSpeedCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysInitialRotSpeedVarCtlr(NiPSysInitialRotSpeedVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysInitialRotAngleCtlr(NiPSysInitialRotAngleCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysInitialRotAngleVarCtlr(NiPSysInitialRotAngleVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysEmitterPlanarAngleCtlr(NiPSysEmitterPlanarAngleCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysEmitterPlanarAngleVarCtlr(NiPSysEmitterPlanarAngleVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierFloatCtlr));
	readNiPSysModifierFloatCtlr (obj->parent, 0);
}

void
readNiPSysAirFieldModifier(NiPSysAirFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysFieldModifier));
	readNiPSysFieldModifier (obj->parent, 0);
	readVector3 (&obj->Direction, 0);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);
	obj->Unknown_Boolean_1 = readbool (READER);
	obj->Unknown_Boolean_2 = readbool (READER);
	obj->Unknown_Boolean_3 = readbool (READER);
	obj->Unknown_Float_4 = readfloat (READER);
}

void
readNiPSysTrailEmitter(NiPSysTrailEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysEmitter));
	readNiPSysEmitter (obj->parent, 0);
	obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Float_4 = readfloat (READER);
	obj->Unknown_Int_3 = readint (READER);
	obj->Unknown_Float_5 = readfloat (READER);
	obj->Unknown_Int_4 = readint (READER);
	obj->Unknown_Float_6 = readfloat (READER);
	obj->Unknown_Float_7 = readfloat (READER);
}

void
readNiLightIntensityController(NiLightIntensityController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
}

void
readNiPSysRadialFieldModifier(NiPSysRadialFieldModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysFieldModifier));
	readNiPSysFieldModifier (obj->parent, 0);
	obj->Radial_Type = readint (READER);
}

inline void
readNiLODData(NiLODData * obj, unsigned int ARG)
{
	COVERAGE
}

void
readNiRangeLODData(NiRangeLODData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiLODData));
	readNiLODData (obj->parent, 0);
	readVector3 (&obj->LOD_Center, 0);
	obj->Num_LOD_Levels = readuint (READER);
	PROTECT_LEN (obj->Num_LOD_Levels, 2000000, "readNiRangeLODData")
	// init 1d array
	obj->LOD_Levels = GETMEM ((obj->Num_LOD_Levels) * sizeof (LODRange));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_LOD_Levels); i++)
		readLODRange (&obj->LOD_Levels[i], 0);
}

void
readNiScreenLODData(NiScreenLODData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiLODData));
	readNiLODData (obj->parent, 0);
	readVector3 (&obj->Bound_Center, 0);
	obj->Bound_Radius = readfloat (READER);
	readVector3 (&obj->World_Center, 0);
	obj->World_Radius = readfloat (READER);
	obj->Proportion_Count = readuint (READER);
	PROTECT_LEN (obj->Proportion_Count, 2000000, "readNiScreenLODData")
	obj->Proportion_Levels =
	    GETMEM ((obj->Proportion_Count) * sizeof (float));
	READ (float, obj->Proportion_Levels, obj->Proportion_Count, SIZEOFDWORD, "readNiScreenLODData")
}

void
readNiRotatingParticles(NiRotatingParticles * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticles));
	readNiParticles (obj->parent, 0);
}

void
readNiSequenceStreamHelper(NiSequenceStreamHelper * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiObjectNET));
	readNiObjectNET (obj->parent, 0);
}

void
readNiShadeProperty(NiShadeProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
}

void
readNiSkinData(NiSkinData * obj, unsigned int ARG)
{
	COVERAGE
	readSkinTransform (&obj->Skin_Transform, 0);
	obj->Num_Bones = readuint (READER);
	PROTECT_LEN (obj->Num_Bones, 2000000, "readNiSkinData")
	if (VersionCheck (0x04000002, 0x0A010000)) {
		obj->Skin_Partition = readRef (READER);
	}
	if (VersionCheck (0x04020100, 0)) {
		obj->Has_Vertex_Weights = readbyte (READER);
	}
	// init 1d array
	obj->Bone_List = GETMEM ((obj->Num_Bones) * sizeof (SkinData));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Bones); i++)
		readSkinData (&obj->Bone_List[i], obj->Has_Vertex_Weights);
}

void
readNiSkinInstance(NiSkinInstance * obj, unsigned int ARG)
{
	COVERAGE
	obj->Data = readRef (READER);
	if (VersionCheck (0x0A020000, 0)) {
		obj->Skin_Partition = readRef (READER);
	}
	obj->Skeleton_Root = readPtr (READER);
	obj->Num_Bones = readuint (READER);
	PROTECT_LEN (obj->Num_Bones, 2000000, "readNiSkinInstance")
	obj->Bones = GETMEM ((obj->Num_Bones) * sizeof (unsigned int));
	READ (uint, obj->Bones, obj->Num_Bones, SIZEOFDWORD, "readNiSkinInstance")
}

void
readNiTriShapeSkinController(NiTriShapeSkinController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Num_Bones = readuint (READER);
	PROTECT_LEN (obj->Num_Bones, 2000000, "readNiTriShapeSkinController")
	obj->Vertex_Counts = GETMEM ((obj->Num_Bones) * sizeof (unsigned int));
	READ (uint, obj->Vertex_Counts, obj->Num_Bones, SIZEOFDWORD, "readNiTriShapeSkinController")
	obj->Bones = GETMEM ((obj->Num_Bones) * sizeof (unsigned int));
	READ (uint, obj->Bones, obj->Num_Bones, SIZEOFDWORD, "readNiTriShapeSkinController")
	// init 2d array
	obj->Bone_Data = GETMEM ((obj->Num_Bones) * sizeof (OldSkinData *));
	int i, j;
	for (i = 0; i < (obj->Num_Bones); i++) {
		PROTECT_LEN (obj->Vertex_Counts[i], 2000000, "readNiTriShapeSkinController")
		obj->Bone_Data[i] =
		    GETMEM ((obj->Vertex_Counts[i]) * sizeof (OldSkinData));
		// read 2d array
		for (j = 0; j < (obj->Vertex_Counts[i]); j++)
			readOldSkinData (&obj->Bone_Data[i][j], 0);
	}
}

void
readNiClodSkinInstance(NiClodSkinInstance * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSkinInstance));
	readNiSkinInstance (obj->parent, 0);
}

void
readNiSkinPartition(NiSkinPartition * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Skin_Partition_Blocks = readuint (READER);
	PROTECT_LEN (obj->Num_Skin_Partition_Blocks, 2000000, "readNiSkinPartition")
	// init 1d array
	obj->Skin_Partition_Blocks =
	    GETMEM ((obj->Num_Skin_Partition_Blocks) * sizeof (SkinPartition));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Skin_Partition_Blocks); i++) {
		readSkinPartition (&obj->Skin_Partition_Blocks[i], 0);
	}
}

void
readNiTexture(NiTexture * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiObjectNET));
	readNiObjectNET (obj->parent, 0);
}

void
readNiSourceTexture(NiSourceTexture * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTexture));
	readNiTexture (obj->parent, 0);
	obj->Use_External = readbyte (READER);
	if ((obj->Use_External == 1)) {
		readFilePath (&obj->File_Name, 0);
	}
	if (VersionCheck (0x0A010000, 0) && (obj->Use_External == 1)) {
		obj->Unknown_Link = readRef (READER);
	}
	if (VersionCheck (0, 0x0A000100) && (obj->Use_External == 0)) {
		obj->Unknown_Byte = readbyte (READER);
	}
	if ((obj->Use_External == 0)) {
		obj->Pixel_Data = readRef (READER);
	}
	obj->Pixel_Layout = readuint (READER);
	obj->Use_Mipmaps = readuint (READER);
	obj->Alpha_Format = readuint (READER);
	obj->Is_Static = readbyte (READER);
	if (VersionCheck (0x0A01006A, 0)) {
		obj->Direct_Render = readbool (READER);
	}
	if (VersionCheck (0x14020007, 0)) {
		obj->Persist_Render_Data = readbool (READER);
	}
}

void
readNiSpecularProperty(NiSpecularProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
}

void
readNiSphericalCollider(NiSphericalCollider * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleModifier));
	readNiParticleModifier (obj->parent, 0);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Short_1 = readushort (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	if (VersionCheck (0, 0x04020002)) {
		obj->Unknown_Short_2 = readushort (READER);
	}
	if (VersionCheck (0x04020100, 0)) {
		obj->Unknown_Float_3 = readfloat (READER);
	}
	obj->Unknown_Float_4 = readfloat (READER);
	obj->Unknown_Float_5 = readfloat (READER);
}

void
readNiSpotLight(NiSpotLight * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPointLight));
	readNiPointLight (obj->parent, 0);
	obj->Cutoff_Angle = readfloat (READER);
	if (VersionCheck (0x14020007, 0)) {
		obj->Unknown_Float = readfloat (READER);
	}
	obj->Exponent = readfloat (READER);
}

void
readNiStencilProperty(NiStencilProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	if (VersionCheck (0, 0x0A000102)) {
		obj->Flags = readFlags (READER);
	}
	if (VersionCheck (0, 0x14000005)) {
		obj->Stencil_Enabled = readbyte (READER);
		READ (uint, &(obj->Stencil_Function), 7, SIZEOFDWORD, "readNiStencilProperty")
		/*obj->Stencil_Function = readuint (READER);
		obj->Stencil_Ref = readuint (READER);
		obj->Stencil_Mask = readuint (READER);
		obj->Fail_Action = readuint (READER);
		obj->Z_Fail_Action = readuint (READER);
		obj->Pass_Action = readuint (READER);
		obj->Draw_Mode = readuint (READER);*/
	}
	if (VersionCheck (0x14010003, 0)) {
		obj->Flags = readFlags (READER);
		obj->Stencil_Ref = readuint (READER);
		obj->Stencil_Mask = readuint (READER);
	}
}

void
readNiStringExtraData(NiStringExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	if (VersionCheck (0, 0x04020200)) {
		obj->Bytes_Remaining = readuint (READER);
	}
	readstring (&obj->String_Data, 0);
}

void
readNiStringPalette(NiStringPalette * obj, unsigned int ARG)
{
	COVERAGE
	readStringPalette (&obj->Palette, 0);
}

void
readNiStringsExtraData(NiStringsExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Num_Strings = readuint (READER);
	PROTECT_LEN (obj->Num_Strings, 2000000, "readNiStringsExtraData")
	// init 1d array
	obj->Data = GETMEM ((obj->Num_Strings) * sizeof (string));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Strings); i++)
		readstring (&obj->Data[i], 0);
}

void
readNiTextKeyExtraData(NiTextKeyExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	if (VersionCheck (0, 0x04020200)) {
		obj->Unknown_Int_1 = readuint (READER);
	}
	obj->Num_Text_Keys = readuint (READER);
	PROTECT_LEN (obj->Num_Text_Keys, 2000000, "readNiTextKeyExtraData")
	// init 1d array
	obj->Text_Keys = GETMEM ((obj->Num_Text_Keys) * sizeof (Key));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Text_Keys); i++)
		readKey (&obj->Text_Keys[i], 1, T_STRING);
}

void
readNiTextureEffect(NiTextureEffect * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiDynamicEffect));
	readNiDynamicEffect (obj->parent, 0);
	readMatrix33 (&obj->Model_Projection_Matrix, 0);
	readVector3 (&obj->Model_Projection_Transform, 0);
	obj->Texture_Filtering = readuint (READER);
	obj->Texture_Clamping = readuint (READER);
	if (VersionCheck (0x14060000, 0)) {
		obj->Unknown = readshort (READER);
	}
	obj->Texture_Type = readuint (READER);
	obj->Coordinate_Generation_Type = readuint (READER);
	if (VersionCheck (0, 0x03010000)) {
		obj->Image = readRef (READER);
	}
	if (VersionCheck (0x04000000, 0)) {
		obj->Source_Texture = readRef (READER);
	}
	obj->Clipping_Plane = readbyte (READER);
	readVector3 (&obj->Unknown_Vector, 0);
	obj->Unknown_Float = readfloat (READER);
	if (VersionCheck (0, 0x0A020000)) {
		obj->PS2_L = readshort (READER);
		obj->PS2_K = readshort (READER);
	}
	if (VersionCheck (0, 0x0401000C)) {
		obj->Unknown_Short = readushort (READER);
	}
}

void
readNiTextureModeProperty(NiTextureModeProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Unknown_Short = readshort (READER);
	if (VersionCheck (0x03010000, 0x0A020000)) {
		obj->PS2_L = readshort (READER);
		obj->PS2_K = readshort (READER);
	}
}

void
readNiImage(NiImage * obj, unsigned int ARG)
{
	COVERAGE
	obj->Use_External = readbyte (READER);
	if (obj->Use_External) {
		readFilePath (&obj->File_Name, 0);
	}
	else {// ((obj->Use_External == 0)) {
		obj->Image_Data = readRef (READER);
	}
	obj->Unknown_Int = readuint (READER);
	if (VersionCheck (0x03010000, 0)) {
		obj->Unknown_Float = readfloat (READER);
	}
}

void
readNiTextureProperty(NiTextureProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	obj->Image = readRef (READER);
	if (VersionCheck (0, 0x03030000)) {
		obj->Unknown_Int_1 = readuint (READER);
		obj->Unknown_Int_2 = readuint (READER);
	}
}

void
readNiMultiTextureProperty(NiMultiTextureProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	obj->Unknown_Int = readuint (READER);
	// read 1d array
	int i;
	for (i = 0; i < (5); i++)
		readMultiTextureElement (&obj->Texture_Elements[i], 0);
}

void
readNiTexturingProperty(NiTexturingProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	if (VersionCheck (0, 0x0A000102) ||
		VersionCheck (0x14010003, 0)) {
		obj->Flags = readFlags (READER);
	}
	if (VersionCheck (0, 0x14000005)) {
		obj->Apply_Mode = readuint (READER);
	}
	obj->Texture_Count = readuint (READER);
	obj->Has_Base_Texture = readbool (READER);
	if ((obj->Has_Base_Texture != 0)) {
		readTexDesc (&obj->Base_Texture, 0);
	}
	obj->Has_Dark_Texture = readbool (READER);
	if ((obj->Has_Dark_Texture != 0)) {
		readTexDesc (&obj->Dark_Texture, 0);
	}
	obj->Has_Detail_Texture = readbool (READER);
	if ((obj->Has_Detail_Texture != 0)) {
		readTexDesc (&obj->Detail_Texture, 0);
	}
	obj->Has_Gloss_Texture = readbool (READER);
	if ((obj->Has_Gloss_Texture != 0)) {
		readTexDesc (&obj->Gloss_Texture, 0);
	}
	obj->Has_Glow_Texture = readbool (READER);
	if ((obj->Has_Glow_Texture != 0)) {
		readTexDesc (&obj->Glow_Texture, 0);
	}
	obj->Has_Bump_Map_Texture = readbool (READER);
	if (obj->Has_Bump_Map_Texture) {
		readTexDesc (&obj->Bump_Map_Texture, 0);
		obj->Bump_Map_Luma_Scale = readfloat (READER);
		obj->Bump_Map_Luma_Offset = readfloat (READER);
		readMatrix22 (&obj->Bump_Map_Matrix, 0);
	}
	if (VersionCheck (0x14020007, 0)) {
		obj->Has_Normal_Texture = readbool (READER);
		if (obj->Has_Normal_Texture)
			readTexDesc (&obj->Normal_Texture, 0);
		obj->Has_Unknown2_Texture = readbool (READER);
		if (obj->Has_Unknown2_Texture)
			readTexDesc (&obj->Unknown2_Texture, 0);
	}
	if ((obj->Has_Unknown2_Texture != 0)) {
		obj->Unknown2_Float = readfloat (READER);
	}
	obj->Has_Decal_0_Texture = readbool (READER);
	if ((obj->Has_Decal_0_Texture != 0)) {
		readTexDesc (&obj->Decal_0_Texture, 0);
	}
	if ((VersionCheck (0, 0x14010003) && (obj->Texture_Count >= 8)) ||
	(obj->Texture_Count >= 10 && VersionCheck(0x14020007, 0)) ) {
		obj->Has_Decal_1_Texture = readbool (READER);
	}
	if ((obj->Has_Decal_1_Texture != 0)) {
		readTexDesc (&obj->Decal_1_Texture, 0);
	}
	if ((VersionCheck (0, 0x14010003) && (obj->Texture_Count >= 9)) ||
	(obj->Texture_Count >= 11 && VersionCheck(0x14020007, 0)) ) {
		obj->Has_Decal_2_Texture = readbool (READER);
	}
	if ((obj->Has_Decal_2_Texture != 0)) {
		readTexDesc (&obj->Decal_2_Texture, 0);
	}
	if ((VersionCheck (0, 0x14010003) && (obj->Texture_Count >= 10)) ||
	(obj->Texture_Count >= 12 && VersionCheck(0x14020007, 0)) ) {
		obj->Has_Decal_3_Texture = readbool (READER);
	}
	if ((obj->Has_Decal_3_Texture != 0)) {
		readTexDesc (&obj->Decal_3_Texture, 0);
	}
	if (VersionCheck (0x0A000100, 0)) {
		obj->Num_Shader_Textures = readuint (READER);
		PROTECT_LEN (obj->Num_Shader_Textures, 2000000, "readNiTexturingProperty")
		// init 1d array
		obj->Shader_Textures =
		    GETMEM ((obj->Num_Shader_Textures) * sizeof (ShaderTexDesc));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_Shader_Textures); i++)
			readShaderTexDesc (&obj->Shader_Textures[i], 0);
	}
}

void
readNiTransformData(NiTransformData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiKeyframeData));
	readNiKeyframeData (obj->parent, 0);
}

void
readNiTriShape(NiTriShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeom));
	readNiTriBasedGeom (obj->parent, 0);
}

void
readNiTriShapeData(NiTriShapeData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeomData));
	readNiTriBasedGeomData (obj->parent, 0);
	obj->Num_Triangle_Points = readuint (READER);
	PROTECT_LEN (obj->Num_Triangle_Points, 2000000, "readNiTriShapeData")
	if (VersionCheck (0x0A010000, 0)) {
		obj->Has_Triangles = readbool (READER);
	}
	if (VersionCheck (0, 0x0A000102) ||
		(obj->Has_Triangles != 0 && VersionCheck(0x0A000103, 0))) {
		obj->Triangles =
		    GETMEM ((obj->parent->Num_Triangles) * sizeof (Triangle));
		if (obj->parent->Num_Triangles > 0)
			READ (ushort, &(obj->Triangles[0].v1), obj->parent->Num_Triangles*3, SIZEOFWORD, "readNiTriShapeData")
	}
	if (VersionCheck (0x03010000, 0)) {
		obj->Num_Match_Groups = readushort (READER);
		// init 1d array
		obj->Match_Groups =
		    GETMEM ((obj->Num_Match_Groups) * sizeof (MatchGroup));
		// read 1d array
		int i;
		for (i = 0; i < (obj->Num_Match_Groups); i++)
			readMatchGroup (&obj->Match_Groups[i], 0);
	}
}

void
readNiTriStrips(NiTriStrips * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeom));
	readNiTriBasedGeom (obj->parent, 0);
}

void
readNiTriStripsData(NiTriStripsData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeomData));
	readNiTriBasedGeomData (obj->parent, 0);
	obj->Num_Strips = readushort (READER);
	obj->Strip_Lengths =
	    GETMEM ((obj->Num_Strips) * sizeof (unsigned short));
	READ (ushort, obj->Strip_Lengths, obj->Num_Strips, SIZEOFWORD, "readNiTriStripsData")
	if (VersionCheck (0x0A000103, 0)) {
		obj->Has_Points = readbool (READER);
	}
	if (VersionCheck (0, 0x0A000102) || 
		(VersionCheck (0x0A000103, 0) && obj->Has_Points != 0)) {
		// init 2d array
		obj->Points =
		    GETMEM ((obj->Num_Strips) * sizeof (unsigned short *));
		int i;
		for (i = 0; i < (obj->Num_Strips); i++) {
			obj->Points[i] =
			    GETMEM ((obj->Strip_Lengths[i]) * sizeof (unsigned short));
			READ(ushort, obj->Points[i], obj->Strip_Lengths[i], SIZEOFWORD, "readNiTriStripsData")
		}
	}
}

void
readNiEnvMappedTriShape(NiEnvMappedTriShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiObjectNET));
	readNiObjectNET (obj->parent, 0);
	obj->Unknown_1 = readushort (READER);
	readMatrix44 (&obj->Unknown_Matrix, 0);
	obj->Num_Children = readuint (READER);
	PROTECT_LEN (obj->Num_Children, 2000000, "readNiEnvMappedTriShape")
	obj->Children = GETMEM ((obj->Num_Children) * sizeof (unsigned int));
	READ (uint, obj->Children, obj->Num_Children, SIZEOFDWORD, "readNiEnvMappedTriShape")
	obj->Child_2 = readRef (READER);
	obj->Child_3 = readRef (READER);
}

void
readNiEnvMappedTriShapeData(NiEnvMappedTriShapeData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriShapeData));
	readNiTriShapeData (obj->parent, 0);
}

void
readNiBezierTriangle4(NiBezierTriangle4 * obj, unsigned int ARG)
{
	COVERAGE
	READ (uint, obj->Unknown_1, 6, SIZEOFDWORD, "readNiBezierTriangle4")
	obj->Unknown_2 = readushort (READER);
	readMatrix33 (&obj->Matrix, 0);
	readVector3 (&obj->Vector_1, 0);
	readVector3 (&obj->Vector_2, 0);
	READ (short, obj->Unknown_3, 4, SIZEOFWORD, "readNiBezierTriangle4")
	obj->Unknown_4 = readbyte (READER);
	obj->Unknown_5 = readuint (READER);
	READ (short, obj->Unknown_6, 24, SIZEOFWORD, "readNiBezierTriangle4")
}

void
readNiBezierMesh(NiBezierMesh * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	obj->Num_Bezier_Triangles = readuint (READER);
	PROTECT_LEN (obj->Num_Bezier_Triangles, 2000000, "readNiBezierMesh")
	obj->Bezier_Triangle =
	    GETMEM ((obj->Num_Bezier_Triangles) * sizeof (unsigned int));
	READ (uint, obj->Bezier_Triangle, obj->Num_Bezier_Triangles, SIZEOFDWORD, "readNiBezierMesh")
	obj->Unknown_1 = readuint (READER);
	obj->Count_1 = readushort (READER);
	obj->Unknown_2 = readushort (READER);
	obj->Points_1 = GETMEM ((obj->Count_1) * sizeof (Vector3));
	if (obj->Count_1 > 0)
		READ (float, &(obj->Points_1[0].x), obj->Count_1*3, SIZEOFDWORD, "readNiBezierMesh")
	obj->Unknown_3 = readuint (READER);
	NIFuint len = obj->Count_1 * 2;
	PROTECT_LEN (len, 2000000, "readNiBezierMesh")
	obj->Points_2 = GETMEM (len * sizeof (float));
	READ (float, obj->Points_2, len, SIZEOFDWORD, "readNiBezierMesh")
	obj->Unknown_4 = readuint (READER);
	obj->Count_2 = readushort (READER);
	len = obj->Count_2 * 4;
	PROTECT_LEN (len, 2000000, "readNiBezierMesh")
	obj->Points_2 = GETMEM (len * sizeof (unsigned short));
	READ (ushort, obj->Data_2, len, SIZEOFWORD, "readNiBezierMesh")
}

void
readNiClod(NiClod * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeom));
	readNiTriBasedGeom (obj->parent, 0);
}

void
readNiClodData(NiClodData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeomData));
	readNiTriBasedGeomData (obj->parent, 0);
	obj->Unknown_Shorts = readushort (READER);
	obj->Unknown_Count_1 = readushort (READER);
	obj->Unknown_Count_2 = readushort (READER);
	obj->Unknown_Count_3 = readushort (READER);
	obj->Unknown_Float = readfloat (READER);
	obj->Unknown_Short = readushort (READER);
	NIFuint len = obj->Unknown_Count_1 * 6;
	PROTECT_LEN (len, 2000000, "readNiClodData")
	obj->Unknown_Clod_Shorts_1 = GETMEM (len * sizeof (unsigned short));
	READ (ushort, obj->Unknown_Clod_Shorts_1, len, SIZEOFWORD, "readNiClodData")
	obj->Unknown_Clod_Shorts_2 =
	    GETMEM ((obj->Unknown_Count_2) * sizeof (unsigned short));
	READ (ushort, obj->Unknown_Clod_Shorts_2, obj->Unknown_Count_2, SIZEOFWORD, "readNiClodData")
	len = obj->Unknown_Count_3 * 6;
	PROTECT_LEN (len, 2000000, "readNiClodData")
	obj->Unknown_Clod_Shorts_3 = GETMEM (len * sizeof (unsigned short));
	READ (ushort, obj->Unknown_Clod_Shorts_3, len, SIZEOFWORD, "readNiClodData")
}

void
readNiUVController(NiUVController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Unknown_Short = readushort (READER);
	obj->Data = readRef (READER);
}

void
readNiUVData(NiUVData * obj, unsigned int ARG)
{
	COVERAGE
	int i;
	for (i = 0; i < (4); i++)
		readKeyGroup (&obj->UV_Groups[i], 0, T_FLOAT);
}

void
readNiVectorExtraData(NiVectorExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	readVector3 (&obj->Vector_Data, 0);
	obj->Unknown_Float = readfloat (READER);
}

void
readNiVertexColorProperty(NiVertexColorProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	if (VersionCheck (0, 0x14000005)) {
		obj->Vertex_Mode = readuint (READER);
		obj->Lighting_Mode = readuint (READER);
	}
}

void
readNiVertWeightsExtraData(NiVertWeightsExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Num_Bytes = readuint (READER);
	obj->Num_Vertices = readushort (READER);
	obj->Weight = GETMEM ((obj->Num_Vertices) * sizeof (float));
	READ (float, obj->Weight, obj->Num_Vertices, SIZEOFDWORD, "readNiVertWeightsExtraData")
}

void
readNiVisData(NiVisData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Keys = readuint (READER);
	PROTECT_LEN (obj->Num_Keys, 2000000, "readNiVisData")
	// init 1d array
	obj->Keys = GETMEM ((obj->Num_Keys) * sizeof (Key));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Keys); i++)
		readKey (&obj->Keys[i], 1, T_BYTE);
}

void
readNiWireframeProperty(NiWireframeProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
}

void
readNiZBufferProperty(NiZBufferProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	if (VersionCheck (0x0401000C, 0x14000005)) {
		obj->Function = readuint (READER);
	}
}

void
readRootCollisionNode(RootCollisionNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
}

void
readNiRawImageData(NiRawImageData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Width = readuint (READER);
	PROTECT_LEN (obj->Width, 20000, "readNiRawImageData")
	obj->Height = readuint (READER);
	PROTECT_LEN (obj->Height, 20000, "readNiRawImageData")
	obj->Image_Type = readuint (READER);
	if (obj->Image_Type == 1) {
		NIFuint len = obj->Width * obj->Height;
		PROTECT_LEN (len, 40000000, "readNiRawImageData")
		obj->RGB_Image_Data = GETMEM (len * sizeof (ByteColor3));
		if (len > 0)
			READ (byte, &(obj->RGB_Image_Data[0].r), len*3, 1, "readNiRawImageData")
	}
	else if (obj->Image_Type == 2) {
		NIFuint len = obj->Width * obj->Height;
		PROTECT_LEN (len, 40000000, "readNiRawImageData")
		obj->RGBA_Image_Data = GETMEM (len * sizeof (ByteColor4));
		if (len > 0)
			READ (byte, &(obj->RGBA_Image_Data[0].r), len*4, 1, "readNiRawImageData")
	} else NPFF (ENIF, "readNiRawImageData: unknown Image_Type: %d\n", obj->Image_Type);
}

void
readNiSortAdjustNode(NiSortAdjustNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Sorting_Mode = readuint (READER);
	if (VersionCheck (0, 0x0A020000)) {
		obj->Unknown_Int_2 = readint (READER);
	}
}

void
readNiSourceCubeMap(NiSourceCubeMap * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSourceTexture));
	readNiSourceTexture (obj->parent, 0);
}

void
readNiPhysXProp(NiPhysXProp * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiObjectNET));
	readNiObjectNET (obj->parent, 0);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Int_1 = readuint (READER);
	PROTECT_LEN (obj->Unknown_Int_1, 2000000, "readNiPhysXProp")
	obj->Unknown_Refs_1 =
	    GETMEM ((obj->Unknown_Int_1) * sizeof (unsigned int));
	READ (uint, obj->Unknown_Refs_1, obj->Unknown_Int_1, SIZEOFDWORD, "readNiPhysXProp")
	obj->Num_Dests = readint (READER);
	PROTECT_LEN (obj->Num_Dests, 2000000, "readNiPhysXProp")
	obj->Transform_Dests =
	    GETMEM ((obj->Num_Dests) * sizeof (unsigned int));
	READ (uint, obj->Transform_Dests, obj->Num_Dests, SIZEOFDWORD, "readNiPhysXProp")
	obj->Unknown_Byte = readbyte (READER);
	if (VersionCheck (0x14050000, 0)) {
		obj->Unknown_Int = readint (READER);
	}
	obj->Prop_Description = readRef (READER);
}

void
readphysXMaterialRef(physXMaterialRef * obj, unsigned int ARG)
{
	COVERAGE
	obj->Number = readbyte (READER);
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Material_Desc = readRef (READER);
}

void
readNiPhysXPropDesc(NiPhysXPropDesc * obj, unsigned int ARG)
{
	COVERAGE
	int i;
	obj->Num_Dests = readint (READER);
	PROTECT_LEN (obj->Num_Dests, 2000000, "readNiPhysXPropDesc")
	obj->Actor_Descs = GETMEM ((obj->Num_Dests) * sizeof (unsigned int));
	READ (uint, obj->Actor_Descs, obj->Num_Dests, SIZEOFDWORD, "readNiPhysXPropDesc")
	obj->Num_Joints = readuint (READER);
	PROTECT_LEN (obj->Num_Joints, 2000000, "readNiPhysXPropDesc")
	obj->Joint_Descs = GETMEM ((obj->Num_Joints) * sizeof (unsigned int));
	READ (uint, obj->Joint_Descs, obj->Num_Joints, SIZEOFDWORD, "readNiPhysXPropDesc")
	obj->Unknown_Int_1 = readint (READER);
	obj->Num_Materials = readuint (READER);
	PROTECT_LEN (obj->Num_Materials, 2000000, "readNiPhysXPropDesc")
	// init 1d array
	obj->Material_Descs =
	    GETMEM ((obj->Num_Materials) * sizeof (physXMaterialRef));
	// read 1d array
	for (i = 0; i < (obj->Num_Materials); i++)
		readphysXMaterialRef (&obj->Material_Descs[i], 0);
	obj->Unknown_Int_2 = readuint (READER);
	if (VersionCheck (0x14050000, 0)) {
		obj->Unknown_Int_3 = readuint (READER);
		readstring (&obj->Unknown_String_4, 0);
		obj->Unknown_Int_5 = readuint (READER);
		obj->Unknown_Byte_6 = readbyte (READER);
	}
}

void
readNiPhysXActorDesc(NiPhysXActorDesc * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	readQuaternion (&obj->Unknown_Quat_1, 0);
	readQuaternion (&obj->Unknown_Quat_2, 0);
	readQuaternion (&obj->Unknown_Quat_3, 0);
	obj->Unknown_Ref_0 = readRef (READER);
	obj->Unknown_Int_4 = readfloat (READER);
	obj->Unknown_Int_5 = readint (READER);
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Unknown_Byte_2 = readbyte (READER);
	obj->Unknown_Int_6 = readint (READER);
	PROTECT_LEN (obj->Unknown_Int_6, 2000000, "readNiPhysXActorDesc")
	obj->Shape_Description = readRef (READER);
	obj->Unknown_Ref_1 = readRef (READER);
	obj->Unknown_Ref_2 = readRef (READER);
	obj->Unknown_Refs_3 =
	    GETMEM ((obj->Unknown_Int_6) * sizeof (unsigned int));
	READ (uint, obj->Unknown_Refs_3, obj->Unknown_Int_6, SIZEOFDWORD, "readNiPhysXPropDesc")
}

void
readNiPhysXBodyDesc(NiPhysXBodyDesc * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x14030006, 0)) {
		READ (byte, &(obj->Unknown_Bytes[0]), 136, 1, "readNiPhysXBodyDesc")
	}
}

void
readNiPhysXD6JointDesc(NiPhysXD6JointDesc * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x14030006, 0)) {
		READ (byte, &(obj->Unknown_Bytes[0]), 388, 1, "readNiPhysXD6JointDesc")
	}
}

void
readNiPhysXShapeDesc(NiPhysXShapeDesc * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Int_1 = readint (READER);
	readQuaternion (&obj->Unknown_Quat_1, 0);
	readQuaternion (&obj->Unknown_Quat_2, 0);
	readQuaternion (&obj->Unknown_Quat_3, 0);
	obj->Unknown_Short_1 = readshort (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Short_2 = readshort (READER);
	READ (float, &(obj->Unknown_Float_1), 3, SIZEOFDWORD, "readNiPhysXShapeDesc")
	/*obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);*/
	READ (int, &(obj->Unknown_Int_3), 5, SIZEOFDWORD, "readNiPhysXShapeDesc")
	/*obj->Unknown_Int_3 = readint (READER);
	obj->Unknown_Int_4 = readint (READER);
	obj->Unknown_Int_5 = readint (READER);
	obj->Unknown_Int_7 = readint (READER);
	obj->Unknown_Int_8 = readint (READER);*/
	obj->Mesh_Description = readRef (READER);
}

void
readNiPhysXMeshDesc(NiPhysXMeshDesc * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Short_1 = readshort (READER);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Short_2 = readshort (READER);
	READ (byte, &(obj->Unknown_Bytes_1[0]), 3, 1, "readNiPhysXMeshDesc")
	obj->Unknown_Byte_1 = readbyte (READER);
	READ (byte, &(obj->Unknown_Bytes_1A[0]), 4, 1, "readNiPhysXMeshDesc")
	READ (byte, &(obj->Unknown_Bytes_2[0]), 8, 1, "readNiPhysXMeshDesc")
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Num_Vertices = readint (READER);
	PROTECT_LEN (obj->Num_Vertices, 2000000, "readNiPhysXMeshDesc")
	obj->Unknown_Int_4 = readint (READER);
	obj->Vertices = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
	if (obj->Num_Vertices > 0)
		READ (float, &(obj->Vertices[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiPhysXMeshDesc")
	READ (byte, &(obj->Unknown_Bytes_3[0]), 982, 1, "readNiPhysXMeshDesc")
	READ (short, &(obj->Unknown_Shorts_1[0]), 368, SIZEOFWORD, "readNiPhysXMeshDesc")
	READ (uint, &(obj->Unknown_Ints_1[0]), 3328, SIZEOFDWORD, "readNiPhysXMeshDesc")
	obj->Unknown_Byte_2 = readbyte (READER);
}

void
readNiPhysXMaterialDesc(NiPhysXMaterialDesc * obj, unsigned int ARG)
{
	COVERAGE
	READ (uint, &(obj->Unknown_Int[0]), 12, SIZEOFDWORD, "readNiPhysXMaterialDesc")
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Unknown_Byte_2 = readbyte (READER);
}

void
readNiPhysXKinematicSrc(NiPhysXKinematicSrc * obj, unsigned int ARG)
{
	COVERAGE
	if (VersionCheck (0x14030006, 0)) {
		READ (byte, &(obj->Unknown_Bytes[0]), 6, 1, "readNiPhysXKinematicSrc")
	}
}

void
readNiPhysXTransformDest(NiPhysXTransformDest * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Unknown_Byte_2 = readbyte (READER);
	obj->Node = readPtr (READER);
}

void
readNiArkAnimationExtraData(NiArkAnimationExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	READ (int, &(obj->Unknown_Ints[0]), 4, SIZEOFDWORD, "readNiArkAnimationExtraData")
	if (VersionCheck (0, 0x0401000C)) {
		READ (byte, &(obj->Unknown_Bytes[0]), 37, 1, "readNiArkAnimationExtraData")
	}
}

void
readNiArkImporterExtraData(NiArkImporterExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Unknown_Int_1 = readint (READER);
	if (VersionCheck (0, 0x0401000C)) {
		obj->Unknown_Int_2 = readint (READER);
	}
	readstring (&obj->Importer_Name, 0);
	READ (byte, &(obj->Unknown_Bytes[0]), 13, 1, "readNiArkImporterExtraData")
	READ (float, &(obj->Unknown_Floats[0]), 7, SIZEOFDWORD, "readNiArkImporterExtraData")
}

void
readNiArkTextureExtraData(NiArkTextureExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	READ (int, &(obj->Unknown_Ints_1[0]), 2, SIZEOFDWORD, "readNiArkTextureExtraData")
	obj->Unknown_Byte = readbyte (READER);
	if (VersionCheck (0, 0x0401000C)) {
		obj->Unknown_Int_2 = readint (READER);
	}
	obj->Num_Textures = readint (READER);
	PROTECT_LEN (obj->Num_Textures, 2000000, "readNiArkTextureExtraData")
	// init 1d array
	obj->Textures = GETMEM ((obj->Num_Textures) * sizeof (ArkTexture));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Textures); i++)
		readArkTexture (&obj->Textures[i], 0);
}

void
readNiArkViewportInfoExtraData(NiArkViewportInfoExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	READ (byte, &(obj->Unknown_Bytes[0]), 13, 1, "readNiArkViewportInfoExtraData")
}

void
readNiArkShaderExtraData(NiArkShaderExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Unknown_Int = readint (READER);
	readstring (&obj->Unknown_String, 0);
}

void
readNiLines(NiLines * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriBasedGeom));
	readNiTriBasedGeom (obj->parent, 0);
}

void
readNiLinesData(NiLinesData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiGeometryData));
	readNiGeometryData (obj->parent, 0);
	// init 1d array
	obj->Lines = GETMEM ((obj->parent->Num_Vertices) * sizeof (byte));
	// read 1d array
	int i;
	for (i = 0; i < (obj->parent->Num_Vertices); i++)
		obj->Lines[i] = readbool (READER);
}

void
readPolygon(Polygon * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Vertices = readushort (READER);
	obj->Vertex_Offset = readushort (READER);
	obj->Num_Triangles = readushort (READER);
	obj->Triangle_Offset = readushort (READER);
}

void
readNiScreenElementsData(NiScreenElementsData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriShapeData));
	readNiTriShapeData (obj->parent, 0);
	obj->Max_Polygons = readushort (READER);
	obj->Polygons = GETMEM ((obj->Max_Polygons) * sizeof (Polygon));
	if (obj->Max_Polygons > 0)
		READ (ushort, &(obj->Polygons[0].Num_Vertices), obj->Max_Polygons, SIZEOFWORD, "readNiScreenElementsData")
	obj->Polygon_Indices =
	    GETMEM ((obj->Max_Polygons) * sizeof (unsigned short));
	READ (ushort, obj->Polygon_Indices, obj->Max_Polygons, SIZEOFWORD, "readNiScreenElementsData")
	obj->Unknown_UShort_1 = readushort (READER);
	obj->Num_Polygons = readushort (READER);
	obj->Used_Vertices = readushort (READER);
	obj->Unknown_UShort_2 = readushort (READER);
	obj->Used_Triangle_Points = readushort (READER);
	obj->Unknown_UShort_3 = readushort (READER);
}

void
readNiScreenElements(NiScreenElements * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriShape));
	readNiTriShape (obj->parent, 0);
}

void
readNiRoomGroup(NiRoomGroup * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Shell_Link = readPtr (READER);
	obj->Num_Rooms = readint (READER);
	PROTECT_LEN (obj->Num_Rooms, 2000000, "readNiRoomGroup")
	obj->Rooms = GETMEM ((obj->Num_Rooms) * sizeof (unsigned int));
	READ (uint, obj->Rooms, obj->Num_Rooms, SIZEOFDWORD, "readNiRoomGroup")
}

void
readNiRoom(NiRoom * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Num_Walls = readint (READER);
	PROTECT_LEN (obj->Num_Walls, 2000000, "readNiRoom")
	obj->Wall_Plane = GETMEM ((obj->Num_Walls) * sizeof (Vector4));
	if (obj->Num_Walls > 0)
		READ (float, &(obj->Wall_Plane[0].x), obj->Num_Walls*4, SIZEOFDWORD, "readNiRoom")
	obj->Num_In_Portals = readint (READER);
	PROTECT_LEN (obj->Num_In_Portals, 2000000, "readNiRoom")
	obj->In_Portals =
	    GETMEM ((obj->Num_In_Portals) * sizeof (unsigned int));
	READ (uint, obj->In_Portals, obj->Num_In_Portals, SIZEOFDWORD, "readNiRoom")
	obj->Num_Portals_2 = readint (READER);
	PROTECT_LEN (obj->Num_Portals_2, 2000000, "readNiRoom")
	obj->Portals_2 = GETMEM ((obj->Num_Portals_2) * sizeof (unsigned int));
	READ (uint, obj->Portals_2, obj->Num_Portals_2, SIZEOFDWORD, "readNiRoom")
	obj->Num_Items = readint (READER);
	PROTECT_LEN (obj->Num_Items, 2000000, "readNiRoom")
	obj->Items = GETMEM ((obj->Num_Items) * sizeof (unsigned int));
	READ (uint, obj->Items, obj->Num_Items, SIZEOFDWORD, "readNiRoom")
}

void
readNiPortal(NiPortal * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	obj->Unknown_Flags = readFlags (READER);
	obj->Unknown_Short_1 = readshort (READER);
	obj->Num_Vertices = readushort (READER);
	obj->Vertices = GETMEM ((obj->Num_Vertices) * sizeof (Vector3));
	if (obj->Num_Vertices > 0)
		READ (float, &(obj->Vertices[0].x), obj->Num_Vertices*3, SIZEOFDWORD, "readNiPortal")
	obj->Target = readPtr (READER);
}

void
readBSFadeNode(BSFadeNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
}

void
readBSShaderProperty(BSShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiProperty));
	readNiProperty (obj->parent, 0);
	obj->Flags = readFlags (READER);
	obj->Shader_Type = readuint (READER);
	obj->Shader_Flags = readuint (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Envmap_Scale = readfloat (READER);
}

void
readBSShaderLightingProperty(BSShaderLightingProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
	obj->Unknown_Int_3 = readint (READER);
}

void
readBSShaderNoLightingProperty(BSShaderNoLightingProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderLightingProperty));
	readBSShaderLightingProperty (obj->parent, 0);
	readSizedString (&obj->File_Name, 0);
	if (((User_Version == 11) && (User_Version_2 > 26))) {
		READ (float, &(obj->Unknown_Float_2), 4, SIZEOFDWORD, "readBSShaderNoLightingProperty")
		/*obj->Unknown_Float_2 = readfloat (READER);
		obj->Unknown_Float_3 = readfloat (READER);
		obj->Unknown_Float_4 = readfloat (READER);
		obj->Unknown_Float_5 = readfloat (READER);*/
	}
}

void
readBSShaderPPLightingProperty(BSShaderPPLightingProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderLightingProperty));
	readBSShaderLightingProperty (obj->parent, 0);
	obj->Texture_Set = readRef (READER);
	if (((User_Version == 11) && (User_Version_2 > 14))) {
		obj->Unknown_Float_2 = readfloat (READER);
		obj->Refraction_Period = readint (READER);
		obj->Unknown_Float_4 = readfloat (READER);
		obj->Unknown_Float_5 = readfloat (READER);
	}
}

void
readBSShaderTextureSet(BSShaderTextureSet * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Textures = readint (READER);
	PROTECT_LEN (obj->Num_Textures, 2000000, "readBSShaderTextureSet")
	// init 1d array
	obj->Textures = GETMEM ((obj->Num_Textures) * sizeof (SizedString));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Textures); i++)
		readSizedString (&obj->Textures[i], 0);
}

void
readWaterShaderProperty(WaterShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
}

void
readSkyShaderProperty(SkyShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
	obj->Unknown_Int_4 = readint (READER);
	readSizedString (&obj->File_Name, 0);
	obj->Unknown_Int_5 = readint (READER);
}

void
readTileShaderProperty(TileShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderLightingProperty));
	readBSShaderLightingProperty (obj->parent, 0);
	readSizedString (&obj->File_Name, 0);
}

void
readDistantLODShaderProperty(DistantLODShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
}

void
readBSDistantTreeShaderProperty(BSDistantTreeShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
}

void
readTallGrassShaderProperty(TallGrassShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
	readSizedString (&obj->File_Name, 0);
}

void
readVolumetricFogShaderProperty(VolumetricFogShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
}

void
readHairShaderProperty(HairShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderProperty));
	readBSShaderProperty (obj->parent, 0);
}

void
readLighting30ShaderProperty(Lighting30ShaderProperty * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSShaderPPLightingProperty));
	readBSShaderPPLightingProperty (obj->parent, 0);
}

void
readBSDismemberSkinInstance(BSDismemberSkinInstance * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiSkinInstance));
	readNiSkinInstance (obj->parent, 0);
	obj->Num_Partitions = readint (READER);
	PROTECT_LEN (obj->Num_Partitions, 2000000, "readBSDismemberSkinInstance")
	obj->Partitions =
	    GETMEM ((obj->Num_Partitions) * sizeof (BodyPartList));
	if (obj->Num_Partitions > 0)
		READ (ushort, &(obj->Partitions[0].Part_Flag), obj->Num_Partitions*2, SIZEOFWORD, "readBSDismemberSkinInstance")
}

void
readBSDecalPlacementVectorExtraData(BSDecalPlacementVectorExtraData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Unknown_Float_1 = readfloat (READER);
	obj->Num_Vector_Blocks = readshort (READER);
	// init 1d array
	obj->Vector_Blocks =
	    GETMEM ((obj->Num_Vector_Blocks) * sizeof (DecalVectorArray));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Vector_Blocks); i++)
		readDecalVectorArray (&obj->Vector_Blocks[i], 0);
}

void
readBSPSysSimpleColorModifier(BSPSysSimpleColorModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	READ (float, &(obj->Fade_In_Percent), 6, SIZEOFDWORD, "readBSPSysSimpleColorModifier")
	/*obj->Fade_In_Percent = readfloat (READER);
	obj->Fade_out_Percent = readfloat (READER);
	obj->Color_1_End_Percent = readfloat (READER);
	obj->Color_1_Start_Percent = readfloat (READER);
	obj->Color_2_End_Percent = readfloat (READER);
	obj->Color_2_Start_Percent = readfloat (READER);*/
	// read 1d array
	int i;
	for (i = 0; i < (3); i++)
		readColor4 (&obj->Colors[i], 0);
}

void
readBSValueNode(BSValueNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Value = readint (READER);
	obj->Unknown_byte = readbyte (READER);
}

void
readBSStripParticleSystem(BSStripParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiParticleSystem));
	readNiParticleSystem (obj->parent, 0);
}

void
readBSStripPSysData(BSStripPSysData * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysData));
	readNiPSysData (obj->parent, 0);
	obj->Unknown_Short_1 = readshort (READER);
	obj->Unknown_byte_1 = readbyte (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Int_3 = readint (READER);
}

void
readBSPSysStripUpdateModifier(BSPSysStripUpdateModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifier));
	readNiPSysModifier (obj->parent, 0);
	obj->Update_Delta_Time = readfloat (READER);
}

void
readBSMaterialEmittanceMultController(BSMaterialEmittanceMultController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
}

void
readBSMasterParticleSystem(BSMasterParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Max_Emitter_Objects = readushort (READER);
	obj->Num_Particle_Systems = readint (READER);
	PROTECT_LEN (obj->Num_Particle_Systems, 2000000, "readBSMasterParticleSystem")
	obj->Particle_Systems =
	    GETMEM ((obj->Num_Particle_Systems) * sizeof (unsigned int));
	READ (uint, obj->Particle_Systems, obj->Num_Particle_Systems, SIZEOFDWORD, "readBSMasterParticleSystem")
}

void
readBSPSysMultiTargetEmitterCtlr(BSPSysMultiTargetEmitterCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
	if (VersionCheck (0, 0x0A010000)) {
		obj->Data = readRef (READER);
	}
	if (VersionCheck (0x0A020000, 0)) {
		obj->Visibility_Interpolator = readRef (READER);
	}
	obj->Unknown_Short_1 = readshort (READER);
	obj->Unknown_Int_1 = readint (READER);
}

void
readBSRefractionStrengthController(BSRefractionStrengthController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiFloatInterpController));
	readNiFloatInterpController (obj->parent, 0);
}

void
readBSOrderedNode(BSOrderedNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	readVector4 (&obj->Alpha_Sort_Bound, 0);
	obj->Is_Static_Bound = readbyte (READER);
}

void
readBSBlastNode(BSBlastNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Unknown_Short_2 = readshort (READER);
}

void
readBSDamageStage(BSDamageStage * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Unknown_Short_2 = readshort (READER);
}

void
readBSRefractionFirePeriodController(BSRefractionFirePeriodController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	if (VersionCheck (0x14020007, 0)) {
		obj->Interpolator = readRef (READER);
	}
}

void
readbhkConvexListShape(bhkConvexListShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShape));
	readbhkShape (obj->parent, 0);
	obj->Num_Sub_Shapes = readuint (READER);
	PROTECT_LEN (obj->Num_Sub_Shapes, 2000000, "readbhkConvexListShape")
	obj->Sub_Shapes =
	    GETMEM ((obj->Num_Sub_Shapes) * sizeof (unsigned int));
	READ (uint, obj->Sub_Shapes, obj->Num_Sub_Shapes, SIZEOFDWORD, "readbhkConvexListShape")
	obj->Material = readuint (READER);
	READ (float, obj->Unknown_Floats, 6, SIZEOFDWORD, "readbhkConvexListShape")
	obj->Unknown_Byte_1 = readbyte (READER);
	obj->Unknown_Float_1 = readfloat (READER);
}

void
readBSTreadTransfSubInfo(BSTreadTransfSubInfo * obj, unsigned int ARG)
{
	COVERAGE
	READ (int, &(obj->Unknown_Int_1), 8, SIZEOFDWORD, "readBSTreadTransfSubInfo")
}

void
readBSTreadTransfInfo(BSTreadTransfInfo * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Float_1 = readfloat (READER);
	// read 1d array
	int i;
	for (i = 0; i < (2); i++)
		readBSTreadTransfSubInfo (&obj->Data[i], 0);
}

void
readBSTreadTransfInterpolator(BSTreadTransfInterpolator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpolator));
	readNiInterpolator (obj->parent, 0);
	obj->Num_Transfers = readint (READER);
	PROTECT_LEN (obj->Num_Transfers, 2000000, "readBSTreadTransfInterpolator")
	// init 1d array
	obj->Tread_Transfer_Info =
	    GETMEM ((obj->Num_Transfers) * sizeof (BSTreadTransfInfo));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Transfers); i++)
		readBSTreadTransfInfo (&obj->Tread_Transfer_Info[i], 0);
	obj->Unknown_Int_1 = readint (READER);
}

void
readBSAnimNotes(BSAnimNotes * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Short_1 = readshort (READER);
}

void
readbhkLiquidAction(bhkLiquidAction * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSerializable));
	readbhkSerializable (obj->parent, 0);
	READ (int, &(obj->Unknown_Int_1), 3, SIZEOFDWORD, "readbhkLiquidAction")
	/*obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Int_3 = readint (READER);*/
	READ (float, &(obj->Unknown_Float_1), 4, SIZEOFDWORD, "readbhkLiquidAction")
	/*obj->Unknown_Float_1 = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Unknown_Float_3 = readfloat (READER);
	obj->Unknown_Float_4 = readfloat (READER);*/
}

void
readBSMultiBoundNode(BSMultiBoundNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Multi_Bound = readRef (READER);
}

void
readBSMultiBound(BSMultiBound * obj, unsigned int ARG)
{
	COVERAGE
	obj->Data = readRef (READER);
}

inline void
readBSMultiBoundData(BSMultiBoundData * obj, unsigned int ARG)
{
	COVERAGE
}

void
readBSMultiBoundSphere(BSMultiBoundSphere * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSMultiBoundData));
	readBSMultiBoundData (obj->parent, 0);
	READ (int, &(obj->Unknown_Int_1), 3, SIZEOFDWORD, "readBSMultiBoundSphere")
	/*obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Int_3 = readint (READER);*/
	obj->Radius = readfloat (READER);
}

void
readBSSegmentedTriangle(BSSegmentedTriangle * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Byte_1 = readbyte (READER);
}

void
readBSSegmentedTriShape(BSSegmentedTriShape * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTriShape));
	readNiTriShape (obj->parent, 0);
	obj->Num_Seg_Triangles = readint (READER);
	PROTECT_LEN (obj->Num_Seg_Triangles, 2000000, "readBSSegmentedTriShape")
	// init 1d array
	obj->Seg_Triangles =
	    GETMEM ((obj->Num_Seg_Triangles) * sizeof (BSSegmentedTriangle));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Seg_Triangles); i++)
		readBSSegmentedTriangle (&obj->Seg_Triangles[i], 0);
}

void
readBSMultiBoundAABB(BSMultiBoundAABB * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (BSMultiBoundData));
	readBSMultiBoundData (obj->parent, 0);
	READ (float, &(obj->Unknown_Floats_1[0]), 6, SIZEOFDWORD, "readBSMultiBoundAABB")
}

void
readAdditionalDataInfo(AdditionalDataInfo * obj, unsigned int ARG)
{
	COVERAGE
	READ (int, &(obj->Data_Type), 6, SIZEOFDWORD, "readAdditionalDataInfo")
	/*obj->Data_Type = readint (READER);
	obj->Block_Size = readint (READER);
	obj->Num_Blocks = readint (READER);
	obj->Block_Size_2 = readint (READER);
	obj->Block_Index = readint (READER);
	obj->Unknown_Int_1 = readint (READER);*/
	obj->Unknown_Byte_1 = readbyte (READER);
}

void
readAdditionalDataBlock(AdditionalDataBlock * obj, unsigned int ARG)
{
	COVERAGE
	obj->Has_Data = readbool (READER);
	if (obj->Has_Data) {
		obj->Block_Size = readint (READER);
		PROTECT_LEN (obj->Block_Size, 2000000, "readAdditionalDataBlock")
		obj->Num_Blocks = readint (READER);
		PROTECT_LEN (obj->Num_Blocks, 2000000, "readAdditionalDataBlock")
		obj->Block_Offsets = GETMEM ((obj->Num_Blocks) * sizeof (int));
		READ (int, obj->Block_Offsets, obj->Num_Blocks, SIZEOFDWORD, "readAdditionalDataBlock")
		obj->Num_Data = readint (READER);
		PROTECT_LEN (obj->Num_Data, 2000000, "readAdditionalDataBlock")
		obj->Data_Sizes = GETMEM ((obj->Num_Data) * sizeof (int));
		READ (int, obj->Data_Sizes, obj->Num_Data, SIZEOFDWORD, "readAdditionalDataBlock")
		NIFuint len = obj->Num_Data * obj->Block_Size;
		obj->Data = GETMEM (len);
		READ (byte, obj->Data, len, 1, "readAdditionalDataBlock")
	}
}

void
readNiAdditionalGeometryData(NiAdditionalGeometryData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Vertices = readushort (READER);
	obj->Num_Block_Infos = readuint (READER);
	PROTECT_LEN (obj->Num_Block_Infos, 2000000, "readNiAdditionalGeometryData")
	// init 1d array
	obj->Block_Infos =
	    GETMEM ((obj->Num_Block_Infos) * sizeof (AdditionalDataInfo));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Block_Infos); i++)
		readAdditionalDataInfo (&obj->Block_Infos[i], 0);
	obj->Num_Blocks = readint (READER);
	PROTECT_LEN (obj->Num_Blocks, 2000000, "readNiAdditionalGeometryData")
	// init 1d array
	obj->Blocks = GETMEM ((obj->Num_Blocks) * sizeof (AdditionalDataBlock));
	// read 1d array
	for (i = 0; i < (obj->Num_Blocks); i++)
		readAdditionalDataBlock (&obj->Blocks[i], 0);
}

void
readBSWArray(BSWArray * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiExtraData));
	readNiExtraData (obj->parent, 0);
	obj->Num_Items = readint (READER);
	PROTECT_LEN (obj->Num_Items, 2000000, "readBSWArray")
	obj->Items = GETMEM (obj->Num_Items * SIZEOFDWORD);
	READ (int, obj->Items, obj->Num_Items, SIZEOFDWORD, "readAdditionalDataBlock")
}

void
readbhkAabbPhantom(bhkAabbPhantom * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkShapePhantom));
	readbhkShapePhantom (obj->parent, 0);
	READ (int, &(obj->Unknown_Ints_1[0]), 15, SIZEOFDWORD, "readbhkAabbPhantom")
}

void
readBSFrustumFOVController(BSFrustumFOVController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Interpolator = readRef (READER);
}

void
readBSDebrisNode(BSDebrisNode * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiNode));
	readNiNode (obj->parent, 0);
	obj->Unknown_byte_1 = readbyte (READER);
	obj->Unknown_Short_1 = readshort (READER);
}

void
readbhkBreakableConstraint(bhkBreakableConstraint * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkConstraint));
	readbhkConstraint (obj->parent, 0);
	READ (int, &(obj->Unknown_Ints_1[0]), 41, SIZEOFDWORD, "readbhkBreakableConstraint")
	obj->Unknown_Short_1 = readshort (READER);
}

void
readbhkOrientHingedBodyAction(bhkOrientHingedBodyAction * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (bhkSerializable));
	readbhkSerializable (obj->parent, 0);
	READ (int, &(obj->Unknown_Ints_1[0]), 17, SIZEOFDWORD, "readbhkBreakableConstraint")
}

void
readRegion(Region * obj, unsigned int ARG)
{
	COVERAGE
	obj->Start_Index = readuint (READER);
	obj->Num_Indices = readuint (READER);
}

void
readNiDataStream(NiDataStream * obj, unsigned int ARG)
{
	COVERAGE
	READ (uint, &(obj->Usage), 5, SIZEOFDWORD, "readNiDataStream")
	/*obj->Usage = readuint (READER);
	obj->Access = readuint (READER);
	obj->Num_Bytes = readuint (READER);
	obj->Cloning_Behavior = readuint (READER);
	obj->Num_Regions = readuint (READER);*/
	PROTECT_LEN (obj->Num_Bytes, 2000000, "readNiDataStream")
	PROTECT_LEN (obj->Num_Regions, 2000000, "readNiDataStream")
	obj->Regions = GETMEM ((obj->Num_Regions) * sizeof (Region));
	if (obj->Num_Regions > 0)
		READ (uint, &(obj->Regions[0].Start_Index), obj->Num_Regions*2, SIZEOFDWORD, "readNiDataStream")
	obj->Num_Components = readuint (READER);
	PROTECT_LEN (obj->Num_Components, 2000000, "readNiDataStream")
	obj->Component_Formats =
	    GETMEM ((obj->Num_Components) * sizeof (unsigned int));
	READ (uint, obj->Component_Formats, obj->Num_Components, SIZEOFDWORD, "readNiDataStream")
	obj->Data = GETMEM ((obj->Num_Bytes) * sizeof (byte));
	READ (byte, obj->Data, obj->Num_Bytes, 1, "readNiDataStream")
	obj->Streamable = readbool (READER);
}

void
readSemanticData(SemanticData * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	obj->Index = readuint (READER);
}

void
readMeshData(MeshData * obj, unsigned int ARG)
{
	COVERAGE
	obj->Stream = readRef (READER);
	obj->Is_Per_Instance = readbool (READER);
	obj->Num_Submeshes = readushort (READER);
	obj->Submesh_To_Region_Map =
	    GETMEM ((obj->Num_Submeshes) * sizeof (unsigned short));
	READ (ushort, obj->Submesh_To_Region_Map, obj->Num_Submeshes, SIZEOFWORD, "readMeshData")
	obj->Num_Components = readint (READER);
	PROTECT_LEN (obj->Num_Components, 2000000, "readMeshData")
	// init 1d array
	obj->Component_Semantics =
	    GETMEM ((obj->Num_Components) * sizeof (SemanticData));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Components); i++)
		readSemanticData (&obj->Component_Semantics[i], 0);
}

void
readMaterialData(MaterialData * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Material_Name, 0);
	obj->Material_Extra_Data = readuint (READER);
}

void
readNiRenderObject(NiRenderObject * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	obj->Num_Materials = readuint (READER);
	PROTECT_LEN (obj->Num_Materials, 2000000, "readNiRenderObject")
	// init 1d array
	obj->Material_Data =
	    GETMEM ((obj->Num_Materials) * sizeof (MaterialData));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Materials); i++)
		readMaterialData (&obj->Material_Data[i], 0);
	obj->Active_Material = readint (READER);
	obj->Material_Needs_Update_Default = readbool (READER);
}

void
readNiMeshModifier(NiMeshModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->Num_Submit_Points = readuint (READER);
	PROTECT_LEN (obj->Num_Submit_Points, 2000000, "readNiMeshModifier")
	obj->Submit_Points =
	    GETMEM ((obj->Num_Submit_Points) * sizeof (unsigned short));
	READ (ushort, obj->Submit_Points, obj->Num_Submit_Points, SIZEOFWORD, "readNiMeshModifier")
	obj->Num_Complete_Points = readuint (READER);
	PROTECT_LEN (obj->Num_Complete_Points, 2000000, "readNiMeshModifier")
	obj->Complete_Points =
	    GETMEM ((obj->Num_Complete_Points) * sizeof (unsigned short));
	READ (ushort, obj->Complete_Points, obj->Num_Complete_Points, SIZEOFWORD, "readNiMeshModifier")
}

void
readNiMesh(NiMesh * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiRenderObject));
	readNiRenderObject (obj->parent, 0);
	obj->Primitive_Type = readuint (READER);
	obj->Num_Submeshes = readushort (READER);
	obj->Instancing_Enabled = readbool (READER);
	readSphereBV (&obj->Bound, 0);
	obj->Num_Datas = readuint (READER);
	PROTECT_LEN (obj->Num_Datas, 2000000, "readNiMesh")
	// init 1d array
	obj->Datas = GETMEM ((obj->Num_Datas) * sizeof (MeshData));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Datas); i++)
		readMeshData (&obj->Datas[i], 0);
	obj->Num_Modifiers = readuint (READER);
	PROTECT_LEN (obj->Num_Modifiers, 2000000, "readNiMesh")
	obj->Modifiers = GETMEM ((obj->Num_Modifiers) * sizeof (unsigned int));
	READ (uint, obj->Modifiers, obj->Num_Modifiers, SIZEOFDWORD, "readNiMesh")
}

void
readNiMorphWeightsController(NiMorphWeightsController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiInterpController));
	readNiInterpController (obj->parent, 0);
	obj->Unknown_2 = readint (READER);
	obj->Num_Interpolators = readuint (READER);
	PROTECT_LEN (obj->Num_Interpolators, 2000000, "readNiMorphWeightsController")
	obj->Interpolators =
	    GETMEM ((obj->Num_Interpolators) * sizeof (unsigned int));
	READ (uint, obj->Interpolators, obj->Num_Interpolators, SIZEOFDWORD, "readNiMorphWeightsController")
	obj->Num_Targets = readuint (READER);
	PROTECT_LEN (obj->Num_Targets, 2000000, "readNiMorphWeightsController")
	// init 1d array
	obj->Target_Names = GETMEM ((obj->Num_Targets) * sizeof (string));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Targets); i++)
		readstring (&obj->Target_Names[i], 0);
}

void
readElementReference(ElementReference * obj, unsigned int ARG)
{
	COVERAGE
	readSemanticData (&obj->Semantic, 0);
	obj->Normalize_Flag = readuint (READER);
}

void
readNiMorphMeshModifier(NiMorphMeshModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiMeshModifier));
	readNiMeshModifier (obj->parent, 0);
	obj->Flags = readbyte (READER);
	obj->Num_Targets = readushort (READER);
	obj->Num_Elements = readuint (READER);
	PROTECT_LEN (obj->Num_Elements, 2000000, "readNiMorphMeshModifier")
	// init 1d array
	obj->Elements =
	    GETMEM ((obj->Num_Elements) * sizeof (ElementReference));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Elements); i++)
		readElementReference (&obj->Elements[i], 0);
}

void
readNiSkinningMeshModifier(NiSkinningMeshModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiMeshModifier));
	readNiMeshModifier (obj->parent, 0);
	obj->Flags = readushort (READER);
	obj->Skeleton_Root = readPtr (READER);
	readSkinTransform (&obj->Skeleton_Transform, 0);
	obj->Num_Bones = readuint (READER);
	PROTECT_LEN (obj->Num_Bones, 2000000, "readNiSkinningMeshModifier")
	obj->Bones = GETMEM ((obj->Num_Bones) * sizeof (unsigned int));
	READ (uint, obj->Bones, obj->Num_Bones, SIZEOFDWORD, 	"readNiSkinningMeshModifier")
	// init 1d array
	obj->Bone_Transforms =
	    GETMEM ((obj->Num_Bones) * sizeof (SkinTransform));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Bones); i++)
		readSkinTransform (&obj->Bone_Transforms[i], 0);
	if ((obj->Flags & 2)) {
		// init 1d array
		obj->Bone_Bounds =
		    GETMEM ((obj->Num_Bones) * sizeof (SphereBV));
		// read 1d array
		for (i = 0; i < (obj->Num_Bones); i++)
			readSphereBV (&obj->Bone_Bounds[i], 0);
	}
}

void
readNiInstancingMeshModifier(NiInstancingMeshModifier * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiMeshModifier));
	readNiMeshModifier (obj->parent, 0);
}

void
readNiSkinningLODController(NiSkinningLODController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
}

void
readNiPSParticleSystem(NiPSParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiAVObject));
	readNiAVObject (obj->parent, 0);
	obj->Unknown_3 = readint (READER);
	PROTECT_LEN (obj->Unknown_3, 2000000, "readNiPSParticleSystem")
	obj->Unknown_38 = GETMEM ((obj->Unknown_3) * sizeof (int));
	READ (int, obj->Unknown_38, obj->Unknown_3, SIZEOFDWORD, 	"readNiPSParticleSystem")
	obj->Unknown_4 = readint (READER);
	obj->Unknown_5 = readint (READER);
	obj->Unknown_39 = GETMEM ((obj->Unknown_3) * sizeof (int));
	READ (int, obj->Unknown_39, obj->Unknown_3, SIZEOFDWORD, 	"readNiPSParticleSystem")
	READ (int, &(obj->Unknown_6), 4, SIZEOFDWORD, "readNiPSParticleSystem")
	/*obj->Unknown_6 = readint (READER);
	obj->Unknown_7 = readint (READER);
	obj->Unknown_8 = readint (READER);
	obj->Unknown_9 = readint (READER);*/
	obj->Unknown_10 = readfloat (READER);
	READ (int, &(obj->Unknown_11), 2, SIZEOFDWORD, "readNiPSParticleSystem")
	/*obj->Unknown_11 = readint (READER);
	obj->Unknown_12 = readint (READER);*/
	obj->Simulator = readRef (READER);
	if ((obj->Unknown_12 > 1)) {
		obj->Generator = readRef (READER);
	}
	READ (int, &(obj->Unknown_15), 3, SIZEOFDWORD, "readNiPSParticleSystem")
	/*obj->Unknown_15 = readint (READER);
	obj->Unknown_16 = readint (READER);
	obj->Unknown_17 = readint (READER);*/
	obj->Emitter = readRef (READER);
	READ (int, &(obj->Unknown_19), 3, SIZEOFDWORD, "readNiPSParticleSystem")
	/*obj->Unknown_19 = readint (READER);
	obj->Unknown_20 = readint (READER);
	obj->Unknown_21 = readint (READER);*/
	READ (byte, &(obj->Unknown_22[0]), 4, 1, "readNiPSParticleSystem")
	if (VersionCheck (0x1E000002, 0)) {
		READ (int, &(obj->Unknown_27), 8, SIZEOFDWORD, "readNiPSParticleSystem")
		/*obj->Unknown_27 = readint (READER);
		obj->Unknown_28 = readint (READER);
		obj->Unknown_29 = readint (READER);
		obj->Unknown_30 = readint (READER);
		obj->Unknown_31 = readint (READER);
		obj->Unknown_32 = readint (READER);
		obj->Unknown_33 = readint (READER);
		obj->Unknown_34 = readint (READER);*/
		obj->Unknown_35 = readbyte (READER);
		obj->Unknown_36 = readint (READER);
		obj->Unknown_37 = readshort (READER);
	}
}

void
readNiPSMeshParticleSystem(NiPSMeshParticleSystem * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSParticleSystem));
	readNiPSParticleSystem (obj->parent, 0);
	READ (int, &(obj->Unknown_23), 3, SIZEOFDWORD, "readNiPSMeshParticleSystem")
	/*obj->Unknown_23 = readint (READER);
	obj->Unknown_24 = readint (READER);
	obj->Unknown_25 = readint (READER);*/
	obj->Unknown_26 = readbyte (READER);
}

void
readNiPSEmitParticlesCtlr(NiPSEmitParticlesCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysEmitterCtlr));
	readNiPSysEmitterCtlr (obj->parent, 0);
}

void
readNiPSForceActiveCtlr(NiPSForceActiveCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Interpolator = readRef (READER);
	obj->Unknown_2 = readint (READER);
}

void
readNiPSSimulator(NiPSSimulator * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiMeshModifier));
	readNiMeshModifier (obj->parent, 0);
	obj->Num_Simulation_Steps = readuint (READER);
	PROTECT_LEN (obj->Num_Simulation_Steps, 2000000, "readNiPSSimulator")
	obj->Simulation_Steps =
	    GETMEM ((obj->Num_Simulation_Steps) * sizeof (unsigned int));
	READ (uint, obj->Simulation_Steps, obj->Num_Simulation_Steps, SIZEOFDWORD, "readNiPSSimulator")
}

inline void
readNiPSSimulatorStep(NiPSSimulatorStep * obj, unsigned int ARG)
{
	COVERAGE
}

void
readNiPSSimulatorGeneralStep(NiPSSimulatorGeneralStep * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSSimulatorStep));
	readNiPSSimulatorStep (obj->parent, 0);
	obj->Num_Size_Keys = readbyte (READER);
	// init 1d array
	obj->Size_Keys = GETMEM ((obj->Num_Size_Keys) * sizeof (Key));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Size_Keys); i++)
		readKey (&obj->Size_Keys[i], 1, T_FLOAT);
	if (VersionCheck (0x1E000002, 0)) {
		obj->Size_Loop_Behavior = readuint (READER);
	}
	if (VersionCheck (0, 0x14060000)) {
		READ (float, &(obj->Unknown_1), 3, SIZEOFDWORD, "readNiPSSimulatorGeneralStep")
		/*obj->Unknown_1 = readfloat (READER);
		obj->Unknown_2 = readfloat (READER);
		obj->Unknown_3 = readfloat (READER);*/
	}
	if (VersionCheck (0x1E000002, 0)) {
		obj->Num_Color_Keys = readbyte (READER);
		// init 1d array
		obj->Color_Keys = GETMEM ((obj->Num_Color_Keys) * sizeof (Key));
		// read 1d array
		for (i = 0; i < (obj->Num_Color_Keys); i++)
			readKey (&obj->Color_Keys[i], 1, T_BYTECOLOR4);
		obj->Color_Loop_Behavior = readuint (READER);
		obj->Num_Rotation_Keys = readbyte (READER);
		// init 1d array
		obj->Rotation_Keys =
		    GETMEM ((obj->Num_Rotation_Keys) * sizeof (QuatKey));
		// read 1d array
		for (i = 0; i < (obj->Num_Rotation_Keys); i++)
			readQuatKey (&obj->Rotation_Keys[i], 1);
		obj->Rotation_Loop_Behavior = readuint (READER);
		obj->Grow_Time = readfloat (READER);
		obj->Shrink_Time = readfloat (READER);
		obj->Grow_Generation = readushort (READER);
		obj->Shrink_Generation = readushort (READER);
	}
}

void
readNiPSSimulatorForcesStep(NiPSSimulatorForcesStep * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSSimulatorStep));
	readNiPSSimulatorStep (obj->parent, 0);
	obj->Num_Forces = readuint (READER);
	PROTECT_LEN (obj->Num_Forces, 2000000, "readNiPSSimulatorForcesStep")
	obj->Forces = GETMEM ((obj->Num_Forces) * sizeof (unsigned int));
	READ (uint, obj->Forces, obj->Num_Forces, SIZEOFDWORD, "readNiPSSimulatorForcesStep")
}

void
readNiPSSimulatorCollidersStep(NiPSSimulatorCollidersStep * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSSimulatorStep));
	readNiPSSimulatorStep (obj->parent, 0);
	obj->Num_Colliders = readuint (READER);
	PROTECT_LEN (obj->Num_Colliders, 2000000, "readNiPSSimulatorCollidersStep")
	obj->Colliders = GETMEM ((obj->Num_Colliders) * sizeof (unsigned int));
	READ (uint, obj->Colliders, obj->Num_Colliders, SIZEOFDWORD, "readNiPSSimulatorCollidersStep")
}

void
readNiPSSimulatorMeshAlignStep (NiPSSimulatorMeshAlignStep * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSSimulatorStep));
	readNiPSSimulatorStep (obj->parent, 0);
	obj->Num_Rotation_Keys = readbyte (READER);
	// init 1d array
	obj->Rotation_Keys =
	    GETMEM ((obj->Num_Rotation_Keys) * sizeof (QuatKey));
	// read 1d array
	int i;
	for (i = 0; i < (obj->Num_Rotation_Keys); i++)
		readQuatKey (&obj->Rotation_Keys[i], 1);
	obj->Rotation_Loop_Behavior = readuint (READER);
}

void
readNiPSSimulatorFinalStep(NiPSSimulatorFinalStep * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSSimulatorStep));
	readNiPSSimulatorStep (obj->parent, 0);
}

void
readNiPSFacingQuadGenerator(NiPSFacingQuadGenerator * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1), 12, 1, "readNiPSFacingQuadGenerator")
}

void
readNiShadowGenerator(NiShadowGenerator * obj, unsigned int ARG)
{
 	COVERAGE
	readstring (&obj->Name, 0);
	obj->Unknown_Flags = readushort (READER);
	obj->Num_Unknown_Links_1 = readuint (READER);
	PROTECT_LEN (obj->Num_Unknown_Links_1, 2000000, "readNiShadowGenerator")
	obj->Unknown_Links_1 =
	    GETMEM ((obj->Num_Unknown_Links_1) * sizeof (unsigned int));
	READ (uint, obj->Unknown_Links_1, obj->Num_Unknown_Links_1, SIZEOFDWORD, "readNiShadowGenerator")
	obj->Unkown_Int_2 = readint (READER);
	obj->Target = readPtr (READER);
	obj->Unkown_Float_4 = readfloat (READER);
	obj->Unkown_Byte_5 = readbyte (READER);
	READ (int, &(obj->Unkown_Int_6), 3, SIZEOFDWORD, "readNiShadowGenerator")
	/*obj->Unkown_Int_6 = readint (READER);
	obj->Unkown_Int_7 = readint (READER);
	obj->Unkown_Int_8 = readint (READER);*/
	obj->Unkown_Byte_9 = readbyte (READER);
}

void
readNiPSBoundUpdater(NiPSBoundUpdater * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1), 2, 1, "readNiPSBoundUpdater")
}

void
readNiPSDragForce(NiPSDragForce * obj, unsigned int ARG)
{
	COVERAGE
	READ (int, &(obj->Unknown_1), 2, SIZEOFDWORD, "readNiPSDragForce")
	/*obj->Unknown_1 = readint (READER);
	obj->Unknown_2 = readint (READER);*/
	obj->Unknown_3 = readbyte (READER);
	READ (float, &(obj->Unknown_4), 6, SIZEOFDWORD, "readNiPSDragForce")
	/*obj->Unknown_4 = readfloat (READER);
	obj->Unknown_5 = readfloat (READER);
	obj->Unknown_6 = readfloat (READER);
	obj->Unknown_7 = readfloat (READER);
	obj->Unknown_8 = readfloat (READER);
	obj->Unknown_9 = readfloat (READER);*/
	obj->Unknown_10 = readint (READER);
}

void
readNiPSGravityForce(NiPSGravityForce * obj, unsigned int ARG)
{
	COVERAGE
	READ (byte, &(obj->Unknown_1), 17, 1, "readNiPSGravityForce")
	/*obj->Unknown_1 = readbyte (READER);
	obj->Unknown_2 = readbyte (READER);
	obj->Unknown_3 = readbyte (READER);
	obj->Unknown_4 = readbyte (READER);
	obj->Unknown_5 = readbyte (READER);
	obj->Unknown_6 = readbyte (READER);
	obj->Unknown_7 = readbyte (READER);
	obj->Unknown_8 = readbyte (READER);
	obj->Unknown_9 = readbyte (READER);
	obj->Unknown_10 = readbyte (READER);
	obj->Unknown_11 = readbyte (READER);
	obj->Unknown_12 = readbyte (READER);
	obj->Unknown_13 = readbyte (READER);
	obj->Unknown_14 = readbyte (READER);
	obj->Unknown_15 = readbyte (READER);
	obj->Unknown_16 = readbyte (READER);
	obj->Unknown_17 = readbyte (READER);*/
	obj->Unknown_18 = readfloat (READER);
	READ (byte, &(obj->Unknown_19), 16, 1, "readNiPSGravityForce")
	/*obj->Unknown_19 = readbyte (READER);
	obj->Unknown_20 = readbyte (READER);
	obj->Unknown_21 = readbyte (READER);
	obj->Unknown_22 = readbyte (READER);
	obj->Unknown_23 = readbyte (READER);
	obj->Unknown_24 = readbyte (READER);
	obj->Unknown_25 = readbyte (READER);
	obj->Unknown_26 = readbyte (READER);
	obj->Unknown_27 = readbyte (READER);
	obj->Unknown_28 = readbyte (READER);
	obj->Unknown_29 = readbyte (READER);
	obj->Unknown_30 = readbyte (READER);
	obj->Unknown_31 = readbyte (READER);
	obj->Unknown_32 = readbyte (READER);
	obj->Unknown_33 = readbyte (READER);
	obj->Unknown_34 = readbyte (READER);*/
	obj->Unknown_35 = readfloat (READER);
	obj->Unknown_36 = readRef (READER);
}

void
readNiPSBoxEmitter(NiPSBoxEmitter * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	READ (float, &(obj->Unknown_1), 2, SIZEOFDWORD, "readNiPSBoxEmitter")
	/*obj->Unknown_1 = readfloat (READER);
	obj->Unknown_2 = readfloat (READER);*/
	READ (byte, &(obj->Unknown_3), 4, 1, "readNiPSBoxEmitter")
	/*obj->Unknown_3 = readbyte (READER);
	obj->Unknown_4 = readbyte (READER);
	obj->Unknown_5 = readbyte (READER);
	obj->Unknown_6 = readbyte (READER);*/
	obj->Unknown_7 = readfloat (READER);
	READ (byte, &(obj->Unknown_8), 4, 1, "readNiPSBoxEmitter")
	/*obj->Unknown_8 = readbyte (READER);
	obj->Unknown_9 = readbyte (READER);
	obj->Unknown_10 = readbyte (READER);
	obj->Unknown_11 = readbyte (READER);*/
	obj->Unknown_12 = readfloat (READER);
	obj->Unknown_13 = readint (READER);
	READ (float, &(obj->Unknown_14), 9, SIZEOFDWORD, "readNiPSBoxEmitter")
	/*obj->Unknown_14 = readfloat (READER);
	obj->Unknown_15 = readfloat (READER);
	obj->Unknown_16 = readfloat (READER);
	obj->Unknown_17 = readfloat (READER);
	obj->Unknown_18 = readfloat (READER);
	obj->Unknown_19 = readfloat (READER);
	obj->Unknown_20 = readfloat (READER);
	obj->Unknown_21 = readfloat (READER);
	obj->Unknown_22 = readfloat (READER);*/
	READ (byte, &(obj->Unknown_23), 26, 1, "readNiPSBoxEmitter")
	/*obj->Unknown_23 = readbyte (READER);
	obj->Unknown_24 = readbyte (READER);
	obj->Unknown_25 = readbyte (READER);
	obj->Unknown_26 = readbyte (READER);
	obj->Unknown_27 = readbyte (READER);
	obj->Unknown_28 = readbyte (READER);
	obj->Unknown_29 = readbyte (READER);
	obj->Unknown_30 = readbyte (READER);
	obj->Unknown_31 = readbyte (READER);
	obj->Unknown_32 = readbyte (READER);
	obj->Unknown_33 = readbyte (READER);
	obj->Unknown_34 = readbyte (READER);
	obj->Unknown_35 = readbyte (READER);
	obj->Unknown_36 = readbyte (READER);
	obj->Unknown_37 = readbyte (READER);
	obj->Unknown_38 = readbyte (READER);
	obj->Unknown_39 = readbyte (READER);
	obj->Unknown_40 = readbyte (READER);
	obj->Unknown_41 = readbyte (READER);
	obj->Unknown_42 = readbyte (READER);
	obj->Unknown_43 = readbyte (READER);
	obj->Unknown_44 = readbyte (READER);
	obj->Unknown_45 = readbyte (READER);
	obj->Unknown_46 = readbyte (READER);
	obj->Unknown_47 = readbyte (READER);
	obj->Unknown_48 = readbyte (READER);*/
}

void
readNiPSMeshEmitter(NiPSMeshEmitter * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	READ (int, &(obj->Unknown_1), 3, SIZEOFDWORD, "readNiPSMeshEmitter")
	/*obj->Unknown_1 = readint (READER);
	obj->Unknown_2 = readint (READER);
	obj->Unknown_3 = readint (READER);*/
	if (VersionCheck (0x1E000002, 0)) {
		obj->Unknown_27 = readint (READER);
	}
	READ (float, &(obj->Unknown_4), 3, SIZEOFDWORD, "readNiPSMeshEmitter")
	/*obj->Unknown_4 = readfloat (READER);
	obj->Unknown_5 = readfloat (READER);
	obj->Unknown_6 = readfloat (READER);*/
	if (VersionCheck (0x1E000002, 0)) {
		obj->Unknown_28 = readfloat (READER);
	}
	obj->Unknown_7 = readint (READER);
	READ (float, &(obj->Unknown_8), 5, SIZEOFDWORD, "readNiPSMeshEmitter")
	/*obj->Unknown_8 = readfloat (READER);
	obj->Unknown_9 = readfloat (READER);
	obj->Unknown_10 = readfloat (READER);
	obj->Unknown_11 = readfloat (READER);
	obj->Unknown_12 = readfloat (READER);*/
	obj->Unknown_13 = readint (READER);
	READ (float, &(obj->Unknown_14), 3, SIZEOFDWORD, "readNiPSMeshEmitter")
	/*obj->Unknown_14 = readfloat (READER);
	obj->Unknown_15 = readfloat (READER);
	obj->Unknown_16 = readfloat (READER);*/
	if (VersionCheck (0, 0x14060000)) {
		READ (int, &(obj->Unknown_17), 2, SIZEOFDWORD, "readNiPSMeshEmitter")
		/*obj->Unknown_17 = readint (READER);
		obj->Unknown_18 = readint (READER);*/
	}
	obj->Unknown_19 = readshort (READER);
	READ (int, &(obj->Unknown_20), 2, SIZEOFDWORD, "readNiPSMeshEmitter")
	/*obj->Unknown_20 = readint (READER);
	obj->Unknown_21 = readint (READER);*/
	if (VersionCheck (0, 0x14060000)) {
		obj->Unknown_22 = readfloat (READER);
		obj->Unknown_23 = readint (READER);
	}
	READ (int, &(obj->Unknown_24), 3, SIZEOFDWORD, "readNiPSMeshEmitter")
	/*obj->Unknown_24 = readint (READER);
	obj->Unknown_25 = readint (READER);
	obj->Unknown_26 = readint (READER);*/
}

void
readNiPSGravityStrengthCtlr(NiPSGravityStrengthCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Unknown_2 = readint (READER);
	obj->Unknown_3 = readint (READER);
}

void
readNiPSPlanarCollider(NiPSPlanarCollider * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	obj->Unknown_Int_1 = readint (READER);
	obj->Unknown_Int_2 = readint (READER);
	obj->Unknown_Short_3 = readshort (READER);
	obj->Unknown_Byte_4 = readbyte (READER);
	READ (float, obj->Unknown_Floats_5, 8, SIZEOFDWORD, "readNiPSPlanarCollider")
	obj->Unknown_Link_6 = readRef (READER);
}

void
readNiPSEmitterSpeedCtlr(NiPSEmitterSpeedCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Interpolator = readRef (READER);
	obj->Unknown_3 = readint (READER);
}

void
readNiPSEmitterRadiusCtlr(NiPSEmitterRadiusCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Interpolator = readRef (READER);
	obj->Unknown_2 = readint (READER);
}

void
readNiPSResetOnLoopCtlr(NiPSResetOnLoopCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
}

void
readNiPSSphereEmitter(NiPSSphereEmitter * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	READ (int, &(obj->Unknown_2), 4, SIZEOFDWORD, "readNiPSSphereEmitter")
	/*obj->Unknown_2 = readint (READER);
	obj->Unknown_3 = readint (READER);
	obj->Unknown_4 = readint (READER);
	obj->Unknown_5 = readint (READER);*/
	obj->Unknown_6 = readfloat (READER);
	obj->Unknown_7 = readint (READER);
	READ (float, &(obj->Unknown_8), 2, SIZEOFDWORD, "readNiPSSphereEmitter")
	obj->Unknown_8 = readfloat (READER);
	obj->Unknown_9 = readfloat (READER);
	obj->Unknown_10 = readint (READER);
	obj->Unknown_11 = readfloat (READER);
	READ (int, &(obj->Unknown_12), 5, SIZEOFDWORD, "readNiPSSphereEmitter")
	/*obj->Unknown_12 = readint (READER);
	obj->Unknown_13 = readint (READER);
	obj->Unknown_14 = readint (READER);
	obj->Unknown_15 = readint (READER);
	obj->Unknown_16 = readint (READER);*/
	obj->Unknown_17 = readfloat (READER);
	READ (int, &(obj->Unknown_18), 2, SIZEOFDWORD, "readNiPSSphereEmitter")
	/*obj->Unknown_18 = readint (READER);
	obj->Unknown_19 = readint (READER);*/
	obj->Unknown_20 = readshort (READER);
	obj->Unknown_21 = readint (READER);
	obj->Unknown_22 = readfloat (READER);
}

void
readNiPSCylinderEmitter(NiPSCylinderEmitter * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSSphereEmitter));
	readNiPSSphereEmitter (obj->parent, 0);
	obj->Unknown_23 = readfloat (READER);
}

void
readNiPSEmitterDeclinationCtlr(NiPSEmitterDeclinationCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
}

void
readNiPSEmitterDeclinationVarCtlr(NiPSEmitterDeclinationVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSEmitterDeclinationCtlr));
	readNiPSEmitterDeclinationCtlr (obj->parent, 0);
}

void
readNiPSEmitterPlanarAngleCtlr(NiPSEmitterPlanarAngleCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
}

void
readNiPSEmitterPlanarAngleVarCtlr(NiPSEmitterPlanarAngleVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSEmitterPlanarAngleCtlr));
	readNiPSEmitterPlanarAngleCtlr (obj->parent, 0);
}

void
readNiPSEmitterRotAngleCtlr(NiPSEmitterRotAngleCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
}

void
readNiPSEmitterRotAngleVarCtlr(NiPSEmitterRotAngleVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSEmitterRotAngleCtlr));
	readNiPSEmitterRotAngleCtlr (obj->parent, 0);
}

void
readNiPSEmitterRotSpeedCtlr(NiPSEmitterRotSpeedCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
}

void
readNiPSEmitterRotSpeedVarCtlr(NiPSEmitterRotSpeedVarCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSEmitterRotSpeedCtlr));
	readNiPSEmitterRotSpeedCtlr (obj->parent, 0);
}

void
readNiPSEmitterLifeSpanCtlr(NiPSEmitterLifeSpanCtlr * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiPSysModifierCtlr));
	readNiPSysModifierCtlr (obj->parent, 0);
}

void
readNiPSBombForce(NiPSBombForce * obj, unsigned int ARG)
{
	COVERAGE
	readstring (&obj->Name, 0);
	obj->Unknown_1 = readbyte (READER);
	READ (int, &(obj->Unknown_2), 9, SIZEOFDWORD, "readNiPSBombForce")
}

void
readNiPSSphericalCollider(NiPSSphericalCollider * obj, unsigned int ARG)
{
	COVERAGE
	obj->Unknown_1 = readint (READER);
	obj->Unknown_2 = readint (READER);
	obj->Unknown_3 = readbyte (READER);
	obj->Unknown_4 = readfloat (READER);
	obj->Unknown_5 = readint (READER);
	obj->Unknown_6 = readshort (READER);
	obj->Unknown_7 = readint (READER);
}

inline void
readNiPSSpawner(NiPSSpawner * obj, unsigned int ARG)
{
	COVERAGE
}

inline void
readNiSequenceData(NiSequenceData * obj, unsigned int ARG)
{
	COVERAGE
}

inline void
readNiTransformEvaluator(NiTransformEvaluator * obj, unsigned int ARG)
{
	COVERAGE
}

inline void
readNiBSplineCompTransformEvaluator(NiBSplineCompTransformEvaluator * obj, unsigned int ARG)
{
	COVERAGE
}

inline void
readNiMeshHWInstance(NiMeshHWInstance * obj, unsigned int ARG)
{
	COVERAGE
}

void
readNiFurSpringController(NiFurSpringController * obj, unsigned int ARG)
{
	COVERAGE
	obj->parent = GETMEM (sizeof (NiTimeController));
	readNiTimeController (obj->parent, 0);
	obj->Unknown_Float = readfloat (READER);
	obj->Unknown_Float_2 = readfloat (READER);
	obj->Num_Bones = readuint (READER);
	PROTECT_LEN (obj->Num_Bones, 2000000, "readNiFurSpringController")
	obj->Bones = GETMEM ((obj->Num_Bones) * sizeof (unsigned int));
	READ (uint, obj->Bones, obj->Num_Bones, SIZEOFDWORD, "readNiFurSpringController")
	obj->Num_Bones_2 = readuint (READER);
	PROTECT_LEN (obj->Num_Bones_2, 2000000, "readNiFurSpringController")
	obj->Bones_2 = GETMEM ((obj->Num_Bones_2) * sizeof (unsigned int));
	READ (uint, obj->Bones_2, obj->Num_Bones_2, SIZEOFDWORD, "readNiFurSpringController")
}

//static int iamthelastline;
