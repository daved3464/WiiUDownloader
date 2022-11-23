// Std libs
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
// Gtk Libs
#include <gtkmm.h>
// CLibs
#include <downloader.h>
#include <gtitles.h>
#include <titleInfo.h>
// UI Libs
#include <WUDGameList.h>
#include <WUDTitle.h>


// Helpers
Glib::ustring getTitleKind(const std::shared_ptr<Glib::ObjectBase> &title) {
    if (auto wt = std::dynamic_pointer_cast<WUDTitle>(title)) {
        return wt->get_kind();
    }

    return Glib::ustring("");
}

Glib::ustring getTitleRegion(const std::shared_ptr<Glib::ObjectBase> &title) {
    if (auto wt = std::dynamic_pointer_cast<WUDTitle>(title)) {
        return wt->get_region();
    }

    return Glib::ustring("");
}

Glib::ustring getTitleName(const std::shared_ptr<Glib::ObjectBase> &title) {
    if (auto wt = std::dynamic_pointer_cast<WUDTitle>(title)) {
        return wt->get_name();
    }

    return Glib::ustring("");
}

// Member functions
WUDGameList::WUDGameList(std::shared_ptr<Gtk::Sorter> sorter)
    : Glib::ObjectBase(typeid(WUDGameList)) {
    this->setupTitles();

    this->setupKindFilter();
    this->setupRegionFilters();
    this->setupNameFilter();
    this->setupFilters();
    this->setupSorter(sorter);

    this->titlesModel = Gtk::MultiSelection::create(this->titlesSortList);
}

void WUDGameList::setupTitles() {
    this->titleEntries = getTitleEntries(TITLE_CATEGORY_ALL);
    this->titlesList = Gio::ListStore<WUDTitle>::create();

    // Append titles to model
    for (unsigned int i = 0; i < getTitleEntriesSize(TITLE_CATEGORY_ALL); i++) {
        // Setup Title ID
        char id[128];
        hex(titleEntries[i].tid, 16, id);

        titlesList->append(
                WUDTitle::create(
                        i,
                        Glib::ustring::format(id),
                        Glib::ustring::format(titleEntries[i].name),
                        Glib::ustring::format(getFormattedKind(titleEntries[i].tid)),
                        Glib::ustring::format(getFormattedRegion(titleEntries[i].region)),
                        titleEntries[i].key,
                        titleEntries[i].tid));
    }
}

void WUDGameList::setupKindFilter() {
    this->kindFilter = Gtk::StringFilter::create(
            Gtk::ClosureExpression<Glib::ustring>::create(
                    sigc::ptr_fun(&getTitleKind)));

    this->kindFilter->set_search(Glib::ustring("Game"));
}

void WUDGameList::setupRegionFilters() {
    this->setupUSAFilter();
    this->setupJapanFilter();
    this->setupEuropeFilter();
}

void WUDGameList::setupJapanFilter() {
    this->JapanFilter = Gtk::StringFilter::create(
            Gtk::ClosureExpression<Glib::ustring>::create(
                    sigc::ptr_fun(&getTitleRegion)));

    this->JapanFilter->set_ignore_case(true);
    this->JapanFilter->set_match_mode(Gtk::StringFilter::MatchMode::SUBSTRING);
    this->JapanFilter->set_search(Glib::ustring(""));
}

void WUDGameList::setupUSAFilter() {
    this->USAFilter = Gtk::StringFilter::create(
            Gtk::ClosureExpression<Glib::ustring>::create(
                    sigc::ptr_fun(&getTitleRegion)));

    this->USAFilter->set_ignore_case(true);
    this->USAFilter->set_match_mode(Gtk::StringFilter::MatchMode::SUBSTRING);
    this->USAFilter->set_search(Glib::ustring(""));
}

void WUDGameList::setupEuropeFilter() {
    this->EuropeFilter = Gtk::StringFilter::create(
            Gtk::ClosureExpression<Glib::ustring>::create(
                    sigc::ptr_fun(&getTitleRegion)));

    this->EuropeFilter->set_ignore_case(true);
    this->EuropeFilter->set_match_mode(Gtk::StringFilter::MatchMode::SUBSTRING);
    this->EuropeFilter->set_search(Glib::ustring(""));
}

void WUDGameList::setupNameFilter() {
    this->nameFilter = Gtk::StringFilter::create(
            Gtk::ClosureExpression<Glib::ustring>::create(
                    sigc::ptr_fun(&getTitleName)));

    this->nameFilter->set_search(Glib::ustring(""));
}

void WUDGameList::setupFilters() {
    auto filters = Gtk::EveryFilter::create();

    filters->append(this->JapanFilter);
    filters->append(this->USAFilter);
    filters->append(this->EuropeFilter);

    filters->append(this->kindFilter);
    filters->append(this->nameFilter);

    this->titlesFilterList = Gtk::FilterListModel::create(this->titlesList, filters);
}

void WUDGameList::setupSorter(std::shared_ptr<Gtk::Sorter> sorter) {
    this->titlesSortList = Gtk::SortListModel::create(this->titlesFilterList, sorter);
}

std::shared_ptr<Gtk::MultiSelection> WUDGameList::getModel() {
    return this->titlesModel;
}

void WUDGameList::set_kind_filter(Glib::ustring kind) {
    this->kindFilter->set_search(kind);
}

void WUDGameList::search_titles(Glib::ustring search) {
    this->nameFilter->set_search(search);
}

void WUDGameList::enable_japan_filter() {
    this->JapanFilter->set_search(Glib::ustring("Japan"));
}

void WUDGameList::disable_japan_filter() {
    this->JapanFilter->set_search(Glib::ustring(""));
}

void WUDGameList::enable_usa_filter() {
    this->USAFilter->set_search(Glib::ustring("USA"));
}

void WUDGameList::disable_usa_filter() {
    this->USAFilter->set_search(Glib::ustring(""));
}

void WUDGameList::enable_europe_filter() {
    this->EuropeFilter->set_search(Glib::ustring("Europe"));
}

void WUDGameList::disable_europe_filter() {
    this->EuropeFilter->set_search(Glib::ustring(""));
}