//
//  main.c
//  proj3
//
//  Created by 梁轶伦 on 11/13/19.
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
#include <string.h>

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

/*GLfloat determinant(mat4 matrix){
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
*/

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

/*mat4 cofactor(mat4 matrix){
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


}*/



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







//L


void vec_print(vec4 v){
    printf("[%.2f, %.2f, %.2f, %.2f]\n", v.x, v.y, v.z, v.w);
}

vec4 scalar_vec_mult(vec4 v, float s){

    vec4 result;
    
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    result.w = v.w * s;
    
    return result;
}

vec4 add_vec(vec4 v1, vec4 v2){
    
    vec4 result;
    
    result.x = (v1.x + v2.x);
    result.y = (v1.y + v2.y);
    result.z = (v1.z + v2.z);
    result.w = (v1.w + v2.w);
    
    return result;
}

/*
    given two vectors v1, v2, subtract vector 2 from vector 1
    return the result vector
*/
vec4 sub_vec(vec4 v1, vec4 v2){
    
    vec4 result;
    
    result.x = (v1.x - v2.x);
    result.y = (v1.y - v2.y);
    result.z = (v1.z - v2.z);
    result.w = (v1.w - v2.w);
    
    return result;
}

/*
    do the dot product operation of two vectors and return the
    resulting scalar
*/
float dot_product_1(vec4 v1, vec4 v2){
    
    float result;
    result = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w));
    return result;
}

/*
    given two vectors v1, v2 calculate the cross product for the first 3
    elements in the vector (x, y, z) and ignore the last (w)
    return the resulting vector with the 4th element equal to 0
*/
vec4 cross_product_1(vec4 v1, vec4 v2){
    vec4 result;
    
    result.x = (v1.y * v2.z) - (v1.z * v2.y);
    result.y = (v1.z * v2.x) - (v1.x * v2.z);
    result.z = (v1.x * v2.y) - (v1.y * v2.x);
    result.w = 0;
    
    return result;
}

float magnitude(vec4 v){
    float result;
    result = sqrt((pow(v.x, 2)) + (pow(v.y, 2)) + (pow(v.z, 2)) + (pow(v.w, 2)));
    return result;
}

vec4 normalize(vec4 v){
    vec4 result;
    result = scalar_vec_mult(v, (1/magnitude(v)));
    return result;
}

int vector_compare(vec4 v1, vec4 v2){
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z){
        return 1;
    } else {
        return 0;
    }
}


/************************** MATRIX OPERATIONS ********************************/

/*
    print the matrix to the consol for debugging purposes
*/
void matrix_print(mat4 m){
    
    printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.x, m.y.x, m.z.x, m.w.x);
    printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.y, m.y.y, m.z.y, m.w.y);
    printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.z, m.y.z, m.z.z, m.w.z);
    printf("[%.2f, %.2f, %.2f, %.2f]\n", m.x.w, m.y.w, m.z.w, m.w.w);
}

/*
    scalar matrix multiplication, result should be a a matrix
*/
mat4 scalar_x_matrix(mat4 m, float s){
    mat4 result;
    
    //first row
    result.x.x = s * m.x.x;
    result.y.x = s * m.y.x;
    result.z.x = s * m.z.x;
    result.w.x = s * m.w.x;
    
    //second row
    result.x.y = s * m.x.y;
    result.y.y = s * m.y.y;
    result.z.y = s * m.z.y;
    result.w.y = s * m.w.y;
    
    //third row
    result.x.z = s * m.x.z;
    result.y.z = s * m.y.z;
    result.z.z = s * m.z.z;
    result.w.z = s * m.w.z;
    
    //fourth row
    result.x.w = s * m.x.w;
    result.y.w = s * m.y.w;
    result.z.w = s * m.z.w;
    result.w.w = s * m.w.w;
    
    return result;
}

/*
    given two matrices add them together and return the result matrix
*/
mat4 add_matrix(mat4 m1, mat4 m2){
    
    //row 1
    m1.x.x += m2.x.x;
    m1.y.x += m2.y.x;
    m1.z.x += m2.z.x;
    m1.w.x += m2.w.x;
    
    //row 2
    m1.x.y += m2.x.y;
    m1.y.y += m2.y.y;
    m1.z.y += m2.z.y;
    m1.w.y += m2.w.y;

    //row 3
    m1.x.z += m2.x.z;
    m1.y.z += m2.y.z;
    m1.z.z += m2.z.z;
    m1.w.z += m2.w.z;
    
    //row 4
    m1.x.w += m2.x.w;
    m1.y.w += m2.y.w;
    m1.z.w += m2.z.w;
    m1.w.w += m2.w.w;
    
    return m1;
}

/*
    given two matrices, subtract matrix 2 from matrix 1, return the
    result
*/
mat4 sub_matrix(mat4 m1, mat4 m2){
    
    //row 1
    m1.x.x -= m2.x.x;
    m1.y.x -= m2.y.x;
    m1.z.x -= m2.z.x;
    m1.w.x -= m2.w.x;
    
    //row 2
    m1.x.y -= m2.x.y;
    m1.y.y -= m2.y.y;
    m1.z.y -= m2.z.y;
    m1.w.y -= m2.w.y;

    //row 3
    m1.x.z -= m2.x.z;
    m1.y.z -= m2.y.z;
    m1.z.z -= m2.z.z;
    m1.w.z -= m2.w.z;
    
    //row 4
    m1.x.w -= m2.x.w;
    m1.y.w -= m2.y.w;
    m1.z.w -= m2.z.w;
    m1.w.w -= m2.w.w;
    
    return m1;
}

