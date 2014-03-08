#ifndef lana_tennis_System_h
#define lana_tennis_System_h

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

class System {
public:
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
        
        Racket(RacketPosition position, int y, int size, float r, float g, float b) {
            this->position = position;
            this->speed = 0;
            this->y = y;
            this->size = size;
            this->halfSize = size/2;
            this->r = r;
            this->g = g;
            this->b = b;
        }
    };
    
    class Pip {
    public:
        static const int startSpeed; // pixels per second
        static const int radius;
        int x, y;
        int xSpeed, ySpeed;
        
        Pip(int x, int y);
        
        void randomiseSpeed();
    };
    
    int width, height;
    Pip pip;
    Racket racket1, racket2;
    
    System(int width, int height);
    
    void recalculatePip(int timeDiff);
    void updateRackets(int timeDiff);
    void updateRacket(Racket & racket, int timeDiff);
    void moveRacketUp();
    void moveRacketDown();

    int guessWherePipIsGoing();
    bool collidesWithRacket(Racket racket);
    
    Pip getPip();
    Racket getRacket1();
    Racket getRacket2();
};

#endif
