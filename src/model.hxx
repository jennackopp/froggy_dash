#pragma once

#include <ge211.hxx>
#include "lane.hxx"
#include "frog.hxx"

struct Model
{
    //CONSTRUCTOR
    Model();

    //GETTERS
    Lane get_bottom_lane() const;
    int get_top_lane_index() const;
    int get_bottom_lane_index() const;
    int get_score() const;

    //MEMBER FUNCTIONS
    void move_up();
    void move_down();
    void move_right();
    void move_left();
    void on_frame(double dt);
    bool hit_by_vehicle();
    bool carried_off_screen();
    bool falls_in_water();

    //MEMBER VARIABLES
    std::vector<Lane> lanes;
    Frog frog;

private:
    //HELPERS
    void add_new_lane(Position pos);
    Lane get_curr_lane(Position pos);
    bool valid_pos(ge211::geometry::Posn<float>);

    //MEMBER VARIABLES
    Velocity velocity_;
    int top_lane_index_;
    int bottom_lane_index_;
    int score_;
    int last_landmark_score_;
};