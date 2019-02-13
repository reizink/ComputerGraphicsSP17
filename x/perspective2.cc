//perspective house
//Reilly Zink
//


#include "Angel.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

const int NumVertices = 16;

point4 points[NumVertices];
color4 colors[NumVertices];

GLfloat radius = 4.0;
GLfloat theta = 0.0;
GLfloat phi = 0.0;
//GLint rotloc;

const GLfloat dr = 5.0 * DegreesToRadians;

GLuint model_view;

GLfloat left = -1.0, right = 1.0;
GLfloat bottom = -1.0, top = 1.0;
GLfloat zNear = 0.25, zFar = 3.0;

GLuint projection;


void init()
{
	point4 vertices[9] = {
		point4(-0.5,-0.5,-0.5, 1.0 ), // 0
		point4( 0.5,-0.5,-0.5, 1.0 ),  //1
		point4( 0.5, 0.5,-0.5, 1.0 ),   //2
		point4(-0.5, 0.5,-0.5, 1.0 ),  //3
		point4(-0.5,-0.5, 0.5, 1.0 ),  //4
		point4( 0.5,-0.5, 0.5, 1.0 ),//5
		point4( 0.5, 0.5, 0.5, 1.0 ),//6
		point4(-0.5, 0.5, 0.5, 1.0 ),//7
		point4( 0.0,0.95, 0.0, 1.0 )//8
};



	color4 black = color4( 0.0, 0.0, 0.0, 1.0); //black
	color4 red = color4( 1.0, 0.0, 0.0, 1.0); //red
	color4 yellow = color4( 1.0, 1.0, 0.0, 1.0); //yellow
	color4 green = color4( 0.0, 1.0, 0.0, 1.0); //green
	color4 blue = color4( 0.0, 0.0, 1.0, 1.0); //blue
	color4 magenta = color4( 1.0, 0.0, 1.0, 1.0); //magenta
	color4 white = color4( 1.0, 1.0, 1.0, 1.0); //white
	color4 cyan = color4( 0.0, 1.0, 1.0, 1.0);  //cyan


	// house body
	points[0]=vertices[4];  colors[0]=red;
	points[1]=vertices[7];	colors[1]=blue;
	points[2]=vertices[5];  colors[2]=green;
	points[3]=vertices[6];  colors[3]=yellow;
	points[4]=vertices[1];  colors[4]=magenta;
	points[5]=vertices[2];  colors[5]=cyan;
 	points[6]=vertices[0];  colors[6]=red;
 	points[7]=vertices[3];  colors[7]=blue;
 	points[8]=vertices[4];  colors[8]=green;
 	points[9]=vertices[7];  colors[9]=magenta;
	// roof
 	points[10]=vertices[8];  colors[10]=cyan;
	points[11]=vertices[7];  colors[11]=magenta;
	points[12]=vertices[6];  colors[12]=yellow;
	points[13]=vertices[2];  colors[13]=green;
 	points[14]=vertices[3];  colors[14]=blue;
 	points[15]=vertices[7];  colors[15]=red;

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
  	/*rotLoc = glGetUniformLocation(program, "rot");

  	if (rotLoc==-1) {
    		std::cerr << "Unable to find rot parameter" << std::endl;
  	}*/

	glEnable( GL_DEPTH_TEST );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------

/*
double rotX=0, rotY=0, rotZ=0;
double scale=1.0;
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  mat4  model_view = (Scale(scale, scale, scale)*
		      RotateX(rotX) *
		      RotateY(rotY) *
		      RotateZ(rotZ));
  // projection transformation parameters
  //GLfloat fovy = -1.0, aspect = 1.0;/////////////////////////////////
  //GLfloat near = .5, far = 3.0;

  mat4 Perspective( 45.0, 1.0, 0.5, 3.0);
  //glUniformMatrix4fv( projection, 1, GL_TRUE, projection );
  glUniformMatrix4fv(rotLoc, 1, GL_TRUE, model_view);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);   //triangles,strip,fan,quads,etc.
  glDrawArrays(GL_TRIANGLE_FAN, 10, 6);   

  glFlush(); 
}
*/

//---------------------------------------------------------------------------

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	point4 eye( radius*sin(theta)*cos(phi), radius*sin(theta)*sin(phi), radius*cos(theta), 1.0 );
	point4 at( 0.0, 0.0, 0.0, 1.0);
	vec4 up( 0.0, 1.0, 0.0, 0.0);

	mat4 mv = LookAt( eye, at, up );
	glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );

	//mat4 p = Frustum( left, right, bottom, top, zNear, zFar );
	mat4 p = Perspective(45.0, 1.0, 0.25, 6.0);
	glUniformMatrix4fv( projection, 1, GL_TRUE, p );
	

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 10 );
	glDrawArrays( GL_TRIANGLE_FAN, 10, 6 );
	glutSwapBuffers();//
}

//--------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
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
			zNear = 0.25;
			zFar = 3.0;
			radius = 1.0;
			theta = 0.0;
			phi = 0.0;
			break;
	}
	
	glutPostRedisplay(); //

}

//--------------------------------------------------------------

void reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
}

//-------------------------------------------------------------

int main( int argc, char **argv )
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

