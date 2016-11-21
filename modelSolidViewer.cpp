
// modelViewer <mesh.obj>

// CMPUT 411/511 Assignment 1 solution

#include <Eigen/Dense>

#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>

#include "camera.h"
#include "mesh.h"
#include "light.h"
#include "texture.h"

using namespace Eigen;


int main(int, char**);
void setup(char*);
void drawScene(void);
void resize(int, int);
void keyInput(unsigned char, int, int);
void specialKeyInput(int, int, int);


/* global variables */
mesh obj;
camera cam;
light light;
texture text;
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
	glutCreateWindow("modelSolidViewer");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	if (argc != 2) {
		cerr << "Usage: modelSolidViewer <meshfile.obj>" << endl;
		exit(1);
	}
	setup(argv[1]); 
	glutMainLoop(); 
	return 0;  
}

void setup(char* fileName)
{
	obj.readObjFile(fileName);
	obj.normalize();
	obj.createVertexArray();
	obj.glCreateDisplayList();
	// obj.glEnableVAO();

	// Local storage for bmp image data.
  BitMapFile *image[1];
   
  // Load the image.
  image[0] = obj.getbmp("skin.bmp"); 
	
	text.loadExternalTextures(image[0]);
	text.textEnv();


	cam.initialize(persp, -0.1, 0.1, -0.1, 0.1, 0.1, 100.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	light.setup();
	
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

  text.bindText();

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
		case 's': text.toggleDisplay(); break;
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


