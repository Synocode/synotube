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
// ps_4_0 output by Cg compiler
// cgc version 3.1.0013, build date Apr 18 2012
// command line args: -profile ps_4_0
// source file: shaders/crt-royale/src/crt-royale-first-pass-linearize-crt-gamma-bob-fields.cg
//vendor NVIDIA Corporation
//version 3.1.0.13
//profile ps_4_0
//program main_fragment
//semantic main_fragment.input_texture : TEXUNIT0
//semantic main_fragment.IN
//var sampler2D input_texture : TEXUNIT0 : _input_texture 0 : 1 : 1
//var float2 IN.video_size :  :  : 2 : 0
//var float2 IN.texture_size :  : _IN._texture_size : 2 : 1
//var float2 IN.output_size :  :  : 2 : 0
//var float IN.frame_count :  : _IN._frame_count : 2 : 1
//var float IN.frame_direction :  :  : 2 : 0
//var float IN.frame_rotation :  :  : 2 : 0
//var float2 VAR.tex_uv : $vin.TEXCOORD0 :  : 0 : 1
//var float2 VAR.uv_step : $vin.TEXCOORD1 :  : 0 : 1
//var float VAR.interlaced : $vin.TEXCOORD2 :  : 0 : 1
//var float4 main_fragment : $vout.COLOR : COLOR : -1 : 1

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
    float4 _position : SV_Position;
    float2 _tex_uv : TEXCOORD0;
    float2 _uv_step : TEXCOORD1;
    float _interlaced : TEXCOORD2;
};

struct X1X {
    float4 _POSITION : SV_Position;
    float2 _TEXCOORD0 : TEXCOORD0;
    float2 _TEXCOORD1 : TEXCOORD1;
    float _TEXCOORD2 : TEXCOORD2;
};

static float _TMP21;
static float _TMP20;
static float _TMP19;
static float _TMP18;
static float4 _TMP17;
static float3 _TMP23;
static float _TMP26;
static float _TMP25;
static float _TMP24;
static float4 _TMP77;
static float4 _TMP89;
static float2 _tex_coords0090;
static float4 _TMP101;
static float2 _tex_coords0102;
static float _c0114;
static float _c0116;
static float _a0116;
static float4 _color0120;
Texture2D<float4> _TMP121 : register(t0);

 // main procedure, the original name was main_fragment
float4 main(uniform SamplerState _input_texture : TEXUNIT0, uniform input _IN,  in X1X cin) : SV_Target
{

    out_vertex _VAR;
    float2 _v_step1;
    float3 _interpolated_line1;
    float _modulus1;
    float _curr_line_texel1;
    float _line_num_last1;
    float3 _color11;

    _VAR._tex_uv = cin._TEXCOORD0;
    _VAR._uv_step = cin._TEXCOORD1;
    _VAR._interlaced = cin._TEXCOORD2;
    _v_step1 = float2( 0.00000000000000000E000f, _VAR._uv_step.y);
    _TMP17 = _TMP121.Sample(_input_texture, _VAR._tex_uv);
    _TMP24 = pow(_TMP17.x,  2.50000000000000000E000f);
    _TMP25 = pow(_TMP17.y,  2.50000000000000000E000f);
    _TMP26 = pow(_TMP17.z,  2.50000000000000000E000f);
    _TMP23 = float3(_TMP24, _TMP25, _TMP26);
    _TMP77 = float4(_TMP23.x, _TMP23.y, _TMP23.z, _TMP17.w);
    _tex_coords0090 = _VAR._tex_uv - _v_step1;
    _TMP17 = _TMP121.Sample(_input_texture, _tex_coords0090);
    _TMP24 = pow(_TMP17.x,  2.50000000000000000E000f);
    _TMP25 = pow(_TMP17.y,  2.50000000000000000E000f);
    _TMP26 = pow(_TMP17.z,  2.50000000000000000E000f);
    _TMP23 = float3(_TMP24, _TMP25, _TMP26);
    _TMP89 = float4(_TMP23.x, _TMP23.y, _TMP23.z, _TMP17.w);
    _tex_coords0102 = _VAR._tex_uv + _v_step1;
    _TMP17 = _TMP121.Sample(_input_texture, _tex_coords0102);
    _TMP24 = pow(_TMP17.x,  2.50000000000000000E000f);
    _TMP25 = pow(_TMP17.y,  2.50000000000000000E000f);
    _TMP26 = pow(_TMP17.z,  2.50000000000000000E000f);
    _TMP23 = float3(_TMP24, _TMP25, _TMP26);
    _TMP101 = float4(_TMP23.x, _TMP23.y, _TMP23.z, _TMP17.w);
    _interpolated_line1 =  5.00000000000000000E-001f*(_TMP89.xyz + _TMP101.xyz);
    _modulus1 = _VAR._interlaced +  1.00000000000000000E000f;
    _TMP18 = abs(_IN._frame_count/_modulus1);
    _TMP19 = frac(_TMP18);
    _TMP20 = abs(_modulus1);
    _c0114 = _TMP19*_TMP20;
    if (_IN._frame_count <  0.00000000000000000E000f) { // if begin
        _TMP21 = -_c0114;
    } else {
        _TMP21 = _c0114;
    } // end if
    _curr_line_texel1 = _VAR._tex_uv.y*_IN._texture_size.y;
    _line_num_last1 = floor(_curr_line_texel1 -  4.99500006437301640E-001f);
    _a0116 = _line_num_last1 + _TMP21;
    _TMP18 = abs(_a0116/_modulus1);
    _TMP19 = frac(_TMP18);
    _TMP20 = abs(_modulus1);
    _c0116 = _TMP19*_TMP20;
    if (_a0116 <  0.00000000000000000E000f) { // if begin
        _TMP21 = -_c0116;
    } else {
        _TMP21 = _c0116;
    } // end if
    _color11 = _TMP77.xyz + _TMP21*(_interpolated_line1 - _TMP77.xyz);
    _color0120 = float4(_color11.x, _color11.y, _color11.z,  1.00000000000000000E000f);
    return _color0120;
} // main end
