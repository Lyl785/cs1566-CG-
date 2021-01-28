/*
 * vector.h
 *
 *  Created on: Sep. 11th, 2019
 *      Author: Yilun Liang
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#ifdef __APPLE__  // include Mac OS X verions of headers
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#endif  // __APPLE__


typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
} vec4;

typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

} vec3;

typedef struct
{
    vec4 x;
    vec4 y;
    vec4 z;

} mat3;




#endif /* VECTOR_H_ */
