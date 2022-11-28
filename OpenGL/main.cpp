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

// settings 
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// define constant numbers for the box mesh
const int NumTriangles = 12;				// number faces, 2 triangles each
const int NumVertices = 3 * NumTriangles;	// number vertices

// define types for the vertex position and color
typedef glm::vec4 point4;
typedef glm::vec4 color4;

// function declaration
void initDrawing();
void display();

void keyboard();
void keyboardUp(unsigned char key, int x, int y);
void keyboardDown(unsigned char key, int x, int y);

void mouse(int x, int y);

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

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Mouse input
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
bool isPerspective = true;

float colorSaturation = 1.0f;

float rotAnim = 0.0f;
float deltaTime, currentTime;
bool g_Animate = true;
// arrays for vertex positions, colors and normals
point4 points[NumVertices];
color4 colors[NumVertices];
glm::vec3   normals[NumVertices];
bool keystates[256];

// Model-view and projection matrices uniform location
GLuint  hModel, hCamera, hProjection;

// variables to control uniform intensity in fragment shader
GLuint hIntensity;
GLfloat fIntensity;
GLuint vboPos, vboCol;


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
	glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
	glutCreateWindow("Second Shaderbased OpenGL Application");

	glewInit();

	// do some initializations 
	initDrawing();

	// register display function 
	glutDisplayFunc(display);
	// register keyboard function 
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

	glutMotionFunc(mouse);

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
	glGenBuffers(1, &vboPos);
	glBindBuffer(GL_ARRAY_BUFFER, vboPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// link to shader where to find the vertex coordinates
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// generate and manage buffer object for vertex color
	// transfer color data to the GPU
	glGenBuffers(1, &vboCol);
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
	if (isPerspective)
		projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	else
		projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -100.0f, 100.0f);


	//glm::vec3 eye(0, 0, 3), center(posX, posY, posZ), up(0, 1, 0);
	//camera = glm::lookAt(eye, center, up);
	camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	model = glm::rotate(model, (float)glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, (float)glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, (float)glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(hModel, 1, GL_FALSE, glm::value_ptr(model));

	//camera = glm::translate(camera, (float)glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
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
void keyboardUp(unsigned char key, int x, int y)
{
	keystates[key] = false;

	keyboard();
	glutPostRedisplay();
}

//=======================================================================
//
// keyboard function, will be called whenever a key is pressed 
//
//=======================================================================
void keyboardDown(unsigned char key, int x, int y)
{
	keystates[key] = true;

	keyboard();
	glutPostRedisplay();

}

//=======================================================================
//
// keyboard function, will be called whenever a key is pressed 
//
//=======================================================================
void keyboard()
{
	const float cameraSpeed = 0.1f; // adjust accordingly

	if (keystates['x'])
		rotX += 10.0f;
	if (keystates['X'])
		rotX -= 10.0f;
	if (keystates['y'])
		rotY += 10.0f;
	if (keystates['Y'])
		rotY -= 10.0f;
	if (keystates['z'])
		rotZ += 10.0f;
	if (keystates['Z'])
		rotZ -= 10.0f;

	if (keystates['w'])
		cameraPos += cameraSpeed * cameraFront;
	if (keystates['s'])
		cameraPos -= cameraSpeed * cameraFront;
	if (keystates['a'])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keystates['d'])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (keystates['-'] && fov < 100.0f)
		fov += 1.0f;
	if (keystates['+'] && fov > 1.0f)
		fov -= 1.0f;

	if (keystates['i'] && colorSaturation < 1.0f)
		colorSaturation += .1f;
	if (keystates['l'] && colorSaturation > .0f)
		colorSaturation -= 0.1f;

	/// update colors intensity
	if ((keystates['i'] && colorSaturation < 1.0f) || (keystates['l'] && colorSaturation >= .0f)) {
		box();
		glBindBuffer(GL_ARRAY_BUFFER, vboCol);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	}

	if (keystates['o'])
		isPerspective = false;
	if (keystates['p'])
		isPerspective = true;


	// enforce redrawing of the scene
	glutPostRedisplay();

}

void mouse(int x, int y) {
	float xpos = static_cast<float>(x);
	float ypos = static_cast<float>(y);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glutPostRedisplay();
}

//=======================================================================
//
// generates a 3D-Box with 8 vertices and 6 faces/12 triangles 
//
//=======================================================================
void box()
{
	g_Index = 0;

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

	color4 front = color4(1, 0, 1, 1) * colorSaturation;
	color4 back = color4(0, 1, 0, 1) * colorSaturation;
	color4 left = color4(1, 0, 0, 1) * colorSaturation;
	color4 right = color4(0, 1, 1, 1) * colorSaturation;
	color4 top = color4(0, 0, 1, 1) * colorSaturation;
	color4 bottom = color4(1, 1, 0, 1) * colorSaturation;
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