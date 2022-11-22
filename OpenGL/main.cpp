//=======================================================================
//
// Project for Computer Graphic
// second OpenGL program, WiSe 2022/23
// object and camera transformations, shader implementation
//
//
// Name:
// Matrikelnummer:
//
//=======================================================================
#include "Dependencies/glew/glew.h"
#include "Dependencies/glut/glut.h"
#include "utilities.h"
#include <string>

// define constant numbers for the box mesh
const int NumTriangles = 12;				// number faces, 2 triangles each
const int NumVertices = 3 * NumTriangles;	// number vertices

// define types for the vertex position and color
typedef glm::vec4 point4;
typedef glm::vec4 color4;

// function declaration
void initDrawing();
void display();
void keyboard(unsigned char key, int x, int y);
void box();
void triangle(const point4& a, const point4& b, const point4& c, const color4 col);

// variables used in differnt functions
GLuint program;
GLuint vPosition;			// entry point for vertex positions into the vertex shader
GLuint vNormal;				// entry point for vertex normals into the vertex shader
GLuint vColor;				// entry point for vertex color into the vertex shader
GLuint vao1 = 0;
int g_Mode = 0;
int g_Index = 0;
float rotX = 0.0f;
float rotY = 0.0f;
float rotZ = 0.0f;
float rotAnim = 0.0f;
float deltaTime, currentTime;
bool g_Animate = true;

// arrays for vertex positions, colors and normals
point4 points[NumVertices];
color4 colors[NumVertices];
glm::vec3   normals[NumVertices];


// Model-view and projection matrices uniform location
GLuint  hModel, hCamera, hProjection;

// variables to control uniform intensity in fragment shader
GLuint hIntensity;
GLfloat fIntensity;


//=======================================================================
//
// Main function
//
//=======================================================================
int main(int argc, char* argv[])
{
	// initialize and open a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Second Shaderbased OpenGL Application");

	glewInit();

	// do some initializations 
	initDrawing();

	// register display function 
	glutDisplayFunc(display);
	// register keyboard function 
	glutKeyboardFunc(keyboard);

	// start the main loop
	glutMainLoop();

	return 0;
}



