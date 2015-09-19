#ifndef MWIN_H
#define MWIN_H

#include <string>
#include <gtkmm.h>
#include "Dialog.h"
#include "ClientService.h"

class MWin : public Gtk::Window {
 public:
  MWin();
  virtual ~MWin();

  // we have two kinds of listview : account_columns meeting_columns
 protected:
  // account listview column
  class account_columns : public Gtk::TreeModel::ColumnRecord
    {
    public:

      account_columns()
	{add(col_username); add(col_email); add(col_phone);}

      Gtk::TreeModelColumn<Glib::ustring> col_username;
      Gtk::TreeModelColumn<Glib::ustring> col_email;
      Gtk::TreeModelColumn<Glib::ustring> col_phone;
    };

  // meeting listview column
  class meeting_columns : public Gtk::TreeModel::ColumnRecord
    {
    public:
      meeting_columns()
	{
	  add(col_sponsor);
	  add(col_participator);
	  add(col_stime);
	  add(col_etime);
	  add(col_title);
	}
      Gtk::TreeModelColumn<Glib::ustring> col_sponsor;
      Gtk::TreeModelColumn<Glib::ustring> col_participator;
      Gtk::TreeModelColumn<Glib::ustring> col_title;
      Gtk::TreeModelColumn<Glib::ustring> col_stime;
      Gtk::TreeModelColumn<Glib::ustring> col_etime;
    };

  account_columns m_account_columns;
  meeting_columns m_meeting_columns;

  // we will have scrolled window
  Gtk::ScrolledWindow scrolledwindow_account;
  Gtk::ScrolledWindow scrolledwindow_meeting;
  // this is the main part, treeview!!!
  Gtk::TreeView treeview_account;
  Gtk::TreeView treeview_meeting;

  Glib::RefPtr<Gtk::ListStore> refptr_account;
  Glib::RefPtr<Gtk::ListStore> refptr_meeting;

 protected:
  //Signal handlers:
  void on_button_logout();
  void on_button_deleteaccount();
  void on_button_listuser();
  void on_button_createmeeting();
  void on_button_listmeetings();
  void on_button_listsmeetings();
  void on_button_listpmeetings();
  void on_button_querytitle();
  void on_button_querytime();
  void on_button_deletemeeting();
  void on_button_deleteameeting();

  void print_meetings(std::list<Meeting> m);
  void StringSplit(string s,char splitchar,vector<string>& vec);

  // child widgets:
  Gtk::Box hbox;

  // two parts of the window
  Gtk::Frame frame_menu;
  Gtk::Frame frame_show;
  Gtk::Frame frame_account;
  Gtk::Frame frame_meeting;


  // in the left part, there is a vbox which contains buttons
  Gtk::Box vbox_menu;
  Gtk::Box vbox_show;
  Gtk::Box vbox_account;
  Gtk::Box vbox_meeting;

  // these "DANTENG" buttons
  Gtk::Button log_out;
  Gtk::Button log_out1;
  Gtk::Button delete_account;
  Gtk::Button list_account;
  Gtk::Button creat_meeting;
  Gtk::Button list_meetings;
  Gtk::Button list_s_meetings;
  Gtk::Button list_p_meetings;
  Gtk::Button query_title;
  Gtk::Button query_time;
  Gtk::Button delete_meeting;
  Gtk::Button delete_a_meetings;
  // in the right part of the window

  Dialog dialog_comman;
  ClientService clientservice;
 public:
  std::string user_logined;
  std::string password_logined;
};

#endif