/*
    given two matrices multiply them
*/
mat4 mult_matrix(mat4 m1, mat4 m2){
    
    mat4 result;
    
    //row 1
    result.x.x = (m2.x.x * m1.x.x) + (m2.x.y * m1.y.x) + (m2.x.z * m1.z.x) + (m2.x.w * m1.w.x);
    result.y.x = (m2.y.x * m1.x.x) + (m2.y.y * m1.y.x) + (m2.y.z * m1.z.x) + (m2.y.w * m1.w.x);
    result.z.x = (m2.z.x * m1.x.x) + (m2.z.y * m1.y.x) + (m2.z.z * m1.z.x) + (m2.z.w * m1.w.x);
    result.w.x = (m2.w.x * m1.x.x) + (m2.w.y * m1.y.x) + (m2.w.z * m1.z.x) + (m2.w.w * m1.w.x);
    
    //row 2
    result.x.y = (m2.x.x * m1.x.y) + (m2.x.y * m1.y.y) + (m2.x.z * m1.z.y) + (m2.x.w * m1.w.y);
    result.y.y = (m2.y.x * m1.x.y) + (m2.y.y * m1.y.y) + (m2.y.z * m1.z.y) + (m2.y.w * m1.w.y);
    result.z.y = (m2.z.x * m1.x.y) + (m2.z.y * m1.y.y) + (m2.z.z * m1.z.y) + (m2.z.w * m1.w.y);
    result.w.y = (m2.w.x * m1.x.y) + (m2.w.y * m1.y.y) + (m2.w.z * m1.z.y) + (m2.w.w * m1.w.y);

    //row 3
    result.x.z = (m2.x.x * m1.x.z) + (m2.x.y * m1.y.z) + (m2.x.z * m1.z.z) + (m2.x.w * m1.w.z);
    result.y.z = (m2.y.x * m1.x.z) + (m2.y.y * m1.y.z) + (m2.y.z * m1.z.z) + (m2.y.w * m1.w.z);
    result.z.z = (m2.z.x * m1.x.z) + (m2.z.y * m1.y.z) + (m2.z.z * m1.z.z) + (m2.z.w * m1.w.z);
    result.w.z = (m2.w.x * m1.x.z) + (m2.w.y * m1.y.z) + (m2.w.z * m1.z.z) + (m2.w.w * m1.w.z);

    //row 4
    result.x.w = (m2.x.x * m1.x.w) + (m2.x.y * m1.y.w) + (m2.x.z * m1.z.w) + (m2.x.w * m1.w.w);
    result.y.w = (m2.y.x * m1.x.w) + (m2.y.y * m1.y.w) + (m2.y.z * m1.z.w) + (m2.y.w * m1.w.w);
    result.z.w = (m2.z.x * m1.x.w) + (m2.z.y * m1.y.w) + (m2.z.z * m1.z.w) + (m2.z.w * m1.w.w);
    result.w.w = (m2.w.x * m1.x.w) + (m2.w.y * m1.y.w) + (m2.w.z * m1.z.w) + (m2.w.w * m1.w.w);
    
    return result;
}

/*
    finding the matrix of minors
*/
mat4 m_minor(mat4 m){
    
    mat4 result;
    
    //top column
    result.x.x = (m.y.y * m.z.z * m.w.w) + (m.y.z * m.z.w * m.w.y) + (m.y.w * m.z.y * m.w.z) - (m.y.w * m.z.z * m.w.y) - (m.y.z * m.z.y * m.w.w) - (m.y.y * m.z.w * m.w.z);
    result.x.y = (m.y.x * m.z.z * m.w.w) + (m.y.z * m.z.w * m.w.x) + (m.y.w * m.z.x * m.w.z) - (m.y.w * m.z.z * m.w.x) - (m.y.z * m.z.x * m.w.w) - (m.y.x * m.z.w * m.w.z);
    result.x.z = (m.y.x * m.z.y * m.w.w) + (m.y.y * m.z.w * m.w.x) + (m.y.w * m.z.x * m.w.y) - (m.y.w * m.z.y * m.w.x) - (m.y.y * m.z.x * m.w.w) - (m.y.x * m.z.w * m.w.y);
    result.x.w = (m.y.x * m.z.y * m.w.z) + (m.y.y * m.z.z * m.w.x) + (m.y.z * m.z.x * m.w.y) - (m.y.z * m.z.y * m.w.x) - (m.y.y * m.z.x * m.w.z) - (m.y.x * m.z.z * m.w.y);

    //second column
    result.y.x = (m.x.y * m.z.z * m.w.w) + (m.x.z * m.z.w * m.w.y) + (m.x.w * m.z.y * m.w.z) - (m.x.w * m.z.z * m.w.y) - (m.x.z * m.z.y * m.w.w) - (m.x.y * m.z.w * m.w.z);
    result.y.y = (m.x.x * m.z.z * m.w.w) + (m.x.z * m.z.w * m.w.x) + (m.x.w * m.z.x * m.w.z) - (m.x.w * m.z.z * m.w.x) - (m.x.z * m.z.x * m.w.w) - (m.x.x * m.z.w * m.w.z);
    result.y.z = (m.x.x * m.z.y * m.w.w) + (m.x.y * m.z.w * m.w.x) + (m.x.w * m.z.x * m.w.y) - (m.x.w * m.z.y * m.w.x) - (m.x.y * m.z.x * m.w.w) - (m.x.x * m.z.w * m.w.y);
    result.y.w = (m.x.x * m.z.y * m.w.z) + (m.x.y * m.z.z * m.w.x) + (m.x.z * m.z.x * m.w.y) - (m.x.z * m.z.y * m.w.x) - (m.x.y * m.z.x * m.w.z) - (m.x.x * m.z.z * m.w.y);
    
    //third column
    result.z.x = (m.x.y * m.y.z * m.w.w) + (m.x.z * m.y.w * m.w.y) + (m.x.w * m.y.y * m.w.z) - (m.x.w * m.y.z * m.w.y) - (m.x.z * m.y.y * m.w.w) - (m.x.y * m.y.w * m.w.z);
    result.z.y = (m.x.x * m.y.z * m.w.w) + (m.x.z * m.y.w * m.w.x) + (m.x.w * m.y.x * m.w.z) - (m.x.w * m.y.z * m.w.x) - (m.x.z * m.y.x * m.w.w) - (m.x.x * m.y.w * m.w.z);
    result.z.z = (m.x.x * m.y.y * m.w.w) + (m.x.y * m.y.w * m.w.x) + (m.x.w * m.y.x * m.w.y) - (m.x.w * m.y.y * m.w.x) - (m.x.y * m.y.x * m.w.w) - (m.x.x * m.y.w * m.w.y);
    result.z.w = (m.x.x * m.y.y * m.w.z) + (m.x.y * m.y.z * m.w.x) + (m.x.z * m.y.x * m.w.y) - (m.x.z * m.y.y * m.w.x) - (m.x.y * m.y.x * m.w.z) - (m.x.x * m.y.z * m.w.y);
    
    //fourth column
    result.w.x = (m.x.y * m.y.z * m.z.w) + (m.x.z * m.y.w * m.z.y) + (m.x.w * m.y.y * m.z.z) - (m.x.w * m.y.z * m.z.y) - (m.x.z * m.y.y * m.z.w) - (m.x.y * m.y.w * m.z.z);
    result.w.y = (m.x.x * m.y.z * m.z.w) + (m.x.z * m.y.w * m.z.x) + (m.x.w * m.y.x * m.z.z) - (m.x.w * m.y.z * m.z.x) - (m.x.z * m.y.x * m.z.w) - (m.x.x * m.y.w * m.z.z);
    result.w.z = (m.x.x * m.y.y * m.z.w) + (m.x.y * m.y.w * m.z.x) + (m.x.w * m.y.x * m.z.y) - (m.x.w * m.y.y * m.z.x) - (m.x.y * m.y.x * m.z.w) - (m.x.x * m.y.w * m.z.y);
    result.w.w = (m.x.x * m.y.y * m.z.z) + (m.x.y * m.y.z * m.z.x) + (m.x.z * m.y.x * m.z.y) - (m.x.z * m.y.y * m.z.x) - (m.x.y * m.y.x * m.z.z) - (m.x.x * m.y.z * m.z.y);

    return result;
}

