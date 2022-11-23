#include "gtitles.h"
#include "gtkmm.h"
#include <WUDColumnView.h>
#include <WUDTitle.h>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>

uint64_t getTitleId(
        const Glib::RefPtr<Glib::ObjectBase> &t,
        const Glib::RefPtr<Glib::ObjectBase> &l) {
    if (auto wt = std::dynamic_pointer_cast<WUDTitle>(l)) {
        printf("tid value: %lu \n", wt->get_tid());
        return wt->get_tid();
    }

    return 0u;
}

Glib::ustring getTitleName(
        const std::shared_ptr<Glib::ObjectBase> &t,
        const std::shared_ptr<Glib::ObjectBase> &l) {
    if (auto wt = std::dynamic_pointer_cast<WUDTitle>(l)) {
        printf("name value: %s \n", wt->get_name().c_str());
        return wt->get_name();
    }
    return Glib::ustring();
}

WUDColumnView::WUDColumnView() {
    builder = Gtk::Builder::create_from_resource("/data/column_view.ui");
    columnView = builder->get_object<Gtk::ColumnView>("column_view");

    setupIdSorter();
    setupNameSorter();
    setupColumnFactories();
    setupColumns();

    columnView->append_column(indexColumn);
    columnView->append_column(queuedColumn);
    columnView->append_column(titleIdColumn);
    columnView->append_column(kindColumn);
    columnView->append_column(regionColumn);
    columnView->append_column(nameColumn);

    columnView->sort_by_column(titleIdColumn, Gtk::SortType::ASCENDING);
}

void WUDColumnView::setModel(std::shared_ptr<Gtk::SelectionModel> model) {
    if (model) {
        columnView->set_model(model);
    }
}

std::shared_ptr<Gtk::ColumnView> WUDColumnView::getColumnView() {
    return columnView;
}

std::shared_ptr<Gtk::Sorter> WUDColumnView::getSorter() {
    return columnView->get_sorter();
}

void WUDColumnView::setupIdSorter() {
    auto prop_expression = Gtk::PropertyExpression<Glib::RefPtr<Glib::ObjectBase>>::
            create(Gtk::ListItem::get_type(), "item");

    auto closure_expression = Gtk::ClosureExpression<uint64_t>::
            create(sigc::ptr_fun(getTitleId), prop_expression);

    //titleIdSorter = Gtk::NumericSorter<uint64_t>::create(closure_expression);
}

void WUDColumnView::setupNameSorter() {
    auto prop_expression = Gtk::PropertyExpression<Glib::RefPtr<Glib::ObjectBase>>::
            create(Gtk::ListItem::get_type(), "item");

    auto closure_expression = Gtk::ClosureExpression<Glib::ustring>::
            create(sigc::ptr_fun(getTitleName), prop_expression);

    //titleNameSorter = Gtk::StringSorter::create(closure_expression);
}

void WUDColumnView::setupColumns() {
    indexColumn = builder->get_object<Gtk::ColumnViewColumn>("index_column");
    indexColumn->set_factory(indexFactory);

    queuedColumn = builder->get_object<Gtk::ColumnViewColumn>("queued_column");
    queuedColumn->set_factory(queuedFactory);

    titleIdColumn = builder->get_object<Gtk::ColumnViewColumn>("title_id_column");
    titleIdColumn->set_factory(titleIdFactory);
    titleIdColumn->set_sorter(titleIdSorter);

    kindColumn = builder->get_object<Gtk::ColumnViewColumn>("kind_column");
    kindColumn->set_factory(kindFactory);

    regionColumn = builder->get_object<Gtk::ColumnViewColumn>("region_column");
    regionColumn->set_factory(regionFactory);

    nameColumn = builder->get_object<Gtk::ColumnViewColumn>("name_column");
    nameColumn->set_factory(nameFactory);
    nameColumn->set_sorter(titleNameSorter);
}

void WUDColumnView::setupColumnFactories() {
    setupIndexFactory();
    setupQueuedFactory();
    setupTitleIdFactory();
    setupKindFactory();
    setupRegionFactory();
    setupNameFactory();
}

