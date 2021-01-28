//
//  main.c
//  proj2
//
//  Created by 梁轶伦 on 11/2/19.
//  Copyright © 2019 Yilun Liang. All rights reserved.
//
#ifdef __APPLE__  // include Mac OS X verions of headers

#include <stdlib.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
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
} vec2;

int print_vector(vec4 vector){
    printf("[ %.2f %.2f %.2f %.2f ]\n", vector.x, vector.y, vector.z, vector.w);
    return 0;
}

int print_matrix(mat4 matrix){
    printf("[");
    printf("[ %.2f %.2f %.2f %.2f ]\n", matrix.x.x, matrix.y.x, matrix.z.x, matrix.w.x);
    printf("[ %.2f %.2f %.2f %.2f ]\n", matrix.x.y, matrix.y.y, matrix.z.y, matrix.w.y);
    printf("[ %.2f %.2f %.2f %.2f ]\n", matrix.x.z, matrix.y.z, matrix.z.z, matrix.w.z);
    printf("[ %.2f %.2f %.2f %.2f ]\n", matrix.x.w, matrix.y.w, matrix.z.w, matrix.w.w);
    printf("]\n");
    return 0;
}

vec4 scalar_vector_multiplication(vec4 vector, GLfloat temp){
    vec4 result = {vector.x * temp, vector.y * temp, vector.z * temp, vector.w * temp};
    return result;
}

vec4 cross_product(vec4 vector1, vec4 vector2){
    vec4 result = {vector1.y * vector2.z - vector1.z * vector2.y, vector1.z *
        vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x, 0.0};
    return result;
}

GLfloat dot_product(vec4 vector1, vec4 vector2){
    GLfloat result = vector1.x * vector2.x +vector1.y *
        vector2.y + vector1.z * vector2.z + vector1.w * vector2.w;
    return result;
}

mat4 scalar_matrix_multiplication(mat4 matrix, GLfloat temp){

    vec4 vector1 = scalar_vector_multiplication(matrix.x, temp);
    vec4 vector2 = scalar_vector_multiplication(matrix.y, temp);
    vec4 vector3 = scalar_vector_multiplication(matrix.z, temp);
    vec4 vector4 = scalar_vector_multiplication(matrix.w, temp);
    mat4 result = {vector1, vector2, vector3, vector4};
    return result;
}
mat4 transpose_of_a_matrix(mat4 matrix){
    
    vec4 new_vector1 = {matrix.x.x, matrix.y.x, matrix.z.x, matrix.w.x};
    vec4 new_vector2 = {matrix.x.y, matrix.y.y, matrix.z.y, matrix.w.y};
    vec4 new_vector3 = {matrix.x.z, matrix.y.z, matrix.z.z, matrix.w.z};
    vec4 new_vector4 = {matrix.x.w, matrix.y.w, matrix.z.w, matrix.w.w};
    mat4 result ={new_vector1, new_vector2, new_vector3, new_vector4};
    return result;
}

vec4 matrix_vector_multiplication(mat4 matrix, vec4 vector){
    
    mat4 new_matrix = transpose_of_a_matrix(matrix);
    vec4 result = {dot_product(new_matrix.x, vector), dot_product(new_matrix.y, vector),
        dot_product(new_matrix.z, vector), dot_product(new_matrix.w, vector)};
    
    return result;
}

GLfloat determinant(mat4 matrix){
    GLfloat result1 = matrix.x.x * (matrix.y.y * matrix.z.z * matrix.w.w + matrix.y.z * matrix.z.w * matrix.w.y + matrix.y.w * matrix.z.y * matrix.w.z - matrix.y.w * matrix.z.z * matrix.w.y - matrix.y.z * matrix.z.y * matrix.w.w - matrix.y.y *  matrix.z.w * matrix.w.z) ;
    //printf("result1 %.2f\n", result1);
    GLfloat result1_1 = matrix.x.x * (matrix.y.y * (matrix.z.z * matrix.w.w - matrix.w.z * matrix.z.w) - matrix.z.y * (matrix.y.z * matrix.w.w - matrix.w.z * matrix.y.w) + matrix.w.y *  (matrix.y.z * matrix.z.w - matrix.z.z * matrix.y.w));
   // printf("result1_1%.2f\n", result1_1);

    
    GLfloat result2 = matrix.x.y * (matrix.y.x * matrix.z.z * matrix.w.w + matrix.z.x * matrix.w.z * matrix.y.w + matrix.w.x * matrix.y.z * matrix.z.w - matrix.w.x * matrix.z.z * matrix.y.w - matrix.z.x * matrix.y.z * matrix.w.w - matrix.y.x *   matrix.w.z * matrix.z.w) ;
     //printf("%.2f\n", result2);
    
    GLfloat result2_2 = matrix.y.x * (matrix.x.y * (matrix.z.z * matrix.w.w - matrix.w.z * matrix.z.w) - matrix.z.y * (matrix.x.z * matrix.w.w - matrix.w.z * matrix.x.w) + matrix.w.y * (matrix.x.z * matrix.z.w - matrix.z.z * matrix.z.z));
    
     GLfloat result3 = matrix.x.z * (matrix.y.x * matrix.z.y * matrix.w.w + matrix.z.x * matrix.w.y * matrix.y.w + matrix.w.x * matrix.y.y * matrix.z.w - matrix.w.x * matrix.z.y * matrix.y.w - matrix.z.x * matrix.y.y * matrix.w.w - matrix.y.x * matrix.w.y * matrix.z.w) ;
     //printf("%.2f\n", result3);
    
    GLfloat result3_3 = matrix.z.x * (matrix.x.y * (matrix.y.z * matrix.w.w - matrix.w.z * matrix.y.w) - matrix.y.y * (matrix.x.z * matrix.w.w - matrix.w.z * matrix.x.w) + matrix.w.y * (matrix.x.z * matrix.y.w - matrix.y.z * matrix.x.w) );
    
      GLfloat result4 = matrix.x.w * (matrix.y.x * matrix.z.y * matrix.w.z + matrix.z.x * matrix.w.y * matrix.y.z + matrix.w.x * matrix.y.y * matrix.z.z - matrix.w.x * matrix.z.y * matrix.y.z - matrix.z.x * matrix.y.y * matrix.w.z - matrix.y.x * matrix.w.y * matrix.z.z) ;
     //printf("%.2f\n", result4);
    
    GLfloat result4_4 = matrix.w.x * (matrix.x.y * (matrix.y.z * matrix.z.w - matrix.z.z * matrix.y.w) - matrix.y.y * (matrix.x.z * matrix.z.w - matrix.z.z * matrix.x.w) + matrix.z.y * (matrix.x.z * matrix.y.w - matrix.y.z * matrix.x.w));
    
    //GLfloat result_a = result1 - result2 + result3 - result4;
    GLfloat result = result1_1 - result2_2 + result3_3 - result4_4;
    //printf(" a %.2f\n", result_a);
    //printf(" b %.2f\n", result);
    return result;
}


