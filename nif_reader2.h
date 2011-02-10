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

#define T_BYTE 0
#define T_FLOAT 1
#define T_QUATERNION 2
#define T_STRING 3
#define T_BYTECOLOR4 4
#define T_COLOR4 5
#define T_VECTOR3 6

int readnif(char *fname);

#define byte unsigned char

typedef struct {
	void *d;
	int t;
} TEMPLATE;

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
	unsigned int Length;
	// [Length], [(null)]
	char *Value;
} SizedString;

typedef struct {
	SizedString String;
	unsigned int Index;
} string;

typedef struct {
	unsigned int Data_Size;
	// [Data_Size], [(null)]
	byte *Data;
} ByteArray;

typedef struct {
	unsigned int Data_Size_1;
	unsigned int Data_Size_2;
	// [Data_Size_2], [Data_Size_1]
	byte **Data;
} ByteMatrix;

typedef struct {
	float r;
	float g;
	float b;
} Color3;

typedef struct {
	byte r;
	byte g;
	byte b;
} ByteColor3;

typedef struct {
	float r;
	float g;
	float b;
	float a;
} Color4;

typedef struct {
	byte r;
	byte g;
	byte b;
	byte a;
} ByteColor4;

typedef struct {
	SizedString String;
	unsigned int Index;
} FilePath;

typedef struct {
	unsigned int Num_Roots;
	// [Num_Roots], [(null)]
	unsigned int *Roots;
} Footer;

typedef struct {
	float Near_Extent;
	float Far_Extent;
	// [3], [(null)]
	unsigned int *Unknown_Ints;
} LODRange;

typedef struct {
	unsigned short Num_Vertices;
	// [Num_Vertices], [(null)]
	unsigned short *Vertex_Indices;
} MatchGroup;

typedef struct {
	float x;
	float y;
	float z;
} Vector3;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct {
	float w;
	float x;
	float y;
	float z;
} Quaternion;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} QuaternionXYZW;

typedef struct {
	float m11;
	float m21;
	float m12;
	float m22;
} Matrix22;

typedef struct {
	float m11;
	float m21;
	float m31;
	float m12;
	float m22;
	float m32;
	float m13;
	float m23;
	float m33;
} Matrix33;

typedef struct {
	float m11;
	float m21;
	float m31;
	float m41;
	float m12;
	float m22;
	float m32;
	float m42;
	float m13;
	float m23;
	float m33;
	float m43;
	float m14;
	float m24;
	float m34;
	float m44;
} Matrix44;

typedef struct {
	unsigned int Width;
	unsigned int Height;
	unsigned int Offset;
} MipMap;

typedef struct {
	unsigned int Num_Nodes;
	// [Num_Nodes], [(null)]
	unsigned int *Nodes;
} NodeGroup;

typedef struct {
	byte Length;
	// [Length], [(null)]
	char *Value;
} ShortString;

typedef struct {
	unsigned int Shape;
	unsigned int Skin_Instance;
} SkinShape;

typedef struct {
	unsigned int Num_Link_Pairs;
	// [Num_Link_Pairs], [(null)]
	SkinShape *Link_Pairs;
} SkinShapeGroup;

typedef struct {
	unsigned short Index;
	float Weight;
} SkinWeight;

typedef struct {
	SizedString Name;
	unsigned int AV_Object;
} AVObject;

typedef struct {
	string Target_Name;
	unsigned int Controller;
	unsigned int Interpolator;
	// DUPLICATE FIELD NAME Controller - fixed
	unsigned int Unknown_Link_2;
	unsigned short Unknown_Short_0;
	byte Priority;
	unsigned int String_Palette;
	string Node_Name;
	// DUPLICATE FIELD NAME Node_Name - fixed
	unsigned int Node_Name_Offset;
	string Property_Type;
	// DUPLICATE FIELD NAME Property_Type - fixed
	unsigned int Property_Type_Offset;
	string Controller_Type;
	// DUPLICATE FIELD NAME Controller_Type - fixed
	unsigned int Controller_Type_Offset;
	string Variable_1;
	// DUPLICATE FIELD NAME Variable_1 - fixed
	unsigned int Variable_1_Offset;
	string Variable_2;
	// DUPLICATE FIELD NAME Variable_2 - fixed
	unsigned int Variable_2_Offset;
} ControllerLink;

typedef struct {
	unsigned int Unknown;
	ShortString Creator;
	ShortString Export_Info_1;
	ShortString Export_Info_2;
} ExportInfo;

typedef struct {
	char *Header_String;
	// [3], [(null)]
	char **Copyright;
	unsigned int Version;
	byte Endian_Type;
	unsigned int User_Version;
	unsigned int Num_Blocks;
	unsigned int User_Version_2;
	unsigned int Unknown_Int_3;
	ExportInfo Export_Info;
	// DUPLICATE FIELD NAME Export_Info - fixed
	unsigned short Num_Block_Types;
	// [Num_Block_Types], [(null)]
	SizedString *Block_Types;
	// [Num_Blocks], [(null)]
	unsigned short *Block_Type_Index;
	// [Num_Blocks], [(null)]
	unsigned int *Block_Size;
	unsigned int Num_Strings;
	unsigned int Max_String_Length;
	// [Num_Strings], [(null)]
	SizedString *Strings;
	unsigned int Unknown_Int_2;
} Header;

typedef struct {
	SizedString Palette;
	unsigned int Length;
} StringPalette;

typedef struct {
	float t;
	float b;
	float c;
} TBC;

typedef struct {
	float Time;
	TEMPLATE Value;
	TEMPLATE Forward;
	TEMPLATE Backward;
	TBC TBC;
} Key;

typedef struct {
	unsigned int Num_Keys;
	unsigned int Interpolation;
	// [Num_Keys], [(null)]
	Key *Keys;
} KeyGroup;

typedef struct {
	float Time;
	// DUPLICATE FIELD NAME Time - fixed
	TEMPLATE Value;
	TBC TBC;
} QuatKey;

typedef struct {
	float u;
	float v;
} TexCoord;

typedef struct {
	unsigned int Source;
	unsigned int Clamp_Mode;
	unsigned int Filter_Mode;
	unsigned short Flags;
	short Unknown_short;
	unsigned int UV_Set;
	short PS2_L;
	short PS2_K;
	unsigned short Unknown1;
	byte Has_Texture_Transform;
	TexCoord Translation;
	TexCoord Tiling;
	float W_Rotation;
	unsigned int Transform_Type_;
	TexCoord Center_Offset;
} TexDesc;

typedef struct {
	byte Is_Used;
	TexDesc Texture_Data;
	unsigned int Map_Index;
} ShaderTexDesc;

typedef struct {
	byte Use_External;
	FilePath File_Name;
	unsigned int Unknown_Link;
	byte Unknown_Byte;
	// DUPLICATE FIELD NAME File_Name
	unsigned int Pixel_Data;
} TexSource;

typedef struct {
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
} Triangle;

typedef struct {
	unsigned short Num_Vertices;
	unsigned short Num_Triangles;
	unsigned short Num_Bones;
	unsigned short Num_Strips;
	unsigned short Num_Weights_Per_Vertex;
	// [Num_Bones], [(null)]
	unsigned short *Bones;
	byte Has_Vertex_Map;
	// [Num_Vertices], [(null)]
	unsigned short *Vertex_Map;
	// DUPLICATE FIELD NAME Vertex_Map - fixed
	byte Has_Vertex_Weights;
	// [Num_Vertices], [Num_Weights_Per_Vertex]
	float **Vertex_Weights;
	// DUPLICATE FIELD NAME Vertex_Weights - fixed
	// [Num_Strips], [(null)]
	unsigned short *Strip_Lengths;
	byte Has_Faces;
	// [Num_Strips], [Strip_Lengths]
	unsigned short **Strips;
	// DUPLICATE FIELD NAME Strips - fixed
	// [Num_Triangles], [(null)]
	Triangle *Triangles;
	// DUPLICATE FIELD NAME Triangles - fixed
	byte Has_Bone_Indices;
	// [Num_Vertices], [Num_Weights_Per_Vertex]
	byte **Bone_Indices;
} SkinPartition;

typedef struct {
	unsigned int Unknown_Int;
	Vector3 Translation;
	Matrix33 Rotation;
	Vector3 Radius;
} BoundingBox;

typedef struct {
	Vector3 Offset;
	unsigned short Orientation;
	byte Position_Ref_1;
	byte Position_Ref_2;
} FurniturePosition;

typedef struct {
	Triangle Triangle;
	unsigned short Welding_Info;
	Vector3 Normal;
} hkTriangle;

typedef struct {
	string Frame_Name;
	unsigned int Num_Keys;
	unsigned int Interpolation;
	// [Num_Keys], [(null)]
	Key *Keys;
	unsigned int Unknown_Int;
	// DUPLICATE FIELD NAME Unknown_Int
	// [ARG], [(null)]
	Vector3 *Vectors;
} Morph;

typedef struct {
	Vector3 Velocity;
	Vector3 Unknown_Vector;
	float Lifetime;
	float Lifespan;
	float Timestamp;
	unsigned short Unknown_Short;
	unsigned short Vertex_ID;
} Particle;

typedef struct {
	Matrix33 Rotation;
	Vector3 Translation;
	float Scale;
} SkinTransform;

typedef struct {
	SkinTransform Skin_Transform;
	Vector3 Bounding_Sphere_Offset;
	float Bounding_Sphere_Radius;
	// [13], [(null)]
	short *Unknown_13_Shorts;
	unsigned short Num_Vertices;
	// [Num_Vertices], [(null)]
	SkinWeight *Vertex_Weights;
	// DUPLICATE FIELD NAME Vertex_Weights
} SkinData;

typedef struct {
	Vector3 Center;
	float Radius;
} SphereBV;

typedef struct {
	byte Layer;
	byte Col_Filter;
	unsigned short Unknown_Short;
} OblivionColFilter;

typedef struct {
	byte Layer;
	byte Col_Filter;
	unsigned short Unknown_Short;
	unsigned int Num_Vertices;
	unsigned int Material;
} OblivionSubShape;

typedef struct {
	float Unknown_Float_1;
	float Unknown_Float_2;
	float Unknown_Float_3;
	float Unknown_Float_4;
	float Unknown_Float_5;
	float Unknown_Float_6;
	byte Unknown_Byte_1;
} MotorDescriptor;

typedef struct {
	Vector4 Pivot_A;
	Vector4 Plane_A;
	Vector4 Twist_A;
	Vector4 Pivot_B;
	Vector4 Plane_B;
	Vector4 Twist_B;
	// DUPLICATE FIELD NAME Twist_A
	// DUPLICATE FIELD NAME Plane_A
	Vector4 Motor_A;
	// DUPLICATE FIELD NAME Pivot_A
	// DUPLICATE FIELD NAME Twist_B
	// DUPLICATE FIELD NAME Plane_B
	Vector4 Motor_B;
	// DUPLICATE FIELD NAME Pivot_B
	float Cone_Max_Angle;
	float Plane_Min_Angle;
	float Plane_Max_Angle;
	float Twist_Min_Angle;
	float Twist_Max_Angle;
	float Max_Friction;
	byte Enable_Motor;
	MotorDescriptor Motor;
} RagdollDescriptor;

typedef struct {
	Vector4 Pivot_A;
	Vector4 Axle_A;
	Vector4 Perp2_Axle_In_A1;
	Vector4 Perp2_Axle_In_A2;
	Vector4 Pivot_B;
	Vector4 Axle_B;
	Vector4 Perp2_Axle_In_B2;
	// DUPLICATE FIELD NAME Axle_A - fixed
	// DUPLICATE FIELD NAME Perp2_Axle_In_A1 - fixed
	// DUPLICATE FIELD NAME Perp2_Axle_In_A2 - fixed
	// DUPLICATE FIELD NAME Pivot_A - fixed
	// DUPLICATE FIELD NAME Axle_B - fixed
	Vector4 Perp2_Axle_In_B1;
	// DUPLICATE FIELD NAME Perp2_Axle_In_B2 - fixed
	// DUPLICATE FIELD NAME Pivot_B - fixed
	float Min_Angle;
	float Max_Angle;
	float Max_Friction;
	byte Enable_Motor;
	MotorDescriptor Motor;
} LimitedHingeDescriptor;

typedef struct {
	Vector4 Pivot_A;
	Vector4 Perp2_Axle_In_A1;
	Vector4 Perp2_Axle_In_A2;
	Vector4 Pivot_B;
	Vector4 Axle_B;
	Vector4 Axle_A;
	// DUPLICATE FIELD NAME Perp2_Axle_In_A1
	// DUPLICATE FIELD NAME Perp2_Axle_In_A2
	// DUPLICATE FIELD NAME Pivot_A
	// DUPLICATE FIELD NAME Axle_B
	Vector4 Perp2_Axle_In_B1;
	Vector4 Perp2_Axle_In_B2;
	// DUPLICATE FIELD NAME Pivot_B
} HingeDescriptor;

typedef struct {
	float Vertex_Weight;
	unsigned short Vertex_Index;
	Vector3 Unknown_Vector;
} OldSkinData;

typedef struct {
	byte Has_Image;
	unsigned int Image;
	unsigned int Clamp_;
	unsigned int Filter_;
	unsigned int UV_Set_;
	short PS2_L;
	short PS2_K;
	short Unknown_Short_3;
} MultiTextureElement;

typedef struct {
	Vector3 Center;
	// [3], [(null)]
	Vector3 *Axis;
	// [3], [(null)]
	float *Extent;
} BoxBV;

typedef struct {
	Vector3 Center;
	Vector3 Origin;
	float Unknown_Float_1;
	float Unknown_Float_2;
} CapsuleBV;

