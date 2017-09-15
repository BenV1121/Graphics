#version 450

layout(points) in;
layout(triagnle_strip, max_vertices - 3) out;
	
in float[] geoValue;
out float outValue;
	
void main()
{
	for(int i = 0; i < 3; i++)
	{
		outValue = geoValue[0] + i;
	}
	EndPrimitive();
}
