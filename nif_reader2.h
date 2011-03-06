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

#ifndef __NIF_READER2_H__
#define __NIF_READER2_H__

#include "niff.h"

int readnif(char *fname);
// a few stat fields
long NIFF_FSIZE;
int NIFF_BLOCK_COUNT;
int NIFF_OBJECT_COUNT;
int NIFF_MALLOCS;
int NIFF_REALLOCS;

enum AlphaFormat {		// uint
	ALPHA_NONE = 0,
	ALPHA_BINARY = 1,
	ALPHA_SMOOTH = 2,
	ALPHA_DEFAULT = 3,
};

enum ApplyMode {		// uint
	APPLY_REPLACE = 0,
	APPLY_DECAL = 1,
	APPLY_MODULATE = 2,
	APPLY_HILIGHT = 3,
	APPLY_HILIGHT2 = 4,
};

enum TexType {			// uint
	BASE_MAP = 0,
	DARK_MAP = 1,
	DETAIL_MAP = 2,
	GLOSS_MAP = 3,
	GLOW_MAP = 4,
	BUMP_MAP = 5,
	NORMAL_MAP = 6,
	UNKNOWN2_MAP = 7,
	DECAL_0_MAP = 8,
	DECAL_1_MAP = 9,
	DECAL_2_MAP = 10,
	DECAL_3_MAP = 11,
};

enum KeyType {			// uint
	LINEAR_KEY = 1,
	QUADRATIC_KEY = 2,
	TBC_KEY = 3,
	XYZ_ROTATION_KEY = 4,
	CONST_KEY = 5,
};

enum LightMode {		// uint
	LIGHT_MODE_EMISSIVE = 0,
	LIGHT_MODE_EMI_AMB_DIF = 1,
};

enum HavokMaterial {		// uint
	HAV_MAT_STONE = 0,
	HAV_MAT_CLOTH = 1,
	HAV_MAT_DIRT = 2,
	HAV_MAT_GLASS = 3,
	HAV_MAT_GRASS = 4,
	HAV_MAT_METAL = 5,
	HAV_MAT_ORGANIC = 6,
	HAV_MAT_SKIN = 7,
	HAV_MAT_WATER = 8,
	HAV_MAT_WOOD = 9,
	HAV_MAT_HEAVY_STONE = 10,
	HAV_MAT_HEAVY_METAL = 11,
	HAV_MAT_HEAVY_WOOD = 12,
	HAV_MAT_CHAIN = 13,
	HAV_MAT_SNOW = 14,
	HAV_MAT_STONE_STAIRS = 15,
	HAV_MAT_CLOTH_STAIRS = 16,
	HAV_MAT_DIRT_STAIRS = 17,
	HAV_MAT_GLASS_STAIRS = 18,
	HAV_MAT_GRASS_STAIRS = 19,
	HAV_MAT_METAL_STAIRS = 20,
	HAV_MAT_ORGANIC_STAIRS = 21,
	HAV_MAT_SKIN_STAIRS = 22,
	HAV_MAT_WATER_STAIRS = 23,
	HAV_MAT_WOOD_STAIRS = 24,
	HAV_MAT_HEAVY_STONE_STAIRS = 25,
	HAV_MAT_HEAVY_METAL_STAIRS = 26,
	HAV_MAT_HEAVY_WOOD_STAIRS = 27,
	HAV_MAT_CHAIN_STAIRS = 28,
	HAV_MAT_SNOW_STAIRS = 29,
	HAV_MAT_ELEVATOR = 30,
	HAV_MAT_RUBBER = 31,
};

enum OblivionLayer {		// byte
	OL_UNIDENTIFIED = 0,
	OL_STATIC = 1,
	OL_ANIM_STATIC = 2,
	OL_TRANSPARENT = 3,
	OL_CLUTTER = 4,
	OL_WEAPON = 5,
	OL_PROJECTILE = 6,
	OL_SPELL = 7,
	OL_BIPED = 8,
	OL_TREES = 9,
	OL_PROPS = 10,
	OL_WATER = 11,
	OL_TRIGGER = 12,
	OL_TERRAIN = 13,
	OL_TRAP = 14,
	OL_NONCOLLIDABLE = 15,
	OL_CLOUD_TRAP = 16,
	OL_GROUND = 17,
	OL_PORTAL = 18,
	OL_STAIRS = 19,
	OL_CHAR_CONTROLLER = 20,
	OL_AVOID_BOX = 21,
	OL_UNKNOWN1 = 22,
	OL_UNKNOWN2 = 23,
	OL_CAMERA_PICK = 24,
	OL_ITEM_PICK = 25,
	OL_LINE_OF_SIGHT = 26,
	OL_PATH_PICK = 27,
	OL_CUSTOM_PICK_1 = 28,
	OL_CUSTOM_PICK_2 = 29,
	OL_SPELL_EXPLOSION = 30,
	OL_DROPPING_PICK = 31,
	OL_OTHER = 32,
	OL_HEAD = 33,
	OL_BODY = 34,
	OL_SPINE1 = 35,
	OL_SPINE2 = 36,
	OL_L_UPPER_ARM = 37,
	OL_L_FOREARM = 38,
	OL_L_HAND = 39,
	OL_L_THIGH = 40,
	OL_L_CALF = 41,
	OL_L_FOOT = 42,
	OL_R_UPPER_ARM = 43,
	OL_R_FOREARM = 44,
	OL_R_HAND = 45,
	OL_R_THIGH = 46,
	OL_R_CALF = 47,
	OL_R_FOOT = 48,
	OL_TAIL = 49,
	OL_SIDE_WEAPON = 50,
	OL_SHIELD = 51,
	OL_QUIVER = 52,
	OL_BACK_WEAPON = 53,
	OL_BACK_WEAPON2 = 54,
	OL_PONYTAIL = 55,
	OL_WING = 56,
	OL_NULL = 57,
};

enum MipMapFormat {		// uint
	MIP_FMT_NO = 0,
	MIP_FMT_YES = 1,
	MIP_FMT_DEFAULT = 2,
};

enum PixelFormat {		// uint
	PX_FMT_RGB8 = 0,
	PX_FMT_RGBA8 = 1,
	PX_FMT_PAL8 = 2,
	PX_FMT_DXT1 = 4,
	PX_FMT_DXT5 = 5,
	PX_FMT_DXT5_ALT = 6,
};

enum PixelLayout {		// uint
	PIX_LAY_PALETTISED = 0,
	PIX_LAY_HIGH_COLOR_16 = 1,
	PIX_LAY_TRUE_COLOR_32 = 2,
	PIX_LAY_COMPRESSED = 3,
	PIX_LAY_BUMPMAP = 4,
	PIX_LAY_PALETTISED_4 = 5,
	PIX_LAY_DEFAULT = 6,
};

enum TexClampMode {		// uint
	CLAMP_S_CLAMP_T = 0,
	CLAMP_S_WRAP_T = 1,
	WRAP_S_CLAMP_T = 2,
	WRAP_S_WRAP_T = 3,
};

enum TexFilterMode {		// uint
	FILTER_NEAREST = 0,
	FILTER_BILERP = 1,
	FILTER_TRILERP = 2,
	FILTER_NEAREST_MIPNEAREST = 3,
	FILTER_NEAREST_MIPLERP = 4,
	FILTER_BILERP_MIPNEAREST = 5,
};

enum VertMode {			// uint
	VERT_MODE_SRC_IGNORE = 0,
	VERT_MODE_SRC_EMISSIVE = 1,
	VERT_MODE_SRC_AMB_DIF = 2,
};

enum CycleType {		// uint
	CYCLE_LOOP = 0,
	CYCLE_REVERSE = 1,
	CYCLE_CLAMP = 2,
};

enum FieldType {		// uint
	FIELD_WIND = 0,
	FIELD_POINT = 1,
};

enum BillboardMode {		// ushort
	ALWAYS_FACE_CAMERA = 0,
	ROTATE_ABOUT_UP = 1,
	RIGID_FACE_CAMERA = 2,
	ALWAYS_FACE_CENTER = 3,
	RIGID_FACE_CENTER = 4,
	ROTATE_ABOUT_UP2 = 9,
};

enum StencilCompareMode {	// uint
	TEST_NEVER = 0,
	TEST_LESS = 1,
	TEST_EQUAL = 2,
	TEST_LESS_EQUAL = 3,
	TEST_GREATER = 4,
	TEST_NOT_EQUAL = 5,
	TEST_GREATER_EQUAL = 6,
	TEST_ALWAYS = 7,
};

enum ZCompareMode {		// uint
	ZCOMP_ALWAYS = 0,
	ZCOMP_LESS = 1,
	ZCOMP_EQUAL = 2,
	ZCOMP_LESS_EQUAL = 3,
	ZCOMP_GREATER = 4,
	ZCOMP_NOT_EQUAL = 5,
	ZCOMP_GREATER_EQUAL = 6,
	ZCOMP_NEVER = 7,
};

enum StencilAction {		// uint
	ACTION_KEEP = 0,
	ACTION_ZERO = 1,
	ACTION_REPLACE = 2,
	ACTION_INCREMENT = 3,
	ACTION_DECREMENT = 4,
	ACTION_INVERT = 5,
};

enum FaceDrawMode {		// uint
	DRAW_CCW_OR_BOTH = 0,
	DRAW_CCW = 1,
	DRAW_CW = 2,
	DRAW_BOTH = 3,
};

enum MotionSystem {		// byte
	MO_SYS_INVALID = 0,
	MO_SYS_DYNAMIC = 1,
	MO_SYS_SPHERE = 2,
	MO_SYS_SPHERE_INERTIA = 3,
	MO_SYS_BOX = 4,
	MO_SYS_BOX_STABILIZED = 5,
	MO_SYS_KEYFRAMED = 6,
	MO_SYS_FIXED = 7,
	MO_SYS_THIN_BOX = 8,
	MO_SYS_CHARACTER = 9,
};

enum DeactivatorType {		// byte
	DEACTIVATOR_INVALID = 0,
	DEACTIVATOR_NEVER = 1,
	DEACTIVATOR_SPATIAL = 2,
};

enum SolverDeactivation {	// byte
	SOLVER_DEACTIVATION_INVALID = 0,
	SOLVER_DEACTIVATION_OFF = 1,
	SOLVER_DEACTIVATION_LOW = 2,
	SOLVER_DEACTIVATION_MEDIUM = 3,
	SOLVER_DEACTIVATION_HIGH = 4,
	SOLVER_DEACTIVATION_MAX = 5,
};

enum MotionQuality {		// byte
	MO_QUAL_INVALID = 0,
	MO_QUAL_FIXED = 1,
	MO_QUAL_KEYFRAMED = 2,
	MO_QUAL_DEBRIS = 3,
	MO_QUAL_MOVING = 4,
	MO_QUAL_CRITICAL = 5,
	MO_QUAL_BULLET = 6,
	MO_QUAL_USER = 7,
	MO_QUAL_CHARACTER = 8,
	MO_QUAL_KEYFRAMED_REPORT = 9,
};

enum ForceType {		// uint
	FORCE_PLANAR = 0,
	FORCE_SPHERICAL = 1,
	FORCE_UNKNOWN = 2,
};

enum TexTransform {		// uint
	TT_TRANSLATE_U = 0,
	TT_TRANSLATE_V = 1,
	TT_ROTATE = 2,
	TT_SCALE_U = 3,
	TT_SCALE_V = 4,
};

enum DecayType {		// uint
	DECAY_NONE = 0,
	DECAY_LINEAR = 1,
	DECAY_EXPONENTIAL = 2,
};

enum SymmetryType {		// uint
	SPHERICAL_SYMMETRY = 0,
	CYLINDRICAL_SYMMETRY = 1,
	PLANAR_SYMMETRY = 2,
};

enum VelocityType {		// uint
	VELOCITY_USE_NORMALS = 0,
	VELOCITY_USE_RANDOM = 1,
	VELOCITY_USE_DIRECTION = 2,
};

enum EmitFrom {			// uint
	EMIT_FROM_VERTICES = 0,
	EMIT_FROM_FACE_CENTER = 1,
	EMIT_FROM_EDGE_CENTER = 2,
	EMIT_FROM_FACE_SURFACE = 3,
	EMIT_FROM_EDGE_SURFACE = 4,
};

enum EffectType {		// uint
	EFFECT_PROJECTED_LIGHT = 0,
	EFFECT_PROJECTED_SHADOW = 1,
	EFFECT_ENVIRONMENT_MAP = 2,
	EFFECT_FOG_MAP = 3,
};

enum CoordGenType {		// uint
	CG_WORLD_PARALLEL = 0,
	CG_WORLD_PERSPECTIVE = 1,
	CG_SPHERE_MAP = 2,
	CG_SPECULAR_CUBE_MAP = 3,
	CG_DIFFUSE_CUBE_MAP = 4,
};

enum EndianType {		// byte
	ENDIAN_BIG = 0,
	ENDIAN_LITTLE = 1,
};

enum TargetColor {		// ushort
	TC_AMBIENT = 0,
	TC_DIFFUSE = 1,
	TC_SPECULAR = 2,
	TC_SELF_ILLUM = 3,
};

enum ConsistencyType {		// ushort
	CT_MUTABLE = 0x0000,
	CT_STATIC = 0x4000,
	CT_VOLATILE = 0x8000,
};

enum SortingMode {		// uint
	SORTING_INHERIT = 0,
	SORTING_OFF = 1,
};

enum PropagationMode {		// uint
	PROPAGATE_ON_SUCCESS = 0,
	PROPAGATE_ON_FAILURE = 1,
	PROPAGATE_ALWAYS = 2,
	PROPAGATE_NEVER = 3,
};

enum CollisionMode {		// uint
	CM_USE_OBB = 0,
	CM_USE_TRI = 1,
	CM_USE_ABV = 2,
	CM_NOTEST = 3,
	CM_USE_NIBOUND = 4,
};

enum BoundVolumeType {		// uint
	BASE_BV = -1,
	SPHERE_BV = 0,
	BOX_BV = 1,
	CAPSULE_BV = 2,
	UNION_BV = 4,
	HALFSPACE_BV = 5,
};

enum hkResponseType {		// byte
	RESPONSE_INVALID = 0,
	RESPONSE_SIMPLE_CONTACT = 1,
	RESPONSE_REPORTING = 2,
	RESPONSE_NONE = 3,
};

enum BSDismemberBodyPartType {	// ushort
	BP_TORSO = 0,
	BP_HEAD = 1,
	BP_HEAD2 = 2,
	BP_LEFTARM = 3,
	BP_LEFTARM2 = 4,
	BP_RIGHTARM = 5,
	BP_RIGHTARM2 = 6,
	BP_LEFTLEG = 7,
	BP_LEFTLEG2 = 8,
	BP_LEFTLEG3 = 9,
	BP_RIGHTLEG = 10,
	BP_RIGHTLEG2 = 11,
	BP_RIGHTLEG3 = 12,
	BP_BRAIN = 13,
	BP_SECTIONCAP_HEAD = 101,
	BP_SECTIONCAP_HEAD2 = 102,
	BP_SECTIONCAP_LEFTARM = 103,
	BP_SECTIONCAP_LEFTARM2 = 104,
	BP_SECTIONCAP_RIGHTARM = 105,
	BP_SECTIONCAP_RIGHTARM2 = 106,
	BP_SECTIONCAP_LEFTLEG = 107,
	BP_SECTIONCAP_LEFTLEG2 = 108,
	BP_SECTIONCAP_LEFTLEG3 = 109,
	BP_SECTIONCAP_RIGHTLEG = 110,
	BP_SECTIONCAP_RIGHTLEG2 = 111,
	BP_SECTIONCAP_RIGHTLEG3 = 112,
	BP_SECTIONCAP_BRAIN = 113,
	BP_TORSOCAP_HEAD = 201,
	BP_TORSOCAP_HEAD2 = 202,
	BP_TORSOCAP_LEFTARM = 203,
	BP_TORSOCAP_LEFTARM2 = 204,
	BP_TORSOCAP_RIGHTARM = 205,
	BP_TORSOCAP_RIGHTARM2 = 206,
	BP_TORSOCAP_LEFTLEG = 207,
	BP_TORSOCAP_LEFTLEG2 = 208,
	BP_TORSOCAP_LEFTLEG3 = 209,
	BP_TORSOCAP_RIGHTLEG = 210,
	BP_TORSOCAP_RIGHTLEG2 = 211,
	BP_TORSOCAP_RIGHTLEG3 = 212,
	BP_TORSOCAP_BRAIN = 213,
	BP_TORSOSECTION_HEAD = 1000,
	BP_TORSOSECTION_HEAD2 = 2000,
	BP_TORSOSECTION_LEFTARM = 3000,
	BP_TORSOSECTION_LEFTARM2 = 4000,
	BP_TORSOSECTION_RIGHTARM = 5000,
	BP_TORSOSECTION_RIGHTARM2 = 6000,
	BP_TORSOSECTION_LEFTLEG = 7000,
	BP_TORSOSECTION_LEFTLEG2 = 8000,
	BP_TORSOSECTION_LEFTLEG3 = 9000,
	BP_TORSOSECTION_RIGHTLEG = 10000,
	BP_TORSOSECTION_RIGHTLEG2 = 11000,
	BP_TORSOSECTION_RIGHTLEG3 = 12000,
	BP_TORSOSECTION_BRAIN = 13000,
};

enum ImageType {		// uint
	RGB = 1,
	RGBA = 2,
};

enum BSPartFlag {		// ushort
	PF_EDITOR_VISIBLE = 0,
	PF_START_NET_BONESET = 8,
};

enum ChannelType {		// uint
	CHNL_RED = 0,
	CHNL_GREEN = 1,
	CHNL_BLUE = 2,
	CHNL_ALPHA = 3,
	CHNL_COMPRESSED = 4,
	CHNL_INDEX = 16,
	CHNL_EMPTY = 19,
};

enum ChannelConvention {	// uint
	CC_FIXED = 0,
	CC_INDEX = 3,
	CC_COMPRESSED = 4,
	CC_EMPTY = 5,
};

enum BSShaderType {		// uint
	SHADER_TALL_GRASS = 0,
	SHADER_DEFAULT = 1,
	SHADER_SKY = 10,
	SHADER_SKIN = 14,
	SHADER_WATER = 17,
	SHADER_LIGHTING30 = 29,
	SHADER_TILE = 32,
	SHADER_NOLIGHTING = 33,
};

enum BSShaderFlags {		// uint
	SF_ZBUFFER_TEST = 0,
	SF_SHADOW_MAP = 1,
	SF_EXTERNAL_EMITTANCE = 2,
	SF_PARALLAX_OCCLUSION = 3,
	SF_DYNAMIC_DECAL = 4,
	SF_DECAL = 5,
	SF_UNKNOWN_6 = 6,
	SF_MULTIPLE_TEXTURES = 7,
	SF_SHADOW_FRUSTUM = 8,
	SF_TREE_BILLBOARD = 9,
	SF_WINDOW_ENVIRONMENT_MAPPING = 10,
	SF_LOCALMAP_HIDE_SECRET = 11,
	SF_DYNAMIC_ALPHA = 12,
	SF_HAIR = 13,
	SF_EYE_ENVIRONMENT_MAPPING = 14,
	SF_FIRE_REFRACTION = 15,
	SF_REFRACTION = 16,
	SF_UNKNOWN_17 = 17,
	NON_PROJECTIVE_SHADOWS = 18,
	SF_UNKNOWN_19 = 19,
	SF_PARALLAX = 20,
	SF_FACEGEN_SKIN = 21,
	SF_UNKNOWN_22 = 22,
	SF_UNKNOWN_23 = 23,
	SF_ENVIRONMENT_MAPPING = 24,
	SF_EMPTY = 25,
	SF_SINGLE_PASS = 26,
	SF_UNKNOWN_27 = 27,
	SF_VERTEX_ALPHA = 28,
	SF_LOWDDETAIL = 29,
	SF_SKINNED = 30,
	SF_UNKNOWN_31 = 31,
};

enum CloningBehavior {		// uint
	CLONING_SHARE = 0,
	CLONING_COPY = 1,
	CLONING_BLANK_COPY = 2,
};

enum ComponentFormat {		// uint
	F_UNKNOWN = 0x00000000,
	F_INT8_1 = 0x00010101,
	F_INT8_2 = 0x00020102,
	F_INT8_3 = 0x00030103,
	F_INT8_4 = 0x00040104,
	F_UINT8_1 = 0x00010105,
	F_UINT8_2 = 0x00020106,
	F_UINT8_3 = 0x00030107,
	F_UINT8_4 = 0x00040108,
	F_NORMINT8_1 = 0x00010109,
	F_NORMINT8_2 = 0x0002010A,
	F_NORMINT8_3 = 0x0003010B,
	F_NORMINT8_4 = 0x0004010C,
	F_NORMUINT8_1 = 0x0001010D,
	F_NORMUINT8_2 = 0x0002010E,
	F_NORMUINT8_3 = 0x0003010F,
	F_NORMUINT8_4 = 0x00040110,
	F_INT16_1 = 0x00010211,
	F_INT16_2 = 0x00020212,
	F_INT16_3 = 0x00030213,
	F_INT16_4 = 0x00040214,
	F_UINT16_1 = 0x00010215,
	F_UINT16_2 = 0x00020216,
	F_UINT16_3 = 0x00030217,
	F_UINT16_4 = 0x00040218,
	F_NORMINT16_1 = 0x00010219,
	F_NORMINT16_2 = 0x0002021A,
	F_NORMINT16_3 = 0x0003021B,
	F_NORMINT16_4 = 0x0004021C,
	F_NORMUINT16_1 = 0x0001021D,
	F_NORMUINT16_2 = 0x0002021E,
	F_NORMUINT16_3 = 0x0003021F,
	F_NORMUINT16_4 = 0x00040220,
	F_INT32_1 = 0x00010421,
	F_INT32_2 = 0x00020422,
	F_INT32_3 = 0x00030423,
	F_INT32_4 = 0x00040424,
	F_UINT32_1 = 0x00010425,
	F_UINT32_2 = 0x00020426,
	F_UINT32_3 = 0x00030427,
	F_UINT32_4 = 0x00040428,
	F_NORMINT32_1 = 0x00010429,
	F_NORMINT32_2 = 0x0002042A,
	F_NORMINT32_3 = 0x0003042B,
	F_NORMINT32_4 = 0x0004042C,
	F_NORMUINT32_1 = 0x0001042D,
	F_NORMUINT32_2 = 0x0002042E,
	F_NORMUINT32_3 = 0x0003042F,
	F_NORMUINT32_4 = 0x00040430,
	F_FLOAT16_1 = 0x00010231,
	F_FLOAT16_2 = 0x00020232,
	F_FLOAT16_3 = 0x00030233,
	F_FLOAT16_4 = 0x00040234,
	F_FLOAT32_1 = 0x00010435,
	F_FLOAT32_2 = 0x00020436,
	F_FLOAT32_3 = 0x00030437,
	F_FLOAT32_4 = 0x00040438,
	F_UINT_10_10_10_L1 = 0x00010439,
	F_NORMINT_10_10_10_L1 = 0x0001043A,
	F_NORMINT_11_11_10 = 0x0001043B,
	F_NORMUINT8_4_BGRA = 0x0004013C,
	F_NORMINT_10_10_10_2 = 0x0001043D,
	F_UINT_10_10_10_2 = 0x0001043E,
};

enum DataStreamUsage {		// uint
	USAGE_VERTEX_INDEX = 0,
	USAGE_VERTEX = 1,
	USAGE_SHADER_CONSTANT = 2,
	USAGE_USER = 3,
};

