#ifndef REGISTER_H
#define REGISTER_H

//#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include "ClientService.h"

class Register : public Gtk::Window {
 public:
  Register();
  virtual ~Register();

 protected:
  //Signal handlers:
  void on_button_register();
  void on_button_cancel();

  // child widgets:
  Gtk::Box vbox;
  Gtk::Box hbox1, hbox2, hbox3, hbox4, hbox5;
  Gtk::Entry entry_name, entry_password, entry_email, entry_phone;
  Gtk::Label label_name, label_password, label_email, label_phone;
  Gtk::Button button_register, button_cancel;
  ClientService clientservice;
};

#endif
