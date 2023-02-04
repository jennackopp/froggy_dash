#include "model.hxx"
#include <catch.hxx>

//check that model is initialized correctly
TEST_CASE("model initialization"){
    Model model = Model();

    //check if first three lanes are grass, as intended
    CHECK(model.lanes[0].get_lane_type() == Lane_type::grass);
    CHECK(model.lanes[1].get_lane_type() == Lane_type::grass);
    CHECK(model.lanes[2].get_lane_type() == Lane_type::grass);
    CHECK(model.lanes[3].get_lane_type() == Lane_type::grass);

    //check if no obstacles in third lane
    CHECK(model.lanes[2].get_obstacles().empty());

    //check if ten elements in model
    CHECK(model.lanes.size() == 10);

    //check getters
    CHECK(model.get_bottom_lane() == model.lanes[0]);
    CHECK(model.get_bottom_lane_index() == 0);
    CHECK(model.get_top_lane_index() == 9);
    CHECK(model.get_score() == 0);
}

//check that model moves up and down as intended, no obstacles in way of frog
TEST_CASE("model.move_up and model.move_down"){
    Model model = Model();
    model.lanes[3].clear_lane();

    //check move up
    model.move_up();
    CHECK(model.get_bottom_lane() == model.lanes[1]);
    CHECK(model.get_bottom_lane_index() == 1);
    CHECK(model.get_top_lane_index() == 10);
    CHECK(model.get_score() == 1);

    CHECK(model.lanes[0].get_shown() == false);
    CHECK(model.lanes[10].get_shown());
    CHECK(model.lanes.size() == 11);
    CHECK(model.lanes[0].get_dims().y == 500);

    bool obst_moved = true;
    for (Obstacle obst : model.lanes[0].get_obstacles()){
        if (not (obst.get_dims().y == 500)){
            obst_moved = false;
            break;
        }
    }
    CHECK(obst_moved);

    //check move down
    model.move_down();
    CHECK(model.get_bottom_lane() == model.lanes[0]);
    CHECK(model.get_bottom_lane_index() == 0);
    CHECK(model.get_top_lane_index() == 9);
    CHECK(model.get_score() == 1);

    CHECK(model.lanes[0].get_shown());
    CHECK(model.lanes[10].get_shown() == false);
    CHECK(model.lanes.size() == 11);
    CHECK(model.lanes[0].get_dims().y == 450);

    obst_moved = true;
    for (Obstacle obst : model.lanes[0].get_obstacles()){
        if (not (obst.get_dims().y == 450)){
            obst_moved = false;
            break;
        }
    }
    CHECK(obst_moved);

    //check move down when at starting pos
    model.move_down();
    CHECK(model.get_bottom_lane() == model.lanes[0]);
    CHECK(model.get_bottom_lane_index() == 0);
    CHECK(model.get_top_lane_index() == 9);
    CHECK(model.get_score() == 1);

    CHECK(model.lanes[0].get_shown());
    CHECK(model.lanes[10].get_shown() == false);
    CHECK(model.lanes.size() == 11);
    CHECK(model.lanes[0].get_dims().y == 450);

    obst_moved = true;
    for (Obstacle obst : model.lanes[0].get_obstacles()){
        if (not (obst.get_dims().y == 450)){
            obst_moved = false;
            break;
        }
    }
    CHECK(obst_moved);

    //check that score doesn't increase again for move up
    model.move_up();

    CHECK(model.get_bottom_lane() == model.lanes[1]);
    CHECK(model.get_bottom_lane_index() == 1);
    CHECK(model.get_top_lane_index() == 10);
    CHECK(model.get_score() == 1);

    CHECK(model.lanes[0].get_shown() == false);
    CHECK(model.lanes[10].get_shown());
    CHECK(model.lanes.size() == 11);
    CHECK(model.lanes[0].get_dims().y == 500);

    obst_moved = true;
    for (Obstacle obst : model.lanes[0].get_obstacles()){
        if (not (obst.get_dims().y == 500)){
            obst_moved = false;
            break;
        }
    }
    CHECK(obst_moved);
}

//check that the frog moves right and left as desired
TEST_CASE("model.move_right and model.move_left"){
    Model model = Model();

    model.move_right();
    CHECK(model.frog.top_left().x == 472);
    CHECK(model.frog.top_left().y == 350);

    model.move_left();
    CHECK(model.frog.top_left().x == 467);
    CHECK(model.frog.top_left().y == 350);
}

//check that the frog cannot move past edge of screen
TEST_CASE("model.move_right and model.move_left at edge of screen"){
    Model model = Model();

    model.frog.move(-467);
    model.move_left();
    CHECK(model.frog.top_left().x == 0);

    model.frog.move(934);
    model.move_right();
    CHECK(model.frog.top_left().x == 934);
}

//check that collision with car is registered
TEST_CASE("frog collision with car"){
    Obstacle car = Obstacle({378, 350, 88, 50}, Obstacle_name::car, {1, 0});
    Frog frog = Frog();
    car = car.next(1);
    CHECK(frog.hits_obstacle(car));
}

//check that frog moves when on log
TEST_CASE("frog moving on log")
{
    Obstacle log = Obstacle({250, 350, 227, 50}, Obstacle_name::log, {1, 0});
    Frog frog = Frog();
    log = log.next(1);
    CHECK(frog.next(1).top_left().x == 487);
}

//check that frog falling in water is registered
TEST_CASE("frog falls in water")
{
    Lane river = Lane({0, 350}, Lane_type::river_log, {1, 0});
    river.clear_lane();
    Frog frog = Frog();
    CHECK(frog.hits_water(river));
}