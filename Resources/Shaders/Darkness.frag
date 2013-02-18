varying out vec4 colorOut;
uniform sampler2D texture;
uniform float lightPosx[2];
uniform float lightPosy[2];
uniform float pixel_threshold;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);

	float lightValue = 0.0;
	for (int i = 0; i<1; ++i){
		vec2 distVec = gl_TexCoord[0]-vec2(lightPosx[i],lightPosy[i]);
		float distance = (distVec.x*distVec.x + distVec.y * distVec.y);
		lightValue+=0.1/(distance+0.1);
	}
	
	lightValue=min(lightValue,1.0);
	
	colorOut = vec4(col.rgb-(1-lightValue)*pixel_threshold,1.0);
}