mat4 minor_matrix(mat4 matrix){
    mat4 result;
    result.x.x = matrix.y.y * (matrix.z.z * matrix.w.w - matrix.w.z * matrix.z.w) - matrix.z.y * (matrix.y.z * matrix.w.w - matrix.w.z * matrix.y.w) + matrix.w.y * (matrix.y.z * matrix.z.w - matrix.z.z * matrix.y.w);
    
    result.x.y = matrix.y.x * (matrix.z.z * matrix.w.w - matrix.w.z * matrix.z.w ) - matrix.z.x * (matrix.y.z * matrix.w.w - matrix.w.z * matrix.y.w) + matrix.w.x * (matrix.y.z * matrix.z.w - matrix.z.z * matrix.y.w);
    
    result.x.z = matrix.y.x * (matrix.z.y * matrix.w.w - matrix.w.y * matrix.z.w) - matrix.z.x * (matrix.y.y * matrix.w.w - matrix.w.y * matrix.y.w) + matrix.w.x * (matrix.y.y * matrix.z.w - matrix.z.y * matrix.y.w);
    
    result.x.w = matrix.y.x * (matrix.z.y * matrix.w.z - matrix.w.y * matrix.z.z) - matrix.z.x * (matrix.y.y * matrix.w.z - matrix.w.y * matrix.y.z) + matrix.w.x * (matrix.y.y * matrix.z.z - matrix.z.y *matrix.y.z);
    
    result.y.x = matrix.x.y * (matrix.z.z * matrix.w.w - matrix.w.z * matrix.z.w) - matrix.z.y * (matrix.x.z * matrix.w.w - matrix.w.z * matrix.x.w) + matrix.w.y * (matrix.x.z * matrix.z.w - matrix.z.z * matrix.x.w);
    
    result.y.y = matrix.x.x * (matrix.z.z * matrix.w.w - matrix.w.z * matrix.z.w) - matrix.z.x * (matrix.x.z * matrix.w.w - matrix.w.z * matrix.x.w) + matrix.w.x * (matrix.x.z * matrix.z.w - matrix.z.z * matrix.x.w);
    
    result.y.z = matrix.x.x * (matrix.z.y * matrix.w.w - matrix.w.y * matrix.z.w) - matrix.z.x * (matrix.x.y * matrix.w.w - matrix.w.y * matrix.x.w) + matrix.w.x * (matrix.x.y * matrix.z.w - matrix.z.y * matrix.x.w);
    
    result.y.w = matrix.x.x * (matrix.z.y * matrix.w.z - matrix.w.y * matrix.z.z) - matrix.z.x * (matrix.x.y * matrix.w.z - matrix.w.y * matrix.x.z) + matrix.w.x * (matrix.x.y * matrix.z.z - matrix.z.y * matrix.x.z);
    
    result.z.x = matrix.x.y * (matrix.y.z * matrix.w.w - matrix.w.z * matrix.y.w) - matrix.y.y * (matrix.x.z * matrix.w.w - matrix.w.z * matrix.x.w) + matrix.w.y * (matrix.x.z * matrix.y.w - matrix.y.z * matrix.x.w);
    
    result.z.y = matrix.x.x * (matrix.y.z * matrix.w.w - matrix.w.z * matrix.y.w) - matrix.y.x * (matrix.x.z * matrix.w.w - matrix.w.z * matrix.x.w) + matrix.w.x * (matrix.x.z * matrix.y.w - matrix.y.z * matrix.x.w);
    
    result.z.z = matrix.x.x * (matrix.y.y * matrix.w.w - matrix.w.y * matrix.y.w) - matrix.y.x * (matrix.x.y * matrix.w.w - matrix.w.y * matrix.x.w) + matrix.w.x * (matrix.x.y * matrix.y.w - matrix.y.y * matrix.x.w);
    
    result.z.w = matrix.x.x * (matrix.y.y * matrix.w.z - matrix.w.y * matrix.y.z) - matrix.y.x * (matrix.x.y * matrix.w.z - matrix.w.y * matrix.x.z) + matrix.w.x * (matrix.x.y * matrix.y.z - matrix.y.y * matrix.x.z);
    
    result.w.x = matrix.x.y * (matrix.y.z * matrix.z.w - matrix.z.z * matrix.y.w) - matrix.y.y * (matrix.x.z * matrix.z.w - matrix.z.z * matrix.x.w) + matrix.z.y * (matrix.x.z * matrix.y.w - matrix.y.z * matrix.x.w);
    
    result.w.y = matrix.x.x * (matrix.y.z * matrix.z.w - matrix.z.z * matrix.y.w) - matrix.y.x * (matrix.x.z * matrix.z.w - matrix.z.z * matrix.x.w) + matrix.z.x * (matrix.x.z * matrix.y.w - matrix.y.z * matrix.x.w);
    
    result.w.z = matrix.x.x * (matrix.y.y * matrix.z.w - matrix.z.y * matrix.y.w) - matrix.y.x * (matrix.x.y * matrix.z.w - matrix.z.y * matrix.x.w) + matrix.z.x * (matrix.x.y * matrix.y.w - matrix.y.y * matrix.x.w);
    
    result.w.w = matrix.x.x * (matrix.y.y * matrix.z.z - matrix.z.y * matrix.y.z) - matrix.y.x * (matrix.x.y * matrix.z.z - matrix.z.y * matrix.x.z) + matrix.z.x * (matrix.x.y * matrix.y.z - matrix.y.y * matrix.x.z);
    
    return result;
    
}

mat4 cofactor(mat4 matrix){
    mat4 result;
    result.x.x = matrix.x.x;
    result.x.y = -matrix.x.y;
    result.x.z = matrix.x.z;
    result.x.w = -matrix.x.w;
    
    result.y.x = -matrix.y.x;
    result.y.y = matrix.y.y;
    result.y.z = -matrix.y.z;
    result.y.w = matrix.y.w;
    
    result.z.x = matrix.z.x;
    result.z.y = -matrix.z.y;
    result.z.z = matrix.z.z;
    result.z.w = -matrix.z.w;
    
    result.w.x = -matrix.w.x;
    result.w.y = matrix.w.y;
    result.w.z = -matrix.w.z;
    result.w.w = matrix.w.w;

    return result;
    
}

