#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// Actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;

// XZ position of the camera
float x = 0.0f, z = 5.0f;

// All variables initialized to 1.0, meaning 
// the triangle will initially be white
bool color = true;
float red = 1.0f, blue = 1.0f, green = 1.0f;

// Angle for rotating triangle
float angle = 0.0f;

bool wireFrameRobot = false;

bool robot = true;

bool clear = false;

bool showAxis = true;

bool solidbear = false;

bool wiredbear = false;

static int shoulderAngle = 0, elbowAngle = 0;

static int upperLegAngle = 0, lowerLegAngle = 0;

static int rightArmAngle = 0, leftArmAngle = 0;

void wireBox(GLdouble width, GLdouble height, GLdouble depth)
{
	glPushMatrix();
	glScalef(width, height, depth);
	glutWireCube(1);
	glPopMatrix();
}

void solidBox(GLdouble width, GLdouble height, GLdouble depth)
{
	glPushMatrix();
	glScalef(width, height, depth);
	glutSolidCube(1);
	glPopMatrix();
}

void wiresphere(GLdouble radius = 1, GLdouble sliceStack = 10, GLdouble width = 1,GLdouble height = 1, GLdouble depth = 1 )
{
	glPushMatrix();
	glScalef(width, height, depth);
	glutWireSphere(radius, sliceStack, sliceStack);
	glPopMatrix();
}

void solidsphere(GLdouble radius = 1, GLdouble sliceStack = 10, GLdouble width = 1, GLdouble height = 1, GLdouble depth = 1)
{
	glPushMatrix();
	glScalef(width, height, depth);
	glutSolidSphere(radius, sliceStack, sliceStack);
	glPopMatrix();
}

