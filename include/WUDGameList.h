#pragma once

#include <gtkmm.h>
#include <string>
#include <vector>

#include <gtitles.h>
#include <keygen.h>
#include <titleInfo.h>

class GameList {
public:
    GameList(
            GtkApplication *app,
            std::shared_ptr<Gtk::Builder> builder,
            const TitleEntry *titleEntries);

    ~GameList();

    void updateTitles(TITLE_CATEGORY cat, MCPRegion reg);

    // Setup methods
    void setupColumns();

    void setupWindow();

    // Signals
    void on_gamelist_row_activated(guint position);

    void on_button_selected(Gdk::Event *ev, TITLE_CATEGORY cat);

    void on_region_selected(Gtk::ToggleButton *button, MCPRegion reg);

    void on_add_to_queue(Gdk::Event *ev);

    void on_selection_changed();

    void on_download_queue(Gdk::Event *ev);

    void on_decrypt_selected(Gtk::ToggleButton *button);

    bool on_search_equal(
            const std::shared_ptr<Gtk::SelectionModel> &model,
            int column,
            const Glib::ustring &key,
            const Gtk::TreeModel::iterator &iter

    );

    // Getters
    GtkWidget *getWindow() { return mainWindow; }

private:
    bool decryptContents = false;
    std::vector<uint64_t> queueVector = {};

    // Initial Data Variables
    const TitleEntry *titleEntries;
    TITLE_CATEGORY currentCategory = TITLE_CATEGORY_GAME;
    MCPRegion selectedRegion = (MCPRegion) (MCP_REGION_JAPAN | MCP_REGION_USA | MCP_REGION_EUROPE);

    std::shared_ptr<Gtk::Builder> builder;
    std::shared_ptr<Gtk::MultiSelection> titlesModel;

    // Widgets
    GtkApplication *app = nullptr;
    GtkWidget *mainWindow = nullptr;
    Gtk::ColumnView *columnView = nullptr;

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