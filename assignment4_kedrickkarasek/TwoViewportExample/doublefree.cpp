#include "point.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define BODY_WIDTH 2
#define BODY_HEIGHT 4
#define BODY_DEPTH 2

using namespace std;

// GLOBAL VARIABLES ////////////////////////////////////////////////////////////

static size_t windowWidth = 640;
static size_t windowHeight = 480;
static float aspectRatio;

GLint leftMouseButton, rightMouseButton;    //status of the mouse buttons
int mouseX = 0, mouseY = 0;                 //last known X and Y of the mouse
bool sphereOn = false;                      //show the camera radius sphere


//note to students reading this code:
//  yes, I should really be more object-oriented with this code.
//  a lot of this would be simplified and better encapsulated inside
//  of a Camera class. don't let your code get this ugly!
enum cameraList { CAMERA_INNER = 0, CAMERA_OUTER = 1 };
enum cameraList currentCamera = CAMERA_OUTER;

#define USING_INNER (currentCamera == CAMERA_INNER)

float M_PI = 3.141592;

Point outerCamTPR;
Point outerCamXYZ;

Point innerCamXYZ;
Point innerCamTPR;
Point innerCamDir;

//stuff for the draw functions
typedef struct
{
    int X;
    int Y;
    int Z;
    double U;
    double V;
}VERTICES;

const double PI = 3.1415926535897;
const int space = 11;
const int VertexCount = (90 / space) * (360 / space) * 4;
VERTICES VERTEX[VertexCount];

//robot stuff
// Robot Joints
static float upperAngle = 0, lowerAngle = -10;
static float leftArmAngle = 0, rightArmAngle = 0;

bool wireFrameActive = false;

bool cordinateLinesActive = true;

// Animation Variables
bool playAnimation = true;
bool visiblePath = true;
bool pathEnabled = true;
bool resetOn = false;
bool circleAnimationPlaying = true;
bool lineAnimationPlaying = false;

int aniSeconds = 0;
int aniState = 0;

static float lineMove = 0;
static float wave = 0;

bool displayRobot = true;


