#pragma once

#include <ge211.hxx>
#include "obstacle.hxx"

using Rectangle = ge211::Rect<float>;
using Velocity = ge211::Dims<float>;
using Position = ge211::Posn<float>;

enum class Lane_type
{
    grass,
    road,
    river_lily,
    river_log,
};

class Lane
{
public:
    //CONSTRUCTOR
    Lane(Position pos, Lane_type lane_type, Velocity velocity);

    //GETTERS
    Lane_type get_lane_type();
    Rectangle get_dims();
    bool get_shown();
    std::vector<Obstacle> get_obstacles();
    Velocity get_velocity();

    //MEMBER FUNCTIONS
    void show_lane();
    void hide_lane();
    void clear_lane();
    void move_lane(float y);
    void update_velocity();
    void update_obstacles(double dt);
    void add_obstacle();
    friend bool operator==(Lane const&, Lane const&);

private:
    //HELPER FUNCTIONS
    void add_obst_helper(Obstacle obst);

    //MEMBER VARIABLES
    Rectangle dims_;
    Lane_type lane_type_;
    Velocity velocity_;
    std::vector<Obstacle> obstacles_;
    bool shown_;
};