enum DataStreamAccess {		// uint
	CPU_Read = 0,
	CPU_Write_Static = 1,
	CPU_Write_Mutable = 2,
	CPU_Write_Volatile = 3,
	GPU_Read = 4,
	GPU_Write = 5,
	CPU_Write_Static_Inititialized = 6,
};

enum MeshPrimitiveType {	// uint
	MESH_PRIMITIVE_TRIANGLES = 0,
	MESH_PRIMITIVE_TRISTRIPS = 1,
	MESH_PRIMITIVE_LINESTRIPS = 2,
	MESH_PRIMITIVE_QUADS = 3,
	MESH_PRIMITIVE_POINTS = 4,
};

enum SyncPoint {		// ushort
	SYNC_ANY = 0x8000,
	SYNC_UPDATE = 0x8010,
	SYNC_POST_UPDATE = 0x8020,
	SYNC_VISIBLE = 0x8030,
	SYNC_RENDER = 0x8040,
	SYNC_PHYSICS_SIMULATE = 0x8050,
	SYNC_PHYSICS_COMPLETED = 0x8060,
	SYNC_REFLECTIONS = 0x8070,
};

enum PSLoopBehavior {		// uint
	PS_LOOP_CLAMP_BIRTH = 0,
	PS_LOOP_CLAMP_DEATH = 1,
	PS_LOOP_AGESCALE = 2,
	PS_LOOP_LOOP = 3,
	PS_LOOP_REFLECT = 4,
};

struct SizedString;
struct string;
struct ByteArray;
struct ByteMatrix;
struct Color3;
struct ByteColor3;
struct Color4;
struct ByteColor4;
struct FilePath;
struct Footer;
struct LODRange;
struct MatchGroup;
struct Vector3;
struct Vector4;
struct Quaternion;
struct QuaternionXYZW;
struct Matrix22;
struct Matrix33;
struct Matrix44;
struct MipMap;
struct NodeGroup;
struct ShortString;
struct SkinShape;
struct SkinShapeGroup;
struct SkinWeight;
struct AVObject;
struct ControllerLink;
struct ExportInfo;
struct Header;
struct StringPalette;
struct TBC;
struct Key;
struct KeyGroup;
struct QuatKey;
struct TexCoord;
struct TexDesc;
struct ShaderTexDesc;
struct TexSource;
struct Triangle;
struct SkinPartition;
struct BoundingBox;
struct FurniturePosition;
struct hkTriangle;
struct Morph;
struct Particle;
struct SkinTransform;
struct SkinData;
struct SphereBV;
struct OblivionColFilter;
struct OblivionSubShape;
struct MotorDescriptor;
struct RagdollDescriptor;
struct LimitedHingeDescriptor;
struct HingeDescriptor;
struct OldSkinData;
struct MultiTextureElement;
struct BoxBV;
struct CapsuleBV;
struct HalfSpaceBV;
struct BoundingVolume;
struct UnionBV;
struct MorphWeight;
struct ArkTexture;
struct InertiaMatrix;
struct DecalVectorArray;
struct BodyPartList;
struct NiObject;
struct Ni3dsAlphaAnimator;
struct Ni3dsAnimationNode;
struct Ni3dsColorAnimator;
struct Ni3dsMorphShape;
struct Ni3dsParticleSystem;
struct Ni3dsPathController;
struct NiParticleModifier;
struct NiPSysCollider;
struct bhkRefObject;
struct bhkSerializable;
struct bhkWorldObject;
struct bhkPhantom;
struct bhkShapePhantom;
struct bhkSimpleShapePhantom;
struct bhkEntity;
struct bhkRigidBody;
struct bhkRigidBodyT;
struct bhkConstraint;
struct bhkLimitedHingeConstraint;
struct bhkMalleableConstraint;
struct bhkStiffSpringConstraint;
struct bhkRagdollConstraint;
struct bhkPrismaticConstraint;
struct bhkHingeConstraint;
struct bhkShape;
struct bhkTransformShape;
struct bhkSphereRepShape;
struct bhkConvexShape;
struct bhkSphereShape;
struct bhkCapsuleShape;
struct bhkBoxShape;
struct bhkConvexVerticesShape;
struct bhkConvexTransformShape;
struct bhkMultiSphereShape;
struct bhkBvTreeShape;
struct bhkMoppBvTreeShape;
struct bhkShapeCollection;
struct bhkListShape;
struct bhkMeshShape;
struct bhkPackedNiTriStripsShape;
struct bhkNiTriStripsShape;
struct NiExtraData;
struct NiInterpolator;
struct NiKeyBasedInterpolator;
struct NiFloatInterpolator;
struct NiTransformInterpolator;
struct NiPoint3Interpolator;
struct NiPathInterpolator;
struct NiBoolInterpolator;
struct NiBoolTimelineInterpolator;
struct NiBlendInterpolator;
struct NiBSplineInterpolator;
struct NiObjectNET;
struct NiCollisionObject;
struct NiCollisionData;
struct bhkNiCollisionObject;
struct bhkCollisionObject;
struct bhkBlendCollisionObject;
struct bhkPCollisionObject;
struct bhkSPCollisionObject;
struct NiAVObject;
struct NiDynamicEffect;
struct NiLight;
struct NiProperty;
struct NiTransparentProperty;
struct NiPSysModifier;
struct NiPSysEmitter;
struct NiPSysVolumeEmitter;
struct NiTimeController;
struct NiInterpController;
struct NiMultiTargetTransformController;
struct NiGeomMorpherController;
struct NiMorphController;
struct NiMorpherController;
struct NiSingleInterpController;
struct NiKeyframeController;
struct NiTransformController;
struct NiPSysModifierCtlr;
struct NiPSysEmitterCtlr;
struct NiPSysModifierBoolCtlr;
struct NiPSysModifierActiveCtlr;
struct NiPSysModifierFloatCtlr;
struct NiPSysEmitterDeclinationCtlr;
struct NiPSysEmitterDeclinationVarCtlr;
struct NiPSysEmitterInitialRadiusCtlr;
struct NiPSysEmitterLifeSpanCtlr;
struct NiPSysEmitterSpeedCtlr;
struct NiPSysGravityStrengthCtlr;
struct NiFloatInterpController;
struct NiFlipController;
struct NiAlphaController;
struct NiTextureTransformController;
struct NiLightDimmerController;
struct NiBoolInterpController;
struct NiVisController;
struct NiPoint3InterpController;
struct NiMaterialColorController;
struct NiLightColorController;
struct NiExtraDataController;
struct NiFloatExtraDataController;
struct NiBoneLODController;
struct NiBSBoneLODController;
struct NiGeometry;
struct NiTriBasedGeom;
struct NiGeometryData;
struct NiTriBasedGeomData;
struct bhkBlendController;
struct BSBound;
struct BSFurnitureMarker;
struct BSParentVelocityModifier;
struct BSPSysArrayEmitter;
struct BSWindModifier;
struct hkPackedNiTriStripsData;
struct NiAlphaProperty;
struct NiAmbientLight;
struct NiParticlesData;
struct NiRotatingParticlesData;
struct NiAutoNormalParticlesData;
struct ParticleDesc;
struct NiPSysData;
struct NiMeshPSysData;
struct NiBinaryExtraData;
struct NiBinaryVoxelExtraData;
struct NiBinaryVoxelData;
struct NiBlendBoolInterpolator;
struct NiBlendFloatInterpolator;
struct NiBlendPoint3Interpolator;
struct NiBlendTransformInterpolator;
struct NiBoolData;
struct NiBooleanExtraData;
struct NiBSplineBasisData;
struct NiBSplineFloatInterpolator;
struct NiBSplineCompFloatInterpolator;
struct NiBSplinePoint3Interpolator;
struct NiBSplineCompPoint3Interpolator;
struct NiBSplineTransformInterpolator;
struct NiBSplineCompTransformInterpolator;
struct BSRotAccumTransfInterpolator;
struct NiBSplineData;
struct NiCamera;
struct NiColorData;
struct NiColorExtraData;
struct NiControllerManager;
struct NiSequence;
struct NiControllerSequence;
struct NiAVObjectPalette;
struct NiDefaultAVObjectPalette;
struct NiDirectionalLight;
struct NiDitherProperty;
struct NiRollController;
struct NiFloatData;
struct NiFloatExtraData;
struct NiFloatsExtraData;
struct NiFogProperty;
struct NiGravity;
struct NiIntegerExtraData;
struct BSXFlags;
struct NiIntegersExtraData;
struct BSKeyframeController;
struct NiKeyframeData;
struct NiLookAtController;
struct NiLookAtInterpolator;
struct NiMaterialProperty;
struct NiMorphData;
struct NiNode;
struct NiBone;
struct AvoidNode;
struct FxWidget;
struct FxButton;
struct FxRadioButton;
struct NiBillboardNode;
struct NiBSAnimationNode;
struct NiBSParticleNode;
struct NiSwitchNode;
struct NiLODNode;
struct NiPalette;
struct NiParticleBomb;
struct NiParticleColorModifier;
struct NiParticleGrowFade;
struct NiParticleMeshModifier;
struct NiParticleRotation;
struct NiParticles;
struct NiAutoNormalParticles;
struct NiParticleMeshes;
struct NiParticleMeshesData;
struct NiParticleSystem;
struct NiMeshParticleSystem;
struct NiParticleSystemController;
struct NiBSPArrayController;
struct NiPathController;
struct ChannelData;
struct ATextureRenderData;
struct NiPersistentSrcTextureRendererData;
struct NiPixelData;
struct NiPlanarCollider;
struct NiPointLight;
struct NiPosData;
struct NiPSysAgeDeathModifier;
struct NiPSysBombModifier;
struct NiPSysBoundUpdateModifier;
struct NiPSysBoxEmitter;
struct NiPSysColliderManager;
struct NiPSysColorModifier;
struct NiPSysCylinderEmitter;
struct NiPSysDragModifier;
struct NiPSysEmitterCtlrData;
struct NiPSysGravityModifier;
struct NiPSysGrowFadeModifier;
struct NiPSysMeshEmitter;
struct NiPSysMeshUpdateModifier;
struct NiPSysPlanarCollider;
struct NiPSysSphericalCollider;
struct NiPSysPositionModifier;
struct NiPSysResetOnLoopCtlr;
struct NiPSysRotationModifier;
struct NiPSysSpawnModifier;
struct NiPSysSphereEmitter;
struct NiPSysUpdateCtlr;
struct NiPSysFieldModifier;
struct NiPSysVortexFieldModifier;
struct NiPSysGravityFieldModifier;
struct NiPSysDragFieldModifier;
struct NiPSysTurbulenceFieldModifier;
struct NiPSysFieldMagnitudeCtlr;
struct NiPSysFieldAttenuationCtlr;
struct NiPSysFieldMaxDistanceCtlr;
struct NiPSysAirFieldAirFrictionCtlr;
struct NiPSysAirFieldInheritVelocityCtlr;
struct NiPSysAirFieldSpreadCtlr;
struct NiPSysInitialRotSpeedCtlr;
struct NiPSysInitialRotSpeedVarCtlr;
struct NiPSysInitialRotAngleCtlr;
struct NiPSysInitialRotAngleVarCtlr;
struct NiPSysEmitterPlanarAngleCtlr;
struct NiPSysEmitterPlanarAngleVarCtlr;
struct NiPSysAirFieldModifier;
struct NiPSysTrailEmitter;
struct NiLightIntensityController;
struct NiPSysRadialFieldModifier;
struct NiLODData;
struct NiRangeLODData;
struct NiScreenLODData;
struct NiRotatingParticles;
struct NiSequenceStreamHelper;
struct NiShadeProperty;
struct NiSkinData;
struct NiSkinInstance;
struct NiTriShapeSkinController;
struct NiClodSkinInstance;
struct NiSkinPartition;
struct NiTexture;
struct NiSourceTexture;
struct NiSpecularProperty;
struct NiSphericalCollider;
struct NiSpotLight;
struct NiStencilProperty;
struct NiStringExtraData;
struct NiStringPalette;
struct NiStringsExtraData;
struct NiTextKeyExtraData;
struct NiTextureEffect;
struct NiTextureModeProperty;
struct NiImage;
struct NiTextureProperty;
struct NiMultiTextureProperty;
struct NiTexturingProperty;
struct NiTransformData;
struct NiTriShape;
struct NiTriShapeData;
struct NiTriStrips;
struct NiTriStripsData;
struct NiEnvMappedTriShape;
struct NiEnvMappedTriShapeData;
struct NiBezierTriangle4;
struct NiBezierMesh;
struct NiClod;
struct NiClodData;
struct NiUVController;
struct NiUVData;
struct NiVectorExtraData;
struct NiVertexColorProperty;
struct NiVertWeightsExtraData;
struct NiVisData;
struct NiWireframeProperty;
struct NiZBufferProperty;
struct RootCollisionNode;
struct NiRawImageData;
struct NiSortAdjustNode;
struct NiSourceCubeMap;
struct NiPhysXProp;
struct physXMaterialRef;
struct NiPhysXPropDesc;
struct NiPhysXActorDesc;
struct NiPhysXBodyDesc;
struct NiPhysXD6JointDesc;
struct NiPhysXShapeDesc;
struct NiPhysXMeshDesc;
struct NiPhysXMaterialDesc;
struct NiPhysXKinematicSrc;
struct NiPhysXTransformDest;
struct NiArkAnimationExtraData;
struct NiArkImporterExtraData;
struct NiArkTextureExtraData;
struct NiArkViewportInfoExtraData;
struct NiArkShaderExtraData;
struct NiLines;
struct NiLinesData;
struct Polygon;
struct NiScreenElementsData;
struct NiScreenElements;
struct NiRoomGroup;
struct NiRoom;
struct NiPortal;
struct BSFadeNode;
struct BSShaderProperty;
struct BSShaderLightingProperty;
struct BSShaderNoLightingProperty;
struct BSShaderPPLightingProperty;
struct BSShaderTextureSet;
struct WaterShaderProperty;
struct SkyShaderProperty;
struct TileShaderProperty;
struct DistantLODShaderProperty;
struct BSDistantTreeShaderProperty;
struct TallGrassShaderProperty;
struct VolumetricFogShaderProperty;
struct HairShaderProperty;
struct Lighting30ShaderProperty;
struct BSDismemberSkinInstance;
struct BSDecalPlacementVectorExtraData;
struct BSPSysSimpleColorModifier;
struct BSValueNode;
struct BSStripParticleSystem;
struct BSStripPSysData;
struct BSPSysStripUpdateModifier;
struct BSMaterialEmittanceMultController;
struct BSMasterParticleSystem;
struct BSPSysMultiTargetEmitterCtlr;
struct BSRefractionStrengthController;
struct BSOrderedNode;
struct BSBlastNode;
struct BSDamageStage;
struct BSRefractionFirePeriodController;
struct bhkConvexListShape;
struct BSTreadTransfSubInfo;
struct BSTreadTransfInfo;
struct BSTreadTransfInterpolator;
struct BSAnimNotes;
struct bhkLiquidAction;
struct BSMultiBoundNode;
struct BSMultiBound;
struct BSMultiBoundData;
struct BSMultiBoundSphere;
struct BSSegmentedTriangle;
struct BSSegmentedTriShape;
struct BSMultiBoundAABB;
struct AdditionalDataInfo;
struct AdditionalDataBlock;
struct NiAdditionalGeometryData;
struct BSWArray;
struct bhkAabbPhantom;
struct BSFrustumFOVController;
struct BSDebrisNode;
struct bhkBreakableConstraint;
struct bhkOrientHingedBodyAction;
struct Region;
struct NiDataStream;
struct SemanticData;
struct MeshData;
struct MaterialData;
struct NiRenderObject;
struct NiMeshModifier;
struct NiMesh;
struct NiMorphWeightsController;
struct ElementReference;
struct NiMorphMeshModifier;
struct NiSkinningMeshModifier;
struct NiInstancingMeshModifier;
struct NiSkinningLODController;
struct NiPSParticleSystem;
struct NiPSMeshParticleSystem;
struct NiPSEmitParticlesCtlr;
struct NiPSForceActiveCtlr;
struct NiPSSimulator;
struct NiPSSimulatorStep;
struct NiPSSimulatorGeneralStep;
struct NiPSSimulatorForcesStep;
struct NiPSSimulatorCollidersStep;
struct NiPSSimulatorMeshAlignStep;
struct NiPSSimulatorFinalStep;
struct NiPSFacingQuadGenerator;
struct NiShadowGenerator;
struct NiPSBoundUpdater;
struct NiPSDragForce;
struct NiPSGravityForce;
struct NiPSBoxEmitter;
struct NiPSMeshEmitter;
struct NiPSGravityStrengthCtlr;
struct NiPSPlanarCollider;
struct NiPSEmitterSpeedCtlr;
struct NiPSEmitterRadiusCtlr;
struct NiPSResetOnLoopCtlr;
struct NiPSSphereEmitter;
struct NiPSCylinderEmitter;
struct NiPSEmitterDeclinationCtlr;
struct NiPSEmitterDeclinationVarCtlr;
struct NiPSEmitterPlanarAngleCtlr;
struct NiPSEmitterPlanarAngleVarCtlr;
struct NiPSEmitterRotAngleCtlr;
struct NiPSEmitterRotAngleVarCtlr;
struct NiPSEmitterRotSpeedCtlr;
struct NiPSEmitterRotSpeedVarCtlr;
struct NiPSEmitterLifeSpanCtlr;
struct NiPSBombForce;
struct NiPSSphericalCollider;
struct NiPSSpawner;
struct NiSequenceData;
struct NiTransformEvaluator;
struct NiBSplineCompTransformEvaluator;
struct NiMeshHWInstance;
struct NiFurSpringController;

typedef struct {
	NIFuint Length;
	NIFchar *Value;
} SizedString;

typedef struct {
	SizedString String;
	NIFuint Index;
} string;

typedef struct {
	NIFuint DataSize;
	NIFbyte *Data;
} ByteArray;

typedef struct {
	NIFuint DataSize1;
	NIFuint DataSize2;
	NIFbyte *Data;
} ByteMatrix;

typedef struct {
	NIFfloat r;
	NIFfloat g;
	NIFfloat b;
} Color3;

typedef struct {
	NIFbyte r;
	NIFbyte g;
	NIFbyte b;
} ByteColor3;

typedef struct {
	NIFfloat r;
	NIFfloat g;
	NIFfloat b;
	NIFfloat a;
} Color4;

typedef struct {
	NIFbyte r;
	NIFbyte g;
	NIFbyte b;
	NIFbyte a;
} ByteColor4;

typedef struct {
	SizedString String;
	NIFuint Index;
} FilePath;

typedef struct {
	NIFuint NumRoots;
	NIFuint *Roots;// [NumRoots]
} Footer;

typedef struct {
	NIFfloat NearExtent;
	NIFfloat FarExtent;
	NIFuint UnknownInts[3];
} LODRange;

typedef struct {
	NIFushort NumVertices;
	NIFushort *VertexIndices;// [NumVertices]
} MatchGroup;

typedef struct {
	NIFfloat x;
	NIFfloat y;
	NIFfloat z;
} Vector3;

typedef struct {
	NIFfloat x;
	NIFfloat y;
	NIFfloat z;
	NIFfloat w;
} Vector4;

typedef struct {
	NIFfloat w;
	NIFfloat x;
	NIFfloat y;
	NIFfloat z;
} Quaternion;

typedef struct {
	NIFfloat x;
	NIFfloat y;
	NIFfloat z;
	NIFfloat w;
} QuaternionXYZW;

typedef struct {
	NIFfloat m11;
	NIFfloat m21;
	NIFfloat m12;
	NIFfloat m22;
} Matrix22;

typedef struct {
	NIFfloat m11;
	NIFfloat m21;
	NIFfloat m31;
	NIFfloat m12;
	NIFfloat m22;
	NIFfloat m32;
	NIFfloat m13;
	NIFfloat m23;
	NIFfloat m33;
} Matrix33;

typedef struct {
	NIFfloat m11;
	NIFfloat m21;
	NIFfloat m31;
	NIFfloat m41;
	NIFfloat m12;
	NIFfloat m22;
	NIFfloat m32;
	NIFfloat m42;
	NIFfloat m13;
	NIFfloat m23;
	NIFfloat m33;
	NIFfloat m43;
	NIFfloat m14;
	NIFfloat m24;
	NIFfloat m34;
	NIFfloat m44;
} Matrix44;

typedef struct {
	NIFuint Width;
	NIFuint Height;
	NIFuint Offset;
} MipMap;

typedef struct {
	NIFuint NumNodes;
	NIFuint *Nodes;// [NumNodes]
} NodeGroup;

typedef struct {
	NIFbyte Length;
	NIFchar *Value;// [Length]
} ShortString;

typedef struct {
	NIFuint Shape;
	NIFuint SkinInstance;
} SkinShape;

typedef struct {
	NIFuint NumLinkPairs;
	SkinShape *LinkPairs;// [NumLinkPairs]
} SkinShapeGroup;

typedef struct {
	NIFushort Index;
	NIFfloat Weight;
} SkinWeight;

typedef struct {
	SizedString Name;
	NIFuint AVObject;
} AVObject;

typedef struct {
	string TargetName;
	NIFuint Controller;
	NIFuint Interpolator;
	// DUPLICATE FIELD NAME Controller - fixed
	NIFuint UnknownLink2;
	NIFushort UnknownShort0;
	NIFbyte Priority;
	NIFuint StringPalette;
	string NodeName;
	// DUPLICATE FIELD NAME NodeName - fixed
	NIFuint NodeNameOffset;
	string PropertyType;
	// DUPLICATE FIELD NAME PropertyType - fixed
	NIFuint PropertyTypeOffset;
	string ControllerType;
	// DUPLICATE FIELD NAME ControllerType - fixed
	NIFuint ControllerTypeOffset;
	string Variable1;
	// DUPLICATE FIELD NAME Variable1 - fixed
	NIFuint Variable1Offset;
	string Variable2;
	// DUPLICATE FIELD NAME Variable2 - fixed
	NIFuint Variable2Offset;
} ControllerLink;

typedef struct {
	NIFuint Unknown;
	ShortString Creator;
	ShortString ExportInfo1;
	ShortString ExportInfo2;
} ExportInfo;

typedef struct {
	NIFchar *HeaderString;
	NIFchar *Copyright[3];
	NIFuint Version;
	NIFbyte EndianType;
	NIFuint UserVersion;
	NIFuint NumBlocks;
	NIFuint UserVersion2;
	NIFuint UnknownInt3;
	ExportInfo ExportInfo;
	// DUPLICATE FIELD NAME ExportInfo - fixed
	NIFushort NumBlockTypes;
	SizedString *BlockTypes;// [NumBlockTypes]
	NIFushort *BlockTypeIndex;// [NumBlocks]
	NIFuint *BlockSize;// [NumBlocks]
	NIFuint NumStrings;
	NIFuint MaxStringLength;
	SizedString *Strings;// [NumStrings]
	NIFuint UnknownInt2;
} Header;

typedef struct {
	SizedString Palette;
	NIFuint Length;
} StringPalette;

typedef struct {
	NIFfloat t;
	NIFfloat b;
	NIFfloat c;
} TBC;

			/*
   		---/    keys	/ /---
					---/*/

typedef struct {
	NIFfloat Time;
	NIFfloat Value;
	NIFfloat Forward;
	NIFfloat Backward;
	TBC TBC;
} NiFloatKey;

