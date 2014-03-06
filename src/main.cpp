
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

#include "../headers/System.h"

using namespace std;

const int width=640, height=480;

int lastTime = 0;

System::Racket racket1 = System::Racket(System::LEFT, height/2, 60, 0, 0.945, 0.757, 0.364);
System::Racket racket2 = System::Racket(System::RIGHT, height/2, 60, 0, 0.035, 0.639, 0.678);
System::Pip pip = System::Pip(width/2, height/2);

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

int guessWherePipIsGoing() {
    int distanceToWall = 0;
    if (pip.xSpeed > 0) {
        distanceToWall = width - (System::Pip::radius * 2) - pip.x;
    } else {
        distanceToWall = pip.x - (System::Pip::radius * 2);
    }
    float angle = atan(pip.ySpeed / abs(pip.xSpeed));
    int guess = pip.y + (tan(angle) * distanceToWall);
    
    cout << "predicted y: " << guess << endl;
    
    if (guess > height) {
        guess = height*3/4;
    } else if (guess < 0) {
        guess = height/4;
    }
    
    return guess;
}

void moveRacketToY(System::Racket & racket, int timeDiff) {
    int targetY = height/2;
    
    racket.speed = 0;
    
    if (racket.position == System::RIGHT && pip.xSpeed > 0) {
        targetY = guessWherePipIsGoing();
    } else if (racket.position == System::LEFT && pip.xSpeed <= 0) {
        targetY = guessWherePipIsGoing();
    }
    
    if(racket.y < targetY) {
        if (height - racket.y <= racket.halfSize) {
            racket.y = height - racket.halfSize;
        } else if (targetY - racket.y > (racket.maxSpeed * timeDiff) / 1000) {
            racket.y += (racket.maxSpeed * timeDiff) / 1000;
            racket.speed += racket.maxSpeed;
        } else {
            racket.y = targetY;
        }
    } else if(racket.y > targetY) {
        if (racket.y <= racket.halfSize) {
            racket.y = racket.halfSize;
        } else if (racket.y - targetY > (racket.maxSpeed * timeDiff) / 1000) {
            racket.y -= (racket.maxSpeed * timeDiff) / 1000;
            racket.speed -= racket.maxSpeed;
        } else {
            racket.y = targetY;
        }
    }
}

bool collidesWithRacket(System::Racket racket) {
    if (pip.y > racket.y + racket.halfSize || pip.y < racket.y - racket.halfSize)
        return false;
    
    switch(racket.position) {
        case System::RIGHT:
            if (pip.x < width - System::Racket::width)
                return false;
            break;
        case System::LEFT:
            if (pip.x > System::Racket::width)
                return false;
        default:
            break;
    }
    
    pip.ySpeed += racket.speed;
    return true;

}

void recalculateBall(int timeDiff) {
    pip.x += (pip.xSpeed * timeDiff)/1000;
    pip.y += (pip.ySpeed * timeDiff)/1000;
    
    if (pip.y < System::Pip::radius || pip.y > height - System::Pip::radius) {
        if (pip.y < System::Pip::radius) pip.y = System::Pip::radius;
        if (pip.y > height - System::Pip::radius) pip.y = height - System::Pip::radius;
        pip.ySpeed = -pip.ySpeed;
    }
    
    if (collidesWithRacket(racket1) || collidesWithRacket(racket2)) {
        if (pip.x < System::Racket::width + System::Pip::radius) pip.x = System::Racket::width + System::Pip::radius;
        if (pip.x > width - System::Racket::width - System::Pip::radius) pip.x = width - System::Racket::width - System::Pip::radius;
        pip.xSpeed = -pip.xSpeed;
    } else if (pip.x < System::Pip::radius || pip.x > width - System::Pip::radius) {
        if (pip.x < System::Pip::radius) pip.x = System::Pip::radius;
        if (pip.x > width - System::Pip::radius) pip.x = width - System::Pip::radius;
        pip.x = width/2;
        pip.y = height/2;
        pip.randomiseSpeed();
    }
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
            recalculateBall(newTime - lastTime);

            moveRacketToY(racket1, newTime - lastTime);
            moveRacketToY(racket2, newTime - lastTime);
            
            lastTime = newTime;
        }
        
        drawNet();
        drawBall();
        drawRacket(racket1);
        drawRacket(racket2);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}