mat4 adjugate_matrix(mat4 matrix){
    
    GLfloat m0 = matrix.y.y * matrix.z.z * matrix.w.w + matrix.z.y * matrix.w.z * matrix.y.w + matrix.w.y * matrix.y.z * matrix.z.w - matrix.w.y * matrix.z.z * matrix.y.w - matrix.z.y * matrix.y.z * matrix.w.w - matrix.y.y * matrix.w.z * matrix.z.w;
  
    
    GLfloat m1 =  -matrix.y.x * matrix.z.z * matrix.w.w - matrix.z.x * matrix.w.z * matrix.y.w - matrix.w.x * matrix.y.z * matrix.z.w + matrix.w.x * matrix.z.z * matrix.y.w + matrix.z.x * matrix.y.z * matrix.w.w + matrix.y.x * matrix.w.z * matrix.w.z;
    
    
     GLfloat m2 = matrix.y.x * matrix.z.y * matrix.w.w + matrix.z.x * matrix.w.y * matrix.y.w + matrix.w.x * matrix.y.y * matrix.z.w - matrix.w.x * matrix.z.y * matrix.y.w - matrix.z.x * matrix.y.y * matrix.w.w - matrix.y.x * matrix.w.y * matrix.z.w;
    
     GLfloat m3 = -matrix.y.x * matrix.z.y * matrix.w.z - matrix.z.x * matrix.w.y * matrix.y.z - matrix.w.x * matrix.y.y * matrix.z.z + matrix.w.x * matrix.z.y * matrix.y.z + matrix.z.x * matrix.y.y * matrix.w.z + matrix.y.x * matrix.w.y * matrix.z.z;
    
     GLfloat m4 = -matrix.x.y * matrix.z.z * matrix.w.w - matrix.z.y * matrix.w.z * matrix.x.w - matrix.w.y * matrix.x.z * matrix.z.w + matrix.w.y * matrix.z.z * matrix.x.w + matrix.z.y * matrix.x.z * matrix.w.w + matrix.x.y * matrix.w.z * matrix.z.w;
    
     GLfloat m5 = matrix.x.x * matrix.z.z * matrix.w.w + matrix.z.x * matrix.w.z * matrix.x.w + matrix.w.x * matrix.x.z * matrix.z.w - matrix.w.x * matrix.z.z * matrix.x.w - matrix.z.x * matrix.x.z * matrix.w.w - matrix.x.x * matrix.w.z * matrix.z.w;
    
     GLfloat m6 = -matrix.x.x * matrix.z.y * matrix.w.x - matrix.z.y * matrix.x.w * matrix.x.w - matrix.w.x * matrix.x.y * matrix.z.w + matrix.w.x * matrix.z.y * matrix.x.w + matrix.z.x * matrix.x.y * matrix.w.w + matrix.x.x * matrix.w.y * matrix.z.w;
    
     GLfloat m7 = matrix.x.x * matrix.z.y * matrix.w.z + matrix.z.x * matrix.w.y * matrix.x.z + matrix.w.x * matrix.x.y * matrix.z.z - matrix.w.z * matrix.z.y * matrix.x.z - matrix.z.x * matrix.x.y * matrix.w.z - matrix.x.x * matrix.w.y * matrix.z.z;
    
     GLfloat m8 = matrix.x.y * matrix.y.z * matrix.w.w + matrix.y.y * matrix.w.z * matrix.x.w + matrix.w.y * matrix.x.z * matrix.y.w - matrix.w.y * matrix.y.z * matrix.x.w - matrix.y.y * matrix.x.z * matrix.w.w - matrix.x.y * matrix.w.z * matrix.y.w;
    
     GLfloat m9 = -matrix.x.x * matrix.y.z * matrix.w.w - matrix.y.x * matrix.w.z * matrix.x.w - matrix.w.x * matrix.x.z * matrix.y.w + matrix.w.x * matrix.y.z * matrix.x.w + matrix.y.x * matrix.x.z * matrix.w.w + matrix.x.x * matrix.w.z * matrix.y.w;
    
     GLfloat m10 = matrix.x.x * matrix.y.y * matrix.w.w + matrix.y.x * matrix.w.y * matrix.x.w + matrix.w.x * matrix.x.y * matrix.y.w - matrix.w.x * matrix.y.y * matrix.x.w - matrix.y.x * matrix.x.y * matrix.w.w - matrix.x.x * matrix.w.y * matrix.y.w;
    
     GLfloat m11 = -matrix.x.x * matrix.y.y * matrix.w.z - matrix.y.x * matrix.w.y * matrix.x.z - matrix.w.x * matrix.x.y * matrix.y.z + matrix.w.x * matrix.y.y * matrix.x.z + matrix.y.x * matrix.x.y * matrix.w.z + matrix.x.x * matrix.w.y * matrix.y.z;
    
     GLfloat m12 = -matrix.x.y * matrix.y.z * matrix.z.w - matrix.y.y * matrix.z.z * matrix.x.w - matrix.z.y * matrix.x.z * matrix.y.w + matrix.z.y * matrix.y.z * matrix.x.w + matrix.y.y * matrix.x.z * matrix.z.w + matrix.x.y * matrix.z.z * matrix.y.w;
    
     GLfloat m13 = matrix.x.x * matrix.y.z * matrix.z.w + matrix.y.x * matrix.z.z * matrix.x.w + matrix.z.x * matrix.x.z * matrix.y.w - matrix.z.x * matrix.y.z * matrix.x.w - matrix.y.x * matrix.x.z * matrix.z.w - matrix.x.x * matrix.z.z * matrix.y.w;
    
     GLfloat m14 =  -matrix.x.x * matrix.y.y * matrix.z.w - matrix.y.x * matrix.z.y * matrix.x.w - matrix.z.x * matrix.x.y * matrix.y.w + matrix.z.x * matrix.y.y * matrix.x.w + matrix.y.x * matrix.x.y * matrix.z.w + matrix.x.x * matrix.z.y * matrix.y.w;
    
     GLfloat m15 = matrix.x.x * matrix.y.y * matrix.z.z + matrix.y.x * matrix.z.y * matrix.x.z + matrix.z.x * matrix.x.y * matrix.y.z - matrix.z.x * matrix.y.y * matrix.x.z - matrix.y.x * matrix.x.y * matrix.z.z - matrix.x.x * matrix.z.y * matrix.y.z;
    
    mat4 result = {{m0, m1, m2, m3}, {m4, m5, m6, m7},{m8, m9, m10 ,m11},{m12, m13, m14, m15}};
    
    return result;
}


mat4 inverse_of_a_matrix(mat4 matrix){
    
    mat4 result;
    result = minor_matrix(matrix);
    result = cofactor(result);
    result = transpose_of_a_matrix(result);
    GLfloat de = 1/determinant(matrix);
    result = scalar_matrix_multiplication(result, de);
    //printf("test for the inverse results./n");
    //print_matrix(result);
    return result;


}



mat4 matrix_matrix_multiplication(mat4 matrix1, mat4 matrix2){
  
    mat4 new_matrix1 = transpose_of_a_matrix(matrix1);
    GLfloat m0 = dot_product(new_matrix1.x, matrix2.x);
    GLfloat m1 = dot_product(new_matrix1.y, matrix2.x);
    GLfloat m2 = dot_product(new_matrix1.z, matrix2.x);
    GLfloat m3 = dot_product(new_matrix1.w, matrix2.x);
    GLfloat m4 = dot_product(new_matrix1.x, matrix2.y);
    GLfloat m5 = dot_product(new_matrix1.y, matrix2.y);
    GLfloat m6 = dot_product(new_matrix1.z, matrix2.y);
    GLfloat m7 = dot_product(new_matrix1.w, matrix2.y);
    GLfloat m8 = dot_product(new_matrix1.x, matrix2.z);
    GLfloat m9 = dot_product(new_matrix1.y, matrix2.z);
    GLfloat m10 = dot_product(new_matrix1.z, matrix2.z);
    GLfloat m11 = dot_product(new_matrix1.w, matrix2.z);
    GLfloat m12 = dot_product(new_matrix1.x, matrix2.w);
    GLfloat m13 = dot_product(new_matrix1.y, matrix2.w);
    GLfloat m14 = dot_product(new_matrix1.z, matrix2.w);
    GLfloat m15 = dot_product(new_matrix1.w, matrix2.w);
    
    mat4 result = {{m0, m1, m2, m3},{m4, m5, m6, m7},{m8, m9, m10, m11},{m12, m13, m14, m15}};
    return result;
}

mat4 rotate_z(float theta)
{
mat4 result;
float s = (M_PI/180.0)*theta;
result.x.x = cos(s); result.y.x = -sin(s); result.z.x = 0; result.w.x = 0;
result.x.y = sin(s); result.y.y = cos(s); result.z.y = 0; result.w.y = 0;
result.x.z = 0; result.y.z = 0; result.z.z = 1; result.w.z = 0;
result.x.w = 0; result.y.w = 0; result.z.w = 0; result.w.w = 1;
return result;
}


mat4 rotate_x(float theta)
{
    mat4 result;
    float s = (M_PI/180.0)*theta;
    result.x.x = 1; result.y.x = 0;      result.z.x = 0;       result.w.x = 0;
    result.x.y = 0; result.y.y = cos(s); result.z.y = -sin(s); result.w.y = 0;
    result.x.z = 0; result.y.z = sin(s); result.z.z = cos(s);  result.w.z = 0;
    result.x.w = 0; result.y.w = 0;      result.z.w = 0;       result.w.w = 1;
    return result;
}


mat4 rotate_y(float theta)
{
    mat4 result;
    float s = (M_PI/180.0)*theta;
    result.x.x = cos(s); result.y.x = 0; result.z.x = sin(s);  result.w.x = 0;
    result.x.y = 0;  result.y.y = 1; result.z.y = 0; result.w.y = 0;
    result.x.z = -sin(s); result.y.z = 0; result.z.z = cos(s); result.w.z = 0;
    result.x.w = 0; result.y.w = 0;      result.z.w = 0;  result.w.w = 1;
    return result;

    
}

mat4 scaling(GLfloat scale_x, GLfloat scale_y, GLfloat scale_z){
    
    mat4 result = {{scale_x , 0, 0, 0}, {0,scale_y , 0, 0}, {0, 0,  scale_z, 0}, {0, 0, 0, 1.0}};
    return result;
    
}

mat4 translate(GLfloat x, GLfloat y, GLfloat z)
{
    mat4 result = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
    result.w.x = x;
    result.w.y = y;
    result.w.z = z;
    return result;
}





