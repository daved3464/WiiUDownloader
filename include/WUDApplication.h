#pragma once

#include <WUDMainWindow.h>
#include <gtkmm.h>
#include <memory>
#include <string>

class WUDApplication {
public:
    WUDApplication(const std::string &id);

    int run(int argc, char *argv[]);

private:
    Glib::RefPtr<Gtk::Application> app{nullptr};
    WUDMainWindow *window;

    void on_activate();
};