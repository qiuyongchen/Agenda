#ifndef LOGIN_H
#define LOGIN_H

//#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include "Register.h"
#include "MWin.h"
#include "ClientService.h"

class Login : public Gtk::Window {
 public:
  Login();
  virtual ~Login();

 protected:
  // Signal handlers:
  void on_button_login();
  void on_button_register();

  // child widgets:
  Gtk::Box vbox;
  Gtk::Box hbox1, hbox2, hbox3;
  Gtk::Entry entry_username;
  Gtk::Entry entry_password;
  Gtk::Label label_username;
  Gtk::Label label_password;
  Gtk::Button button_login;
  Gtk::Button button_register;

  // child windows:
  Register reg;
  MWin mainwindow;
  ClientService clientservice;
};

#endif
