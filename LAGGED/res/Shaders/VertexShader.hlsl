struct VertexPosColor
{
	float3 position : POSITION;
	float3 color : COLOR;
	float3 bump : WANK;
};

struct VSOut
{
	float4 Color : COLOR;
	float4 Position : SV_POSITION;	//SV_Position is a required parameter that needs to be output by a vertex shader. 
									//The SV_Position variable is not required for the pixel shader. The reason it is placed last has something to do with byte offsets
									//Since SV_Position is not required for the pixel shader, it can be omitted from the PS input parameters. However, byte offsets of the other params must match between the output and input of linked shader stages. For this reason, Position parameter is placed last.
};

struct ModelViewProjection
{
	matrix MVP;
};
ConstantBuffer<ModelViewProjection> mvp_CB : register(b0);

//Experimental!
cbuffer ModelViewProj : register (b0, space0)
{
	matrix MVP;
};

//This function doesn't HAVE to be called "main", as we can specify what entry point function we want to use when compiling the shader, but it's just nice like this
VSOut main(VertexPosColor VSIn)
{
	VSOut output;
	output.Position = mul(mvp_CB.MVP, float4(VSIn.position, 1.0f)); //Calculate clip-space position. Position must be converted to float4 since we're using a 4*4 matrix
	output.Color = float4(VSIn.color, 1.0f);
	return output;
}