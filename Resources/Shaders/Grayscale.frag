varying out vec4 colorOut;
uniform sampler2D texture;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	//vec3 adjCol = vec3(col.r*0.222, col.g*0.707, col.b*0.071);
	vec3 adjCol = vec3(col.rgb*1.1);
	float grayCol = adjCol.r+adjCol.g+adjCol.b;
	colorOut = vec4(grayCol,grayCol,grayCol,1.0);
}