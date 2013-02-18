
#define NUM_LIGHTS 2

varying out vec4 colorOut;
uniform sampler2D texture;
uniform float lightPosx[NUM_LIGHTS];
uniform float lightPosy[NUM_LIGHTS];
uniform float maxDis;
uniform float brightness;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	float lightValue = 0.0;

	float curValue = 0.0;

	
	for (int i = 0; i < NUM_LIGHTS; ++i){
		float dis = distance(gl_TexCoord[0].xy, vec2(lightPosx[i], lightPosy[i]));
		
		curValue = brightness*(maxDis - dis) / maxDis;
		//curValue = max(curValue, 0);
		curValue = clamp(curValue, 0, 1);
		curValue *= curValue;

		lightValue += curValue;
		//lightValue = max(curValue, lightValue);
	}
	
	lightValue = min(1, lightValue);
	colorOut = vec4(col.rgb * lightValue, 1.0);
}	
