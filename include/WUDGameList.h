#pragma once

#include "WUDColumnView.h"
#include <WUDTitle.h>
#include <gtkmm.h>
#include <memory>
#include <string>
#include <vector>

#include <gtitles.h>
#include <keygen.h>
#include <titleInfo.h>

class WUDGameList : Glib::ObjectBase {
private:
    void setupTitles();
    void setupFilters();
    void setupSorter(std::shared_ptr<Gtk::Sorter> sorter);

    void setupRegionFilters();
    
    void setupUSAFilter();
    void setupJapanFilter();
    void setupEuropeFilter();

    void setupKindFilter();
    void setupNameFilter();

    // Initial Data Variables
    std::shared_ptr<Gio::ListStore<WUDTitle>> titlesList;
    std::shared_ptr<Gtk::FilterListModel> titlesFilterList;
    std::shared_ptr<Gtk::SortListModel> titlesSortList;
    std::shared_ptr<Gtk::MultiSelection> titlesModel;

    std::shared_ptr<Gtk::StringFilter> USAFilter;
    std::shared_ptr<Gtk::StringFilter> JapanFilter;
    std::shared_ptr<Gtk::StringFilter> EuropeFilter;

    std::shared_ptr<Gtk::StringFilter> kindFilter;
    std::shared_ptr<Gtk::StringFilter> nameFilter;

    const TitleEntry *titleEntries;

public:
    bool decryptContents = false;
    std::vector<uint64_t> queueVector = {};

public:
    WUDGameList(std::shared_ptr<Gtk::Sorter> sorter);

    std::shared_ptr<Gtk::MultiSelection> getModel();    

    void enable_usa_filter();
    void enable_japan_filter();
    void enable_europe_filter();

    void disable_usa_filter();
    void disable_japan_filter();
    void disable_europe_filter();

    void set_kind_filter(Glib::ustring kind);
    void search_titles(Glib::ustring search);
};