typedef struct {
	NIFfloat Time;
	NIFbyte Value;
	NIFbyte Forward;
	NIFbyte Backward;
	TBC TBC;
} NiByteKey;

typedef struct {
	NIFfloat Time;
	Color4 Value;
	Color4 Forward;
	Color4 Backward;
	TBC TBC;
} NiColor4Key;

typedef struct {
	NIFfloat Time;
	Vector3 Value;
	Vector3 Forward;
	Vector3 Backward;
	TBC TBC;
} NiVector3Key;

typedef struct {
	NIFfloat Time;
	string Value;
	string Forward;
	string Backward;
	TBC TBC;
} NiStringKey;

typedef struct {
	NIFfloat Time;
	ByteColor4 Value;
	ByteColor4 Forward;
	ByteColor4 Backward;
	TBC TBC;
} NiByteColor4Key;

typedef struct {
	NIFuint NumKeys;
	NIFuint Interpolation;
	NiFloatKey *Keys;// [NumKeys]
} NiFloatKeyGroup;

typedef struct {
	NIFuint NumKeys;
	NIFuint Interpolation;
	NiByteKey *Keys;// [NumKeys]
} NiByteKeyGroup;

typedef struct {
	NIFuint NumKeys;
	NIFuint Interpolation;
	NiColor4Key *Keys;// [NumKeys]
} NiColor4KeyGroup;

typedef struct {
	NIFuint NumKeys;
	NIFuint Interpolation;
	NiVector3Key *Keys;// [NumKeys], [(null)]
} NiVector3KeyGroup;

typedef struct {
	NIFfloat Time;
	// DUPLICATE FIELD NAME Time - fixed
	Quaternion Value;
	TBC TBC;
} QuatKey;

typedef struct {
	NIFfloat u;
	NIFfloat v;
} TexCoord;

typedef struct {
	NIFuint Source;
	NIFuint ClampMode;
	NIFuint FilterMode;
	NIFushort Flags;
	NIFshort Unknownshort;
	NIFuint UVSet;
	NIFshort PS2L;
	NIFshort PS2K;
	NIFushort Unknown1;
	NIFbyte HasTextureTransform;
	TexCoord Translation;
	TexCoord Tiling;
	NIFfloat WRotation;
	NIFuint TransformType;
	TexCoord CenterOffset;
} TexDesc;

typedef struct {
	NIFbyte IsUsed;
	TexDesc TextureData;
	NIFuint MapIndex;
} ShaderTexDesc;

typedef struct {
	NIFbyte UseExternal;
	FilePath FileName;
	NIFuint UnknownLink;
	NIFbyte UnknownByte;
	// DUPLICATE FIELD NAME FileName
	NIFuint PixelData;
} TexSource;

typedef struct {
	NIFushort v1;
	NIFushort v2;
	NIFushort v3;
} Triangle;

typedef struct {
	NIFushort NumVertices;
	NIFushort NumTriangles;
	NIFushort NumBones;
	NIFushort NumStrips;
	NIFushort NumWeightsPerVertex;
	NIFushort *Bones;// [NumBones]
	NIFbyte HasVertexMap;
	NIFushort *VertexMap;// [NumVertices]
	// DUPLICATE FIELD NAME VertexMap - fixed
	NIFbyte HasVertexWeights;
	NIFfloat *VertexWeights;// [NumVertices], [NumWeightsPerVertex]
	// DUPLICATE FIELD NAME VertexWeights - fixed
	NIFushort *StripLengths;// [NumStrips]
	NIFbyte HasFaces;
	NIFushort **Strips;// [NumStrips], [StripLengths]
	// DUPLICATE FIELD NAME Strips - fixed
	Triangle *Triangles;// [NumTriangles]
	// DUPLICATE FIELD NAME Triangles - fixed
	NIFbyte HasBoneIndices;
	NIFbyte *BoneIndices;// [NumVertices], [NumWeightsPerVertex]
} SkinPartition;

typedef struct {
	NIFuint UnknownInt;
	Vector3 Translation;
	Matrix33 Rotation;
	Vector3 Radius;
} BoundingBox;

typedef struct {
	Vector3 Offset;
	NIFushort Orientation;
	NIFbyte PositionRef1;
	NIFbyte PositionRef2;
} FurniturePosition;

typedef struct {
	Triangle Triangle;
	NIFushort WeldingInfo;
	Vector3 Normal;
} hkTriangle;

typedef struct {
	string FrameName;
	// ----------- KeyGroup, not applied here for unkn. reason
	NIFuint NumKeys;
	NIFuint Interpolation;
	NiFloatKey *Keys;// [NumKeys]
	// ---------------------------
	NIFuint UnknownInt;
	// DUPLICATE FIELD NAME UnknownInt
	Vector3 *Vectors;// [ARG]
} Morph;

typedef struct {
	Vector3 Velocity;
	Vector3 UnknownVector;
	NIFfloat Lifetime;
	NIFfloat Lifespan;
	NIFfloat Timestamp;
	NIFushort UnknownShort;
	NIFushort VertexID;
} Particle;

typedef struct {
	Matrix33 Rotation;
	Vector3 Translation;
	NIFfloat Scale;
} SkinTransform;

typedef struct {
	SkinTransform SkinTransform;
	Vector3 BoundingSphereOffset;
	NIFfloat BoundingSphereRadius;
	NIFshort Unknown13Shorts[13];
	NIFushort NumVertices;
	SkinWeight *VertexWeights;// [NumVertices]
	// DUPLICATE FIELD NAME VertexWeights - fixed
} SkinData;

typedef struct {
	Vector3 Center;
	NIFfloat Radius;
} SphereBV;

typedef struct {
	NIFbyte Layer;
	NIFbyte ColFilter;
	NIFushort UnknownShort;
} OblivionColFilter;

typedef struct {
	NIFbyte Layer;
	NIFbyte ColFilter;
	NIFushort UnknownShort;
	NIFuint NumVertices;
	NIFuint Material;
} OblivionSubShape;

typedef struct {
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFfloat UnknownFloat4;
	NIFfloat UnknownFloat5;
	NIFfloat UnknownFloat6;
	NIFbyte UnknownByte1;
} MotorDescriptor;

typedef struct {
	Vector4 PivotA;
	Vector4 PlaneA;
	Vector4 TwistA;
	Vector4 PivotB;
	Vector4 PlaneB;
	Vector4 TwistB;
	Vector4 MotorA;
	Vector4 MotorB;
	NIFfloat ConeMaxAngle;
	NIFfloat PlaneMinAngle;
	NIFfloat PlaneMaxAngle;
	NIFfloat TwistMinAngle;
	NIFfloat TwistMaxAngle;
	NIFfloat MaxFriction;
	NIFbyte EnableMotor;
	MotorDescriptor Motor;
} RagdollDescriptor;

typedef struct {
	Vector4 PivotA;
	Vector4 AxleA;
	Vector4 Perp2AxleInA1;
	Vector4 Perp2AxleInA2;
	Vector4 PivotB;
	Vector4 AxleB;
	Vector4 Perp2AxleInB2;
	Vector4 Perp2AxleInB1;
	NIFfloat MinAngle;
	NIFfloat MaxAngle;
	NIFfloat MaxFriction;
	NIFbyte EnableMotor;
	MotorDescriptor Motor;
} LimitedHingeDescriptor;

typedef struct {
	Vector4 PivotA;
	Vector4 Perp2AxleInA1;
	Vector4 Perp2AxleInA2;
	Vector4 PivotB;
	Vector4 AxleB;
	Vector4 AxleA;
	Vector4 Perp2AxleInB1;
	Vector4 Perp2AxleInB2;
} HingeDescriptor;

typedef struct {
	NIFfloat VertexWeight;
	NIFushort VertexIndex;
	Vector3 UnknownVector;
} OldSkinData;

typedef struct {
	NIFbyte HasImage;
	NIFuint Image;
	NIFuint Clamp;
	NIFuint Filter;
	NIFuint UVSet;
	NIFshort PS2L;
	NIFshort PS2K;
	NIFshort UnknownShort3;
} MultiTextureElement;

typedef struct {
	Vector3 Center;
	Vector3 Axis[3];
	NIFfloat Extent[3];
} BoxBV;

typedef struct {
	Vector3 Center;
	Vector3 Origin;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
} CapsuleBV;

typedef struct {
	Vector3 Normal;
	Vector3 Center;
} HalfSpaceBV;

typedef struct {
	NIFuint CollisionType;
	SphereBV Sphere;
	BoxBV Box;
	CapsuleBV Capsule;
	struct UnionBV *Union;
	HalfSpaceBV HalfSpace;
} BoundingVolume;

typedef struct {
	NIFuint NumBV;
	BoundingVolume *BoundingVolumes;// [NumBV]
} UnionBV;

typedef struct {
	NIFuint Interpolator;
	NIFfloat Weight;
} MorphWeight;

typedef struct {
	string TextureName;
	NIFint UnknownInt3;
	NIFint UnknownInt4;
	NIFuint TexturingProperty;
	NIFbyte UnknownBytes[9];
} ArkTexture;

typedef struct {
	NIFfloat m11;
	NIFfloat m12;
	NIFfloat m13;
	NIFfloat m14;
	NIFfloat m21;
	NIFfloat m22;
	NIFfloat m23;
	NIFfloat m24;
	NIFfloat m31;
	NIFfloat m32;
	NIFfloat m33;
	NIFfloat m34;
} InertiaMatrix;

typedef struct {
	NIFshort NumVectors;
	Vector3 *Points;// [NumVectors]
	Vector3 *Normals;// [NumVectors]
} DecalVectorArray;

typedef struct {
	NIFushort PartFlag;
	NIFushort BodyPart;
} BodyPartList;

typedef struct {
} NiObject;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1[40];
	NIFuint Parent;
	NIFuint Num1;
	NIFuint Num2;
	NIFuint *Unknown2;// [Num1], [Num2]
} Ni3dsAlphaAnimator;

typedef struct {
	NiObject *parent;
	string Name;
	NIFuint HasData;
	NIFfloat UnknownFloats1[21];
	NIFushort UnknownShort;
	NIFuint Child;
	NIFfloat UnknownFloats2[12];
	NIFuint Count;
	NIFbyte *UnknownArray;// [Count], [5]
} Ni3dsAnimationNode;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1[184];
} Ni3dsColorAnimator;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1[14];
} Ni3dsMorphShape;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1[14];
} Ni3dsParticleSystem;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1[20];
} Ni3dsPathController;

typedef struct {
	NiObject *parent;
	NIFuint NextModifier;
	NIFuint Controller;
} NiParticleModifier;

typedef struct {
	NiObject *parent;
	NIFfloat Bounce;
	NIFbyte SpawnonCollide;
	NIFbyte DieonCollide;
	NIFuint SpawnModifier;
	NIFuint Parent;
	NIFuint NextCollider;
	NIFuint ColliderObject;
} NiPSysCollider;

typedef struct {
	NiObject *parent;
} bhkRefObject;

typedef struct {
	bhkRefObject *parent;
} bhkSerializable;

typedef struct {
	bhkSerializable *parent;
	NIFuint Shape;
	NIFbyte Layer;
	NIFbyte ColFilter;
	NIFushort UnknownShort;
} bhkWorldObject;

typedef struct {
	bhkWorldObject *parent;
} bhkPhantom;

typedef struct {
	bhkPhantom *parent;
} bhkShapePhantom;

typedef struct {
	bhkShapePhantom *parent;
	NIFfloat UnkownFloats[7];
	NIFfloat UnknownFloats2[3*5];
	NIFfloat UnknownFloat;
} bhkSimpleShapePhantom;

typedef struct {
	bhkWorldObject *parent;
} bhkEntity;

typedef struct {
	bhkEntity *parent;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFint Unknown3Ints[3];
	NIFbyte CollisionResponse;
	NIFbyte UnknownByte;
	NIFushort ProcessContactCallbackDelay;
	NIFushort Unknown2Shorts[2];
	NIFbyte LayerCopy;
	NIFbyte ColFilterCopy;
	NIFushort Unknown7Shorts[7];
	Vector4 Translation;
	QuaternionXYZW Rotation;
	Vector4 LinearVelocity;
	Vector4 AngularVelocity;
	InertiaMatrix Inertia;
	Vector4 Center;
	NIFfloat Mass;
	NIFfloat LinearDamping;
	NIFfloat AngularDamping;
	NIFfloat Friction;
	NIFfloat Restitution;
	NIFfloat MaxLinearVelocity;
	NIFfloat MaxAngularVelocity;
	NIFfloat PenetrationDepth;
	NIFbyte MotionSystem;
	NIFbyte DeactivatorType;
	NIFbyte SolverDeactivation;
	NIFbyte QualityType;
	NIFuint AutoRemoveLevel;
	NIFuint UserDatasInContactPointProperties;
	NIFuint ForceCollideOntoPpu;
	NIFuint NumConstraints;
	NIFuint *Constraints;// [NumConstraints]
	NIFuint UnknownInt9;
} bhkRigidBody;

typedef struct {
	bhkRigidBody *parent;
} bhkRigidBodyT;

typedef struct {
	bhkSerializable *parent;
	NIFuint NumEntities;
	NIFuint *Entities;// [NumEntities]
	NIFuint Priority;
} bhkConstraint;

typedef struct {
	bhkConstraint *parent;
	LimitedHingeDescriptor LimitedHinge;
} bhkLimitedHingeConstraint;

typedef struct {
	bhkConstraint *parent;
	NIFuint Type;
	NIFuint UnknownInt2;
	NIFuint UnknownLink1;
	NIFuint UnknownLink2;
	NIFuint UnknownInt3;
	HingeDescriptor Hinge;
	RagdollDescriptor Ragdoll;
	LimitedHingeDescriptor LimitedHinge;
	NIFfloat Tau;
	NIFfloat Damping;
} bhkMalleableConstraint;

typedef struct {
	bhkConstraint *parent;
	Vector4 PivotA;
	Vector4 PivotB;
	NIFfloat Length;
} bhkStiffSpringConstraint;

typedef struct {
	bhkConstraint *parent;
	RagdollDescriptor Ragdoll;
} bhkRagdollConstraint;

typedef struct {
	bhkConstraint *parent;
	Vector4 PivotA;
	Vector4 RotationMatrixA[4];
	Vector4 PivotB;
	Vector4 SlidingB;
	Vector4 PlaneB;
	Vector4 SlidingA;
	Vector4 RotationA;
	Vector4 PlaneA;
	Vector4 RotationB;
	NIFfloat MinDistance;
	NIFfloat MaxDistance;
	NIFfloat Friction;
	NIFbyte UnknownByte1;
} bhkPrismaticConstraint;

typedef struct {
	bhkConstraint *parent;
	HingeDescriptor Hinge;
} bhkHingeConstraint;

typedef struct {
	bhkSerializable *parent;
} bhkShape;

typedef struct {
	bhkShape *parent;
	NIFuint Shape;
	NIFuint Material;
	NIFfloat UnknownFloat1;
	NIFbyte Unknown8Bytes[8];
	Matrix44 Transform;
} bhkTransformShape;

typedef struct {
	bhkShape *parent;
	NIFuint Material;
	NIFfloat Radius;
} bhkSphereRepShape;

typedef struct {
	bhkSphereRepShape *parent;
} bhkConvexShape;

typedef struct {
	bhkConvexShape *parent;
} bhkSphereShape;

typedef struct {
	bhkConvexShape *parent;
	NIFbyte Unknown8Bytes[8];
	Vector3 FirstPoint;
	NIFfloat Radius1;
	Vector3 SecondPoint;
	NIFfloat Radius2;
} bhkCapsuleShape;

typedef struct {
	bhkConvexShape *parent;
	NIFbyte Unknown8Bytes[8];
	Vector3 Dimensions;
	NIFfloat MinimumSize;
} bhkBoxShape;

typedef struct {
	bhkConvexShape *parent;
	NIFfloat Unknown6Floats[6];
	NIFuint NumVertices;
	Vector4 *Vertices;// [NumVertices]
	NIFuint NumNormals;
	Vector4 *Normals;// [NumNormals]
} bhkConvexVerticesShape;

typedef struct {
	bhkTransformShape *parent;
} bhkConvexTransformShape;

typedef struct {
	bhkSphereRepShape *parent;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFuint NumSpheres;
	SphereBV *Spheres;// [NumSpheres]
} bhkMultiSphereShape;

typedef struct {
	bhkShape *parent;
} bhkBvTreeShape;

typedef struct {
	bhkBvTreeShape *parent;
	NIFuint Shape;
	NIFuint Material;
	NIFuint UnknownInt1;
	NIFuint UnknownInt2;
	NIFfloat UnknownFloat;
	NIFuint MOPPDataSize;
	Vector3 Origin;
	NIFfloat Scale;
	NIFbyte *OldMOPPData;// [MOPPDataSize-1]
	NIFbyte *MOPPData;// [MOPPDataSize]
} bhkMoppBvTreeShape;

typedef struct {
	bhkShape *parent;
} bhkShapeCollection;

typedef struct {
	bhkShapeCollection *parent;
	NIFuint NumSubShapes;
	NIFuint *SubShapes;// [NumSubShapes]
	NIFuint Material;
	NIFfloat UnknownFloats[6];
	NIFuint NumUnknownInts;
	NIFuint *UnknownInts;// [NumUnknownInts]
} bhkListShape;

typedef struct {
	bhkShape *parent;
	NIFfloat Unknown1[9];
	NIFint NumUnknownFloats;
	NIFfloat *UnknownFloats;// [NumUnknownFloats], [3]
	NIFint Unknown2[3];
	NIFuint NumStripsData;
	NIFuint *StripsData;// [NumStripsData]
} bhkMeshShape;

typedef struct {
	bhkShapeCollection *parent;
	NIFushort NumSubShapes;
	OblivionSubShape *SubShapes;// [NumSubShapes]
	NIFuint UnknownInt1;
	NIFuint UnknownInt2;
	NIFfloat UnknownFloat1;
	NIFuint UnknownInt3;
	Vector3 ScaleCopy;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	Vector3 Scale;
	NIFfloat UnknownFloat4;
	NIFuint Data;
} bhkPackedNiTriStripsShape;

typedef struct {
	bhkShapeCollection *parent;
	NIFuint Material;
	NIFfloat UnknownFloat1;
	NIFuint UnknownInt1;
	NIFuint UnknownInts1[4];
	NIFuint UnknownInt2;
	Vector3 Scale;
	NIFuint UnknownInt3;
	NIFuint NumStripsData;
	NIFuint *StripsData;// [NumStripsData]
	NIFuint NumDataLayers;
	OblivionColFilter *DataLayers;// [NumDataLayers]
} bhkNiTriStripsShape;

typedef struct {
	NiObject *parent;
	string Name;
	NIFuint NextExtraData;
} NiExtraData;

typedef struct {
	NiObject *parent;
} NiInterpolator;

typedef struct {
	NiInterpolator *parent;
} NiKeyBasedInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	NIFfloat FloatValue;
	NIFuint Data;
} NiFloatInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	Vector3 Translation;
	Quaternion Rotation;
	NIFfloat Scale;
	NIFbyte UnknownBytes[3];
	NIFuint Data;
} NiTransformInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	Vector3 Point3Value;
	NIFuint Data;
} NiPoint3Interpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	NIFushort UnknownShort;
	NIFuint UnknownInt;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFushort UnknownShort2;
	NIFuint PosData;
	NIFuint FloatData;
} NiPathInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	NIFbyte BoolValue;
	NIFuint Data;
} NiBoolInterpolator;

typedef struct {
	NiBoolInterpolator *parent;
} NiBoolTimelineInterpolator;

typedef struct {
	NiInterpolator *parent;
	NIFushort UnknownShort;
	NIFuint UnknownInt;
} NiBlendInterpolator;

typedef struct {
	NiInterpolator *parent;
	NIFfloat StartTime;
	NIFfloat StopTime;
	NIFuint SplineData;
	NIFuint BasisData;
} NiBSplineInterpolator;

typedef struct {
	NiObject *parent;
	string Name;
	NIFuint HasOldExtraData;
	string OldExtraPropName;
	NIFuint OldExtraInternalId;
	string OldExtraString;
	NIFbyte UnknownByte;
	NIFuint ExtraData;
	NIFuint NumExtraDataList;
	NIFuint *ExtraDataList;// [NumExtraDataList]
	NIFuint Controller;
} NiObjectNET;

typedef struct {
	NiObject *parent;
	NIFuint Target;
} NiCollisionObject;

typedef struct {
	NiCollisionObject *parent;
	NIFuint PropagationMode;
	NIFuint CollisionMode;
	NIFbyte UseABV;
	BoundingVolume BoundingVolume;
} NiCollisionData;

typedef struct {
	NiCollisionObject *parent;
	NIFushort Flags;
	NIFuint Body;
} bhkNiCollisionObject;

typedef struct {
	bhkNiCollisionObject *parent;
} bhkCollisionObject;

typedef struct {
	bhkCollisionObject *parent;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
} bhkBlendCollisionObject;

typedef struct {
	bhkNiCollisionObject *parent;
} bhkPCollisionObject;

typedef struct {
	bhkPCollisionObject *parent;
} bhkSPCollisionObject;

typedef struct {
	NiObjectNET *parent;
	NIFushort Flags;
	NIFushort UnknownShort1;
	Vector3 Translation;
	Matrix33 Rotation;
	NIFfloat Scale;
	Vector3 Velocity;
	NIFuint NumProperties;
	NIFuint *Properties;// [NumProperties]
	NIFuint Unknown1[4];
	NIFbyte Unknown2;
	NIFbyte HasBoundingBox;
	BoundingBox BoundingBox;
	NIFuint CollisionObject;
} NiAVObject;

typedef struct {
	NiAVObject *parent;
	NIFbyte SwitchState;
	NIFuint NumAffectedNodeListPointers;
	NIFuint NumAffectedNodes;
	NIFuint *AffectedNodeListPointers;// [NumAffectedNodeListPointers]
	NIFuint *AffectedNodes;// [NumAffectedNodes]
} NiDynamicEffect;

typedef struct {
	NiDynamicEffect *parent;
	NIFfloat Dimmer;
	Color3 AmbientColor;
	Color3 DiffuseColor;
	Color3 SpecularColor;
} NiLight;

typedef struct {
	NiObjectNET *parent;
} NiProperty;

typedef struct {
	NiProperty *parent;
	NIFbyte Unknown[6];
} NiTransparentProperty;

typedef struct {
	NiObject *parent;
	string Name;
	NIFuint Order;
	NIFuint Target;
	NIFbyte Active;
} NiPSysModifier;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat Speed;
	NIFfloat SpeedVariation;
	NIFfloat Declination;
	NIFfloat DeclinationVariation;
	NIFfloat PlanarAngle;
	NIFfloat PlanarAngleVariation;
	Color4 InitialColor;
	NIFfloat InitialRadius;
	NIFfloat RadiusVariation;
	NIFfloat LifeSpan;
	NIFfloat LifeSpanVariation;
} NiPSysEmitter;

typedef struct {
	NiPSysEmitter *parent;
	NIFuint EmitterObject;
} NiPSysVolumeEmitter;

typedef struct {
	NiObject *parent;
	NIFuint NextController;
	NIFushort Flags;
	NIFfloat Frequency;
	NIFfloat Phase;
	NIFfloat StartTime;
	NIFfloat StopTime;
	NIFuint Target;
	NIFuint UnknownInteger;
} NiTimeController;

typedef struct {
	NiTimeController *parent;
} NiInterpController;

