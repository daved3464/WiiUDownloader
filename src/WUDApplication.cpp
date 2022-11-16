#include "WUDGameList.h"
#include "giomm/resource.h"
#include "gtitles.h"
#include "gtkmm/builder.h"
#include <WUDApplication.h>
#include <adwaita.h>
#include <cstdio>
#include <gtkmm.h>
#include <iostream>

WUDApplication::WUDApplication(const std::string &id, GApplicationFlags flags)
    : app{adw_application_new(id.c_str(), flags)} {
    g_signal_connect(app, "activate", G_CALLBACK((void (*)(GtkApplication *, gpointer))[](GtkApplication * app, gpointer data) {
                         reinterpret_cast<WUDApplication *>(data)->on_activate(app);
                     }),
                     this);
}

int WUDApplication::run(int argc, char **argv) {
    return g_application_run(G_APPLICATION(app), argc, argv);
}

void WUDApplication::on_activate(GtkApplication *app) {
    adw_init();
    
    this->gameList = new GameList(
            app,
            Gtk::Builder::create_from_resource("/data/wiiudownloader.ui"),
            getTitleEntries(TITLE_CATEGORY_GAME));
}