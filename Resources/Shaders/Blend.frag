varying out vec4 colorOut;
uniform sampler2D texture;
uniform float alpha;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	colorOut = vec4(col.rgb,col.a * alpha);
}