vec4 cube_vertices[36] =
{
{ 0.5, 0.5, 0.5, 1.0}, // Top right
{ -0.5, 0.5, 0.5, 1.0}, // Top left
{ -0.5, -0.5, 0.5, 1.0}, // Bottom left
{ 0.5, 0.5, 0.5, 1.0}, // Top right
{ -0.5, -0.5, 0.5, 1.0}, // Bottom left
{ 0.5, -0.5, 0.5, 1.0},
    
    { -0.5, 0.5, -0.5, 1.0}, // Top right
    { 0.5, 0.5, -0.5, 1.0}, // Top left
    {  0.5, -0.5, -0.5, 1.0}, // Bottom left
    { -0.5, 0.5, -0.5, 1.0}, // Top right
    { 0.5, -0.5, -0.5, 1.0}, // Bottom left
    { -0.5, -0.5, -0.5, 1.0},
    
    { 0.5, 0.5, -0.5, 1.0}, // Top right
    { -0.5, 0.5, -0.5, 1.0}, // Top left
    { -0.5, 0.5, 0.5, 1.0}, // Bottom left
    { 0.5, 0.5, -0.5, 1.0}, // Top right
    { -0.5, 0.5, 0.5, 1.0}, // Bottom left
    { 0.5, 0.5, 0.5, 1.0},
    
    { -0.5, -0.5, -0.5, 1.0}, // Top right
    { 0.5, -0.5, -0.5, 1.0}, // Top left
    { 0.5, -0.5, 0.5, 1.0}, // Bottom left
    { -0.5, -0.5, -0.5, 1.0}, // Top right
    { 0.5, -0.5, 0.5, 1.0}, // Bottom left
    { -0.5, -0.5, 0.5, 1.0},
    
    
    { 0.5, 0.5, -0.5, 1.0}, // Top right
        { 0.5, 0.5, 0.5, 1.0}, // Top left
        { 0.5, -0.5, 0.5, 1.0}, // Bottom left
        { 0.5, 0.5, -0.5, 1.0}, // Top right
        { 0.5, -0.5, 0.5, 1.0}, // Bottom left
        { 0.5, -0.5, -0.5, 1.0},

   { -0.5, 0.5, 0.5, 1.0}, // Top right
    { -0.5, 0.5, -0.5, 1.0}, // Top left
    {  -0.5, -0.5, -0.5, 1.0}, // Bottom left
    { -0.5, 0.5, 0.5, 1.0}, // Top right
    {  -0.5, -0.5, -0.5, 1.0}, // Bottom left
    { -0.5, -0.5, 0.5, 1.0},

     
};


vec4 colors[36] =
{{1.0, 0.0, 0.0, 1.0}, // Red
{1.0, 0.0, 0.0, 1.0}, // Green
{1.0, 0.0, 0.0, 1.0}, // Blue
{1.0, 0.0, 0.0, 1.0}, // Red
{1.0, 0.0, 0.0, 1.0}, // Blue
{1.0, 0.0, 0.0, 1.0},
    
{0.0, 1.0, 0.0, 1.0},
{0.0, 1.0, 0.0, 1.0},
{0.0, 1.0, 0.0, 1.0},
{0.0, 1.0, 0.0, 1.0},
{0.0, 1.0, 0.0, 1.0},
{0.0, 1.0, 0.0, 1.0},
    
{0.0, 0.0, 1.0, 1.0},
{0.0, 0.0, 1.0, 1.0},
{0.0, 0.0, 1.0, 1.0},
{0.0, 0.0, 1.0, 1.0},
{0.0, 0.0, 1.0, 1.0},
{0.0, 0.0, 1.0, 1.0},
    
{1.0, 0.0, 1.0, 1.0},
{1.0, 0.0, 1.0, 1.0},
{1.0, 0.0, 1.0, 1.0},
{1.0, 0.0, 1.0, 1.0},
{1.0, 0.0, 1.0, 1.0},
{1.0, 0.0, 1.0, 1.0},
    
{1.0, 1.0, 0.0, 1.0},
{1.0, 1.0, 0.0, 1.0},
{1.0, 1.0, 0.0, 1.0},
{1.0, 1.0, 0.0, 1.0},
{1.0, 1.0, 0.0, 1.0},
{1.0, 1.0, 0.0, 1.0},
    
{0.0, 1.0, 1.0, 1.0},
{0.0, 1.0, 1.0, 1.0},
{0.0, 1.0, 1.0, 1.0},
{0.0, 1.0, 1.0, 1.0},
{0.0, 1.0, 1.0, 1.0},
{0.0, 1.0, 1.0, 1.0},
    
    
    
    
};





void base(){
    
    
    
    
}

vec2 cube_tex_coords[36] = {{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5},{0.0,0.0},{0.0,0.5},{0.5,0.5}};

vec2 cube_tex_coords_b[36] = {{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,0.5},{0.5,1.0},{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,0.5},{0.5,1.0},{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,0.5},{0.5,1.0},{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,0.5},{0.5,1.0},{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,0.5},{0.5,1.0},{0.0,0.5},{0.0,1.0},{0.5,1.0},{0.0,0.5},{0.5,0.5},{0.5,1.0}};


vec2 cube_tex_coords_p[36] = {{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.0},{1.0,0.5},{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.0},{1.0,0.5},{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.0},{1.0,0.5},{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.0},{1.0,0.5},{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.0},{1.0,0.5},{0.5,0.0},{0.5,0.5},{1.0,0.5},{0.5,0.0},{1.0,0.0},{1.0,0.5}};



 
enum {
    W = 8,         // width of maze
    H = 8,          // height of maze
};
 
enum {
    North,
    East,
    South,
    West,
    NDir
};
 
char visited[H][W];
char horz[H][W - 1];        // horizontal E-W paths in the maze
char vert[H - 1][W];        // veritcal N-S paths in the maze
 

int adjacent(int dir[], int x, int y)
{
    int ndir = 0;
 
    if (y > 0     && visited[y - 1][x] == 0) dir[ndir++] = North;
    if (x < W - 1 && visited[y][x + 1] == 0) dir[ndir++] = East;
    if (y < H - 1 && visited[y + 1][x] == 0) dir[ndir++] = South;
    if (x > 0     && visited[y][x - 1] == 0) dir[ndir++] = West;
 
    return ndir;
}
 

void dfs(int x, int y)
{
    int dir[NDir];
    int ndir;
 
    visited[y][x] = 1;
 
    ndir = adjacent(dir, x, y);
 
    while (ndir) {
        int pick = rand() % ndir;
 
        switch (dir[pick]) {
        case North: vert[y - 1][x] = 1; dfs(x, y - 1); break;
        case East:  horz[y][x] = 1;     dfs(x + 1, y); break;
        case South: vert[y][x] = 1;     dfs(x, y + 1); break;
        case West:  horz[y][x - 1] = 1; dfs(x - 1, y); break;
        }
 
        ndir = adjacent(dir, x, y);
    }
}
 

void map(void)
{
    int i, j;
 
    for (i = 0; i < W; i++) {
        putchar('_');
        putchar('_');
    }
    
    putchar('\n');
 
    for (j = 0; j < H; j++) {
        putchar('|');
 
        for (i = 0; i < W; i++) {
            putchar(j < H - 1 && vert[j][i] ? ' ' : '_');
            putchar(i < W - 1 && horz[j][i] ? '_' : '|');
        }
 
        putchar('\n');
    }
}



struct cell{
    int south;
    int north;
    int west;
    int east;
};