typedef struct {
	Vector3 Normal;
	Vector3 Center;
} HalfSpaceBV;

typedef struct {
	unsigned int Collision_Type;
	SphereBV Sphere;
	BoxBV Box;
	CapsuleBV Capsule;
	struct UnionBV *Union;
	HalfSpaceBV HalfSpace;
} BoundingVolume;

typedef struct {
	unsigned int Num_BV;
	// [Num_BV], [(null)]
	BoundingVolume *Bounding_Volumes;
} UnionBV;

typedef struct {
	unsigned int Interpolator;
	float Weight_;
} MorphWeight;

typedef struct {
	string Texture_Name;
	int Unknown_Int_3;
	int Unknown_Int_4;
	unsigned int Texturing_Property;
	// [9], [(null)]
	byte *Unknown_Bytes;
} ArkTexture;

typedef struct {
	float m11;
	float m12;
	float m13;
	float m14;
	float m21;
	float m22;
	float m23;
	float m24;
	float m31;
	float m32;
	float m33;
	float m34;
} InertiaMatrix;

typedef struct {
	short Num_Vectors;
	// [Num_Vectors], [(null)]
	Vector3 *Points;
	// [Num_Vectors], [(null)]
	Vector3 *Normals;
} DecalVectorArray;

typedef struct {
	unsigned short Part_Flag;
	unsigned short Body_Part;
} BodyPartList;

typedef struct {
} NiObject;

typedef struct {
	NiObject *parent;
	// [40], [(null)]
	byte *Unknown_1;
	unsigned int Parent;
	unsigned int Num_1;
	unsigned int Num_2;
	// [Num_1], [Num_2]
	unsigned int **Unknown_2;
} Ni3dsAlphaAnimator;

typedef struct {
	NiObject *parent;
	string Name;
	unsigned int Has_Data;
	// [21], [(null)]
	float *Unknown_Floats_1;
	unsigned short Unknown_Short;
	unsigned int Child;
	// [12], [(null)]
	float *Unknown_Floats_2;
	unsigned int Count;
	// [Count], [5]
	byte **Unknown_Array;
} Ni3dsAnimationNode;

typedef struct {
	NiObject *parent;
	// [184], [(null)]
	byte *Unknown_1;
} Ni3dsColorAnimator;

typedef struct {
	NiObject *parent;
	// [14], [(null)]
	byte *Unknown_1;
} Ni3dsMorphShape;

typedef struct {
	NiObject *parent;
	// [14], [(null)]
	byte *Unknown_1;
} Ni3dsParticleSystem;

typedef struct {
	NiObject *parent;
	// [20], [(null)]
	byte *Unknown_1;
} Ni3dsPathController;

typedef struct {
	NiObject *parent;
	unsigned int Next_Modifier;
	unsigned int Controller;
} NiParticleModifier;

typedef struct {
	NiObject *parent;
	float Bounce;
	byte Spawn_on_Collide;
	byte Die_on_Collide;
	unsigned int Spawn_Modifier;
	unsigned int Parent;
	unsigned int Next_Collider;
	unsigned int Collider_Object;
} NiPSysCollider;

typedef struct {
	NiObject *parent;
} bhkRefObject;

typedef struct {
	bhkRefObject *parent;
} bhkSerializable;

typedef struct {
	bhkSerializable *parent;
	unsigned int Shape;
	byte Layer;
	byte Col_Filter;
	unsigned short Unknown_Short;
} bhkWorldObject;

typedef struct {
	bhkWorldObject *parent;
} bhkPhantom;

typedef struct {
	bhkPhantom *parent;
} bhkShapePhantom;

typedef struct {
	bhkShapePhantom *parent;
	// [7], [(null)]
	float *Unkown_Floats;
	// [3], [5]
	float **Unknown_Floats_2;
	float Unknown_Float;
} bhkSimpleShapePhantom;

typedef struct {
	bhkWorldObject *parent;
} bhkEntity;

typedef struct {
	bhkEntity *parent;
	int Unknown_Int_1;
	int Unknown_Int_2;
	// [3], [(null)]
	int *Unknown_3_Ints;
	byte Collision_Response_;
	byte Unknown_Byte;
	unsigned short Process_Contact_Callback_Delay_;
	// [2], [(null)]
	unsigned short *Unknown_2_Shorts;
	byte Layer_Copy;
	byte Col_Filter_Copy;
	// [7], [(null)]
	unsigned short *Unknown_7_Shorts;
	Vector4 Translation;
	QuaternionXYZW Rotation;
	Vector4 Linear_Velocity;
	Vector4 Angular_Velocity;
	InertiaMatrix Inertia;
	Vector4 Center;
	float Mass;
	float Linear_Damping;
	float Angular_Damping;
	float Friction;
	float Restitution;
	float Max_Linear_Velocity;
	float Max_Angular_Velocity;
	float Penetration_Depth;
	byte Motion_System;
	byte Deactivator_Type;
	byte Solver_Deactivation;
	byte Quality_Type;
	unsigned int Auto_Remove_Level;
	unsigned int User_Datas_In_Contact_Point_Properties_;
	unsigned int Force_Collide_Onto_Ppu_;
	unsigned int Num_Constraints;
	// [Num_Constraints], [(null)]
	unsigned int *Constraints;
	unsigned int Unknown_Int_9;
} bhkRigidBody;

typedef struct {
	bhkRigidBody *parent;
} bhkRigidBodyT;

typedef struct {
	bhkSerializable *parent;
	unsigned int Num_Entities;
	// [Num_Entities], [(null)]
	unsigned int *Entities;
	unsigned int Priority;
} bhkConstraint;

typedef struct {
	bhkConstraint *parent;
	LimitedHingeDescriptor Limited_Hinge;
} bhkLimitedHingeConstraint;

typedef struct {
	bhkConstraint *parent;
	unsigned int Type;
	unsigned int Unknown_Int_2;
	unsigned int Unknown_Link_1;
	unsigned int Unknown_Link_2;
	unsigned int Unknown_Int_3;
	HingeDescriptor Hinge;
	RagdollDescriptor Ragdoll;
	LimitedHingeDescriptor Limited_Hinge;
	float Tau;
	float Damping;
} bhkMalleableConstraint;

typedef struct {
	bhkConstraint *parent;
	Vector4 Pivot_A;
	Vector4 Pivot_B;
	float Length;
} bhkStiffSpringConstraint;

typedef struct {
	bhkConstraint *parent;
	RagdollDescriptor Ragdoll;
} bhkRagdollConstraint;

typedef struct {
	bhkConstraint *parent;
	Vector4 Pivot_A;
	// [4], [(null)]
	Vector4 *Rotation_Matrix_A;
	Vector4 Pivot_B;
	Vector4 Sliding_B;
	Vector4 Plane_B;
	Vector4 Sliding_A;
	Vector4 Rotation_A;
	Vector4 Plane_A;
	// DUPLICATE FIELD NAME Pivot_A
	// DUPLICATE FIELD NAME Sliding_B
	Vector4 Rotation_B;
	// DUPLICATE FIELD NAME Plane_B
	// DUPLICATE FIELD NAME Pivot_B
	float Min_Distance;
	float Max_Distance;
	float Friction;
	byte Unknown_Byte_1;
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
	unsigned int Shape;
	unsigned int Material;
	float Unknown_Float_1;
	// [8], [(null)]
	byte *Unknown_8_Bytes;
	Matrix44 Transform;
} bhkTransformShape;

typedef struct {
	bhkShape *parent;
	unsigned int Material;
	float Radius;
} bhkSphereRepShape;

typedef struct {
	bhkSphereRepShape *parent;
} bhkConvexShape;

typedef struct {
	bhkConvexShape *parent;
} bhkSphereShape;

typedef struct {
	bhkConvexShape *parent;
	// [8], [(null)]
	byte *Unknown_8_Bytes;
	Vector3 First_Point;
	float Radius_1;
	Vector3 Second_Point;
	float Radius_2;
} bhkCapsuleShape;

typedef struct {
	bhkConvexShape *parent;
	// [8], [(null)]
	byte *Unknown_8_Bytes;
	Vector3 Dimensions;
	float Minimum_Size;
} bhkBoxShape;

typedef struct {
	bhkConvexShape *parent;
	// [6], [(null)]
	float *Unknown_6_Floats;
	unsigned int Num_Vertices;
	// [Num_Vertices], [(null)]
	Vector4 *Vertices;
	unsigned int Num_Normals;
	// [Num_Normals], [(null)]
	Vector4 *Normals;
} bhkConvexVerticesShape;

typedef struct {
	bhkTransformShape *parent;
} bhkConvexTransformShape;

typedef struct {
	bhkSphereRepShape *parent;
	float Unknown_Float_1;
	float Unknown_Float_2;
	unsigned int Num_Spheres;
	// [Num_Spheres], [(null)]
	SphereBV *Spheres;
} bhkMultiSphereShape;

typedef struct {
	bhkShape *parent;
} bhkBvTreeShape;

typedef struct {
	bhkBvTreeShape *parent;
	unsigned int Shape;
	unsigned int Material;
	unsigned int Unknown_Int_1;
	unsigned int Unknown_Int_2;
	float Unknown_Float;
	unsigned int MOPP_Data_Size;
	Vector3 Origin;
	float Scale;
	// [MOPP_Data_Size_-_1], [(null)]
	 byte /*parent-> or expr. */  * Old_MOPP_Data;
	// [MOPP_Data_Size], [(null)]
	byte *MOPP_Data;
} bhkMoppBvTreeShape;

typedef struct {
	bhkShape *parent;
} bhkShapeCollection;

typedef struct {
	bhkShapeCollection *parent;
	unsigned int Num_Sub_Shapes;
	// [Num_Sub_Shapes], [(null)]
	unsigned int *Sub_Shapes;
	unsigned int Material;
	// [6], [(null)]
	float *Unknown_Floats;
	unsigned int Num_Unknown_Ints;
	// [Num_Unknown_Ints], [(null)]
	unsigned int *Unknown_Ints;
} bhkListShape;

typedef struct {
	bhkShape *parent;
	// [9], [(null)]
	float *Unknown_1;
	int Num_Unknown_Floats;
	// [Num_Unknown_Floats], [3]
	float **Unknown_Floats;
	// [3], [(null)]
	int *Unknown_2;
	unsigned int Num_Strips_Data;
	// [Num_Strips_Data], [(null)]
	unsigned int *Strips_Data;
} bhkMeshShape;

typedef struct {
	bhkShapeCollection *parent;
	unsigned short Num_Sub_Shapes;
	// [Num_Sub_Shapes], [(null)]
	OblivionSubShape *Sub_Shapes;
	unsigned int Unknown_Int_1;
	unsigned int Unknown_Int_2;
	float Unknown_Float_1;
	unsigned int Unknown_Int_3;
	Vector3 Scale_Copy_;
	float Unknown_Float_2;
	float Unknown_Float_3;
	Vector3 Scale;
	float Unknown_Float_4;
	unsigned int Data;
} bhkPackedNiTriStripsShape;

typedef struct {
	bhkShapeCollection *parent;
	unsigned int Material;
	float Unknown_Float_1;
	unsigned int Unknown_Int_1;
	// [4], [(null)]
	unsigned int *Unknown_Ints_1;
	unsigned int Unknown_Int_2;
	Vector3 Scale;
	unsigned int Unknown_Int_3;
	unsigned int Num_Strips_Data;
	// [Num_Strips_Data], [(null)]
	unsigned int *Strips_Data;
	unsigned int Num_Data_Layers;
	// [Num_Data_Layers], [(null)]
	OblivionColFilter *Data_Layers;
} bhkNiTriStripsShape;

typedef struct {
	NiObject *parent;
	string Name;
	unsigned int Next_Extra_Data;
} NiExtraData;

typedef struct {
	NiObject *parent;
} NiInterpolator;

typedef struct {
	NiInterpolator *parent;
} NiKeyBasedInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	float Float_Value;
	unsigned int Data;
} NiFloatInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	Vector3 Translation;
	Quaternion Rotation;
	float Scale;
	// [3], [(null)]
	byte *Unknown_Bytes;
	unsigned int Data;
} NiTransformInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	Vector3 Point_3_Value;
	unsigned int Data;
} NiPoint3Interpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	unsigned short Unknown_Short;
	unsigned int Unknown_Int;
	float Unknown_Float_1;
	float Unknown_Float_2;
	unsigned short Unknown_Short_2;
	unsigned int Pos_Data;
	unsigned int Float_Data;
} NiPathInterpolator;

typedef struct {
	NiKeyBasedInterpolator *parent;
	byte Bool_Value;
	unsigned int Data;
} NiBoolInterpolator;

typedef struct {
	NiBoolInterpolator *parent;
} NiBoolTimelineInterpolator;

typedef struct {
	NiInterpolator *parent;
	unsigned short Unknown_Short;
	unsigned int Unknown_Int;
} NiBlendInterpolator;

typedef struct {
	NiInterpolator *parent;
	float Start_Time;
	float Stop_Time;
	unsigned int Spline_Data;
	unsigned int Basis_Data;
} NiBSplineInterpolator;

typedef struct {
	NiObject *parent;
	string Name;
	unsigned int Has_Old_Extra_Data;
	string Old_Extra_Prop_Name;
	unsigned int Old_Extra_Internal_Id;
	string Old_Extra_String;
	byte Unknown_Byte;
	unsigned int Extra_Data;
	unsigned int Num_Extra_Data_List;
	// [Num_Extra_Data_List], [(null)]
	unsigned int *Extra_Data_List;
	unsigned int Controller;
} NiObjectNET;

