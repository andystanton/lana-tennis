
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;

const int width=640, height=480;
const int pipRadius = 10;
const int racketWidth = 20;

int pipX = width/2, pipY = height/2;
int pipXSpeed = 200, pipYSpeed = 200; // pixels per second

int racketSpeed = 200;

int lastTime = 0;

enum RacketPosition {LEFT, RIGHT};

struct Racket {
    int y;
    int size;
    float r, g, b;
    RacketPosition position;
    
    Racket(RacketPosition position, int y, int size, float r, float g, float b) {
        this->position = position;
        this->y = y;
        this->size = size;
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

Racket racket1 = Racket(LEFT, height/2, 120, 1, 1, 0);
Racket racket2 = Racket(RIGHT, height/2, 120, 0, 1, 1);


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
    
    glColor3f(0, 0, 1);
    glLineWidth(5);
    
    glBegin(GL_LINES);
        glVertex2d(width/2, 0);
        glVertex2d(width/2, height);
    glEnd();
    
    glPopMatrix();
}

void drawBall() {
    glPushMatrix();
    
    glColor3f(1.0, 0.0, 0.0);
    
    glBegin(GL_QUADS);
        glVertex2d(pipX - pipRadius, pipY + pipRadius);
        glVertex2d(pipX + pipRadius, pipY + pipRadius);
        glVertex2d(pipX + pipRadius, pipY - pipRadius);
        glVertex2d(pipX - pipRadius, pipY - pipRadius);
    glEnd();
    
    
    glPopMatrix();
}

void drawRacket(Racket racket) {
    glPushMatrix();
    
    glColor3f(racket.r, racket.g, racket.b);
    
    int racketLeft;
    
    switch(racket.position) {
        case RIGHT:
            racketLeft = width - racketWidth;
            break;
        case LEFT:
            racketLeft = 0;
        default:
            break;
    }
    
    glBegin(GL_QUADS);
        glVertex2d(racketLeft, racket.y + racket.size/2);
        glVertex2d(racketLeft + racketWidth, racket.y + racket.size/2);
        glVertex2d(racketLeft + racketWidth, racket.y - racket.size/2);
        glVertex2d(racketLeft, racket.y - racket.size/2);
    glEnd();
    
    glPopMatrix();
}

int guessWherePipIsGoing() {
    int distanceToWall = 0;
    if (pipXSpeed > 0) {
        distanceToWall = width - (pipRadius * 2) - pipX;
    } else {
        distanceToWall = pipX - (pipRadius * 2);
    }
    float angle = atan(pipYSpeed / abs(pipXSpeed));
    //cout << "with an angle of atan(" << pipYSpeed/pipXSpeed <<  ")=" << angle << endl;
    //return pipY + (tan(angle) * distanceToWall);
    //cout << "predicted y: " << (distanceToWall / abs(pipXSpeed)) * pipYSpeed << endl;
    
    cout << "predicted y: " << pipY + (tan(angle) * distanceToWall) << endl;
    
    return pipY + (tan(angle) * distanceToWall);
}

void moveRacketToY(Racket & racket, int timeDiff) {
    int y = height/2;
    if (racket.position == RIGHT && pipXSpeed > 0) {
        y = guessWherePipIsGoing();
    } else if (racket.position == LEFT && pipXSpeed <= 0) {
        y = guessWherePipIsGoing();
    }
    
    if(racket.y < y) {
        if (height - racket.y <= racket.size / 2) {
            racket.y = height - racket.size / 2;
        } else if (y - racket.y > (racketSpeed * timeDiff) / 1000) {
            racket.y += (racketSpeed * timeDiff) / 1000;
        } else {
            racket.y = y;
        }
    } else if(racket.y > y) {
        if (racket.y <= racket.size / 2) {
            racket.y = racket.size / 2;
        } else if (racket.y - y > (racketSpeed * timeDiff) / 1000) {
            racket.y -= (racketSpeed * timeDiff) / 1000;
        } else {
            racket.y = y;
        }
    }
}

bool collidesWithRacket(Racket racket) {
    if (pipY > racket.y + racket.size/2 || pipY < racket.y - racket.size/2)
        return false;
    
    switch(racket.position) {
        case RIGHT:
            if (pipX < width - racketWidth)
                return false;
            break;
        case LEFT:
            if (pipX > racketWidth)
                return false;
        default:
            break;
    }
    
    return true;

}

void recalculateBall(int timeDiff) {
    pipX += (pipXSpeed * timeDiff)/1000;
    pipY += (pipYSpeed * timeDiff)/1000;
    
    if (pipY < pipRadius || pipY > height - pipRadius) {
        if (pipY < pipRadius) pipY = pipRadius;
        if (pipY > height - pipRadius) pipY = height - pipRadius;
        pipYSpeed = -pipYSpeed;
    }
    
    if (collidesWithRacket(racket1) || collidesWithRacket(racket2)) {
        if (pipX < racketWidth + pipRadius) pipX = racketWidth + pipRadius;
        if (pipX > width - racketWidth - pipRadius) pipX = width - racketWidth - pipRadius;
        pipXSpeed = -pipXSpeed;
    } else if (pipX < pipRadius || pipX > width - pipRadius) {
        if (pipX < pipRadius) pipX = pipRadius;
        if (pipX > width - pipRadius) pipX = width - pipRadius;
        pipX = width/2;
        pipY = height/2;
    }
}

int main(void) {
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