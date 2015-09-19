//#include "Login.h"
#include "Login.h"

#include <gtkmm/application.h>
#include <gtkmm/main.h>
#include <gtkmm.h>
int main(int argc, char *argv[]) {
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
     				"Login");

  Gtk::Main kit(argc, argv);

  Login login ;

  g_print("before we hide the 'Login to agenda'\n");

  //  return app->run(login);
  Gtk::Main::run(login);

  return 0;
}
