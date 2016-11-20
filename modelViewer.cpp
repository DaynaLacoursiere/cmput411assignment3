
// modelViewer <mesh.obj>

// CMPUT 411/511 Assignment 1 solution

#include <Eigen/Dense>

#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

#include "camera.h"
#include "mesh.h"

using namespace Eigen;


int main(int, char**);
void setup(char*);
void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);


/* global variables */
static unsigned int texture[2]; // Array of texture ids.
static int id = 0; // Currently displayed texture id.
mesh obj;
camera cam;
Vector3f initialPosition(0.0, 0.0, -1.0);
bool fog = true;
const float fogColor[4] = {0.0, 0.0, 0.0, 0.0};


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("modelViewer");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	if (argc != 2) {
		cerr << "Usage: modelViewer <meshfile.obj>" << endl;
		exit(1);
	}
	setup(argv[1]); 
	glutMainLoop(); 
	return 0;  
}

void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1];
   
   // Load the image.
   image[0] = obj.getbmp("../a3files/mesh/skin.bmp"); 
   
   // Create texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 

   // Specify image data for currently active texture object.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void setup(char* fileName)
{
	obj.readObjFile(fileName);
	obj.normalize();
	obj.createVertexArray();
	obj.glCreateDisplayList();
	// obj.glEnableVAO();

	loadExternalTextures();

  // Specify how texture values combine with current surface color values.
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 


	cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

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
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


}


void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.glVolume(); // camera view volume

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  float matAmbAndDif1[] = {0.0, 0.0, 0.0, 1.0};
  float matSpec[] = {1.0, 1.0, 1.0, 1.0};
  float matShine[] = {50.0};

  // Material properties of the box.
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);


  // Activate texture object.
  glBindTexture(GL_TEXTURE_2D, texture[id]);


	cam.glPosition(); // camera transformation
	obj.glPosition(initialPosition); // object transformation

	if (fog) { // set fog
		glEnable(GL_FOG);
		glHint(GL_FOG_HINT, GL_NICEST);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogi(GL_FOG_START, 1.0);
		glFogi(GL_FOG_END, 5.0);
	} else
		glDisable(GL_FOG);

	// draw model
	obj.glColor();
	obj.glCallDisplayList();
	// obj.glDrawVAO();
	glutSwapBuffers();
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
		case 'w': obj.writeObjFile("meshout.obj"); break;
		case 'n': obj.zTransl(-0.1);  break;
		case 'N': obj.zTransl(0.1);   break;
		case 'p': obj.xRotate(-10.0); break;
		case 'P': obj.xRotate(10.0);  break;
		case 'y': obj.yRotate(-10.0); break;
		case 'Y': obj.yRotate(10.0);  break;
		case 'r': obj.zRotate(-10.0); break;
		case 'R': obj.zRotate(10.0);  break;
		case 's': obj.toggleDisplay(); break;
		case 'f': fog = false;        break; // toggle fog off
		case 'F': fog = true;         break; // toggle fog on
		case 'x': // reset
			obj.reset();
			cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
			fog = false;
			break;
		default: break;
	}
	cam.keyInput(key); // camera controls

	glutPostRedisplay();
}


void specialKeyInput(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_LEFT:  obj.xTransl(-0.1); break;
		case GLUT_KEY_RIGHT: obj.xTransl(0.1);  break;
		case GLUT_KEY_DOWN:  obj.yTransl(-0.1); break;
		case GLUT_KEY_UP:    obj.yTransl(0.1);  break;
		default: break;
	}
	glutPostRedisplay();
}


