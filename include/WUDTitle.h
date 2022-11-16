#pragma once

#include <glibmm.h>
#include <gtkmm.h>

class Title : public Glib::Object {
public:
    Title(unsigned int index) {
    }

    unsigned int index;
};