typedef struct {
	NiInterpController *parent;
	NIFushort NumExtraTargets;
	NIFuint *ExtraTargets;// [NumExtraTargets]
} NiMultiTargetTransformController;

typedef struct {
	NiInterpController *parent;
	NIFushort ExtraFlags;
	NIFbyte Unknown2;
	NIFuint Data;
	NIFbyte AlwaysUpdate;
	NIFuint NumInterpolators;
	NIFuint *Interpolators;// [NumInterpolators]
	MorphWeight *InterpolatorWeights;// [NumInterpolators]
	NIFuint NumUnknownInts;
	NIFuint *UnknownInts;// [NumUnknownInts]
} NiGeomMorpherController;

typedef struct {
	NiInterpController *parent;
} NiMorphController;

typedef struct {
	NiInterpController *parent;
	NIFuint Data;
} NiMorpherController;

typedef struct {
	NiInterpController *parent;
	NIFuint Interpolator;
} NiSingleInterpController;

typedef struct {
	NiSingleInterpController *parent;
	NIFuint Data;
} NiKeyframeController;

typedef struct {
	NiKeyframeController *parent;
} NiTransformController;

typedef struct {
	NiSingleInterpController *parent;
	string ModifierName;
} NiPSysModifierCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
	NIFuint Data;
	NIFuint VisibilityInterpolator;
} NiPSysEmitterCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSysModifierBoolCtlr;

typedef struct {
	NiPSysModifierBoolCtlr *parent;
	NIFuint Data;
} NiPSysModifierActiveCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
	NIFuint Data;
} NiPSysModifierFloatCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterDeclinationCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterDeclinationVarCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterInitialRadiusCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterLifeSpanCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterSpeedCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysGravityStrengthCtlr;

typedef struct {
	NiSingleInterpController *parent;
} NiFloatInterpController;

typedef struct {
	NiFloatInterpController *parent;
	NIFuint TextureSlot;
	NIFuint UnknownInt2;
	NIFfloat Delta;
	NIFuint NumSources;
	NIFuint *Sources;// [NumSources]
	NIFuint *Images;// [NumSources]
} NiFlipController;

typedef struct {
	NiFloatInterpController *parent;
	NIFuint Data;
} NiAlphaController;

typedef struct {
	NiFloatInterpController *parent;
	NIFbyte Unknown2;
	NIFuint TextureSlot;
	NIFuint Operation;
	NIFuint Data;
} NiTextureTransformController;

typedef struct {
	NiFloatInterpController *parent;
} NiLightDimmerController;

typedef struct {
	NiSingleInterpController *parent;
} NiBoolInterpController;

typedef struct {
	NiBoolInterpController *parent;
	NIFuint Data;
} NiVisController;

typedef struct {
	NiSingleInterpController *parent;
	NIFushort TargetColor;
	NIFuint Data;
} NiPoint3InterpController;

typedef struct {
	NiPoint3InterpController *parent;
} NiMaterialColorController;

typedef struct {
	NiPoint3InterpController *parent;
} NiLightColorController;

typedef struct {
	NiSingleInterpController *parent;
} NiExtraDataController;

typedef struct {
	NiExtraDataController *parent;
	string ControllerData;
	NIFbyte NumExtraBytes;
	NIFbyte UnknownBytes[7];
	NIFbyte *UnknownExtraBytes;// [NumExtraBytes]
} NiFloatExtraDataController;

typedef struct {
	NiTimeController *parent;
	NIFuint UnknownInt1;
	NIFuint NumNodeGroups;
	NIFuint NumNodeGroups2;
	NodeGroup *NodeGroups;// [NumNodeGroups]
	NIFuint NumShapeGroups;
	// DUPLICATE FIELD NAME NumShapeGroups
	SkinShapeGroup *ShapeGroups1;// [NumShapeGroups]
	// DUPLICATE FIELD NAME ShapeGroups1
	NIFuint NumShapeGroups2;
	// DUPLICATE FIELD NAME NumShapeGroups2
	NIFuint *ShapeGroups2;// [NumShapeGroups2]
	// DUPLICATE FIELD NAME ShapeGroups2
	NIFint UnknownInt2;
	NIFint UnknownInt3;
} NiBoneLODController;

typedef struct {
	NiBoneLODController *parent;
} NiBSBoneLODController;

typedef struct {
	NiAVObject *parent;
	NIFuint Data;
	NIFuint SkinInstance;
	NIFuint NumMaterials;
	string *MaterialName;// [NumMaterials]
	NIFint *MaterialExtraData;// [NumMaterials]
	NIFint ActiveMaterial;
	NIFbyte HasShader;
	string ShaderName;
	NIFint UnknownInteger;
	NIFbyte UnknownByte;
	NIFint UnknownInteger2;
	NIFbyte DirtyFlag;
} NiGeometry;

typedef struct {
	NiGeometry *parent;
} NiTriBasedGeom;

typedef struct {
	NiObject *parent;
	NIFint UnknownInt;
	NIFushort NumVertices;
	NIFbyte KeepFlags;
	NIFbyte CompressFlags;
	NIFbyte HasVertices;
	Vector3 *Vertices;// [NumVertices]
	NIFushort NumUVSets;
	NIFushort BSNumUVSets;
	NIFbyte HasNormals;
	Vector3 *Normals;// [NumVertices]
	Vector3 *Tangents;// [NumVertices]
	Vector3 *Binormals;// [NumVertices]
	Vector3 Center;
	NIFfloat Radius;
	NIFshort Unknown13shorts[13];
	NIFbyte HasVertexColors;
	Color4 *VertexColors;// [NumVertices]
	// DUPLICATE FIELD NAME NumUVSets - fixed
	NIFbyte HasUV;
	TexCoord *UVSets;// [NumUVSets & 63] OR [NumUVSets & 1], [Num_Vertices]
	// DUPLICATE FIELD NAME UVSets - fixed
	NIFushort ConsistencyFlags;
	NIFuint AdditionalData;
} NiGeometryData;

typedef struct {
	NiGeometryData *parent;
	NIFushort NumTriangles;
} NiTriBasedGeomData;

typedef struct {
	NiTimeController *parent;
	NIFuint UnknownInt;
} bhkBlendController;

typedef struct {
	NiExtraData *parent;
	Vector3 Center;
	Vector3 Dimensions;
} BSBound;

typedef struct {
	NiExtraData *parent;
	NIFuint NumPositions;
	FurniturePosition *Positions;// [NumPositions]
} BSFurnitureMarker;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat Damping;
} BSParentVelocityModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
} BSPSysArrayEmitter;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat Strength;
} BSWindModifier;

typedef struct {
	bhkShapeCollection *parent;
	NIFuint NumTriangles;
	hkTriangle *Triangles;// [NumTriangles]
	NIFuint NumVertices;
	NIFbyte UnknownByte1;
	Vector3 *Vertices;// [NumVertices]
	NIFushort NumSubShapes;
	OblivionSubShape *SubShapes;// [NumSubShapes]
} hkPackedNiTriStripsData;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFbyte Threshold;
} NiAlphaProperty;

typedef struct {
	NiLight *parent;
} NiAmbientLight;

typedef struct {
	NiObject *parent;
	string Name;
	NIFushort NumVertices;
	NIFbyte KeepFlags;
	NIFbyte CompressFlags;
	NIFbyte HasVertices;
	Vector3 *Vertices;// [NumVertices]
	NIFbyte NumUVSets;
	NIFbyte TSpaceFlag;
	NIFbyte HasNormals;
	Vector3 *Normals;// [NumVertices]
	Vector3 *Tangents;// [NumVertices]
	Vector3 *Binormals;// [NumVertices]
	Vector3 Center;
	NIFfloat Radius;
	NIFbyte HasVertexColors;
	Color4 *VertexColors;// [NumVertices]
	// DUPLICATE FIELD NAME NumUVSets - fixed
	// DUPLICATE FIELD NAME TSpaceFlag - fixed
	NIFbyte HasUV;
	TexCoord *UVSets;// [NumUVSets & 63], [NumVertices]
	NIFushort ConsistencyFlags;
	NIFuint AdditionalData;
	NIFushort NumParticles;
	NIFfloat ParticleRadius;
	NIFbyte HasRadii;
	NIFfloat *Radii;// [NumVertices]
	NIFushort NumActive;
	NIFbyte HasSizes;
	NIFfloat *Sizes;// [NumVertices]
	NIFbyte HasRotations;
	Quaternion *Rotations;// [NumVertices]
	NIFbyte HasRotationAngles;
	NIFfloat *RotationAngles;// [NumVertices]
	NIFbyte HasRotationAxes;
	Vector3 *RotationAxes;// [NumVertices]
	NIFbyte HasUnknownStuff1;
	NIFshort NumUnknownStuff1;
	Vector4 *UnknownStuff1;// [NumUnknownStuff1]
} NiParticlesData;

typedef struct {
	NiParticlesData *parent;
	NIFbyte HasRotations2;
	Quaternion *Rotations2;// [parent->NumVertices]
} NiRotatingParticlesData;

typedef struct {
	NiParticlesData *parent;
} NiAutoNormalParticlesData;

typedef struct {
	Vector3 Translation;
	NIFfloat UnknownFloats1[3];
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFint UnknownInt1;
} ParticleDesc;

typedef struct {
	NiRotatingParticlesData *parent;
	ParticleDesc *ParticleDescriptions;// [parent->NumVertices]
	NIFbyte HasUnknownFloats3;
	NIFfloat *UnknownFloats3;// [parent->NumVertices]
	NIFushort UnknownShort1;
	NIFushort UnknownShort2;
} NiPSysData;

typedef struct {
	NiPSysData *parent;
	NIFuint UnknownInt2;
	NIFbyte UnknownByte3;
	NIFuint NumUnknownInts1;
	NIFuint *UnknownInts1;// [NumUnknownInts1]
	NIFuint UnknownNode;
} NiMeshPSysData;

typedef struct {
	NiExtraData *parent;
	ByteArray BinaryData;
} NiBinaryExtraData;

typedef struct {
	NiExtraData *parent;
	NIFuint UnknownInt;
	NIFuint Data;
} NiBinaryVoxelExtraData;

typedef struct {
	NiObject *parent;
	NIFushort UnknownShort1;
	NIFushort UnknownShort2;
	NIFushort UnknownShort3;
	NIFfloat Unknown7Floats[7];
	NIFbyte UnknownBytes1[7*12];
	NIFuint NumUnknownVectors;
	Vector4 *UnknownVectors;// [NumUnknownVectors]
	NIFuint NumUnknownBytes2;
	NIFbyte *UnknownBytes2;// [NumUnknownBytes2]
	NIFuint Unknown5Ints[5];
} NiBinaryVoxelData;

typedef struct {
	NiBlendInterpolator *parent;
	NIFbyte BoolValue;
} NiBlendBoolInterpolator;

typedef struct {
	NiBlendInterpolator *parent;
	NIFfloat FloatValue;
} NiBlendFloatInterpolator;

typedef struct {
	NiBlendInterpolator *parent;
	Vector3 PointValue;
} NiBlendPoint3Interpolator;

typedef struct {
	NiBlendInterpolator *parent;
} NiBlendTransformInterpolator;

typedef struct {
	NiObject *parent;
	NiByteKeyGroup Data;
} NiBoolData;

typedef struct {
	NiExtraData *parent;
	NIFbyte BooleanData;
} NiBooleanExtraData;

typedef struct {
	NiObject *parent;
	NIFuint NumControlPoints;
} NiBSplineBasisData;

typedef struct {
	NiBSplineInterpolator *parent;
} NiBSplineFloatInterpolator;

typedef struct {
	NiBSplineFloatInterpolator *parent;
	NIFfloat Base;
	NIFuint Offset;
	NIFfloat Bias;
	NIFfloat Multiplier;
} NiBSplineCompFloatInterpolator;

typedef struct {
	NiBSplineInterpolator *parent;
	NIFfloat UnknownFloats[6];
} NiBSplinePoint3Interpolator;

typedef struct {
	NiBSplinePoint3Interpolator *parent;
} NiBSplineCompPoint3Interpolator;

typedef struct {
	NiBSplineInterpolator *parent;
	Vector3 Translation;
	Quaternion Rotation;
	NIFfloat Scale;
	NIFuint TranslationOffset;
	NIFuint RotationOffset;
	NIFuint ScaleOffset;
} NiBSplineTransformInterpolator;

typedef struct {
	NiBSplineTransformInterpolator *parent;
	NIFfloat TranslationBias;
	NIFfloat TranslationMultiplier;
	NIFfloat RotationBias;
	NIFfloat RotationMultiplier;
	NIFfloat ScaleBias;
	NIFfloat ScaleMultiplier;
} NiBSplineCompTransformInterpolator;

typedef struct {
	NiTransformInterpolator *parent;
} BSRotAccumTransfInterpolator;

typedef struct {
	NiObject *parent;
	NIFuint NumFloatControlPoints;
	NIFfloat *FloatControlPoints;// [NumFloatControlPoints]
	NIFuint NumShortControlPoints;
	NIFshort *ShortControlPoints;// [NumShortControlPoints]
} NiBSplineData;

typedef struct {
	NiAVObject *parent;
	NIFushort UnknownShort;
	NIFfloat FrustumLeft;
	NIFfloat FrustumRight;
	NIFfloat FrustumTop;
	NIFfloat FrustumBottom;
	NIFfloat FrustumNear;
	NIFfloat FrustumFar;
	NIFbyte UseOrthographicProjection;
	NIFfloat ViewportLeft;
	NIFfloat ViewportRight;
	NIFfloat ViewportTop;
	NIFfloat ViewportBottom;
	NIFfloat LODAdjust;
	NIFuint UnknownLink;
	NIFuint UnknownInt;
	NIFuint UnknownInt2;
	NIFuint UnknownInt3;
} NiCamera;

typedef struct {
	NiObject *parent;
	NiColor4KeyGroup Data;
} NiColorData;

typedef struct {
	NiExtraData *parent;
	Color4 Data;
} NiColorExtraData;

typedef struct {
	NiTimeController *parent;
	NIFbyte Cumulative;
	NIFuint NumControllerSequences;
	NIFuint *ControllerSequences;// [NumControllerSequences]
	NIFuint ObjectPalette;
} NiControllerManager;

typedef struct {
	NiObject *parent;
	string Name;
	string TextKeysName;
	NIFuint TextKeys;
	NIFint UnknownInt4;
	NIFint UnknownInt5;
	NIFuint NumControlledBlocks;
	NIFuint UnknownInt1;
	ControllerLink *ControlledBlocks;// [NumControlledBlocks]
} NiSequence;

typedef struct {
	NiSequence *parent;
	NIFfloat Weight;
	NIFuint TextKeys;
	NIFuint CycleType;
	NIFuint UnknownInt0;
	NIFfloat Frequency;
	NIFfloat StartTime;
	NIFfloat UnknownFloat2;
	NIFfloat StopTime;
	NIFbyte UnknownByte;
	NIFuint Manager;
	string TargetName;
	NIFuint StringPalette;
	NIFshort UnknownShort1;
	NIFshort UnknownShort2;
	NIFuint UnknownInt3;
} NiControllerSequence;

typedef struct {
	NiObject *parent;
} NiAVObjectPalette;

typedef struct {
	NiAVObjectPalette *parent;
	NIFuint UnknownInt;
	NIFuint NumObjs;
	AVObject *Objs;// [NumObjs]
} NiDefaultAVObjectPalette;

typedef struct {
	NiLight *parent;
} NiDirectionalLight;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
} NiDitherProperty;

typedef struct {
	NiSingleInterpController *parent;
	NIFuint Data;
} NiRollController;

typedef struct {
	NiObject *parent;
	NiFloatKeyGroup Data;
} NiFloatData;

typedef struct {
	NiExtraData *parent;
	NIFfloat FloatData;
} NiFloatExtraData;

typedef struct {
	NiExtraData *parent;
	NIFuint NumFloats;
	NIFfloat *Data;// [NumFloats]
} NiFloatsExtraData;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFfloat FogDepth;
	Color3 FogColor;
} NiFogProperty;

typedef struct {
	NiParticleModifier *parent;
	NIFfloat UnknownFloat1;
	NIFfloat Force;
	NIFuint Type;
	Vector3 Position;
	Vector3 Direction;
} NiGravity;

typedef struct {
	NiExtraData *parent;
	NIFuint IntegerData;
} NiIntegerExtraData;

typedef struct {
	NiIntegerExtraData *parent;
} BSXFlags;

typedef struct {
	NiExtraData *parent;
	NIFuint NumIntegers;
	NIFuint *Data;// [NumIntegers]
} NiIntegersExtraData;

typedef struct {
	NiKeyframeController *parent;
	NIFuint Data2;
} BSKeyframeController;

typedef struct {
	NiObject *parent;
	NIFuint NumRotationKeys;
	NIFuint RotationType;
	QuatKey *QuaternionKeys;// [NumRotationKeys]
	NIFfloat UnknownFloat;
	NiFloatKeyGroup XYZRotations[3];
	NiVector3KeyGroup Translations;
	NiFloatKeyGroup Scales;
} NiKeyframeData;

typedef struct {
	NiTimeController *parent;
	NIFushort Unknown1;
	NIFuint LookAtNode;
} NiLookAtController;

typedef struct {
	NiInterpolator *parent;
	NIFushort UnknownShort;
	NIFuint LookAt;
	string Target;
	Vector3 Translation;
	Quaternion Rotation;
	NIFfloat Scale;
	NIFuint UnknownLink1;
	NIFuint UnknownLink2;
	NIFuint UnknownLink3;
} NiLookAtInterpolator;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	Color3 AmbientColor;
	Color3 DiffuseColor;
	Color3 SpecularColor;
	Color3 EmissiveColor;
	NIFfloat Glossiness;
	NIFfloat Alpha;
	NIFfloat EmitMulti;
} NiMaterialProperty;

typedef struct {
	NiObject *parent;
	NIFuint NumMorphs;
	NIFuint NumVertices;
	NIFbyte RelativeTargets;
	Morph *Morphs;// [NumMorphs]
} NiMorphData;

typedef struct {
	NiAVObject *parent;
	NIFuint NumChildren;
	NIFuint *Children;// [NumChildren]
	NIFuint NumEffects;
	NIFuint *Effects;// [NumEffects]
} NiNode;

typedef struct {
	NiNode *parent;
} NiBone;

typedef struct {
	NiNode *parent;
} AvoidNode;

typedef struct {
	NiNode *parent;
	NIFbyte Unknown1;
	NIFbyte Unknown292Bytes[292];
} FxWidget;

typedef struct {
	FxWidget *parent;
} FxButton;

typedef struct {
	FxWidget *parent;
	NIFuint UnknownInt1;
	NIFuint UnknownInt2;
	NIFuint UnknownInt3;
	NIFuint NumButtons;
	NIFuint *Buttons;// [NumButtons]
} FxRadioButton;

typedef struct {
	NiNode *parent;
	NIFushort BillboardMode;
} NiBillboardNode;

typedef struct {
	NiNode *parent;
} NiBSAnimationNode;

typedef struct {
	NiNode *parent;
} NiBSParticleNode;

typedef struct {
	NiNode *parent;
	NIFushort UnknownFlags1;
	NIFint UnknownInt1;
} NiSwitchNode;

typedef struct {
	NiSwitchNode *parent;
	Vector3 LODCenter;
	NIFuint NumLODLevels;
	LODRange *LODLevels;// [NumLODLevels]
	NIFuint LODLevelData;
} NiLODNode;

typedef struct {
	NiObject *parent;
	NIFbyte UnknownByte;
	NIFuint NumEntries;
	ByteColor4 Palette[256];
} NiPalette;

typedef struct {
	NiParticleModifier *parent;
	NIFfloat Decay;
	NIFfloat Duration;
	NIFfloat DeltaV;
	NIFfloat Start;
	NIFuint DecayType;
	NIFuint SymmetryType;
	Vector3 Position;
	Vector3 Direction;
} NiParticleBomb;

typedef struct {
	NiParticleModifier *parent;
	NIFuint ColorData;
} NiParticleColorModifier;

typedef struct {
	NiParticleModifier *parent;
	NIFfloat Grow;
	NIFfloat Fade;
} NiParticleGrowFade;

typedef struct {
	NiParticleModifier *parent;
	NIFuint NumParticleMeshes;
	NIFuint *ParticleMeshes;// [NumParticleMeshes]
} NiParticleMeshModifier;

typedef struct {
	NiParticleModifier *parent;
	NIFbyte RandomInitialAxis;
	Vector3 InitialAxis;
	NIFfloat RotationSpeed;
} NiParticleRotation;

typedef struct {
	NiGeometry *parent;
} NiParticles;

typedef struct {
	NiParticles *parent;
} NiAutoNormalParticles;

typedef struct {
	NiParticles *parent;
} NiParticleMeshes;

typedef struct {
	NiRotatingParticlesData *parent;
	NIFuint UnknownLink2;
} NiParticleMeshesData;

typedef struct {
	NiParticles *parent;
	NIFbyte WorldSpace;
	NIFuint NumModifiers;
	NIFuint *Modifiers;// [NumModifiers]
} NiParticleSystem;

typedef struct {
	NiParticleSystem *parent;
} NiMeshParticleSystem;

typedef struct {
	NiTimeController *parent;
	NIFuint OldSpeed;
	NIFfloat Speed;
	NIFfloat SpeedRandom;
	NIFfloat VerticalDirection;
	NIFfloat VerticalAngle;
	NIFfloat HorizontalDirection;
	NIFfloat HorizontalAngle;
	Vector3 UnknownNormal;
	Color4 UnknownColor;
	NIFfloat Size;
	NIFfloat EmitStartTime;
	NIFfloat EmitStopTime;
	NIFbyte UnknownByte;
	NIFuint OldEmitRate;
	NIFfloat EmitRate;
	NIFfloat Lifetime;
	NIFfloat LifetimeRandom;
	NIFushort EmitFlags;
	Vector3 StartRandom;
	NIFuint Emitter;
	NIFushort UnknownShort2;
	NIFfloat UnknownFloat13;
	NIFuint UnknownInt1;
	NIFuint UnknownInt2;
	NIFushort UnknownShort3;
	Vector3 ParticleVelocity;
	Vector3 ParticleUnknownVector;
	NIFfloat ParticleLifetime;
	NIFuint ParticleLink;
	NIFuint ParticleTimestamp;
	NIFushort ParticleUnknownShort;
	NIFushort ParticleVertexId;
	NIFushort NumParticles;
	NIFushort NumValid;
	Particle *Particles;// [NumParticles]
	NIFuint UnknownLink;
	NIFuint ParticleExtra;
	NIFuint UnknownLink2;
	NIFbyte Trailer;
	NIFuint ColorData;
	NIFfloat UnknownFloat1;
	NIFfloat *UnknownFloats2;// [ParticleUnknownShort]
} NiParticleSystemController;

typedef struct {
	NiParticleSystemController *parent;
} NiBSPArrayController;

typedef struct {
	NiTimeController *parent;
	NIFushort UnknownShort2;
	NIFuint UnknownInt1;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFushort UnknownShort;
	NIFuint PosData;
	NIFuint FloatData;
} NiPathController;

typedef struct {
	NIFuint Type;
	NIFuint Convention;
	NIFbyte BitsPerChannel;
	NIFbyte UnknownByte1;
} ChannelData;

