#ifndef UTILITIES_H
#define UTILITIES_H

#include "Dependencies\glew\glew.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Dependencies/glut/glut.h"
#include "lodepng.h"

// Define a helpful macro for handling offsets into buffer objects
// used from Angel.h (Edward Angel)
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Utilities
{
private:
    Utilities();

public:
    struct OBJ{
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
        std::vector<glm::vec2> textureCoordinates;
    };

	struct PNG{
		size_t width, height;
		std::vector<GLubyte> colors;
	};

	struct BMP{
		size_t width, height;
		std::vector<GLbyte> colors;
	};

    static std::string loadFile(const std::string& filename);
	static void loadBMP(const std::string& filename, BMP &bmp);
	static void loadPNG(const std::string& filename, PNG &png);
    static void loadObj(const std::string& filename, OBJ &obj);
    static void tokenize(const std::string& str, const std::string& delimiter, std::vector<std::string> &result);
	static GLuint compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	static void compileStatus(GLuint shader);
};

#endif // UTILITIES_H
