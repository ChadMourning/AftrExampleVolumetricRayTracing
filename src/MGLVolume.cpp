#include "MGLVolume.h"
#include "GLSLShader.h"
#include "Camera.h"
using namespace Aftr;

const unsigned int TEXTURE_WIDTH = 16, TEXTURE_HEIGHT = 16, TEXTURE_DEPTH = 16;

MGLVolume* MGLVolume::New(WO* parent)
{
   MGLVolume* ptr = new MGLVolume(parent);
   ptr->onCreate();
   return ptr;
}

MGLVolume::MGLVolume(WO* parent) : MGL(parent)
{

}

void MGLVolume::onCreate()
{
   //generate desnity map, switch to noise function later
   GLfloat* data = new GLfloat[TEXTURE_WIDTH * TEXTURE_HEIGHT * TEXTURE_DEPTH * 4];
   for (int i = 0; i < TEXTURE_WIDTH; i++)
      for (int j = 0; j < TEXTURE_HEIGHT; j++)
         for(int k = 0; k < TEXTURE_DEPTH; k++)
            for(int l = 0; l < 4; l++)
            data[i * (4 * TEXTURE_HEIGHT * TEXTURE_DEPTH) +
                 j * (4 * TEXTURE_DEPTH) +
                 k * 4 +
                 l] = rand() % 256;
   
   glGenTextures(1, &densityTexture);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_3D, densityTexture);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   //other
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_DEPTH, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, data);


   //make vertex buffer

   //make index buffer

   glGenBuffers(1, &vertexBuffer);
   glGenBuffers(1, &indexBuffer);

   GLfloat vertices[] = { -1, -1, -1,//-z
                        1, -1, -1,
                        1, 1, -1,
                        1, -1, -1,
                        1, 1, -1,
                        -1, 1, -1,
                        -1, -1, 1,//+z
                        1, -1, 1,
                        1, 1, 1,
                        1, -1, 1,
                        1, 1, 1,
                        -1, 1, 1,
     1, -1, -1,//+x
     1,  1, -1,
     1,  1, 1,
     1,  1, -1,
     1,  1, 1,
     1,  -1, 1,
     -1,  -1, -1,//-x
     -1,  1, -1,
     -1,  1, 1,
     -1, 1, -1,
     -1,  1, 1,
     -1,  -1, 1,
     -1,1, -1,//+y
      1,1, -1,
      1,1, 1,
      1,1, -1,
      1,1, 1,
      -1,1, 1,
     -1,-1, -1,//-y
      1,-1, -1,
      1,-1, 1,
      1,-1, -1,
      1,-1, 1,
      -1,-1, 1 };

   Vector scale = { 100,200,300 };
   for (int i = 0; i < 8; i++)
   {
      vertices[i * 3 + 0] *= scale.x;
      vertices[i * 3 + 1] *= scale.y;
      vertices[i * 3 + 2] *= scale.z;
   }

   GLubyte indices[24];
   for (int i = 0; i < 24; i++)
      indices[i] = i;

   //send interleaved data for patches
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), (void*) vertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLubyte), (void*) indices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   GLuint vertexLocation = ManagerShader::getDefaultAttributeLocationVertexPosition();

   glGenVertexArrays(1, &vaoHandle);
   glBindVertexArray(vaoHandle);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

   GLuint stride = 3 * sizeof(GLfloat);
   glEnableVertexAttribArray(vertexLocation);
   glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);

   glBindVertexArray(0);
   
}

void MGLVolume::render(const Camera& cam)
{
   //bind skin
   Mat4 modelMatrix = this->getModelMatrix(); ///< Returns the full 4x4 model matrix used by a shader, including orientation, position, scale.
   Mat4 normalMatrix = cam.getCameraViewMatrix() * this->dispMatrix; //for transforming normals in the shader

   std::tuple< const Mat4&, const Mat4&, const Camera& > shaderParams(modelMatrix, normalMatrix, cam);
   this->getSkin().bind(&shaderParams);
   glDisable(GL_BLEND);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_3D, densityTexture);

   glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, nullptr);
   

   this->getSkin().unbind();
}

void MGLVolume::update(GLSLShader* compute)
{
/*
   glBindImageTexture(0, (counter % 2 == 0) ? texture : texture2, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
   glBindImageTexture(1, (counter % 2 == 0) ? texture2 : texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
   compute->bind();
   glDispatchCompute(TEXTURE_WIDTH, TEXTURE_HEIGHT, 1);
   glMemoryBarrier(GL_ALL_BARRIER_BITS);
   ++counter;
   */
}