typedef struct {
	NiObject *parent;
	NIFuint PixelFormat;
	NIFuint RedMask;
	NIFuint GreenMask;
	NIFuint BlueMask;
	NIFuint AlphaMask;
	NIFbyte BitsPerPixel;
	NIFbyte Unknown3Bytes[3];
	NIFbyte Unknown8Bytes[8];
	NIFuint UnknownInt;
	// DUPLICATE FIELD NAME BitsPerPixel - fixed
	NIFint UnknownInt2;
	NIFuint UnknownInt3;
	NIFbyte Flags;
	NIFuint UnknownInt4;
	NIFbyte UnknownByte1;
	ChannelData Channels[4];
	NIFuint Palette;
	NIFuint NumMipmaps;
	NIFuint BytesPerPixel;
	MipMap *Mipmaps;// [NumMipmaps]
} ATextureRenderData;

typedef struct {
	ATextureRenderData *parent;
	NIFuint NumPixels;
	NIFuint UnknownInt6;
	NIFuint NumFaces;
	NIFuint UnknownInt7;
	NIFbyte *PixelData;// [NumFaces], [NumPixels]
} NiPersistentSrcTextureRendererData;

typedef struct {
	ATextureRenderData *parent;
	NIFuint NumPixels;
	NIFuint NumFaces;
	NIFbyte *PixelData;// [NumFaces], [NumPixels]
	// DUPLICATE FIELD NAME PixelData - fixed
} NiPixelData;

typedef struct {
	NiParticleModifier *parent;
	NIFushort UnknownShort;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFushort UnknownShort2;
	NIFfloat UnknownFloat3;
	NIFfloat UnknownFloat4;
	NIFfloat UnknownFloat5;
	NIFfloat UnknownFloat6;
	NIFfloat UnknownFloat7;
	NIFfloat UnknownFloat8;
	NIFfloat UnknownFloat9;
	NIFfloat UnknownFloat10;
	NIFfloat UnknownFloat11;
	NIFfloat UnknownFloat12;
	NIFfloat UnknownFloat13;
	NIFfloat UnknownFloat14;
	NIFfloat UnknownFloat15;
	NIFfloat UnknownFloat16;
} NiPlanarCollider;

typedef struct {
	NiLight *parent;
	NIFfloat ConstantAttenuation;
	NIFfloat LinearAttenuation;
	NIFfloat QuadraticAttenuation;
} NiPointLight;

typedef struct {
	NiObject *parent;
	NiVector3KeyGroup Data;
} NiPosData;

typedef struct {
	NiPSysModifier *parent;
	NIFbyte SpawnonDeath;
	NIFuint SpawnModifier;
} NiPSysAgeDeathModifier;

typedef struct {
	NiPSysModifier *parent;
	NIFuint BombObject;
	Vector3 BombAxis;
	NIFfloat Decay;
	NIFfloat DeltaV;
	NIFuint DecayType;
	NIFuint SymmetryType;
} NiPSysBombModifier;

typedef struct {
	NiPSysModifier *parent;
	NIFushort UpdateSkip;
} NiPSysBoundUpdateModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
	NIFfloat Width;
	NIFfloat Height;
	NIFfloat Depth;
} NiPSysBoxEmitter;

typedef struct {
	NiPSysModifier *parent;
	NIFuint Collider;
} NiPSysColliderManager;

typedef struct {
	NiPSysModifier *parent;
	NIFuint Data;
} NiPSysColorModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
	NIFfloat Radius;
	NIFfloat Height;
} NiPSysCylinderEmitter;

typedef struct {
	NiPSysModifier *parent;
	NIFuint Parent;
	Vector3 DragAxis;
	NIFfloat Percentage;
	NIFfloat Range;
	NIFfloat RangeFalloff;
} NiPSysDragModifier;

typedef struct {
	NiObject *parent;
	NiFloatKeyGroup FloatKeys;
	NIFuint NumVisibilityKeys;
	NiByteKey *VisibilityKeys;// [NumVisibilityKeys]
} NiPSysEmitterCtlrData;

typedef struct {
	NiPSysModifier *parent;
	NIFuint GravityObject;
	Vector3 GravityAxis;
	NIFfloat Decay;
	NIFfloat Strength;
	NIFuint ForceType;
	NIFfloat Turbulence;
	NIFfloat TurbulenceScale;
	NIFbyte UnknownByte;
} NiPSysGravityModifier;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat GrowTime;
	NIFushort GrowGeneration;
	NIFfloat FadeTime;
	NIFushort FadeGeneration;
	NIFfloat BaseScale;
} NiPSysGrowFadeModifier;

typedef struct {
	NiPSysEmitter *parent;
	NIFuint NumEmitterMeshes;
	NIFuint *EmitterMeshes;// [NumEmitterMeshes]
	NIFuint InitialVelocityType;
	NIFuint EmissionType;
	Vector3 EmissionAxis;
} NiPSysMeshEmitter;

typedef struct {
	NiPSysModifier *parent;
	NIFuint NumMeshes;
	NIFuint *Meshes;// [NumMeshes]
} NiPSysMeshUpdateModifier;

typedef struct {
	NiPSysCollider *parent;
	NIFfloat Width;
	NIFfloat Height;
	Vector3 XAxis;
	Vector3 YAxis;
} NiPSysPlanarCollider;

typedef struct {
	NiPSysCollider *parent;
	NIFfloat Radius;
} NiPSysSphericalCollider;

typedef struct {
	NiPSysModifier *parent;
} NiPSysPositionModifier;

typedef struct {
	NiTimeController *parent;
} NiPSysResetOnLoopCtlr;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat InitialRotationSpeed;
	NIFfloat InitialRotationSpeedVariation;
	NIFfloat InitialRotationAngle;
	NIFfloat InitialRotationAngleVariation;
	NIFbyte RandomRotSpeedSign;
	NIFbyte RandomInitialAxis;
	Vector3 InitialAxis;
} NiPSysRotationModifier;

typedef struct {
	NiPSysModifier *parent;
	NIFushort NumSpawnGenerations;
	NIFfloat PercentageSpawned;
	NIFushort MinNumtoSpawn;
	NIFushort MaxNumtoSpawn;
	NIFfloat SpawnSpeedChaos;
	NIFfloat SpawnDirChaos;
	NIFfloat LifeSpan;
	NIFfloat LifeSpanVariation;
	NIFint Unknownint;
} NiPSysSpawnModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
	NIFfloat Radius;
} NiPSysSphereEmitter;

typedef struct {
	NiTimeController *parent;
} NiPSysUpdateCtlr;

typedef struct {
	NiPSysModifier *parent;
	NIFuint FieldObject;
	NIFfloat Magnitude;
	NIFfloat Attenuation;
	NIFbyte UseMaxDistance;
	NIFfloat MaxDistance;
} NiPSysFieldModifier;

typedef struct {
	NiPSysFieldModifier *parent;
	Vector3 Direction;
} NiPSysVortexFieldModifier;

typedef struct {
	NiPSysFieldModifier *parent;
	Vector3 Direction;
} NiPSysGravityFieldModifier;

typedef struct {
	NiPSysFieldModifier *parent;
	NIFbyte UseDirection;
	Vector3 Direction;
} NiPSysDragFieldModifier;

typedef struct {
	NiPSysFieldModifier *parent;
	NIFfloat Frequency;
} NiPSysTurbulenceFieldModifier;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysFieldMagnitudeCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysFieldAttenuationCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysFieldMaxDistanceCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysAirFieldAirFrictionCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysAirFieldInheritVelocityCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysAirFieldSpreadCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysInitialRotSpeedCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysInitialRotSpeedVarCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysInitialRotAngleCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysInitialRotAngleVarCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterPlanarAngleCtlr;

typedef struct {
	NiPSysModifierFloatCtlr *parent;
} NiPSysEmitterPlanarAngleVarCtlr;

typedef struct {
	NiPSysFieldModifier *parent;
	Vector3 Direction;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFbyte UnknownBoolean1;
	NIFbyte UnknownBoolean2;
	NIFbyte UnknownBoolean3;
	NIFfloat UnknownFloat4;
} NiPSysAirFieldModifier;

typedef struct {
	NiPSysEmitter *parent;
	NIFint UnknownInt1;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFint UnknownInt2;
	NIFfloat UnknownFloat4;
	NIFint UnknownInt3;
	NIFfloat UnknownFloat5;
	NIFint UnknownInt4;
	NIFfloat UnknownFloat6;
	NIFfloat UnknownFloat7;
} NiPSysTrailEmitter;

typedef struct {
	NiFloatInterpController *parent;
} NiLightIntensityController;

typedef struct {
	NiPSysFieldModifier *parent;
	NIFint RadialType;
} NiPSysRadialFieldModifier;

typedef struct {
	NiObject *parent;
} NiLODData;

typedef struct {
	NiLODData *parent;
	Vector3 LODCenter;
	NIFuint NumLODLevels;
	LODRange *LODLevels;// [NumLODLevels]
} NiRangeLODData;

typedef struct {
	NiLODData *parent;
	Vector3 BoundCenter;
	NIFfloat BoundRadius;
	Vector3 WorldCenter;
	NIFfloat WorldRadius;
	NIFuint ProportionCount;
	NIFfloat *ProportionLevels;// [ProportionCount]
} NiScreenLODData;

typedef struct {
	NiParticles *parent;
} NiRotatingParticles;

typedef struct {
	NiObjectNET *parent;
} NiSequenceStreamHelper;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
} NiShadeProperty;

typedef struct {
	NiObject *parent;
	SkinTransform SkinTransform;
	NIFuint NumBones;
	NIFuint SkinPartition;
	NIFbyte HasVertexWeights;
	SkinData *BoneList;// [NumBones]
} NiSkinData;

typedef struct {
	NiObject *parent;
	NIFuint Data;
	NIFuint SkinPartition;
	NIFuint SkeletonRoot;
	NIFuint NumBones;
	NIFuint *Bones;// [NumBones]
} NiSkinInstance;

typedef struct {
	NiTimeController *parent;
	NIFuint NumBones;
	NIFuint *VertexCounts;// [NumBones]
	NIFuint *Bones;// [NumBones]
	OldSkinData **BoneData;// [NumBones], [VertexCounts]
} NiTriShapeSkinController;

typedef struct {
	NiSkinInstance *parent;
} NiClodSkinInstance;

typedef struct {
	NiObject *parent;
	NIFuint NumSkinPartitionBlocks;
	SkinPartition *SkinPartitionBlocks;// [NumSkinPartitionBlocks]
} NiSkinPartition;

typedef struct {
	NiObjectNET *parent;
} NiTexture;

typedef struct {
	NiTexture *parent;
	NIFbyte UseExternal;
	FilePath FileName;
	NIFuint UnknownLink;
	NIFbyte UnknownByte;
	// DUPLICATE FIELD NAME FileName
	NIFuint PixelData;
	NIFuint PixelLayout;
	NIFuint UseMipmaps;
	NIFuint AlphaFormat;
	NIFbyte IsStatic;
	NIFbyte DirectRender;
	NIFbyte PersistRenderData;
} NiSourceTexture;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
} NiSpecularProperty;

typedef struct {
	NiParticleModifier *parent;
	NIFfloat UnknownFloat1;
	NIFushort UnknownShort1;
	NIFfloat UnknownFloat2;
	NIFushort UnknownShort2;
	NIFfloat UnknownFloat3;
	NIFfloat UnknownFloat4;
	NIFfloat UnknownFloat5;
} NiSphericalCollider;

typedef struct {
	NiPointLight *parent;
	NIFfloat CutoffAngle;
	NIFfloat UnknownFloat;
	NIFfloat Exponent;
} NiSpotLight;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFbyte StencilEnabled;
	NIFuint StencilFunction;
	NIFuint StencilRef;
	NIFuint StencilMask;
	NIFuint FailAction;
	NIFuint ZFailAction;
	NIFuint PassAction;
	NIFuint DrawMode;
	// DUPLICATE FIELD NAME Flags - fixed
	// DUPLICATE FIELD NAME StencilRef - fixed
	// DUPLICATE FIELD NAME StencilMask - fixed
} NiStencilProperty;

typedef struct {
	NiExtraData *parent;
	NIFuint BytesRemaining;
	string StringData;
} NiStringExtraData;

typedef struct {
	NiObject *parent;
	StringPalette Palette;
} NiStringPalette;

typedef struct {
	NiExtraData *parent;
	NIFuint NumStrings;
	string *Data;// [NumStrings]
} NiStringsExtraData;

typedef struct {
	NiExtraData *parent;
	NIFuint UnknownInt1;
	NIFuint NumTextKeys;
	NiStringKey *TextKeys;// [NumTextKeys]
} NiTextKeyExtraData;

typedef struct {
	NiDynamicEffect *parent;
	Matrix33 ModelProjectionMatrix;
	Vector3 ModelProjectionTransform;
	NIFuint TextureFiltering;
	NIFuint TextureClamping;
	NIFshort Unknown;
	NIFuint TextureType;
	NIFuint CoordinateGenerationType;
	NIFuint Image;
	NIFuint SourceTexture;
	NIFbyte ClippingPlane;
	Vector3 UnknownVector;
	NIFfloat UnknownFloat;
	NIFshort PS2L;
	NIFshort PS2K;
	NIFushort UnknownShort;
} NiTextureEffect;

typedef struct {
	NiProperty *parent;
	NIFshort UnknownShort;
	NIFshort PS2L;
	NIFshort PS2K;
} NiTextureModeProperty;

typedef struct {
	NiObject *parent;
	NIFbyte UseExternal;
	FilePath FileName;
	NIFuint ImageData;
	NIFuint UnknownInt;
	NIFfloat UnknownFloat;
} NiImage;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFuint Image;
	NIFuint UnknownInt1;
	NIFuint UnknownInt2;
} NiTextureProperty;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFuint UnknownInt;
	MultiTextureElement TextureElements[5];
} NiMultiTextureProperty;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	// DUPLICATE FIELD NAME Flags - fixed
	NIFuint ApplyMode;
	NIFuint TextureCount;
	NIFbyte HasBaseTexture;
	TexDesc BaseTexture;
	NIFbyte HasDarkTexture;
	TexDesc DarkTexture;
	NIFbyte HasDetailTexture;
	TexDesc DetailTexture;
	NIFbyte HasGlossTexture;
	TexDesc GlossTexture;
	NIFbyte HasGlowTexture;
	TexDesc GlowTexture;
	NIFbyte HasBumpMapTexture;
	TexDesc BumpMapTexture;
	NIFfloat BumpMapLumaScale;
	NIFfloat BumpMapLumaOffset;
	Matrix22 BumpMapMatrix;
	NIFbyte HasNormalTexture;
	TexDesc NormalTexture;
	NIFbyte HasUnknown2Texture;
	TexDesc Unknown2Texture;
	NIFfloat Unknown2Float;
	NIFbyte HasDecal0Texture;
	TexDesc Decal0Texture;
	NIFbyte HasDecal1Texture;
	// DUPLICATE FIELD NAME HasDecal1Texture - fixed
	TexDesc Decal1Texture;
	NIFbyte HasDecal2Texture;
	// DUPLICATE FIELD NAME HasDecal2Texture - fixed
	TexDesc Decal2Texture;
	NIFbyte HasDecal3Texture;
	// DUPLICATE FIELD NAME HasDecal3Texture - fixed
	TexDesc Decal3Texture;
	NIFuint NumShaderTextures;
	ShaderTexDesc *ShaderTextures;// [NumShaderTextures]
} NiTexturingProperty;

typedef struct {
	NiKeyframeData *parent;
} NiTransformData;

typedef struct {
	NiTriBasedGeom *parent;
} NiTriShape;

typedef struct {
	NiTriBasedGeomData *parent;
	NIFuint NumTrianglePoints;
	NIFbyte HasTriangles;
	Triangle *Triangles;// [parent->NumTriangles]
	// DUPLICATE FIELD NAME Triangles - fixed
	NIFushort NumMatchGroups;
	MatchGroup *MatchGroups;// [NumMatchGroups]
} NiTriShapeData;

typedef struct {
	NiTriBasedGeom *parent;
} NiTriStrips;

typedef struct {
	NiTriBasedGeomData *parent;
	NIFushort NumStrips;
	NIFushort *StripLengths;// [NumStrips]
	NIFbyte HasPoints;
	NIFushort **Points;// [NumStrips], [StripLengths]
	// DUPLICATE FIELD NAME Points - fixed
} NiTriStripsData;

typedef struct {
	NiObjectNET *parent;
	NIFushort Unknown1;
	Matrix44 UnknownMatrix;
	NIFuint NumChildren;
	NIFuint *Children;// [NumChildren]
	NIFuint Child2;
	NIFuint Child3;
} NiEnvMappedTriShape;

typedef struct {
	NiTriShapeData *parent;
} NiEnvMappedTriShapeData;

typedef struct {
	NiObject *parent;
	NIFuint Unknown1[6];
	NIFushort Unknown2;
	Matrix33 Matrix;
	Vector3 Vector1;
	Vector3 Vector2;
	NIFshort Unknown3[4];
	NIFbyte Unknown4;
	NIFuint Unknown5;
	NIFshort Unknown6[24];
} NiBezierTriangle4;

typedef struct {
	NiAVObject *parent;
	NIFuint NumBezierTriangles;
	NIFuint *BezierTriangle;// [NumBezierTriangles]
	NIFuint Unknown1;
	NIFushort Count1;
	NIFushort Unknown2;
	Vector3 *Points1;// [Count1]
	NIFuint Unknown3;
	NIFfloat *Points2;// [Count1], [2]
	NIFuint Unknown4;
	NIFushort Count2;
	NIFushort *Data2;// [Count2], [4]
} NiBezierMesh;

typedef struct {
	NiTriBasedGeom *parent;
} NiClod;

typedef struct {
	NiTriBasedGeomData *parent;
	NIFushort UnknownShorts;
	NIFushort UnknownCount1;
	NIFushort UnknownCount2;
	NIFushort UnknownCount3;
	NIFfloat UnknownFloat;
	NIFushort UnknownShort;
	NIFushort *UnknownClodShorts1;// [UnknownCount1], [6]
	NIFushort *UnknownClodShorts2;// [UnknownCount2]
	NIFushort *UnknownClodShorts3;// [UnknownCount3], [6]
} NiClodData;

typedef struct {
	NiTimeController *parent;
	NIFushort UnknownShort;
	NIFuint Data;
} NiUVController;

typedef struct {
	NiObject *parent;
	NiFloatKeyGroup UVGroups[4];
} NiUVData;

typedef struct {
	NiExtraData *parent;
	Vector3 VectorData;
	NIFfloat UnknownFloat;
} NiVectorExtraData;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFuint VertexMode;
	NIFuint LightingMode;
} NiVertexColorProperty;

typedef struct {
	NiExtraData *parent;
	NIFuint NumBytes;
	NIFushort NumVertices;
	NIFfloat *Weight;// [NumVertices]
} NiVertWeightsExtraData;

typedef struct {
	NiObject *parent;
	NIFuint NumKeys;
	NiByteKey *Keys;// [NumKeys]
} NiVisData;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
} NiWireframeProperty;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFuint Function;
} NiZBufferProperty;

typedef struct {
	NiNode *parent;
} RootCollisionNode;

typedef struct {
	NiObject *parent;
	NIFuint Width;
	NIFuint Height;
	NIFuint ImageType;
	ByteColor3 *RGBImageData;// [Width], [Height]
	ByteColor4 *RGBAImageData;// [Width], [Height]
} NiRawImageData;

typedef struct {
	NiNode *parent;
	NIFuint SortingMode;
	NIFint UnknownInt2;
} NiSortAdjustNode;

typedef struct {
	NiSourceTexture *parent;
} NiSourceCubeMap;

typedef struct {
	NiObjectNET *parent;
	NIFfloat UnknownFloat1;
	NIFuint UnknownInt1;
	NIFuint *UnknownRefs1;// [UnknownInt1]
	NIFint NumDests;
	NIFuint *TransformDests;// [NumDests]
	NIFbyte UnknownByte;
	NIFint UnknownInt;
	NIFuint PropDescription;
} NiPhysXProp;

typedef struct {
	NIFbyte Number;
	NIFbyte UnknownByte1;
	NIFuint MaterialDesc;
} physXMaterialRef;

typedef struct {
	NiObject *parent;
	NIFint NumDests;
	NIFuint *ActorDescs;// [NumDests]
	NIFuint NumJoints;
	NIFuint *JointDescs;// [NumJoints]
	NIFint UnknownInt1;
	NIFuint NumMaterials;
	physXMaterialRef *MaterialDescs;// [NumMaterials]
	NIFuint UnknownInt2;
	NIFuint UnknownInt3;
	string UnknownString4;
	NIFuint UnknownInt5;
	NIFbyte UnknownByte6;
} NiPhysXPropDesc;

typedef struct {
	NiObject *parent;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	Quaternion UnknownQuat1;
	Quaternion UnknownQuat2;
	Quaternion UnknownQuat3;
	NIFuint UnknownRef0;
	NIFfloat UnknownInt4;
	NIFint UnknownInt5;
	NIFbyte UnknownByte1;
	NIFbyte UnknownByte2;
	NIFint UnknownInt6;
	NIFuint ShapeDescription;
	NIFuint UnknownRef1;
	NIFuint UnknownRef2;
	NIFuint *UnknownRefs3;// [UnknownInt6]
} NiPhysXActorDesc;

typedef struct {
	NiObject *parent;
	NIFbyte UnknownBytes[136];
} NiPhysXBodyDesc;

typedef struct {
	NiObject *parent;
	NIFbyte UnknownBytes[388];
} NiPhysXD6JointDesc;

typedef struct {
	NiObject *parent;
	NIFint UnknownInt1;
	Quaternion UnknownQuat1;
	Quaternion UnknownQuat2;
	Quaternion UnknownQuat3;
	NIFshort UnknownShort1;
	NIFint UnknownInt2;
	NIFshort UnknownShort2;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFint UnknownInt3;
	NIFint UnknownInt4;
	NIFint UnknownInt5;
	NIFint UnknownInt7;
	NIFint UnknownInt8;
	NIFuint MeshDescription;
} NiPhysXShapeDesc;

typedef struct {
	NiObject *parent;
	NIFshort UnknownShort1;
	NIFfloat UnknownFloat1;
	NIFshort UnknownShort2;
	NIFbyte UnknownBytes1[3];
	NIFbyte UnknownByte1;
	// DUPLICATE FIELD NAME UnknownBytes1 - fixed
	NIFbyte UnknownBytes1A[4];
	NIFbyte UnknownBytes2[8];
	NIFfloat UnknownFloat2;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFint NumVertices;
	NIFint UnknownInt4;
	Vector3 *Vertices;// [NumVertices]
	NIFbyte UnknownBytes3[982];
	NIFshort UnknownShorts1[368];
	NIFuint UnknownInts1[3328];
	NIFbyte UnknownByte2;
} NiPhysXMeshDesc;

typedef struct {
	NiObject *parent;
	NIFuint UnknownInt[12];
	NIFbyte UnknownByte1;
	NIFbyte UnknownByte2;
} NiPhysXMaterialDesc;

typedef struct {
	NiObject *parent;
	NIFbyte UnknownBytes[6];
} NiPhysXKinematicSrc;

typedef struct {
	NiObject *parent;
	NIFbyte UnknownByte1;
	NIFbyte UnknownByte2;
	NIFuint Node;
} NiPhysXTransformDest;

typedef struct {
	NiExtraData *parent;
	NIFint UnknownInts[4];
	NIFbyte UnknownBytes[37];
} NiArkAnimationExtraData;

typedef struct {
	NiExtraData *parent;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	string ImporterName;
	NIFbyte UnknownBytes[13];
	NIFfloat UnknownFloats[7];
} NiArkImporterExtraData;