typedef struct {
	NiObject *parent;
	unsigned int Target;
} NiCollisionObject;

typedef struct {
	NiCollisionObject *parent;
	unsigned int Propagation_Mode;
	unsigned int Collision_Mode;
	byte Use_ABV;
	BoundingVolume Bounding_Volume;
} NiCollisionData;

typedef struct {
	NiCollisionObject *parent;
	unsigned short Flags;
	unsigned int Body;
} bhkNiCollisionObject;

typedef struct {
	bhkNiCollisionObject *parent;
} bhkCollisionObject;

typedef struct {
	bhkCollisionObject *parent;
	float Unknown_Float_1;
	float Unknown_Float_2;
} bhkBlendCollisionObject;

typedef struct {
	bhkNiCollisionObject *parent;
} bhkPCollisionObject;

typedef struct {
	bhkPCollisionObject *parent;
} bhkSPCollisionObject;

typedef struct {
	NiObjectNET *parent;
	unsigned short Flags;
	unsigned short Unknown_Short_1;
	Vector3 Translation;
	Matrix33 Rotation;
	float Scale;
	Vector3 Velocity;
	unsigned int Num_Properties;
	// [Num_Properties], [(null)]
	unsigned int *Properties;
	// [4], [(null)]
	unsigned int *Unknown_1;
	byte Unknown_2;
	byte Has_Bounding_Box;
	BoundingBox Bounding_Box;
	unsigned int Collision_Object;
} NiAVObject;

typedef struct {
	NiAVObject *parent;
	byte Switch_State;
	unsigned int Num_Affected_Node_List_Pointers;
	unsigned int Num_Affected_Nodes;
	// [Num_Affected_Node_List_Pointers], [(null)]
	unsigned int *Affected_Node_List_Pointers;
	// [Num_Affected_Nodes], [(null)]
	unsigned int *Affected_Nodes;
} NiDynamicEffect;

typedef struct {
	NiDynamicEffect *parent;
	float Dimmer;
	Color3 Ambient_Color;
	Color3 Diffuse_Color;
	Color3 Specular_Color;
} NiLight;

typedef struct {
	NiObjectNET *parent;
} NiProperty;

typedef struct {
	NiProperty *parent;
	// [6], [(null)]
	byte *Unknown;
} NiTransparentProperty;

typedef struct {
	NiObject *parent;
	string Name;
	unsigned int Order;
	unsigned int Target;
	byte Active;
} NiPSysModifier;

typedef struct {
	NiPSysModifier *parent;
	float Speed;
	float Speed_Variation;
	float Declination;
	float Declination_Variation;
	float Planar_Angle;
	float Planar_Angle_Variation;
	Color4 Initial_Color;
	float Initial_Radius;
	float Radius_Variation;
	float Life_Span;
	float Life_Span_Variation;
} NiPSysEmitter;

typedef struct {
	NiPSysEmitter *parent;
	unsigned int Emitter_Object;
} NiPSysVolumeEmitter;

typedef struct {
	NiObject *parent;
	unsigned int Next_Controller;
	unsigned short Flags;
	float Frequency;
	float Phase;
	float Start_Time;
	float Stop_Time;
	unsigned int Target;
	unsigned int Unknown_Integer;
} NiTimeController;

typedef struct {
	NiTimeController *parent;
} NiInterpController;

typedef struct {
	NiInterpController *parent;
	unsigned short Num_Extra_Targets;
	// [Num_Extra_Targets], [(null)]
	unsigned int *Extra_Targets;
} NiMultiTargetTransformController;

typedef struct {
	NiInterpController *parent;
	unsigned short Extra_Flags;
	byte Unknown_2;
	unsigned int Data;
	byte Always_Update;
	unsigned int Num_Interpolators;
	// [Num_Interpolators], [(null)]
	unsigned int *Interpolators;
	// [Num_Interpolators], [(null)]
	MorphWeight *Interpolator_Weights;
	unsigned int Num_Unknown_Ints;
	// [Num_Unknown_Ints], [(null)]
	unsigned int *Unknown_Ints;
} NiGeomMorpherController;

typedef struct {
	NiInterpController *parent;
} NiMorphController;

typedef struct {
	NiInterpController *parent;
	unsigned int Data;
} NiMorpherController;

typedef struct {
	NiInterpController *parent;
	unsigned int Interpolator;
} NiSingleInterpController;

typedef struct {
	NiSingleInterpController *parent;
	unsigned int Data;
} NiKeyframeController;

typedef struct {
	NiKeyframeController *parent;
} NiTransformController;

typedef struct {
	NiSingleInterpController *parent;
	string Modifier_Name;
} NiPSysModifierCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
	unsigned int Data;
	unsigned int Visibility_Interpolator;
} NiPSysEmitterCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
} NiPSysModifierBoolCtlr;

typedef struct {
	NiPSysModifierBoolCtlr *parent;
	unsigned int Data;
} NiPSysModifierActiveCtlr;

typedef struct {
	NiPSysModifierCtlr *parent;
	unsigned int Data;
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
	unsigned int Texture_Slot;
	unsigned int Unknown_Int_2;
	float Delta;
	unsigned int Num_Sources;
	// [Num_Sources], [(null)]
	unsigned int *Sources;
	// [Num_Sources], [(null)]
	unsigned int *Images;
} NiFlipController;

typedef struct {
	NiFloatInterpController *parent;
	unsigned int Data;
} NiAlphaController;

typedef struct {
	NiFloatInterpController *parent;
	byte Unknown2;
	unsigned int Texture_Slot;
	unsigned int Operation;
	unsigned int Data;
} NiTextureTransformController;

typedef struct {
	NiFloatInterpController *parent;
} NiLightDimmerController;

typedef struct {
	NiSingleInterpController *parent;
} NiBoolInterpController;

typedef struct {
	NiBoolInterpController *parent;
	unsigned int Data;
} NiVisController;

typedef struct {
	NiSingleInterpController *parent;
	unsigned short Target_Color;
	unsigned int Data;
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
	string Controller_Data;
	byte Num_Extra_Bytes;
	// [7], [(null)]
	byte *Unknown_Bytes;
	// [Num_Extra_Bytes], [(null)]
	byte *Unknown_Extra_Bytes;
} NiFloatExtraDataController;

typedef struct {
	NiTimeController *parent;
	unsigned int Unknown_Int_1;
	unsigned int Num_Node_Groups;
	unsigned int Num_Node_Groups_2;
	// [Num_Node_Groups], [(null)]
	NodeGroup *Node_Groups;
	unsigned int Num_Shape_Groups;
	// DUPLICATE FIELD NAME Num_Shape_Groups
	// [Num_Shape_Groups], [(null)]
	SkinShapeGroup *Shape_Groups_1;
	// DUPLICATE FIELD NAME Shape_Groups_1
	unsigned int Num_Shape_Groups_2;
	// DUPLICATE FIELD NAME Num_Shape_Groups_2
	// [Num_Shape_Groups_2], [(null)]
	unsigned int *Shape_Groups_2;
	// DUPLICATE FIELD NAME Shape_Groups_2
	int Unknown_Int_2;
	int Unknown_Int_3;
} NiBoneLODController;

typedef struct {
	NiBoneLODController *parent;
} NiBSBoneLODController;

typedef struct {
	NiAVObject *parent;
	unsigned int Data;
	unsigned int Skin_Instance;
	unsigned int Num_Materials;
	// [Num_Materials], [(null)]
	string *Material_Name;
	// [Num_Materials], [(null)]
	int *Material_Extra_Data;
	int Active_Material;
	byte Has_Shader;
	string Shader_Name;
	int Unknown_Integer;
	byte Unknown_Byte;
	int Unknown_Integer_2;
	byte Dirty_Flag;
} NiGeometry;

typedef struct {
	NiGeometry *parent;
} NiTriBasedGeom;

typedef struct {
	NiObject *parent;
	int Unknown_Int;
	unsigned short Num_Vertices;
	byte Keep_Flags;
	byte Compress_Flags;
	byte Has_Vertices;
	// [Num_Vertices], [(null)]
	Vector3 *Vertices;
	unsigned short Num_UV_Sets;
	unsigned short BS_Num_UV_Sets;
	byte Has_Normals;
	// [Num_Vertices], [(null)]
	Vector3 *Normals;
	// [Num_Vertices], [(null)]
	Vector3 *Tangents;
	// [Num_Vertices], [(null)]
	Vector3 *Binormals;
	Vector3 Center;
	float Radius;
	// [13], [(null)]
	short *Unknown_13_shorts;
	byte Has_Vertex_Colors;
	// [Num_Vertices], [(null)]
	Color4 *Vertex_Colors;
	// DUPLICATE FIELD NAME Num_UV_Sets - fixed
	byte Has_UV;
	// [Num_UV_Sets_&_63], [Num_Vertices]
	TexCoord **UV_Sets;
	// DUPLICATE FIELD NAME UV_Sets - fixed
	unsigned short Consistency_Flags;
	unsigned int Additional_Data;
} NiGeometryData;

typedef struct {
	NiGeometryData *parent;
	unsigned short Num_Triangles;
} NiTriBasedGeomData;

typedef struct {
	NiTimeController *parent;
	unsigned int Unknown_Int;
} bhkBlendController;

typedef struct {
	NiExtraData *parent;
	Vector3 Center;
	Vector3 Dimensions;
} BSBound;

typedef struct {
	NiExtraData *parent;
	unsigned int Num_Positions;
	// [Num_Positions], [(null)]
	FurniturePosition *Positions;
} BSFurnitureMarker;

typedef struct {
	NiPSysModifier *parent;
	float Damping;
} BSParentVelocityModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
} BSPSysArrayEmitter;

typedef struct {
	NiPSysModifier *parent;
	float Strength;
} BSWindModifier;

typedef struct {
	bhkShapeCollection *parent;
	unsigned int Num_Triangles;
	// [Num_Triangles], [(null)]
	hkTriangle *Triangles;
	unsigned int Num_Vertices;
	byte Unknown_Byte_1;
	// [Num_Vertices], [(null)]
	Vector3 *Vertices;
	unsigned short Num_Sub_Shapes;
	// [Num_Sub_Shapes], [(null)]
	OblivionSubShape *Sub_Shapes;
} hkPackedNiTriStripsData;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	byte Threshold;
} NiAlphaProperty;

typedef struct {
	NiLight *parent;
} NiAmbientLight;

typedef struct {
	NiObject *parent;
	string Name;
	unsigned short Num_Vertices;
	byte Keep_Flags;
	byte Compress_Flags;
	byte Has_Vertices;
	// [Num_Vertices], [(null)]
	Vector3 *Vertices;
	byte Num_UV_Sets;
	byte TSpace_Flag;
	byte Has_Normals;
	// [Num_Vertices], [(null)]
	Vector3 *Normals;
	// [Num_Vertices], [(null)]
	Vector3 *Tangents;
	// [Num_Vertices], [(null)]
	Vector3 *Binormals;
	Vector3 Center;
	float Radius;
	byte Has_Vertex_Colors;
	// [Num_Vertices], [(null)]
	Color4 *Vertex_Colors;
	// DUPLICATE FIELD NAME Num_UV_Sets - fixed
	// DUPLICATE FIELD NAME TSpace_Flag - fixed
	byte Has_UV;
	// [Num_UV_Sets_&_63], [Num_Vertices]
	TexCoord **UV_Sets;
	unsigned short Consistency_Flags;
	unsigned int Additional_Data;
	unsigned short Num_Particles;
	float Particle_Radius;
	byte Has_Radii;
	// [Num_Vertices], [(null)]
	float *Radii;
	unsigned short Num_Active;
	byte Has_Sizes;
	// [Num_Vertices], [(null)]
	float *Sizes;
	byte Has_Rotations;
	// [Num_Vertices], [(null)]
	Quaternion *Rotations;
	byte Has_Rotation_Angles;
	// [Num_Vertices], [(null)]
	float *Rotation_Angles;
	byte Has_Rotation_Axes;
	// [Num_Vertices], [(null)]
	Vector3 *Rotation_Axes;
	byte Has_Unknown_Stuff_1;
	short Num_Unknown_Stuff_1;
	// [Num_Unknown_Stuff_1], [(null)]
	Vector4 *Unknown_Stuff_1;
} NiParticlesData;

typedef struct {
	NiParticlesData *parent;
	byte Has_Rotations_2;
	// [Num_Vertices], [(null)]
	 Quaternion /*parent-> or expr. */  * Rotations_2;
} NiRotatingParticlesData;

typedef struct {
	NiParticlesData *parent;
} NiAutoNormalParticlesData;

typedef struct {
	Vector3 Translation;
	// [3], [(null)]
	float *Unknown_Floats_1;
	float Unknown_Float_1;
	float Unknown_Float_2;
	float Unknown_Float_3;
	int Unknown_Int_1;
} ParticleDesc;

typedef struct {
	NiRotatingParticlesData *parent;
	// [Num_Vertices], [(null)]
	 ParticleDesc /*parent-> or expr. */  * Particle_Descriptions;
	byte Has_Unknown_Floats_3;
	// [Num_Vertices], [(null)]
	float /*parent-> or expr. */ *Unknown_Floats_3;
	unsigned short Unknown_Short_1;
	unsigned short Unknown_Short_2;
} NiPSysData;

typedef struct {
	NiPSysData *parent;
	unsigned int Unknown_Int_2;
	byte Unknown_Byte_3;
	unsigned int Num_Unknown_Ints_1;
	// [Num_Unknown_Ints_1], [(null)]
	unsigned int *Unknown_Ints_1;
	unsigned int Unknown_Node;
} NiMeshPSysData;

