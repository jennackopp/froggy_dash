#pragma once

#include <ge211.hxx>
#include "lane.hxx"
#include "obstacle.hxx"

using Rectangle = ge211::Rect<float>;

struct Frog
{
public:
    //CONSTRUCTOR
    Frog();

    //GETTERS
    ge211::Posn<float> top_left() const;
    bool get_live() const;

    //MEMBER FUNCTIONS
    void kill_frog();
    bool hits_obstacle(Obstacle const& obstacle) const;
    bool hits_water(Lane lane) const;
    Frog next(double dt) const;
    void move(float x);
    void update_velocity(float x);

private:
    //MEMBER VARIABLES
    Rectangle dims_;
    Velocity velocity_;
    bool live_;
};