typedef struct {
	NiExtraData *parent;
	NIFint UnknownInts1[2];
	NIFbyte UnknownByte;
	NIFint UnknownInt2;
	NIFint NumTextures;
	ArkTexture *Textures;// [NumTextures]
} NiArkTextureExtraData;

typedef struct {
	NiExtraData *parent;
	NIFbyte UnknownBytes[13];
} NiArkViewportInfoExtraData;

typedef struct {
	NiExtraData *parent;
	NIFint UnknownInt;
	string UnknownString;
} NiArkShaderExtraData;

typedef struct {
	NiTriBasedGeom *parent;
} NiLines;

typedef struct {
	NiGeometryData *parent;
	NIFbyte *Lines;// [parent->NumVertices]
} NiLinesData;

typedef struct {
	NIFushort NumVertices;
	NIFushort VertexOffset;
	NIFushort NumTriangles;
	NIFushort TriangleOffset;
} Polygon;

typedef struct {
	NiTriShapeData *parent;
	NIFushort MaxPolygons;
	Polygon *Polygons;// [MaxPolygons]
	NIFushort *PolygonIndices;// [MaxPolygons]
	NIFushort UnknownUShort1;
	NIFushort NumPolygons;
	NIFushort UsedVertices;
	NIFushort UnknownUShort2;
	NIFushort UsedTrianglePoints;
	NIFushort UnknownUShort3;
} NiScreenElementsData;

typedef struct {
	NiTriShape *parent;
} NiScreenElements;

typedef struct {
	NiNode *parent;
	NIFuint ShellLink;
	NIFint NumRooms;
	NIFuint *Rooms;// [NumRooms]
} NiRoomGroup;

typedef struct {
	NiNode *parent;
	NIFint NumWalls;
	Vector4 *WallPlane;// [NumWalls]
	NIFint NumInPortals;
	NIFuint *InPortals;// [NumInPortals]
	NIFint NumPortals2;
	NIFuint *Portals2;// [NumPortals2]
	NIFint NumItems;
	NIFuint *Items;// [NumItems]
} NiRoom;

typedef struct {
	NiAVObject *parent;
	NIFushort UnknownFlags;
	NIFshort UnknownShort1;
	NIFushort NumVertices;
	Vector3 *Vertices;// [NumVertices]
	NIFuint Target;
} NiPortal;

typedef struct {
	NiNode *parent;
} BSFadeNode;

typedef struct {
	NiProperty *parent;
	NIFushort Flags;
	NIFuint ShaderType;
	NIFuint ShaderFlags;
	NIFint UnknownInt2;
	NIFfloat EnvmapScale;
} BSShaderProperty;

typedef struct {
	BSShaderProperty *parent;
	NIFint UnknownInt3;
} BSShaderLightingProperty;

typedef struct {
	BSShaderLightingProperty *parent;
	SizedString FileName;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFfloat UnknownFloat4;
	NIFfloat UnknownFloat5;
} BSShaderNoLightingProperty;

typedef struct {
	BSShaderLightingProperty *parent;
	NIFuint TextureSet;
	NIFfloat UnknownFloat2;
	NIFint RefractionPeriod;
	NIFfloat UnknownFloat4;
	NIFfloat UnknownFloat5;
} BSShaderPPLightingProperty;

typedef struct {
	NiObject *parent;
	NIFint NumTextures;
	SizedString *Textures;// [NumTextures]
} BSShaderTextureSet;

typedef struct {
	BSShaderProperty *parent;
} WaterShaderProperty;

typedef struct {
	BSShaderProperty *parent;
	NIFint UnknownInt4;
	SizedString FileName;
	NIFint UnknownInt5;
} SkyShaderProperty;

typedef struct {
	BSShaderLightingProperty *parent;
	SizedString FileName;
} TileShaderProperty;

typedef struct {
	BSShaderProperty *parent;
} DistantLODShaderProperty;

typedef struct {
	BSShaderProperty *parent;
} BSDistantTreeShaderProperty;

typedef struct {
	BSShaderProperty *parent;
	SizedString FileName;
} TallGrassShaderProperty;

typedef struct {
	BSShaderProperty *parent;
} VolumetricFogShaderProperty;

typedef struct {
	BSShaderProperty *parent;
} HairShaderProperty;

typedef struct {
	BSShaderPPLightingProperty *parent;
} Lighting30ShaderProperty;

typedef struct {
	NiSkinInstance *parent;
	NIFint NumPartitions;
	BodyPartList *Partitions;// [NumPartitions]
} BSDismemberSkinInstance;

typedef struct {
	NiExtraData *parent;
	NIFfloat UnknownFloat1;
	NIFshort NumVectorBlocks;
	DecalVectorArray *VectorBlocks;// [NumVectorBlocks]
} BSDecalPlacementVectorExtraData;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat FadeInPercent;
	NIFfloat FadeoutPercent;
	NIFfloat Color1EndPercent;
	NIFfloat Color1StartPercent;
	NIFfloat Color2EndPercent;
	NIFfloat Color2StartPercent;
	Color4 Colors[3];
} BSPSysSimpleColorModifier;

typedef struct {
	NiNode *parent;
	NIFint Value;
	NIFbyte Unknownbyte;
} BSValueNode;

typedef struct {
	NiParticleSystem *parent;
} BSStripParticleSystem;

typedef struct {
	NiPSysData *parent;
	NIFshort UnknownShort1;
	NIFbyte Unknownbyte1;
	NIFint UnknownInt2;
	NIFint UnknownInt3;
} BSStripPSysData;

typedef struct {
	NiPSysModifier *parent;
	NIFfloat UpdateDeltaTime;
} BSPSysStripUpdateModifier;

typedef struct {
	NiFloatInterpController *parent;
} BSMaterialEmittanceMultController;

typedef struct {
	NiNode *parent;
	NIFushort MaxEmitterObjects;
	NIFint NumParticleSystems;
	NIFuint *ParticleSystems;// [NumParticleSystems]
} BSMasterParticleSystem;

typedef struct {
	NiPSysModifierCtlr *parent;
	NIFuint Data;
	NIFuint VisibilityInterpolator;
	NIFshort UnknownShort1;
	NIFint UnknownInt1;
} BSPSysMultiTargetEmitterCtlr;

typedef struct {
	NiFloatInterpController *parent;
} BSRefractionStrengthController;

typedef struct {
	NiNode *parent;
	Vector4 AlphaSortBound;
	NIFbyte IsStaticBound;
} BSOrderedNode;

typedef struct {
	NiNode *parent;
	NIFbyte UnknownByte1;
	NIFshort UnknownShort2;
} BSBlastNode;

typedef struct {
	NiNode *parent;
	NIFbyte UnknownByte1;
	NIFshort UnknownShort2;
} BSDamageStage;

typedef struct {
	NiTimeController *parent;
	NIFuint Interpolator;
} BSRefractionFirePeriodController;

typedef struct {
	bhkShape *parent;
	NIFuint NumSubShapes;
	NIFuint *SubShapes;// [NumSubShapes]
	NIFuint Material;
	NIFfloat UnknownFloats[6];
	NIFbyte UnknownByte1;
	NIFfloat UnknownFloat1;
} bhkConvexListShape;

typedef struct {
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFint UnknownInt3;
	NIFint UnknownInt4;
	NIFint UnknownInt5;
	NIFint UnknownInt6;
	NIFint UnknownInt7;
	NIFint UnknownInt8;
} BSTreadTransfSubInfo;

typedef struct {
	NIFfloat UnknownFloat1;
	BSTreadTransfSubInfo Data[2];
} BSTreadTransfInfo;

typedef struct {
	NiInterpolator *parent;
	NIFint NumTransfers;
	BSTreadTransfInfo *TreadTransferInfo;// [NumTransfers]
	NIFint UnknownInt1;
} BSTreadTransfInterpolator;

typedef struct {
	NiObject *parent;
	NIFshort UnknownShort1;
} BSAnimNotes;

typedef struct {
	bhkSerializable *parent;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFint UnknownInt3;
	NIFfloat UnknownFloat1;
	NIFfloat UnknownFloat2;
	NIFfloat UnknownFloat3;
	NIFfloat UnknownFloat4;
} bhkLiquidAction;

typedef struct {
	NiNode *parent;
	NIFuint MultiBound;
} BSMultiBoundNode;

typedef struct {
	NiObject *parent;
	NIFuint Data;
} BSMultiBound;

typedef struct {
	NiObject *parent;
} BSMultiBoundData;

typedef struct {
	BSMultiBoundData *parent;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFint UnknownInt3;
	NIFfloat Radius;
} BSMultiBoundSphere;

typedef struct {
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFbyte UnknownByte1;
} BSSegmentedTriangle;

typedef struct {
	NiTriShape *parent;
	NIFint NumSegTriangles;
	BSSegmentedTriangle *SegTriangles;// [NumSegTriangles]
} BSSegmentedTriShape;

typedef struct {
	BSMultiBoundData *parent;
	NIFfloat UnknownFloats1[6];
} BSMultiBoundAABB;

typedef struct {
	NIFint DataType;
	NIFint BlockSize;
	NIFint NumBlocks;
	NIFint BlockSize2;
	NIFint BlockIndex;
	NIFint UnknownInt1;
	NIFbyte UnknownByte1;
} AdditionalDataInfo;

typedef struct {
	NIFbyte HasData;
	NIFint BlockSize;
	NIFint NumBlocks;
	NIFint *BlockOffsets;// [NumBlocks]
	NIFint NumData;
	NIFint *DataSizes;// [NumData]
	NIFbyte *Data;// [NumData], [BlockSize]
} AdditionalDataBlock;

typedef struct {
	NiObject *parent;
	NIFushort NumVertices;
	NIFuint NumBlockInfos;
	AdditionalDataInfo *BlockInfos;// [NumBlockInfos]
	NIFint NumBlocks;
	AdditionalDataBlock *Blocks;// [NumBlocks]
} NiAdditionalGeometryData;

typedef struct {
	NiExtraData *parent;
	NIFint NumItems;
	NIFint *Items;// [NumItems]
} BSWArray;

typedef struct {
	bhkShapePhantom *parent;
	NIFint UnknownInts1[15];
} bhkAabbPhantom;

typedef struct {
	NiTimeController *parent;
	NIFuint Interpolator;
} BSFrustumFOVController;

typedef struct {
	NiNode *parent;
	NIFbyte Unknownbyte1;
	NIFshort UnknownShort1;
} BSDebrisNode;

typedef struct {
	bhkConstraint *parent;
	NIFint UnknownInts1[41];
	NIFshort UnknownShort1;
} bhkBreakableConstraint;

typedef struct {
	bhkSerializable *parent;
	NIFint UnknownInts1[17];
} bhkOrientHingedBodyAction;

typedef struct {
	NIFuint StartIndex;
	NIFuint NumIndices;
} Region;

typedef struct {
	NiObject *parent;
	NIFuint Usage;
	NIFuint Access;
	NIFuint NumBytes;
	NIFuint CloningBehavior;
	NIFuint NumRegions;
	Region *Regions;// [NumRegions]
	NIFuint NumComponents;
	NIFuint *ComponentFormats;// [NumComponents]
	NIFbyte *Data;// [NumBytes]
	NIFbyte Streamable;
} NiDataStream;

typedef struct {
	string Name;
	NIFuint Index;
} SemanticData;

typedef struct {
	NIFuint Stream;
	NIFbyte IsPerInstance;
	NIFushort NumSubmeshes;
	NIFushort *SubmeshToRegionMap;// [NumSubmeshes]
	NIFint NumComponents;
	SemanticData *ComponentSemantics;// [NumComponents]
} MeshData;

typedef struct {
	string MaterialName;
	NIFuint MaterialExtraData;
} MaterialData;

typedef struct {
	NiAVObject *parent;
	NIFuint NumMaterials;
	MaterialData *MaterialData;// [NumMaterials]
	NIFint ActiveMaterial;
	NIFbyte MaterialNeedsUpdateDefault;
} NiRenderObject;

typedef struct {
	NiObject *parent;
	NIFuint NumSubmitPoints;
	NIFushort *SubmitPoints;// [NumSubmitPoints]
	NIFuint NumCompletePoints;
	NIFushort *CompletePoints;// [NumCompletePoints]
} NiMeshModifier;

typedef struct {
	NiRenderObject *parent;
	NIFuint PrimitiveType;
	NIFushort NumSubmeshes;
	NIFbyte InstancingEnabled;
	SphereBV Bound;
	NIFuint NumDatas;
	MeshData *Datas;// [NumDatas]
	NIFuint NumModifiers;
	NIFuint *Modifiers;// [NumModifiers]
} NiMesh;

typedef struct {
	NiInterpController *parent;
	NIFint Unknown2;
	NIFuint NumInterpolators;
	NIFuint *Interpolators;// [NumInterpolators]
	NIFuint NumTargets;
	string *TargetNames;// [NumTargets]
} NiMorphWeightsController;

typedef struct {
	SemanticData Semantic;
	NIFuint NormalizeFlag;
} ElementReference;

typedef struct {
	NiMeshModifier *parent;
	NIFbyte Flags;
	NIFushort NumTargets;
	NIFuint NumElements;
	ElementReference *Elements;// [NumElements]
} NiMorphMeshModifier;

typedef struct {
	NiMeshModifier *parent;
	NIFushort Flags;
	NIFuint SkeletonRoot;
	SkinTransform SkeletonTransform;
	NIFuint NumBones;
	NIFuint *Bones;// [NumBones]
	SkinTransform *BoneTransforms;// [NumBones]
	SphereBV *BoneBounds;// [NumBones]
} NiSkinningMeshModifier;

typedef struct {
	NiMeshModifier *parent;
} NiInstancingMeshModifier;

typedef struct {
	NiTimeController *parent;
} NiSkinningLODController;

typedef struct {
	NiAVObject *parent;
	NIFint Unknown3;
	NIFint *Unknown38;// [Unknown3]
	NIFint Unknown4;
	NIFint Unknown5;
	NIFint *Unknown39;// [Unknown3]
	NIFint Unknown6;
	NIFint Unknown7;
	NIFint Unknown8;
	NIFint Unknown9;
	NIFfloat Unknown10;
	NIFint Unknown11;
	NIFint Unknown12;
	NIFuint Simulator;
	NIFuint Generator;
	NIFint Unknown15;
	NIFint Unknown16;
	NIFint Unknown17;
	NIFuint Emitter;
	NIFint Unknown19;
	NIFint Unknown20;
	NIFint Unknown21;
	NIFbyte Unknown22[4];
	NIFint Unknown27;
	NIFint Unknown28;
	NIFint Unknown29;
	NIFint Unknown30;
	NIFint Unknown31;
	NIFint Unknown32;
	NIFint Unknown33;
	NIFint Unknown34;
	NIFbyte Unknown35;
	NIFint Unknown36;
	NIFshort Unknown37;
} NiPSParticleSystem;

typedef struct {
	NiPSParticleSystem *parent;
	NIFint Unknown23;
	NIFint Unknown24;
	NIFint Unknown25;
	NIFbyte Unknown26;
} NiPSMeshParticleSystem;

typedef struct {
	NiPSysEmitterCtlr *parent;
} NiPSEmitParticlesCtlr;

typedef struct {
	NiTimeController *parent;
	NIFuint Interpolator;
	NIFint Unknown2;
} NiPSForceActiveCtlr;

typedef struct {
	NiMeshModifier *parent;
	NIFuint NumSimulationSteps;
	NIFuint *SimulationSteps;// [NumSimulationSteps]
} NiPSSimulator;

typedef struct {
	NiObject *parent;
} NiPSSimulatorStep;

typedef struct {
	NiPSSimulatorStep *parent;
	NIFbyte NumSizeKeys;
	NiFloatKey *SizeKeys;// [NumSizeKeys]
	NIFuint SizeLoopBehavior;
	NIFfloat Unknown1;
	NIFfloat Unknown2;
	NIFfloat Unknown3;
	NIFbyte NumColorKeys;
	NiByteColor4Key *ColorKeys;// [NumColorKeys]
	NIFuint ColorLoopBehavior;
	NIFbyte NumRotationKeys;
	QuatKey *RotationKeys;// [NumRotationKeys]
	NIFuint RotationLoopBehavior;
	NIFfloat GrowTime;
	NIFfloat ShrinkTime;
	NIFushort GrowGeneration;
	NIFushort ShrinkGeneration;
} NiPSSimulatorGeneralStep;

typedef struct {
	NiPSSimulatorStep *parent;
	NIFuint NumForces;
	NIFuint *Forces;// [NumForces]
} NiPSSimulatorForcesStep;

typedef struct {
	NiPSSimulatorStep *parent;
	NIFuint NumColliders;
	NIFuint *Colliders;// [NumColliders]
} NiPSSimulatorCollidersStep;

typedef struct {
	NiPSSimulatorStep *parent;
	NIFbyte NumRotationKeys;
	QuatKey *RotationKeys;// [NumRotationKeys]
	NIFuint RotationLoopBehavior;
} NiPSSimulatorMeshAlignStep;

typedef struct {
	NiPSSimulatorStep *parent;
} NiPSSimulatorFinalStep;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1;
	NIFbyte Unknown2;
	NIFbyte Unknown3;
	NIFbyte Unknown4;
	NIFbyte Unknown5;
	NIFbyte Unknown6;
	NIFbyte Unknown7;
	NIFbyte Unknown8;
	NIFbyte Unknown9;
	NIFbyte Unknown10;
	NIFbyte Unknown11;
	NIFbyte Unknown12;
} NiPSFacingQuadGenerator;

typedef struct {
	NiObject *parent;
	string Name;
	NIFushort UnknownFlags;
	NIFuint NumUnknownLinks1;
	NIFuint *UnknownLinks1;// [NumUnknownLinks1]
	NIFint UnkownInt2;
	NIFuint Target;
	NIFfloat UnkownFloat4;
	NIFbyte UnkownByte5;
	NIFint UnkownInt6;
	NIFint UnkownInt7;
	NIFint UnkownInt8;
	NIFbyte UnkownByte9;
} NiShadowGenerator;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1;
	NIFbyte Unknown2;
} NiPSBoundUpdater;

typedef struct {
	NiObject *parent;
	NIFint Unknown1;
	NIFint Unknown2;
	NIFbyte Unknown3;
	NIFfloat Unknown4;
	NIFfloat Unknown5;
	NIFfloat Unknown6;
	NIFfloat Unknown7;
	NIFfloat Unknown8;
	NIFfloat Unknown9;
	NIFint Unknown10;
} NiPSDragForce;

typedef struct {
	NiObject *parent;
	NIFbyte Unknown1;
	NIFbyte Unknown2;
	NIFbyte Unknown3;
	NIFbyte Unknown4;
	NIFbyte Unknown5;
	NIFbyte Unknown6;
	NIFbyte Unknown7;
	NIFbyte Unknown8;
	NIFbyte Unknown9;
	NIFbyte Unknown10;
	NIFbyte Unknown11;
	NIFbyte Unknown12;
	NIFbyte Unknown13;
	NIFbyte Unknown14;
	NIFbyte Unknown15;
	NIFbyte Unknown16;
	NIFbyte Unknown17;
	NIFfloat Unknown18;
	NIFbyte Unknown19;
	NIFbyte Unknown20;
	NIFbyte Unknown21;
	NIFbyte Unknown22;
	NIFbyte Unknown23;
	NIFbyte Unknown24;
	NIFbyte Unknown25;
	NIFbyte Unknown26;
	NIFbyte Unknown27;
	NIFbyte Unknown28;
	NIFbyte Unknown29;
	NIFbyte Unknown30;
	NIFbyte Unknown31;
	NIFbyte Unknown32;
	NIFbyte Unknown33;
	NIFbyte Unknown34;
	NIFfloat Unknown35;
	NIFuint Unknown36;
} NiPSGravityForce;

typedef struct {
	NiObject *parent;
	string Name;
	NIFfloat Unknown1;
	NIFfloat Unknown2;
	NIFbyte Unknown3;
	NIFbyte Unknown4;
	NIFbyte Unknown5;
	NIFbyte Unknown6;
	NIFfloat Unknown7;
	NIFbyte Unknown8;
	NIFbyte Unknown9;
	NIFbyte Unknown10;
	NIFbyte Unknown11;
	NIFfloat Unknown12;
	NIFint Unknown13;
	NIFfloat Unknown14;
	NIFfloat Unknown15;
	NIFfloat Unknown16;
	NIFfloat Unknown17;
	NIFfloat Unknown18;
	NIFfloat Unknown19;
	NIFfloat Unknown20;
	NIFfloat Unknown21;
	NIFfloat Unknown22;
	NIFbyte Unknown23;
	NIFbyte Unknown24;
	NIFbyte Unknown25;
	NIFbyte Unknown26;
	NIFbyte Unknown27;
	NIFbyte Unknown28;
	NIFbyte Unknown29;
	NIFbyte Unknown30;
	NIFbyte Unknown31;
	NIFbyte Unknown32;
	NIFbyte Unknown33;
	NIFbyte Unknown34;
	NIFbyte Unknown35;
	NIFbyte Unknown36;
	NIFbyte Unknown37;
	NIFbyte Unknown38;
	NIFbyte Unknown39;
	NIFbyte Unknown40;
	NIFbyte Unknown41;
	NIFbyte Unknown42;
	NIFbyte Unknown43;
	NIFbyte Unknown44;
	NIFbyte Unknown45;
	NIFbyte Unknown46;
	NIFbyte Unknown47;
	NIFbyte Unknown48;
} NiPSBoxEmitter;

typedef struct {
	NiObject *parent;
	string Name;
	NIFint Unknown1;
	NIFint Unknown2;
	NIFint Unknown3;
	NIFint Unknown27;
	NIFfloat Unknown4;
	NIFfloat Unknown5;
	NIFfloat Unknown6;
	NIFfloat Unknown28;
	NIFint Unknown7;
	NIFfloat Unknown8;
	NIFfloat Unknown9;
	NIFfloat Unknown10;
	NIFfloat Unknown11;
	NIFfloat Unknown12;
	NIFint Unknown13;
	NIFfloat Unknown14;
	NIFfloat Unknown15;
	NIFfloat Unknown16;
	NIFint Unknown17;
	NIFint Unknown18;
	NIFshort Unknown19;
	NIFint Unknown20;
	NIFint Unknown21;
	NIFfloat Unknown22;
	NIFint Unknown23;
	NIFint Unknown24;
	NIFint Unknown25;
	NIFint Unknown26;
} NiPSMeshEmitter;

typedef struct {
	NiTimeController *parent;
	NIFint Unknown2;
	NIFint Unknown3;
} NiPSGravityStrengthCtlr;

typedef struct {
	NiObject *parent;
	string Name;
	NIFint UnknownInt1;
	NIFint UnknownInt2;
	NIFshort UnknownShort3;
	NIFbyte UnknownByte4;
	NIFfloat UnknownFloats5[8];
	NIFuint UnknownLink6;
} NiPSPlanarCollider;

typedef struct {
	NiTimeController *parent;
	NIFuint Interpolator;
	NIFint Unknown3;
} NiPSEmitterSpeedCtlr;

typedef struct {
	NiTimeController *parent;
	NIFuint Interpolator;
	NIFint Unknown2;
} NiPSEmitterRadiusCtlr;

typedef struct {
	NiTimeController *parent;
} NiPSResetOnLoopCtlr;

