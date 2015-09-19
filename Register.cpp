#include "Register.h"

Register::Register() :
  label_name("username"),
  label_password("password"),
  label_email("email"),
  label_phone("phone"),
  button_register("register"),
  button_cancel("cancel"),
  vbox(Gtk::ORIENTATION_VERTICAL)
{

  set_size_request(300, 380);  // set size for window
  set_title("Register to agenda");  // set title for window
  set_resizable(FALSE);  // if resizable is FALSE then we can't change the size of the window
  set_position(Gtk::WIN_POS_CENTER);  // set position for the window

  add(vbox);

  vbox.pack_start(hbox1);

  hbox1.pack_start(label_name);  // pack label into box
  label_name.set_size_request(100,10);  // label is widget so we can request size for it
  hbox1.pack_start(entry_name);

  vbox.pack_start(hbox2);
  hbox2.pack_start(label_password);
  label_password.set_size_request(100,10);
  hbox2.pack_start(entry_password);

  vbox.pack_start(hbox3);
  hbox3.pack_start(label_email);
  label_email.set_size_request(100,10);
  hbox3.pack_start(entry_email);

  vbox.pack_start(hbox4);
  hbox4.pack_start(label_phone);
  label_phone.set_size_request(100,10);
  hbox4.pack_start(entry_phone);

  vbox.pack_start(hbox5);
  hbox5.pack_start(button_register);
  hbox5.pack_start(button_cancel);

  //  add(button_register);

  button_register.signal_clicked().connect(sigc::mem_fun(*this,
			 &Register::on_button_register));
  button_cancel.signal_clicked().connect(sigc::mem_fun(*this,
			 &Register::on_button_cancel));

  show_all_children();

}

Register::~Register() {

}

void Register::on_button_register() {
  while(true) {
    if (clientservice.userRegister(entry_name.get_text(), entry_password.get_text(), entry_email.get_text(), entry_phone.get_text())) {
      break;
    } else {
      g_print("register fail \n!");
      break;
    }
  }

  entry_name.set_text("");
  entry_password.set_text("");
  entry_email.set_text("");
  entry_phone.set_text("");

  this->hide();
  //  Gtk::Main::run(
}

void Register::on_button_cancel() {
  this->hide();
  //  Gtk::Main::run(
}
