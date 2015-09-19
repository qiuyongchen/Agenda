#include "Login.h"
#include <iostream>

Login::Login()
  : label_username("username"),
    label_password("password"),
    button_login("login"),
    button_register("register"),
    vbox(Gtk::ORIENTATION_VERTICAL)
{
  // window
  set_size_request(300, 177);
  //  set_title("Login to agenda");
  set_resizable(FALSE);
  set_position(Gtk::WIN_POS_CENTER);

  add(vbox);

  vbox.pack_start(hbox1);
  hbox1.pack_start(label_username);
  hbox1.pack_start(entry_username);

  vbox.pack_start(hbox2);
  hbox2.pack_start(label_password);
  hbox2.pack_start(entry_password);

  vbox.pack_start(hbox3);
  hbox3.pack_start(button_login);
  hbox3.pack_start(button_register);
  button_register.signal_clicked().connect(sigc::mem_fun(*this,
			 &Login::on_button_register));

  button_login.signal_clicked().connect(sigc::mem_fun(*this,
			 &Login::on_button_login));
  show_all_children();
}

Login::~Login() {

}

void Login::on_button_login() {
  while(true) {
    if (clientservice.userLogIn(entry_username.get_text(), entry_password.get_text())) {
      mainwindow.user_logined = entry_username.get_text();
      mainwindow.password_logined = entry_password.get_text();
      break;
    } else {
      g_print("username or password not true! \n");
      break;
    }
  }

  this->hide();
  Gtk::Main::run(mainwindow);
  Gtk::Main::run(*this);
}

void Login::on_button_register() {
  this->hide();
  Gtk::Main::run(reg);
  Gtk::Main::run(*this);
}

