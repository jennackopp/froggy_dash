#include "obstacle.hxx"

Obstacle::Obstacle(
        Rectangle dims,
        Obstacle_name name,
        Velocity velocity)

        : dims_ {dims},
          obstacle_name_ {name},
          velocity_ {velocity}
{ }

Rectangle
Obstacle::get_dims() const
{return dims_;}

Obstacle_name
Obstacle::get_obstacle_name() const
{return obstacle_name_;}

void
Obstacle::move_obst(float y)
{
    dims_ = {dims_.x, dims_.y + y, dims_.width, dims_.height};
}

bool
Obstacle::obst_overlap(Obstacle obst) const
{
    if (((obst.get_dims().x + obst.get_dims().width) < dims_.x) or
        ((dims_.x + dims_.width) < obst.get_dims().x)){
        return false;
    } else{
        return true;
    }
}

//used code from brick_out (Ball::next)
Obstacle
Obstacle::next(double dt) const
{
    Obstacle result(*this);
    result.dims_.x += result.velocity_.width * dt;
    return result;
}

void
Obstacle::update_velocity()
{velocity_ *= 1.1;}

bool
operator==(Obstacle const& o1, Obstacle const& o2)
{
    return o1.dims_ == o2.dims_ &&
           o1.obstacle_name_ == o2.obstacle_name_ &&
           o1.velocity_ == o2.velocity_;
}