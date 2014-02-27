/* Fragment shader */
uniform sampler2D tex;
void main()
{
	//gl_FragColor = vec4(0, 1, 1, 1);
	//gl_FragColor = gl_Color;
//swizzling
	vec4 color=texture2D(tex,gl_TexCoord[0].st);

	 color.x*=gl_Color.x;
	color.y*=gl_Color.y;
      color.z*=gl_Color.z;

	

	gl_FragColor=color;

}
