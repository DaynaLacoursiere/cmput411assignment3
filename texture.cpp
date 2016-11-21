#include "texture.h"

using namespace std;


void texture::loadExternalTextures(BitMapFile * image)         
{   
  // Create texture object texture[0]. 
  glBindTexture(GL_TEXTURE_2D, text[0]); 

  // Specify image data for currently active texture object.
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->sizeX, image->sizeY, 0, 
                GL_RGBA, GL_UNSIGNED_BYTE, image->data);

  // Set texture parameters for wrapping.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Set texture parameters for filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void texture::toggleDisplay()
{
  int i = displayType + 1;
  if (i == 4)
    displayType = 0;
  else
    displayType = i;

  switch(displayType)
  {
    case 0:
    // Wire Frame
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    break;

    case 1:
    // Flat shaded
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    break;

    case 2:
    // Smooth Shaded
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    break;

    case 3:
    // Texture mapped
    glEnable(GL_TEXTURE_2D);
    break;
  }
}