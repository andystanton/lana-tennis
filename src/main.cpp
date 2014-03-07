
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

#include "../headers/System.h"

using namespace std;

const int width=640, height=480;

int lastTime = 0;

System sys = System(width, height);

void reshape(GLFWwindow* window, int width, int height ) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION );
    glLoadIdentity();
    glScalef(1.f, -1.f, 1.f);
    glOrtho(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW );
    glLoadIdentity();
}

void drawNet() {
    glPushMatrix();
    
    glColor3f(1, 0.192, 0.455);
    glLineWidth(5);
    
    glBegin(GL_LINES);
        glVertex2d(width/2, 0);
        glVertex2d(width/2, height);
    glEnd();
    
    glPopMatrix();
}

void drawBall() {
    glPushMatrix();
    
    glColor3f(0.580, 0.733, 0.408);
    
    System::Pip pip = sys.getPip();
    
    glBegin(GL_QUADS);
        glVertex2d(pip.x - System::Pip::radius, pip.y + System::Pip::radius);
        glVertex2d(pip.x + System::Pip::radius, pip.y + System::Pip::radius);
        glVertex2d(pip.x + System::Pip::radius, pip.y - System::Pip::radius);
        glVertex2d(pip.x - System::Pip::radius, pip.y - System::Pip::radius);
    glEnd();
    
    
    glPopMatrix();
}

void drawRacket(System::Racket racket) {
    glPushMatrix();
    
    glColor3f(racket.r, racket.g, racket.b);
    
    int racketLeft;
    
    switch(racket.position) {
        case System::RIGHT:
            racketLeft = width - System::Racket::width;
            break;
        case System::LEFT:
            racketLeft = 0;
        default:
            break;
    }
    
    glBegin(GL_QUADS);
        glVertex2d(racketLeft, racket.y + racket.halfSize);
        glVertex2d(racketLeft + System::Racket::width, racket.y + racket.halfSize);
        glVertex2d(racketLeft + System::Racket::width, racket.y - racket.halfSize);
        glVertex2d(racketLeft, racket.y - racket.halfSize);
    glEnd();
    
    glPopMatrix();
}


int main(void) {
    srand(time(NULL));
    
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, "Lana Tennis", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    reshape(window, width, height);
    
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glfwSetWindowSizeCallback(window, reshape);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.455, 0.1, 0.322, 0.f);
        
        int newTime = floor(glfwGetTime() * 1000);
        if (newTime - lastTime > 10) {
            sys.recalculatePip(newTime - lastTime);
            sys.updateRackets(newTime - lastTime);
            lastTime = newTime;
        }
        
        drawNet();
        drawBall();
        drawRacket(sys.getRacket1());
        drawRacket(sys.getRacket2());
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}