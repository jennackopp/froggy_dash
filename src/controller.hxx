#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    //CONSTRUCTOR
    Controller(Model& model);

    //MEMBER FUNCTIONS
    void on_key(ge211::Key key) override;
    void on_frame(double dt) override;
    void draw(ge211::Sprite_set& set) override;
    ge211::Dims<int> initial_window_dimensions() const override;

    //MEMBER VARIABLES
    Model& model;
    View view;
};
