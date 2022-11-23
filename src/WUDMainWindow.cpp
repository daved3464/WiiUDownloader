#include "titleInfo.h"
#include <cstdio>
#include <adwaita.h>
#include <gtk/gtk.h>
#include <gtkmm.h>

#include <WUDColumnView.h>
#include <WUDGameList.h>
#include <WUDMainWindow.h>
#include <memory>

WUDMainWindow::WUDMainWindow(std::shared_ptr<Gtk::Application> app) {
    this->app = app;
    this->builder = Gtk::Builder::create_from_resource("/data/main_window.ui");

    this->setupList();
    this->setupKindFilters();
    this->setupRegionFilters();
    this->setupSearch();
    this->setupWindow();
}

void WUDMainWindow::setupList() {
    this->columnView = new WUDColumnView();
    this->gameList = new WUDGameList(this->columnView->getSorter());
    this->columnView->setModel(this->gameList->getModel());
}

void WUDMainWindow::setupKindFilters() {
    // Setup games button
    gamesButton = builder->get_widget<Gtk::ToggleButton>("games_button");
    gamesButton->set_active();
    gamesButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_button_selected),
                    Glib::ustring("Game")));

    // Setup updates button
    updatesButton = builder->get_widget<Gtk::ToggleButton>("updates_button");
    updatesButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_button_selected),
                    Glib::ustring("Update")));

    // Setup DLC button
    dlcsButton = builder->get_widget<Gtk::ToggleButton>("dlcs_button");
    dlcsButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_button_selected),
                    Glib::ustring("DLC")));

    // Setup Demo Button
    demosButton = builder->get_widget<Gtk::ToggleButton>("demo_button");
    demosButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_button_selected),
                    Glib::ustring("Demo")));

    // Setup ALL Button
    allButton = builder->get_widget<Gtk::ToggleButton>("all_button");
    allButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_button_selected),
                    Glib::ustring("")));
}

void WUDMainWindow::setupRegionFilters() {
    //- Region Buttons -//
    // Setup Japan Button
    japanButton = builder->get_widget<Gtk::CheckButton>("japan_button");
    japanButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_region_selected),
                    japanButton,
                    MCP_REGION_JAPAN));

    // Setup USA Button
    usaButton = builder->get_widget<Gtk::CheckButton>("usa_button");
    usaButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_region_selected),
                    usaButton,
                    MCP_REGION_USA));

    // Setup Europe Button
    europeButton = builder->get_widget<Gtk::CheckButton>("europe_button");
    europeButton->signal_toggled().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_region_selected),
                    europeButton,
                    MCP_REGION_EUROPE));
}

void WUDMainWindow::setupSearch() {
    auto searchEntry = builder->get_widget<Gtk::SearchEntry>("search_entry");

    searchEntry->signal_search_changed().connect(
            sigc::bind(
                    sigc::mem_fun(*this, &WUDMainWindow::on_search_changed),
                    searchEntry));
}

void WUDMainWindow::setupWindow() {
    this->mainWindow = GTK_WIDGET(builder->get_object("main_window")->gobj());
    
    auto search_bar = builder->get_object<Gtk::SearchBar>("search_bar");

    search_bar->set_search_mode();
    search_bar->set_child_visible();

    builder->get_widget<Gtk::ScrolledWindow>("scrolled_window")
            ->set_child(*this->columnView->getColumnView());

    /*
    // Show window

    // Setup Queue Add Button
    addToQueueButton = builder->get_widget<Gtk::Button>("addToQueue_button");
    addToQueueButton->signal_clicked().connect(sigc::mem_fun(*this, &WUDMainWindow::on_add_to_queue));

    // Setup Download Button
    downloadQueueButton = builder->get_widget<Gtk::Button>("downloadQueue_button");
    downloadQueueButton->signal_clicked().connect(sigc::mem_fun(*this, &WUDMainWindow::on_download_queue));


    // Setup Decrypt Contents Toggle
    decryptContentsButton = builder->get_widget<Gtk::CheckButton>("decryptContents_button");
    decryptContentsButton->signal_toggled().connect(
            sigc::bind(sigc::mem_fun(*this, &WUDMainWindow::on_decrypt_selected), decryptContentsButton));
    decryptContentsButton->set_active();

    // Setup View
    columnView = builder->get_widget<Gtk::ColumnView>("columnView");

    columnView->signal_activate().connect(
            sigc::mem_fun(*this, &WUDMainWindow::on_WUDGameList_row_activated));

    columnView->get_model()->signal_selection_changed().connect(
            sigc::mem_fun(*this, &WUDMainWindow::on_selection_changed));

    // Initial Title list sorting
    updateTitles(currentCategory, selectedRegion);

    //    Gtk::CellRendererToggle *renderer = Gtk::manage(new Gtk::CellRendererToggle());

    //  columnView->append_column("Queue", *renderer);

    //   Gtk::ListStore *columns = columnView->get_columns();

    columns->add_attribute(*renderer, "active", columns.toQueue);

    columnView->append_column("TitleID", columns.titleId);
    columnView->get_column(1)->set_sort_column(columns.titleId);

    columnView->append_column("Kind", columns.kind);

    columnView->append_column("Region", columns.region);

    columnView->append_column("Name", columns.name);
    columnView->get_column(4)->set_sort_column(columns.name);
    

    // Search for name
    columnView->set_search_column(5);

    // Sort by name by default
    columnModel->set_sort_column(GTK_TREE_SORTABLE_UNSORTED_SORT_COLUMN_ID, Gtk::SortType::SORT_ASCENDING);
    columnModel->set_sort_column(5, Gtk::SortType::SORT_ASCENDING);

    columnView->set_search_equal_func(sigc::mem_fun(*this, &WUDMainWindow::on_search_equal));
     */


    gtk_application_add_window(app->gobj(), GTK_WINDOW(mainWindow));
    gtk_widget_show(mainWindow);


    gtk_application_add_window(app->gobj(), GTK_WINDOW(mainWindow));
    gtk_widget_show(mainWindow);
}