/*
    turning matrix of minor into matrix of cofactor
*/
mat4 cofactor(mat4 m){
    
    mat4 result;
    
    result.x.x = m.x.x;
    result.x.y = (-1) * m.x.y;
    result.x.z = m.x.z;
    result.x.w = (-1) * m.x.w;
    
    result.y.x = (-1) * m.y.x;
    result.y.y = m.y.y;
    result.y.z = (-1) * m.y.z;
    result.y.w = m.y.w;
    
    result.z.x = m.z.x;
    result.z.y = (-1) * m.z.y;
    result.z.z = m.z.z;
    result.z.w = (-1) * m.z.w;
    
    result.w.x = (-1) * m.w.x;
    result.w.y = m.w.y;
    result.w.z = (-1) * m.w.z;
    result.w.w = m.w.w;
    
    return result;
}

/*
    determinant of a matrix
*/
float determinant(mat4 m, mat4 cofactor){
    
    float result, a, b, c, d;
    
    a = m.x.x * cofactor.x.x;
    b = m.x.y * cofactor.x.y;
    c = m.x.z * cofactor.x.z;
    d = m.x.w * cofactor.x.w;
    
    result = a + b + c + d;
    return result;
}

/*
    produce the transpose of the given matrix
*/
mat4 transpose(mat4 m){
    
    mat4 result;
    
    result.x.x = m.x.x;
    result.x.y = m.y.x;
    result.x.z = m.z.x;
    result.x.w = m.w.x;
    
    result.y.x = m.x.y;
    result.y.y = m.y.y;
    result.y.z = m.z.y;
    result.y.w = m.w.y;
    
    result.z.x = m.x.z;
    result.z.y = m.y.z;
    result.z.z = m.z.z;
    result.z.w = m.w.z;
    
    result.w.x = m.x.w;
    result.w.y = m.y.w;
    result.w.z = m.z.w;
    result.w.w = m.w.w;
    
    return result;
}

/*
    find the inverse of the given matrix
*/
mat4 inverse(mat4 m){
    mat4 minor = m_minor(m);
    mat4 cof = cofactor(minor);
    mat4 trans = transpose(cof);
    float det = determinant(m, cof);
    mat4 result = scalar_x_matrix(trans, (1/det));
    return result;
}

/*
    given a matrix m and a vector v multiply the two together
    and return the resulting vector
*/
vec4 matrix_x_vector(mat4 m, vec4 v){
    
    vec4 result;
    
    result.x = ((m.x.x * v.x) + (m.y.x * v.y) + (m.z.x * v.z) + (m.w.x * v.w));
    result.y = ((m.x.y * v.x) + (m.y.y * v.y) + (m.z.y * v.z) + (m.w.y * v.w));
    result.z = ((m.x.z * v.x) + (m.y.z * v.y) + (m.z.z * v.z) + (m.w.z * v.w));
    result.w = ((m.x.w * v.x) + (m.y.w * v.y) + (m.z.w * v.z) + (m.w.w * v.w));
    
    return result;
}



mat4 identity(){
    mat4 result = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    return result;
}
mat4 translate(float dx, float dy, float dz){
    
    mat4 translation = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {dx,dy,dz,1}};
    return translation;
}

