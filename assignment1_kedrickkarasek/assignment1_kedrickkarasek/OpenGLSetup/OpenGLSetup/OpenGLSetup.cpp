#include <GL/glut.h>
#define WIN_X 100
#define WIN_Y 100
#define WIN_H 600 // in pixels
#define WIN_W 600

bool color = true;
float red = 1.0;
float green = 0.0;
float blue = 0.0;

float xRotation;
float yRotation;
float zRotation;
float rotationAngle;

bool wireFrameRobot = false;

bool clear = false;

bool showAxis = true;

bool solidbear = false;

bool wiredbear = false;

float cameraOrth = 2.0;

bool perspective = false;

//Models / Coordinate Lines
void solidRobot() {
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);

	//body
	glPushMatrix();
	glutSolidCube(4);
	glPopMatrix();

	//head
	glPushMatrix();
	glTranslatef(0,3.9,0);
	glutSolidSphere(2, 10, 10);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glScalef(3, 1, 1);
	glTranslatef(1.15, .7, 0);
	glutSolidCube(1);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glScalef(3, 1, 1);
	glTranslatef(-1.15, .7, 0);
	glutSolidCube(1);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScalef(1, 5, 1);
	glTranslatef(1, -.8, 0);
	glutSolidCube(1);
	glPopMatrix();
	
	//left leg
	glPushMatrix();
	glScalef(1, 5, 1);
	glTranslatef(-1, -.8, 0);
	glutSolidCube(1);
	glPopMatrix();

}

void wireframeRobot(){
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);

	//body
	glPushMatrix();
	glutWireCube(4);
	glPopMatrix();

	//head
	glPushMatrix();
	glTranslatef(0, 3.9, 0);
	glutWireSphere(2, 10, 10);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glScalef(3, 1, 1);
	glTranslatef(1.15, .7, 0);
	glutWireCube(1);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glScalef(3, 1, 1);
	glTranslatef(-1.15, .7, 0);
	glutWireCube(1);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScalef(1, 5, 1);
	glTranslatef(1, -.8, 0);
	glutWireCube(1);
	glPopMatrix();

	//left leg
	glPushMatrix();
	glScalef(1, 5, 1);
	glTranslatef(-1, -.8, 0);
	glutWireCube(1);
	glPopMatrix();
}

void solidTeddyBear(){
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);

	//body
	glPushMatrix();
	glScalef(1.2, 1.5 , 1.2);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	//head
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	//left ear
	glPushMatrix();
	glTranslatef(-.5, 3, 0);
	glutSolidSphere(.4, 10, 10);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(.5, 3, 0);
	glutSolidSphere(.4, 10, 10);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glScalef(1, .4, .4);
	glTranslatef(1.2, 1, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glScalef(1, .4, .4);
	glTranslatef(-1.2, 1, 0);
	glutSolidSphere(1, 10, 10);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScalef(.4, 1, .4);
	glTranslatef(1.5, -1.5, 0);
	glutSolidSphere(1,10, 10);
	glPopMatrix();
	
	//left leg
	glPushMatrix();
	glScalef(.4, 1, .4);
	glTranslatef(-1.5, -1.5, 0);
	glutSolidSphere(1,10,10);
	glPopMatrix();
	
}

void wiredTeddyBear() {
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);

	//body
	glPushMatrix();
	glScalef(1.2, 1.5, 1.2);
	glutWireSphere(1, 10, 10);
	glPopMatrix();

	//head
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glutWireSphere(1, 10, 10);
	glPopMatrix();

	//left ear
	glPushMatrix();
	glTranslatef(-.5, 3, 0);
	glutWireSphere(.4, 10, 10);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(.5, 3, 0);
	glutWireSphere(.4, 10, 10);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glScalef(1, .4, .4);
	glTranslatef(1.2, 1, 0);
	glutWireSphere(1, 10, 10);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glScalef(1, .4, .4);
	glTranslatef(-1.2, 1, 0);
	glutWireSphere(1, 10, 10);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glScalef(.4, 1, .4);
	glTranslatef(1.5, -1.5, 0);
	glutWireSphere(1, 10, 10);
	glPopMatrix();

	//left leg
	glPushMatrix();
	glScalef(.4, 1, .4);
	glTranslatef(-1.5, -1.5, 0);
	glutWireSphere(1, 10, 10);
	glPopMatrix();

}

void coordinateLines() { //draw red x-axis, green y-axis, blue-zaxis

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(10, 0, 0);
		glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 10, 0);
		glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 10);
		glEnd();
}

//Menu stuff
void mymenu(int id) {
}

