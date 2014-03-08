#include <stdio.h>
#include "../headers/System.h"

System::System(int width, int height)
        : pip(width/2, height/2)
        , racket1(LEFT, height/2, 60, 0.945, 0.757, 0.364)
        , racket2(RIGHT, height/2, 60, 0.035, 0.639, 0.678) {
    this->width = width;
    this->height = height;
    this->pip = System::Pip(width/2, height/2);
}

void System::recalculatePip(int timeDiff) {
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

bool System::collidesWithRacket(System::Racket racket) {
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

int System::guessWherePipIsGoing() {
    int distanceToWall = 0;
    if (pip.xSpeed > 0) {
        distanceToWall = width - (System::Pip::radius * 2) - pip.x;
    } else {
        distanceToWall = pip.x - (System::Pip::radius * 2);
    }
    float angle = atan(pip.ySpeed / abs(pip.xSpeed));
    int guess = pip.y + (tan(angle) * distanceToWall);
    
    //cout << "predicted y: " << guess << endl;
    
    if (guess > height) {
        guess = height*3/4;
    } else if (guess < 0) {
        guess = height/4;
    }
    
    return guess;
}

void System::updateRacket(Racket & racket, int timeDiff) {
    int targetY = height/2;
    
    racket.speed = 0;
    
    if ((racket.position == System::RIGHT && pip.xSpeed > 0) || (racket.position == System::LEFT && pip.xSpeed <= 0)) {
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

void System::updateRackets(int timeDiff) {
    //updateRacket(racket1, timeDiff);
    updateRacket(racket2, timeDiff);
}

void System::Pip::randomiseSpeed() {
    xSpeed = (Pip::startSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
    ySpeed = (Pip::startSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
}

System::Pip::Pip(int x, int y) {
    this->x = x;
    this->y = y;
    randomiseSpeed();
}

System::Pip System::getPip() {
    return pip;
}

System::Racket System::getRacket1() {
    return racket1;
}

System::Racket System::getRacket2() {
    return racket2;
}

void System::moveRacketUp() {
    int targetY = racket1.y + 30;
    
    if (height - racket1.y <= racket1.halfSize) {
        racket1.y = height - racket1.halfSize;
    } else {
        racket1.y = targetY;
    }
}

void System::moveRacketDown() {
    int targetY = racket1.y - 30;
    
    if (racket1.y <= racket1.halfSize) {
        racket1.y = racket1.halfSize;
    } else {
        racket1.y = targetY;
    }
}

const int System::Racket::maxSpeed = 200;
const int System::Racket::width = 20;

const int System::Pip::startSpeed = 300;
const int System::Pip::radius = 10;

