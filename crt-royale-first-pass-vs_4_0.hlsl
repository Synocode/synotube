// shaders/crt-royale/src/crt-royale-first-pass-linearize-crt-gamma-bob-fields.cg
// shaders/crt-royale/src/../user-settings.h
// shaders/crt-royale/src/bind-shader-params.h
// shaders/crt-royale/src/../user-settings.h
// shaders/crt-royale/src/derived-settings-and-constants.h
// shaders/crt-royale/src/../user-settings.h
// shaders/crt-royale/src/user-cgp-constants.h
// shaders/crt-royale/src/../../../../include/gamma-management.h
// shaders/crt-royale/src/scanline-functions.h
// shaders/crt-royale/src/../user-settings.h
// shaders/crt-royale/src/derived-settings-and-constants.h
// shaders/crt-royale/src/../../../../include/special-functions.h
// shaders/crt-royale/src/../../../../include/gamma-management.h
// vs_4_0 output by Cg compiler
// cgc version 3.1.0013, build date Apr 18 2012
// command line args: -profile vs_4_0
// source file: shaders/crt-royale/src/crt-royale-first-pass-linearize-crt-gamma-bob-fields.cg
//vendor NVIDIA Corporation
//version 3.1.0.13
//profile vs_4_0
//program main_vertex
//semantic main_vertex.modelViewProj
//semantic main_vertex.IN
//var float4x4 modelViewProj :  : _modelViewProj[0], 4 : 3 : 1
//var float2 IN.video_size :  : _IN._video_size : 4 : 1
//var float2 IN.texture_size :  : _IN._texture_size : 4 : 1
//var float2 IN.output_size :  :  : 4 : 0
//var float IN.frame_count :  :  : 4 : 0
//var float IN.frame_direction :  :  : 4 : 0
//var float IN.frame_rotation :  :  : 4 : 0
//var float4 position : $vin.POSITION : POSITION : 0 : 1
//var float4 color : $vin.COLOR :  : 1 : 0
//var float2 tex_uv : $vin.TEXCOORD0 : TEXCOORD0 : 2 : 1
//var float4 main_vertex.position : $vout.POSITION :  : -1 : 1
//var float2 main_vertex.tex_uv : $vout.TEXCOORD0 :  : -1 : 1
//var float2 main_vertex.uv_step : $vout.TEXCOORD1 :  : -1 : 1
//var float main_vertex.interlaced : $vout.TEXCOORD2 :  : -1 : 1

#pragma pack_matrix(row_major)

struct input {
    float2 _video_size;
    float2 _texture_size;
    float2 _output_size;
    float _frame_count;
    float _frame_direction;
    float _frame_rotation;
};

struct out_vertex {
    float4 _position1 : POSITION;
    float2 _tex_uv1 : TEXCOORD0;
    float2 _uv_step : TEXCOORD1;
    float _interlaced : TEXCOORD2;
};

struct X1X {
    float4 _POSITION : SV_Position;
    float2 _TEXCOORD0 : TEXCOORD0;
    float2 _TEXCOORD1 : TEXCOORD1;
    float _TEXCOORD2 : TEXCOORD2;
};

static bool _TMP12;
static float4 _r0064;
static bool _sd_interlace10066;
static X1X cout;

 // main procedure, the original name was main_vertex
X1X main(in float4 _position : POSITION, in float2 _tex_uv : TEXCOORD0, uniform float4x4 _modelViewProj, uniform input _IN)
{

    out_vertex _OUT;

    _r0064.x = dot(_modelViewProj._11_12_13_14, _position);
    _r0064.y = dot(_modelViewProj._21_22_23_24, _position);
    _r0064.z = dot(_modelViewProj._31_32_33_34, _position);
    _r0064.w = dot(_modelViewProj._41_42_43_44, _position);
    _OUT._position1 = _r0064;
    _OUT._tex_uv1 = _tex_uv;
    _OUT._uv_step = float2(  1.00000000000000000E000f,  1.00000000000000000E000f)/_IN._texture_size;
    _sd_interlace10066 = _IN._video_size.y >  2.88500000000000000E002f && _IN._video_size.y <  5.76500000000000000E002f;
    _TMP12 = _sd_interlace10066 || false;
    _OUT._interlaced = (float) _TMP12;
    cout._POSITION = _OUT._position1;
    cout._TEXCOORD0 = _OUT._tex_uv1;
    cout._TEXCOORD1 = _OUT._uv_step;
    cout._TEXCOORD2 = _OUT._interlaced;
    return cout;
} // main end
