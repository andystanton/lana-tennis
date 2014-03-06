#include <stdio.h>
#include "../headers/System.h"

void System::Pip::randomiseSpeed() {
    xSpeed = (Pip::startSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
    ySpeed = (Pip::startSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
}

System::Pip::Pip(int x, int y) {
    this->x = x;
    this->y = y;
    randomiseSpeed();
}

const int System::Racket::maxSpeed = 200;
const int System::Racket::width = 20;

const int System::Pip::startSpeed = 300;
const int System::Pip::radius = 10;