typedef struct {
	NiExtraData *parent;
	ByteArray Binary_Data;
} NiBinaryExtraData;

typedef struct {
	NiExtraData *parent;
	unsigned int Unknown_Int;
	unsigned int Data;
} NiBinaryVoxelExtraData;

typedef struct {
	NiObject *parent;
	unsigned short Unknown_Short_1;
	unsigned short Unknown_Short_2;
	unsigned short Unknown_Short_3;
	// [7], [(null)]
	float *Unknown_7_Floats;
	// [7], [12]
	byte **Unknown_Bytes_1;
	unsigned int Num_Unknown_Vectors;
	// [Num_Unknown_Vectors], [(null)]
	Vector4 *Unknown_Vectors;
	unsigned int Num_Unknown_Bytes_2;
	// [Num_Unknown_Bytes_2], [(null)]
	byte *Unknown_Bytes_2;
	// [5], [(null)]
	unsigned int *Unknown_5_Ints;
} NiBinaryVoxelData;

typedef struct {
	NiBlendInterpolator *parent;
	byte Bool_Value;
} NiBlendBoolInterpolator;

typedef struct {
	NiBlendInterpolator *parent;
	float Float_Value;
} NiBlendFloatInterpolator;

typedef struct {
	NiBlendInterpolator *parent;
	Vector3 Point_Value;
} NiBlendPoint3Interpolator;

typedef struct {
	NiBlendInterpolator *parent;
} NiBlendTransformInterpolator;

typedef struct {
	NiObject *parent;
	KeyGroup Data;
} NiBoolData;

typedef struct {
	NiExtraData *parent;
	byte Boolean_Data;
} NiBooleanExtraData;

typedef struct {
	NiObject *parent;
	unsigned int Num_Control_Points;
} NiBSplineBasisData;

typedef struct {
	NiBSplineInterpolator *parent;
} NiBSplineFloatInterpolator;

typedef struct {
	NiBSplineFloatInterpolator *parent;
	float Base;
	unsigned int Offset;
	float Bias;
	float Multiplier;
} NiBSplineCompFloatInterpolator;

typedef struct {
	NiBSplineInterpolator *parent;
	// [6], [(null)]
	float *Unknown_Floats;
} NiBSplinePoint3Interpolator;

typedef struct {
	NiBSplinePoint3Interpolator *parent;
} NiBSplineCompPoint3Interpolator;

typedef struct {
	NiBSplineInterpolator *parent;
	Vector3 Translation;
	Quaternion Rotation;
	float Scale;
	unsigned int Translation_Offset;
	unsigned int Rotation_Offset;
	unsigned int Scale_Offset;
} NiBSplineTransformInterpolator;

typedef struct {
	NiBSplineTransformInterpolator *parent;
	float Translation_Bias;
	float Translation_Multiplier;
	float Rotation_Bias;
	float Rotation_Multiplier;
	float Scale_Bias;
	float Scale_Multiplier;
} NiBSplineCompTransformInterpolator;

typedef struct {
	NiTransformInterpolator *parent;
} BSRotAccumTransfInterpolator;

typedef struct {
	NiObject *parent;
	unsigned int Num_Float_Control_Points;
	// [Num_Float_Control_Points], [(null)]
	float *Float_Control_Points;
	unsigned int Num_Short_Control_Points;
	// [Num_Short_Control_Points], [(null)]
	short *Short_Control_Points;
} NiBSplineData;

typedef struct {
	NiAVObject *parent;
	unsigned short Unknown_Short;
	float Frustum_Left;
	float Frustum_Right;
	float Frustum_Top;
	float Frustum_Bottom;
	float Frustum_Near;
	float Frustum_Far;
	byte Use_Orthographic_Projection;
	float Viewport_Left;
	float Viewport_Right;
	float Viewport_Top;
	float Viewport_Bottom;
	float LOD_Adjust;
	unsigned int Unknown_Link;
	unsigned int Unknown_Int;
	unsigned int Unknown_Int_2;
	unsigned int Unknown_Int_3;
} NiCamera;

typedef struct {
	NiObject *parent;
	KeyGroup Data;
} NiColorData;

typedef struct {
	NiExtraData *parent;
	Color4 Data;
} NiColorExtraData;

typedef struct {
	NiTimeController *parent;
	byte Cumulative;
	unsigned int Num_Controller_Sequences;
	// [Num_Controller_Sequences], [(null)]
	unsigned int *Controller_Sequences;
	unsigned int Object_Palette;
} NiControllerManager;

typedef struct {
	NiObject *parent;
	string Name;
	string Text_Keys_Name;
	unsigned int Text_Keys;
	int Unknown_Int_4;
	int Unknown_Int_5;
	unsigned int Num_Controlled_Blocks;
	unsigned int Unknown_Int_1;
	// [Num_Controlled_Blocks], [(null)]
	ControllerLink *Controlled_Blocks;
} NiSequence;

typedef struct {
	NiSequence *parent;
	float Weight;
	unsigned int Text_Keys;
	unsigned int Cycle_Type;
	unsigned int Unknown_Int_0;
	float Frequency;
	float Start_Time;
	float Unknown_Float_2;
	float Stop_Time;
	byte Unknown_Byte;
	unsigned int Manager;
	string Target_Name;
	unsigned int String_Palette;
	short Unknown_Short_1;
	short Unknown_Short_2;
	unsigned int Unknown_Int_3;
} NiControllerSequence;

typedef struct {
	NiObject *parent;
} NiAVObjectPalette;

typedef struct {
	NiAVObjectPalette *parent;
	unsigned int Unknown_Int;
	unsigned int Num_Objs;
	// [Num_Objs], [(null)]
	AVObject *Objs;
} NiDefaultAVObjectPalette;

typedef struct {
	NiLight *parent;
} NiDirectionalLight;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
} NiDitherProperty;

typedef struct {
	NiSingleInterpController *parent;
	unsigned int Data;
} NiRollController;

typedef struct {
	NiObject *parent;
	KeyGroup Data;
} NiFloatData;

typedef struct {
	NiExtraData *parent;
	float Float_Data;
} NiFloatExtraData;

typedef struct {
	NiExtraData *parent;
	unsigned int Num_Floats;
	// [Num_Floats], [(null)]
	float *Data;
} NiFloatsExtraData;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	float Fog_Depth;
	Color3 Fog_Color;
} NiFogProperty;

typedef struct {
	NiParticleModifier *parent;
	float Unknown_Float_1;
	float Force;
	unsigned int Type;
	Vector3 Position;
	Vector3 Direction;
} NiGravity;

typedef struct {
	NiExtraData *parent;
	unsigned int Integer_Data;
} NiIntegerExtraData;

typedef struct {
	NiIntegerExtraData *parent;
} BSXFlags;

typedef struct {
	NiExtraData *parent;
	unsigned int Num_Integers;
	// [Num_Integers], [(null)]
	unsigned int *Data;
} NiIntegersExtraData;

typedef struct {
	NiKeyframeController *parent;
	unsigned int Data_2;
} BSKeyframeController;

typedef struct {
	NiObject *parent;
	unsigned int Num_Rotation_Keys;
	unsigned int Rotation_Type;
	// [Num_Rotation_Keys], [(null)]
	QuatKey *Quaternion_Keys;
	float Unknown_Float;
	// [3], [(null)]
	KeyGroup *XYZ_Rotations;
	KeyGroup Translations;
	KeyGroup Scales;
} NiKeyframeData;

typedef struct {
	NiTimeController *parent;
	unsigned short Unknown1;
	unsigned int Look_At_Node;
} NiLookAtController;

typedef struct {
	NiInterpolator *parent;
	unsigned short Unknown_Short;
	unsigned int Look_At;
	string Target;
	Vector3 Translation;
	Quaternion Rotation;
	float Scale;
	unsigned int Unknown_Link_1;
	unsigned int Unknown_Link_2;
	unsigned int Unknown_Link_3;
} NiLookAtInterpolator;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	Color3 Ambient_Color;
	Color3 Diffuse_Color;
	Color3 Specular_Color;
	Color3 Emissive_Color;
	float Glossiness;
	float Alpha;
	float Emit_Multi;
} NiMaterialProperty;

typedef struct {
	NiObject *parent;
	unsigned int Num_Morphs;
	unsigned int Num_Vertices;
	byte Relative_Targets;
	// [Num_Morphs], [(null)]
	Morph *Morphs;
} NiMorphData;

typedef struct {
	NiAVObject *parent;
	unsigned int Num_Children;
	// [Num_Children], [(null)]
	unsigned int *Children;
	unsigned int Num_Effects;
	// [Num_Effects], [(null)]
	unsigned int *Effects;
} NiNode;

typedef struct {
	NiNode *parent;
} NiBone;

typedef struct {
	NiNode *parent;
} AvoidNode;

typedef struct {
	NiNode *parent;
	byte Unknown1;
	// [292], [(null)]
	byte *Unknown_292_Bytes;
} FxWidget;

typedef struct {
	FxWidget *parent;
} FxButton;

typedef struct {
	FxWidget *parent;
	unsigned int Unknown_Int_1;
	unsigned int Unknown_Int__2;
	unsigned int Unknown_Int_3;
	unsigned int Num_Buttons;
	// [Num_Buttons], [(null)]
	unsigned int *Buttons;
} FxRadioButton;

typedef struct {
	NiNode *parent;
	unsigned short Billboard_Mode;
} NiBillboardNode;

typedef struct {
	NiNode *parent;
} NiBSAnimationNode;

typedef struct {
	NiNode *parent;
} NiBSParticleNode;

typedef struct {
	NiNode *parent;
	unsigned short Unknown_Flags_1;
	int Unknown_Int_1;
} NiSwitchNode;

typedef struct {
	NiSwitchNode *parent;
	Vector3 LOD_Center;
	unsigned int Num_LOD_Levels;
	// [Num_LOD_Levels], [(null)]
	LODRange *LOD_Levels;
	unsigned int LOD_Level_Data;
} NiLODNode;

typedef struct {
	NiObject *parent;
	byte Unknown_Byte;
	unsigned int Num_Entries;
	// [256], [(null)]
	ByteColor4 *Palette;
} NiPalette;

typedef struct {
	NiParticleModifier *parent;
	float Decay_;
	float Duration_;
	float DeltaV_;
	float Start_;
	unsigned int Decay_Type_;
	unsigned int Symmetry_Type_;
	Vector3 Position_;
	Vector3 Direction_;
} NiParticleBomb;

typedef struct {
	NiParticleModifier *parent;
	unsigned int Color_Data;
} NiParticleColorModifier;

typedef struct {
	NiParticleModifier *parent;
	float Grow;
	float Fade;
} NiParticleGrowFade;

typedef struct {
	NiParticleModifier *parent;
	unsigned int Num_Particle_Meshes;
	// [Num_Particle_Meshes], [(null)]
	unsigned int *Particle_Meshes;
} NiParticleMeshModifier;

typedef struct {
	NiParticleModifier *parent;
	byte Random_Initial_Axis_;
	Vector3 Initial_Axis_;
	float Rotation_Speed_;
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
	unsigned int Unknown_Link_2;
} NiParticleMeshesData;

typedef struct {
	NiParticles *parent;
	byte World_Space;
	unsigned int Num_Modifiers;
	// [Num_Modifiers], [(null)]
	unsigned int *Modifiers;
} NiParticleSystem;

typedef struct {
	NiParticleSystem *parent;
} NiMeshParticleSystem;

typedef struct {
	NiTimeController *parent;
	unsigned int Old_Speed;
	float Speed;
	float Speed_Random;
	float Vertical_Direction;
	float Vertical_Angle;
	float Horizontal_Direction;
	float Horizontal_Angle;
	Vector3 Unknown_Normal_;
	Color4 Unknown_Color_;
	float Size;
	float Emit_Start_Time;
	float Emit_Stop_Time;
	byte Unknown_Byte;
	unsigned int Old_Emit_Rate;
	float Emit_Rate;
	float Lifetime;
	float Lifetime_Random;
	unsigned short Emit_Flags;
	Vector3 Start_Random;
	unsigned int Emitter;
	unsigned short Unknown_Short_2_;
	float Unknown_Float_13_;
	unsigned int Unknown_Int_1_;
	unsigned int Unknown_Int_2_;
	unsigned short Unknown_Short_3_;
	Vector3 Particle_Velocity;
	Vector3 Particle_Unknown_Vector;
	float Particle_Lifetime;
	unsigned int Particle_Link;
	unsigned int Particle_Timestamp;
	unsigned short Particle_Unknown_Short;
	unsigned short Particle_Vertex_Id;
	unsigned short Num_Particles;
	unsigned short Num_Valid;
	// [Num_Particles], [(null)]
	Particle *Particles;
	unsigned int Unknown_Link;
	unsigned int Particle_Extra;
	unsigned int Unknown_Link_2;
	byte Trailer;
	unsigned int Color_Data;
	float Unknown_Float_1;
	// [Particle_Unknown_Short], [(null)]
	float *Unknown_Floats_2;
} NiParticleSystemController;

typedef struct {
	NiParticleSystemController *parent;
} NiBSPArrayController;

typedef struct {
	NiTimeController *parent;
	unsigned short Unknown_Short_2;
	unsigned int Unknown_Int_1;
	float Unknown_Float_2;
	float Unknown_Float_3;
	unsigned short Unknown_Short;
	unsigned int Pos_Data;
	unsigned int Float_Data;
} NiPathController;

typedef struct {
	unsigned int Type;
	unsigned int Convention;
	byte Bits_Per_Channel;
	byte Unknown_Byte_1;
} ChannelData;

