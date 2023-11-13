#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

layout(push_constant) uniform params_t
{
    mat4 mProjView;
    mat4 mModel;
} params;


layout(location = 0) in VS_OUT
{
    vec3 wPos;
    vec3 wNorm;
    vec3 wTangent;
    vec2 texCoord;
} gIn[];

layout(location = 0) out VS_OUT
{
    vec3 wPos;
    vec3 wNorm;
    vec3 wTangent;
    vec2 texCoord;

} gOut;

const float EPS = 0.01;
const float NORM_LENGTH = 0.05;

void main ()
{
    for ( int i = 0; i < gl_in.length (); i++ )
    {
        gOut.wPos     = gIn[i].wPos;
        gOut.wNorm    = gIn[i].wNorm;
        gOut.wTangent = gIn[i].wTangent;
        gOut.texCoord = gIn[i].texCoord;
        gl_Position   = params.mProjView * vec4(gIn[i].wPos, 1.0);
        EmitVertex();
    }

    EndPrimitive();

    vec3 norm = (gIn[0].wNorm + gIn[1].wNorm + gIn[2].wNorm) / 3.0;
    vec3 centerW = vec3(
        (gIn[0].wPos.x + gIn[1].wPos.x + gIn[2].wPos.x) / 3.0,
        (gIn[0].wPos.y + gIn[1].wPos.y + gIn[2].wPos.y) / 3.0,
        (gIn[0].wPos.z + gIn[1].wPos.z + gIn[2].wPos.z) / 3.0
    );
    vec3 normEndW = centerW + gIn[0].wNorm * NORM_LENGTH;

    gl_Position = params.mProjView * vec4(centerW, 1.0);
    EmitVertex();

    gl_Position = params.mProjView * vec4(centerW + vec3(EPS), 1.0);
    EmitVertex();

    gl_Position = params.mProjView * vec4(normEndW, 1.0);
    EmitVertex();

    EndPrimitive();
}