//function for drawing a Box
static void QuadBox(GLfloat size, GLenum type)
{
    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(type);
        glNormal3fv(&n[i][0]);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(&v[faces[i][0]][0]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(&v[faces[i][1]][0]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(&v[faces[i][2]][0]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
}

//function for drawing Sphere
void CreateSphere(double R, double H, double K, double Z) {
    int n;
    double a;
    double b;
    n = 0;
    for (b = 0; b <= 90 - space; b += space) {

        for (a = 0; a <= 360 - space; a += space)
        {
            VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
            VERTEX[n].V = (2 * b) / 360;
            VERTEX[n].U = (a) / 360;

            n++;
            VERTEX[n].X = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
            VERTEX[n].V = (2 * (b + space)) / 360;
            VERTEX[n].U = (a) / 360;
            n++;
            VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b) / 180 * PI) - Z;
            VERTEX[n].V = (2 * b) / 360;
            VERTEX[n].U = (a + space) / 360;
            n++;
            VERTEX[n].X = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
            VERTEX[n].Y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
            VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
            VERTEX[n].V = (2 * (b + space)) / 360;
            VERTEX[n].U = (a + space) / 360;
            n++;
        }
    }
}

//for texture to be applied to sphere
void DisplaySphere(double R)
{

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int b;
    glScalef(0.0125 * R, 0.0125 * R, 0.0125 * R);
    //glRotatef (90, 1, 0, 0);
    glBegin(GL_TRIANGLE_STRIP);
    for (b = 0; b < VertexCount; b++)
    {
        glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
        glVertex3f(VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
    }



    for (b = 0; b < VertexCount; b++)
    {

        glTexCoord2f(VERTEX[b].U, -VERTEX[b].V);

        glVertex3f(VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);

    }

    glEnd();
}

//draws the sphere
void DrawSphere(double size)
{
    if (!wireFrameActive) {
        CreateSphere(size, 0, 0, 0);
        DisplaySphere(5);
    }

    if (wireFrameActive) {
        glPushMatrix();
        glutWireSphere(size, 10, 10);
        glPopMatrix();
    }
}

//draws the box
void Drawbox(double w, double h, double l) {
    if (!wireFrameActive) {
        glPushMatrix();
        glScalef(w, h, l);
        QuadBox(1, GL_QUADS);
        glPopMatrix();
    }

    if (wireFrameActive) {
        glPushMatrix();
        glScalef(w, h, l);
        glutWireCube(1);
        glPopMatrix();
    }

}

void drawRobot() {

    //head
    glPushMatrix();
    glTranslatef(0, 2.9f, 0);
    DrawSphere(1.0);
    glPopMatrix();

    //body
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    Drawbox(BODY_WIDTH, BODY_HEIGHT, BODY_DEPTH);
    glPopMatrix();

    //right arm
    //upper
    glPushMatrix();
    glTranslatef(1.0f, 1.5f, 0);
    glRotatef((GLfloat)-85, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)-upperAngle, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0, 0);
    Drawbox(2.0, 0.4, 1.0);

    //lower
    glTranslatef(1.0f, 0, 0);
    glRotatef((GLfloat)-lowerAngle, 0.0, 1.0, 0.0);
    glTranslatef(1.0f, 0, 0);
    Drawbox(2.0, 0.4, 1.0); // Lower arm
    glPopMatrix();

    //left arm
    //upper
    glPushMatrix();
    glTranslatef(-1.0f, 1.5f, 0);
    glRotatef((GLfloat)+85, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)-upperAngle, 0.0, 1.0, 0.0);
    glTranslatef(-1.0, 0, 0);
    Drawbox(2.0, 0.4, 1.0);

    //lower
    glTranslatef(-1.0f, 0, 0);
    glRotatef((GLfloat)+lowerAngle, 0.0, 1.0, 0.0);
    glTranslatef(-1.0f, 0, 0);
    Drawbox(2.0, 0.4, 1.0); // Lower arm
    glPopMatrix();

    //right leg
    //upper
    glPushMatrix();
    glTranslatef(.5, -1.5, 0);
    glRotatef((GLfloat)-90, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)+upperAngle, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0, 0);
    Drawbox(2.0, 0.4, 1.0);

    //lower
    glTranslatef(1.0f, 0, 0);
    glRotatef((GLfloat)+lowerAngle, 0.0, 1.0, 0.0);
    glTranslatef(1.0f, 0, 0);
    Drawbox(2.0, 0.4, 1.0); // Lower arm
    glPopMatrix();

    //left leg
    //upper
    glPushMatrix();
    glTranslatef(-.5, -1.5, 0);
    glRotatef((GLfloat)-90, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)-upperAngle, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0, 0);
    Drawbox(2.0, 0.4, 1.0);

    //lower
    glTranslatef(1.0f, 0, 0);
    glRotatef((GLfloat)+lowerAngle, 0.0, 1.0, 0.0);
    glTranslatef(1.0f, 0, 0);
    Drawbox(2.0, 0.4, 1.0); // Lower arm
    glPopMatrix();
    
}

//draw function for cordinate lines
void coordinateLines() { //draw red x-axis, green y-axis, blue-zaxis
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(5, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 5);
    glEnd();
}


// recomputeOrientation() //////////////////////////////////////////////////////
//
// This function updates the camera's position in cartesian coordinates based 
//  on its position in spherical coordinates. Should be called every time 
//  either camera's spherical coordinates are updated.
//
////////////////////////////////////////////////////////////////////////////////
void recomputeOrientation(Point& xyz, Point& tpr)
{
    xyz.x = tpr.z * sinf(tpr.x) * sinf(tpr.y);
    xyz.z = tpr.z * -cosf(tpr.x) * sinf(tpr.y);
    xyz.y = tpr.z * -cosf(tpr.y);
    glutPostRedisplay();
}

// resizeWindow() //////////////////////////////////////////////////////////////
//
//  GLUT callback for window resizing. Resets GL_PROJECTION matrix and viewport.
//
////////////////////////////////////////////////////////////////////////////////
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
    gluPerspective(45.0, aspectRatio, 0.1, 100000);

    glutPostRedisplay();
}



// mouseCallback() /////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
void mouseCallback(int button, int state, int thisX, int thisY)
{
    //update the left and right mouse button states, if applicable
    if (button == GLUT_LEFT_BUTTON)
        leftMouseButton = state;
    else if (button == GLUT_RIGHT_BUTTON)
        rightMouseButton = state;

    //and update the last seen X and Y coordinates of the mouse
    mouseX = thisX;
    mouseY = thisY;
}

