#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "bitmap.h"

#define SOLAR_YEAR 360.0f
#define EARTH_YEAR 4320.0f
#define VENUS_YEAR 2880.0f
#define MARS_YEAR 8640.0f
#define MOON_YEAR 360.0f
#define MERCURY_YEAR 1440.0f
#define JUPITER_YEAR 10800.0f
#define SATURN_YEAR 21600.0f
#define URANUS_YEAR 43200.0f
#define NEPTUNE_YEAR 86400.0f
#define PLUTO_YEAR 129600.0f
#define EARTH_DAY 11.0f
#define VENUS_DAY 66.0f
#define MARS_DAY 11.0f
#define MOON_DAY 27.3f
#define MERCURY_DAY 58.6f
#define JUPITER_DAY 9.9f
#define SATURN_DAY 10.7f
#define URANUS_DAY 17.2f
#define NEPTUNE_DAY 16.1f
#define PLUTO_DAY 153.3f

#define SUN_RADIUS 0.10f
#define VENUS_RADIUS 0.25f
#define EARTH_RADIUS 0.25f
#define MOON_RADIUS 0.08f
#define MARS_RADIUS 0.15f
#define MERCURY_RADIUS 0.1f
#define JUPITER_RADIUS 0.45f
#define SATURN_RADIUS 0.40f
#define URANUS_RADIUS 0.30f
#define NEPTUNE_RADIUS 0.28f
#define PLUTO_RADIUS 0.10f

#define MERCURY_ORBIT 0.30f
#define VENUS_ORBIT 0.75f
#define EARTH_ORBIT 1.75f
#define MOON_ORBIT 0.35f
#define MARS_ORBIT 2.50f
#define JUPITER_ORBIT 3.25f
#define SATURN_ORBIT 4.70f
#define URANUS_ORBIT 6.0f
#define NEPTUNE_ORBIT 6.90f
#define PLUTO_ORBIT 7.50f

#define MERCURY_TILT 0.034f
#define VENUS_TILT 177.3f
#define EARTH_TILT 23.44f
#define MARS_TILT 25.19f
#define JUPITER_TILT 3.13f
#define SATURN_TILT 26.73f
#define URANUS_TILT 97.77f
#define NEPTUNE_TILT 28.32f
#define PLUTO_TILT 122.53f

#define RING_INNER_RADIUS (SATURN_RADIUS * 1.5f)
#define RING_OUTER_RADIUS (SATURN_RADIUS * 2.5f)

#define MAX_STAR 10000

typedef struct {
    float pos[3];
    float col[4];
} Star;

Star star[MAX_STAR];
int mouseButton;
int _ScreenMode = 0;
int lastX = 0, lastY = 0;
float winwidth, winheight;
float cameraDistance = 6.0f;
float camRotX = 0.0f;
float camRotY = 0.0f;
float lastRotX = 0.0f;
float lastRotY = 0.0f;
float globalRotationAngle = 0.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = 6.0f;
float centerX = 0.0f;
float centerY = 0.0f;
float centerZ = 0.0f;
float venusAngle = 0.0f, earthAngle = 0.0f, moonAngle = 0.0f, marsAngle = 0.0f;
float mercuryAngle = 0.0f, jupiterAngle = 0.0f, saturnAngle = 0.0f;
float uranusAngle = 0.0f, neptuneAngle = 0.0f, plutoAngle = 0.0f;
bool mouseDown = false;
bool globalRotationEnabled = true;
bool animationEnabled = true;
bool isInResetState = false;

float venusRotationAngle = 0.0f, earthRotationAngle = 0.0f, moonRotationAngle = 0.0f;
float marsRotationAngle = 0.0f, mercuryRotationAngle = 0.0f, jupiterRotationAngle = 0.0f;
float saturnRotationAngle = 0.0f, uranusRotationAngle = 0.0f, neptuneRotationAngle = 0.0f;
float plutoRotationAngle = 0.0f;

GLuint venusTexture, earthTexture, moonTexture, marsTexture, mercuryTexture;
GLuint jupiterTexture, saturnTexture, uranusTexture, neptuneTexture, plutoTexture;

