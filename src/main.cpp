#include "gtkmm/application.h"
#include "sigc++/adaptors/bind.h"
#include "sigc++/functors/mem_fun.h"
#include <gtkmm.h>
#include <iostream>

#include <WUDApplication.h>
#include <WUDGameList.h>

int main(int argc, char *argv[]) {
    WUDApplication app("com.github.Xpl0itU.WiiUDownloader");

    return app.run(argc, argv);
}