struct cell maze[8][8];
void maze_fun(){
//struct cell maze[8][8];
    
    maze[0][0].south = 0;
    maze[0][0].north = 1;
    maze[0][0].west = 0;
    maze[0][0].east = 1;
    
    maze[0][1].south = 1;
    maze[0][1].north = 1;
    maze[0][1].west = 1;
    maze[0][1].east = 0;
    
    maze[0][2].south = 1;
    maze[0][2].north = 1;
    maze[0][2].west = 0;
    maze[0][2].east = 0;
    
    maze[0][3].south = 1;
    maze[0][3].north = 1;
    maze[0][3].west = 0;
    maze[0][3].east = 0;
    
    maze[0][4].south = 1;
    maze[0][4].north = 1;
    maze[0][4].west = 0;
    maze[0][4].east = 0;
    
    maze[0][5].south = 0;
    maze[0][5].north = 1;
    maze[0][5].west = 0;
    maze[0][5].east = 0;
    
    maze[0][6].south = 1;
    maze[0][6].north = 1;
    maze[0][6].west = 0;
    maze[0][6].east = 0;
    
    maze[0][7].south = 1;
    maze[0][7].north = 1;
    maze[0][7].west = 0;
    maze[0][7].east = 1;
    
    maze[1][0].south = 1;
    maze[1][0].north = 0;
    maze[1][0].west = 1;
    maze[1][0].east = 0;
    
    maze[1][1].south = 1;
    maze[1][1].north = 1;
    maze[1][1].west = 0;
    maze[1][1].east = 0;
      
    maze[1][2].south = 0;
    maze[1][2].north = 1;
    maze[1][2].west = 0;
    maze[1][2].east = 1;
      
    maze[1][3].south = 0;
    maze[1][3].north = 1;
    maze[1][3].west = 1;
    maze[1][3].east = 0;
      
    maze[1][4].south = 0;
    maze[1][4].north = 1;
    maze[1][4].west = 0;
    maze[1][4].east = 1;
      
    maze[1][5].south = 0;
    maze[1][5].north = 0;
    maze[1][5].west = 1;
    maze[1][5].east = 1;
      
    maze[1][6].south = 0;
    maze[1][6].north = 1;
    maze[1][6].west = 1;
    maze[1][6].east = 0;
      
    maze[1][7].south = 0;
    maze[1][7].north = 1;
    maze[1][7].west = 0;
    maze[1][7].east = 1;
    
    maze[2][0].south = 1;
    maze[2][0].north = 1;
    maze[2][0].west = 1;
    maze[2][0].east = 0;
    
    maze[2][1].south = 0;
    maze[2][1].north = 1;
    maze[2][1].west = 0;
    maze[2][1].east = 1;
    
    maze[2][2].south = 0;
    maze[2][2].north = 0;
    maze[2][2].west = 1;
    maze[2][2].east = 1;
    
    maze[2][3].south = 0;
    maze[2][3].north = 0;
    maze[2][3].west = 1;
    maze[2][3].east = 1;
    
    maze[2][4].south = 0;
    maze[2][4].north = 0;
    maze[2][4].west = 1;
    maze[2][4].east = 1;
    
    maze[2][5].south = 1;
    maze[2][5].north = 0;
    maze[2][5].west = 1;
    maze[2][5].east = 0;
    
    maze[2][6].south = 1;
    maze[2][6].north = 0;
    maze[2][6].west = 0;
    maze[2][6].east = 1;
    
    maze[2][7].south = 0;
    maze[2][7].north = 0;
    maze[2][7].west = 1;
    maze[2][7].east = 1;
    
    maze[3][0].south = 0;
    maze[3][0].north = 1;
    maze[3][0].west = 1;
    maze[3][0].east = 0;
    
    maze[3][1].south = 1;
    maze[3][1].north = 0;
    maze[3][1].west = 0;
    maze[3][1].east = 1;
    
    maze[3][2].south = 1;
    maze[3][2].north = 0;
    maze[3][2].west = 1;
    maze[3][2].east = 0;
    
    maze[3][3].south = 1;
    maze[3][3].north = 0;
    maze[3][3].west = 0;
    maze[3][3].east = 1;
    
    maze[3][4].south = 1;
    maze[3][4].north = 0;
    maze[3][4].west = 1;
    maze[3][4].east = 0;
    
    maze[3][5].south = 1;
    maze[3][5].north = 1;
    maze[3][5].west = 0;
    maze[3][5].east = 0;
    
    maze[3][6].south = 0;
    maze[3][6].north = 1;
    maze[3][6].west = 0;
    maze[3][6].east = 1;
    
    maze[3][7].south = 0;
    maze[3][7].north = 0;
    maze[3][7].west = 1;
    maze[3][7].east = 1;
    
    maze[4][0].south = 0;
    maze[4][0].north = 0;
    maze[4][0].west = 1;
    maze[4][0].east = 1;
    
    maze[4][1].south = 0;
    maze[4][1].north = 1;
    maze[4][1].west = 1;
    maze[4][1].east = 0;
    
    maze[4][2].south = 1;
    maze[4][2].north = 1;
    maze[4][2].west = 0;
    maze[4][2].east = 0;
    
    maze[4][3].south = 0;
    maze[4][3].north = 1;
    maze[4][3].west = 0;
    maze[4][3].east = 1;

    maze[4][4].south = 0;
    maze[4][4].north = 1;
    maze[4][4].west = 1;
    maze[4][4].east = 0;

    maze[4][5].south = 1;
    maze[4][5].north = 1;
    maze[4][5].west = 0;
    maze[4][5].east = 0;

    maze[4][6].south = 1;
    maze[4][6].north = 0;
    maze[4][6].west = 0;
    maze[4][6].east = 1;
    
    maze[4][7].south = 0;
    maze[4][7].north = 0;
    maze[4][7].west = 1;
    maze[4][7].east = 1;
    
    maze[5][0].south = 0;
    maze[5][0].north = 0;
    maze[5][0].west = 1;
    maze[5][0].east = 1;
    
    maze[5][1].south = 0;
    maze[5][1].north = 0;
    maze[5][1].west = 1;
    maze[5][1].east = 1;
    
    maze[5][2].south = 0;
    maze[5][2].north = 1;
    maze[5][2].west = 1;
    maze[5][2].east = 1;
    
    maze[5][3].south = 0;
    maze[5][3].north = 0;
    maze[5][3].west = 1;
    maze[5][3].east = 1;
    
    maze[5][4].south = 0;
    maze[5][4].north = 0;
    maze[5][4].west = 1;
    maze[5][4].east = 1;
    
    maze[5][5].south = 1;
    maze[5][5].north = 1;
    maze[5][5].west = 1;
    maze[5][5].east = 0;
    
    maze[5][6].south = 1;
    maze[5][6].north = 1;
    maze[5][6].west = 0;
    maze[5][6].east = 0;
    
    maze[5][7].south = 0;
    maze[5][7].north = 0;
    maze[5][7].west = 0;
    maze[5][7].east = 1;
    
    maze[6][0].south = 0;
    maze[6][0].north = 0;
    maze[6][0].west = 1;
    maze[6][0].east = 1;
    
    maze[6][1].south = 0;
    maze[6][1].north = 0;
    maze[6][1].west = 1;
    maze[6][1].east = 1;
    
    maze[6][2].south = 0;
    maze[6][2].north = 0;
    maze[6][2].west = 1;
    maze[6][2].east = 1;
    
    maze[6][3].south = 0;
    maze[6][3].north = 0;
    maze[6][3].west = 1;
    maze[6][3].east = 1;
    
    maze[6][4].south = 0;
    maze[6][4].north = 0;
    maze[6][4].west = 1;
    maze[6][4].east = 1;
    
    maze[6][5].south = 0;
    maze[6][5].north = 1;
    maze[6][5].west = 1;
    maze[6][5].east = 0;
    
    maze[6][6].south = 0;
    maze[6][6].north = 1;
    maze[6][6].west = 0;
    maze[6][6].east = 1;
    
    maze[6][7].south = 0;
    maze[6][7].north = 0;
    maze[6][7].west = 1;
    maze[6][7].east = 1;
    
    maze[7][0].south = 1;
    maze[7][0].north = 0;
    maze[7][0].west = 1;
    maze[7][0].east = 0;
    
    maze[7][1].south = 1;
    maze[7][1].north = 0;
    maze[7][1].west = 0;
    maze[7][1].east = 0;
        
    maze[7][2].south = 1;
    maze[7][2].north = 0;
    maze[7][2].west = 0;
    maze[7][2].east = 1;
        
    maze[7][3].south = 1;
    maze[7][3].north = 0;
    maze[7][3].west = 1;
    maze[7][3].east = 0;
        
    maze[7][4].south = 1;
    maze[7][4].north = 0;
    maze[7][4].west = 0;
    maze[7][4].east = 0;

    maze[7][5].south = 1;
    maze[7][5].north = 0;
    maze[7][5].west = 0;
    maze[7][5].east = 1;
        
    maze[7][6].south = 1;
    maze[7][6].north = 0;
    maze[7][6].west = 1;
    maze[7][6].east = 0;
        
    maze[7][7].south = 1;
    maze[7][7].north = 0;
    maze[7][7].west = 0;
    maze[7][7].east = 0;
    
};

