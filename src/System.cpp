#include <stdio.h>
#include "../headers/System.h"

void System::Pip::randomiseSpeed() {
    xSpeed = (startSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
    ySpeed = (startSpeed + 25 - (rand() % 50)) * (rand()%2 > 0 ? 1 : -1);
}

System::Pip::Pip(int x, int y) {
    this->x = x;
    this->y = y;
}

const int System::Racket::width = 20;