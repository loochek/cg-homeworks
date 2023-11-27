#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0 ) out VS_OUT
{
  vec2 texCoord;
} vOut;

void main()
{
  if (gl_VertexIndex == 0)
  {
    gl_Position   = vec4(-1.0f, -1.0f, 0.0f, 1.0f);
    vOut.texCoord = vec2(0.0f, 0.0f);
  }
  else if (gl_VertexIndex == 1 || gl_VertexIndex == 3)
  {
    gl_Position   = vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    vOut.texCoord = vec2(0.0f, 1.0f);
  }
  else if (gl_VertexIndex == 2 || gl_VertexIndex == 4)
  {
    gl_Position   = vec4(1.0f, -1.0f, 0.0f, 1.0f);
    vOut.texCoord = vec2(1.0f, 0.0f);
  }
  else
  { 
    gl_Position   = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    vOut.texCoord = vec2(1.0f, 1.0f);
  }
}