GLubyte* TextureLoadBitmap(const char* filename, int* w, int* h) {
    FILE* fp;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    if (fopen_s(&fp, filename, "rb") != 0) return NULL;
    if (fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }
    if (bfh.bfType != 0x4D42) {
        fclose(fp);
        return NULL;
    }
    if (fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }
    if (bih.biBitCount != 24) {
        fclose(fp);
        return NULL;
    }

    int rowSize = ((bih.biWidth * 3 + 3) & ~3);
    int imageSize = rowSize * bih.biHeight;
    GLubyte* imageData = (GLubyte*)malloc(imageSize);

    if (!imageData) {
        fclose(fp);
        return NULL;
    }

    fseek(fp, bfh.bfOffBits, SEEK_SET);
    if (fread(imageData, 1, imageSize, fp) != imageSize) {
        free(imageData);
        fclose(fp);
        return NULL;
    }

    *w = bih.biWidth;
    *h = bih.biHeight;
    fclose(fp);
    return imageData;
}

GLuint LoadTexture(const char* filename) {
    int width, height;
    GLuint textureID;
    GLubyte* imageData = TextureLoadBitmap(filename, &width, &height);
    if (!imageData) return 0;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    GLubyte* convertedData = (GLubyte*)malloc(width * height * 3);
    if (!convertedData) {
        free(imageData);
        return 0;
    }

    for (int i = 0; i < width * height; i++) {
        int srcIdx = i * 3;
        convertedData[srcIdx] = imageData[srcIdx + 2];
        convertedData[srcIdx + 1] = imageData[srcIdx + 1];
        convertedData[srcIdx + 2] = imageData[srcIdx];
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, convertedData);

    free(imageData);
    free(convertedData);
    return textureID;
}

void drawTexturedSphere(GLdouble radius, GLint slices, GLint stacks) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluSphere(quadric, radius, slices, stacks);
    gluDeleteQuadric(quadric);
}

void drawSaturnRings() {
    glPushMatrix();

    // Draw the rings
    glBegin(GL_QUADS);
    const int segments = 100;
    for (int i = 0; i < segments; i++) {
        float theta1 = ((float)i / segments) * 2.0f * M_PI;
        float theta2 = ((float)(i + 1) / segments) * 2.0f * M_PI;

        // Outer ring vertices
        glVertex3f(RING_OUTER_RADIUS * cos(theta1), 0.0f, RING_OUTER_RADIUS * sin(theta1));
        glVertex3f(RING_OUTER_RADIUS * cos(theta2), 0.0f, RING_OUTER_RADIUS * sin(theta2));

        // Inner ring vertices
        glVertex3f(RING_INNER_RADIUS * cos(theta2), 0.0f, RING_INNER_RADIUS * sin(theta2));
        glVertex3f(RING_INNER_RADIUS * cos(theta1), 0.0f, RING_INNER_RADIUS * sin(theta1));
    }
    glEnd();

    glPopMatrix();
}

void reset() {
    mercuryAngle = venusAngle = earthAngle = moonAngle = marsAngle = 0.0f;
    jupiterAngle = saturnAngle = uranusAngle = neptuneAngle = plutoAngle = 0.0f;
    mercuryRotationAngle = venusRotationAngle = earthRotationAngle = 0.0f;
    moonRotationAngle = marsRotationAngle = jupiterRotationAngle = 0.0f;
    saturnRotationAngle = uranusRotationAngle = neptuneRotationAngle = 0.0f;
    plutoRotationAngle = globalRotationAngle = 0.0f;
    centerX = centerY = centerZ = 0.0f;
    glutPostRedisplay();
    isInResetState = true;
}

