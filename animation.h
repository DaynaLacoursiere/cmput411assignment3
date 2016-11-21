
#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>
#include <cmath>
#include <vector>
#include <Eigen/Dense>


#ifndef ANIMATION_H
#define ANIMATION_H

using namespace std;


struct animation
{
  vector< vector< pair< unsigned int, float > > > attachments;

  void readAttFile(char*);
  void writeAttFile(char*);
};

#endif
