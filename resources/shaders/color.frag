uniform sampler2D 	source;
uniform vec3 		color;

void main()
{
	vec4 texColor = texture(source, gl_TexCoord[0].xy);
	gl_FragColor = vec4(texColor.r + color.r, texColor.g + color.g, texColor.b + color.b, texColor.a);
}