GLuint model_view_location, projection_location;
mat4 model_view = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0,0.0, 0.0}, {0.0,0.0,1.0,0.0},{0.0, 0.0, 0.0, 1.0}};
mat4 projection = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0,0.0, 0.0}, {0.0,0.0,1.0,0.0},{0.0, 0.0, 0.0, 1.0}};
vec4 vertices[36*79*2 + 81*36 + 36];



vec2 tex_coords[36*79*2+81*36 + 36];


int num_vertices = 36*79*2 + 81*36 + 36;
int v_index= 0;

void base_fun(){
    mat4 tri_b = matrix_matrix_multiplication(translate(4, -0.5, 4), scaling(20, 0.5, 20));
    int i;
    for(i = 0; i<36; i++){
        vertices[v_index] = matrix_vector_multiplication(tri_b, cube_vertices[i]);
        tex_coords[v_index] = cube_tex_coords_b[i];
        v_index++;
    }
}

void pole(){
    int r,c, i;
    GLfloat index_x_pole = 0, index_y_pole = 0, index_z_pole = 0;
    mat4 tri_p;
    
    for(r = 0; r < 9; r++){
        index_x_pole = 0;
        for(c = 0; c < 9; c++){
            tri_p = matrix_matrix_multiplication(translate(index_x_pole,index_y_pole ,index_z_pole ),scaling(0.3, 1.5, 0.3) );
      
            for(i = 0; i < 36; i++){
                vertices[v_index] = matrix_vector_multiplication(tri_p, cube_vertices[i]);
                tex_coords[v_index] = cube_tex_coords_p[i];
                v_index++;
                }
            index_x_pole++;
            }
        index_z_pole++;
    }
}

void world(){
    maze_fun();
    int r; int c; int i;
    GLfloat index_x_1 = 0.5, index_y_1 = 0, index_z_1 = 0;
    GLfloat index_x_2 = 1, index_y_2 = 0, index_z_2 = 0.5;
    GLfloat index_x_3 = 0, index_y_3 = 0, index_z_3 = 0.5;
    GLfloat index_x_4 = 0.5, index_y_4 = 0, index_z_4 = 1;
    
    mat4 tri;
    //mat4 tri_p;
    for(r = 0; r < 8; r++){
        index_x_1 = 0.5;
        index_x_2 = 1;
        for(c = 0; c < 8; c++){
  
            
            if(maze[r][c].north == 1){
                tri = matrix_matrix_multiplication(translate(index_x_1, index_y_1, index_z_1), scaling(1.0, 1.0, 0.1));
                
                for(i = 0; i < 36; i++){
                    vertices[v_index] = matrix_vector_multiplication(tri, cube_vertices[i]);
                    tex_coords[v_index] = cube_tex_coords[i];
                    v_index+= 1;
                }
            }
            
            
            if(maze[r][c].east == 1){
                
                tri = matrix_matrix_multiplication(translate(index_x_2, index_y_2, index_z_2), scaling(0.1,1.0,1.0));
                
                for(i = 0; i < 36; i++){
                    vertices[v_index] = matrix_vector_multiplication(tri, cube_vertices[i]);
                    tex_coords[v_index] = cube_tex_coords[i];
                    v_index+= 1;
                }
            }
            
            if(v_index == 1)
            {
                printf("vertices[0]\n");
                print_vector(vertices[0]);
            }
            //tri_p = matrix_matrix_multiplication(translate(c, 0, r), //scaling(0.1, 1.0, 0.1));
            
        

            
            
            index_x_1++;
            index_x_2++;
        }
        index_z_1++;
        index_z_2++;
    }
    
    for(r = 0; r < 8; r++){
    index_x_3 = 0;
    index_x_4 = 0.5;
        for(c = 0; c < 8; c++){
            if(maze[r][c].south == 1){
                 tri = matrix_matrix_multiplication(translate(index_x_4, index_y_4, index_z_4), scaling(1.0, 1.0, 0.1));
                 
                 for(i = 0; i < 36; i++){
                     vertices[v_index] = matrix_vector_multiplication(tri, cube_vertices[i]);
                     tex_coords[v_index] = cube_tex_coords[i];
                     v_index+= 1;
                 }
             }
             
             
             if(maze[r][c].west == 1){
                 
                 tri = matrix_matrix_multiplication(translate(index_x_3, index_y_3, index_z_3), scaling(0.1,1.0,1.0));
                 
                 for(i = 0; i < 36; i++){
                     vertices[v_index] = matrix_vector_multiplication(tri, cube_vertices[i]);
                     tex_coords[v_index] = cube_tex_coords[i];
                     v_index+= 1;
                 }
             }
            
                      index_x_3++;
                      index_x_4++;
            
        }
    
    index_z_3++;
    index_z_4++;
    }
    
};

//struct cell solve[8][8];
/*void maze_solve(){
    int r, c;
    maze_fun();
    for(r = 0; r < 8; r++){
        for(c = 0; c < 8; c++){
            
        }
    }
    
};*/

mat4 ortho(GLfloat left, GLfloat right,
           GLfloat bottom, GLfloat top,
           GLfloat near, GLfloat far){
    
    mat4 T;
    T.x.x = 1; T.x.y = 0; T.x.z = 0; T.x.w = 0;
    T.y.x = 0; T.y.y = 1; T.y.z = 0; T.y.w = 0;
    T.z.x = 0; T.z.y = 0; T.z.z = 1; T.z.w = 0;
    T.w.x = -((right+left)/2) ; T.w.y = -((top+bottom)/2) ;
    T.w.z = -((near+far)/2);  T.w.w = 1.0;
    
    mat4 S;
    S.x.x = 2/(left-right); S.x.y = 0; S.x.z = 0; S.x.w = 0;
    S.y.x = 0; S.y.y = 2/(top-bottom); S.y.z = 0; S.y.w = 0;
    S.z.x = 0; S.z.y = 0; S.z.z = 2/(near-far); S.z.w = 0;
    S.w.x = 0; S.w.y = 0; S.w.z = 0; S.w.w = 1;
    
    mat4 result = matrix_matrix_multiplication(S, T);
    return result;
};

mat4 frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
             GLfloat near, GLfloat far){
    
    mat4 result;
    result.x.x = -2*near/(right-left); result.x.y = 0; result.x.z = 0; result.x.w = 0;
    result.y.x = 0; result.y.y = -2 * near/(top-bottom); result.y.z = 0; result.y.w = 0;
    result.z.x = (left+right)/(right-left); result.z.y = (bottom+top)/(top-bottom); result.z.z = (near+far)/(far-near); result.z.w = -1;
    result.w.x = 0; result.w.y = 0; result.w.z = -((2*near*far)/(far-near)); result.w.w = 0;
    
    return result;
    
};