typedef struct {
	NiObject *parent;
	unsigned int Pixel_Format;
	unsigned int Red_Mask;
	unsigned int Green_Mask;
	unsigned int Blue_Mask;
	unsigned int Alpha_Mask;
	byte Bits_Per_Pixel;
	// [3], [(null)]
	byte *Unknown_3_Bytes;
	// [8], [(null)]
	byte *Unknown_8_Bytes;
	unsigned int Unknown_Int;
	// DUPLICATE FIELD NAME Bits_Per_Pixel - fixed
	int Unknown_Int_2;
	unsigned int Unknown_Int_3;
	byte Flags;
	unsigned int Unknown_Int_4;
	byte Unknown_Byte_1;
	// [4], [(null)]
	ChannelData *Channels;
	unsigned int Palette;
	unsigned int Num_Mipmaps;
	unsigned int Bytes_Per_Pixel;
	// [Num_Mipmaps], [(null)]
	MipMap *Mipmaps;
} ATextureRenderData;

typedef struct {
	ATextureRenderData *parent;
	unsigned int Num_Pixels;
	unsigned int Unknown_Int_6;
	unsigned int Num_Faces;
	unsigned int Unknown_Int_7;
	// [Num_Faces], [Num_Pixels]
	byte **Pixel_Data;
} NiPersistentSrcTextureRendererData;

typedef struct {
	ATextureRenderData *parent;
	unsigned int Num_Pixels;
	unsigned int Num_Faces;
	// [Num_Faces], [Num_Pixels]
	byte **Pixel_Data;
	// DUPLICATE FIELD NAME Pixel_Data - fixed
} NiPixelData;

typedef struct {
	NiParticleModifier *parent;
	unsigned short Unknown_Short;
	float Unknown_Float_1;
	float Unknown_Float_2;
	unsigned short Unknown_Short_2;
	float Unknown_Float_3;
	float Unknown_Float_4;
	float Unknown_Float_5;
	float Unknown_Float_6;
	float Unknown_Float_7;
	float Unknown_Float_8;
	float Unknown_Float_9;
	float Unknown_Float_10;
	float Unknown_Float_11;
	float Unknown_Float_12;
	float Unknown_Float_13;
	float Unknown_Float_14;
	float Unknown_Float_15;
	float Unknown_Float_16;
} NiPlanarCollider;

typedef struct {
	NiLight *parent;
	float Constant_Attenuation;
	float Linear_Attenuation;
	float Quadratic_Attenuation;
} NiPointLight;

typedef struct {
	NiObject *parent;
	KeyGroup Data;
} NiPosData;

typedef struct {
	NiPSysModifier *parent;
	byte Spawn_on_Death;
	unsigned int Spawn_Modifier;
} NiPSysAgeDeathModifier;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Bomb_Object;
	Vector3 Bomb_Axis;
	float Decay;
	float Delta_V;
	unsigned int Decay_Type;
	unsigned int Symmetry_Type;
} NiPSysBombModifier;

typedef struct {
	NiPSysModifier *parent;
	unsigned short Update_Skip;
} NiPSysBoundUpdateModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
	float Width;
	float Height;
	float Depth;
} NiPSysBoxEmitter;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Collider;
} NiPSysColliderManager;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Data;
} NiPSysColorModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
	float Radius;
	float Height;
} NiPSysCylinderEmitter;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Parent;
	Vector3 Drag_Axis;
	float Percentage;
	float Range;
	float Range_Falloff;
} NiPSysDragModifier;

typedef struct {
	NiObject *parent;
	KeyGroup Float_Keys_;
	unsigned int Num_Visibility_Keys_;
	// [Num_Visibility_Keys_], [(null)]
	Key *Visibility_Keys_;
} NiPSysEmitterCtlrData;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Gravity_Object;
	Vector3 Gravity_Axis;
	float Decay;
	float Strength;
	unsigned int Force_Type;
	float Turbulence;
	float Turbulence_Scale;
	byte Unknown_Byte;
} NiPSysGravityModifier;

typedef struct {
	NiPSysModifier *parent;
	float Grow_Time;
	unsigned short Grow_Generation;
	float Fade_Time;
	unsigned short Fade_Generation;
	float Base_Scale;
} NiPSysGrowFadeModifier;

typedef struct {
	NiPSysEmitter *parent;
	unsigned int Num_Emitter_Meshes;
	// [Num_Emitter_Meshes], [(null)]
	unsigned int *Emitter_Meshes;
	unsigned int Initial_Velocity_Type;
	unsigned int Emission_Type;
	Vector3 Emission_Axis;
} NiPSysMeshEmitter;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Num_Meshes;
	// [Num_Meshes], [(null)]
	unsigned int *Meshes;
} NiPSysMeshUpdateModifier;

typedef struct {
	NiPSysCollider *parent;
	float Width;
	float Height;
	Vector3 X_Axis;
	Vector3 Y_Axis;
} NiPSysPlanarCollider;

typedef struct {
	NiPSysCollider *parent;
	float Radius;
} NiPSysSphericalCollider;

typedef struct {
	NiPSysModifier *parent;
} NiPSysPositionModifier;

typedef struct {
	NiTimeController *parent;
} NiPSysResetOnLoopCtlr;

typedef struct {
	NiPSysModifier *parent;
	float Initial_Rotation_Speed;
	float Initial_Rotation_Speed_Variation;
	float Initial_Rotation_Angle;
	float Initial_Rotation_Angle_Variation;
	byte Random_Rot_Speed_Sign;
	byte Random_Initial_Axis;
	Vector3 Initial_Axis;
} NiPSysRotationModifier;

typedef struct {
	NiPSysModifier *parent;
	unsigned short Num_Spawn_Generations;
	float Percentage_Spawned;
	unsigned short Min_Num_to_Spawn;
	unsigned short Max_Num_to_Spawn;
	float Spawn_Speed_Chaos;
	float Spawn_Dir_Chaos;
	float Life_Span;
	float Life_Span_Variation;
	int Unknown_int;
} NiPSysSpawnModifier;

typedef struct {
	NiPSysVolumeEmitter *parent;
	float Radius;
} NiPSysSphereEmitter;

typedef struct {
	NiTimeController *parent;
} NiPSysUpdateCtlr;

typedef struct {
	NiPSysModifier *parent;
	unsigned int Field_Object;
	float Magnitude;
	float Attenuation;
	byte Use_Max_Distance;
	float Max_Distance;
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
	byte Use_Direction_;
	Vector3 Direction;
} NiPSysDragFieldModifier;

typedef struct {
	NiPSysFieldModifier *parent;
	float Frequency;
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
	float Unknown_Float_2;
	float Unknown_Float_3;
	byte Unknown_Boolean_1;
	byte Unknown_Boolean_2;
	byte Unknown_Boolean_3;
	float Unknown_Float_4;
} NiPSysAirFieldModifier;

typedef struct {
	NiPSysEmitter *parent;
	int Unknown_Int_1;
	float Unknown_Float_1;
	float Unknown_Float_2;
	float Unknown_Float_3;
	int Unknown_Int_2;
	float Unknown_Float_4;
	int Unknown_Int_3;
	float Unknown_Float_5;
	int Unknown_Int_4;
	float Unknown_Float_6;
	float Unknown_Float_7;
} NiPSysTrailEmitter;

typedef struct {
	NiFloatInterpController *parent;
} NiLightIntensityController;

typedef struct {
	NiPSysFieldModifier *parent;
	int Radial_Type;
} NiPSysRadialFieldModifier;

typedef struct {
	NiObject *parent;
} NiLODData;

typedef struct {
	NiLODData *parent;
	Vector3 LOD_Center;
	unsigned int Num_LOD_Levels;
	// [Num_LOD_Levels], [(null)]
	LODRange *LOD_Levels;
} NiRangeLODData;

typedef struct {
	NiLODData *parent;
	Vector3 Bound_Center;
	float Bound_Radius;
	Vector3 World_Center;
	float World_Radius;
	unsigned int Proportion_Count;
	// [Proportion_Count], [(null)]
	float *Proportion_Levels;
} NiScreenLODData;

typedef struct {
	NiParticles *parent;
} NiRotatingParticles;

typedef struct {
	NiObjectNET *parent;
} NiSequenceStreamHelper;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
} NiShadeProperty;

typedef struct {
	NiObject *parent;
	SkinTransform Skin_Transform;
	unsigned int Num_Bones;
	unsigned int Skin_Partition;
	byte Has_Vertex_Weights;
	// [Num_Bones], [(null)]
	SkinData *Bone_List;
} NiSkinData;

typedef struct {
	NiObject *parent;
	unsigned int Data;
	unsigned int Skin_Partition;
	unsigned int Skeleton_Root;
	unsigned int Num_Bones;
	// [Num_Bones], [(null)]
	unsigned int *Bones;
} NiSkinInstance;

typedef struct {
	NiTimeController *parent;
	unsigned int Num_Bones;
	// [Num_Bones], [(null)]
	unsigned int *Vertex_Counts;
	// [Num_Bones], [(null)]
	unsigned int *Bones;
	// [Num_Bones], [Vertex_Counts]
	OldSkinData **Bone_Data;
} NiTriShapeSkinController;

typedef struct {
	NiSkinInstance *parent;
} NiClodSkinInstance;

typedef struct {
	NiObject *parent;
	unsigned int Num_Skin_Partition_Blocks;
	// [Num_Skin_Partition_Blocks], [(null)]
	SkinPartition *Skin_Partition_Blocks;
} NiSkinPartition;

typedef struct {
	NiObjectNET *parent;
} NiTexture;

typedef struct {
	NiTexture *parent;
	byte Use_External;
	FilePath File_Name;
	unsigned int Unknown_Link;
	byte Unknown_Byte;
	// DUPLICATE FIELD NAME File_Name
	unsigned int Pixel_Data;
	unsigned int Pixel_Layout;
	unsigned int Use_Mipmaps;
	unsigned int Alpha_Format;
	byte Is_Static;
	byte Direct_Render;
	byte Persist_Render_Data;
} NiSourceTexture;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
} NiSpecularProperty;

typedef struct {
	NiParticleModifier *parent;
	float Unknown_Float_1;
	unsigned short Unknown_Short_1;
	float Unknown_Float_2;
	unsigned short Unknown_Short_2;
	float Unknown_Float_3;
	float Unknown_Float_4;
	float Unknown_Float_5;
} NiSphericalCollider;

typedef struct {
	NiPointLight *parent;
	float Cutoff_Angle;
	float Unknown_Float;
	float Exponent;
} NiSpotLight;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	byte Stencil_Enabled;
	unsigned int Stencil_Function;
	unsigned int Stencil_Ref;
	unsigned int Stencil_Mask;
	unsigned int Fail_Action;
	unsigned int Z_Fail_Action;
	unsigned int Pass_Action;
	unsigned int Draw_Mode;
	// DUPLICATE FIELD NAME Flags - fixed
	// DUPLICATE FIELD NAME Stencil_Ref - fixed
	// DUPLICATE FIELD NAME Stencil_Mask - fixed
} NiStencilProperty;

typedef struct {
	NiExtraData *parent;
	unsigned int Bytes_Remaining;
	string String_Data;
} NiStringExtraData;

typedef struct {
	NiObject *parent;
	StringPalette Palette;
} NiStringPalette;

typedef struct {
	NiExtraData *parent;
	unsigned int Num_Strings;
	// [Num_Strings], [(null)]
	string *Data;
} NiStringsExtraData;

typedef struct {
	NiExtraData *parent;
	unsigned int Unknown_Int_1;
	unsigned int Num_Text_Keys;
	// [Num_Text_Keys], [(null)]
	Key *Text_Keys;
} NiTextKeyExtraData;

typedef struct {
	NiDynamicEffect *parent;
	Matrix33 Model_Projection_Matrix;
	Vector3 Model_Projection_Transform;
	unsigned int Texture_Filtering;
	unsigned int Texture_Clamping;
	short Unknown;
	unsigned int Texture_Type;
	unsigned int Coordinate_Generation_Type;
	unsigned int Image;
	unsigned int Source_Texture;
	byte Clipping_Plane;
	Vector3 Unknown_Vector;
	float Unknown_Float;
	short PS2_L;
	short PS2_K;
	unsigned short Unknown_Short;
} NiTextureEffect;

typedef struct {
	NiProperty *parent;
	short Unknown_Short;
	short PS2_L;
	short PS2_K;
} NiTextureModeProperty;

typedef struct {
	NiObject *parent;
	byte Use_External;
	FilePath File_Name;
	unsigned int Image_Data;
	unsigned int Unknown_Int;
	float Unknown_Float;
} NiImage;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	unsigned int Image;
	unsigned int Unknown_Int_1;
	unsigned int Unknown_Int_2;
} NiTextureProperty;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	unsigned int Unknown_Int;
	// [5], [(null)]
	MultiTextureElement *Texture_Elements;
} NiMultiTextureProperty;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	// DUPLICATE FIELD NAME Flags - fixed
	unsigned int Apply_Mode;
	unsigned int Texture_Count;
	byte Has_Base_Texture;
	TexDesc Base_Texture;
	byte Has_Dark_Texture;
	TexDesc Dark_Texture;
	byte Has_Detail_Texture;
	TexDesc Detail_Texture;
	byte Has_Gloss_Texture;
	TexDesc Gloss_Texture;
	byte Has_Glow_Texture;
	TexDesc Glow_Texture;
	byte Has_Bump_Map_Texture;
	TexDesc Bump_Map_Texture;
	float Bump_Map_Luma_Scale;
	float Bump_Map_Luma_Offset;
	Matrix22 Bump_Map_Matrix;
	byte Has_Normal_Texture;
	TexDesc Normal_Texture;
	byte Has_Unknown2_Texture;
	TexDesc Unknown2_Texture;
	float Unknown2_Float;
	byte Has_Decal_0_Texture;
	TexDesc Decal_0_Texture;
	byte Has_Decal_1_Texture;
	// DUPLICATE FIELD NAME Has_Decal_1_Texture - fixed
	TexDesc Decal_1_Texture;
	byte Has_Decal_2_Texture;
	// DUPLICATE FIELD NAME Has_Decal_2_Texture - fixed
	TexDesc Decal_2_Texture;
	byte Has_Decal_3_Texture;
	// DUPLICATE FIELD NAME Has_Decal_3_Texture - fixed
	TexDesc Decal_3_Texture;
	unsigned int Num_Shader_Textures;
	// [Num_Shader_Textures], [(null)]
	ShaderTexDesc *Shader_Textures;
} NiTexturingProperty;

