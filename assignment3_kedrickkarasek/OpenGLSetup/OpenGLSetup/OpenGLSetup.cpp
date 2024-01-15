#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define PI 3.141592

#include <iostream>
#define std

#define BODY_WIDTH 2
#define BODY_HEIGHT 4
#define BODY_DEPTH 2

// Camera Varaibles
static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;
GLint leftMouseButton, rightMouseButton; //status of the mouse buttons
int mouseX = 0, mouseY = 0; //last known X and Y of the mouse
float cameraTheta, cameraPhi, cameraRadius; //camera position in spherical coordinates
float x, y, z; //camera position in cartesian coordinates


// Robot Joints
static float upperAngle = 0, lowerAngle = -10;
static float leftArmAngle = 0, rightArmAngle = 0;

static float circleMove = 0;

static float lineMove = 0;

static float wave = 0;

bool displayWireMesh = false;

bool displayRobot = true;

bool displayCoordinateLines = true;

// Robot Colors
float redHeadRobot = 0.0f;
float greenHeadRobot = 1.0f;
float blueHeadRobot = 0.0f;

float redBodyRobot = 1.0f;
float greenBodyRobot = 0.0f;
float blueBodyRobot = 0.0f;

float redArmOrLeg = 0.0f;
float greenArmOrLeg = 0.0f;
float blueArmOrLeg = 1.0f;


// Animation Variables
bool playAnimation = true;
bool visiblePath = true;
bool pathEnabled = true;
bool resetOn = false;
bool circleAnimationPlaying = true;
bool lineAnimationPlaying = false;

int aniSeconds = 0;
int aniState = 0;

// wireBox(w, h, d) makes a wireframe box with width w, height h and
// depth d centered at the origin. It uses the GLUT wire cube function.
// The calls to glPushMatrix and glPopMatrix are essential here; they enable
// this function to be called from just about anywhere and guarantee that
// the glScalef call does not pollute code that follows a call to myWireBox.
void drawBox(GLdouble width, GLdouble height, GLdouble depth, float red, float green, float blue) {
	if (displayWireMesh)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glScalef(width, height, depth);
		glutWireCube(1.0);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glColor3f(red, green, blue);
		glScalef(width, height, depth);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}

void drawSphere(GLdouble width, GLdouble height, GLdouble depth, float red, float green, float blue) {
	if (displayWireMesh)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		glScalef(width, height, depth);
		glutWireSphere(1.0f, 10, 10);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glColor3f(red, green, blue);
		glScalef(width, height, depth);
		glutSolidSphere(1.0f, 10, 10);
		glPopMatrix();
	}
}

// Draw Circle
void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	GLint numberOfVertices = numberOfSides + 1;

	GLfloat doublePi = 2.0f * PI;

	GLfloat circleVerticesX[20];
	GLfloat circleVerticesY[20];
	GLfloat circleVerticesZ[20];

	for (int i = 0; i < numberOfVertices; i++)
	{
		circleVerticesX[i] = x + (radius * cos(i * doublePi / numberOfSides));
		circleVerticesY[i] = y + (radius * sin(i * doublePi / numberOfSides));
		circleVerticesZ[i] = z;
	}

	GLfloat allCircleVertices[20 * 3];

	for (int i = 0; i < numberOfVertices; i++)
	{
		allCircleVertices[i * 3] = circleVerticesX[i];
		allCircleVertices[(i * 3) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 3) + 2] = circleVerticesZ[i];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, allCircleVertices);
	glDrawArrays(GL_LINE_STRIP, 0, numberOfVertices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawRobot() {

	//head
	glPushMatrix();
	glTranslatef(0, 2.9f, 0);
	drawSphere(1.0, 1.0, 1.0, redHeadRobot, greenHeadRobot, blueHeadRobot);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	drawBox(BODY_WIDTH, BODY_HEIGHT, BODY_DEPTH, redBodyRobot, greenBodyRobot, blueBodyRobot);
	glPopMatrix();

	//right arm
	//upper
	glPushMatrix();
	glTranslatef(1.0f, 1.5f, 0);
	glRotatef((GLfloat)-85, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)-upperAngle, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg);

	//lower
	glTranslatef(1.0f, 0, 0);
	glRotatef((GLfloat)-lowerAngle, 0.0, 1.0, 0.0);
	glTranslatef(1.0f, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg); // Lower arm
	glPopMatrix();

	//left arm
	//upper
	glPushMatrix();
	glTranslatef(-1.0f, 1.5f, 0);
	glRotatef((GLfloat)+85, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)-upperAngle, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg);

	//lower
	glTranslatef(-1.0f, 0, 0);
	glRotatef((GLfloat)+lowerAngle, 0.0, 1.0, 0.0);
	glTranslatef(-1.0f, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg); // Lower arm
	glPopMatrix();

	//right leg
	//upper
	glPushMatrix();
	glTranslatef(.5, -1.5, 0);
	glRotatef((GLfloat)-90, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)+upperAngle, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg);

	//lower
	glTranslatef(1.0f, 0, 0);
	glRotatef((GLfloat)+lowerAngle, 0.0, 1.0, 0.0);
	glTranslatef(1.0f, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg); // Lower arm
	glPopMatrix();

	//left leg
	//upper
	glPushMatrix();
	glTranslatef(-.5, -1.5, 0);
	glRotatef((GLfloat)-90, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)-upperAngle, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg);

	//lower
	glTranslatef(1.0f, 0, 0);
	glRotatef((GLfloat)+lowerAngle, 0.0, 1.0, 0.0);
	glTranslatef(1.0f, 0, 0);
	drawBox(2.0, 0.4, 1.0, redArmOrLeg, greenArmOrLeg, blueArmOrLeg); // Lower arm
	glPopMatrix();

}

