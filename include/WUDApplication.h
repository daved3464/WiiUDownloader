#pragma once

#include <memory>
#include <string>
#include <adwaita.h>
#include <WUDGameList.h>

class WUDApplication {
public:
    WUDApplication(const std::string &id, GApplicationFlags flags = G_APPLICATION_FLAGS_NONE);

    int run(int argc, char *argv[]);

private:
    AdwApplication *app{nullptr};
    GameList *gameList;

    void on_activate(GtkApplication *app);
};