typedef struct {
	NiKeyframeData *parent;
} NiTransformData;

typedef struct {
	NiTriBasedGeom *parent;
} NiTriShape;

typedef struct {
	NiTriBasedGeomData *parent;
	unsigned int Num_Triangle_Points;
	byte Has_Triangles;
	// [Num_Triangles], [(null)]
	 Triangle /*parent-> or expr. */  * Triangles;
	// DUPLICATE FIELD NAME Triangles - fixed
	unsigned short Num_Match_Groups;
	// [Num_Match_Groups], [(null)]
	MatchGroup *Match_Groups;
} NiTriShapeData;

typedef struct {
	NiTriBasedGeom *parent;
} NiTriStrips;

typedef struct {
	NiTriBasedGeomData *parent;
	unsigned short Num_Strips;
	// [Num_Strips], [(null)]
	unsigned short *Strip_Lengths;
	byte Has_Points;
	// [Num_Strips], [Strip_Lengths]
	unsigned short **Points;
	// DUPLICATE FIELD NAME Points - fixed
} NiTriStripsData;

typedef struct {
	NiObjectNET *parent;
	unsigned short Unknown_1;
	Matrix44 Unknown_Matrix;
	unsigned int Num_Children;
	// [Num_Children], [(null)]
	unsigned int *Children;
	unsigned int Child_2;
	unsigned int Child_3;
} NiEnvMappedTriShape;

typedef struct {
	NiTriShapeData *parent;
} NiEnvMappedTriShapeData;

typedef struct {
	NiObject *parent;
	// [6], [(null)]
	unsigned int *Unknown_1;
	unsigned short Unknown_2;
	Matrix33 Matrix;
	Vector3 Vector_1;
	Vector3 Vector_2;
	// [4], [(null)]
	short *Unknown_3;
	byte Unknown_4;
	unsigned int Unknown_5;
	// [24], [(null)]
	short *Unknown_6;
} NiBezierTriangle4;

typedef struct {
	NiAVObject *parent;
	unsigned int Num_Bezier_Triangles;
	// [Num_Bezier_Triangles], [(null)]
	unsigned int *Bezier_Triangle;
	unsigned int Unknown_1;
	unsigned short Count_1;
	unsigned short Unknown_2;
	// [Count_1], [(null)]
	Vector3 *Points_1;
	unsigned int Unknown_3;
	// [Count_1], [2]
	float **Points_2;
	unsigned int Unknown_4;
	unsigned short Count_2;
	// [Count_2], [4]
	unsigned short **Data_2;
} NiBezierMesh;

typedef struct {
	NiTriBasedGeom *parent;
} NiClod;

typedef struct {
	NiTriBasedGeomData *parent;
	unsigned short Unknown_Shorts;
	unsigned short Unknown_Count_1;
	unsigned short Unknown_Count_2;
	unsigned short Unknown_Count_3;
	float Unknown_Float;
	unsigned short Unknown_Short;
	// [Unknown_Count_1], [6]
	unsigned short **Unknown_Clod_Shorts_1;
	// [Unknown_Count_2], [(null)]
	unsigned short *Unknown_Clod_Shorts_2;
	// [Unknown_Count_3], [6]
	unsigned short **Unknown_Clod_Shorts_3;
} NiClodData;

typedef struct {
	NiTimeController *parent;
	unsigned short Unknown_Short;
	unsigned int Data;
} NiUVController;

typedef struct {
	NiObject *parent;
	// [4], [(null)]
	KeyGroup *UV_Groups;
} NiUVData;

typedef struct {
	NiExtraData *parent;
	Vector3 Vector_Data;
	float Unknown_Float;
} NiVectorExtraData;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	unsigned int Vertex_Mode;
	unsigned int Lighting_Mode;
} NiVertexColorProperty;

typedef struct {
	NiExtraData *parent;
	unsigned int Num_Bytes;
	unsigned short Num_Vertices;
	// [Num_Vertices], [(null)]
	float *Weight;
} NiVertWeightsExtraData;

typedef struct {
	NiObject *parent;
	unsigned int Num_Keys;
	// [Num_Keys], [(null)]
	Key *Keys;
} NiVisData;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
} NiWireframeProperty;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	unsigned int Function;
} NiZBufferProperty;

typedef struct {
	NiNode *parent;
} RootCollisionNode;

typedef struct {
	NiObject *parent;
	unsigned int Width;
	unsigned int Height;
	unsigned int Image_Type;
	// [Width], [Height]
	ByteColor3 **RGB_Image_Data;
	// [Width], [Height]
	ByteColor4 **RGBA_Image_Data;
} NiRawImageData;

typedef struct {
	NiNode *parent;
	unsigned int Sorting_Mode;
	int Unknown_Int_2;
} NiSortAdjustNode;

typedef struct {
	NiSourceTexture *parent;
} NiSourceCubeMap;

typedef struct {
	NiObjectNET *parent;
	float Unknown_Float_1;
	unsigned int Unknown_Int_1;
	// [Unknown_Int_1], [(null)]
	unsigned int *Unknown_Refs_1;
	int Num_Dests;
	// [Num_Dests], [(null)]
	unsigned int *Transform_Dests;
	byte Unknown_Byte;
	int Unknown_Int;
	unsigned int Prop_Description;
} NiPhysXProp;

typedef struct {
	byte Number;
	byte Unknown_Byte_1;
	unsigned int Material_Desc;
} physXMaterialRef;

typedef struct {
	NiObject *parent;
	int Num_Dests;
	// [Num_Dests], [(null)]
	unsigned int *Actor_Descs;
	unsigned int Num_Joints;
	// [Num_Joints], [(null)]
	unsigned int *Joint_Descs;
	int Unknown_Int_1;
	unsigned int Num_Materials;
	// [Num_Materials], [(null)]
	physXMaterialRef *Material_Descs;
	unsigned int Unknown_Int_2;
	unsigned int Unknown_Int_3;
	string Unknown_String_4;
	unsigned int Unknown_Int_5;
	byte Unknown_Byte_6;
} NiPhysXPropDesc;

typedef struct {
	NiObject *parent;
	int Unknown_Int_1;
	int Unknown_Int_2;
	Quaternion Unknown_Quat_1;
	Quaternion Unknown_Quat_2;
	Quaternion Unknown_Quat_3;
	unsigned int Unknown_Ref_0;
	float Unknown_Int_4;
	int Unknown_Int_5;
	byte Unknown_Byte_1;
	byte Unknown_Byte_2;
	int Unknown_Int_6;
	unsigned int Shape_Description;
	unsigned int Unknown_Ref_1;
	unsigned int Unknown_Ref_2;
	// [Unknown_Int_6], [(null)]
	unsigned int *Unknown_Refs_3;
} NiPhysXActorDesc;

typedef struct {
	NiObject *parent;
	// [136], [(null)]
	byte *Unknown_Bytes;
} NiPhysXBodyDesc;

typedef struct {
	NiObject *parent;
	// [388], [(null)]
	byte *Unknown_Bytes;
} NiPhysXD6JointDesc;

typedef struct {
	NiObject *parent;
	int Unknown_Int_1;
	Quaternion Unknown_Quat_1;
	Quaternion Unknown_Quat_2;
	Quaternion Unknown_Quat_3;
	short Unknown_Short_1;
	int Unknown_Int_2;
	short Unknown_Short_2;
	float Unknown_Float_1;
	float Unknown_Float_2;
	float Unknown_Float_3;
	int Unknown_Int_3;
	int Unknown_Int_4;
	int Unknown_Int_5;
	int Unknown_Int_7;
	int Unknown_Int_8;
	unsigned int Mesh_Description;
} NiPhysXShapeDesc;

typedef struct {
	NiObject *parent;
	short Unknown_Short_1;
	float Unknown_Float_1;
	short Unknown_Short_2;
	// [3], [(null)]
	byte *Unknown_Bytes_1;
	byte Unknown_Byte_1;
	// DUPLICATE FIELD NAME Unknown_Bytes_1
	// [8], [(null)]
	byte *Unknown_Bytes_2;
	float Unknown_Float_2;
	int Unknown_Int_1;
	int Unknown_Int_2;
	int Num_Vertices;
	int Unknown_Int_4;
	// [Num_Vertices], [(null)]
	Vector3 *Vertices;
	// [982], [(null)]
	byte *Unknown_Bytes_3;
	// [368], [(null)]
	short *Unknown_Shorts_1;
	// [3328], [(null)]
	unsigned int *Unknown_Ints_1;
	byte Unknown_Byte_2;
} NiPhysXMeshDesc;

typedef struct {
	NiObject *parent;
	// [12], [(null)]
	unsigned int *Unknown_Int;
	byte Unknown_Byte_1;
	byte Unknown_Byte_2;
} NiPhysXMaterialDesc;

typedef struct {
	NiObject *parent;
	// [6], [(null)]
	byte *Unknown_Bytes;
} NiPhysXKinematicSrc;

typedef struct {
	NiObject *parent;
	byte Unknown_Byte_1;
	byte Unknown_Byte_2;
	unsigned int Node;
} NiPhysXTransformDest;

typedef struct {
	NiExtraData *parent;
	// [4], [(null)]
	int *Unknown_Ints;
	// [37], [(null)]
	byte *Unknown_Bytes;
} NiArkAnimationExtraData;

typedef struct {
	NiExtraData *parent;
	int Unknown_Int_1;
	int Unknown_Int_2;
	string Importer_Name;
	// [13], [(null)]
	byte *Unknown_Bytes;
	// [7], [(null)]
	float *Unknown_Floats;
} NiArkImporterExtraData;

typedef struct {
	NiExtraData *parent;
	// [2], [(null)]
	int *Unknown_Ints_1;
	byte Unknown_Byte;
	int Unknown_Int_2;
	int Num_Textures;
	// [Num_Textures], [(null)]
	ArkTexture *Textures;
} NiArkTextureExtraData;

typedef struct {
	NiExtraData *parent;
	// [13], [(null)]
	byte *Unknown_Bytes;
} NiArkViewportInfoExtraData;

typedef struct {
	NiExtraData *parent;
	int Unknown_Int;
	string Unknown_String;
} NiArkShaderExtraData;

typedef struct {
	NiTriBasedGeom *parent;
} NiLines;

typedef struct {
	NiGeometryData *parent;
	// [Num_Vertices], [(null)]
	 byte /*parent-> or expr. */  * Lines;
} NiLinesData;

typedef struct {
	unsigned short Num_Vertices;
	unsigned short Vertex_Offset;
	unsigned short Num_Triangles;
	unsigned short Triangle_Offset;
} Polygon;

typedef struct {
	NiTriShapeData *parent;
	unsigned short Max_Polygons;
	// [Max_Polygons], [(null)]
	Polygon *Polygons;
	// [Max_Polygons], [(null)]
	unsigned short *Polygon_Indices;
	unsigned short Unknown_UShort_1;
	unsigned short Num_Polygons;
	unsigned short Used_Vertices;
	unsigned short Unknown_UShort_2;
	unsigned short Used_Triangle_Points;
	unsigned short Unknown_UShort_3;
} NiScreenElementsData;

typedef struct {
	NiTriShape *parent;
} NiScreenElements;

typedef struct {
	NiNode *parent;
	unsigned int Shell_Link;
	int Num_Rooms;
	// [Num_Rooms], [(null)]
	unsigned int *Rooms;
} NiRoomGroup;

typedef struct {
	NiNode *parent;
	int Num_Walls;
	// [Num_Walls], [(null)]
	Vector4 *Wall_Plane;
	int Num_In_Portals;
	// [Num_In_Portals], [(null)]
	unsigned int *In_Portals;
	int Num_Portals_2;
	// [Num_Portals_2], [(null)]
	unsigned int *Portals_2;
	int Num_Items;
	// [Num_Items], [(null)]
	unsigned int *Items;
} NiRoom;

typedef struct {
	NiAVObject *parent;
	unsigned short Unknown_Flags;
	short Unknown_Short_1;
	unsigned short Num_Vertices;
	// [Num_Vertices], [(null)]
	Vector3 *Vertices;
	unsigned int Target;
} NiPortal;

typedef struct {
	NiNode *parent;
} BSFadeNode;

typedef struct {
	NiProperty *parent;
	unsigned short Flags;
	unsigned int Shader_Type;
	unsigned int Shader_Flags;
	int Unknown_Int_2;
	float Envmap_Scale;
} BSShaderProperty;

typedef struct {
	BSShaderProperty *parent;
	int Unknown_Int_3;
} BSShaderLightingProperty;

typedef struct {
	BSShaderLightingProperty *parent;
	SizedString File_Name;
	float Unknown_Float_2;
	float Unknown_Float_3;
	float Unknown_Float_4;
	float Unknown_Float_5;
} BSShaderNoLightingProperty;