mat4 look_at(GLfloat eyex, GLfloat eyey, GLfloat eyez,
             GLfloat atx, GLfloat aty, GLfloat atz,
             GLfloat upx, GLfloat upy, GLfloat upz){
    
    vec4 vpn;
    vpn.x = eyex - atx;
    vpn.y = eyey - aty;
    vpn.z = eyez - atz;
    vpn.w = 0.0;
    
    GLfloat absvpn = 1/sqrt(pow(vpn.x,2) + pow(vpn.y,2)+ pow(vpn.z,2));
    vec4 n = scalar_vector_multiplication(vpn, absvpn);
    
    vec4 vup;
    vup.x = upx;
    vup.y = upy;
    vup.z = upz;
    vup.w = 0.0;
    
    vec4 u_prime = cross_product(vup, n);
    GLfloat absu = 1/sqrt(pow(u_prime.x,2) + pow(u_prime.y,2) + pow(u_prime.z,2));
    vec4 u = scalar_vector_multiplication(u_prime, absu);
    
    
    vec4 v_prime = cross_product(n, u);
    GLfloat absv = 1/sqrt(pow(v_prime.x,2) + pow(v_prime.y, 2) + pow(v_prime.z, 2));
    vec4 v = scalar_vector_multiplication(v_prime, absv);
    
    mat4 result;
    result.x.x = u.x; result.y.x = u.y; result.z.x = u.z; result.w.x = 0;
    result.x.y = v.x; result.y.y = v.y; result.z.y = v.z; result.w.y = 0;
    result.x.z = n.x; result.y.z = n.y; result.z.z = n.z; result.w.z = 0;
    result.x.w = eyex;   result.y.w = eyey;   result.z.w = eyez;   result.w.w = 1;
    
   // printf("test result for the result\n");
    //print_matrix(result);
    
    result = transpose_of_a_matrix(result);
    //printf("test for the transpose\n");
    //print_matrix(result1);
    
    result = inverse_of_a_matrix(result);
   // printf("test for the inverse\n");
   // print_matrix(result);
    
    //mat4 result3 = matrix_matrix_multiplication(result, result1);
   // printf("test for the mult");
   // print_matrix(result3);
    //print_matrix(result);
    //mat4 matrix1 = {{1,-5,9,13},{2,6,-10,14},{3,7,11,15},{4,8,12,-16}};
    //mat4 m2 = inverse_of_a_matrix(matrix1);
    //print_matrix(matrix1);
    //print_matrix(result);
    return result;
    
};




void init(void)
{
    int width = 800;
    int height = 800;
    GLubyte my_texels[width][height][3];
    
    FILE *fp = fopen("/Users/liangyilun/Desktop/cs1566/proj2/proj2/p2texture04.raw", "r");
    fread(my_texels, width * height * 3, 1, fp);
    fclose(fp);
    
    GLuint program = initShader("/Users/liangyilun/Desktop/cs1566/proj2/proj2/vshader.glsl", "/Users/liangyilun/Desktop/cs1566/proj2/proj2/fshader.glsl");
    glUseProgram(program);
    
    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_texels);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(tex_coords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(tex_coords), tex_coords);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));
    
    //ctm_location = glGetUniformLocation(program, "ctm");
    
    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vertices) + sizeof(colors)));
    
    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);
    
    model_view_location = glGetUniformLocation(program,
    "model_view_matrix");
    projection_location = glGetUniformLocation(program,
    "projection_matrix");
    
    printf("texture_location: %i\n", texture_location);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
    
    //print_vector(vertices[0]);
    //print_vector(vertices[1]);
    //print_vector(vertices[2]);
    
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //model_view = look_at(-4, 10, 4, 4, 0, 4, 0, 1, 0);
   // projection = ortho(-5, 5, -5, 5, -1, -20);
    //model_view = scaling(0.1, 0.1, 0.1);
   
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE,
    (GLfloat *) &model_view);
    //projection = rotate_x(90);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE,
    (GLfloat *) &projection);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
        exit(0);
    
        
    
}
 



int dir_arr[1000];
int index_dir = 0;
int move_arr[1000];
int index_move = 0;
int answer_x = 7, answer_y = 7;
int facing_degree = 180;
int solution_x = 0, solution_y = 0;
int solved = 0;
void go_down(){
    if(facing_degree == 270){
       
        if ((solution_x ==  answer_x && solution_y == answer_y) && maze[solution_x][solution_y].east == 0){
           // printf("solved");
            solved = 1;
            dir_arr[index_dir] = 0;
            return;
            
    }
        if (maze[solution_x][solution_y].east == 1){
            if (maze[solution_x][solution_y].south ==0){
                solution_x = solution_x + 1;
                dir_arr[index_dir] = 1;
                //move_arr[index_move] = 2;
            }
            else{
                facing_degree = 0;
                dir_arr[index_dir] = 2;
                move_arr[index_move] = 3;
                
            }
        }else{
            facing_degree = 180;
            solution_y += 1;
            dir_arr[index_dir] = 3;
            index_dir++;
            dir_arr[index_dir] = 1;
            //move_arr[index_move] = 4;
            //index_move++;
            //move_arr[index_move] = 4;
        
            //neweye.x = neweye.x + 1;
            //look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1, 0.0);
        }
         printf("%d ",  dir_arr[index_dir]);
        index_dir++;
        printf(" %d %d\n",solution_x, solution_y);
       //  printf("print the value for the array %d %d %d\n",    result_arr[index_arr].x,result_arr[index_arr].y,result_arr[index_arr].z);
    }

};

void go_up(){
    if(facing_degree == 90){
        
        if ((solution_x ==  answer_x && solution_y == answer_y) && maze[solution_x][solution_y].east == 0){
            
           // printf("solved");
            solved = 1;
            dir_arr[index_dir] = 0;
            return;
            
    }
        if (maze[solution_x][solution_y].west == 1){
            if (maze[solution_x][solution_y].north ==0){
                solution_x = solution_x - 1;
                dir_arr[index_dir] = 1;
              
            }
            else{
                facing_degree = 180;
                dir_arr[index_dir] = 2;
            
            }
        }else{
            facing_degree = 0;
            solution_y -= 1;
            dir_arr[index_dir] = 3;
            index_dir++;
            dir_arr[index_dir] = 1;
         
        }
         printf("%d",  dir_arr[index_dir]);
        index_dir++;
        printf(" %d %d\n",solution_x, solution_y);
       //  printf("print the value for the array %d %d %d\n",    result_arr[index_arr].x,result_arr[index_arr].y,result_arr[index_arr].z);
    }
};



void go_left(){
    if(facing_degree == 0 ){
        
        if ((solution_x ==  answer_x && solution_y == answer_y) && maze[solution_x][solution_y].east == 0){
            //printf("solved");
            solved = 1;
            dir_arr[index_dir] = 0;
            return;
            
    }
        if (maze[solution_x][solution_y].south == 1){
            if (maze[solution_x][solution_y].west == 0){
                solution_y = solution_y - 1;
                    dir_arr[index_dir] = 1;
              
            }
            else{
                facing_degree = 90;
                dir_arr[index_dir] = 2;
               
            }
        }else{
            facing_degree = 270;
            solution_x += 1;
            dir_arr[index_dir] = 3;
            index_dir++;
            dir_arr[index_dir] = 1;
        
        }
         printf("%d",  dir_arr[index_dir]);
        index_dir++;
        printf(" %d %d\n",solution_x, solution_y);
        // printf("print the value for the array %d %d %d\n",    result_arr[index_arr].x,result_arr[index_arr].y,result_arr[index_arr].z);
    }
};

