/*
 * triangle.c
 *
 *
 */


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

#include <stdio.h>
#include <math.h>
#include <time.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define num_vertices 10800
#define inner_radian 0.5f
#define outter_radian 0.25f
#define triangle_per_band 60
#define band_per_torus 60
#define radius 0.5f




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

vec4 vertices[589860];
vec4 colors[589860];
vec4 normals[589860];
vec4 arr_side[100000];
GLfloat global_scale = 1.0;
int start_drag = 0;

vec4 random_color_generator();




void cone(){
    
    int num_vertices_band = 3 * triangle_per_band;
    GLfloat incrument_of_torus = 2 * M_PI / band_per_torus;
    GLfloat incrument_of_ring = 2 * M_PI / (triangle_per_band / 2);

    for(int i = 0; i < band_per_torus; i++){
        GLfloat theta = i * incrument_of_torus;
        GLfloat theta_next = (i + 1) * incrument_of_torus;
        for(int j = 0; j < num_vertices_band; j += 6){
            int index = i * num_vertices_band + j;
            GLfloat phi = (j/6) * incrument_of_ring;
            GLfloat phi_next = (j/6 + 1) * incrument_of_ring;
            
            GLfloat p1_x = 0.0;
            GLfloat p1_y = outter_radian * sinf(phi);
            GLfloat p1_z = inner_radian + outter_radian * cosf(phi);
            GLfloat p2_x = 0.0;
            GLfloat p2_y = outter_radian * sinf(phi_next);
            GLfloat p2_z = inner_radian + outter_radian * cosf(phi_next);
            
            vertices[index].x = p2_z * sin(theta);
            vertices[index].y = p2_y;
            vertices[index].z = p2_z * cos(theta);
            vertices[index].w = 1.0;
            
            vertices[index + 1].x = p1_z * sin(theta);
            vertices[index + 1].y = p1_y;
            vertices[index + 1].z = p1_z * cos(theta);
            vertices[index + 1].w = 1.0;
            
            vertices[index + 2].x = p2_z * sin(theta_next);
            vertices[index + 2].y = p2_y;
            vertices[index + 2].z = p2_z * cos(theta_next);
            vertices[index + 2].w = 1.0;
                       
            
            vertices[index + 3].x = p1_z * sin(theta_next);
            vertices[index + 3].y = p1_y;
            vertices[index + 3].z = p1_z * cos(theta_next);
            vertices[index + 3].w = 1.0;
                       
            vertices[index + 4].x = p2_z * sin(theta_next);
            vertices[index + 4].y = p2_y;
            vertices[index + 4].z = p2_z * cos(theta_next);
            vertices[index + 4].w = 1.0;
                       
            vertices[index + 5].x = p1_z * sin(theta);
            vertices[index + 5].y = p1_y;
            vertices[index + 5].z = p1_z * cos(theta);
            vertices[index + 5].w = 1.0;
                       
                       
            
        }
    }
    
    int count = 3600 * 3;
    for(int k = 0; k < count; k += 3){
        colors[k] = random_color_generator();
        colors[k + 1] = colors[k];
        colors[k + 2] = colors[k];
        
    }
    
   /* for(int i = 0; i < 40; i++)
    {
        vertices[index].x = 0.0;
        vertices[index].y = -0.5;    // don't forget to change it to -0.5
        vertices[index].z = 0.0;
        vertices[index].w = 1.0;
        
        arr_side[index].x = 0.0;
        arr_side[index].y = 0.0;
        arr_side[index].z = 0.0;
        arr_side[index].w = 1.0;
        
        vec4 vertex1 = {cos((arc + 4.5) * M_PI/180) * radius, -0.5, sin((arc + 4.5) * M_PI/180) * radius, 1.0};
    
        
        vertices[index + 1].x = vertex1.x;
        vertices[index + 1].y = vertex1.y;
        vertices[index + 1].z = vertex1.z;
        vertices[index + 1].w = 1.0;
        
        arr_side[index + 1].x = vertex1.x;
        arr_side[index + 1].y = vertex1.y;
        arr_side[index + 1].z = vertex1.z;
        arr_side[index + 1].w = 1.0;
        
        vec4 vertex2 = {cos(arc * M_PI/180) * radius, -0.5, sin(arc * M_PI/180) * radius, 1.0};
        vertices[index + 2].x = vertex2.x;
        vertices[index + 2].y = vertex2.y;
        vertices[index + 2].z = vertex2.z;
        vertices[index + 2].w = 1.0;
        
        arr_side[index + 2].x = vertex2.x;
        arr_side[index + 2].y = vertex2.y;
        arr_side[index + 2].z = vertex2.z;
        arr_side[index + 2].w = 1.0;
        
        
        
        colors[index] = random_color_generator();
        colors[index + 1] = colors[index];
        colors[index + 2] = colors[index];
        
        index += 3;
        arc += 4.5;
    }*/
    
    //printf("Hello!");
    
}



GLuint ctm_location;
mat4 ctm = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
GLfloat x_value = 0;
int isGoingRight = 1;

mat4 translate(GLfloat x, GLfloat y, GLfloat z)
{
    mat4 result = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
    result.w.x = x;
    result.w.y = y;
    result.w.z = z;
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

mat4 translation(vec4 mov){
    mat4 result = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {mov.x,mov.y,mov.z,1}};

    return result;

    
}
mat4 scaling(GLfloat scale){
    global_scale *= scale;
    mat4 result = {{global_scale , 0, 0, 0}, {0,global_scale , 0, 0}, {0, 0,  global_scale, 0}, {0, 0, 0, 1.0}};
    return result;
    
}

//mat4 transformation_matrix(){
    
    
    
    
    
//}