//rotating about the x, y or z axis
mat4 rotatez(float angle){
    
    float rad = (M_PI / 180)*angle;
    mat4 result;
    
    result.x.x = cos(rad);    result.y.x = (-1)*sin(rad);    result.z.x = 0;    result.w.x = 0;
    result.x.y = sin(rad);    result.y.y = cos(rad);        result.z.y = 0;    result.w.x = 0;
    result.x.z = 0;            result.y.z = 0;                result.z.z = 1;    result.w.z = 0;
    result.x.w = 0;            result.y.w = 0;                result.z.w = 0;    result.w.w = 1;
    
    return result;
}
mat4 rotatex(float angle){
    
    float rad = (M_PI / 180)*angle;
    mat4 result;
    
    result.x.x = 1;            result.y.x = 0;                result.z.x = 0;            result.w.x = 0;
    result.x.y = 0;            result.y.y = cos(rad);        result.z.y = (-1)*sin(rad);    result.w.x = 0;
    result.x.z = 0;            result.y.z = sin(rad);        result.z.z = cos(rad);    result.w.z = 0;
    result.x.w = 0;            result.y.w = 0;                result.z.w = 0;            result.w.w = 1;
    
    return result;
}
mat4 rotatey(float angle){
    
    float rad = (M_PI / 180)*angle;
    mat4 result;
    
    result.x.x = cos(rad);    result.y.x = 0;            result.z.x = sin(rad);        result.w.x = 0;
    result.x.y = 0;            result.y.y = 1;            result.z.y = 0;                result.w.x = 0;
    result.x.z = (-1)*sin(rad);    result.y.z = 0;        result.z.z = cos(rad);        result.w.z = 0;
    result.x.w = 0;            result.y.w = 0;            result.z.w = 0;                result.w.w = 1;
    
    return result;
}

//scaling the matrix
mat4 scale(float x, float y, float z){
    
    mat4 scale = {{x, 0, 0, 0}, {0, y, 0, 0}, {0, 0, z, 0}, {0, 0, 0, 1}};
    return scale;
    
}

//look_at function for viewing
mat4 look_at(vec4 eyePoint, vec4 atPoint, vec4 upVector){
    
    vec4 view_plane_normal, normal_up, normal_v;
    
    view_plane_normal = normalize(sub_vec(eyePoint, atPoint));
    normal_up = normalize(cross_product(upVector, view_plane_normal));
    normal_v = normalize(cross_product(view_plane_normal, normal_up));
    
    mat4 temp = {
                        {normal_up.x, normal_v.x, view_plane_normal.x, eyePoint.x},
                        {normal_up.y, normal_v.y, view_plane_normal.y, eyePoint.y},
                        {normal_up.z, normal_v.z, view_plane_normal.z, eyePoint.z},
                        {0, 0, 0, 1}};
    mat4 model_view_matrix = inverse(transpose(temp));
    return model_view_matrix;
}

mat4 frustum(float left, float right, float bottom, float top, float near, float far){
    mat4 frustum = {
                    {-(2*near/(right-left)), 0, 0, 0},
                    {0, -(2*near/(top-bottom)), 0, 0},
                    {((left+right)/(right-left)), ((top+bottom)/(top-bottom)), ((near + far)/(far - near)), -1},
                    {0, 0, -((2*near*far)/(far - near)), 0}
                    };
    return frustum;
    
}

vec4 product(vec4 v1, vec4 v2){
    vec4 result;
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
    result.w = 1.0;
    
    return result;
}


 double rad = (M_PI / 180);
 vec4 vertices[589860];
 int num_vertices = 589860; //(20*20 floor tiles)*36 vertices + (98304 sphere vertices * 6 spheres) = 604224
 vec4 colors[589860];
vec4 normals[589860];
 int v_index = 0;
 mat4 frusty;

 double degree = M_PI/2;
 int ball_movement = 0;
 float green_degree = 0;
 float blue_degree = 0;
 float yellow_degree = 0;
 float orange_degree = 0;

 float sx = 1;
 float sy = 1;
 float sz = 1;

 float lightx = 0;
 float lighty = 3;
 float lightz = 0;

 typedef struct
 {
     vec4 reflect_ambient;
     vec4 reflect_diffuse;
     vec4 reflect_specular;
     GLfloat shininess;
 } material;

 mat4 model_view_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 projection_matrix = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

 vec4 light_position = {0, 0, 0, 0};
 vec4 vPosition = {0,0,0,0};
 vec4 eyePosition = {0, 3, 8, 1};

 vec4 ambient_product, diffuse_product, specular_product;

 vec4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
 vec4 light_specular = {1.0, 1.0, 1.0, 1.0};
 vec4 light_ambient = {0.2, 0.2, 0.2, 1.0};

 vec4 regular_ball_scale = {0.5, 0.5, 0.5, 1};
 vec4 light_ball_scale = {0.3, 0.3, 0.3, 1};

 GLuint ctm_location;
 GLuint is_shadow;
 GLuint model_view_location;
 GLuint projection_location;
 GLuint ambient_product_location;
 GLuint diffuse_product_location;
 GLuint specular_product_location;
 GLuint light_location;
 GLuint shininess_location;
 GLuint attenuation_constant_location;
 GLuint attenuation_linear_location;
 GLuint attenuation_quadratic_location;
 GLuint is_shadow_location;

 mat4 red_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 green_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 blue_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 yellow_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 orange_ball_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 light_source_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
 mat4 table_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

 vec4 cube_vertices[36] =
    {
     //cube front
     {-0.5, -0.5,  0.5, 1.0},    // bottom left
     { 0.5, -0.5,  0.5, 1.0},    // bottom right
     { 0.5,  0.5,  0.5, 1.0},    // top right
     {-0.5, -0.5,  0.5, 1.0},    // bottom left
     { 0.5,  0.5,  0.5, 1.0},    // top right
     {-0.5,  0.5,  0.5, 1.0},    // top left
     
     //cube right
     { 0.5, 0.5,  0.5, 1.0},        // bottom left
     { 0.5, -0.5, 0.5, 1.0},        // bottom right
     { 0.5, -0.5, -0.5, 1.0},    // top right
     { 0.5, -0.5, -0.5, 1.0},        // bottom left
     { 0.5, 0.5, -0.5, 1.0},        // top right
     { 0.5, 0.5, 0.5, 1.0},       // top left
     
     //cube left
     { -0.5,  -0.5, 0.5, 1.0},    // bottom left
     { -0.5, 0.5, 0.5, 1.0},    // bottom right
     { -0.5,  -0.5, -0.5, 1.0},    // top right
     { -0.5,  -0.5, -0.5, 1.0},    // bottom left
     {-0.5, 0.5, 0.5, 1.0},    // top right
     { -0.5,  0.5, -0.5, 1.0},    // top left
     
     //cube back
     {0.5,  0.5, -0.5, 1.0},    // bottom left
     {0.5,  -0.5,  -0.5, 1.0},    // bottom right
     { -0.5,  -0.5,  -0.5, 1.0},    // top right
     {-0.5,  0.5, -0.5, 1.0},    // bottom left
     { 0.5,  0.5,  -0.5, 1.0},    // top right
     {-0.5, -0.5, -0.5, 1.0},    // top left
     
     //cube top
     { 0.5,  0.5, 0.5, 1.0},    // bottom left
     {0.5,  0.5, -0.5, 1.0},    // bottom right
     {-0.5,  0.5,  -0.5, 1.0},    // top right
     {0.5, 0.5, 0.5, 1.0},    // bottom left
     {-0.5,  0.5,  -0.5, 1.0},    // top right
     { -0.5, 0.5,  0.5, 1.0},    // top left
     
     //cube bottom
     {-0.5, -0.5, 0.5, 1.0},    // bottom left
     { -0.5, -0.5, -0.5, 1.0},    // bottom right
     { 0.5, -0.5,  0.5, 1.0},    // top right
     {-0.5, -0.5, -0.5, 1.0},    // bottom left
     { 0.5, -0.5,  -0.5, 1.0},    // top right
     {0.5, -0.5,  0.5, 1.0}     // top left
     };
     
 material materials[5] = {
     {{1, 0, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 1}, 10}, //red
     {{0, 1, 0, 1}, {0, 1, 0, 1}, {1, 1, 1, 1}, 10},    //green
     {{0, 0, 1, 1}, {0, 0, 1, 1}, {1, 1, 1, 1}, 10},    //blue
     {{1, 1, 0, 1}, {1, 1, 0, 1}, {1, 1, 1, 1}, 10},    //yellow
     {{1, 0.5, 0, 1}, {1, 0.5, 0, 1}, {1, 1, 1, 1}, 10} //orange
 };

