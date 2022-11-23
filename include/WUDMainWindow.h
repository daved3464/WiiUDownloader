#pragma once

#include "gtitles.h"
#include "titleInfo.h"
#include <WUDColumnView.h>
#include <WUDGameList.h>
#include <gtkmm.h>
#include <memory>

class WUDMainWindow {
public:
    WUDMainWindow(std::shared_ptr<Gtk::Application> app);

    void on_decrypt_selected(Gtk::ToggleButton *button);
    void on_download_queue(Gdk::Event *ev);
    void on_selection_changed();
    void on_add_to_queue(Gdk::Event *ev);
    void on_button_selected(const Glib::ustring title_category);
    void on_region_selected(Gtk::CheckButton *button, MCPRegion region);
    void on_gamelist_row_activated(guint position);

    void on_search_changed(const Gtk::SearchEntry* entry);

    bool on_search_equal(
        const std::shared_ptr<Gtk::SelectionModel> &model,
        int column,
        const Glib::ustring &key,
        const Gtk::TreeModel::iterator &iter
    );

private:
    void setupWindow();
    void setupList();
    void setupKindFilters();
    void setupRegionFilters();
    void setupSearch();

    std::shared_ptr<Gtk::Application> app = nullptr;
    std::shared_ptr<Gtk::Builder> builder = nullptr;

    WUDColumnView *columnView;
    WUDGameList *gameList;

    // Widgets
    GtkWidget *mainWindow = nullptr;

    // Title type selectors
    Gtk::ToggleButton *gamesButton = nullptr;
    Gtk::ToggleButton *updatesButton = nullptr;
    Gtk::ToggleButton *dlcsButton = nullptr;
    Gtk::ToggleButton *demosButton = nullptr;
    Gtk::ToggleButton *allButton = nullptr;

    // Title region selectors
    Gtk::CheckButton *japanButton = nullptr;
    Gtk::CheckButton *usaButton = nullptr;
    Gtk::CheckButton *europeButton = nullptr;

    // Action buttons
    Gtk::Button *addToQueueButton = nullptr;
    Gtk::Button *downloadQueueButton = nullptr;

    Gtk::CheckButton *decryptContentsButton = nullptr;
};