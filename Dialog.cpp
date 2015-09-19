#include "Dialog.h"

Dialog::Dialog() :
  vbox(Gtk::ORIENTATION_VERTICAL),
  button_ok("OK"),
  button_cancel("CANCEl")
{
  set_size_request(600, 100);
  set_resizable(FALSE);
  set_position(Gtk::WIN_POS_CENTER);

  add(vbox);
  vbox.pack_start(label);
  vbox.pack_start(entry);
  vbox.pack_start(hbox);

  hbox.pack_start(button_ok);
  hbox.pack_start(button_cancel);

  button_ok.signal_clicked().connect(sigc::mem_fun(*this,
		      &Dialog::on_button_ok));
  button_cancel.signal_clicked().connect(sigc::mem_fun(*this,
		      &Dialog::on_button_cancel));

  show_all_children();
}

Dialog::~Dialog() {

}

Glib::ustring Dialog::get_entry_text() {
  return entry.get_text();
}

void Dialog::set_label_text(Glib::ustring str) {
  label.set_text(str);
}

void Dialog::set_title_text(Glib::ustring str) {
  set_title(str);
}

void Dialog::on_button_ok() {
  entry_text = get_entry_text();
  hide();
}

void Dialog::on_button_cancel() {
  hide();
}