void keyboard(unsigned char key, int x, int y) {
    static int viewPosition = 0;
    switch (key) {
    case 'g': case 'G': globalRotationEnabled = !globalRotationEnabled; break;
    case 't': case 'T': animationEnabled = !animationEnabled; break;
    case 'r': case 'R': reset(); break;
    case 'v': case 'V':
        switch (viewPosition) {
        case 0:  // Right view
            cameraX = 18.0f; cameraY = 0.0f; cameraZ = 0.0f;
            break;
        case 1:  // Left view
            cameraX = -18.0f; cameraY = 0.0f; cameraZ = 0.0f;
            break;
        case 2:  // Front view
            cameraX = 0.0f; cameraY = 0.0f; cameraZ = 18.0f;
            break;
        case 3:  // Back view
            cameraX = 0.0f; cameraY = 0.0f; cameraZ = -18.0f;
            break;
        }
        viewPosition = (viewPosition + 1) % 4;
        glutPostRedisplay();
        break;
    case 'b': case 'B':
        cameraX = 0.0f;
        cameraY = 0.0f;
        cameraZ = 6.0f;
        centerX = centerY = centerZ = 0.0f;
        glutPostRedisplay();
        break;
    case 27: exit(0); break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        mouseDown = true;
        lastX = x;
        lastY = y;

        lastRotX = camRotX;
        lastRotY = camRotY;

        if (button == GLUT_LEFT_BUTTON) {
            mouseButton = GLUT_LEFT_BUTTON;
        }
        else if (button == 3) { // Mouse wheel up
            float zoomFactor = 0.9f;
            cameraX *= zoomFactor;
            cameraY *= zoomFactor;
            cameraZ *= zoomFactor;
        }
        else if (button == 4) { // Mouse wheel down
            float zoomFactor = 1.1f;
            cameraX *= zoomFactor;
            cameraY *= zoomFactor;
            cameraZ *= zoomFactor;
        }
        glutPostRedisplay();
    }
    else {
        mouseDown = false;
    }
}


