
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

const int width=640, height=480;
const int pipRadius = 10;
const int racketWidth = 20;

int pipX = width/2, pipY = height/2;

int pipStartSpeed = 300; // pixels per second

int pipXSpeed, pipYSpeed;

int racketSpeed = 200;

int lastTime = 0;

enum RacketPosition {LEFT, RIGHT};

struct Racket {
    int y;
    int size;
    int halfSize;
    int speed;
    float r, g, b;
    RacketPosition position;
    
    Racket(RacketPosition position, int y, int size, int speed, float r, float g, float b) {
        this->position = position;
        this->speed = speed;
        this->y = y;
        this->size = size;
        this->halfSize = size/2;
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

Racket racket1 = Racket(LEFT, height/2, 60, 0, 0.945, 0.757, 0.364);
Racket racket2 = Racket(RIGHT, height/2, 60, 0, 0.035, 0.639, 0.678);


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
        glVertex2d(racketLeft, racket.y + racket.halfSize);
        glVertex2d(racketLeft + racketWidth, racket.y + racket.halfSize);
        glVertex2d(racketLeft + racketWidth, racket.y - racket.halfSize);
        glVertex2d(racketLeft, racket.y - racket.halfSize);
    glEnd();
    
    glPopMatrix();
}

void resetPipSpeed() {
    pipXSpeed = (pipStartSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
    pipYSpeed = (pipStartSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
}

int guessWherePipIsGoing() {
    int distanceToWall = 0;
    if (pipXSpeed > 0) {
        distanceToWall = width - (pipRadius * 2) - pipX;
    } else {
        distanceToWall = pipX - (pipRadius * 2);
    }
    float angle = atan(pipYSpeed / abs(pipXSpeed));
    int guess = pipY + (tan(angle) * distanceToWall);
    
    cout << "predicted y: " << guess << endl;
    
    if (guess > height) {
        guess = height*3/4;
    } else if (guess < 0) {
        guess = height/4;
    }
    
    return guess;
}

void moveRacketToY(Racket & racket, int timeDiff) {
    int targetY = height/2;
    
    racket.speed = 0;
    
    if (racket.position == RIGHT && pipXSpeed > 0) {
        targetY = guessWherePipIsGoing();
    } else if (racket.position == LEFT && pipXSpeed <= 0) {
        targetY = guessWherePipIsGoing();
    }
    
    if(racket.y < targetY) {
        if (height - racket.y <= racket.halfSize) {
            racket.y = height - racket.halfSize;
        } else if (targetY - racket.y > (racketSpeed * timeDiff) / 1000) {
            racket.y += (racketSpeed * timeDiff) / 1000;
            racket.speed += racketSpeed;
        } else {
            racket.y = targetY;
        }
    } else if(racket.y > targetY) {
        if (racket.y <= racket.halfSize) {
            racket.y = racket.halfSize;
        } else if (racket.y - targetY > (racketSpeed * timeDiff) / 1000) {
            racket.y -= (racketSpeed * timeDiff) / 1000;
            racket.speed -= racketSpeed;
        } else {
            racket.y = targetY;
        }
    }
}

bool collidesWithRacket(Racket racket) {
    if (pipY > racket.y + racket.halfSize || pipY < racket.y - racket.halfSize)
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
    
    pipYSpeed += racket.speed;
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
        resetPipSpeed();
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
    
    resetPipSpeed();
    
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