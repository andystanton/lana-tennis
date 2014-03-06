#ifndef lana_tennis_System_h
#define lana_tennis_System_h

#include <iostream>

class System {
public:
    class Pip {
    public:
        const int startSpeed = 300; // pixels per second
        const int radius = 10;
        int x, y;
        int xSpeed = startSpeed, ySpeed = startSpeed;
        
        Pip(int x, int y);
    
        void randomiseSpeed();
    };
    
    enum RacketPosition {LEFT, RIGHT};
    
    struct Racket {
        static const int width;
        const int maxSpeed = 200;
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
};

#endif