void drawBear() {
	
	//head
	glPushMatrix();
	glTranslatef(0, 3, 0);
	drawSphere(1.0, 1.0, 1.0, 150, 75, 0);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	drawSphere(1.5, 2.0, 1.0, 150, 75, 0);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glTranslatef(1, 1.3, 0);
	glRotatef((GLfloat)-rightArmAngle, 0.0, 0.0, 1.0);
	glTranslatef(1, 0, 0);
	drawSphere(1.5, 0.5, 0.5, 150, 75, 0);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glTranslatef(-1.2, 1.7, 0);
	glRotatef((GLfloat)+85, 0.0, 0.0, 1.0);
	glTranslatef(-1.2, 0, 0);
	drawSphere(1.5, 0.5, 0.5, 150, 75, 0);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glTranslatef(.5, -1, 0);
	drawSphere(.5, .5, .5, 150, 75, 0);
	glPopMatrix();

	//left leg
	glPushMatrix();
	glTranslatef(-.5, -1, 0);
	drawSphere(.5, .5, .5, 150, 75, 0);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(.5, 3.5, 0);
	drawSphere(.5, .5, .5, 150, 75, 0);
	glPopMatrix();

	//left ear
	glPushMatrix();
	glTranslatef(-.5, 3.5, 0);
	drawSphere(.5, .5, .5, 150, 75, 0);
	glPopMatrix();
	
}

void coordinateLines() { //draw red x-axis, green y-axis, blue-zaxis

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
	glEnd();
}

void drawScene()
{
	glPushMatrix();
	glTranslatef(0.0f, -5.0f, 0.0f);
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();
	glPopMatrix();

	glPushMatrix();

	//if circle animation is playing
	if (displayRobot && circleAnimationPlaying)
	{

		glPushMatrix();
		glTranslatef(0, 0, 0);
		glRotatef(circleMove -.7, 0.0, 1.0, 0.0);
		glTranslatef(8, 0, 0);
		drawRobot();
		glPopMatrix();


		if (pathEnabled) {
			glPushMatrix();
			glRotatef(90, 1.0, 0, 0);
			drawCircle(0, 0, 0, 10, 19);
			glPopMatrix();
		}
	}

	//if line animation is playing
	if (displayRobot && lineAnimationPlaying)
	{

		glPushMatrix();
		glTranslatef(lineMove, 0, 0);
		glRotatef(90,0,2.0,0);
		glTranslatef(0, 0, 0);
		drawRobot();
		glPopMatrix();

		if (pathEnabled) {
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_LINES);
			glColor3f(.5, .5, 1); glVertex3f(0, 0, 0); glVertex3f(-11, 0, 0);
			glEnd();
			//draw line
		}

	}

	if (!displayRobot) {
		glPushMatrix();
		drawBear();
		glPopMatrix;
	}

	if (displayCoordinateLines) {
		glPushMatrix();
		coordinateLines();
		glPopMatrix();
	}

	glPopMatrix();
}


// Displays the arm in its current position and orientation. The whole
// function is bracketed by glPushMatrix and glPopMatrix calls because every
// time we call it we are in an "environment" in which a gluLookAt is in
// effect. (Note that in particular, replacing glPushMatrix with
// glLoadIdentity makes you lose the camera setting from gluLookAt).
void display(void) {

	//update the modelview matrix based on the camera's position
	glMatrixMode(GL_MODELVIEW); //make sure we aren't changing the projection matrix!
	glLoadIdentity();
	gluLookAt(x, y, z, //camera is located at (x,y,z)
		0, 0, 0, //camera is looking at (0,0,0)
		0.0f, 1.0f, 0.0f); //up vector is (0,1,0) (positive Y)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glScalef(0.3, 0.3, 0.3);
	drawScene();

	glutSwapBuffers();
}

// Handles the reshape event by setting the viewport so that it takes up the
// whole visible region, then sets the projection matrix to something reason-
// able that maintains proper aspect ratio.
void resizeWindow(int w, int h)
{
	aspectRatio = w / (float)h;
	windowWidth = w;
	windowHeight = h;
	//update the viewport to fill the window
	glViewport(0, 0, w, h);
	//update the projection matrix with the new window properties
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspectRatio, 0.1, 10);
}

