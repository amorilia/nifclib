#!/bin/bash

FILE="nif_reader.c"
FILE2="nif_reader2.c"

cat $FILE | sed s/'\&amp\;'/'\&'/g |\
	sed s/'\&lt\;'/'<'/g | sed s/'\&gt\;'/'>'/g |\
	sed s/'User Version 2'/'User_Version_2'/g |\
	sed s/'User Version'/'User_Version'/g |\
	sed s/'Has Texture Transform'/'obj\->Has_Texture_Transform'/g |\
	sed s/'Use External'/'obj\->Use_External'/g |\
	sed s/'Num Strips'/'obj\->Num_Strips'/g |\
	sed s/'Enable Motor'/'obj\->Enable_Motor'/g |\
	sed s/'Has Image'/'obj\->Has_Image'/g |\
	sed s/'Collision Type'/'obj\->Collision_Type'/g |\
	sed s/'Has Data'/'obj\->Has_Data'/g |\
	sed s/'Has Old Extra Data'/'obj\->Has_Old_Extra_Data'/g |\
	sed s/'Has Bounding Box'/'obj\->Has_Bounding_Box'/g |\
	sed s/'Has Shader'/'obj\->Has_Shader'/g |\
	sed s/'Has Normals'/'obj\->Has_Normals'/g |\
	sed s/'BS Num UV Sets'/'obj\->BS_Num_UV_Sets'/g |\
	sed s/'Num UV Sets'/'obj\->Num_UV_Sets'/g |\
	sed s/'Has Vertex Colors'/'obj\->Has_Vertex_Colors'/g |\
	sed s/'TSpace Flag'/'obj\->TSpace_Flag'/g |\
	sed s/'Has Radii'/'obj\->Has_Radii'/g |\
	sed s/'Has Rotations'/'obj\->Has_Rotations'/g |\
	sed s/'Has Rotation Angles'/'obj\->Has_Rotation_Angles'/g |\
	sed s/'Has Rotation Axes'/'obj\->Has_Rotation_Axes'/g |\
	sed s/'Has Unknown Stuff 1'/'obj\->Has_Unknown_Stuff_1'/g |\
	sed s/'Has Rotations 2'/'obj\->Has_Rotations_2'/g |\
	sed s/'Has Unknown Floats 3 '/'obj\->Has_Unknown_Floats_3'/g |\
	sed s/'Rotation Type'/'obj\->Rotation_Type'/g  |\
	sed s/'Num Rotation Keys'/'obj\->Num_Rotation_Keys'/g |\
	sed s/'Has Base Texture'/'obj\->Has_Base_Texture'/g |\
	sed s/'Has Dark Texture'/'obj\->Has_Dark_Texture'/g |\
	sed s/'Has Detail Texture'/'obj\->Has_Detail_Texture'/g |\
	sed s/'Has Gloss Texture'/'obj\->Has_Gloss_Texture'/g |\
	sed s/'Has Glow Texture'/'obj\->Has_Glow_Texture'/g |\
	sed s/'Has Bump Map Texture'/'obj\->Has_Bump_Map_Texture'/g |\
	sed s/'Has Normal Texture'/'obj\->Has_Normal_Texture'/g |\
	sed s/'Has Unknown2 Texture'/'obj\->Has_Unknown2_Texture'/g |\
	sed s/'Has Decal 0 Texture'/'obj\->Has_Decal_0_Texture'/g |\
	sed s/'Has Decal 1 Texture'/'obj\->Has_Decal_1_Texture'/g |\
	sed s/'Has Decal 2 Texture'/'obj\->Has_Decal_2_Texture'/g |\
	sed s/'Has Decal 3 Texture'/'obj\->Has_Decal_3_Texture'/g |\
	sed s/'Texture Count'/'obj\->Texture_Count'/g |\
	sed s/'Image Type'/'obj\->Image_Type'/g |\
	sed s/'Unknown 12'/'obj\->Unknown_12'/g |\
	sed s/'Num Keys'/'obj\->Num_Keys'/g |\
	sed s/'Is Used'/'obj\->Is_Used'/g |\
	sed s/'Has Bone Indices'/'obj\->Has_Bone_Indices'/g |\
	sed s/'if ((Type'/'if ((obj\->Type'/g |\
	sed s/'if ((Flags'/'if ((obj\->Flags'/g |\
	sed s/'Use ABV'/'obj\->Use_ABV'/g |\
	sed s/'Has Vertices'/'obj\->Has_Vertices'/g |\
	sed s/'Num Vertices'/'obj\->Num_Vertices'/g |\
	sed s/'Has Vertex Weights'/'obj\->Has_Vertex_Weights'/g |\
	sed s/'Has Sizes'/'obj\->Has_Sizes'/g |\
	sed s/'10.0.1.0'/'0x0A000100'/g |\
	sed s/'20.2.0.7'/'0x14020007'/g \
	> $FILE2

echo done.
