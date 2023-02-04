#include "view.hxx"

//colors for lanes
static ge211::Color const grass_color {0, 200, 0};
static ge211::Color const road_color {0, 0, 0};
static ge211::Color const river_color {0, 0, 255};

//DO I NEED COLOR FOR SCORE TEXT SPRITE????
static ge211::Color const score_color {255, 255, 255};

View::View(Model const& model, ge211::Mixer& mixer)
        : model_ {model},
          frog_sprite_ {"frog.png"},
          frog_transform_ {ge211::geometry::Transform::rotation(0)},
          bush_sprite_ {"bush.png"},
          car_sprite_ {"car.png"},
          truck_sprite_ {"truck.png"},
          lily_pad_sprite_ {"lily_pad.png"},
          log_sprite_ {"log.png"},
          grass_sprite_ {{1000, 50}, grass_color},
          road_sprite_ {{1000, 50}, road_color},
          river_sprite_ {{1000, 50}, river_color},
          mixer_ {mixer},
          splat_sound_ {},
          splash_sound_ {}
{
    //set up audio
    if (mixer_.is_enabled()) {
        load_audio_();
    }
}

//set frog transform to correct setting
void
View::face_up()
{
    frog_transform_ = ge211::geometry::Transform::rotation(0);
}

//set frog transform to correct setting
void
View::face_down()
{
    frog_transform_ = ge211::geometry::Transform::flip_v();
}

//set frog transform to correct setting
void
View::face_right()
{
    frog_transform_ = ge211::geometry::Transform::rotation(90);
}

//set frog transform to correct setting
void
View::face_left()
{
    frog_transform_ = ge211::geometry::Transform::rotation(-90);
}

void
View::draw(ge211::Sprite_set& set)
{
    for (Lane curr_lane : model_.lanes){
        if (curr_lane.get_shown()){
            //draw lane sprites
            if (curr_lane.get_lane_type() == Lane_type::grass) {
                set.add_sprite(grass_sprite_,
                               {static_cast<int>(curr_lane.get_dims().x),
                                static_cast<int>(curr_lane.get_dims().y)},
                               0);
            }
            if (curr_lane.get_lane_type() == Lane_type::road) {
                set.add_sprite(road_sprite_,
                               {static_cast<int>(curr_lane.get_dims().x),
                                static_cast<int>(curr_lane.get_dims().y)},
                               0);
            }
            if ((curr_lane.get_lane_type() == Lane_type::river_log) or
                (curr_lane.get_lane_type() == Lane_type::river_lily)){
                set.add_sprite(river_sprite_,
                               {static_cast<int>(curr_lane.get_dims().x),
                                static_cast<int>(curr_lane.get_dims().y)},
                               0);
            }

            //draw obstacles in each lane
            for (Obstacle obst : curr_lane.get_obstacles()){
                if (obst.get_obstacle_name() == Obstacle_name::bush){
                    set.add_sprite(bush_sprite_,
                                   {static_cast<int>(obst.get_dims().x),
                                    static_cast<int>(obst.get_dims().y)},
                                   1);
                }
                if (obst.get_obstacle_name() == Obstacle_name::car){
                    //if vehicles moving left, flip vehicle sprites
                    if (curr_lane.get_velocity().width < 0){
                        set.add_sprite(car_sprite_,
                                       {static_cast<int>(obst.get_dims().x),
                                        static_cast<int>(obst.get_dims().y)},
                                       1, ge211::geometry::Transform::flip_h());
                    } else{
                        set.add_sprite(car_sprite_,
                                       {static_cast<int>(obst.get_dims().x),
                                        static_cast<int>(obst.get_dims().y)},
                                       1);
                    }
                }
                if (obst.get_obstacle_name() == Obstacle_name::truck){
                    //if vehicles moving left, flip vehicle sprites
                    if (curr_lane.get_velocity().width < 0){
                        set.add_sprite(truck_sprite_,
                                       {static_cast<int>(obst.get_dims().x),
                                        static_cast<int>(obst.get_dims().y)},
                                       1, ge211::geometry::Transform::flip_h());
                    } else{
                        set.add_sprite(truck_sprite_,
                                       {static_cast<int>(obst.get_dims().x),
                                        static_cast<int>(obst.get_dims().y)},
                                       1);
                    }
                }
                if (obst.get_obstacle_name() == Obstacle_name::lily_pad){
                    set.add_sprite(lily_pad_sprite_,
                                   {static_cast<int>(obst.get_dims().x),
                                    static_cast<int>(obst.get_dims().y)},
                                   1);
                }
                if (obst.get_obstacle_name() == Obstacle_name::log){
                    set.add_sprite(log_sprite_,
                                   {static_cast<int>(obst.get_dims().x),
                                    static_cast<int>(obst.get_dims().y)},
                                   1);
                }
            }
        }
    }

    //draw frog sprite
    if (model_.frog.get_live()){
        set.add_sprite(frog_sprite_,
                       {static_cast<int>(model_.frog.top_left().x),
                        static_cast<int>(model_.frog.top_left().y)},
                       2, frog_transform_);
    }

    //update and draw score sprite
    ge211::Text_sprite::Builder current_score(sans18_);
    current_score.color(score_color);
    current_score << model_.get_score();
    score_sprite_.reconfigure(current_score);
    set.add_sprite(score_sprite_, {950, 10}, 3);
}

void
View::load_audio_()
{
    splat_sound_.try_load("splat.wav", mixer_);
    splash_sound_.try_load("splash.wav", mixer_);
}

void
View::play_splat() const
{
    mixer_.try_play_effect(splat_sound_);
    splat_sound_.empty();
}

void
View::play_splash() const
{
    mixer_.try_play_effect(splash_sound_);
    splash_sound_.empty();
}