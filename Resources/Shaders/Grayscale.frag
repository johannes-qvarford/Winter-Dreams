#version 110
uniform sampler2D texture;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	vec3 adjCol = vec3(col.rgb*0.5);
	float grayCol = adjCol.r+adjCol.g+adjCol.b;
	gl_FragColor = vec4(grayCol,grayCol,grayCol,1.0);
}