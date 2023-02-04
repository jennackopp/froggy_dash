#include "model.hxx"

Model::Model()
        : lanes {},
          frog {Frog()},
          velocity_ {20, 0},
          top_lane_index_ {9},
          bottom_lane_index_ {0},
          score_ {0},
          last_landmark_score_{0}
{
    int i = 0;
    Position curr_pos = {0, 450};
    //first three lanes will all be grass
    while (i < 4){
        Lane lane = Lane(curr_pos, Lane_type::grass, {0, 0});
        lanes.push_back(lane);
        curr_pos.y -= 50;
        i++;
    }
    //clear obstacles from lane frog starts in
    lanes[2].clear_lane();
    //rest of lanes will be random
    while (i < 10){
        add_new_lane(curr_pos);
        curr_pos.y -= 50;
        i++;
    }
}

Lane
Model::get_bottom_lane() const
{return lanes[bottom_lane_index_];}

int
Model::get_top_lane_index() const
{return top_lane_index_;}

int
Model::get_bottom_lane_index() const
{return bottom_lane_index_;}

int
Model::get_score() const
{return score_;}

//if frog can move forward, update lanes vector accordingly and increase score
void
Model::move_up()
{
    //only move forward if next frog pos is valid
    Position next_pos = {frog.top_left().x, frog.top_left().y - 50};
    if (valid_pos(next_pos)){
        //move lane and its obstacles down 50
        for (Lane& lane : lanes){
            lane.move_lane(50);
        }
        //add new lane to vector if needed, update score
        unsigned int max = top_lane_index_ + 1;
        if (lanes.size() == max){
            add_new_lane({0, 0});
            score_++;
        }
        //update top and bottom lanes
        top_lane_index_ += 1;
        lanes[top_lane_index_].show_lane();
        lanes[bottom_lane_index_].hide_lane();
        bottom_lane_index_ += 1;
    }

    //update velocity when score increases by ten
    if (last_landmark_score_ += 10 == score_){
        velocity_ *= 1.1;
        for (Lane& lane : lanes){
            lane.update_velocity();
        }
        last_landmark_score_ += 10;
    }
}

//if frog can move backward, move lanes vector accordingly
void
Model::move_down()
{
    Position next_pos = {frog.top_left().x, frog.top_left().y + 50};
    //can only move down if not back at start and next frog pos is valid
    if ((not lanes[0].get_shown()) and valid_pos(next_pos)){
        //move lane and its obstacles up 50
        for (Lane& lane : lanes){
            lane.move_lane(-50);
        }
        //update top and bottom lanes
        lanes[top_lane_index_].hide_lane();
        top_lane_index_ -= 1;
        bottom_lane_index_ -= 1;
        lanes[bottom_lane_index_].show_lane();
    }
}

//if frog can move right, move frog x pos right
void
Model::move_right()
{
    Position next_pos = {frog.top_left().x + 5, frog.top_left().y};
    if (valid_pos(next_pos)){
        frog.move(5);
    }
}

//if frog can move left, move frog x pos left
void
Model::move_left()
{
    Position next_pos = {frog.top_left().x - 5, frog.top_left().y};
    if (valid_pos(next_pos)){
        frog.move(-5);
    }
}

//move obstacles accordingly, iterate thru vector of lanes, move frog if on log
void
Model::on_frame(double dt)
{
    for (Lane& lane : lanes){
        lane.update_obstacles(dt);
    }

    //move frog if on log
    Lane curr_lane = get_curr_lane(frog.top_left());
    if (curr_lane.get_lane_type() == Lane_type::river_log and
        not frog.hits_water(curr_lane)){
        frog.update_velocity(curr_lane.get_velocity().width);
        frog = frog.next(dt);
    }
}

void
Model::add_new_lane(Position pos)
{
    Lane_type lane_type = Lane_type(rand() % 4);
    //no two river_lily lanes back to back
    if ((lanes[top_lane_index_].get_lane_type() == Lane_type::river_lily) and
        lane_type == Lane_type::river_lily){
        int lane_decider = rand() % 3;
        if (lane_decider == 0){
            lane_type = Lane_type::grass;
        } else if (lane_decider == 1){
            lane_type = Lane_type::road;
        } else {
            lane_type = Lane_type::river_log;
        }
    }
    //randomly choose direction for road and river_log lanes
    //add lane to vector
    if (lane_type == Lane_type::road or lane_type == Lane_type::river_log){
        int velocity_decider = rand() % 2;
        if (velocity_decider == 0){
            Lane lane = Lane(pos, lane_type, velocity_);
            lanes.push_back(lane);
        } else{
            Lane lane = Lane(pos, lane_type, velocity_.operator*=(-1));
            lanes.push_back(lane);
        }
    } else{
        Lane lane = Lane(pos, lane_type, {0, 0});
        lanes.push_back(lane);
    }
}

Lane
Model::get_curr_lane(Position pos)
{
    for (Lane lane : lanes){
        if (pos.y == lane.get_dims().y){
            return lane;
        }
    }
    return lanes[0];
}

bool
Model::valid_pos(ge211::geometry::Posn<float> pos)
{
    //if position does not fit on board, INVALID POS!!!
    if ((pos.x < 0) or ((pos.x + 66) > 1000)){
        return false;
    }

    Lane curr_lane = get_curr_lane(pos);
    for (Obstacle obst : curr_lane.get_obstacles()){
        //if obstacle intersects w frog at pos AND obst blocks pos, INVALID POS!
        if (((obst.get_dims().x + obst.get_dims().width) >= pos.x) and
            ((pos.x + 66) >= obst.get_dims().x) and
            (obst.get_obstacle_name() == Obstacle_name::bush)){
            return false;
        }
    }
    //if none of these conditions are true, then the pos is VALID
    return true;
}

bool
Model::hit_by_vehicle()
{
    bool hit_by_vehicle = false;
    Lane curr_lane = get_curr_lane(frog.top_left());
    if (curr_lane.get_lane_type() == Lane_type::road){
        for (Obstacle obst : curr_lane.get_obstacles()){
            if (frog.hits_obstacle(obst)){
                score_ = 0;
                hit_by_vehicle = true;
            }
        }
    }
    return hit_by_vehicle;
}

bool
Model::carried_off_screen()
{
    bool carried_off_screen = false;
    Lane curr_lane = get_curr_lane(frog.top_left());
    //if frog pos is off screen and the lane is a river_log
    if ((curr_lane.get_lane_type() == Lane_type::river_log) and
        ((frog.top_left().x < 0) or ((frog.top_left().x + 66) > 1000))){
        score_ = 0;
        carried_off_screen = true;
    }
    return carried_off_screen;
}

bool
Model::falls_in_water()
{
    bool falls_in_water = false;
    Lane curr_lane = get_curr_lane(frog.top_left());
    if (frog.hits_water(curr_lane)){
        score_ = 0;
        falls_in_water = true;
    }
    return falls_in_water;
}