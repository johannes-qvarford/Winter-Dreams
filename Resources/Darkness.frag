varying out vec4 colorOut;
uniform sampler2D texture;
vec2 playerPos=(0.5,0.5);
uniform float pixel_threshold;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	vec2 distVec = gl_TexCoord[0]-playerPos;
	float distance = sqrt(distVec.x*distVec.x + distVec.y * distVec.y);
	colorOut = vec4(col.r-distance*pixel_threshold,col.g-distance*pixel_threshold,col.b-distance*pixel_threshold,1.0);
}