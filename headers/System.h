#ifndef lana_tennis_System_h
#define lana_tennis_System_h

#include <iostream>

class System {
public:
    class Pip {
    public:
        static const int startSpeed; // pixels per second
        static const int radius;
        int x, y;
        int xSpeed, ySpeed;
        
        Pip(int x, int y);
    
        void randomiseSpeed();
    };
    
    enum RacketPosition {LEFT, RIGHT};
    
    struct Racket {
        static const int width;
        static const int maxSpeed;
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
