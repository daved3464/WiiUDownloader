#pragma once

#include "gtitles.h"
#include "titleInfo.h"
#include <cstdint>
#include <gtkmm.h>
#include <memory>

class WUDTitle : public Glib::Object {
public:
    static std::shared_ptr<WUDTitle> create(
            uint index,
            Glib::ustring id,
            Glib::ustring name,
            Glib::ustring kind,
            Glib::ustring region,
            const TITLE_KEY key,
            const uint64_t tid) {
        return Glib::make_refptr_for_instance(new WUDTitle{
                index,
                id,
                name,
                kind,
                region,
                key,
                tid});
    };

    uint get_index() { return index; };

    Glib::ustring get_id() { return id; };
    Glib::ustring get_name() { return name; };
    Glib::ustring get_kind() { return kind; };
    Glib::ustring get_region() { return region; };

    TITLE_KEY get_key() { return key; };
    uint64_t get_tid() { return tid; };

    bool get_queued() { return queued; };
    bool get_selected() { return selected; };

private:
    WUDTitle(uint index,
             Glib::ustring id,
             Glib::ustring name,
             Glib::ustring kind,
             Glib::ustring region,
             const TITLE_KEY key,
             const uint64_t tid) : index(index),
                                   queued(false),
                                   selected(false),
                                   id(id),
                                   kind(kind),
                                   region(region),
                                   name(name),
                                   key(key),
                                   tid(tid){};
    uint index;
    bool queued;
    bool selected;
    Glib::ustring id;
    Glib::ustring kind;
    Glib::ustring region;
    Glib::ustring name;

    const TITLE_KEY key;
    const uint64_t tid;
};