//
//  main.c
//  Proj4
//
//  Created by 梁轶伦 on 12/3/19.
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

vec4 vector_vector_addition(vec4 vector1, vec4 vector2){
    vec4 result = {vector1.x + vector2.x, vector1.y +
        vector2.y, vector1.z + vector2.z, vector1.w + vector2.w};
    return result;
}

vec4 vector_vector_subtraction(vec4 vector1, vec4 vector2){
    vec4 result = {vector1.x - vector2.x, vector1.y -
        vector2.y, vector1.z - vector2.z, vector1.w - vector2.w};
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


vec4 normalize(vec4 l){
    vec4 result;
    result.x = l.x/(sqrt(pow(l.x, 2) + pow(l.y, 2) + pow(l.z, 2)));
    result.y = l.y/(sqrt(pow(l.x, 2) + pow(l.y, 2) + pow(l.z, 2)));
    result.z = l.z/(sqrt(pow(l.x, 2) + pow(l.y, 2) + pow(l.z, 2)));
    result.w = 0;
    return result;
}


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

    
    result = transpose_of_a_matrix(result);
 
    result = inverse_of_a_matrix(result);
   
    return result;
    
};

mat4 rotate_arb(float theta, float u, float v, float w)
{
    mat4 result;
    float s = (M_PI/180.0) * theta;
    float L = (u * u + v * v + w * w);
    float u2 = u * u;
    float v2 = v * v;
    float w2 = w * w;
    
    result.x.x = (u2 + (v2 + w2) * cos(s)) / L;
    result.x.y = (u * v * (1 - cos(s)) - w * sqrt(L) * sin(s)) / L;
    result.x.z = (u * w * (1 - cos(s)) + v * sqrt(L) * sin(s)) / L;
    result.x.w = 0.0;
    
    result.y.x = (u * v * (1 - cos(s)) + w * sqrt(L) * sin(s)) / L;
    result.y.y = (v2 + (u2 + w2) * cos(s)) / L;
    result.y.z = (v * w * (1 - cos(s)) - u * sqrt(L) * sin(s)) / L;
    result.y.w = 0.0;
    
    result.z.x = (u * w * (1 - cos(s)) - v * sqrt(L) * sin(s)) / L;
    result.z.y = (v * w * (1 - cos(s)) + u * sqrt(L) * sin(s)) / L;
    result.z.z = (w2 + (u2 + v2) * cos(s)) / L;
    result.z.w = 0.0;
    
    result.w.x = 0.0;
    result.w.y = 0.0;
    result.w.z = 0.0;
    result.w.w = 1.0;
    
    
    return result;
    
    
}
int control_left = 0;
int control_right = 0;
int control_down = 0;
int control_up = 0;
int control_front = 0;
int control_back =0;
int start_drag = 0;
int v_index = 0;
int x_index = 0;
vec4 vertices[132*27];
vec4 colors[132*27];
mat4 reference_location[27];

mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

mat4 cube_ctm_record[27] ={
    {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}}
     
};

mat4 cube_ctm[27] ={
    {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}},
     {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}}
     
};

mat4 cube_1_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_2_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_3_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_4_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_5_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_6_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_7_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_8_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
mat4 cube_9_ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};