void WUDColumnView::setupIndexFactory() {
    indexFactory = Gtk::SignalListItemFactory::create();

    indexFactory->signal_setup().connect(sigc::mem_fun(
            *this, &WUDColumnView::setup_label_factory));

    indexFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> li) {
        auto label = dynamic_cast<Gtk::Label *>(li->get_child());

        if (label) {
            if (auto wt =
                        std::dynamic_pointer_cast<WUDTitle>(li->get_item())) {
                label->set_text(Glib::ustring::format(wt->get_index()));
            }
        }
    });
}

void WUDColumnView::setupQueuedFactory() {
    queuedFactory = Gtk::SignalListItemFactory::create();

    queuedFactory->signal_setup().connect(sigc::mem_fun(
            *this, &WUDColumnView::setup_check_button_factory));

    queuedFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> li) {
        auto check = dynamic_cast<Gtk::CheckButton *>(li->get_child());

        if (check) {
            if (auto wt =
                        std::dynamic_pointer_cast<WUDTitle>(li->get_item())) {

                check->set_active(wt->get_queued());
            }
        }
    });
}

void WUDColumnView::setupTitleIdFactory() {

    titleIdFactory = Gtk::SignalListItemFactory::create();

    titleIdFactory->signal_setup().connect(sigc::mem_fun(
            *this, &WUDColumnView::setup_label_factory));

    titleIdFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> li) {
        auto label = dynamic_cast<Gtk::Label *>(li->get_child());

        if (label) {
            if (auto wt =
                        std::dynamic_pointer_cast<WUDTitle>(li->get_item())) {
                label->set_text(Glib::ustring::format(wt->get_id()));
            }
        }
    });
}

void WUDColumnView::setupKindFactory() {

    kindFactory = Gtk::SignalListItemFactory::create();

    kindFactory->signal_setup().connect(sigc::mem_fun(
            *this, &WUDColumnView::setup_label_factory));

    kindFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> li) {
        auto label = dynamic_cast<Gtk::Label *>(li->get_child());

        if (label) {
            if (auto wt =
                        std::dynamic_pointer_cast<WUDTitle>(li->get_item())) {
                label->set_text(Glib::ustring::format(wt->get_kind()));
            }
        }
    });
}

void WUDColumnView::setupRegionFactory() {

    regionFactory = Gtk::SignalListItemFactory::create();

    regionFactory->signal_setup().connect(sigc::mem_fun(
            *this, &WUDColumnView::setup_label_factory));

    regionFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> li) {
        auto label = dynamic_cast<Gtk::Label *>(li->get_child());

        if (label) {
            if (auto wt =
                        std::dynamic_pointer_cast<WUDTitle>(li->get_item())) {
                label->set_text(Glib::ustring::format(wt->get_region()));
            }
        }
    });
}

void WUDColumnView::setupNameFactory() {

    nameFactory = Gtk::SignalListItemFactory::create();

    nameFactory->signal_setup().connect(sigc::mem_fun(
            *this, &WUDColumnView::setup_label_factory));

    nameFactory->signal_bind().connect([](const std::shared_ptr<Gtk::ListItem> li) {
        auto label = dynamic_cast<Gtk::Label *>(li->get_child());

        label->set_halign(Gtk::Align::START);

        if (label) {
            if (auto wt =
                        std::dynamic_pointer_cast<WUDTitle>(li->get_item())) {
                label->set_text(Glib::ustring::format(wt->get_name()));
            }
        }
    });
}

void WUDColumnView::setup_label_factory(const std::shared_ptr<Gtk::ListItem> li) {
    auto child = new Gtk::Label();
    li->set_child(*child);
}

void WUDColumnView::setup_check_button_factory(const std::shared_ptr<Gtk::ListItem> li) {
    auto child = new Gtk::CheckButton();
    child->set_can_focus(false);
    child->set_can_target(false);
    li->set_child(*child);
}