// mouseMotion() ///////////////////////////////////////////////////////////////
//
//  GLUT callback for mouse movement. We update the current camera's spherical
//      coordinates based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
void mouseMotion(int x, int y)
{
    if (leftMouseButton == GLUT_DOWN)
    {
        Point* curTPR = (USING_INNER ? &innerCamTPR : &outerCamTPR);      //just for conciseness below
        curTPR->x += (x - mouseX) * 0.005;
        curTPR->y += (USING_INNER ? -1 : 1) * (y - mouseY) * 0.005;

        // make sure that phi stays within the range (0, M_PI)
        if (curTPR->y <= 0)
            curTPR->y = 0 + 0.001;
        if (curTPR->y >= M_PI)
            curTPR->y = M_PI - 0.001;

        //update camera (x,y,z) based on (radius,theta,phi)
        if (USING_INNER)
        {
            recomputeOrientation(innerCamDir, innerCamTPR);
            innerCamDir.normalize();
        }
        else {
            recomputeOrientation(outerCamXYZ, outerCamTPR);
        }
    }
    else if (rightMouseButton == GLUT_DOWN && !USING_INNER) {
        double totalChangeSq = (x - mouseX) + (y - mouseY);

        Point* curTPR = &outerCamTPR;      //just for conciseness below
        curTPR->z += totalChangeSq * 0.01;

        //limit the camera radius to some reasonable values so the user can't get lost
        if (curTPR->z < 2.0)
            curTPR->z = 2.0;
        if (curTPR->z > 10.0 * (currentCamera + 1))
            curTPR->z = 10.0 * (currentCamera + 1);

        //update camera (x,y,z) based on (radius,theta,phi)
        recomputeOrientation(outerCamXYZ, outerCamTPR);
    }

    mouseX = x;
    mouseY = y;
}



// initScene() /////////////////////////////////////////////////////////////////
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//
////////////////////////////////////////////////////////////////////////////////
void initScene()
{
    glEnable(GL_DEPTH_TEST);

    float lightCol[4] = { 1, 1, 1, 1 };
    float ambientCol[4] = { 0.3, 0.3, 0.3, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_POINT_SMOOTH);

    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);

    glutPostRedisplay();
}


// drawSceneElements() /////////////////////////////////////////////////////////
//
//  Because we'll be drawing the scene twice from different viewpoints,
//      we encapsulate the code to draw the scene here, so that we can just
//      call this function twice once the projection and modelview matrices
//      have been set appropriately.
//
////////////////////////////////////////////////////////////////////////////////
void drawSceneElements(void)
{
    glDisable(GL_LIGHTING);
    //draw a simple grid under the teapot
    glColor3f(1, 1, 1);
    for (int dir = 0; dir < 2; dir++)
    {
        for (int i = -5; i < 6; i++)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = -5; j < 6; j++)
                glVertex3f(dir < 1 ? i : j, -0.73f, dir < 1 ? j : i);
            glEnd();
        }
    }

    //and then draw the teapot itself!
    glEnable(GL_LIGHTING);

    //see documentation for glutSolidTeapot; glutSolidTeapot must be called with 
    //a different winding set. there is a known 'bug' that results in the 
    //winding of the teapot to be backwards.
    glFrontFace(GL_CW);
    //draw robot here
    //glutSolidTeapot(1.0f);
    if (displayRobot && lineAnimationPlaying)
    {

        glPushMatrix();
        glTranslatef(lineMove, 0, 0);
        glRotatef(90, 0, 2.0, 0);
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

    if (cordinateLinesActive) {
        coordinateLines();
    }
    glFrontFace(GL_CCW);

}