// Perfroms application specific initialization: turn off smooth shading,
// sets the viewing transformation once and for all. In this application we
// won't be moving the camera at all, so it makes sense to do this.
void init() {

	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 2, 8, 0, 0, 0, 0, 1, 0);
}

void recomputeOrientation()
{
	x = cameraRadius * sinf(cameraTheta) * sinf(cameraPhi);
	z = cameraRadius * cosf(cameraTheta) * sinf(cameraPhi);
	y = cameraRadius * cosf(cameraPhi);
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////////
// GLUT callback for mouse clicks. We save the state of the mouse button
// so when this is called so that we can check the status of the mouse
// buttons inside the motion callback (whether they are up or down).
/////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY)
{
	// Update the left and right mouse button states, if applicable
	if (button == GLUT_LEFT_BUTTON)
		leftMouseButton = state;
	else if (button == GLUT_RIGHT_BUTTON)
		rightMouseButton = state;
	// and update the last seen X and Y coordinates of the mouse
	mouseX = thisX;
	mouseY = thisY;
}

//////////////////////////////////////////////////////////////////////////////
// GLUT callback for mouse movement. We update cameraPhi, cameraTheta and/or
// cameraRadius based on how much the user has moved the mouse in the X or Y
// directions (in screen space) and whether they have held down the left or
// right mouse buttons. If the user hasn't held down any buttons, then the 
// function just updates the last seen mouse X and Y coordinates.
//////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y)
{
	if (leftMouseButton == GLUT_DOWN)
	{
		cameraTheta += (mouseX - x) * 0.005;
		cameraPhi += (mouseY - y) * 0.005;
		// Make sure that phi stays within the range (0, PI)
		if (cameraPhi <= 0)
			cameraPhi = 0 + 0.001;
		if (cameraPhi >= PI)
			cameraPhi = PI - 0.001;
		recomputeOrientation(); // Update camera (x, y, z)
	}
	// Camera zoom in/out
	else if (rightMouseButton == GLUT_DOWN)
	{
		double totalChangeSq = (x - mouseX) + (y - mouseY);

		cameraRadius += totalChangeSq * 0.01;

		// Limit the camera radius to some reasonable values so the user can't get lost
		if (cameraRadius < 2.0)
			cameraRadius = 2.0;
		if (cameraRadius > 10.0)
			cameraRadius = 10.0;
		recomputeOrientation(); // Update camera (x, y, z) based on (radius, theta, phi)
	}
	mouseX = x;
	mouseY = y;
}

void timer(int v)
{

		if (playAnimation)
		{
			if (aniSeconds <= 30 && aniState == 0)
			{
				upperAngle += 1;
				circleMove += 1;
				lineMove -= .2;
				rightArmAngle += 1;
				aniSeconds++;
				if (aniSeconds >= 30) {
					aniState = 1;
				}
			}
			else
			{
				upperAngle -= 1;
				circleMove += 1;
				rightArmAngle -= 1;
				aniSeconds--;
				lineMove = 0;
				if (aniSeconds <= -30)
					aniState = 0;
			}
		}

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, v);
}

void resetAnimation() {
	aniSeconds = 0;
	aniState = 0;
	circleMove = 0;
	lineMove = 0;
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case'1':
		displayWireMesh = true;
		break;
	case'2':
		displayWireMesh = false;
		break;
	case'3':
		displayCoordinateLines = !displayCoordinateLines;
		break;
	case'4':
		pathEnabled = !pathEnabled;
		break;
	case'r':
		resetAnimation();
		//need to figure out how to reset the animation (specificly what values)
		break;
	case'a':
		playAnimation = !playAnimation;
		break;
	case'p':
		circleAnimationPlaying = !circleAnimationPlaying;
		lineAnimationPlaying = !lineAnimationPlaying;
		break;
	case't':
		displayRobot = !displayRobot;
		break;
	}
	if (key == 27) // escape key
		exit(0);
	glutPostRedisplay();
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Animation Example With Circle");

	printf("\n\
-----------------------------------------------------------------------------------\n\
		1: display a wireframe model of the robot and scene\n\
		2 : display a solid model of the robot and the scene\n\
		3 : toggle to on / off the three axes(axes only)\n\
		4 : robot walking path visible toggle ON / OFF\n\
		r : move the robot to the initial position to be animated\n\
		a : animation walking toggle ON / OFF(animation only)\n\
		p : walking path options of the robot \n\
		t : toggle teddy bear on or off \n\
		ESC : terminate the program \n\
------------------------------------------------------------------------------------\n");

	// Init depth checking
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	cameraRadius = 7.0f;
	cameraTheta = 2.80;
	cameraPhi = 1.0;
	recomputeOrientation();

	// Register callbacks
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutReshapeFunc(resizeWindow);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(processNormalKeys);
	init();
	glutMainLoop();
}