vec4 sqVertices[132] =
{
{ 0.5, 0.5, 0.6, 1.0}, // Top right
{ -0.5, 0.5, 0.6, 1.0}, // Top left
{ -0.5, -0.5, 0.6, 1.0}, // Bottom left
{ 0.5, 0.5, 0.6, 1.0}, // Top right
{ -0.5, -0.5, 0.6, 1.0}, // Bottom left
{ 0.5, -0.5, 0.6, 1.0},
    
    { -0.5, 0.5, -0.6, 1.0}, // Top right
    { 0.5, 0.5, -0.6, 1.0}, // Top left
    {  0.5, -0.5, -0.6, 1.0}, // Bottom left
    { -0.5, 0.5, -0.6, 1.0}, // Top right
    { 0.5, -0.5, -0.6, 1.0}, // Bottom left
    { -0.5, -0.5, -0.6, 1.0},
    
    { 0.5, 0.6, -0.5, 1.0}, // Top right
    { -0.5, 0.6, -0.5, 1.0}, // Top left
    { -0.5, 0.6, 0.5, 1.0}, // Bottom left
    { 0.5, 0.6, -0.5, 1.0}, // Top right
    { -0.5, 0.6, 0.5, 1.0}, // Bottom left
    { 0.5, 0.6, 0.5, 1.0},
    
    { -0.5, -0.6, -0.5, 1.0}, // Top right
    { 0.5, -0.6, -0.5, 1.0}, // Top left
    { 0.5, -0.6, 0.5, 1.0}, // Bottom left
    { -0.5, -0.6, -0.5, 1.0}, // Top right
    { 0.5, -0.6, 0.5, 1.0}, // Bottom left
    { -0.5, -0.6, 0.5, 1.0},
    
    
    { 0.6, 0.5, -0.5, 1.0}, // Top right
        { 0.6, 0.5, 0.5, 1.0}, // Top left
        { 0.6, -0.5, 0.5, 1.0}, // Bottom left
        { 0.6, 0.5, -0.5, 1.0}, // Top right
        { 0.6, -0.5, 0.5, 1.0}, // Bottom left
        { 0.6, -0.5, -0.5, 1.0},

   { -0.6, 0.5, 0.5, 1.0}, // Top right
    { -0.6, 0.5, -0.5, 1.0}, // Top left
    {  -0.6, -0.5, -0.5, 1.0}, // Bottom left
    { -0.6, 0.5, 0.5, 1.0}, // Top right
    {  -0.6, -0.5, -0.5, 1.0}, // Bottom left
    { -0.6, -0.5, 0.5, 1.0},
    

    {0.5, 0.6, 0.5, 1.0},
    {-0.5, 0.6, 0.5, 1.0},
    {-0.5, 0.5, 0.6, 1.0},
     {0.5, 0.6, 0.5, 1.0},
    {-0.5, 0.5, 0.6, 1.0},
    {0.5, 0.5, 0.6, 1.0},
    
    {-0.5, 0.6, -0.5, 1.0},
       {0.5, 0.6, -0.5, 1.0},
       {0.5, 0.5, -0.6, 1.0},
        {-0.5, 0.6, -0.5, 1.0},
       {0.5, 0.5, -0.6, 1.0},
       {-0.5, 0.5, -0.6, 1.0},
    
    { 0.5, 0.6, -0.5, 1.0},
    { 0.5, 0.6, 0.5, 1.0},
    { 0.6, 0.5, 0.5, 1.0},
    { 0.5, 0.6, -0.5, 1.0},
     { 0.6, 0.5, 0.5, 1.0},
    {0.6, 0.5, -0.5, 1.0},
    
    { -0.5, 0.6, 0.5, 1.0},
    { -0.5, 0.6, -0.5, 1.0},
    { -0.6, 0.5, -0.5, 1.0},
    { -0.5, 0.6, 0.5, 1.0},
     { -0.6, 0.5, -0.5, 1.0},
    {-0.6, 0.5, 0.5, 1.0},
    
    
    {0.5, -0.5, 0.6, 1.0},
    {-0.5, -0.5, 0.6, 1.0},
    {-0.5, -0.6, 0.5, 1.0},
    {0.5, -0.5, 0.6, 1.0},
    {-0.5, -0.6, 0.5, 1.0},
    {0.5, -0.6, 0.5, 1.0},
    
    {-0.5, -0.5, -0.6, 1.0},
       {0.5, -0.5, -0.6, 1.0},
       {0.5, -0.6, -0.5, 1.0},
       {-0.5, -0.5, -0.6, 1.0},
       {0.5, -0.6, -0.5, 1.0},
       {-0.5, -0.6, -0.5, 1.0},
    
    {0.6, -0.5, -0.5, 1.0},
    {0.6, -0.5, 0.5, 1.0},
    {0.5, -0.6, 0.5, 1.0},
    {0.6, -0.5, -0.5, 1.0},
    {0.5, -0.6, 0.5, 1.0},
    {0.5, -0.6, -0.5, 1.0},
    
    
    {-0.6, -0.5, 0.5, 1.0},
    {-0.6, -0.5, -0.5, 1.0},
    {-0.5, -0.6, -0.5, 1.0},
    {-0.6, -0.5, 0.5, 1.0},
    {-0.5, -0.6, -0.5, 1.0},
    {-0.5, -0.6, 0.5, 1.0},
    
    {0.6, 0.5 , 0.5, 1.0},
    {0.5, 0.5 , 0.6, 1.0},
    {0.5, -0.5 , 0.6, 1.0},
     {0.6, 0.5 , 0.5, 1.0},
     {0.5, -0.5 , 0.6, 1.0},
    { 0.6, -0.5, 0.5, 1.0},
    
      {-0.6, 0.5 , -0.5, 1.0},
      {-0.5, 0.5 , -0.6, 1.0},
      {-0.5, -0.5 , -0.6, 1.0},
       {-0.6, 0.5 , -0.5, 1.0},
       {-0.5, -0.5 , -0.6, 1.0},
      { -0.6, -0.5, -0.5, 1.0},
    
    { -0.5, 0.5, 0.6, 1.0},
     { -0.6, 0.5, 0.5, 1.0},
    { -0.6, -0.5, 0.5, 1.0},
     { -0.5, 0.5, 0.6, 1.0},
     { -0.6, -0.5, 0.5, 1.0},
    { -0.5, -0.5, 0.6, 1.0},
    
    
    { 0.5, 0.5, -0.6, 1.0},
     { 0.6, 0.5, -0.5, 1.0},
    { 0.6, -0.5, -0.5, 1.0},
     { 0.5, 0.5, -0.6, 1.0},
     { 0.6, -0.5, -0.5, 1.0},
    { 0.5, -0.5, -0.6, 1.0},
    
    { 0.5, 0.6, 0.5, 1.0},
    { 0.5, 0.5, 0.6, 1.0},
    { 0.6, 0.5, 0.5, 1.0},
    
    { -0.5, 0.6, -0.5, 1.0},
    { -0.5, 0.5, -0.6, 1.0},
    { -0.6, 0.5, -0.5, 1.0},
    
     { -0.5, 0.6, 0.5, 1.0},
     { -0.6, 0.5, 0.5, 1.0},
     { -0.5, 0.5, 0.6, 1.0},
    
    
     { 0.5, 0.6, -0.5, 1.0},
     { 0.6, 0.5, -0.5, 1.0},
     { 0.5, 0.5, -0.6, 1.0},
    
    { 0.5, -0.6, 0.5, 1.0},
    { 0.6, -0.5, 0.5, 1.0},
    { 0.5, -0.5, 0.6, 1.0},
    
     { -0.5, -0.6, -0.5, 1.0},
     { -0.6, -0.5, -0.5, 1.0},
     { -0.5, -0.5, -0.6, 1.0},
    
    {-0.5, -0.6, 0.5, 1.0},
     {-0.5, -0.5, 0.6, 1.0},
    {-0.6, -0.5, 0.5, 1.0},
   
    
    {0.5, -0.6, -0.5, 1.0},
    {0.5, -0.5, -0.6, 1.0},
    {0.6, -0.5, -0.5, 1.0},
  
    
    
    
    
    
    
    
    
};