//=======================================================================
//
// Init function, will be called once at the beginnning
//
//=======================================================================
void initDrawing()
{
	GLuint vboPos, vboCol;

	// =======================================================
	// load or create models
	// call 3D-box function
	box();

	glEnable(GL_DEPTH_TEST);

	// =======================================================
	// load and compile shader program
	// vertex and fragment shsader

	std::string vertexShader = Utilities::loadFile("color.vert");
	std::string fragmentShader = Utilities::loadFile("color.frag");

	// compile vertex and fragment shader
	program = Utilities::compileShader(vertexShader, fragmentShader);
	// activate shader programs
	glUseProgram(program);


	// =======================================================
	// Vertex array objects:
	// now create and activate vertex array objects to handle the link between the buffers
	// and the input attributes to the vertex shader
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);

	
	// =======================================================
	// Buffer objects:
	// Create and initialize buffer objects to copy vertex data (e.g. position, color)
	// to the GPU and link vertex data to the corresponding buffer object

	// generate and manage buffer object for vertex coordinates
	// transfer position data to the GPU
	glGenBuffers(1,&vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// link to shader where to find the vertex coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// generate and manage buffer object for vertex color
	// transfer color data to the GPU
	glGenBuffers(1,&vboCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	// link to shader where to find the vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);


	// Load location for uniform variable from shader program
	// for the model, camera and projection matrix
	hModel = glGetUniformLocation(program, "Model");
	hCamera = glGetUniformLocation(program, "Camera");
	hProjection = glGetUniformLocation(program, "Projection");


}


//=======================================================================
//
// display function, will be called whenever the scene needs to be drawn
//
//=======================================================================
void display()
{

	// define background color
	const GLfloat color[] = { 0.9, 0.9, 0.9, 1 };

	// some initalizations
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 0, color);

	// declare and initalize model matrix
	glm::mat4 model = glm::mat4(1.0);
	// declare and initalize camera matrix
	glm::mat4 camera = glm::mat4(1.0);
	// declare and initalize projection matrix
	glm::mat4 projection = glm::mat4(1.0);
	
	// define camera settings (position, orientation, projection mode)
	// ... need to be implemented


	// model transformations if needed (use modified var..iables from keyboard-function, e.g. rotX)
	// ... need to be implemented
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f);

	glm::vec3 eye(0, 0, 3), center(0, 0, 0), up(0, 1, 0);
	camera = glm::lookAt(eye, center, up);

	model = glm::rotate(model, (float)glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(hModel, 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(hCamera, 1, GL_FALSE, glm::value_ptr(camera));
	glUniformMatrix4fv(hProjection, 1, GL_FALSE, glm::value_ptr(projection));

	// set shader parameter if needed (e.g. model, camera, projection matrix, light, material)
	// ... need to be implemented

	// activate object rendering - object to render will be controlled by the corresponding
	// VertexArrayObject; shader to use will be controlled by the shader program variable
	glDrawArrays(GL_TRIANGLES, 0, sizeof(points));
	
	glutSwapBuffers();
}

//=======================================================================
//
// keyboard function, will be called whenever a key is pressed 
//
//=======================================================================
void keyboard(unsigned char key, int x, int y)
{

	// select the pressed key
	switch (key)
	{
		// handle interaction
		// e.g. change the rotation angle about the x-axis
		case 'x':
			rotX += 10.0f;
			break;

		case 'q':
			exit(EXIT_SUCCESS);
			break;
	}

	// enforce redrawing of the scene
	glutPostRedisplay();

}


//=======================================================================
//
// generates a 3D-Box with 8 vertices and 6 faces/12 triangles 
//
//=======================================================================
void box()
{
	// define 8 box vertices
	// and the center of the box at the origin
	point4 v[8] = {
		glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
		glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
		glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f),
		glm::vec4(0.5f, 0.5f, -0.5f, 1.0f)
	};

	// define a color for each surface of the box
	color4 front = color4(1.0f, 0.0f, 1.0f, 1.0f);
	color4 back = color4(0.0f, 1.0f, 0.0f, 1.0f);
	color4 left = color4(1.0f, 0.0f, 0.0f, 1.0f);
	color4 right = color4(0.0f, 1.0f, 1.0f, 1.0f);
	color4 top = color4(0.0f, 0.0f, 1.0f, 1.0f);
	color4 bottom = color4(1.0f, 1.0f, 0.0f, 1.0f);

	// define the 12 triangles, counterclockwise
	// front => mangenta
	triangle(v[0], v[1], v[2], front);
	triangle(v[0], v[2], v[3], front);

	// back => green
	triangle(v[4], v[5], v[6], back);
	triangle(v[4], v[6], v[7], back);

	// left => red
	triangle(v[5], v[3], v[2], left);
	triangle(v[5], v[2], v[6], left);

	// right => cyan
	triangle(v[0], v[4], v[7], right);
	triangle(v[0], v[7], v[1], right);

	// top => blue
	triangle(v[1], v[6], v[2], top);
	triangle(v[1], v[7], v[6], top);

	// bottom => yellow
	triangle(v[5], v[0], v[3], bottom);
	triangle(v[5], v[4], v[0], bottom);

}

//=======================================================================
//
// generates a single triangle with normals per vertex
// used to generate a triangle mesh
//
//=======================================================================
void triangle(const point4& a, const point4& b, const point4& c, const color4 col)
{


	//vec3  normal = normalize( cross(b - a, c - b) );
	// calculate normal via cross product based on two vectors between the vertices
	glm::vec3  normal = glm::cross(glm::vec3(b) - glm::vec3(a), glm::vec3(c) - glm::vec3(b));

	normal = glm::normalize(normal);

	glm::vec3  normalA(a.x, a.y, a.z);
	normalA = normalize(normalA);
	glm::vec3  normalB(b.x, b.y, b.z);
	normalB = normalize(normalB);
	glm::vec3  normalC(c.x, c.y, c.z);
	normalC = normalize(normalC);

	normals[g_Index] = normalA;  points[g_Index] = a; colors[g_Index] = col; g_Index++;
	normals[g_Index] = normalB;  points[g_Index] = b; colors[g_Index] = col; g_Index++;
	normals[g_Index] = normalC;  points[g_Index] = c; colors[g_Index] = col; g_Index++;

}