#include "lane.hxx"

Lane::Lane(
        ge211::Posn<float> pos,
        Lane_type lane_type,
        Velocity velocity)

        :dims_ {pos.x, pos.y, 1000, 50},
         lane_type_ {lane_type},
         velocity_ {velocity},
         obstacles_ {},
         shown_ {true}
{
    int i = 0;
    if (lane_type_ == Lane_type::road or lane_type_ == Lane_type::river_log){
        while (i < 20){
            add_obstacle();
            i++;
        }
    } else{
        while (i < 4){
            add_obstacle();
            i++;
        }
    }
}

Lane_type
Lane::get_lane_type()
{return lane_type_;}

Rectangle
Lane::get_dims()
{return dims_;}

bool
Lane::get_shown()
{return shown_;}

std::vector<Obstacle>
Lane::get_obstacles()
{return obstacles_;}

Velocity
Lane::get_velocity()
{return velocity_;}

void
Lane::show_lane()
{shown_ = true;}

void
Lane::hide_lane()
{shown_ = false;}

void
Lane::clear_lane()
{obstacles_.clear();}

//move the lane and all its obstacles
void
Lane::move_lane(float y)
{
    dims_ = {dims_.x, dims_.y + y, dims_.width, dims_.height};
    for (Obstacle& obst : obstacles_){
        obst.move_obst(y);
    }
}

void
Lane::update_velocity()
{
    //boost velocity for lane and for obstacles it contains
    velocity_ *= 1.1;
    for (Obstacle& obst : obstacles_){
        obst.update_velocity();
    }
}

void
Lane::update_obstacles(double dt)
{
    unsigned int i = 0;
    //for every obstacle
    while (i < obstacles_.size()){
        obstacles_[i] = obstacles_[i].next(dt);
        i++;
    }
}

void
Lane::add_obst_helper(Obstacle obst)
{
    bool add = true;
    for (Obstacle obst1 : obstacles_){
        if (obst1.obst_overlap(obst)){
            add = false;
            break;
        }
    }
    if (add){
        obstacles_.push_back(obst);
    }
}

void
Lane::add_obstacle()
{
    //add bush to grass lane
    if (lane_type_ == Lane_type::grass) {
        float x = rand() % 949;
        Obstacle bush = Obstacle({x, dims_.y, 51, 50},
                                 Obstacle_name::bush,
                                 {0, 0});
        //add bush if no overlapping obst
        add_obst_helper(bush);
    }

    //add either car or truck to road lane
    if (lane_type_ == Lane_type::road) {
        int obst_decider = rand() % 2;

        //0 will make car
        if (obst_decider == 0) {

            //if car moves in from right
            if (velocity_.width < 0) {
                //find random x value in range from 0 to 2000
                float x = rand() % 2000;
                Obstacle car = Obstacle({x, dims_.y,88, 50},
                                        Obstacle_name::car,
                                        velocity_);
                //add car if no overlapping obst
                add_obst_helper(car);
            }

            //if car moves in from left
            if (velocity_.width > 0) {
                //find random x value in range from -1088 to 912
                float x = rand() % 912 + (-1088);
                Obstacle car = Obstacle({x, dims_.y,88, 50},
                                        Obstacle_name::car,
                                        velocity_);
                //add car if no overlapping obst
                add_obst_helper(car);
            }
        }

        //1 will make truck
        if (obst_decider == 1) {

            //if truck moves in from right
            if (velocity_.width < 0) {
                //find random x value in range from 0 to 2000
                float x = rand() % 2000;
                Obstacle truck = Obstacle({x, dims_.y,95, 50},
                                          Obstacle_name::truck,
                                          velocity_);
                //add truck if no overlapping obst
                add_obst_helper(truck);
            }

            //if truck moves in from left
            if (velocity_.width > 0) {
                //find random x value in range from -1095 to 905
                float x = rand() % 905 + (-1095);
                Obstacle truck = Obstacle({x, dims_.y,95, 50},
                                          Obstacle_name::truck,
                                          velocity_);
                //add truck if no overlapping obst
                add_obst_helper(truck);
            }
        }
    }

    //add lily pad to river_lily lane
    if (lane_type_ == Lane_type::river_lily) {
        float x = rand() % 950;
        //CHANGE WIDTH TO REAL LILY PAD WIDTH
        Obstacle lily_pad = Obstacle({x, dims_.y, 50, 50},
                                     Obstacle_name::lily_pad,
                                     {0, 0});
        //add lily pad if no overlapping obst
        add_obst_helper(lily_pad);
    }

    //add log to river_log lane
    if (lane_type_ == Lane_type::river_log) {

        //if log moves in from right
        if (velocity_.width < 0) {
            //find random x value in range from 0 to 2000
            float x = rand() % 2000;
            Obstacle log = Obstacle({x, dims_.y,227, 50},
                                    Obstacle_name::log,
                                    velocity_);
            //add log if no overlapping obst
            add_obst_helper(log);
        }

        //if log moves in from left
        if (velocity_.width > 0) {
            //find random x value in range from -1227 to 773
            float x = rand() % 773 + (-1227);
            Obstacle log = Obstacle({x, dims_.y,227, 50},
                                    Obstacle_name::log,
                                    velocity_);
            //add log if no overlapping obst
            add_obst_helper(log);
        }
    }
}

bool
operator==(Lane const& l1, Lane const& l2)
{
    return l1.dims_ == l2.dims_ &&
           l1.lane_type_ == l2.lane_type_ &&
           l1.velocity_ == l2.velocity_ &&
           l1.obstacles_ == l2.obstacles_ &&
           l1.shown_ == l2.shown_;
}