typedef struct {
	BSShaderLightingProperty *parent;
	unsigned int Texture_Set;
	float Unknown_Float_2;
	int Refraction_Period;
	float Unknown_Float_4;
	float Unknown_Float_5;
} BSShaderPPLightingProperty;

typedef struct {
	NiObject *parent;
	int Num_Textures;
	// [Num_Textures], [(null)]
	SizedString *Textures;
} BSShaderTextureSet;

typedef struct {
	BSShaderProperty *parent;
} WaterShaderProperty;

typedef struct {
	BSShaderProperty *parent;
	int Unknown_Int_4;
	SizedString File_Name;
	int Unknown_Int_5;
} SkyShaderProperty;

typedef struct {
	BSShaderLightingProperty *parent;
	SizedString File_Name;
} TileShaderProperty;

typedef struct {
	BSShaderProperty *parent;
} DistantLODShaderProperty;

typedef struct {
	BSShaderProperty *parent;
} BSDistantTreeShaderProperty;

typedef struct {
	BSShaderProperty *parent;
	SizedString File_Name;
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
	int Num_Partitions;
	// [Num_Partitions], [(null)]
	BodyPartList *Partitions;
} BSDismemberSkinInstance;

typedef struct {
	NiExtraData *parent;
	float Unknown_Float_1;
	short Num_Vector_Blocks;
	// [Num_Vector_Blocks], [(null)]
	DecalVectorArray *Vector_Blocks;
} BSDecalPlacementVectorExtraData;

typedef struct {
	NiPSysModifier *parent;
	float Fade_In_Percent;
	float Fade_out_Percent;
	float Color_1_End_Percent;
	float Color_1_Start_Percent;
	float Color_2_End_Percent;
	float Color_2_Start_Percent;
	// [3], [(null)]
	Color4 *Colors;
} BSPSysSimpleColorModifier;

typedef struct {
	NiNode *parent;
	int Value;
	byte Unknown_byte;
} BSValueNode;

typedef struct {
	NiParticleSystem *parent;
} BSStripParticleSystem;

typedef struct {
	NiPSysData *parent;
	short Unknown_Short_1;
	byte Unknown_byte_1;
	int Unknown_Int_2;
	int Unknown_Int_3;
} BSStripPSysData;

typedef struct {
	NiPSysModifier *parent;
	float Update_Delta_Time;
} BSPSysStripUpdateModifier;

typedef struct {
	NiFloatInterpController *parent;
} BSMaterialEmittanceMultController;

typedef struct {
	NiNode *parent;
	unsigned short Max_Emitter_Objects;
	int Num_Particle_Systems;
	// [Num_Particle_Systems], [(null)]
	unsigned int *Particle_Systems;
} BSMasterParticleSystem;

typedef struct {
	NiPSysModifierCtlr *parent;
	unsigned int Data;
	unsigned int Visibility_Interpolator;
	short Unknown_Short_1;
	int Unknown_Int_1;
} BSPSysMultiTargetEmitterCtlr;

typedef struct {
	NiFloatInterpController *parent;
} BSRefractionStrengthController;

typedef struct {
	NiNode *parent;
	Vector4 Alpha_Sort_Bound;
	byte Is_Static_Bound;
} BSOrderedNode;

typedef struct {
	NiNode *parent;
	byte Unknown_Byte_1;
	short Unknown_Short_2;
} BSBlastNode;

typedef struct {
	NiNode *parent;
	byte Unknown_Byte_1;
	short Unknown_Short_2;
} BSDamageStage;

typedef struct {
	NiTimeController *parent;
	unsigned int Interpolator;
} BSRefractionFirePeriodController;

typedef struct {
	bhkShape *parent;
	unsigned int Num_Sub_Shapes;
	// [Num_Sub_Shapes], [(null)]
	unsigned int *Sub_Shapes;
	unsigned int Material;
	// [6], [(null)]
	float *Unknown_Floats;
	byte Unknown_Byte_1;
	float Unknown_Float_1;
} bhkConvexListShape;

typedef struct {
	int Unknown_Int_1;
	int Unknown_Int_2;
	int Unknown_Int_3;
	int Unknown_Int_4;
	int Unknown_Int_5;
	int Unknown_Int_6;
	int Unknown_Int_7;
	int Unknown_Int_8;
} BSTreadTransfSubInfo;

typedef struct {
	float Unknown_Float_1;
	// [2], [(null)]
	BSTreadTransfSubInfo *Data;
} BSTreadTransfInfo;

typedef struct {
	NiInterpolator *parent;
	int Num_Transfers;
	// [Num_Transfers], [(null)]
	BSTreadTransfInfo *Tread_Transfer_Info;
	int Unknown_Int_1;
} BSTreadTransfInterpolator;

typedef struct {
	NiObject *parent;
	short Unknown_Short_1;
} BSAnimNotes;

typedef struct {
	bhkSerializable *parent;
	int Unknown_Int_1;
	int Unknown_Int_2;
	int Unknown_Int_3;
	float Unknown_Float_1;
	float Unknown_Float_2;
	float Unknown_Float_3;
	float Unknown_Float_4;
} bhkLiquidAction;

typedef struct {
	NiNode *parent;
	unsigned int Multi_Bound;
} BSMultiBoundNode;

typedef struct {
	NiObject *parent;
	unsigned int Data;
} BSMultiBound;

typedef struct {
	NiObject *parent;
} BSMultiBoundData;

typedef struct {
	BSMultiBoundData *parent;
	int Unknown_Int_1;
	int Unknown_Int_2;
	int Unknown_Int_3;
	float Radius;
} BSMultiBoundSphere;

typedef struct {
	int Unknown_Int_1;
	int Unknown_Int_2;
	byte Unknown_Byte_1;
} BSSegmentedTriangle;

typedef struct {
	NiTriShape *parent;
	int Num_Seg_Triangles;
	// [Num_Seg_Triangles], [(null)]
	BSSegmentedTriangle *Seg_Triangles;
} BSSegmentedTriShape;

typedef struct {
	BSMultiBoundData *parent;
	// [6], [(null)]
	float *Unknown_Floats_1;
} BSMultiBoundAABB;

typedef struct {
	int Data_Type;
	int Block_Size;
	int Num_Blocks;
	int Block_Size_2;
	int Block_Index;
	int Unknown_Int_1;
	byte Unknown_Byte_1;
} AdditionalDataInfo;

typedef struct {
	byte Has_Data;
	int Block_Size;
	int Num_Blocks;
	// [Num_Blocks], [(null)]
	int *Block_Offsets;
	int Num_Data;
	// [Num_Data], [(null)]
	int *Data_Sizes;
	// [Num_Data], [Block_Size]
	byte **Data;
} AdditionalDataBlock;

typedef struct {
	NiObject *parent;
	unsigned short Num_Vertices;
	unsigned int Num_Block_Infos;
	// [Num_Block_Infos], [(null)]
	AdditionalDataInfo *Block_Infos;
	int Num_Blocks;
	// [Num_Blocks], [(null)]
	AdditionalDataBlock *Blocks;
} NiAdditionalGeometryData;

typedef struct {
	NiExtraData *parent;
	int Num_Items;
	// [Num_Items], [(null)]
	int *Items;
} BSWArray;

typedef struct {
	bhkShapePhantom *parent;
	// [15], [(null)]
	int *Unknown_Ints_1;
} bhkAabbPhantom;

typedef struct {
	NiTimeController *parent;
	unsigned int Interpolator;
} BSFrustumFOVController;

typedef struct {
	NiNode *parent;
	byte Unknown_byte_1;
	short Unknown_Short_1;
} BSDebrisNode;

typedef struct {
	bhkConstraint *parent;
	// [41], [(null)]
	int *Unknown_Ints_1;
	short Unknown_Short_1;
} bhkBreakableConstraint;

typedef struct {
	bhkSerializable *parent;
	// [17], [(null)]
	int *Unknown_Ints_1;
} bhkOrientHingedBodyAction;

typedef struct {
	unsigned int Start_Index;
	unsigned int Num_Indices;
} Region;

typedef struct {
	NiObject *parent;
	unsigned int Usage;
	unsigned int Access;
	unsigned int Num_Bytes;
	unsigned int Cloning_Behavior;
	unsigned int Num_Regions;
	// [Num_Regions], [(null)]
	Region *Regions;
	unsigned int Num_Components;
	// [Num_Components], [(null)]
	unsigned int *Component_Formats;
	// [Num_Bytes], [(null)]
	byte *Data;
	byte Streamable;
} NiDataStream;

typedef struct {
	string Name;
	unsigned int Index;
} SemanticData;

typedef struct {
	unsigned int Stream;
	byte Is_Per_Instance;
	unsigned short Num_Submeshes;
	// [Num_Submeshes], [(null)]
	unsigned short *Submesh_To_Region_Map;
	int Num_Components;
	// [Num_Components], [(null)]
	SemanticData *Component_Semantics;
} MeshData;

typedef struct {
	string Material_Name;
	unsigned int Material_Extra_Data;
} MaterialData;

typedef struct {
	NiAVObject *parent;
	unsigned int Num_Materials;
	// [Num_Materials], [(null)]
	MaterialData *Material_Data;
	int Active_Material;
	byte Material_Needs_Update_Default;
} NiRenderObject;

typedef struct {
	NiObject *parent;
	unsigned int Num_Submit_Points;
	// [Num_Submit_Points], [(null)]
	unsigned short *Submit_Points;
	unsigned int Num_Complete_Points;
	// [Num_Complete_Points], [(null)]
	unsigned short *Complete_Points;
} NiMeshModifier;

typedef struct {
	NiRenderObject *parent;
	unsigned int Primitive_Type;
	unsigned short Num_Submeshes;
	byte Instancing_Enabled;
	SphereBV Bound;
	unsigned int Num_Datas;
	// [Num_Datas], [(null)]
	MeshData *Datas;
	unsigned int Num_Modifiers;
	// [Num_Modifiers], [(null)]
	unsigned int *Modifiers;
} NiMesh;

typedef struct {
	NiInterpController *parent;
	int Unknown_2;
	unsigned int Num_Interpolators;
	// [Num_Interpolators], [(null)]
	unsigned int *Interpolators;
	unsigned int Num_Targets;
	// [Num_Targets], [(null)]
	string *Target_Names;
} NiMorphWeightsController;

typedef struct {
	SemanticData Semantic;
	unsigned int Normalize_Flag;
} ElementReference;

typedef struct {
	NiMeshModifier *parent;
	byte Flags;
	unsigned short Num_Targets;
	unsigned int Num_Elements;
	// [Num_Elements], [(null)]
	ElementReference *Elements;
} NiMorphMeshModifier;

typedef struct {
	NiMeshModifier *parent;
	unsigned short Flags;
	unsigned int Skeleton_Root;
	SkinTransform Skeleton_Transform;
	unsigned int Num_Bones;
	// [Num_Bones], [(null)]
	unsigned int *Bones;
	// [Num_Bones], [(null)]
	SkinTransform *Bone_Transforms;
	// [Num_Bones], [(null)]
	SphereBV *Bone_Bounds;
} NiSkinningMeshModifier;

typedef struct {
	NiMeshModifier *parent;
} NiInstancingMeshModifier;

typedef struct {
	NiTimeController *parent;
} NiSkinningLODController;

typedef struct {
	NiAVObject *parent;
	int Unknown_3;
	// [Unknown_3], [(null)]
	int *Unknown_38;
	int Unknown_4;
	int Unknown_5;
	// [Unknown_3], [(null)]
	int *Unknown_39;
	int Unknown_6;
	int Unknown_7;
	int Unknown_8;
	int Unknown_9;
	float Unknown_10;
	int Unknown_11;
	int Unknown_12;
	unsigned int Simulator;
	unsigned int Generator;
	int Unknown_15;
	int Unknown_16;
	int Unknown_17;
	unsigned int Emitter;
	int Unknown_19;
	int Unknown_20;
	int Unknown_21;
	// [4], [(null)]
	byte *Unknown_22;
	int Unknown_27;
	int Unknown_28;
	int Unknown_29;
	int Unknown_30;
	int Unknown_31;
	int Unknown_32;
	int Unknown_33;
	int Unknown_34;
	byte Unknown_35;
	int Unknown_36;
	short Unknown_37;
} NiPSParticleSystem;

typedef struct {
	NiPSParticleSystem *parent;
	int Unknown_23;
	int Unknown_24;
	int Unknown_25;
	byte Unknown_26;
} NiPSMeshParticleSystem;

typedef struct {
	NiPSysEmitterCtlr *parent;
} NiPSEmitParticlesCtlr;

typedef struct {
	NiTimeController *parent;
	unsigned int Interpolator;
	int Unknown_2;
} NiPSForceActiveCtlr;

typedef struct {
	NiMeshModifier *parent;
	unsigned int Num_Simulation_Steps;
	// [Num_Simulation_Steps], [(null)]
	unsigned int *Simulation_Steps;
} NiPSSimulator;

typedef struct {
	NiObject *parent;
} NiPSSimulatorStep;

typedef struct {
	NiPSSimulatorStep *parent;
	byte Num_Size_Keys;
	// [Num_Size_Keys], [(null)]
	Key *Size_Keys;
	unsigned int Size_Loop_Behavior;
	float Unknown_1;
	float Unknown_2;
	float Unknown_3;
	byte Num_Color_Keys;
	// [Num_Color_Keys], [(null)]
	Key *Color_Keys;
	unsigned int Color_Loop_Behavior;
	byte Num_Rotation_Keys;
	// [Num_Rotation_Keys], [(null)]
	QuatKey *Rotation_Keys;
	unsigned int Rotation_Loop_Behavior;
	float Grow_Time;
	float Shrink_Time;
	unsigned short Grow_Generation;
	unsigned short Shrink_Generation;
} NiPSSimulatorGeneralStep;

