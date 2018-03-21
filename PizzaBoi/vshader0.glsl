#version 150

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;
uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;

void main() 
{ 

  gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;
  color = vColor;
 

}