vec4 normalize(vec4 l){
    vec4 result;
    result.x = l.x/(sqrt(pow(l.x, 2) + pow(l.y, 2) + pow(l.z, 2)));
    result.y = l.y/(sqrt(pow(l.x, 2) + pow(l.y, 2) + pow(l.z, 2)));
    result.z = l.z/(sqrt(pow(l.x, 2) + pow(l.y, 2) + pow(l.z, 2)));
    result.w = 0;
    return result;
}


int v_index = 0;

vec4 table[36] = {
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
        { -0.5, -0.5, 0.5, 1.0}

};

void table_fun(){
    for(int i = 0; i < 36; i++){
        vec4 dark_green = {0.0, 1.0, 0.0, 1.0};
        vertices[v_index] = table[i];
        colors[v_index] = dark_green;
        v_index++;
    }
    
}



vec4 color(){
    //generates random RGB values for coloring
    vec4 color;
    color.x = (float)rand() / (float)(RAND_MAX);
    color.y = (float)rand() / (float)(RAND_MAX);
    color.z = (float)rand() / (float)(RAND_MAX);
    color.w = 1.0;
    
    return color;
}

//vertex and color arrays' placeholder
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

void triangle(vec4 top, vec4 bottom_right, vec4 bottom_left){
    
    //incriment v_index through the color and vertex arrays to color the triangle
    vec4 curr = color();
    colors[v_index] = curr;
    vertices[v_index++] = top;
    colors[v_index] = curr;
    vertices[v_index++] = bottom_left;
    colors[v_index] = curr;
    vertices[v_index++] = bottom_right;
    
}

void subdivide(vec4 v1, vec4 v2, vec4 v3, int level){
    
    //base case... draw the triangle
    if (level == 0){
        triangle(v1, v2, v3);
    
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
        subdivide(v1, v12, v13, level - 1);
        subdivide(v12, v2, v23, level - 1);
        subdivide(v13, v23, v3, level - 1);
        subdivide(v12, v23, v13, level - 1);
    }
}

void sphere(){
    
    /*Building the Octohedron*/
    
    /*triangle for top pyramid*/
    vec4 top = {0.0, 1.0, 0.0, 1.0};
    vec4 bottom_right = {1.0, 0.0, 0.0, 1.0};
    vec4 bottom_left = {0.0, 0.0, 1.0, 1.0};
    
    //subdivide the first side of the octohedron
    subdivide(top, bottom_right, bottom_left, 5);
    
    //for the rest of the sides for the top half of the octohedron
    //rotate the face 90 degrees and then subdivide it into the
    //smaller triangles as well
    for (int i = 0; i < 3; i++){

        bottom_left = matrix_vector_multiplication(rotate_y(90), bottom_left);
        bottom_right = matrix_vector_multiplication(rotate_y(90), bottom_right);

        subdivide(top, bottom_right, bottom_left, 5);
    }
    
    //triangle for bottom pyramid of octohedron
    vec4 bottom = {0.0, -1.0, 0.0, 1.0};
    vec4 upper_right = {1.0, 0.0, 0.0, 1.0};
    vec4 upper_left = {0.0, 0.0, 1.0, 1.0};
    
    subdivide(bottom, upper_left, upper_right, 5);
    
    //same process for the bottom half
    for (int i = 0; i < 3; i++){
    
        upper_left = matrix_vector_multiplication(rotate_y(90), upper_left);
        upper_right = matrix_vector_multiplication(rotate_y(90), upper_right);

        subdivide(bottom, upper_left, upper_right, 5);
    }
}






void init(void)
{
    //time_t t;
     //srand((unsigned)time(&t));
     //cone();
    sphere();
    table_fun();
    normal_vec();
    GLuint program = initShader("/Users/liangyilun/Desktop/cs1566/proj1/proj1/vshader_ctm.glsl", "/Users/liangyilun/Desktop/cs1566/proj1/proj1/fshader_ctm.glsl");
    glUseProgram(program);

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

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    glDrawArrays(GL_TRIANGLES, 0, 100000);

    glutSwapBuffers();
}

vec4 random_color_generator(){
    
    GLfloat random_red = (GLfloat) rand() / RAND_MAX;
    GLfloat random_green = (GLfloat) rand() / RAND_MAX;
    GLfloat random_blue = (GLfloat) rand() / RAND_MAX;
    vec4 result = {random_red, random_green, random_blue, 1.0};
    return result;
  
    
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
        ctm = matrix_matrix_multiplication(ctm, matrix_arb);
        //cube_ctm[[[i]]] = ctm;
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


mat4 ori = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};
void keyboard(unsigned char key, int mousex, int mousey)
{
    printf("key: %i\n", key);
    if(key == 'q')
    	exit(0);

        
    else if(key == 'i'){
        ctm = scaling(0.98);
        
    }
    else if(key == 'o'){
        ctm = scaling(1.02);
    }
    else if(key == 'r'){
         ctm = matrix_matrix_multiplication(ctm, rotate_y(10));Z
        ori = ctm;
        ctm = matrix_matrix_multiplication(ctm, translate(1, 1, 1));
       
       
               
    }
    else if(key == 'd'){
         ctm = ori;
    }
    else if (key == 'u'){
        ctm = matrix_matrix_multiplication(rotate_x(9.0), ctm);
    }
    glutPostRedisplay();
}

void idle(void)
{
    if(isGoingRight)
    {
	x_value += 0.05;
	if(fabs(x_value - 1.0) < 0.000001)
	{
	    x_value = 1.0;
	    isGoingRight = 0;
	}
    }
    else
    {
	x_value -= 0.05;
	if(fabs(x_value - (-1.0)) < 0.000001)
	{
	    x_value = -1.0;
	    isGoingRight = 1;
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
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
