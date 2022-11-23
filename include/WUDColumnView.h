#pragma once

#include <gtk/gtk.h>
#include <gtkmm.h>
#include <memory>

class WUDColumnView {
public:
    WUDColumnView();

    void setModel(std::shared_ptr<Gtk::SelectionModel> model);

    std::shared_ptr<Gtk::ColumnView> getColumnView();
    std::shared_ptr<Gtk::Sorter> getSorter();

private:
    void setupColumns();
    void setupFactoryWidgets();
    void setupColumnFactories();

    void setupIndexFactory();
    void setupQueuedFactory();
    void setupTitleIdFactory();
    void setupKindFactory();
    void setupRegionFactory();
    void setupNameFactory();

    void setupIdSorter();
    void setupNameSorter();

    void setup_label_factory(const std::shared_ptr<Gtk::ListItem> li);
    void setup_check_button_factory(const std::shared_ptr<Gtk::ListItem> li);

    static Glib::ustring get_title_tid(const std::shared_ptr<Glib::ObjectBase> this_);

    std::shared_ptr<Gtk::Builder> builder = nullptr;

    std::shared_ptr<Gtk::SelectionModel> model = nullptr;
    std::shared_ptr<Gtk::ColumnView> columnView = nullptr;

    std::shared_ptr<Gtk::ColumnViewColumn> indexColumn = nullptr;
    std::shared_ptr<Gtk::ColumnViewColumn> queuedColumn = nullptr;
    std::shared_ptr<Gtk::ColumnViewColumn> titleIdColumn = nullptr;
    std::shared_ptr<Gtk::ColumnViewColumn> kindColumn = nullptr;
    std::shared_ptr<Gtk::ColumnViewColumn> regionColumn = nullptr;
    std::shared_ptr<Gtk::ColumnViewColumn> nameColumn = nullptr;

    std::shared_ptr<Gtk::SignalListItemFactory> indexFactory = nullptr;
    std::shared_ptr<Gtk::SignalListItemFactory> queuedFactory = nullptr;
    std::shared_ptr<Gtk::SignalListItemFactory> titleIdFactory = nullptr;
    std::shared_ptr<Gtk::SignalListItemFactory> kindFactory = nullptr;
    std::shared_ptr<Gtk::SignalListItemFactory> regionFactory = nullptr;
    std::shared_ptr<Gtk::SignalListItemFactory> nameFactory = nullptr;

    std::shared_ptr<Gtk::NumericSorter<uint64_t>> titleIdSorter = nullptr;
    std::shared_ptr<Gtk::StringSorter> titleNameSorter = nullptr;
};