// drawInnerCamera() ///////////////////////////////////////////////////////////
//
//  Draws a representation of the inner camera in space. This should only be
//      called when rendering the scene from the POV of the outer camera,
//      so that we can visualize where the inner camera is positioned
//      and what it is looking at.
//
////////////////////////////////////////////////////////////////////////////////
void drawInnerCamera()
{
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z);
    glRotatef(-innerCamTPR.x * 180.0 / M_PI, 0, 1, 0);
    glRotatef(innerCamTPR.y * 180.0 / M_PI, 1, 0, 0);
    glScalef(1, -2, 0.75);


    glColor3f(0, 1, 0);
    glutWireCube(1.0f);

    //draw the reels on top of the camera...
    for (int currentReel = 0; currentReel < 2; currentReel++)
    {
        float radius = 0.25f;
        int resolution = 32;
        Point reelCenter = Point(0, -0.25 + (currentReel == 0 ? 0 : 0.5), -0.5);
        glBegin(GL_LINES);
        Point s = reelCenter - Point(0, 0.25, 0);
        glVertex3f(s.x, s.y, s.z);
        for (int i = 0; i < resolution; i++)
        {
            float ex = -cosf(i / (float)resolution * M_PI);
            float why = sinf(i / (float)resolution * M_PI);
            Point p = Point(0, ex * radius, -why * radius * 3) + reelCenter;
            glVertex3f(p.x, p.y, p.z);  //end of this line...
            glVertex3f(p.x, p.y, p.z);  //and start of the next
        }
        Point f = reelCenter + Point(0, 0.25, 0);
        glVertex3f(f.x, f.y, f.z);
        glEnd();
    }

    //and just draw the lens shield manually because 
    //i don't want to think about shear matrices.
    //clockwise looking from behind the camera:
    float lensOff = 0.3f;
    float lensOut = 0.2f;
    Point v0 = Point(0.5, 0.5, -0.5);
    Point v1 = Point(-0.5, 0.5, -0.5);
    Point v2 = Point(-0.5, 0.5, 0.5);
    Point v3 = Point(0.5, 0.5, 0.5);

    Point l0 = v0 + Point(lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, -lensOff);
    Point l1 = v1 + Point(-lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, -lensOff);
    Point l2 = v2 + Point(-lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, lensOff);
    Point l3 = v3 + Point(lensOut, 0, 0) + Point(0, lensOut, 0) + Point(0, 0, lensOff);


    glBegin(GL_LINE_STRIP);
    l0.glVertex();
    l1.glVertex();
    l2.glVertex();
    l3.glVertex();
    l0.glVertex();
    glEnd();

    //and connect the two
    glBegin(GL_LINES);
    v0.glVertex();  l0.glVertex();
    v1.glVertex();  l1.glVertex();
    v2.glVertex();  l2.glVertex();
    v3.glVertex();  l3.glVertex();
    glEnd();



    glPopAttrib();
}

// renderCallback() ////////////////////////////////////////////////////////////
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a teapot to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//
////////////////////////////////////////////////////////////////////////////////
void renderCallback(void)
{
    //clear the render buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float borderWidth = 3;
    //start with the code from the outer camera, which covers the whole screen!
    glViewport(0, 0, windowWidth, windowHeight);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);    glPushMatrix(); glLoadIdentity();   gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    if (currentCamera == CAMERA_OUTER)
        glColor3f(1, 0, 0);
    else
        glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();
    glViewport(borderWidth, borderWidth, windowWidth - borderWidth * 2, windowHeight - borderWidth * 2);
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();

    glMatrixMode(GL_PROJECTION);    glPopMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(outerCamXYZ.x, outerCamXYZ.y, outerCamXYZ.z,
        0, 0, 0,
        0, 1, 0);

    drawSceneElements();
    drawInnerCamera();

    ///     draw border and background for preview box in upper corner  //////////////////////

        //next, do the code for the inner camera, which only sets in the top-right
        //corner!
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    //step 1: set the projection matrix using gluOrtho2D -- but save it first!
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    //step 2: clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //step 3: set the viewport matrix a little larger than needed...
    glViewport(2 * windowWidth / 3.0 - borderWidth, 2 * windowHeight / 3.0 - borderWidth,
        windowWidth / 3.0 + borderWidth, windowHeight / 3.0 + borderWidth);
    //step 3a: and fill it with a white rectangle!
    if (currentCamera == CAMERA_OUTER)
        glColor3f(1, 1, 1);
    else
        glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();

    //step 4: trim the viewport window to the size we want it...
    glViewport(2 * windowWidth / 3.0, 2 * windowHeight / 3.0,
        windowWidth / 3.0, windowHeight / 3.0);
    //step 4a: and color it black! the padding we gave it before is now a border.
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(0, 1); glVertex2f(1, 1); glVertex2f(1, 0);
    glEnd();

    //before rendering the scene in the corner, pop the old projection matrix back
    //and re-enable lighting!
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    ///     begin drawing scene in upper corner //////////////////////////////////////////////

    glViewport(2 * windowWidth / 3.0, 2 * windowHeight / 3.0,
        windowWidth / 3.0, windowHeight / 3.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(innerCamXYZ.x, innerCamXYZ.y, innerCamXYZ.z,      //camera is located at (x,y,z)
        innerCamXYZ.x + innerCamDir.x,                  //looking at a point that is
        innerCamXYZ.y + innerCamDir.y,                  //one unit along its direction
        innerCamXYZ.z + innerCamDir.z,
        0.0f, 1.0f, 0.0f);                                //up vector is (0,1,0) (positive Y)

    glClear(GL_DEPTH_BUFFER_BIT);                   //ensure that the overlay is always on top!


    drawSceneElements();

    //push the back buffer to the screen
    glutSwapBuffers();
}