typedef struct {
	NiObject *parent;
	string Name;
	NIFint Unknown2;
	NIFint Unknown3;
	NIFint Unknown4;
	NIFint Unknown5;
	NIFfloat Unknown6;
	NIFint Unknown7;
	NIFfloat Unknown8;
	NIFfloat Unknown9;
	NIFint Unknown10;
	NIFfloat Unknown11;
	NIFint Unknown12;
	NIFint Unknown13;
	NIFint Unknown14;
	NIFint Unknown15;
	NIFint Unknown16;
	NIFfloat Unknown17;
	NIFint Unknown18;
	NIFint Unknown19;
	NIFshort Unknown20;
	NIFint Unknown21;
	NIFfloat Unknown22;
} NiPSSphereEmitter;

typedef struct {
	NiPSSphereEmitter *parent;
	NIFfloat Unknown23;
} NiPSCylinderEmitter;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSEmitterDeclinationCtlr;

typedef struct {
	NiPSEmitterDeclinationCtlr *parent;
} NiPSEmitterDeclinationVarCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSEmitterPlanarAngleCtlr;

typedef struct {
	NiPSEmitterPlanarAngleCtlr *parent;
} NiPSEmitterPlanarAngleVarCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSEmitterRotAngleCtlr;

typedef struct {
	NiPSEmitterRotAngleCtlr *parent;
} NiPSEmitterRotAngleVarCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSEmitterRotSpeedCtlr;

typedef struct {
	NiPSEmitterRotSpeedCtlr *parent;
} NiPSEmitterRotSpeedVarCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSEmitterLifeSpanCtlr;

typedef struct {
	NiObject *parent;
	string Name;
	NIFbyte Unknown1;
	NIFint Unknown2;
	NIFint Unknown3;
	NIFint Unknown4;
	NIFint Unknown5;
	NIFint Unknown6;
	NIFint Unknown7;
	NIFint Unknown8;
	NIFint Unknown9;
	NIFint Unknown10;
} NiPSBombForce;

typedef struct {
	NiObject *parent;
	NIFint Unknown1;
	NIFint Unknown2;
	NIFbyte Unknown3;
	NIFfloat Unknown4;
	NIFint Unknown5;
	NIFshort Unknown6;
	NIFint Unknown7;
} NiPSSphericalCollider;

typedef struct {
	NiObject *parent;
} NiPSSpawner;

typedef struct {
	NiObject *parent;
} NiSequenceData;

typedef struct {
	NiObject *parent;
} NiTransformEvaluator;

typedef struct {
	NiObject *parent;
} NiBSplineCompTransformEvaluator;

typedef struct {
	NiObject *parent;
} NiMeshHWInstance;

typedef struct {
	NiTimeController *parent;
	NIFfloat UnknownFloat;
	NIFfloat UnknownFloat2;
	NIFuint NumBones;
	NIFuint *Bones;// [NumBones]
	NIFuint NumBones2;
	NIFuint *Bones2;// [NumBones2]
} NiFurSpringController;

inline void readSizedString (SizedString * obj, unsigned int ARG);
inline void readstring (string * obj, unsigned int ARG);
void readByteArray (ByteArray * obj, unsigned int ARG);
void readByteMatrix (ByteMatrix * obj, unsigned int ARG);
inline void readColor3 (Color3 * obj, unsigned int ARG);
inline void readByteColor3 (ByteColor3 * obj, unsigned int ARG);
inline void readColor4 (Color4 * obj, unsigned int ARG);
inline void readByteColor4 (ByteColor4 * obj, unsigned int ARG);
void readFilePath (FilePath * obj, unsigned int ARG);
void readFooter (Footer * obj, unsigned int ARG);
void readLODRange (LODRange * obj, unsigned int ARG);
inline void readMatchGroup (MatchGroup * obj, unsigned int ARG);
inline void readVector3 (Vector3 * obj, unsigned int ARG);
inline void readVector4 (Vector4 * obj, unsigned int ARG);
inline void readQuaternion (Quaternion * obj, unsigned int ARG);
inline void readQuaternionXYZW (QuaternionXYZW * obj, unsigned int ARG);
inline void readMatrix22 (Matrix22 * obj, unsigned int ARG);
inline void readMatrix33 (Matrix33 * obj, unsigned int ARG);
inline void readMatrix44 (Matrix44 * obj, unsigned int ARG);
void readMipMap (MipMap * obj, unsigned int ARG);
void readNodeGroup (NodeGroup * obj, unsigned int ARG);
void readShortString (ShortString * obj, unsigned int ARG);
inline void readSkinShape (SkinShape * obj, unsigned int ARG);
void readSkinShapeGroup (SkinShapeGroup * obj, unsigned int ARG);
inline void readSkinWeight (SkinWeight * obj, unsigned int ARG);
void readAVObject (AVObject * obj, unsigned int ARG);
void readControllerLink (ControllerLink * obj, unsigned int ARG);
void readExportInfo (ExportInfo * obj, unsigned int ARG);
void readHeader (Header * obj, unsigned int ARG);
void readStringPalette (StringPalette * obj, unsigned int ARG);
void readTBC (TBC * obj, unsigned int ARG);
//inline void readKey (Key * obj, unsigned int ARG, int t);
//inline void readKeyGroup (KeyGroup * obj, unsigned int ARG, int t);
inline void readQuatKey (QuatKey * obj, unsigned int ARG);
inline void readTexCoord (TexCoord * obj, unsigned int ARG);
void readTexDesc (TexDesc * obj, unsigned int ARG);
void readShaderTexDesc (ShaderTexDesc * obj, unsigned int ARG);
void readTexSource (TexSource * obj, unsigned int ARG);
inline void readTriangle (Triangle * obj, unsigned int ARG);
void readSkinPartition (SkinPartition * obj, unsigned int ARG);
void readBoundingBox (BoundingBox * obj, unsigned int ARG);
void readFurniturePosition (FurniturePosition * obj, unsigned int ARG);
inline void readhkTriangle (hkTriangle * obj, unsigned int ARG);
void readMorph (Morph * obj, unsigned int ARG);
void readParticle (Particle * obj, unsigned int ARG);
void readSkinTransform (SkinTransform * obj, unsigned int ARG);
void readSkinData (SkinData * obj, unsigned int ARG);
void readSphereBV (SphereBV * obj, unsigned int ARG);
void readOblivionColFilter (OblivionColFilter * obj, unsigned int ARG);
void readOblivionSubShape (OblivionSubShape * obj, unsigned int ARG);
void readMotorDescriptor (MotorDescriptor * obj, unsigned int ARG);
void readRagdollDescriptor (RagdollDescriptor * obj, unsigned int ARG);
void readLimitedHingeDescriptor (LimitedHingeDescriptor * obj,
				 unsigned int ARG);
void readHingeDescriptor (HingeDescriptor * obj, unsigned int ARG);
void readOldSkinData (OldSkinData * obj, unsigned int ARG);
void readMultiTextureElement (MultiTextureElement * obj, unsigned int ARG);
void readBoxBV (BoxBV * obj, unsigned int ARG);
void readCapsuleBV (CapsuleBV * obj, unsigned int ARG);
void readHalfSpaceBV (HalfSpaceBV * obj, unsigned int ARG);
void readBoundingVolume (BoundingVolume * obj, unsigned int ARG);
void readUnionBV (UnionBV * obj, unsigned int ARG);
void readMorphWeight (MorphWeight * obj, unsigned int ARG);
void readArkTexture (ArkTexture * obj, unsigned int ARG);
void readInertiaMatrix (InertiaMatrix * obj, unsigned int ARG);
void readDecalVectorArray (DecalVectorArray * obj, unsigned int ARG);
void readBodyPartList (BodyPartList * obj, unsigned int ARG);
//void readNiObject (NiObject * obj, unsigned int ARG);
void readNi3dsAlphaAnimator (Ni3dsAlphaAnimator * obj, unsigned int ARG);
void readNi3dsAnimationNode (Ni3dsAnimationNode * obj, unsigned int ARG);
void readNi3dsColorAnimator (Ni3dsColorAnimator * obj, unsigned int ARG);
void readNi3dsMorphShape (Ni3dsMorphShape * obj, unsigned int ARG);
void readNi3dsParticleSystem (Ni3dsParticleSystem * obj, unsigned int ARG);
void readNi3dsPathController (Ni3dsPathController * obj, unsigned int ARG);
void readNiParticleModifier (NiParticleModifier * obj, unsigned int ARG);
void readNiPSysCollider (NiPSysCollider * obj, unsigned int ARG);
inline void readbhkRefObject (bhkRefObject * obj, unsigned int ARG);
void readbhkSerializable (bhkSerializable * obj, unsigned int ARG);
void readbhkWorldObject (bhkWorldObject * obj, unsigned int ARG);
void readbhkPhantom (bhkPhantom * obj, unsigned int ARG);
void readbhkShapePhantom (bhkShapePhantom * obj, unsigned int ARG);
void readbhkSimpleShapePhantom (bhkSimpleShapePhantom * obj, unsigned int ARG);
void readbhkEntity (bhkEntity * obj, unsigned int ARG);
void readbhkRigidBody (bhkRigidBody * obj, unsigned int ARG);
void readbhkRigidBodyT (bhkRigidBodyT * obj, unsigned int ARG);
void readbhkConstraint (bhkConstraint * obj, unsigned int ARG);
void readbhkLimitedHingeConstraint (bhkLimitedHingeConstraint * obj,
				    unsigned int ARG);
void readbhkMalleableConstraint (bhkMalleableConstraint * obj,
				 unsigned int ARG);
void readbhkStiffSpringConstraint (bhkStiffSpringConstraint * obj,
				   unsigned int ARG);
void readbhkRagdollConstraint (bhkRagdollConstraint * obj, unsigned int ARG);
void readbhkPrismaticConstraint (bhkPrismaticConstraint * obj,
				 unsigned int ARG);
void readbhkHingeConstraint (bhkHingeConstraint * obj, unsigned int ARG);
void readbhkShape (bhkShape * obj, unsigned int ARG);
void readbhkTransformShape (bhkTransformShape * obj, unsigned int ARG);
void readbhkSphereRepShape (bhkSphereRepShape * obj, unsigned int ARG);
void readbhkConvexShape (bhkConvexShape * obj, unsigned int ARG);
void readbhkSphereShape (bhkSphereShape * obj, unsigned int ARG);
void readbhkCapsuleShape (bhkCapsuleShape * obj, unsigned int ARG);
void readbhkBoxShape (bhkBoxShape * obj, unsigned int ARG);
void readbhkConvexVerticesShape (bhkConvexVerticesShape * obj,
				 unsigned int ARG);
void readbhkConvexTransformShape (bhkConvexTransformShape * obj,
				  unsigned int ARG);
void readbhkMultiSphereShape (bhkMultiSphereShape * obj, unsigned int ARG);
void readbhkBvTreeShape (bhkBvTreeShape * obj, unsigned int ARG);
void readbhkMoppBvTreeShape (bhkMoppBvTreeShape * obj, unsigned int ARG);
void readbhkShapeCollection (bhkShapeCollection * obj, unsigned int ARG);
void readbhkListShape (bhkListShape * obj, unsigned int ARG);
void readbhkMeshShape (bhkMeshShape * obj, unsigned int ARG);
void readbhkPackedNiTriStripsShape (bhkPackedNiTriStripsShape * obj,
				    unsigned int ARG);
void readbhkNiTriStripsShape (bhkNiTriStripsShape * obj, unsigned int ARG);
void readNiExtraData (NiExtraData * obj, unsigned int ARG);
inline void readNiInterpolator (NiInterpolator * obj, unsigned int ARG);
void readNiKeyBasedInterpolator (NiKeyBasedInterpolator * obj,
				 unsigned int ARG);
void readNiFloatInterpolator (NiFloatInterpolator * obj, unsigned int ARG);
void readNiTransformInterpolator (NiTransformInterpolator * obj,
				  unsigned int ARG);
void readNiPoint3Interpolator (NiPoint3Interpolator * obj, unsigned int ARG);
void readNiPathInterpolator (NiPathInterpolator * obj, unsigned int ARG);
void readNiBoolInterpolator (NiBoolInterpolator * obj, unsigned int ARG);
void readNiBoolTimelineInterpolator (NiBoolTimelineInterpolator * obj,
				     unsigned int ARG);
void readNiBlendInterpolator (NiBlendInterpolator * obj, unsigned int ARG);
void readNiBSplineInterpolator (NiBSplineInterpolator * obj, unsigned int ARG);
inline void readNiObjectNET (NiObjectNET * obj, unsigned int ARG);
void readNiCollisionObject (NiCollisionObject * obj, unsigned int ARG);
void readNiCollisionData (NiCollisionData * obj, unsigned int ARG);
void readbhkNiCollisionObject (bhkNiCollisionObject * obj, unsigned int ARG);
void readbhkCollisionObject (bhkCollisionObject * obj, unsigned int ARG);
void readbhkBlendCollisionObject (bhkBlendCollisionObject * obj,
				  unsigned int ARG);
void readbhkPCollisionObject (bhkPCollisionObject * obj, unsigned int ARG);
void readbhkSPCollisionObject (bhkSPCollisionObject * obj, unsigned int ARG);
inline void readNiAVObject (NiAVObject * obj, unsigned int ARG);
void readNiDynamicEffect (NiDynamicEffect * obj, unsigned int ARG);
void readNiLight (NiLight * obj, unsigned int ARG);
void readNiProperty (NiProperty * obj, unsigned int ARG);
void readNiTransparentProperty (NiTransparentProperty * obj, unsigned int ARG);
void readNiPSysModifier (NiPSysModifier * obj, unsigned int ARG);
void readNiPSysEmitter (NiPSysEmitter * obj, unsigned int ARG);
void readNiPSysVolumeEmitter (NiPSysVolumeEmitter * obj, unsigned int ARG);
void readNiTimeController (NiTimeController * obj, unsigned int ARG);
void readNiInterpController (NiInterpController * obj, unsigned int ARG);
void readNiMultiTargetTransformController (NiMultiTargetTransformController *
					   obj, unsigned int ARG);
void readNiGeomMorpherController (NiGeomMorpherController * obj,
				  unsigned int ARG);
void readNiMorphController (NiMorphController * obj, unsigned int ARG);
void readNiMorpherController (NiMorpherController * obj, unsigned int ARG);
void readNiSingleInterpController (NiSingleInterpController * obj,
				   unsigned int ARG);
void readNiKeyframeController (NiKeyframeController * obj, unsigned int ARG);
void readNiTransformController (NiTransformController * obj, unsigned int ARG);
void readNiPSysModifierCtlr (NiPSysModifierCtlr * obj, unsigned int ARG);
void readNiPSysEmitterCtlr (NiPSysEmitterCtlr * obj, unsigned int ARG);
void readNiPSysModifierBoolCtlr (NiPSysModifierBoolCtlr * obj,
				 unsigned int ARG);
void readNiPSysModifierActiveCtlr (NiPSysModifierActiveCtlr * obj,
				   unsigned int ARG);
void readNiPSysModifierFloatCtlr (NiPSysModifierFloatCtlr * obj,
				  unsigned int ARG);
void readNiPSysEmitterDeclinationCtlr (NiPSysEmitterDeclinationCtlr * obj,
				       unsigned int ARG);
void readNiPSysEmitterDeclinationVarCtlr (NiPSysEmitterDeclinationVarCtlr * obj,
					  unsigned int ARG);
void readNiPSysEmitterInitialRadiusCtlr (NiPSysEmitterInitialRadiusCtlr * obj,
					 unsigned int ARG);
void readNiPSysEmitterLifeSpanCtlr (NiPSysEmitterLifeSpanCtlr * obj,
				    unsigned int ARG);
void readNiPSysEmitterSpeedCtlr (NiPSysEmitterSpeedCtlr * obj,
				 unsigned int ARG);
void readNiPSysGravityStrengthCtlr (NiPSysGravityStrengthCtlr * obj,
				    unsigned int ARG);
void readNiFloatInterpController (NiFloatInterpController * obj,
				  unsigned int ARG);
void readNiFlipController (NiFlipController * obj, unsigned int ARG);
void readNiAlphaController (NiAlphaController * obj, unsigned int ARG);
void readNiTextureTransformController (NiTextureTransformController * obj,
				       unsigned int ARG);
void readNiLightDimmerController (NiLightDimmerController * obj,
				  unsigned int ARG);
void readNiBoolInterpController (NiBoolInterpController * obj,
				 unsigned int ARG);
void readNiVisController (NiVisController * obj, unsigned int ARG);
void readNiPoint3InterpController (NiPoint3InterpController * obj,
				   unsigned int ARG);
void readNiMaterialColorController (NiMaterialColorController * obj,
				    unsigned int ARG);
void readNiLightColorController (NiLightColorController * obj,
				 unsigned int ARG);
void readNiExtraDataController (NiExtraDataController * obj, unsigned int ARG);
void readNiFloatExtraDataController (NiFloatExtraDataController * obj,
				     unsigned int ARG);
void readNiBoneLODController (NiBoneLODController * obj, unsigned int ARG);
void readNiBSBoneLODController (NiBSBoneLODController * obj, unsigned int ARG);
void readNiGeometry (NiGeometry * obj, unsigned int ARG);
void readNiTriBasedGeom (NiTriBasedGeom * obj, unsigned int ARG);
void readNiGeometryData (NiGeometryData * obj, unsigned int ARG);
void readNiTriBasedGeomData (NiTriBasedGeomData * obj, unsigned int ARG);
void readbhkBlendController (bhkBlendController * obj, unsigned int ARG);
void readBSBound (BSBound * obj, unsigned int ARG);
void readBSFurnitureMarker (BSFurnitureMarker * obj, unsigned int ARG);
void readBSParentVelocityModifier (BSParentVelocityModifier * obj,
				   unsigned int ARG);
void readBSPSysArrayEmitter (BSPSysArrayEmitter * obj, unsigned int ARG);
void readBSWindModifier (BSWindModifier * obj, unsigned int ARG);
void readhkPackedNiTriStripsData (hkPackedNiTriStripsData * obj,
				  unsigned int ARG);
void readNiAlphaProperty (NiAlphaProperty * obj, unsigned int ARG);
void readNiAmbientLight (NiAmbientLight * obj, unsigned int ARG);
void readNiParticlesData (NiParticlesData * obj, unsigned int ARG);
void readNiRotatingParticlesData (NiRotatingParticlesData * obj,
				  unsigned int ARG);
void readNiAutoNormalParticlesData (NiAutoNormalParticlesData * obj,
				    unsigned int ARG);
void readParticleDesc (ParticleDesc * obj, unsigned int ARG);
void readNiPSysData (NiPSysData * obj, unsigned int ARG);
void readNiMeshPSysData (NiMeshPSysData * obj, unsigned int ARG);
void readNiBinaryExtraData (NiBinaryExtraData * obj, unsigned int ARG);
void readNiBinaryVoxelExtraData (NiBinaryVoxelExtraData * obj,
				 unsigned int ARG);
void readNiBinaryVoxelData (NiBinaryVoxelData * obj, unsigned int ARG);
void readNiBlendBoolInterpolator (NiBlendBoolInterpolator * obj,
				  unsigned int ARG);
void readNiBlendFloatInterpolator (NiBlendFloatInterpolator * obj,
				   unsigned int ARG);
void readNiBlendPoint3Interpolator (NiBlendPoint3Interpolator * obj,
				    unsigned int ARG);
void readNiBlendTransformInterpolator (NiBlendTransformInterpolator * obj,
				       unsigned int ARG);
void readNiBoolData (NiBoolData * obj, unsigned int ARG);
void readNiBooleanExtraData (NiBooleanExtraData * obj, unsigned int ARG);
void readNiBSplineBasisData (NiBSplineBasisData * obj, unsigned int ARG);
void readNiBSplineFloatInterpolator (NiBSplineFloatInterpolator * obj,
				     unsigned int ARG);
void readNiBSplineCompFloatInterpolator (NiBSplineCompFloatInterpolator * obj,
					 unsigned int ARG);
void readNiBSplinePoint3Interpolator (NiBSplinePoint3Interpolator * obj,
				      unsigned int ARG);
void readNiBSplineCompPoint3Interpolator (NiBSplineCompPoint3Interpolator * obj,
					  unsigned int ARG);
void readNiBSplineTransformInterpolator (NiBSplineTransformInterpolator * obj,
					 unsigned int ARG);
void readNiBSplineCompTransformInterpolator (NiBSplineCompTransformInterpolator
					     * obj, unsigned int ARG);
void readBSRotAccumTransfInterpolator (BSRotAccumTransfInterpolator * obj,
				       unsigned int ARG);
void readNiBSplineData (NiBSplineData * obj, unsigned int ARG);
void readNiCamera (NiCamera * obj, unsigned int ARG);
void readNiColorData (NiColorData * obj, unsigned int ARG);
void readNiColorExtraData (NiColorExtraData * obj, unsigned int ARG);
void readNiControllerManager (NiControllerManager * obj, unsigned int ARG);
void readNiSequence (NiSequence * obj, unsigned int ARG);
void readNiControllerSequence (NiControllerSequence * obj, unsigned int ARG);
inline void readNiAVObjectPalette (NiAVObjectPalette * obj, unsigned int ARG);
void readNiDefaultAVObjectPalette (NiDefaultAVObjectPalette * obj,
				   unsigned int ARG);
void readNiDirectionalLight (NiDirectionalLight * obj, unsigned int ARG);
void readNiDitherProperty (NiDitherProperty * obj, unsigned int ARG);
void readNiRollController (NiRollController * obj, unsigned int ARG);
void readNiFloatData (NiFloatData * obj, unsigned int ARG);
void readNiFloatExtraData (NiFloatExtraData * obj, unsigned int ARG);
void readNiFloatsExtraData (NiFloatsExtraData * obj, unsigned int ARG);
void readNiFogProperty (NiFogProperty * obj, unsigned int ARG);
void readNiGravity (NiGravity * obj, unsigned int ARG);
void readNiIntegerExtraData (NiIntegerExtraData * obj, unsigned int ARG);
void readBSXFlags (BSXFlags * obj, unsigned int ARG);
void readNiIntegersExtraData (NiIntegersExtraData * obj, unsigned int ARG);
void readBSKeyframeController (BSKeyframeController * obj, unsigned int ARG);
void readNiKeyframeData (NiKeyframeData * obj, unsigned int ARG);
void readNiLookAtController (NiLookAtController * obj, unsigned int ARG);
void readNiLookAtInterpolator (NiLookAtInterpolator * obj, unsigned int ARG);
void readNiMaterialProperty (NiMaterialProperty * obj, unsigned int ARG);
void readNiMorphData (NiMorphData * obj, unsigned int ARG);
inline void readNiNode (NiNode * obj, unsigned int ARG);
void readNiBone (NiBone * obj, unsigned int ARG);
void readAvoidNode (AvoidNode * obj, unsigned int ARG);
void readFxWidget (FxWidget * obj, unsigned int ARG);
void readFxButton (FxButton * obj, unsigned int ARG);
void readFxRadioButton (FxRadioButton * obj, unsigned int ARG);
void readNiBillboardNode (NiBillboardNode * obj, unsigned int ARG);
void readNiBSAnimationNode (NiBSAnimationNode * obj, unsigned int ARG);
void readNiBSParticleNode (NiBSParticleNode * obj, unsigned int ARG);
void readNiSwitchNode (NiSwitchNode * obj, unsigned int ARG);
void readNiLODNode (NiLODNode * obj, unsigned int ARG);
void readNiPalette (NiPalette * obj, unsigned int ARG);
void readNiParticleBomb (NiParticleBomb * obj, unsigned int ARG);
void readNiParticleColorModifier (NiParticleColorModifier * obj,
				  unsigned int ARG);