void solidRobot() {
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);


	//head
	glPushMatrix();
	glTranslatef(0, 1.35, 0);
	solidsphere(.25);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	solidBox(.50, .75, .50);
	glPopMatrix();
	
	//right arm
	//upper
	glPushMatrix();
	glTranslatef(.25,1,0);
	glRotatef((GLfloat)shoulderAngle, 0.0, 0.0, 1.0);
	glTranslatef(.1, 0, 0);
	solidBox(.25, .1, .1);

	//lower
	glTranslatef(.1, 0, 0);
	glRotatef((GLfloat)elbowAngle, 0.0, 0.0, 1.0);
	glTranslatef(.1,0,0);
	solidBox(.25, .1, .1);
	glPopMatrix();

	//left arm
	//upper
	glPushMatrix();
	glTranslatef(-.25, 1, 0);
	glRotatef(-(GLfloat)shoulderAngle, 0.0, 0.0, 1.0);
	glTranslatef(-.1, 0, 0);
	solidBox(.25, .1, .1);

	//lower
	glTranslatef(-.1, 0, 0);
	glRotatef(-(GLfloat)elbowAngle, 0.0, 0.0, 1.0);
	glTranslatef(-.1, 0, 0);
	solidBox(.25, .1, .1);
	glPopMatrix();

	//right leg
	//upper
	glPushMatrix();
	glTranslatef(.16, .40, 0);
	glRotatef((GLfloat)upperLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	solidBox(.15, .30, .15);

	//lower
	glTranslatef(0, -.15, 0);
	glRotatef((GLfloat)lowerLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	solidBox(.15, .30, .15);
	glPopMatrix();


	//left leg
	//upper
	glPushMatrix();
	glTranslatef(-.16, .40, 0);
	glRotatef(-(GLfloat)upperLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	solidBox(.15, .30, .15);

	//lower
	glTranslatef(0, -.15, 0);
	glRotatef(-(GLfloat)lowerLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	solidBox(.15, .30, .15);
	glPopMatrix();
	
	
}

void wireframeRobot() {
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);

	//head
	glPushMatrix();
	glTranslatef(0, 1.35, 0);
	wiresphere(.25);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	wireBox(.50, .75, .50);
	glPopMatrix();

	//right arm
	//upper
	glPushMatrix();
	glTranslatef(.25, 1, 0);
	glRotatef((GLfloat)shoulderAngle, 0.0, 0.0, 1.0);
	glTranslatef(.1, 0, 0);
	wireBox(.25, .1, .1);

	//lower
	glTranslatef(.1, 0, 0);
	glRotatef((GLfloat)elbowAngle, 0.0, 0.0, 1.0);
	glTranslatef(.1, 0, 0);
	wireBox(.25, .1, .1);
	glPopMatrix();

	//left arm
	//upper
	glPushMatrix();
	glTranslatef(-.25, 1, 0);
	glRotatef(-(GLfloat)shoulderAngle, 0.0, 0.0, 1.0);
	glTranslatef(-.1, 0, 0);
	wireBox(.25, .1, .1);

	//lower
	glTranslatef(-.1, 0, 0);
	glRotatef(-(GLfloat)elbowAngle, 0.0, 0.0, 1.0);
	glTranslatef(-.1, 0, 0);
	wireBox(.25, .1, .1);
	glPopMatrix();

	//right leg
	//upper
	glPushMatrix();
	glTranslatef(.16, .40, 0);
	glRotatef((GLfloat)upperLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	wireBox(.15, .30, .15);

	//lower
	glTranslatef(0, -.15, 0);
	glRotatef((GLfloat)lowerLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	wireBox(.15, .30, .15);
	glPopMatrix();


	//left leg
	//upper
	glPushMatrix();
	glTranslatef(-.16, .40, 0);
	glRotatef(-(GLfloat)upperLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	wireBox(.15, .30, .15);

	//lower
	glTranslatef(0, -.15, 0);
	glRotatef(-(GLfloat)lowerLegAngle, 0, 0, 1.0);
	glTranslatef(0, -.15, 0);
	wireBox(.15, .30, .15);
	glPopMatrix();
}

void solidTeddyBear() {
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);
	//head
	glPushMatrix();
	glTranslatef(0, 1.35, 0);
	solidsphere(.25);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	solidsphere(.50, 10, .5);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glTranslatef(.1, 1, 0);
	glRotatef((GLfloat)rightArmAngle, 0.0, 0.0, 1.0);
	glTranslatef(.2, 0, 0);
	solidsphere(.25, 10, 1, .5);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glTranslatef(-.1, 1, 0);
	glRotatef(-(GLfloat)leftArmAngle, 0.0, 0.0, 1.0);
	glTranslatef(-.2, 0, 0);
	solidsphere(.25, 10, 1, .5);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glTranslatef(.15, .25,0);
	solidsphere(.15,10);
	glPopMatrix();

	//left leg
	glPushMatrix();
	glTranslatef(-.15, .25,0);
	solidsphere(.15, 10);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(.15, 1.55, 0);
	solidsphere(.1, 10);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(-.15, 1.55, 0);
	solidsphere(.1, 10);
	glPopMatrix();
}

void wiredTeddyBear() {
	if (color)
		glColor3f(red, green, blue);
	else
		glColor3f(1.0, 1.0, 1.0);

	//head
	glPushMatrix();
	glTranslatef(0, 1.35, 0);
	wiresphere(.25);
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0.0f, 0.75f, 0.0f);
	wiresphere(.50, 10, .5);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glTranslatef(.1, 1, 0);
	glRotatef((GLfloat)rightArmAngle, 0.0, 0.0, 1.0);
	glTranslatef(.2, 0, 0);
	wiresphere(.25, 10, 1, .5);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glTranslatef(-.1, 1, 0);
	glRotatef(-(GLfloat)leftArmAngle, 0.0, 0.0, 1.0);
	glTranslatef(-.2, 0, 0);
	wiresphere(.25, 10, 1, .5);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glTranslatef(.15, .25, 0);
	wiresphere(.15, 10);
	glPopMatrix();

	//left leg
	glPushMatrix();
	glTranslatef(-.15, .25, 0);
	wiresphere(.15, 10);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(.15, 1.55, 0);
	wiresphere(.1, 10);
	glPopMatrix();

	//right ear
	glPushMatrix();
	glTranslatef(-.15, 1.55, 0);
	wiresphere(.1, 10);
	glPopMatrix();
	
}

void coordinateLines() { //draw red x-axis, green y-axis, blue-zaxis

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(2, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 2, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 2);
	glEnd();
}

// To reshape the display when the window size is changed by the user
void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width).
	if (h == 0) h = 1;
	float ratio = w * 1.0 / h;
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset Matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the correct perspective
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	// Get back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);
	// Draw the ground (a plane) 
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.f);
	glVertex3f(-100.0f, 0.0f, 100.f);
	glVertex3f(100.0f, 0.0f, 100.f);
	glVertex3f(100.0f, 0.0f, -100.f);
	glEnd();
	
	// Draw 36 of whatever
	for (int i = -3; i < 3; i++)
		for (int j = -3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i * 10.0, 0, j * 10.0);
			if (!wireFrameRobot && !clear && !solidbear && !wiredbear)
			{
				glPushMatrix();
				glTranslatef(0, .2, 0);
				solidRobot();
				glPopMatrix();
			}

			if (wireFrameRobot && !clear && !solidbear && !wiredbear)
			{
				glPushMatrix();
				glTranslatef(0,.2,0);
				wireframeRobot();
				glPopMatrix();
			}
			if (showAxis && !clear) {
				glPushMatrix();
				glTranslatef(0, 1, 0);
				coordinateLines();
				glPopMatrix();
			}
			if (solidbear && !wireFrameRobot && !clear && !wiredbear)
				solidTeddyBear();

			if (!wireFrameRobot && !clear && wiredbear)
				wiredTeddyBear();
			glPopMatrix();
		}
	glFlush();
	glutSwapBuffers(); // For double buffer display
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case'1':
		if (robot && !solidbear) {
			wireFrameRobot = true;
		}
		if (solidbear && !robot) {
			wiredbear = true;
		}
		break;
	case'2':
		wireFrameRobot = false;
		wiredbear = false;
		break;
	case'3':
		showAxis = !showAxis;
		break;
	case'4':
		(elbowAngle += 5) %= 360;
		break;
	case'5':
		(shoulderAngle += 5) %= 360;
		break;
	case'6':
		(upperLegAngle += 5) %= 360;
		break;
	case'7':
		(lowerLegAngle += 5) %= 360;
		break;
	case'8':
		(rightArmAngle += 5) %= 360;
		break;
	case'9':
		(leftArmAngle += 5) %= 360;
		break;
	case't':
		solidbear = !solidbear;
		robot = !robot;
		break;
	}
	if (key == 27) // escape key
		exit(0);
	glutPostRedisplay();
}

