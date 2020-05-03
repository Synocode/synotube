struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    return input;
}


Texture2D tx : register( t0 );
SamplerState samLinear : register( s0 );

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT input) : SV_Target
{
    float4 outputColor = tx.Sample( samLinear, input.Tex );
    outputColor.g *= 0.1;
   // outputColor.r = outputColor.b;
    //outputColor.g = outputColor.r;
    return outputColor;
}