material table_material = {
    {0.0, 0.5, 0.0, 10}
};

material light_material = {
    {1.0, 1.0, 1.0, 10}
};


 vec4 red_color = {1, 0, 0, 1};
 vec4 green_color = {0, 1, 0, 1};
 vec4 blue_color = {0, 0, 1, 1};
 vec4 yellow_color = {1, 1, 0, 0};
 vec4 orange_color = {1, 0.647, 0, 1};
 vec4 white_color = {1, 1, 1, 1};
 vec4 floor_color = {0.18, 0.31, 0.18, 1};

     
 vec4 red_ball_location = {0.0, 0.5, 0.0, 1.0};
 vec4 green_ball_location = {1.0, 0.5, 0.0, 1.0};
 vec4 blue_ball_location = {2.0, 0.5, 0.0, 1.0};
 vec4 yellow_ball_location = {3.0, 0.5, 0.0, 1.0};
 vec4 orange_ball_location = {4.0, 0.5, 0.0, 1.0};
 vec4 light_sphere_location = {0.0, 3.0, 0.0, 1.0};

 void normal_vec(){
     vec4 result;
     for(int i = 0; i<589860 ; i+=3){
         vec4 v_1 = vector_vector_subtraction(vertices[i+1], vertices[i]);
         vec4 v_2 = vector_vector_subtraction(vertices[i+2], vertices[i]);
          result = normalize(cross_product(v_1, v_2));
         normals[i] = result;
         normals[i+1] =result;
         normals[i+2] = result;
     }
     
 }

 void fill_floor(){
     int i, j, k, posx, posz = 0;
     mat4 temp;
     temp = mult_matrix(translate(0, 0, 0), scale(20, 0.005, 20));
     for (k = 0; k < 36; k++){
         vertices[v_index] = matrix_x_vector(temp, cube_vertices[k]);
         colors[v_index] = floor_color;
         v_index++;
     }
 }

 vec4 color(char *color_name){
     //generates random RGB values for coloring
     vec4 result;
     char *red = "red";
     char *green = "green";
     char *blue = "blue";
     char *yellow = "yellow";
     char *orange = "orange";
     char *white = "white";
     char *floor = "floor";
     
     int result_red = strcmp(color_name, red);
     int result_green = strcmp(color_name, green);
     int result_blue = strcmp(color_name, blue);
     int result_yellow = strcmp(color_name, yellow);
     int result_orange = strcmp(color_name, orange);
     int result_white = strcmp(color_name, white);
     int result_floor = strcmp(color_name, floor);
     
     if (!result_red){
         result = red_color;
     }
     
     if (!result_green){
         result = green_color;
     }
     
     if (!result_blue){
         result=  blue_color;
     }
     
     if (!result_yellow){
         result= yellow_color;
     }
     
     if (!result_orange){
         result = orange_color;
     }
     
     if (!result_white){
         result= white_color;
     }
     
     if(!result_floor){
         result =  floor_color;
     }
     return result;
 }

 void triangle(vec4 top, vec4 bottom_right, vec4 bottom_left, vec4 pos, vec4 size, char *c){
     mat4 temp = matrix_matrix_multiplication(translate(pos.x, pos.y, pos.z), scaling(size.x, size.y, size.z));
     //incriment v_index through the normal and vertex arrays to normal the triangle
     vec4 curr = color(c);
     colors[v_index] = curr;
     vertices[v_index++] = matrix_vector_multiplication(temp, top);
     colors[v_index] = curr;
     vertices[v_index++] = matrix_vector_multiplication(temp, bottom_left);
     colors[v_index] = curr;
     vertices[v_index++] = matrix_vector_multiplication(temp, bottom_right);
     
 }

 void subdivide(vec4 v1, vec4 v2, vec4 v3, int level, vec4 pos, vec4 size, char *color){
     
     //base case... draw the triangle
     if (level == 0){
         triangle(v1, v2, v3, pos, size, color);
     
     //tail recursive case
     } else {
         /* calculating midpoint of edges */
         //edge from v1 to v2
         float v12x = 0.5 * (v1.x + v2.x);
         float v12y = 0.5 * (v1.y + v2.y);
         float v12z = 0.5 * (v1.z + v2.z);
         
         vec4 v12 = {v12x, v12y, v12z, 0}; //put the point into a vector
         
         //normalize them to put points on the sphere
         v12 = normalize(v12);
         //the normalize messed with the w point so change it back to 1.0
         v12.w = 1.0;
         
         //edge from v1 to v3
         float v13x = 0.5 * (v1.x + v3.x);
         float v13y = 0.5 * (v1.y + v3.y);
         float v13z = 0.5 * (v1.z + v3.z);
         
         vec4 v13 = {v13x, v13y, v13z, 0}; //put the points into a vector
         
         //normalize them to put points on the sphere
         v13 = normalize(v13);
         v13.w = 1.0;
         
         //edge from v2 to v3
         float v23x = 0.5 * (v3.x + v2.x);
         float v23y = 0.5 * (v3.y + v2.y);
         float v23z = 0.5 * (v3.z + v2.z);
         
         vec4 v23 = {v23x, v23y, v23z, 0}; //put the points into a vector
         
         //normalize them to put points on the sphere
         v23 = normalize(v23);
         v23.w = 1.0;

         /* recurse */
         subdivide(v1, v12, v13, level - 1, pos, size, color);
         subdivide(v12, v2, v23, level - 1, pos, size, color);
         subdivide(v13, v23, v3, level - 1, pos, size, color);
         subdivide(v12, v23, v13, level - 1, pos, size, color);
     }
 }

 void sphere(vec4 pos, vec4 size, char *color){
     
     /*Building the Octohedron*/
     /*triangle for top pyramid*/
     vec4 top = {0.0, 1.0, 0.0, 1.0};
     vec4 bottom_right = {1.0, 0.0, 0.0, 1.0};
     vec4 bottom_left = {0.0, 0.0, 1.0, 1.0};
     
     //subdivide the first side of the octohedron
     subdivide(top, bottom_right, bottom_left, 6, pos, size, color);
     
     //for the rest of the sides for the top half of the octohedron
     //rotate the face 90 degrees and then subdivide it into the
     //smaller triangles as well
     for (int i = 0; i < 3; i++){

         bottom_left = matrix_vector_multiplication(rotate_y(90), bottom_left);
         bottom_right = matrix_vector_multiplication(rotate_y(90), bottom_right);

         subdivide(top, bottom_right, bottom_left, 6, pos, size, color);
     }
     
     //triangle for bottom pyramid of octohedron
     vec4 bottom = {0.0, -1.0, 0.0, 1.0};
     vec4 upper_right = {1.0, 0.0, 0.0, 1.0};
     vec4 upper_left = {0.0, 0.0, 1.0, 1.0};
     
     subdivide(bottom, upper_left, upper_right, 6, pos, size, color);
     
     //same process for the bottom half
     for (int i = 0; i < 3; i++){
     
         upper_left = matrix_vector_multiplication(rotate_y(90), upper_left);
         upper_right = matrix_vector_multiplication(rotate_y(90), upper_right);

         subdivide(bottom, upper_left, upper_right, 6, pos, size, color);
     }
 }

 void place_balls(){
     //red ball
     sphere(red_ball_location, regular_ball_scale, "red");
     //green ball
     sphere(green_ball_location, regular_ball_scale, "green");
     //blue ball
     sphere(blue_ball_location, regular_ball_scale, "blue");
     //yellow ball
     sphere(yellow_ball_location, regular_ball_scale, "yellow");
     //orange ball
     sphere(orange_ball_location, regular_ball_scale, "orange");
     //light sphere
     sphere(light_sphere_location, light_ball_scale, "white");
     
 }
 void set_stage(){
     float eyex, eyey, eyez;
     float atx, aty, atz;
     projection_matrix = frustum(-.5, .5, -.5, .5, -.5, .5);
     
         eyex = 0;
         eyey = 3;
         eyez = 8;
         
         atx = 0;
         aty = 0;
         atz = 0;
         
         vec4 eye = {eyex, eyey, eyez, 1};
         vec4 at = {atx, aty, atz, 1};
         vec4 light = {lightx, lighty, lightz, 1};
         vec4 up = {0, 1, 0, 1};
         
         light_position = light;
         eyePosition = eye;
     
         model_view_matrix = look_at(eye, at, up);
 }

 void apply_shadow(){
     is_shadow = 1;
 }


 void init(void)
 {
     set_stage();
     fill_floor();
     place_balls();
     normal_vec();
     apply_shadow();

     GLuint program = initShader("/Users/liangyilun/Desktop/cs1566/proj3/proj3/vshader.glsl", "/Users/liangyilun/Desktop/cs1566/proj3/proj3/fshader.glsl");
     glUseProgram(program);
     ctm_location = glGetUniformLocation(program, "ctm");

     GLuint vao;
     glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     GLuint buffer;
     glGenBuffers(1, &buffer);
     glBindBuffer(GL_ARRAY_BUFFER, buffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), NULL, GL_STATIC_DRAW);
     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
     glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);

     GLuint vPosition = glGetAttribLocation(program, "vPosition");
     glEnableVertexAttribArray(vPosition);
     glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

     GLuint vNormal = glGetAttribLocation(program, "vNormal");
     glEnableVertexAttribArray(vNormal);
     glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + sizeof(vertices));

     glEnable(GL_CULL_FACE);
     model_view_location = glGetUniformLocation(program, "model_view_matrix");
     projection_location = glGetUniformLocation(program, "projection_matrix");
     light_location = glGetUniformLocation(program, "light_position");
     is_shadow_location = glGetUniformLocation(program, "is_shadow");
        
     ambient_product_location = glGetUniformLocation(program, "ambient_product");
     diffuse_product_location = glGetUniformLocation(program, "diffuse_product");
     specular_product_location = glGetUniformLocation(program, "specular_product");
 
     attenuation_constant_location = glGetUniformLocation(program, "attenuation_constant");
     attenuation_linear_location = glGetUniformLocation(program, "attenuation_linear");
     attenuation_quadratic_location = glGetUniformLocation(program, "attenuation_quadratic");
     shininess_location = glGetUniformLocation(program, "shininess");
     
     glEnable(GL_DEPTH_TEST);
     glClearColor(0, 0, 0, 1.0);
     glDepthRange(1,0);
 }

 void display(void)
 {
     
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *) &model_view_matrix);
     glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *) &projection_matrix);
     glUniform1f(attenuation_constant_location, 0);
      glUniform1f(attenuation_linear_location, 0);
      glUniform1f(attenuation_quadratic_location, 0);
     
     //draw table, no shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &table_ctm);
     ambient_product = product(table_material.reflect_ambient, light_ambient);
     diffuse_product = product(table_material.reflect_diffuse, light_diffuse);
     specular_product = product(table_material.reflect_specular, light_specular);
     glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
     glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
     glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
     glUniform1fv(shininess_location, 1, (GLfloat *) &table_material.shininess);
     glDrawArrays(GL_TRIANGLES, 0, 36);
     
     //draw red ball with shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &red_ball_ctm);
     ambient_product = product(materials[0].reflect_ambient, light_ambient);
     diffuse_product = product(materials[0].reflect_diffuse, light_diffuse);
     specular_product = product(materials[0].reflect_specular, light_specular);
     glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
     glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
     glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
    glUniform1fv(shininess_location, 1, (GLfloat *) &materials[0].shininess);
     glDrawArrays(GL_TRIANGLES, 36, 98304);
     
     glUniform1i(is_shadow_location, 1);
     
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &red_ball_ctm);
     ambient_product = product(materials[0].reflect_ambient, light_ambient);
         diffuse_product = product(materials[0].reflect_diffuse, light_diffuse);
         specular_product = product(materials[0].reflect_specular, light_specular);
             glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
             glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
             glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
             glUniform1fv(shininess_location, 1, (GLfloat *) &materials[0].shininess);
     glDrawArrays(GL_TRIANGLES, 36, 98304);
     
     //draw green ball with shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &green_ball_ctm);
     ambient_product = product(materials[1].reflect_ambient, light_ambient);
         diffuse_product = product(materials[1].reflect_diffuse, light_diffuse);
         specular_product = product(materials[1].reflect_specular, light_specular);
             glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
             glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
             glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
             glUniform1fv(shininess_location, 1, (GLfloat *) &materials[1].shininess);
     glDrawArrays(GL_TRIANGLES, 98340, 98304);
     
     glUniform1i(is_shadow_location, 1);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &green_ball_ctm);
     ambient_product = product(materials[1].reflect_ambient, light_ambient);
             diffuse_product = product(materials[1].reflect_diffuse, light_diffuse);
             specular_product = product(materials[1].reflect_specular, light_specular);
                 glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
                 glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
                 glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
                 glUniform1fv(shininess_location, 1, (GLfloat *) &materials[1].shininess);
     glDrawArrays(GL_TRIANGLES, 98340, 98304);
     
     //draw blue ball with shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &blue_ball_ctm);
     ambient_product = product(materials[2].reflect_ambient, light_ambient);
             diffuse_product = product(materials[2].reflect_diffuse, light_diffuse);
             specular_product = product(materials[2].reflect_specular, light_specular);
                 glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
                 glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
                 glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
                 glUniform1fv(shininess_location, 1, (GLfloat *) &materials[2].shininess);
     glDrawArrays(GL_TRIANGLES, 196644, 98304);
     
     glUniform1i(is_shadow_location, 1);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &blue_ball_ctm);
     ambient_product = product(materials[2].reflect_ambient, light_ambient);
     diffuse_product = product(materials[2].reflect_diffuse, light_diffuse);
     specular_product = product(materials[2].reflect_specular, light_specular);
         glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
         glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
         glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
         glUniform1fv(shininess_location, 1, (GLfloat *) &materials[2].shininess);
     glDrawArrays(GL_TRIANGLES, 196644, 98304);
     
     //draw yellow ballwith shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &yellow_ball_ctm);
     ambient_product = product(materials[3].reflect_ambient, light_ambient);
        diffuse_product = product(materials[3].reflect_diffuse, light_diffuse);
        specular_product = product(materials[3].reflect_specular, light_specular);
            glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
            glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
            glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
            glUniform1fv(shininess_location, 1, (GLfloat *) &materials[3].shininess);
     glDrawArrays(GL_TRIANGLES, 294948, 98304);
     
     glUniform1i(is_shadow_location, 1);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &yellow_ball_ctm);
     ambient_product = product(materials[3].reflect_ambient, light_ambient);
        diffuse_product = product(materials[3].reflect_diffuse, light_diffuse);
        specular_product = product(materials[3].reflect_specular, light_specular);
            glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
            glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
            glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
            glUniform1fv(shininess_location, 1, (GLfloat *) &materials[3].shininess);
     glDrawArrays(GL_TRIANGLES, 294948, 98304);
     
     //draw orange ball with shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &orange_ball_ctm);
     ambient_product = product(materials[4].reflect_ambient, light_ambient);
     diffuse_product = product(materials[4].reflect_diffuse, light_diffuse);
     specular_product = product(materials[4].reflect_specular, light_specular);
     glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
     glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
     glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
     glUniform1fv(shininess_location, 1, (GLfloat *) &materials[4].shininess);
     glDrawArrays(GL_TRIANGLES, 393252, 98304);
     
     glUniform1i(is_shadow_location, 1);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &orange_ball_ctm);
     ambient_product = product(materials[4].reflect_ambient, light_ambient);
     diffuse_product = product(materials[4].reflect_diffuse, light_diffuse);
     specular_product = product(materials[4].reflect_specular, light_specular);
         glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
         glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
         glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
         glUniform1fv(shininess_location, 1, (GLfloat *) &materials[4].shininess);
     glDrawArrays(GL_TRIANGLES, 393252, 98304);
     
     //draw light ball, no shadow
     glUniform1i(is_shadow_location, 0);
     glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &light_source_ctm);
     ambient_product = product(light_material.reflect_ambient, light_ambient);
     diffuse_product = product(light_material.reflect_diffuse, light_diffuse);
     specular_product = product(light_material.reflect_specular, light_specular);
         glUniform4fv(ambient_product_location, 1, (GLfloat *) &ambient_product);
         glUniform4fv(diffuse_product_location, 1, (GLfloat *) &diffuse_product);
         glUniform4fv(specular_product_location, 1, (GLfloat *) &specular_product);
         glUniform1fv(shininess_location, 1, (GLfloat *) &light_material.shininess);
     glDrawArrays(GL_TRIANGLES, 491556, 98304);
     
     glUniform4fv(light_location, 1, (GLfloat *) &light_position);
     
     glutSwapBuffers();
 }

 void idle(void){
     if (ball_movement == 1){
         mat4 tempred;
         mat4 tempgreen;
         mat4 tempblue;
         mat4 tempyellow;
         mat4 temporange;
         
         tempred = mult_matrix(scale(1, 1, 1), translate(0,0,0));
         red_ball_ctm = tempred;
     
         tempgreen = mult_matrix(rotatey(green_degree), translate(0, 0, 0));
         green_ball_ctm = tempgreen;
     
         tempblue = mult_matrix(rotatey(blue_degree), translate(0, 0, 0));
         blue_ball_ctm = tempblue;
         
         tempyellow = matrix_matrix_multiplication(rotate_y(yellow_degree), translate(0, 0, 0));
         yellow_ball_ctm = tempyellow;
         
         temporange = mult_matrix(rotatey(orange_degree), translate(0, 0, 0));
         orange_ball_ctm = temporange;
     
         green_degree += 1.3;
         blue_degree += 0.45;
         yellow_degree += 2;
         orange_degree += 1.0;
     
     }
     glutPostRedisplay();
 }

 void keyboard(unsigned char key, int mousex, int mousey)
 {
     if(key == 'r')
         exit(0);
     
     if(key == 'b')
         ball_movement = 1;
     
     //zoom out
     if (key == 'o'){
         //vec4 newEye
         sx /= 1.02;
         sy /= 1.02;
         sz /= 1.02;
         mat4 temp = model_view_matrix;
         model_view_matrix = mult_matrix(temp, scale(sx, sy, sz));
     }
     //zoom in
     if (key == 'z'){
         sx *= 1.02;
         sy *= 1.02;
         sz *= 1.02;
         mat4 temp = model_view_matrix;
         model_view_matrix = mult_matrix(temp, scale(sx, sy, sz));
     }
     
     //move light source away from viewer
     if (key == 'q'){
         light_sphere_location.z -= 0.2;
         light_position.z -= 0.2;
         light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
         
     }
     
     //move light source closer to viewer
     if (key == 'e'){
         light_sphere_location.z += 0.2;
         light_position.z += 0.2;
         light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
     }
     
     //move light source right
     if (key == 'd'){
         light_sphere_location.x += 0.2;
         light_position.x += 0.2;
         light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
     }
     
     //move light source left
     if (key == 'a'){
         light_sphere_location.x -= 0.2;
         light_position.x -= 0.2;
         light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
     }
     
     //move light source away from table
     if (key == 'w'){
         light_sphere_location.y += 0.2;
         light_position.y += 0.2;
         light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
     }
     
     //move light source closer to table
     if (key == 's'){
         light_sphere_location.y -= 0.2;
         light_position.y -= 0.2;
         light_source_ctm = mult_matrix(scale(1, 1, 1), translate(light_sphere_location.x, light_sphere_location.y, light_sphere_location.z));
     }
     
     //circle right
     if (key == 'l'){
         degree -= 0.05;
         vec4 eye = {8*cos(degree), 3, 8*sin(degree), 1.0};
         vec4 at = {0, 0, 0, 1.0};
         vec4 up = {0, 1, 0, 1.0};
         model_view_matrix = look_at(eye, at, up);
         
     }
     
     //circle left
     if (key == 'j'){
         degree += 0.05;
         vec4 eye = {8*cos(degree), 3, 8*sin(degree), 1.0};
         vec4 at = {0, 0, 0, 1.0};
         vec4 up = {0, 1, 0, 1.0};
         model_view_matrix =  look_at(eye, at, up);
     }
     
     //pan up
     if (key == 'u'){
         //degree += 0.05;
         vec4 eye = {8*cos(degree), 3*sin(degree), 8*sin(degree), 1.0};
         vec4 at = {0, 0, 0, 1.0};
         vec4 up = {0, 1, 0, 1.0};
         model_view_matrix =  look_at(eye, at, up);
     }
     

   

     glutPostRedisplay();
 }


 int main(int argc, char **argv)
 {
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
     glutInitWindowSize(512, 512);
     glutCreateWindow("Shadows");
     //glewInit();
     init();
     glutDisplayFunc(display);
     glutKeyboardFunc(keyboard);
     glutIdleFunc(idle);
     glutMainLoop();

     return 0;
 }