// Change the camera looking direction and its position with the keyboard input
void processSpecialKeys(int key, int xx, int yy)
{
	float fraction = 0.1f;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		break;
	case GLUT_KEY_F4:
		(elbowAngle -= 5) %= 360;
		break;
	case GLUT_KEY_F5:
		(shoulderAngle -= 5) %= 360;
		break;
	case GLUT_KEY_F6:
		(upperLegAngle -= 5) %= 360;
		break;
	case GLUT_KEY_F7:
		(lowerLegAngle -= 5) %= 360;
		break;
	case GLUT_KEY_F8:
		(rightArmAngle -= 5) %= 360;
		break;
	case GLUT_KEY_F9:
		(leftArmAngle -= 5) %= 360;
		break;
	}
}

int main(int argc, char** argv)
{
	printf("\n\
----------------------------------------------------------------------------\n\
Controls for the program are as follows: \n\
1  : display a wireframe (mesh only) model\n\
2  : display a solid model\n\
3  : toggle to on / off the three axes\n\
4  : increase the elbow angle by 5 degrees for both arms(the left and right arms)\n\
F4 : decrease the elbow angle by 5 degrees for both arms(the left and right arms)\n\
5  : increase the shoulder angle by 5 degrees for both arms(the left and right arms)\n\
F5 : decrease the shoulder angle by 5 degrees for both arms(left and right arms)\n\
6  : increase the angles of the upper leg joints by 5 degrees for both legs \n\
F6 : decrease the angles of the upper leg joints by 5 degrees for both legs\n\
7  : increase the angles of the lower leg joints by 5 degrees for both legs\n\
F7 : decrease the angles of the lower leg joints by 5 degrees for both legs\n\
8  : increase the angles of the Teddy Bears right arm by 5 degrees for both legs \n\
F8 : decrease the angles of the Teddy Bears right arm by 5 degrees for both legs\n\
9  : increase the angles of the Teddy Bears left arm by 5 degrees for both legs\n\
F9 : decrease the angles of the Teddy Bears left arm by 5 degrees for both legs\n\
ESC: terminate the program\n\
----------------------------------------------------------------------------\n");
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Snowman Example");
	// Register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	// OpenGL init
	glEnable(GL_DEPTH_TEST); // Enable hidden surface removal
	// Enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}