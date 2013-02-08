varying out vec4 colorOut;
uniform sampler2D texture;
uniform float pixel_threshold;

void main()
{
	vec4 col = texture2D(texture,gl_TexCoord[0].xy);
	if (col.r>pixel_threshold) {col.r=1;} else {col.r=0;}
	if (col.g>pixel_threshold) {col.g=1;} else {col.g=0;}
	if (col.b>pixel_threshold) {col.b=1;} else {col.b=0;}
	colorOut = vec4(col.r,col.g,col.b,1.0);
}