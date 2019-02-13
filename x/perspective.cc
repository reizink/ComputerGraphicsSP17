//

#include "Angel.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

const int NumVertices = 36;

point4 points[NumVertices];
color4 colors[NumVertices];

point4 vertices[8] = {
	point4( -0.5, -0.5,  0.5, 1.0 ),
	point4( -0.5,  0.5,  0.5, 1.0 ),
	point4(  0.5,  0.5,  0.5, 1.0 ),
	point4(  0.5, -0.5,  0.5, 1.0 ),
	point4( -0.5, -0.5, -0.5, 1.0 ),
	point4( -0.5,  0.5, -0.5, 1.0 ),
	point4(  0.5,  0.5, -0.5, 1.0 ),
	point4(  0.5, -0.5, -0.5, 1.0 )
};

//---------------------------------------

color4 vertex_colors[8] ={
	color4( 0.0, 0.0, 0.0, 1.0), //black
	color4( 1.0, 0.0, 0.0, 1.0), //red
	color4( 1.0, 1.0, 0.0, 1.0), //yellow
	color4( 0.0, 1.0, 0.0, 1.0), //green
	color4( 0.0, 0.0, 1.0, 1.0), //blue
	color4( 1.0, 0.0, 1.0, 1.0), //magenta
	color4( 1.0, 1.0, 1.0, 1.0), //white
	color4( 0.0, 1.0, 1.0, 1.0)  //cyan
};

GLfloat radius = 1.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;

const GLfloat dr = 5.0 * DegreesToRadians;

GLuint model_view;

GLfloat left = -1.0, right = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 0.5, zFar = 3.0;

GLuint projection;

int Index = 0;

void
quad( int a, int b, int c, int d )
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//---------------------------------------------------------------

void
colorcube( void )
{
	quad( 1, 0, 3, 2 );
	quad( 2, 3, 7, 6 );
	quad( 3, 0, 4, 7 );
	quad( 6, 5, 1, 2 );
	quad( 4, 5, 6, 7 );
	quad( 5, 4, 0, 1 );
}

//--------------------------------------------------------------

void
init( void )
{
	colorcube();//

	GLuint vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	GLuint buffer;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points),  points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points),  sizeof(colors), colors );

	GLuint program = InitShader( "vshader42.glsl", "fshader42.glsl" );
	glUseProgram( program );

	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor);
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );

	model_view = glGetUniformLocation(program, "model_view" );
	projection = glGetUniformLocation(program, "projection" );

	glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------

void
display( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	point4 eye( radius*sin(theta)*cos(phi), radius*sin(theta)*sin(phi), radius*cos(theta), 1.0 );
	point4 at( 0.0, 0.0, 0.0, 1.0);
	vec4 up( 0.0, 1.0, 0.0, 0.0);

	mat4 mv = LookAt( eye, at, up );
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	mat4 p = Frustum( left, right, bottom, top, zNear, zFar );
	glUniformMatrix4fv( projection, 1, GL_TRUE, p );

	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	glutSwapBuffers();//
}

//--------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033: //escape key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;

		case 'x': left *= 1.1; right *= 1.1; break;
		case 'X': left *= 0.9; right *= 0.9; break;
		case 'y': bottom *= 1.1; top *= 1.1; break;
		case 'Y': bottom *= 0.9; top *= 0.9; break;
		case 'z': zNear *= 1.1; zFar *= 1.1; break;
		case 'Z': zNear *= 0.9; zFar *= 0.9; break;
		case 'r': radius *= 2.0; break;
		case 'R': radius *= 0.5; break;
		case 'o': theta += dr; break;
		case 'O': theta -= dr; break;
		case 'p': phi += dr; break;
		case 'P': phi -= dr; break;

		case ' ':
			left = -1.0;
			right = 1.0;
			bottom = -1.0;
			top = 1.0;
			zNear = 0.5;
			zFar = 3.0;
			radius = 1.0;
			theta = 0.0;
			phi = 0.0;
			break;
	}
	
	glutPostRedisplay(); //

}

//--------------------------------------------------------------

void
reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
}

//-------------------------------------------------------------

int
main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 512, 512 );
	glutCreateWindow( "Color Cube" );

	glewInit();//

	init();//

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );

	glutMainLoop();//
	return 0;
}

