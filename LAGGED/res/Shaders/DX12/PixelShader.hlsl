struct PixelShaderInput
{
	float4 color : COLOR;
	//No position attribute here! Only color is used, so we can leave out the position. 
};

//Output the color to the (only) bound render target using SV_Target
float4 main(PixelShaderInput PSIn) : SV_Target
{
	return PSIn.color;
}