void changeXRotation(int id) {
	switch (id) {
	case 1:
		xRotation = 1.0;
		yRotation = 0.0;
		zRotation = 0.0;
		rotationAngle = 30;
		glutPostRedisplay();
		break;
	case 2:
		xRotation = 1.0;
		yRotation = 0.0;
		zRotation = 0.0;
		rotationAngle = 60;
		glutPostRedisplay();
		break;
	case 3:
		xRotation = 1.0;
		yRotation = 0.0;
		zRotation = 0.0;
		rotationAngle = 90;
		glutPostRedisplay();
		break;
	case 4:
		xRotation = 1.0;
		yRotation = 0.0;
		zRotation = 0.0;
		rotationAngle = 120;
		glutPostRedisplay();
		break;
	case 5:
		xRotation = 1.0;
		yRotation = 0.0;
		zRotation = 0.0;
		rotationAngle = 180;
		glutPostRedisplay();
		break;
	}
}

void changeYRotation(int id) {
	switch (id) {
	case 1:
		xRotation = 0.0;
		yRotation = 1.0;
		zRotation = 0.0;
		rotationAngle = 30;
		glutPostRedisplay();
		break;
	case 2:
		xRotation = 0.0;
		yRotation = 1.0;
		zRotation = 0.0;
		rotationAngle = 60;
		glutPostRedisplay();
		break;
	case 3:
		xRotation = 0.0;
		yRotation = 1.0;
		zRotation = 0.0;
		rotationAngle = 90;
		glutPostRedisplay();
		break;
	case 4:
		xRotation = 0.0;
		yRotation = 1.0;
		zRotation = 0.0;
		rotationAngle = 120;
		glutPostRedisplay();
		break;
	case 5:
		xRotation = 0.0;
		yRotation = 1.0;
		zRotation = 0.0;
		rotationAngle = 180;
		glutPostRedisplay();
		break;
	}
}

void changeZRotation(int id){
	switch (id) {
	case 1:
		xRotation = 0.0;
		yRotation = 0.0;
		zRotation = 1.0;
		rotationAngle = 30;
		glutPostRedisplay();
		break;
	case 2:
		xRotation = 0.0;
		yRotation = 0.0;
		zRotation = 1.0;
		rotationAngle = 60;
		glutPostRedisplay();
		break;
	case 3:
		xRotation = 0.0;
		yRotation = 0.0;
		zRotation = 1.0;
		rotationAngle = 90;
		glutPostRedisplay();
		break;
	case 4:
		xRotation = 0.0;
		yRotation = 0.0;
		zRotation = 1.0;
		rotationAngle = 120;
		glutPostRedisplay();
		break;
	case 5:
		xRotation = 0.0;
		yRotation = 0.0;
		zRotation = 1.0;
		rotationAngle = 180;
		glutPostRedisplay();
		break;
	}
}

void changeView(int id) {
	switch (id) {
	case 1: //perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-cameraOrth, cameraOrth, -cameraOrth, cameraOrth, 5, 100);
		perspective = true;
		glutPostRedisplay();
		break;
	case 2: //orthogonal
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-cameraOrth, cameraOrth, -cameraOrth, cameraOrth, 5, 100);
		perspective = false;
		glutPostRedisplay();
		break;
	}
}

void changeColor(int id) {
	switch (id) {
	case 1:
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		glutPostRedisplay();
		break;
	case 2:
		red = 0.0;
		green = 1.0;
		blue = 0.0;
		glutPostRedisplay();
		break;
	case 3:
		red = 0.0;
		green = 0.0;
		blue = 1.0;
		glutPostRedisplay();
		break;
	}
}

void solidMenu(int id) {
	switch (id) {
	case 1:
		if (wiredbear) {
			wiredbear = false;
		}
		solidbear = !solidbear;
		glutPostRedisplay();
		break;
	}
}

void wiredMenu(int id) {
	switch (id) {
	case 1:
		if (solidbear) {
			solidbear = false;
		}
		wiredbear = !wiredbear;
		glutPostRedisplay();
		break;
	}
}

void resetModels(int id) {
	switch (id) {
	case 1:
		wiredbear = false;
		solidbear = false;
		wireFrameRobot = false;
		glutPostRedisplay();
	}
}