typedef struct {
	NiPSSimulatorStep *parent;
	unsigned int Num_Forces;
	// [Num_Forces], [(null)]
	unsigned int *Forces;
} NiPSSimulatorForcesStep;

typedef struct {
	NiPSSimulatorStep *parent;
	unsigned int Num_Colliders;
	// [Num_Colliders], [(null)]
	unsigned int *Colliders;
} NiPSSimulatorCollidersStep;

typedef struct {
	NiPSSimulatorStep *parent;
	byte Num_Rotation_Keys;
	// [Num_Rotation_Keys], [(null)]
	QuatKey *Rotation_Keys;
	unsigned int Rotation_Loop_Behavior;
} NiPSSimulatorMeshAlignStep;

typedef struct {
	NiPSSimulatorStep *parent;
} NiPSSimulatorFinalStep;

typedef struct {
	NiObject *parent;
	byte Unknown_1;
	byte Unknown_2;
	byte Unknown_3;
	byte Unknown_4;
	byte Unknown_5;
	byte Unknown_6;
	byte Unknown_7;
	byte Unknown_8;
	byte Unknown_9;
	byte Unknown_10;
	byte Unknown_11;
	byte Unknown_12;
} NiPSFacingQuadGenerator;

typedef struct {
	NiObject *parent;
	string Name;
	unsigned short Unknown_Flags;
	unsigned int Num_Unknown_Links_1;
	// [Num_Unknown_Links_1], [(null)]
	unsigned int *Unknown_Links_1;
	int Unkown_Int_2;
	unsigned int Target;
	float Unkown_Float_4;
	byte Unkown_Byte_5;
	int Unkown_Int_6;
	int Unkown_Int_7;
	int Unkown_Int_8;
	byte Unkown_Byte_9;
} NiShadowGenerator;

typedef struct {
	NiObject *parent;
	byte Unknown_1;
	byte Unknown_2;
} NiPSBoundUpdater;

typedef struct {
	NiObject *parent;
	int Unknown_1;
	int Unknown_2;
	byte Unknown_3;
	float Unknown_4;
	float Unknown_5;
	float Unknown_6;
	float Unknown_7;
	float Unknown_8;
	float Unknown_9;
	int Unknown_10;
} NiPSDragForce;

typedef struct {
	NiObject *parent;
	byte Unknown_1;
	byte Unknown_2;
	byte Unknown_3;
	byte Unknown_4;
	byte Unknown_5;
	byte Unknown_6;
	byte Unknown_7;
	byte Unknown_8;
	byte Unknown_9;
	byte Unknown_10;
	byte Unknown_11;
	byte Unknown_12;
	byte Unknown_13;
	byte Unknown_14;
	byte Unknown_15;
	byte Unknown_16;
	byte Unknown_17;
	float Unknown_18;
	byte Unknown_19;
	byte Unknown_20;
	byte Unknown_21;
	byte Unknown_22;
	byte Unknown_23;
	byte Unknown_24;
	byte Unknown_25;
	byte Unknown_26;
	byte Unknown_27;
	byte Unknown_28;
	byte Unknown_29;
	byte Unknown_30;
	byte Unknown_31;
	byte Unknown_32;
	byte Unknown_33;
	byte Unknown_34;
	float Unknown_35;
	unsigned int Unknown_36;
} NiPSGravityForce;

typedef struct {
	NiObject *parent;
	string Name;
	float Unknown_1;
	float Unknown_2;
	byte Unknown_3;
	byte Unknown_4;
	byte Unknown_5;
	byte Unknown_6;
	float Unknown_7;
	byte Unknown_8;
	byte Unknown_9;
	byte Unknown_10;
	byte Unknown_11;
	float Unknown_12;
	int Unknown_13;
	float Unknown_14;
	float Unknown_15;
	float Unknown_16;
	float Unknown_17;
	float Unknown_18;
	float Unknown_19;
	float Unknown_20;
	float Unknown_21;
	float Unknown_22;
	byte Unknown_23;
	byte Unknown_24;
	byte Unknown_25;
	byte Unknown_26;
	byte Unknown_27;
	byte Unknown_28;
	byte Unknown_29;
	byte Unknown_30;
	byte Unknown_31;
	byte Unknown_32;
	byte Unknown_33;
	byte Unknown_34;
	byte Unknown_35;
	byte Unknown_36;
	byte Unknown_37;
	byte Unknown_38;
	byte Unknown_39;
	byte Unknown_40;
	byte Unknown_41;
	byte Unknown_42;
	byte Unknown_43;
	byte Unknown_44;
	byte Unknown_45;
	byte Unknown_46;
	byte Unknown_47;
	byte Unknown_48;
} NiPSBoxEmitter;

typedef struct {
	NiObject *parent;
	string Name;
	int Unknown_1;
	int Unknown_2;
	int Unknown_3;
	int Unknown_27;
	float Unknown_4;
	float Unknown_5;
	float Unknown_6;
	float Unknown_28;
	int Unknown_7;
	float Unknown_8;
	float Unknown_9;
	float Unknown_10;
	float Unknown_11;
	float Unknown_12;
	int Unknown_13;
	float Unknown_14;
	float Unknown_15;
	float Unknown_16;
	int Unknown_17;
	int Unknown_18;
	short Unknown_19;
	int Unknown_20;
	int Unknown_21;
	float Unknown_22;
	int Unknown_23;
	int Unknown_24;
	int Unknown_25;
	int Unknown_26;
} NiPSMeshEmitter;

typedef struct {
	NiTimeController *parent;
	int Unknown_2;
	int Unknown_3;
} NiPSGravityStrengthCtlr;

typedef struct {
	NiObject *parent;
	string Name;
	int Unknown_Int_1;
	int Unknown_Int_2;
	short Unknown_Short_3;
	byte Unknown_Byte_4;
	// [8], [(null)]
	float *Unknown_Floats_5;
	unsigned int Unknown_Link_6;
} NiPSPlanarCollider;

typedef struct {
	NiTimeController *parent;
	unsigned int Interpolator;
	int Unknown_3;
} NiPSEmitterSpeedCtlr;

typedef struct {
	NiTimeController *parent;
	unsigned int Interpolator;
	int Unknown_2;
} NiPSEmitterRadiusCtlr;

typedef struct {
	NiTimeController *parent;
} NiPSResetOnLoopCtlr;

typedef struct {
	NiObject *parent;
	string Name;
	int Unknown_2;
	int Unknown_3;
	int Unknown_4;
	int Unknown_5;
	float Unknown_6;
	int Unknown_7;
	float Unknown_8;
	float Unknown_9;
	int Unknown_10;
	float Unknown_11;
	int Unknown_12;
	int Unknown_13;
	int Unknown_14;
	int Unknown_15;
	int Unknown_16;
	float Unknown_17;
	int Unknown_18;
	int Unknown_19;
	short Unknown_20;
	int Unknown_21;
	float Unknown_22;
} NiPSSphereEmitter;

typedef struct {
	NiPSSphereEmitter *parent;
	float Unknown_23;
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
	byte Unknown_1;
	int Unknown_2;
	int Unknown_3;
	int Unknown_4;
	int Unknown_5;
	int Unknown_6;
	int Unknown_7;
	int Unknown_8;
	int Unknown_9;
	int Unknown_10;
} NiPSBombForce;

typedef struct {
	NiObject *parent;
	int Unknown_1;
	int Unknown_2;
	byte Unknown_3;
	float Unknown_4;
	int Unknown_5;
	short Unknown_6;
	int Unknown_7;
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
	float Unknown_Float;
	float Unknown_Float_2;
	unsigned int Num_Bones;
	// [Num_Bones], [(null)]
	unsigned int *Bones;
	unsigned int Num_Bones_2;
	// [Num_Bones_2], [(null)]
	unsigned int *Bones_2;
} NiFurSpringController;

void readSizedString (SizedString * obj, unsigned int ARG);
void readstring (string * obj, unsigned int ARG);
void readByteArray (ByteArray * obj, unsigned int ARG);
void readByteMatrix (ByteMatrix * obj, unsigned int ARG);
void readColor3 (Color3 * obj, unsigned int ARG);
void readByteColor3 (ByteColor3 * obj, unsigned int ARG);
void readColor4 (Color4 * obj, unsigned int ARG);
void readByteColor4 (ByteColor4 * obj, unsigned int ARG);
void readFilePath (FilePath * obj, unsigned int ARG);
void readFooter (Footer * obj, unsigned int ARG);
void readLODRange (LODRange * obj, unsigned int ARG);
void readMatchGroup (MatchGroup * obj, unsigned int ARG);
void readVector3 (Vector3 * obj, unsigned int ARG);
void readVector4 (Vector4 * obj, unsigned int ARG);
void readQuaternion (Quaternion * obj, unsigned int ARG);
void readQuaternionXYZW (QuaternionXYZW * obj, unsigned int ARG);
void readMatrix22 (Matrix22 * obj, unsigned int ARG);
void readMatrix33 (Matrix33 * obj, unsigned int ARG);
void readMatrix44 (Matrix44 * obj, unsigned int ARG);
void readMipMap (MipMap * obj, unsigned int ARG);
void readNodeGroup (NodeGroup * obj, unsigned int ARG);
void readShortString (ShortString * obj, unsigned int ARG);
void readSkinShape (SkinShape * obj, unsigned int ARG);
void readSkinShapeGroup (SkinShapeGroup * obj, unsigned int ARG);
void readSkinWeight (SkinWeight * obj, unsigned int ARG);
void readAVObject (AVObject * obj, unsigned int ARG);
void readControllerLink (ControllerLink * obj, unsigned int ARG);
void readExportInfo (ExportInfo * obj, unsigned int ARG);
void readHeader (Header * obj, unsigned int ARG);
void readStringPalette (StringPalette * obj, unsigned int ARG);
void readTBC (TBC * obj, unsigned int ARG);
void readKey (Key * obj, unsigned int ARG, int t);
void readKeyGroup (KeyGroup * obj, unsigned int ARG, int t);
void readQuatKey (QuatKey * obj, unsigned int ARG);
void readTexCoord (TexCoord * obj, unsigned int ARG);
void readTexDesc (TexDesc * obj, unsigned int ARG);
void readShaderTexDesc (ShaderTexDesc * obj, unsigned int ARG);
void readTexSource (TexSource * obj, unsigned int ARG);
void readTriangle (Triangle * obj, unsigned int ARG);
void readSkinPartition (SkinPartition * obj, unsigned int ARG);
void readBoundingBox (BoundingBox * obj, unsigned int ARG);
void readFurniturePosition (FurniturePosition * obj, unsigned int ARG);
void readhkTriangle (hkTriangle * obj, unsigned int ARG);
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
void readNiObject (NiObject * obj, unsigned int ARG);
void readNi3dsAlphaAnimator (Ni3dsAlphaAnimator * obj, unsigned int ARG);
void readNi3dsAnimationNode (Ni3dsAnimationNode * obj, unsigned int ARG);
void readNi3dsColorAnimator (Ni3dsColorAnimator * obj, unsigned int ARG);
void readNi3dsMorphShape (Ni3dsMorphShape * obj, unsigned int ARG);
void readNi3dsParticleSystem (Ni3dsParticleSystem * obj, unsigned int ARG);
void readNi3dsPathController (Ni3dsPathController * obj, unsigned int ARG);
void readNiParticleModifier (NiParticleModifier * obj, unsigned int ARG);
void readNiPSysCollider (NiPSysCollider * obj, unsigned int ARG);
void readbhkRefObject (bhkRefObject * obj, unsigned int ARG);
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
void readNiInterpolator (NiInterpolator * obj, unsigned int ARG);
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
void readNiObjectNET (NiObjectNET * obj, unsigned int ARG);
void readNiCollisionObject (NiCollisionObject * obj, unsigned int ARG);
void readNiCollisionData (NiCollisionData * obj, unsigned int ARG);
void readbhkNiCollisionObject (bhkNiCollisionObject * obj, unsigned int ARG);
void readbhkCollisionObject (bhkCollisionObject * obj, unsigned int ARG);
void readbhkBlendCollisionObject (bhkBlendCollisionObject * obj,
				  unsigned int ARG);
void readbhkPCollisionObject (bhkPCollisionObject * obj, unsigned int ARG);
void readbhkSPCollisionObject (bhkSPCollisionObject * obj, unsigned int ARG);
void readNiAVObject (NiAVObject * obj, unsigned int ARG);
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
void readNiAVObjectPalette (NiAVObjectPalette * obj, unsigned int ARG);
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
void readNiNode (NiNode * obj, unsigned int ARG);
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
void readNiLODData (NiLODData * obj, unsigned int ARG);
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
void readBSMultiBoundData (BSMultiBoundData * obj, unsigned int ARG);
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
void readNiPSSimulatorStep (NiPSSimulatorStep * obj, unsigned int ARG);
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
void readNiPSSpawner (NiPSSpawner * obj, unsigned int ARG);
void readNiSequenceData (NiSequenceData * obj, unsigned int ARG);
void readNiTransformEvaluator (NiTransformEvaluator * obj, unsigned int ARG);
void readNiBSplineCompTransformEvaluator (NiBSplineCompTransformEvaluator * obj,
					  unsigned int ARG);
void readNiMeshHWInstance (NiMeshHWInstance * obj, unsigned int ARG);
void readNiFurSpringController (NiFurSpringController * obj, unsigned int ARG);
#endif