vec4 sqColors[132] =
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
    
       {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         {0.5, 0.5, 0.5, 1.0},
       
            {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
    {0.5, 0.5, 0.5, 1.0},
        {0.5, 0.5, 0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},
    
           {0.5, 0.5, 0.5, 1.0},
                 {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             
     {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         
          {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           
    

    {0.5, 0.5, 0.5, 1.0},
         {0.5, 0.5, 0.5, 1.0},
       {0.5, 0.5, 0.5, 1.0},
     
              {0.5, 0.5, 0.5, 1.0},
                 {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             
    
     {0.5, 0.5, 0.5, 1.0},
        {0.5, 0.5, 0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},
    
             {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
    
      {0.5, 0.5, 0.5, 1.0},
         {0.5, 0.5, 0.5, 1.0},
       {0.5, 0.5, 0.5, 1.0},
     
              {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
         {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         
    
            {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           
    
         {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         {0.5, 0.5, 0.5, 1.0},
       
              {0.5, 0.5, 0.5, 1.0},
                 {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             
    
     {0.5, 0.5, 0.5, 1.0},
            {0.5, 0.5, 0.5, 1.0},
          {0.5, 0.5, 0.5, 1.0},
        
           {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            {0.5, 0.5, 0.5, 1.0},
          
      {0.5, 0.5, 0.5, 1.0},
        {0.5, 0.5, 0.5, 1.0},
      {0.5, 0.5, 0.5, 1.0},
    
            {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           
    
         {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         
            {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
    
             {0.5, 0.5, 0.5, 1.0},
                 {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             
           {0.5, 0.5, 0.5, 1.0},
                 {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             
    
    
            {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
    
         {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         
    
           {0.5, 0.5, 0.5, 1.0},
               {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           
       
              {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
    
        {0.5, 0.5, 0.5, 1.0},
            {0.5, 0.5, 0.5, 1.0},
          {0.5, 0.5, 0.5, 1.0},
        
    
          {0.5, 0.5, 0.5, 1.0},
                {0.5, 0.5, 0.5, 1.0},
              {0.5, 0.5, 0.5, 1.0},
            
    
       {0.5, 0.5, 0.5, 1.0},
         {0.5, 0.5, 0.5, 1.0},
       {0.5, 0.5, 0.5, 1.0},
     
      
        {0.5, 0.5, 0.5, 1.0},
             {0.5, 0.5, 0.5, 1.0},
           {0.5, 0.5, 0.5, 1.0},
         
};







GLuint ctm_location;


void cube_level_1(){
    mat4 tri;
    GLfloat x = -0.12, y = -0.12, z =-0.12;
    for(int j = 0; j < 3; j ++){
        x = -0.12;
        for(int i = 0; i < 3; i ++){
            reference_location[x_index] = translate(x, y, z);
            tri = matrix_matrix_multiplication(translate(x, y, z), scaling(0.1, 0.1, 0.1));
           
            
            x_index ++;
            for(int k = 0; k < 132; k++){
                vertices[v_index] = matrix_vector_multiplication(tri, sqVertices[k]);
                colors[v_index] = sqColors[k];
                v_index++;
            }
            x+= 0.12;
        
    }
        z+= 0.12;
    }
  

}

void cube_level_2(){
    mat4 tri;
    GLfloat x = -0.12, y = 0.0, z =-0.12;
    
    for(int j = 0; j < 3; j ++){
         x = -0.12;
        for(int i = 0; i < 3; i ++){
            reference_location[x_index] = translate(x, y, z);
            tri = matrix_matrix_multiplication(translate(x, y, z), scaling(0.1, 0.1, 0.1));
            
            x_index ++;
            for(int k = 0; k < 132; k++){
                vertices[v_index] = matrix_vector_multiplication(tri, sqVertices[k]);
                colors[v_index] = sqColors[k];
                v_index++;
            }
           x+= 0.12;
        
    }
        z+= 0.12;
    }
    //print_matrix(reference_location[i]);

    
}
void cube_level_3(){
    
    mat4 tri;
    GLfloat x = -0.12, y = 0.12, z =-0.12;
    
    for(int j = 0; j < 3; j ++){
         x= -0.12;
        for(int i = 0; i < 3; i ++){
            reference_location[x_index] = translate(x, y, z);
            tri = matrix_matrix_multiplication(translate(x, y, z), scaling(0.1, 0.1, 0.1));
           
            x_index ++;
            for(int k = 0; k < 132; k++){
                vertices[v_index] = matrix_vector_multiplication(tri, sqVertices[k]);
                colors[v_index] = sqColors[k];
                v_index++;
            }
             x+= 0.12;
        
    }
        z+=0.12;
    }
   // print_matrix(reference_location[i]);

    
}

int rotate_degree_down = 1;

int rotate_times = 0;
int rotate_times_up = 0;
int ctm_down[9] = {0, 1, 2, 3, 4 ,5 ,6 ,7 ,8};
int ctm_up[9] = {18, 19, 20, 21, 22 ,23 ,24 ,25 ,26};
int ctm_left[9] = {0,3,6,9,12,15,18,21,24};
int ctm_right[9] = {2,5,8,11,14,17,20,23,26};
int ctm_front[9] = {6, 7, 8, 15, 16, 17, 24, 25, 26};
int ctm_back[9] = {0,1,2,9,10,11,18,19,20};


void up(){
    for(int i = 0; i < 9; i++){
       cube_ctm[ctm_up[i]] = matrix_matrix_multiplication(translate(0, -0.12, 0),cube_ctm[ctm_up[i]]);
        cube_ctm[ctm_up[i]] = matrix_matrix_multiplication(rotate_y(-1),cube_ctm[ctm_up[i]]);
          cube_ctm[ctm_up[i]] = matrix_matrix_multiplication(translate(0, 0.12, 0),cube_ctm[ctm_up[i]] );
    }
     rotate_times_up++;
    
    
}

void down(){
    // mat4 tri;
    //mat4 tri, tro, rt, transform, transform1;
    for(int i = 0; i < 9; i++){
     
        cube_ctm[ctm_down[i]] = matrix_matrix_multiplication(translate(0, 0.12, 0),cube_ctm[ctm_down[i]]);
        
      cube_ctm[ctm_down[i]] = matrix_matrix_multiplication(rotate_y(1),cube_ctm[ctm_down[i]]);
        
        cube_ctm[ctm_down[i]] = matrix_matrix_multiplication(translate(0, -0.12, 0), cube_ctm[ctm_down[i]]);
        //cube_ctm_record[i] = cube_ctm[i];
        
    }
    
  
    rotate_times++;
    
    
}

void front(){
    
    for(int i = 0; i < 9; i++){
        cube_ctm[ctm_front[i]] = matrix_matrix_multiplication(translate(0, 0, -0.12),cube_ctm[ctm_front[i]]);
        cube_ctm[ctm_front[i]] = matrix_matrix_multiplication(rotate_z(-1), cube_ctm[ctm_front[i]]);
        cube_ctm[ctm_front[i]] = matrix_matrix_multiplication(translate(0, 0, 0.12), cube_ctm[ctm_front[i]]);
    }
   /* for(int i = ; i < 9; i++){
       // cube_ctm_record[i] = cube_ctm[i];
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, -0.12));
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i],rotate_z(rotate_degree_down) );
         cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, 0.12));
    }
    for(int i = 15; i < 18; i++){
        //cube_ctm_record[i] = cube_ctm[i];
       cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, -0.12));
           cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], rotate_z(rotate_degree_down));
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, 0.12));
       }
    for(int i = 24; i < 27; i++){
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, -0.12));
           cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], rotate_z(rotate_degree_down));
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, 0.12));
       }*/
    
    rotate_times++;
    
}

void back(){
    for(int i = 0; i < 9; i++){
           cube_ctm[ctm_back[i]] = matrix_matrix_multiplication(translate(0, 0, 0.12),cube_ctm[ctm_back[i]]);
           cube_ctm[ctm_back[i]] = matrix_matrix_multiplication(rotate_z(1), cube_ctm[ctm_back[i]]);
           cube_ctm[ctm_back[i]] = matrix_matrix_multiplication(translate(0, 0, -0.12), cube_ctm[ctm_back[i]]);
       }
    
    /*for(int i = 0; i < 3; i++){
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, 0.12));
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i],rotate_z(-1) );
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, -0.12));
    }
    for(int i = 9; i <12; i++){
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, 0.12));
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i],rotate_z(-1) );
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, -0.12));
        }
    for(int i = 18; i < 21; i++){
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, 0.12));
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i],rotate_z(-1) );
        cube_ctm[i] = matrix_matrix_multiplication(cube_ctm[i], translate(0, 0, -0.12));
    }*/
    rotate_times++;
}

void left(){
    for (int i =0;i<9; i++){
        cube_ctm[ctm_left[i]] = matrix_matrix_multiplication(translate(0.12, 0, 0),cube_ctm[ctm_left[i]]);
        cube_ctm[ctm_left[i]] = matrix_matrix_multiplication(rotate_x(1), cube_ctm[ctm_left[i]]);
        cube_ctm[ctm_left[i]] = matrix_matrix_multiplication(translate(-0.12, 0, 0), cube_ctm[ctm_left[i]]);
    }
     rotate_times++;
}
void right(){
    for (int i =0;i<9; i++){
           cube_ctm[ctm_right[i]] = matrix_matrix_multiplication(translate(-0.12, 0, 0),cube_ctm[ctm_right[i]]);
           cube_ctm[ctm_right[i]] = matrix_matrix_multiplication(rotate_x(-1), cube_ctm[ctm_right[i]]);
           cube_ctm[ctm_right[i]] = matrix_matrix_multiplication(translate(0.12, 0, 0), cube_ctm[ctm_right[i]]);
       }
        rotate_times++;
    
    
}

int shuffle_count = 0;
int shuffle_control = 0;
void shuffle(){
    shuffle_control = 1;
    int num;
    for (int i = 0; i < 1; i++) {
         num = (rand() % (10 - 1 + 1)) + 1;
    }
    
    shuffle_count= num;
    
}


void init(void)
{
    //time_t t;
     //srand((unsigned)time(&t));
     //cone();
    cube_level_1();
    cube_level_2();
    cube_level_3();
    GLuint program = initShader("/Users/liangyilun/Desktop/cs1566/Proj4/Proj4/vshader.glsl", "/Users/liangyilun/Desktop/cs1566/Proj4/Proj4/fshader.glsl");
    glUseProgram(program);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));

    ctm_location = glGetUniformLocation(program, "ctm");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

   // glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    //glDrawArrays(GL_TRIANGLES, 0, 132*27);
    
    for(int i = 0; i < 27; i++){
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_ctm[i]);
          glDrawArrays(GL_TRIANGLES, 132*i, 132);
    }
    /*glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_1_ctm);
     glDrawArrays(GL_TRIANGLES, 0, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_2_ctm);
    glDrawArrays(GL_TRIANGLES, 132, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_3_ctm);
    glDrawArrays(GL_TRIANGLES, 132*29, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_4_ctm);
    glDrawArrays(GL_TRIANGLES, 132*30, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_5_ctm);
    glDrawArrays(GL_TRIANGLES, 132*31, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_6_ctm);
    glDrawArrays(GL_TRIANGLES, 132*32, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_7_ctm);
    glDrawArrays(GL_TRIANGLES, 132*33, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_8_ctm);
    glDrawArrays(GL_TRIANGLES, 132*34, 132);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &cube_9_ctm);
    glDrawArrays(GL_TRIANGLES, 132*35, 132);*/

    glutSwapBuffers();
}

vec4 degree_calculator(vec4 ori, vec4 fin){
    vec4 result;
    // dot product for Z: (ori.x * fin.x) + (ori.y * fin.y)
    result.x = acos(((ori.z * fin.z) + (ori.y * fin.y))/sqrt(pow(ori.z,2) * pow(fin.z,2)) + sqrt(pow(ori.y,2) * pow(fin.y,2)));
    result.y = acos(((ori.z * fin.z) + (ori.x * fin.x))/sqrt(pow(ori.z,2) * pow(fin.z,2)) + sqrt(pow(ori.x,2) * pow(fin.x,2)));
    result.z = acos(((ori.x * fin.x) + (ori.y * fin.y))/sqrt(pow(ori.x,2) * pow(fin.x,2)) + sqrt(pow(ori.y,2) * pow(fin.y,2)));
    
    return result;

}

GLfloat vector_mag ( vec4 vector){
    return sqrt(pow(vector.x, 2) + pow(vector.y,2) + pow(vector.z,2));
}


void motion(int x, int y){
    
    vec4 ori;
    vec4 fin;
    GLfloat new_x = (x-256.0)/256.0;
    GLfloat new_y = (y-256.0)/256.0;
    GLfloat new_z = 1 - (pow(new_x,2) + pow(new_y,2));
    fin.x = new_x;
    fin.y = new_y;
    fin.z = new_z;

    if(start_drag){
    
        
        start_drag = 0;
        
       
    }else{
        vec4 cross = cross_product(ori, fin);
        GLfloat combine = dot_product(ori, fin);
        GLfloat degree = acos(combine/(vector_mag(ori) * vector_mag(fin)));
        if(isnan(degree)){
            degree = 0;
        }
        mat4 matrix_arb = rotate_arb(degree, cross.x, cross.y, cross.z);
       //result = degree_calculator(ori, fin);
        for(int i = 0; i <27; i++){
        cube_ctm[i]= matrix_matrix_multiplication(cube_ctm[i], matrix_arb);
        }
        
    }
    fin = ori;
    glutPostRedisplay();
    
    
}

void mouse(int button, int state, int x, int y ){
    

    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        start_drag = 1;
        
        
    }
        

    // Store the currentPoint in the points vector when left button is released.
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        start_drag = 0;
        
    }
      
    
    
    
}



void keyboard(unsigned char key, int mousex, int mousey)
{
    printf("key: %i\n", key);
    if(key == 'q')
        exit(0);

        
    else if(key == 'i'){
        ctm = matrix_matrix_multiplication( ctm, scaling(0.98, 0.98, 0.98));
        
    }
    else if(key == 'o'){
        ctm = matrix_matrix_multiplication(ctm, scaling(1.02, 1.02, 1.02));
    }
    
    /*else if (key == 'r'){
        for(int i = 0; i <27; i++){
            cube_ctm[i]= matrix_matrix_multiplication(cube_ctm[i], rotate_y(5));
        }
    }*/
    
    /*else if(key == 'f'){
        for(int i = 0; i <27; i++){
            cube_ctm[i]= matrix_matrix_multiplication(cube_ctm[i], rotate_x(5));
        }
        
    }*/
    else if(key == 'd'){
        control_down = 1;
        /*for(int i = 0 ; i < 9; i ++){
        cube_ctm[i] = matrix_matrix_multiplication(rotate_y(9.0), cube_ctm[i]);*/
        
    }
    else if(key == 'u'){
        control_up = 1;
    }
    
    else if(key == 'f'){
           control_front = 1;
       }
    else if(key == 'b'){
        control_back = 1;
    }
    else if (key =='l'){
        control_left = 1;
    }
    else if (key =='r'){
           control_right = 1;
       }
    
    else if(key == 's'){
        //control_up = 1;
        //control_front = 1;
        shuffle();
    }
    //else if (key == 'u'){
      //  ctm = matrix_matrix_multiplication(rotate_x(9.0), ctm);
    //}
    glutPostRedisplay();
}

void swap(mat4 m1, mat4 m2){
    mat4 temp;
    temp = m1;
    m1 = m2;
    m2 = temp;
}

int t0, t1, t2, t3, t4, t5, t6, t7, t8;

void idle(void)
{
    if(shuffle_control == 1){
        if(rotate_times <= 90){
            down();
        }
        if(rotate_times > 90 && rotate_times <= 180){
            up();
        }
        if (rotate_times == 180){
            rotate_times = 0;
            control_down = 0;
        }
    }
    if(control_down == 1 ){
        if(rotate_times <= 90){
            down();
        }
        if(rotate_times == 90){
            t0 = ctm_down[0];
            t1 = ctm_down[1];
            t2 = ctm_down[2];
            t3 = ctm_down[3];
            t4 = ctm_down[4];
            t5 = ctm_down[5];
            t6 = ctm_down[6];
            t7 = ctm_down[7];
            t8 = ctm_down[8];
            ctm_down[0] = t2;
            ctm_down[1] = t5;
            ctm_down[2] = t8;
            ctm_down[3] = t1;
            ctm_down[4] = t4;
            ctm_down[5] = t7;
            ctm_down[6] = t0;
            ctm_down[7] = t3;
            ctm_down[8] = t6;
            ctm_front[0] = ctm_down[6];
            ctm_front[1] = ctm_down[7];
            ctm_front[2] = ctm_down[8];
            ctm_back[0] = ctm_down[0];
            ctm_back[1] = ctm_down[1];
            ctm_back[2] = ctm_down[2];
            ctm_right[0] = ctm_down[2];
            ctm_right[1] = ctm_down[5];
            ctm_right[2] = ctm_down[8];
            ctm_left[0] = ctm_down[0];
            ctm_left[1] = ctm_down[3];
            ctm_left[2]=  ctm_down[6];
            //ctm_front
            rotate_times = 0;
            control_down = 0;
            
        }
       //control_down = 0;
       //rotate_times = 0;
    }
    
    if(control_up == 1 ){
        if(rotate_times_up <= 90){
            up();
        }
        if(rotate_times_up == 90){
        // 18, 19, 20, 21, 22 ,23 ,24 ,25 ,26
            t0 = ctm_up[0];
            t1 = ctm_up[1];
            t2 = ctm_up[2];
                       t3 = ctm_up[3];
                       t4 = ctm_up[4];
                       t5 = ctm_up[5];
                       t6 = ctm_up[6];
                       t7 = ctm_up[7];
                       t8 = ctm_up[8];
                       ctm_up[0] = t6;
                       ctm_up[1] = t3;
                       ctm_up[2] = t0;
                       ctm_up[3] = t7;
                       ctm_up[4] = t4;
                       ctm_up[5] = t1;
                       ctm_up[6] = t8;
                       ctm_up[7] = t5;
                       ctm_up[8] = t2;
                       ctm_front[6] = ctm_up[6];
                       ctm_front[7] = ctm_up[7];
                       ctm_front[8] = ctm_up[8];
                       ctm_back[6] = ctm_up[0];
                       ctm_back[7] = ctm_up[1];
                       ctm_back[8] = ctm_up[2];
                        ctm_right[0] = ctm_left[6];
                      ctm_right[3] = ctm_left[6];
                      ctm_right[6] = ctm_left[6];
                      ctm_left[0] = ctm_left[6];
                      ctm_left[3] =ctm_left[6];
                      ctm_left[6]= ctm_left[6];
                       //ctm_front
                       rotate_times = 0;
                       control_down = 0;
            
            
            
            rotate_times_up = 0;
            control_up = 0;
        }
       //control_down = 0;
       //rotate_times = 0;
    }
    
    if(control_front == 1 ){
           if(rotate_times <= 90){
               front();
           }
           if(rotate_times == 90){
               t0 = ctm_front[0];
                t1 = ctm_front[1];
                t2 = ctm_front[2];
                t3 = ctm_front[3];
                t4 = ctm_front[4];
                t5 = ctm_front[5];
                t6 = ctm_front[6];
                        t7 = ctm_front[7];
                        t8 = ctm_front[8];
                        ctm_front[0] = t2;
                        ctm_front[1] = t5;
                        ctm_front[2] = t8;
                        ctm_front[3] = t1;
                        ctm_front[4] = t4;
                        ctm_front[5] = t7;
                        ctm_front[6] = t0;
                        ctm_front[7] = t3;
                        ctm_front[8] = t6;
                        ctm_down[6] = ctm_front[0];
                        ctm_down[7] = ctm_front[1];
                        ctm_down[8] = ctm_front[2];
                        ctm_up[6] = ctm_front[6];
                        ctm_up[7] = ctm_front[7];
                        ctm_up[8] = ctm_front[8];
                    ctm_right[0] = ctm_left[6];
                         ctm_right[3] = ctm_left[6];
                         ctm_right[6] = ctm_left[6];
                         ctm_left[0] = ctm_left[6];
                         ctm_left[3] =ctm_left[6];
                         ctm_left[6]= ctm_left[6];
               rotate_times = 0;
               control_front = 0;
           }
          //control_down = 0;
          //rotate_times = 0;
       }
    
    if(control_back == 1){
        if(rotate_times <= 90){
            back();
        }
        if(rotate_times == 90){
            t0 = ctm_back[0];
             t1 = ctm_back[1];
             t2 = ctm_back[2];
             t3 = ctm_back[3];
             t4 = ctm_back[4];
             t5 = ctm_back[5];
             t6 = ctm_back[6];
                     t7 = ctm_back[7];
                     t8 = ctm_back[8];
                     ctm_back[0] = t6;
                    ctm_back[1] = t3;
                     ctm_back[2] = t0;
                    ctm_back[3] = t7;
                     ctm_back[4] = t4;
                     ctm_back[5] = t1;
                     ctm_back[6] = t8;
                     ctm_back[7] = t5;
                     ctm_back[8] = t1;
                     ctm_down[6] = ctm_back[0];
                     ctm_down[7] = ctm_back[1];
                     ctm_down[8] = ctm_back[2];
                     ctm_up[6] = ctm_back[6];
                     ctm_up[7] = ctm_back[7];
                     ctm_up[8] = ctm_back[8];
            ctm_right[0] = ctm_left[6];
                      ctm_right[3] = ctm_left[6];
                      ctm_right[6] = ctm_left[6];
                      ctm_left[0] = ctm_left[6];
                      ctm_left[3] =ctm_left[6];
                      ctm_left[6]= ctm_left[6];
            rotate_times = 0;
            control_back =0;
         
       
        }
    }
    
    if(control_left == 1){
        if(rotate_times <= 90){
            left();
        }
        if(rotate_times == 90){
            t0 = ctm_left[0];
             t1 = ctm_left[1];
             t2 = ctm_left[2];
             t3 = ctm_left[3];
             t4 = ctm_left[4];
             t5 = ctm_left[5];
             t6 = ctm_left[6];
                     t7 = ctm_left[7];
                     t8 = ctm_left[8];
                     ctm_left[0] = t2;
                    ctm_left[1] = t5;
                     ctm_left[2] = t8;
                    ctm_left[3] = t1;
                     ctm_left[4] = t4;
                     ctm_left[5] = t7;
                     ctm_left[6] = t0;
                     ctm_left[7] = t3;
                     ctm_left[8] = t6;
                     ctm_down[0] = ctm_left[0];
                     ctm_down[3] = ctm_left[1];
                     ctm_down[6] = ctm_left[2];
                     ctm_up[0] = ctm_left[6];
                     ctm_up[3] = ctm_left[7];
                     ctm_up[6] = ctm_left[8];
            ctm_front[0] =ctm_left[2];
            ctm_front[3] = ctm_left[5];
            ctm_front[6] = ctm_left[8];
            ctm_back[0] = ctm_left[0];
            ctm_back[3] = ctm_left[3];
            ctm_back[6] = ctm_left[6];
            ctm_right[0] = ctm_left[6];
            ctm_right[3] = ctm_left[6];
            ctm_right[6] = ctm_left[6];
            ctm_left[0] = ctm_left[6];
            ctm_left[3] =ctm_left[6];
            ctm_left[6]= ctm_left[6];
            rotate_times = 0;
            control_left = 0;
           
        }
    }
    if(control_right == 1){
        if(rotate_times <= 90){
            right();
        }
        if(rotate_times == 90){
            t0 = ctm_right[0];
             t1 = ctm_right[1];
             t2 = ctm_right[2];
             t3 = ctm_right[3];
             t4 = ctm_right[4];
             t5 = ctm_right[5];
             t6 = ctm_right[6];
                     t7 = ctm_right[7];
                     t8 = ctm_right[8];
                     ctm_right[0] = t6;
                    ctm_right[1] = t3;
                     ctm_right[2] = t0;
                    ctm_right[3] = t7;
                     ctm_right[4] = t4;
                     ctm_right[5] = t1;
                     ctm_right[6] = t8;
                     ctm_right[7] = t5;
                     ctm_back[8] = t1;
                     ctm_down[2] = ctm_right[0];
                     ctm_down[5] = ctm_right[1];
                     ctm_down[8] = ctm_right[2];
                     ctm_up[2] = ctm_right[6];
                     ctm_up[5] = ctm_right[7];
                     ctm_up[8] = ctm_right[8];
            ctm_front[2] =ctm_right[2];
            ctm_front[5] =ctm_right[5];
            ctm_front[8] =ctm_right[8];
            ctm_back[2] =ctm_right[2];
            ctm_back[5] =ctm_right[5];
            ctm_back[8] =ctm_right[8];
            rotate_times = 0;
            
            control_right = 0;
        
        }
    }
    
    //ctm = translate(x_value, 0.0, 0.0);

    glutPostRedisplay();
}




int main(int argc, char **argv)
{
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Torus with CTM");
    //glewInit();
    init();
   /* for(int i = 0 ; i < 27; i++){
           print_matrix(reference_location[i]);
    }*/
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}