void createMenu() {

	int xRotation = glutCreateMenu(changeXRotation);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);
	glutAddMenuEntry("120", 4);
	glutAddMenuEntry("180", 5);
	
	int yRotation = glutCreateMenu(changeYRotation);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);
	glutAddMenuEntry("120", 4);
	glutAddMenuEntry("180", 5);

	int zRotation = glutCreateMenu(changeZRotation);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);
	glutAddMenuEntry("120", 4);
	glutAddMenuEntry("180", 5);

	int rotationMenu = glutCreateMenu(mymenu);
	glutAddSubMenu("X", xRotation);
	glutAddSubMenu("Y", yRotation);
	glutAddSubMenu("Z", zRotation);

	int viewMenu = glutCreateMenu(changeView);
	glutAddMenuEntry("Perspective", 1);
	glutAddMenuEntry("Orthogonal", 2);

	int bodyColorMenu = glutCreateMenu(changeColor);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);

	int sExtraModelMenu = glutCreateMenu(solidMenu);
	glutAddMenuEntry("Teddy Bear", 1);

	int wExtraModelMenu = glutCreateMenu(wiredMenu);
	glutAddMenuEntry("Teddy Bear", 1);

	int modelReset = glutCreateMenu(resetModels);
	glutAddMenuEntry("Reset to Default", 1);

	int modelMenu = glutCreateMenu(mymenu);
	glutAddSubMenu("Solid", sExtraModelMenu);
	glutAddSubMenu("Wired", wExtraModelMenu);
	glutAddSubMenu("Reset Model", modelReset);

	glutCreateMenu(mymenu);
	glutAddSubMenu("Rotations", rotationMenu);
	glutAddSubMenu("View", viewMenu);
	glutAddSubMenu("Body Color", bodyColorMenu);
	glutAddSubMenu("Extra Models", modelMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//Key and mouse input
void procKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': //display a wireframe (mesh only) model
		wireFrameRobot = true;
		glutPostRedisplay();
		break;
	case 's': //display solid model
		wireFrameRobot = false;
		glutPostRedisplay();
		break;
	case 'c': //clear the screen (black background)
		clear = true;
		glutPostRedisplay();
		break;
	case 'm': //display the model
		clear = false;
		glutPostRedisplay();
		break;
	case 'a': //display axis (toggle on/off) ///// issue here with trying to hide the axis and show the axis doesnt always show
		showAxis = !showAxis;
		glutPostRedisplay();
		break;
	case 27: //escape
		exit(0);
	}
}

void procMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		color = !color;
		glutPostRedisplay();
	}

	//scroll wheel
	if (!perspective) {
		if (button == 3) {
			if (state == GLUT_UP) {
				cameraOrth -= 0.5;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-cameraOrth, cameraOrth, -cameraOrth, cameraOrth, 5, 100);
				glMatrixMode(GL_MODELVIEW);
				glutPostRedisplay();
			}
		}
		if (button == 4)
		{
			if (state != GLUT_UP)
			{
				cameraOrth += 0.5;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-cameraOrth, cameraOrth, -cameraOrth, cameraOrth, 5, 100);
				glMatrixMode(GL_MODELVIEW);
				glutPostRedisplay();
			}
		}
	}

	if (perspective) {
		if (state == GLUT_UP) {
			cameraOrth -= 0.5;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glFrustum(-cameraOrth, cameraOrth, -cameraOrth, cameraOrth, 5, 100);
			glMatrixMode(GL_MODELVIEW);
			glutPostRedisplay();
		}

		if (button == 4)
		{
			if (state != GLUT_UP)
			{
				cameraOrth += 0.5;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glFrustum(-cameraOrth, cameraOrth, -cameraOrth, cameraOrth, 5, 100);
				glMatrixMode(GL_MODELVIEW);
				glutPostRedisplay();
			}
		}
	}

	}

//Display
void mydisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	glRotatef(rotationAngle, xRotation, yRotation, zRotation);


	if(!wireFrameRobot && !clear && !solidbear && !wiredbear)
		solidRobot();
	
	if (wireFrameRobot && !clear && !solidbear && !wiredbear)
		wireframeRobot();

	if (showAxis && !clear)
		coordinateLines();

	if (solidbear && !wireFrameRobot && !clear && !wiredbear)
		solidTeddyBear();

	if (!solidbear && !wireFrameRobot && !clear && wiredbear)
		wiredTeddyBear();

	glFlush();
	glutSwapBuffers();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, 5, 100);
	glMatrixMode(GL_MODELVIEW); //camera is a physical object in the scene
	glLoadIdentity();
	//postion camera at (0,0,20) looking at (0,0,0) with the up vector <0,1,0>
	gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
}

//Main
int main(int argc, char** argv) {
	
	glutInit(&argc, argv); //initalize
	glutInitDisplayMode(GLUT_RGB); // Sets display mode (single buffer with rbg)
	glutInitWindowSize(WIN_W, WIN_H); // window size of 600 x 600
	glutInitWindowPosition(WIN_X, WIN_Y); // window position of (100,100)
	glutCreateWindow("Kedrick Karasek , 811106848"); //open window with title 

	init();

	createMenu();

	glutDisplayFunc(mydisplay);

	//proc stuff 
	glutKeyboardFunc(procKeys);
	glutMouseFunc(procMouse);
	//end of proc stuff


	glutMainLoop();
	return 0;
}