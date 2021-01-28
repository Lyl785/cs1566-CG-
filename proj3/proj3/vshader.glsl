#version 120

attribute vec4 vPosition;
attribute vec4 vNormal;
varying vec4 color;
uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;
uniform mat4 ctm;
uniform vec4 light_position;
uniform vec4 ambient_product;
uniform vec4 diffuse_product;
uniform vec4 specular_product;
uniform int is_shadow;
uniform float shininess;
uniform float attenuation_constant;
uniform float attenuation_linear;
uniform float attenuation_quadratic;
vec4 ambient, diffuse, specular;

void main()
{
    
    if (is_shadow == 0){
        ambient = ambient_product;
    
        vec4 N = normalize(model_view_matrix * ctm * vNormal);
    
        vec4 L_temp = model_view_matrix * (light_position - ctm * vPosition);
        vec4 L = normalize(L_temp);
        diffuse = max(dot(L,N), 0.0) * diffuse_product;
    
        vec4 eye_point = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 V = normalize(eye_point - (projection_matrix * model_view_matrix *vPosition));
        vec4 H = normalize(L + V);
        specular = pow(max(dot(N, H), 0.0), shininess) * specular_product;
        
        float distance = length(L_temp);
        float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) +
        (attenuation_quadratic * distance * distance));
        gl_Position = projection_matrix * model_view_matrix * ctm * vPosition;
        color = ambient + (diffuse + specular);
        
    } else {
        
        vec4 newPosition = ctm * vPosition;
        
        float x = light_position.x - (light_position.y * ((light_position.x - newPosition.x)/(light_position.y - newPosition.y)));
        
        float z = light_position.z - (light_position.y * ((light_position.z - newPosition.z)/(light_position.y - newPosition.y)));
        
        gl_Position = projection_matrix * model_view_matrix * vec4(x, 0.01, z, 1);
        color = vec4(0, 0, 0, 1);
    }
}

