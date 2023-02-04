#pragma once

#include <ge211.hxx>

using Velocity = ge211::Dims<float>;
using Rectangle = ge211::Rect<float>;

enum class Obstacle_name
{
    bush,
    car,
    truck,
    log,
    lily_pad,
};

class Obstacle
{
public:
    //CONSTRUCTOR
    Obstacle(Rectangle dims, Obstacle_name name, Velocity velocity);

    //GETTERS
    Rectangle get_dims() const;
    Obstacle_name get_obstacle_name() const;

    //MEMBER FUNCTIONS
    void move_obst(float y);
    bool obst_overlap(Obstacle obst) const;
    Obstacle next(double dt) const;
    void update_velocity();
    friend bool operator==(Obstacle const&, Obstacle const&);

private:
    //MEMBER VARIABLES
    Rectangle dims_;
    Obstacle_name obstacle_name_;
    Velocity velocity_;
};