void go_right(){
    if(facing_degree == 180){
       
        if ((solution_x ==  answer_x && solution_y == answer_y) && maze[solution_x][solution_y].east == 0){
           // printf("solved");
            solved = 1;
             dir_arr[index_dir] = 0;
            return;
    }
        if (maze[solution_x][solution_y].north == 1){
            if (maze[solution_x][solution_y].east ==0){
                solution_y = solution_y + 1;
                 dir_arr[index_dir] = 1;
             
                
            }
            else{
                facing_degree = 270;
                 dir_arr[index_dir] = 2;
                //printf("degree1 %d\n",facing_degree);
           

                
            }
        }else{
            facing_degree = 90;
            solution_x -= 1;
            dir_arr[index_dir] = 3;
            index_dir++;
            dir_arr[index_dir] = 1;
           
            
        
            //printf("degree2 %d\n",facing_degree);

        }
        
        printf("%d",  dir_arr[index_dir]);
        index_dir++;
        printf(" %d %d\n",solution_x, solution_y);
        
        //printf("degree3 %d\n",facing_degree);
    }
};

void solve_maze(){
    while(solved == 0)
    //for(int i = 0; i < 5; i++)
    {
        go_up();
        go_down();
        go_left();
        go_right();
       
    }
    
};

int face = 4;
int move_counter = 0;
GLfloat new_value, new_value2;
GLfloat old_left = -5, old_right = 5;

GLfloat arc = 0;
vec4 neweye = {-2, 3, 2, 1};
GLfloat neweyex, neweyey, neweyez, newatx, newaty, newatz;
GLfloat old_atx = 4, old_aty = 0, old_atz = 4;




GLfloat new_neweye_x, new_neweye_y, new_neweye_z, new_new_atx, new_new_aty, new_new_atz;
void idle(){
   
    //GLfloat eyex = -4, eyey = 10, eyez = 4;

    //eyex = sin(arc * M_PI/180) * -4;
    //eyez = cos(arc * M_PI/180) * 4;
  
    if(arc < 180){
    /*mat4 tro = translate(0, 0, 0);
    mat4 rt = rotate_y(arc);
    mat4 trb = translate(4, 0, 4);
    mat4 transform = matrix_matrix_multiplication(trb, matrix_matrix_multiplication(rt, tro));
     neweye = matrix_vector_multiplication(transform, etemp);
    */
    neweye.x = sin(arc * M_PI/180) * -4;
    neweye.z = cos(arc * M_PI/180) * 4;

    model_view = look_at(neweye.x, neweye.y, neweye.z, 4, 0, 4, 0.0, 1, 0.0);
        //printf("neweye.x - flying: %f\n", neweye.x);
    projection = frustum(-5, 5, -5, 5, -1, -20);
    arc++;
    }
    else if(arc == 180){
        //printf("neweye.x: %f\n", neweye.x);
        neweyex = (-0.5 - neweye.x)/100;
        neweyey = (0 - neweye.y)/100;
        neweyez = (0.5 - neweye.z)/100;
        
        newatx = (0.5 - old_atx)/100;
        newaty = (0.0 - old_aty)/100;
        newatz = (0.5 - old_atz)/100;
        
        new_value = (0.35 - 5)/100;
        new_value2 = (-0.35 + 5)/100;
        arc++;
    }
    else if(arc > 180 && arc <= 280)
    {
        neweye.x += neweyex;
        neweye.y += neweyey;
        neweye.z += neweyez;
        
        old_atx += newatx;
        old_aty += newaty;
        old_atz += newatz;
        
        old_left += new_value2;
        old_right += new_value;
        //printf("%f\n", neweye.x);
        model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1, 0.0);
        projection = frustum(old_left, old_right, old_left, old_right, -0.35, -20);
         //projection = ortho(-5, 5, -5, 5, -1, -20);
         //glutPostRedisplay();
        arc++;
        }
    else if(arc == 281){
        neweye.x += 1;
        old_atx += 1;
        arc++;
    }
    
     else if(arc > 281 && arc <=380){
         printf(" test for move counter. %d\n", move_counter);
         printf(" test for dir_arr. %d\n", dir_arr[move_counter]);
         if(dir_arr[move_counter] == 1){
             
             printf("Face value. %d\n", face);
             if(face == 2){
                 new_new_atz = 1.0/100;
                 new_neweye_z = 1.0/100;
                 neweye.z += new_neweye_z;
                 old_atz +=  new_new_atz;
                 model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
         }
      
          
             else if(face == 1){
                 new_new_atz = -1.0/100;
                 new_neweye_z = -1.0/100;
                 neweye.z += new_neweye_z;
                 old_atz += new_neweye_z;
                 model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
            
            
        }
             else if (face == 4){
                 new_new_atx = 1.0/100;
                 new_neweye_x = 1.0/100;
                 neweye.x += new_neweye_x;
                 old_atx += new_neweye_x;
                 model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
            
        }
             else if(face == 3){
                 new_new_atx = -1.0/100;
                 new_neweye_x = -1.0/100;
                 neweye.x += new_neweye_x;
                 old_atx += new_neweye_x;
                 model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
            
        }
             
                     
                
                 
    }
         else if(dir_arr[move_counter] == 2){
             if(face == 2){
                 new_new_atz = -1.0/100;
                 new_new_atx = -1.0/100;
                 old_atz += new_new_atz;
                 old_atx += new_new_atx;
                 if(arc==380){
                            face = 3;
                                 }
                 model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
             }
             else if(face == 1){
                new_new_atz = 1.0/100;
                new_new_atx = 1.0/100;
                old_atz += new_new_atz;
                old_atx += new_new_atx;
                 if(arc==380){
                        face = 4;
                                }
                model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
                 
                 
             }
             else if(face == 4){
                        new_new_atz = 1.0/100;
                        new_new_atx = -1.0/100;
                        old_atz += new_new_atz;
                        old_atx += new_new_atx;
                        if(arc==380){
                                face = 2;
                                        }
                        model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
             
             
         }
             else if(face == 3){
                              new_new_atz = -1.0/100;
                              new_new_atx = 1.0/100;
                              old_atz += new_new_atz;
                              old_atx += new_new_atx;
                              if(arc==380){
                                    face = 1;
                                              }
                        model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
             }
         }
         else if(dir_arr[move_counter] == 3){
             if(face == 2){
                 new_new_atz = -1.0/100;
                       new_new_atx = 1.0/100;
                       old_atz += new_new_atz;
                       old_atx += new_new_atx;
                       if(arc==380){
                                face = 4;
                                       }
                 model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
                 
             }
             else if(face == 1){
                 new_new_atz = 1.0/100;
                 new_new_atx = -1.0/100;
                 old_atz += new_new_atz;
                 old_atx += new_new_atx;
                 if(arc==380){
                        face = 3;
                                 }
                model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
                 
                 
             }
             else if(face == 4){
                             new_new_atz = -1.0/100;
                             new_new_atx = -1.0/100;
                             old_atz += new_new_atz;
                             old_atx += new_new_atx;
                 if(arc==380){
                             face = 1;
                 }
                            model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
                             
                             
                         }
             
             else if(face == 3){
                            new_new_atz = 1.0/100;
                            new_new_atx = 1.0/100;
                            old_atz += new_new_atz;
                            old_atx += new_new_atx;
                            if(arc==380){
                                                        face = 2;
                                            }
                            model_view = look_at(neweye.x, neweye.y, neweye.z, old_atx, old_aty, old_atz, 0.0, 1.0, 0.0);
                                        
                                        
                                    }
             
         }
         
         arc++;
     }else{
         
         move_counter++;
         arc = 282;
         
     }
    
    printf("hello\n");
    glutPostRedisplay();
    
    
}



int main(int argc, char **argv)
{
    srand(time(NULL));
    
       dfs(0, 0);
       map();
   
    base_fun();
    world();
    pole();
    solve_maze( );
    //mat4 matrix1 = {{1,-5,9,13},{2,6,-10,14},{3,7,11,15},{4,8,12,-16}};
    //inverse_of_a_matrix(matrix1);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Texture Lab");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
    mat4 test = look_at(-4, 4, 4, 4, 0, 4, 0.0, 1, 0.0);
    printf("test for matrix.\n");
    print_matrix(test);
    
    glutMainLoop();

    return 0;
}
