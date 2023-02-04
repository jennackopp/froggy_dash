#include "controller.hxx"

Controller::Controller(Model& model)
        : model(model),
          view(model, mixer())
{ }

void
Controller::on_key(ge211::Key key)
{
    if (model.frog.get_live()){

        //move board accordingly and orient frog facing up
        if (key == ge211::Key::up()) {
            view.face_up();
            model.move_up();
        }

        //move board accordingly and orient frog facing down
        if (key == ge211::Key::down()){
            view.face_down();
            model.move_down();
        }

        //move frog pos right and orient frog facing right
        if (key == ge211::Key::right()) {
            view.face_right();
            model.move_right();
        }

        //move frog pos left and orient frog facing left
        if (key == ge211::Key::left()){
            view.face_left();
            model.move_left();
        }
    }
}

void
Controller::on_frame(double dt)
{
    model.on_frame(dt);

    //check for game over and do proper behavior
    bool condition_one = model.hit_by_vehicle();
    if (condition_one && model.frog.get_live()){
        view.play_splat();
        model.frog.kill_frog();
    }
    bool condition_two = model.carried_off_screen();
    if (condition_two && model.frog.get_live()){
        model.frog.kill_frog();
    }

    bool condition_three = model.falls_in_water();
    if (condition_three && model.frog.get_live()){
        view.play_splash();
        model.frog.kill_frog();
    }
}

void
Controller::draw(ge211::Sprite_set& set)
{view.draw(set);}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{return {1000, 500};}