void timer(int v)
{

    if (playAnimation)
    {
        if (aniSeconds <= 30 && aniState == 0)
        {
            upperAngle += 1;
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
    lineMove = 0;
}


// normalKeys() ////////////////////////////////////////////////////////////////
//
//  GLUT keyboard callback.
//
////////////////////////////////////////////////////////////////////////////////
void normalKeys(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
    {
        playAnimation = !playAnimation;
        glutPostRedisplay();
    }
    if (key == 'i' || key == 'I')
    {
        currentCamera = CAMERA_INNER;
        glutPostRedisplay();
    }

    if (key == 'o' || key == 'O')
    {
        currentCamera = CAMERA_OUTER;
        glutPostRedisplay();
    }

    if (key == '1')
    {
        displayRobot = !displayRobot;
        glutPostRedisplay();
    }

    if (key == '2') {
        cordinateLinesActive = !cordinateLinesActive;
        glutPostRedisplay();
    }
    if (key == 27) // escape key
        exit(0);
    glutPostRedisplay();

}

//process special keys
void processSpecialKeys(int key, int x, int y)
{
    float fraction = 0.1f;
    switch (key)
    {
    /*case GLUT_KEY_LEFT:
        angle -= 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_RIGHT:
        angle += 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
        */
        float movementConstant = 0.3;
        if (key == GLUT_KEY_UP) {
            if (USING_INNER)
            {
                innerCamXYZ += innerCamDir * movementConstant * (key == GLUT_KEY_UP ? 1 : 3);
            }
            else {
                outerCamTPR.z -= 1.0f * movementConstant;

                //limit the camera radius to some reasonable values so the user can't get lost
                if (outerCamTPR.z < 2.0)
                    outerCamTPR.z = 2.0;
                if (outerCamTPR.z > 10.0 * (currentCamera + 1))
                    outerCamTPR.z = 10.0 * (currentCamera + 1);
                recomputeOrientation(outerCamXYZ, outerCamTPR);
            }
            glutPostRedisplay();
        }

        if (key == GLUT_KEY_DOWN) {
            if (USING_INNER)
            {
                innerCamXYZ -= innerCamDir * movementConstant * (key == GLUT_KEY_DOWN ? 1 : 3);
            }
            else {
                outerCamTPR.z += 1.0f * movementConstant;

                //limit the camera radius to some reasonable values so the user can't get lost
                if (outerCamTPR.z < 2.0)
                    outerCamTPR.z = 2.0;
                if (outerCamTPR.z > 10.0 * (currentCamera + 1))
                    outerCamTPR.z = 10.0 * (currentCamera + 1);
                recomputeOrientation(outerCamXYZ, outerCamTPR);
            }
            glutPostRedisplay();
        }
    }
}



// main() //////////////////////////////////////////////////////////////////////
//
//  Program entry point. Does not process command line arguments.
//
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    //create a double-buffered GLUT window at (50,50) with predefined windowsize
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("double cameras... woahhhhh double cameras");

    printf("\n\
-----------------------------------------------------------------------------------\n\
		 Up Arrow  : Move camera forward\n\
        Down Arrow : Move camera backward\n\
        Left Arrow : Move camera left\n\
        Right Arrow: Move camera right\n\
            1      : Toggle wired or solid\n\
            2      : Toggle coordinate lines\n\
            a      : Toggle animation\n\
            i      : Select inner camera\n\
            o      : Select outter camera\n\
------------------------------------------------------------------------------------\n");

    //give the camera a 'pretty' starting point!
    innerCamXYZ = Point(5, 5, 5);
    innerCamTPR = Point(-M_PI / 4.0, M_PI / 4.0, 1);
    recomputeOrientation(innerCamDir, innerCamTPR);
    innerCamDir.normalize();

    outerCamTPR = Point(1.50, 2.0, 14.0);
    outerCamXYZ = Point(0, 0, 0);
    recomputeOrientation(outerCamXYZ, outerCamTPR);

    //register callback functions...
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    glutKeyboardFunc(normalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutDisplayFunc(renderCallback);
    glutReshapeFunc(resizeWindow);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);

    //do some basic OpenGL setup
    initScene();

    //and enter the GLUT loop, never to exit.
    glutMainLoop();
    return(0);
}