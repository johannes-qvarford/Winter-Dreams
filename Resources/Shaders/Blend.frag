#version 110
uniform sampler2D texture;
uniform float alpha;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	gl_FragColor = vec4(col.rgb,col.a * alpha);
}