void readNiParticleGrowFade (NiParticleGrowFade * obj, unsigned int ARG);
void readNiParticleMeshModifier (NiParticleMeshModifier * obj,
				 unsigned int ARG);
void readNiParticleRotation (NiParticleRotation * obj, unsigned int ARG);
void readNiParticles (NiParticles * obj, unsigned int ARG);
void readNiAutoNormalParticles (NiAutoNormalParticles * obj, unsigned int ARG);
void readNiParticleMeshes (NiParticleMeshes * obj, unsigned int ARG);
void readNiParticleMeshesData (NiParticleMeshesData * obj, unsigned int ARG);
void readNiParticleSystem (NiParticleSystem * obj, unsigned int ARG);
void readNiMeshParticleSystem (NiMeshParticleSystem * obj, unsigned int ARG);
void readNiParticleSystemController (NiParticleSystemController * obj,
				     unsigned int ARG);
void readNiBSPArrayController (NiBSPArrayController * obj, unsigned int ARG);
void readNiPathController (NiPathController * obj, unsigned int ARG);
void readChannelData (ChannelData * obj, unsigned int ARG);
void readATextureRenderData (ATextureRenderData * obj, unsigned int ARG);
void readNiPersistentSrcTextureRendererData (NiPersistentSrcTextureRendererData
					     * obj, unsigned int ARG);
void readNiPixelData (NiPixelData * obj, unsigned int ARG);
void readNiPlanarCollider (NiPlanarCollider * obj, unsigned int ARG);
void readNiPointLight (NiPointLight * obj, unsigned int ARG);
void readNiPosData (NiPosData * obj, unsigned int ARG);
void readNiPSysAgeDeathModifier (NiPSysAgeDeathModifier * obj,
				 unsigned int ARG);
void readNiPSysBombModifier (NiPSysBombModifier * obj, unsigned int ARG);
void readNiPSysBoundUpdateModifier (NiPSysBoundUpdateModifier * obj,
				    unsigned int ARG);
void readNiPSysBoxEmitter (NiPSysBoxEmitter * obj, unsigned int ARG);
void readNiPSysColliderManager (NiPSysColliderManager * obj, unsigned int ARG);
void readNiPSysColorModifier (NiPSysColorModifier * obj, unsigned int ARG);
void readNiPSysCylinderEmitter (NiPSysCylinderEmitter * obj, unsigned int ARG);
void readNiPSysDragModifier (NiPSysDragModifier * obj, unsigned int ARG);
void readNiPSysEmitterCtlrData (NiPSysEmitterCtlrData * obj, unsigned int ARG);
void readNiPSysGravityModifier (NiPSysGravityModifier * obj, unsigned int ARG);
void readNiPSysGrowFadeModifier (NiPSysGrowFadeModifier * obj,
				 unsigned int ARG);
void readNiPSysMeshEmitter (NiPSysMeshEmitter * obj, unsigned int ARG);
void readNiPSysMeshUpdateModifier (NiPSysMeshUpdateModifier * obj,
				   unsigned int ARG);
void readNiPSysPlanarCollider (NiPSysPlanarCollider * obj, unsigned int ARG);
void readNiPSysSphericalCollider (NiPSysSphericalCollider * obj,
				  unsigned int ARG);
void readNiPSysPositionModifier (NiPSysPositionModifier * obj,
				 unsigned int ARG);
void readNiPSysResetOnLoopCtlr (NiPSysResetOnLoopCtlr * obj, unsigned int ARG);
void readNiPSysRotationModifier (NiPSysRotationModifier * obj,
				 unsigned int ARG);
void readNiPSysSpawnModifier (NiPSysSpawnModifier * obj, unsigned int ARG);
void readNiPSysSphereEmitter (NiPSysSphereEmitter * obj, unsigned int ARG);
void readNiPSysUpdateCtlr (NiPSysUpdateCtlr * obj, unsigned int ARG);
void readNiPSysFieldModifier (NiPSysFieldModifier * obj, unsigned int ARG);
void readNiPSysVortexFieldModifier (NiPSysVortexFieldModifier * obj,
				    unsigned int ARG);
void readNiPSysGravityFieldModifier (NiPSysGravityFieldModifier * obj,
				     unsigned int ARG);
void readNiPSysDragFieldModifier (NiPSysDragFieldModifier * obj,
				  unsigned int ARG);
void readNiPSysTurbulenceFieldModifier (NiPSysTurbulenceFieldModifier * obj,
					unsigned int ARG);
void readNiPSysFieldMagnitudeCtlr (NiPSysFieldMagnitudeCtlr * obj,
				   unsigned int ARG);
void readNiPSysFieldAttenuationCtlr (NiPSysFieldAttenuationCtlr * obj,
				     unsigned int ARG);
void readNiPSysFieldMaxDistanceCtlr (NiPSysFieldMaxDistanceCtlr * obj,
				     unsigned int ARG);
void readNiPSysAirFieldAirFrictionCtlr (NiPSysAirFieldAirFrictionCtlr * obj,
					unsigned int ARG);
void readNiPSysAirFieldInheritVelocityCtlr (NiPSysAirFieldInheritVelocityCtlr *
					    obj, unsigned int ARG);
void readNiPSysAirFieldSpreadCtlr (NiPSysAirFieldSpreadCtlr * obj,
				   unsigned int ARG);
void readNiPSysInitialRotSpeedCtlr (NiPSysInitialRotSpeedCtlr * obj,
				    unsigned int ARG);
void readNiPSysInitialRotSpeedVarCtlr (NiPSysInitialRotSpeedVarCtlr * obj,
				       unsigned int ARG);
void readNiPSysInitialRotAngleCtlr (NiPSysInitialRotAngleCtlr * obj,
				    unsigned int ARG);
void readNiPSysInitialRotAngleVarCtlr (NiPSysInitialRotAngleVarCtlr * obj,
				       unsigned int ARG);
void readNiPSysEmitterPlanarAngleCtlr (NiPSysEmitterPlanarAngleCtlr * obj,
				       unsigned int ARG);
void readNiPSysEmitterPlanarAngleVarCtlr (NiPSysEmitterPlanarAngleVarCtlr * obj,
					  unsigned int ARG);
void readNiPSysAirFieldModifier (NiPSysAirFieldModifier * obj,
				 unsigned int ARG);
void readNiPSysTrailEmitter (NiPSysTrailEmitter * obj, unsigned int ARG);
void readNiLightIntensityController (NiLightIntensityController * obj,
				     unsigned int ARG);
void readNiPSysRadialFieldModifier (NiPSysRadialFieldModifier * obj,
				    unsigned int ARG);
inline void readNiLODData (NiLODData * obj, unsigned int ARG);
void readNiRangeLODData (NiRangeLODData * obj, unsigned int ARG);
void readNiScreenLODData (NiScreenLODData * obj, unsigned int ARG);
void readNiRotatingParticles (NiRotatingParticles * obj, unsigned int ARG);
void readNiSequenceStreamHelper (NiSequenceStreamHelper * obj,
				 unsigned int ARG);
void readNiShadeProperty (NiShadeProperty * obj, unsigned int ARG);
void readNiSkinData (NiSkinData * obj, unsigned int ARG);
void readNiSkinInstance (NiSkinInstance * obj, unsigned int ARG);
void readNiTriShapeSkinController (NiTriShapeSkinController * obj,
				   unsigned int ARG);
void readNiClodSkinInstance (NiClodSkinInstance * obj, unsigned int ARG);
void readNiSkinPartition (NiSkinPartition * obj, unsigned int ARG);
void readNiTexture (NiTexture * obj, unsigned int ARG);
void readNiSourceTexture (NiSourceTexture * obj, unsigned int ARG);
void readNiSpecularProperty (NiSpecularProperty * obj, unsigned int ARG);
void readNiSphericalCollider (NiSphericalCollider * obj, unsigned int ARG);
void readNiSpotLight (NiSpotLight * obj, unsigned int ARG);
void readNiStencilProperty (NiStencilProperty * obj, unsigned int ARG);
void readNiStringExtraData (NiStringExtraData * obj, unsigned int ARG);
void readNiStringPalette (NiStringPalette * obj, unsigned int ARG);
void readNiStringsExtraData (NiStringsExtraData * obj, unsigned int ARG);
void readNiTextKeyExtraData (NiTextKeyExtraData * obj, unsigned int ARG);
void readNiTextureEffect (NiTextureEffect * obj, unsigned int ARG);
void readNiTextureModeProperty (NiTextureModeProperty * obj, unsigned int ARG);
void readNiImage (NiImage * obj, unsigned int ARG);
void readNiTextureProperty (NiTextureProperty * obj, unsigned int ARG);
void readNiMultiTextureProperty (NiMultiTextureProperty * obj,
				 unsigned int ARG);
void readNiTexturingProperty (NiTexturingProperty * obj, unsigned int ARG);
void readNiTransformData (NiTransformData * obj, unsigned int ARG);
void readNiTriShape (NiTriShape * obj, unsigned int ARG);
void readNiTriShapeData (NiTriShapeData * obj, unsigned int ARG);
void readNiTriStrips (NiTriStrips * obj, unsigned int ARG);
void readNiTriStripsData (NiTriStripsData * obj, unsigned int ARG);
void readNiEnvMappedTriShape (NiEnvMappedTriShape * obj, unsigned int ARG);
void readNiEnvMappedTriShapeData (NiEnvMappedTriShapeData * obj,
				  unsigned int ARG);
void readNiBezierTriangle4 (NiBezierTriangle4 * obj, unsigned int ARG);
void readNiBezierMesh (NiBezierMesh * obj, unsigned int ARG);
void readNiClod (NiClod * obj, unsigned int ARG);
void readNiClodData (NiClodData * obj, unsigned int ARG);
void readNiUVController (NiUVController * obj, unsigned int ARG);
void readNiUVData (NiUVData * obj, unsigned int ARG);
void readNiVectorExtraData (NiVectorExtraData * obj, unsigned int ARG);
void readNiVertexColorProperty (NiVertexColorProperty * obj, unsigned int ARG);
void readNiVertWeightsExtraData (NiVertWeightsExtraData * obj,
				 unsigned int ARG);
void readNiVisData (NiVisData * obj, unsigned int ARG);
void readNiWireframeProperty (NiWireframeProperty * obj, unsigned int ARG);
void readNiZBufferProperty (NiZBufferProperty * obj, unsigned int ARG);
void readRootCollisionNode (RootCollisionNode * obj, unsigned int ARG);
void readNiRawImageData (NiRawImageData * obj, unsigned int ARG);
void readNiSortAdjustNode (NiSortAdjustNode * obj, unsigned int ARG);
void readNiSourceCubeMap (NiSourceCubeMap * obj, unsigned int ARG);
void readNiPhysXProp (NiPhysXProp * obj, unsigned int ARG);
void readphysXMaterialRef (physXMaterialRef * obj, unsigned int ARG);
void readNiPhysXPropDesc (NiPhysXPropDesc * obj, unsigned int ARG);
void readNiPhysXActorDesc (NiPhysXActorDesc * obj, unsigned int ARG);
void readNiPhysXBodyDesc (NiPhysXBodyDesc * obj, unsigned int ARG);
void readNiPhysXD6JointDesc (NiPhysXD6JointDesc * obj, unsigned int ARG);
void readNiPhysXShapeDesc (NiPhysXShapeDesc * obj, unsigned int ARG);
void readNiPhysXMeshDesc (NiPhysXMeshDesc * obj, unsigned int ARG);
void readNiPhysXMaterialDesc (NiPhysXMaterialDesc * obj, unsigned int ARG);
void readNiPhysXKinematicSrc (NiPhysXKinematicSrc * obj, unsigned int ARG);
void readNiPhysXTransformDest (NiPhysXTransformDest * obj, unsigned int ARG);
void readNiArkAnimationExtraData (NiArkAnimationExtraData * obj,
				  unsigned int ARG);
void readNiArkImporterExtraData (NiArkImporterExtraData * obj,
				 unsigned int ARG);
void readNiArkTextureExtraData (NiArkTextureExtraData * obj, unsigned int ARG);
void readNiArkViewportInfoExtraData (NiArkViewportInfoExtraData * obj,
				     unsigned int ARG);
void readNiArkShaderExtraData (NiArkShaderExtraData * obj, unsigned int ARG);
void readNiLines (NiLines * obj, unsigned int ARG);
void readNiLinesData (NiLinesData * obj, unsigned int ARG);
void readPolygon (Polygon * obj, unsigned int ARG);
void readNiScreenElementsData (NiScreenElementsData * obj, unsigned int ARG);
void readNiScreenElements (NiScreenElements * obj, unsigned int ARG);
void readNiRoomGroup (NiRoomGroup * obj, unsigned int ARG);
void readNiRoom (NiRoom * obj, unsigned int ARG);
void readNiPortal (NiPortal * obj, unsigned int ARG);
void readBSFadeNode (BSFadeNode * obj, unsigned int ARG);
void readBSShaderProperty (BSShaderProperty * obj, unsigned int ARG);
void readBSShaderLightingProperty (BSShaderLightingProperty * obj,
				   unsigned int ARG);
void readBSShaderNoLightingProperty (BSShaderNoLightingProperty * obj,
				     unsigned int ARG);
void readBSShaderPPLightingProperty (BSShaderPPLightingProperty * obj,
				     unsigned int ARG);
void readBSShaderTextureSet (BSShaderTextureSet * obj, unsigned int ARG);
void readWaterShaderProperty (WaterShaderProperty * obj, unsigned int ARG);
void readSkyShaderProperty (SkyShaderProperty * obj, unsigned int ARG);
void readTileShaderProperty (TileShaderProperty * obj, unsigned int ARG);
void readDistantLODShaderProperty (DistantLODShaderProperty * obj,
				   unsigned int ARG);
void readBSDistantTreeShaderProperty (BSDistantTreeShaderProperty * obj,
				      unsigned int ARG);
void readTallGrassShaderProperty (TallGrassShaderProperty * obj,
				  unsigned int ARG);
void readVolumetricFogShaderProperty (VolumetricFogShaderProperty * obj,
				      unsigned int ARG);
void readHairShaderProperty (HairShaderProperty * obj, unsigned int ARG);
void readLighting30ShaderProperty (Lighting30ShaderProperty * obj,
				   unsigned int ARG);
void readBSDismemberSkinInstance (BSDismemberSkinInstance * obj,
				  unsigned int ARG);
void readBSDecalPlacementVectorExtraData (BSDecalPlacementVectorExtraData * obj,
					  unsigned int ARG);
void readBSPSysSimpleColorModifier (BSPSysSimpleColorModifier * obj,
				    unsigned int ARG);
void readBSValueNode (BSValueNode * obj, unsigned int ARG);
void readBSStripParticleSystem (BSStripParticleSystem * obj, unsigned int ARG);
void readBSStripPSysData (BSStripPSysData * obj, unsigned int ARG);
void readBSPSysStripUpdateModifier (BSPSysStripUpdateModifier * obj,
				    unsigned int ARG);
void readBSMaterialEmittanceMultController (BSMaterialEmittanceMultController *
					    obj, unsigned int ARG);
void readBSMasterParticleSystem (BSMasterParticleSystem * obj,
				 unsigned int ARG);
void readBSPSysMultiTargetEmitterCtlr (BSPSysMultiTargetEmitterCtlr * obj,
				       unsigned int ARG);
void readBSRefractionStrengthController (BSRefractionStrengthController * obj,
					 unsigned int ARG);
void readBSOrderedNode (BSOrderedNode * obj, unsigned int ARG);
void readBSBlastNode (BSBlastNode * obj, unsigned int ARG);
void readBSDamageStage (BSDamageStage * obj, unsigned int ARG);
void readBSRefractionFirePeriodController (BSRefractionFirePeriodController *
					   obj, unsigned int ARG);
void readbhkConvexListShape (bhkConvexListShape * obj, unsigned int ARG);
void readBSTreadTransfSubInfo (BSTreadTransfSubInfo * obj, unsigned int ARG);
void readBSTreadTransfInfo (BSTreadTransfInfo * obj, unsigned int ARG);
void readBSTreadTransfInterpolator (BSTreadTransfInterpolator * obj,
				    unsigned int ARG);
void readBSAnimNotes (BSAnimNotes * obj, unsigned int ARG);
void readbhkLiquidAction (bhkLiquidAction * obj, unsigned int ARG);
void readBSMultiBoundNode (BSMultiBoundNode * obj, unsigned int ARG);
void readBSMultiBound (BSMultiBound * obj, unsigned int ARG);
inline void readBSMultiBoundData (BSMultiBoundData * obj, unsigned int ARG);
void readBSMultiBoundSphere (BSMultiBoundSphere * obj, unsigned int ARG);
void readBSSegmentedTriangle (BSSegmentedTriangle * obj, unsigned int ARG);
void readBSSegmentedTriShape (BSSegmentedTriShape * obj, unsigned int ARG);
void readBSMultiBoundAABB (BSMultiBoundAABB * obj, unsigned int ARG);
void readAdditionalDataInfo (AdditionalDataInfo * obj, unsigned int ARG);
void readAdditionalDataBlock (AdditionalDataBlock * obj, unsigned int ARG);
void readNiAdditionalGeometryData (NiAdditionalGeometryData * obj,
				   unsigned int ARG);
void readBSWArray (BSWArray * obj, unsigned int ARG);
void readbhkAabbPhantom (bhkAabbPhantom * obj, unsigned int ARG);
void readBSFrustumFOVController (BSFrustumFOVController * obj,
				 unsigned int ARG);
void readBSDebrisNode (BSDebrisNode * obj, unsigned int ARG);
void readbhkBreakableConstraint (bhkBreakableConstraint * obj,
				 unsigned int ARG);
void readbhkOrientHingedBodyAction (bhkOrientHingedBodyAction * obj,
				    unsigned int ARG);
void readRegion (Region * obj, unsigned int ARG);
void readNiDataStream (NiDataStream * obj, unsigned int ARG);
void readSemanticData (SemanticData * obj, unsigned int ARG);
void readMeshData (MeshData * obj, unsigned int ARG);
void readMaterialData (MaterialData * obj, unsigned int ARG);
void readNiRenderObject (NiRenderObject * obj, unsigned int ARG);
void readNiMeshModifier (NiMeshModifier * obj, unsigned int ARG);
void readNiMesh (NiMesh * obj, unsigned int ARG);
void readNiMorphWeightsController (NiMorphWeightsController * obj,
				   unsigned int ARG);
void readElementReference (ElementReference * obj, unsigned int ARG);
void readNiMorphMeshModifier (NiMorphMeshModifier * obj, unsigned int ARG);
void readNiSkinningMeshModifier (NiSkinningMeshModifier * obj,
				 unsigned int ARG);
void readNiInstancingMeshModifier (NiInstancingMeshModifier * obj,
				   unsigned int ARG);
void readNiSkinningLODController (NiSkinningLODController * obj,
				  unsigned int ARG);
void readNiPSParticleSystem (NiPSParticleSystem * obj, unsigned int ARG);
void readNiPSMeshParticleSystem (NiPSMeshParticleSystem * obj,
				 unsigned int ARG);
void readNiPSEmitParticlesCtlr (NiPSEmitParticlesCtlr * obj, unsigned int ARG);
void readNiPSForceActiveCtlr (NiPSForceActiveCtlr * obj, unsigned int ARG);
void readNiPSSimulator (NiPSSimulator * obj, unsigned int ARG);
inline void readNiPSSimulatorStep (NiPSSimulatorStep * obj, unsigned int ARG);
void readNiPSSimulatorGeneralStep (NiPSSimulatorGeneralStep * obj,
				   unsigned int ARG);
void readNiPSSimulatorForcesStep (NiPSSimulatorForcesStep * obj,
				  unsigned int ARG);
void readNiPSSimulatorCollidersStep (NiPSSimulatorCollidersStep * obj,
				     unsigned int ARG);
void readNiPSSimulatorMeshAlignStep (NiPSSimulatorMeshAlignStep * obj,
				     unsigned int ARG);
void readNiPSSimulatorFinalStep (NiPSSimulatorFinalStep * obj,
				 unsigned int ARG);
void readNiPSFacingQuadGenerator (NiPSFacingQuadGenerator * obj,
				  unsigned int ARG);
void readNiShadowGenerator (NiShadowGenerator * obj, unsigned int ARG);
void readNiPSBoundUpdater (NiPSBoundUpdater * obj, unsigned int ARG);
void readNiPSDragForce (NiPSDragForce * obj, unsigned int ARG);
void readNiPSGravityForce (NiPSGravityForce * obj, unsigned int ARG);
void readNiPSBoxEmitter (NiPSBoxEmitter * obj, unsigned int ARG);
void readNiPSMeshEmitter (NiPSMeshEmitter * obj, unsigned int ARG);
void readNiPSGravityStrengthCtlr (NiPSGravityStrengthCtlr * obj,
				  unsigned int ARG);
void readNiPSPlanarCollider (NiPSPlanarCollider * obj, unsigned int ARG);
void readNiPSEmitterSpeedCtlr (NiPSEmitterSpeedCtlr * obj, unsigned int ARG);
void readNiPSEmitterRadiusCtlr (NiPSEmitterRadiusCtlr * obj, unsigned int ARG);
void readNiPSResetOnLoopCtlr (NiPSResetOnLoopCtlr * obj, unsigned int ARG);
void readNiPSSphereEmitter (NiPSSphereEmitter * obj, unsigned int ARG);
void readNiPSCylinderEmitter (NiPSCylinderEmitter * obj, unsigned int ARG);
void readNiPSEmitterDeclinationCtlr (NiPSEmitterDeclinationCtlr * obj,
				     unsigned int ARG);
void readNiPSEmitterDeclinationVarCtlr (NiPSEmitterDeclinationVarCtlr * obj,
					unsigned int ARG);
void readNiPSEmitterPlanarAngleCtlr (NiPSEmitterPlanarAngleCtlr * obj,
				     unsigned int ARG);
void readNiPSEmitterPlanarAngleVarCtlr (NiPSEmitterPlanarAngleVarCtlr * obj,
					unsigned int ARG);
void readNiPSEmitterRotAngleCtlr (NiPSEmitterRotAngleCtlr * obj,
				  unsigned int ARG);
void readNiPSEmitterRotAngleVarCtlr (NiPSEmitterRotAngleVarCtlr * obj,
				     unsigned int ARG);
void readNiPSEmitterRotSpeedCtlr (NiPSEmitterRotSpeedCtlr * obj,
				  unsigned int ARG);
void readNiPSEmitterRotSpeedVarCtlr (NiPSEmitterRotSpeedVarCtlr * obj,
				     unsigned int ARG);
void readNiPSEmitterLifeSpanCtlr (NiPSEmitterLifeSpanCtlr * obj,
				  unsigned int ARG);
void readNiPSBombForce (NiPSBombForce * obj, unsigned int ARG);
void readNiPSSphericalCollider (NiPSSphericalCollider * obj, unsigned int ARG);
inline void readNiPSSpawner (NiPSSpawner * obj, unsigned int ARG);
inline void readNiSequenceData (NiSequenceData * obj, unsigned int ARG);
inline void readNiTransformEvaluator (NiTransformEvaluator * obj, unsigned int ARG);
inline void readNiBSplineCompTransformEvaluator (NiBSplineCompTransformEvaluator * obj,
					  unsigned int ARG);
inline void readNiMeshHWInstance (NiMeshHWInstance * obj, unsigned int ARG);
void readNiFurSpringController (NiFurSpringController * obj, unsigned int ARG);
#endif