
#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

#ifndef LIGHT_H
#define LIGHT_H

using namespace std;


struct light
{
  void setup()
  {
    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);

    // Light property vectors.
    float lightAmb[] = {0.0, 0.0, 0.0, 1.0};
    float lightDifAndSpec[] = {1.0, 1.0, 1.0, 1.0};
    float lightPos[] = {0.0, 1.5, 3.0, 1.0};
    float globAmb[] = {0.2, 0.2, 0.2, 1.0};

    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
     
    glEnable(GL_LIGHT0); // Enable particular light source.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }


};

#endif
