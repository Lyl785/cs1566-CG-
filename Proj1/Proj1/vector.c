//
//  main.c
//  Lab2
//
//  Created by 梁轶伦 on 9/9/19.
//  Copyright © 2019 Yilun Liang. All rights reserved.
//

#include <stdio.h>
#include "vector.h"

int print_vector(vec4 vector){
    printf("[ %.2f %.2f %.2f %.2f ]\n", vector.x, vector.y, vector.z, vector.w);
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

GLfloat dot_product(vec4 vector1, vec4 vector2){
    GLfloat result = vector1.x * vector2.x +vector1.y *
        vector2.y + vector1.z * vector2.z + vector1.w * vector2.w;
    return result;
}

vec4 cross_product(vec4 vector1, vec4 vector2){
    vec4 result = {vector1.y * vector2.z - vector1.z * vector2.y, vector1.z *
        vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x, 0.0};
    return result;
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

mat4 scalar_matrix_multiplication(mat4 matrix, GLfloat temp){

    vec4 vector1 = scalar_vector_multiplication(matrix.x, temp);
    vec4 vector2 = scalar_vector_multiplication(matrix.y, temp);
    vec4 vector3 = scalar_vector_multiplication(matrix.z, temp);
    vec4 vector4 = scalar_vector_multiplication(matrix.w, temp);
    mat4 result = {vector1, vector2, vector3, vector4};
    return result;
}

mat4 matrix_matrix_addition(mat4 matrix1, mat4 matrix2){
    
    vec4 vector1 = vector_vector_addition(matrix1.x, matrix2.x);
    vec4 vector2 = vector_vector_addition(matrix1.y, matrix2.y);
    vec4 vector3 = vector_vector_addition(matrix1.z, matrix2.z);
    vec4 vector4 = vector_vector_addition(matrix1.w, matrix2.w);
    mat4 result = {vector1, vector2, vector3, vector4};
    return result;
}

mat4 matrix_matrix_subtraction(mat4 matrix1, mat4 matrix2){
    
    vec4 vector1 = vector_vector_subtraction(matrix1.x, matrix2.x);
    vec4 vector2 = vector_vector_subtraction(matrix1.y, matrix2.y);
    vec4 vector3 = vector_vector_subtraction(matrix1.z, matrix2.z);
    vec4 vector4 = vector_vector_subtraction(matrix1.w, matrix2.w);
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







GLfloat determinant(mat4 matrix){
    GLfloat result1 = matrix.x.x * (matrix.y.y * matrix.z.z * matrix.w.w + matrix.y.z * matrix.z.w * matrix.w.y + matrix.y.w * matrix.z.y * matrix.w.z - matrix.y.w * matrix.z.z * matrix.w.y - matrix.y.z * matrix.z.y * matrix.w.w - matrix.y.y *  matrix.z.w * matrix.w.z) ;
   // printf("%.2f\n", result1);
    
    GLfloat result2 = matrix.x.y * (matrix.y.x * matrix.z.z * matrix.w.w + matrix.z.x * matrix.w.z * matrix.y.w + matrix.w.x * matrix.y.z * matrix.z.w - matrix.w.x * matrix.z.z * matrix.y.w - matrix.z.x * matrix.y.z * matrix.w.w - matrix.y.x *   matrix.w.z * matrix.z.w) ;
     //printf("%.2f\n", result2);
    
     GLfloat result3 = matrix.x.z * (matrix.y.x * matrix.z.y * matrix.w.w + matrix.z.x * matrix.w.y * matrix.y.w + matrix.w.x * matrix.y.y * matrix.z.w - matrix.w.x * matrix.z.y * matrix.y.w - matrix.z.x * matrix.y.y * matrix.w.w - matrix.y.x * matrix.w.y * matrix.z.w) ;
     //printf("%.2f\n", result3);
    
      GLfloat result4 = matrix.x.w * (matrix.y.x * matrix.z.y * matrix.w.z + matrix.z.x * matrix.w.y * matrix.y.z + matrix.w.x * matrix.y.y * matrix.z.z - matrix.w.x * matrix.z.y * matrix.y.z - matrix.z.x * matrix.y.y * matrix.w.z - matrix.y.x * matrix.w.y * matrix.z.z) ;
     //printf("%.2f\n", result4);
    
    GLfloat result = result1 - result2 + result3 - result4;
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
    
    mat4 result = scalar_matrix_multiplication(adjugate_matrix(matrix) , 1/determinant(matrix));
    
    return result;


}




vec4 matrix_vector_multiplication(mat4 matrix, vec4 vector){
    
    mat4 new_matrix = transpose_of_a_matrix(matrix);
    vec4 result = {dot_product(new_matrix.x, vector), dot_product(new_matrix.y, vector),
        dot_product(new_matrix.z, vector), dot_product(new_matrix.w, vector)};
    
    return result;
}





