#include <WUDApplication.h>
#include <WUDMainWindow.h>
#include <adwaita.h>
#include <gtkmm.h>

WUDApplication::WUDApplication(const std::string &id)
    : app{Gtk::Application::create(id.c_str())} {

    app->signal_startup().connect([]() { adw_init(); });
    app->signal_activate().connect([&]() { this->on_activate(); });
}

int WUDApplication::run(int argc, char **argv) {
    return app->run(argc, argv);
}

void WUDApplication::on_activate() {
    this->window = new WUDMainWindow(app);
}