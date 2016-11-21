
// motionViewer <motion.bvh>

#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

#include "skeleton.h"
#include "motion.h"
#include "timer.h"
#include "camera.h"
#include "mesh.h"
#include "light.h"
#include "texture.h"
#include "animation.h"
#include "attachment.h"


int main(int, char**);
void setup(char*,char*,char*);
void drawScene(void);
void idle(void);
void resize(int, int);
void keyInput(unsigned char, int, int);


/* global variables */
mesh obj;
camera cam;
skeleton skel;
motion mot;
timer tim;
light light;
texture text;
animation anim;
attachment att(&obj, &skel);
float boneRadii[] = {
  0.5, 2.0, 1.0, 0.5, 0.25, 0.0,
  0.5, 2.0, 1.0, 0.5, 0.25, 0.0,
  0.5, 2.0, 1.5, 1.5, 0.5, 0.5, 0.5,
  2.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.25, 0.0, 0.0,
  2.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.25, 0.0, 0.0
};
int highlightBone(0);
bool highlightMode = false;
Vector3f initialPosition(0.0, 0.0, -1.0);
bool interpolate(true);


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitContextVersion(4, 2);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("personViewer");
  glutDisplayFunc(drawScene);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);
  glutIdleFunc(idle);
  if (argc != 4) {
    cerr << "Usage: personViewer <meshfile.obj> <motionfile.bvh> <attachfile.att>" << endl;
    exit(1);
  }
  setup(argv[1], argv[2], argv[3]); 
  glutMainLoop();
  return 0;
}


void setup(char *meshFile, char *motionFile, char *attachFile) 
{

  // mesh
  obj.readObjFile(meshFile);
  // obj.normalize();
  obj.createVertexArray();
  obj.glCreateDisplayList();

  // texture
  // Local storage for bmp image data.
  BitMapFile *image[1];
   
  // Load the image.
  image[0] = obj.getbmp("../a3files/mesh/skin.bmp"); 
  
  text.loadExternalTextures(image[0]);
  text.textEnv();

  // skeleton
  skel.readBvhSkeleton(motionFile);
  skel.determineRadius();
  skel.recoverBones();

  att.distancesVisibility(boneRadii);
  att.readW(attachFile);

  // motion
  mot.readBvhMotion(motionFile, skel);
  mot.determineRange(skel.translationIndices);

  // timer
  tim.initialize(true, mot.sequence.size(), mot.defaultGapTime);

  // camera
  cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
  cam.positionMotion(mot.range, skel.radius);

  // light
  light.setup();

  // animation
  anim.readAttFile(attachFile);

  // gl
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // camera view volume
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  cam.glVolume(); 

}


void drawScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // camera
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 
  cam.glPosition();

  // skeleton
  skel.glColor();
  skel.interpolatePose(&mot, tim.loopFrac, interpolate);
  skel.glDraw();

  

  // texture
  text.bindText();

  // mesh
  // obj.glPosition(initialPosition); // object transformation

  // draw model
  obj.glColor();
  obj.glCallDisplayList();

  glutSwapBuffers();
}


void idle(void)
{
  tim.glIdle();
}


void resize(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
}


void keyInput(unsigned char key, int x, int y)
{
  switch(key) 
  {
    case 'q': exit(0);            break; // quit
    case 'w': // write bvh file
      skel.writeBvhSkeleton("skeletonout.bvh");
      mot.writeBvhMotion("skeletonout.bvh");
      obj.writeObjFile("meshout.obj");
      anim.writeAttFile("attachout.att");
      break;
    case 'n': obj.zTransl(-0.1);  break;
    case 'N': obj.zTransl(0.1);   break;
    case 'p': obj.xRotate(-10.0); break;
    case 'P': obj.xRotate(10.0);  break;
    case 'y': obj.yRotate(-10.0); break;
    case 'Y': obj.yRotate(10.0);  break;
    case 'r': obj.zRotate(-10.0); break;
    case 'R': obj.zRotate(10.0);  break;
    case 's': text.toggleDisplay(); break;
    case 'x': // reset
      obj.reset();
      tim.initialize(true, mot.sequence.size(), mot.defaultGapTime);
      cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 200.0);
      cam.positionMotion(mot.range, skel.radius);
      break;
    default: break;
  }
  cam.keyInput(key); // camera controls
  tim.keyInput(key); // timer controls

  glutPostRedisplay();
}
