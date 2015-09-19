#ifndef DIALOG_H
#define DIALOG_H

#include <gtkmm.h>

class Dialog : public Gtk::Window {
 public:
  Dialog();
  virtual ~Dialog();
  Glib::ustring get_entry_text();
  void set_label_text(Glib::ustring str);
  void set_title_text(Glib::ustring str);
  void clear_entry_text() {entry.set_text("");}
 protected:
  void on_button_ok();
  void on_button_cancel();

  Gtk::Box vbox, hbox;
  Gtk::Label label;
  Gtk::Entry entry;
  Gtk::Button button_ok, button_cancel;

  Glib::ustring entry_text;

};

#endif

