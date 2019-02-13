varying vec4 color;
//varying vec4 fColor;

void main()
{
	//fColor = color;
	gl_FragColor = color;
	gl_FragColor.r -= 0.4;
}
