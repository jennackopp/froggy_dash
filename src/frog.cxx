#include "frog.hxx"

Frog::Frog()
        : dims_ {467, 350, 66, 50},
          velocity_ {20, 0},
          live_ {true}
        //frog initialized in the middle x-wise, three lanes up from bottom
        // y-wise, according to top left position
{ }

ge211::Posn<float>
Frog::top_left() const
{return {dims_.x, dims_.y};}

bool
Frog::get_live() const
{return live_;}

void
Frog::kill_frog()
{live_ = false;}

bool
Frog::hits_obstacle(Obstacle const& obstacle) const
{
    //check if right side of frog is left of left side of obstacle
    if (((dims_.x + dims_.width) < obstacle.get_dims().x) or
        //check if right side of obstacle is left of left side of frog
        ((obstacle.get_dims().x + obstacle.get_dims().width) < dims_.x) or
        //check if bottom of frog is above the top of the obstacle
        ((dims_.y + dims_.height) < obstacle.get_dims().y) or
        //check if bottom of obstacle is over the top of the frog
        ((obstacle.get_dims().y + obstacle.get_dims().height) < dims_.y)){
        return false;
    }
    return true;
}

bool
Frog::hits_water(Lane lane) const
{
    if ((lane.get_lane_type() == Lane_type::river_log) or
        (lane.get_lane_type() == Lane_type::river_lily)){

        //return false if any obstacle is hit
        for (Obstacle obst : lane.get_obstacles()){
            if (hits_obstacle(obst)){
                return false;
            }
        }
        //if no obstacles hit
        return true;
    }
    //if not a river
    return false;
}

Frog
Frog::next(double dt) const
{
    Frog result(*this);
    result.dims_.x += result.velocity_.width * dt;
    return result;
}

void
Frog::move(float x)
{dims_.x += x;}

void
Frog::update_velocity(float x)
{velocity_.width = x;}