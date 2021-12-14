#pragma once
//Needed for euler_angles.hpp
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

// Professor's glm helpers
#include <glm/james/mat4stack.h>
#include <glm/james/inline_helpers.h>
#include <glm/james/aabb.h>

// Include glew first
#define GLEW_STATIC
#include <GL\glew.h>
// Include glfw
#include <GLFW\glfw3.h>

// FreeImage
#ifndef FREEIMAGE_LIB
#define FREEIMAGE_LIB
#endif // !FREEIMAGE_LIB
#include <FreeImage.h>