varying out vec4 colorOut;
uniform sampler2D texture;
uniform float lightPosx[2];
uniform float lightPosy[2];
uniform float maxDis;
uniform float brightness;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	float lightValue = 0.0;
	float curValue = 0.0;
	

	for (int i = 0; i<2; ++i){
		float dis = distance(gl_TexCoord[0].xy, vec2(lightPosx[i], lightPosy[i]));
		curValue = brightness * (maxDis - dis) / maxDis;
		curValue = clamp(curValue, 0, 1);

		lightValue += curValue;
//		lightValue = max(curValue, lightValue);
	}
	
	lightValue=clamp(lightValue,0.0, 1.0);
	
	colorOut = vec4(col.rgb * lightValue, 1.0);
}