void WUDMainWindow::on_decrypt_selected(Gtk::ToggleButton *button) {
    gameList->decryptContents = !gameList->decryptContents;
}

void WUDMainWindow::on_download_queue(Gdk::Event *ev) {
    if (gameList->queueVector.empty())
        return;
    /*     

    mainWindow->set_sensitive(false);

    for (auto queuedItem: queueVector) {
        char tid[128];
        sprintf(tid, "%016lx", queuedItem);
        download_title(tid, decryptContents);
    }

    queueVector.clear();
    mainWindow->set_sensitive(true); */
}

void WUDMainWindow::on_selection_changed() {
    /*     Glib::RefPtr <Gtk::SelectionModel> selection = columnView->get_model();
    //    Gtk::TreeModel::Row row = *selection->get_selection();
 */
    /*if (row) {
        if (row[columns.toQueue] == true) {
            addToQueueButton->set_label("Remove from queue");
        } else {
            addToQueueButton->set_label("Add to queue");
        }
    }*/
}

void WUDMainWindow::on_add_to_queue(Gdk::Event *ev) {
    /*     Glib::RefPtr <Gtk::SelectionModel> selection = columnView->get_model(); */
    // Gtk::TreeModel::Row row = *selection->get_selection();

    /*if (row) {
        row[columns.toQueue] = !row[columns.toQueue];
        if (row[columns.toQueue]) {
            queueVector.push_back(titleEntries[row[columns.index]].tid);
            addToQueueButton->set_label("Remove from queue");
        } else {
            queueVector.erase(std::remove(queueVector.begin(), queueVector.end(), titleEntries[row[columns.index]].tid), queueVector.end());
            addToQueueButton->set_label("Add to queue");
        }
    }*/
}

void WUDMainWindow::on_button_selected(const Glib::ustring title_category) {
    this->gameList->set_kind_filter(title_category);

    /*     currentCategory = title_category;
    titleEntries = getTitleEntries(currentCategory);
    updateTitles(currentCategory, selectedRegion);
    return; */
}

void WUDMainWindow::on_region_selected(Gtk::CheckButton *button, MCPRegion reg) {

    if (button->get_active()) {
        if (reg == MCP_REGION_JAPAN) {
            this->gameList->enable_japan_filter();
        }

        if (reg == MCP_REGION_USA) {
            this->gameList->enable_usa_filter();
        }

        if (reg == MCP_REGION_EUROPE) {
            this->gameList->enable_europe_filter();
        }
    }

    if (!button->get_active()) {
        if (reg == MCP_REGION_JAPAN) {
            this->gameList->disable_japan_filter();
        }

        if (reg == MCP_REGION_USA) {
            this->gameList->disable_usa_filter();
        }

        if (reg == MCP_REGION_EUROPE) {
            this->gameList->disable_europe_filter();
        }
    }
}

void WUDMainWindow::on_gamelist_row_activated(guint position) {
    /*     Glib::RefPtr <Gtk::SelectionModel> selection = columnView->get_model();

    selection->get_selection(position, 1); */

    /* if () {
        mainWindow->set_sensitive(false);
        char selectedTID[128];
        sprintf(selectedTID, "%016llx", titleEntries[title->id].tid);
        download_title(selectedTID, decryptContents);
        mainWindow->set_sensitive(true);
    }*/
}

void WUDMainWindow::on_search_changed(const Gtk::SearchEntry *entry) {
    this->gameList->search_titles(entry->get_text());
}

bool WUDMainWindow::on_search_equal(
        const Glib::RefPtr<Gtk::SelectionModel> &model,
        int column,
        const Glib::ustring &key,
        const Gtk::TreeModel::iterator &iter) {

    //  std::string string_name(name.lowercase());
    std::string string_key(key.lowercase());
    /*
    Gtk::TreeModel::Row row = *iter;
    if (row) {
        Glib::ustring name = row[columns.name];
        if (string_name.find(string_key) != std::string::npos) {
            return false;
        }

        Glib::ustring titleId = row[columns.titleId];
        if (strcmp(titleId.c_str(), key.c_str()) == 0) {
            return false;
        }
    }
    */
    return true;
}