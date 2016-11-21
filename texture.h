
#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>
#include "mesh.h"

#ifndef TEXTURE_H
#define TEXTURE_H

using namespace std;


struct texture
{
  unsigned int text[1];
  int displayType;
  void setup();
  void loadExternalTextures(BitMapFile * image);
  void toggleDisplay();
  void textEnv()
  {
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  }
  void bindText()
  {
    // Activate texture object.
    glBindTexture(GL_TEXTURE_2D, text[0]);
  }

};

#endif