void motion(int x, int y) {
    if (mouseDown) {
        float dx = (x - lastX) * 0.2f;
        float dy = (y - lastY) * 0.2f;

        if (mouseButton == GLUT_LEFT_BUTTON) {
            
            float panSpeed = 0.01f;

            float rightX = cameraZ;
            float rightZ = -cameraX;
            float rightLength = sqrt(rightX * rightX + rightZ * rightZ);
            if (rightLength > 0) {
                rightX /= rightLength;
                rightZ /= rightLength;
            }

            centerX += rightX * dx * panSpeed;
            centerZ += rightZ * dx * panSpeed;
            centerY -= dy * panSpeed;
        }

        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

void Special(int key, int x, int y) {
    if (key == GLUT_KEY_F1) {
        _ScreenMode++;
        if (_ScreenMode > 3) _ScreenMode = 0;
        if (_ScreenMode == 0)
            glutReshapeWindow(200, 200);
        else if (_ScreenMode == 1)
            glutReshapeWindow(400, 400);
        else if (_ScreenMode == 2)
            glutReshapeWindow(600, 600);
        else if (_ScreenMode == 3)
            glutReshapeWindow(50, 50);
    }
}

void reshape(int w, int h) {
    winwidth = (GLfloat)w;
    winheight = (GLfloat)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f);

    //stars
    glPushMatrix();
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_STAR; i++) {
        glVertex3f(star[i].pos[0], star[i].pos[1], star[i].pos[2]);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();

    if (globalRotationEnabled) {
        globalRotationAngle = fmod(globalRotationAngle, 360.0f);
        glRotatef(globalRotationAngle, 0.0f, 1.0f, 0.0f);
    }

    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //sun
    GLfloat sunEmissive[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_EMISSION, sunEmissive);
    glutSolidSphere(SUN_RADIUS, 30, 30);
    glPopMatrix();

    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    if (animationEnabled) {
        mercuryAngle += 360.0f / MERCURY_YEAR;
        venusAngle += 360.0f / VENUS_YEAR;
        earthAngle += 360.0f / EARTH_YEAR;
        moonAngle += 360.0f / MOON_YEAR;
        marsAngle += 360.0f / MARS_YEAR;
        jupiterAngle += 360.0f / JUPITER_YEAR;
        saturnAngle += 360.0f / SATURN_YEAR;
        uranusAngle += 360.0f / URANUS_YEAR;
        neptuneAngle += 360.0f / NEPTUNE_YEAR;
        plutoAngle += 360.0f / PLUTO_YEAR;
        mercuryRotationAngle -= 360.0f / MERCURY_DAY;
        venusRotationAngle -= 360.0f / VENUS_DAY;
        earthRotationAngle -= 360.0f / EARTH_DAY;
        marsRotationAngle -= 360.0f / MARS_DAY;
        jupiterRotationAngle -= 360.0f / JUPITER_DAY;
        saturnRotationAngle -= 360.0f / SATURN_DAY;
        uranusRotationAngle -= 360.0f / URANUS_DAY;
        neptuneRotationAngle -= 360.0f / NEPTUNE_DAY;
        plutoRotationAngle -= 360.0f / PLUTO_DAY;
    }
    //planets
#define DRAW_PLANET(name, texture, radius, orbit, angle, rotAngle, tilt) \
    glPushMatrix(); \
    glRotatef(currentTilt, 1.0f, 0.0f, 0.0f); \
    glRotatef(angle, 0.0f, 1.0f, 0.0f); \
    glTranslatef(orbit, 0.0f, 0.0f); \
    glRotatef(rotAngle, 0.0f, 1.0f, 0.0f); \
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); \
    glEnable(GL_TEXTURE_2D); \
    glBindTexture(GL_TEXTURE_2D, texture); \
    drawTexturedSphere(radius, 30, 30); \
    glDisable(GL_TEXTURE_2D); \
    glPopMatrix();

    float currentTilt;

    currentTilt = isInResetState ? 0.0f : MERCURY_TILT;
    DRAW_PLANET(Mercury, mercuryTexture, MERCURY_RADIUS, MERCURY_ORBIT, mercuryAngle, mercuryRotationAngle, MERCURY_TILT);
    currentTilt = isInResetState ? 0.0f : VENUS_TILT;
    DRAW_PLANET(Venus, venusTexture, VENUS_RADIUS, VENUS_ORBIT, venusAngle, venusRotationAngle, VENUS_TILT);
    currentTilt = isInResetState ? 0.0f : EARTH_TILT;
    DRAW_PLANET(Earth, earthTexture, EARTH_RADIUS, EARTH_ORBIT, earthAngle, earthRotationAngle, EARTH_TILT);
    currentTilt = isInResetState ? 0.0f : MARS_TILT;
    DRAW_PLANET(Mars, marsTexture, MARS_RADIUS, MARS_ORBIT, marsAngle, marsRotationAngle, MARS_TILT);
    currentTilt = isInResetState ? 0.0f : JUPITER_TILT;
    DRAW_PLANET(Jupiter, jupiterTexture, JUPITER_RADIUS, JUPITER_ORBIT, jupiterAngle, jupiterRotationAngle, JUPITER_TILT);

    currentTilt = isInResetState ? 0.0f : SATURN_TILT;
    glPushMatrix();
    glRotatef(currentTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(saturnAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(SATURN_ORBIT, 0.0f, 0.0f);
    glRotatef(saturnRotationAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    //Saturn
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, saturnTexture);
    drawTexturedSphere(SATURN_RADIUS, 30, 30);
    glDisable(GL_TEXTURE_2D);

    //Saturn's rings
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.8f, 0.8f, 0.7f); // Yellowish-grey color for rings
    drawSaturnRings();
    glColor3f(1.0f, 1.0f, 1.0f); // Reset color
    glPopMatrix();

	//DUranus
    currentTilt = isInResetState ? 0.0f : URANUS_TILT;
    glPushMatrix();
    glRotatef(currentTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(uranusAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(URANUS_ORBIT, 0.0f, 0.0f);
    glRotatef(uranusRotationAngle, 0.0f, 0.0f, -1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, uranusTexture);
    drawTexturedSphere(URANUS_RADIUS, 30, 30);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    currentTilt = isInResetState ? 0.0f : NEPTUNE_TILT;
    DRAW_PLANET(Neptune, neptuneTexture, NEPTUNE_RADIUS, NEPTUNE_ORBIT, neptuneAngle, neptuneRotationAngle, NEPTUNE_TILT);

    //Pluto
    currentTilt = isInResetState ? 0.0f : PLUTO_TILT;
    glPushMatrix();
    glRotatef(currentTilt, 1.0f, 0.0f, 0.0f);
    glRotatef(plutoAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(PLUTO_ORBIT, 0.0f, 0.0f);
    glRotatef(plutoRotationAngle, 0.0f, 0.0f, -1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, plutoTexture);
    drawTexturedSphere(PLUTO_RADIUS, 30, 30);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //Moon
    glPushMatrix();
    glRotatef(earthAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(EARTH_ORBIT, 0.0f, 0.0f);
    glRotatef(moonAngle, 0.0f, 1.0f, 0.0f);
    glTranslatef(MOON_ORBIT, 0.0f, 0.0f);
    glRotatef(5.145f, 1.0f, 0.0f, 0.0f); // moon tilt relative to Earth's orbital plane
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    drawTexturedSphere(MOON_RADIUS, 30, 30);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void init() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

    const char* textures[] = {
        "mercury.bmp", "venus.bmp", "earth.bmp", "moon.bmp", "mars.bmp",
        "jupiter.bmp", "saturn.bmp", "uranus.bmp", "neptune.bmp", "pluto.bmp"
    };
    GLuint* textureIDs[] = {
        &mercuryTexture, &venusTexture, &earthTexture, &moonTexture, &marsTexture,
        &jupiterTexture, &saturnTexture, &uranusTexture, &neptuneTexture, &plutoTexture
    };

    for (int i = 0; i < 10; i++) {
        *textureIDs[i] = LoadTexture(textures[i]);
        if (!*textureIDs[i]) {
            fprintf(stderr, "Failed to load texture: %s\n", textures[i]);
        }
    }

    //random stars
    for (int i = 0; i < MAX_STAR; i++) {
        float theta = (float)rand() / RAND_MAX * 2.0f * M_PI;
        float phi = (float)rand() / RAND_MAX * M_PI;
        float radius = 20.0f;
        star[i].pos[0] = radius * sin(phi) * cos(theta);
        star[i].pos[1] = radius * sin(phi) * sin(theta);
        star[i].pos[2] = radius * cos(phi);
    }

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void idle() {
    if (animationEnabled) {
        isInResetState = false;

#define UPDATE_PLANET(angle, year, rotAngle, day) \
            angle += 360.0f / year; \
            rotAngle -= 360.0f / day;

        UPDATE_PLANET(mercuryAngle, MERCURY_YEAR, mercuryRotationAngle, MERCURY_DAY);
        UPDATE_PLANET(venusAngle, VENUS_YEAR, venusRotationAngle, VENUS_DAY);
        UPDATE_PLANET(earthAngle, EARTH_YEAR, earthRotationAngle, EARTH_DAY);
        UPDATE_PLANET(moonAngle, MOON_YEAR, moonRotationAngle, MOON_DAY);
        UPDATE_PLANET(marsAngle, MARS_YEAR, marsRotationAngle, MARS_DAY);
        UPDATE_PLANET(jupiterAngle, JUPITER_YEAR, jupiterRotationAngle, JUPITER_DAY);
        UPDATE_PLANET(saturnAngle, SATURN_YEAR, saturnRotationAngle, SATURN_DAY);
        UPDATE_PLANET(uranusAngle, URANUS_YEAR, uranusRotationAngle, URANUS_DAY);
        UPDATE_PLANET(neptuneAngle, NEPTUNE_YEAR, neptuneRotationAngle, NEPTUNE_DAY);
        UPDATE_PLANET(plutoAngle, PLUTO_YEAR, plutoRotationAngle, PLUTO_DAY);

    }
    if (globalRotationEnabled) {
        globalRotationAngle += 360.0f / SOLAR_YEAR;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Solar System Simulation");

    glutDisplayFunc(display);
    glutSpecialFunc(Special);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    init();
    glutMainLoop();
    return 0;
}