#ifndef MAIN_H
#define MAIN_H

class Player {
    public:
        int width, height;
        float x, y, velX, velY, accelX, accelY, forceX, forceY, velG, forceG;
        float mass;

        Player();
        int* getCenter();
        bool pointInPlayer(int px, int py);
        void applyPhysics(float dt);
        void shoot(float angle);
};

#endif