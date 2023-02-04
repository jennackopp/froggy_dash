#pragma once

#include "model.hxx"

class View
{
public:
    //CONSTRUCTOR
    View(Model const& model, ge211::Mixer& mixer);

    //MEMBER FUNCTIONS
    void face_up();
    void face_down();
    void face_right();
    void face_left();
    void draw(ge211::Sprite_set& set);
    void play_splat() const;
    void play_splash() const;

private:
    //loads the audio clips if possible, called in constructor
    //followed bejeweled example as template for setting up audio
    void load_audio_();

    //MEMBER VARIABLES
    Model const& model_;

    ge211::Font sans18_{"sans.ttf", 18};
    ge211::Text_sprite score_sprite_;

    ge211::Image_sprite frog_sprite_;
    ge211::Transform frog_transform_;
    ge211::Image_sprite bush_sprite_;
    ge211::Image_sprite car_sprite_;
    ge211::Image_sprite truck_sprite_;
    ge211::Image_sprite lily_pad_sprite_;
    ge211::Image_sprite log_sprite_;

    ge211::Rectangle_sprite const grass_sprite_;
    ge211::Rectangle_sprite const road_sprite_;
    ge211::Rectangle_sprite const river_sprite_;

    ge211::Mixer& mixer_;
    ge211::Sound_effect splat_sound_;
    ge211::Sound_effect splash_sound_;
};
