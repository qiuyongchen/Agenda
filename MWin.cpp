#include "MWin.h"

MWin::MWin() :
  hbox(Gtk::ORIENTATION_HORIZONTAL),
  vbox_menu(Gtk::ORIENTATION_VERTICAL),
  vbox_show(Gtk::ORIENTATION_VERTICAL),
  vbox_account(Gtk::ORIENTATION_VERTICAL),
  vbox_meeting(Gtk::ORIENTATION_VERTICAL),
  log_out("Log_Out"),
  log_out1("LogdOut"),
  delete_account("Delete_Account"),
  list_account("List_Account"),
  creat_meeting("Create_Meeting"),
  list_meetings("List_Meetings"),
  list_s_meetings("List_S_Meetings"),
  list_p_meetings("List_P_Meetings"),
  query_title("Query_Title"),
  query_time("Query_Time"),
  delete_meeting("Delete_Meeting"),
  delete_a_meetings("Delete_A_Meetings")
{

  user_logined = std::string("w");
  password_logined = std::string("w");

  set_size_request(750, 400);  // set size for window
  set_title("MWin to agenda");  // set title for window
  set_resizable(FALSE);  // if resizable is FALSE then we can't change the size of the window
  set_position(Gtk::WIN_POS_CENTER);  // set position for the window

  // this window will be divided into two parts, left and right
  add(hbox);

  // add the left part and the right part into this window
  hbox.pack_start(frame_menu);
  hbox.pack_start(frame_show);

  frame_menu.unset_label();
  frame_menu.set_size_request(150, 400);
  frame_menu.set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  // in the left part, we put a vbox there
  frame_menu.add(vbox_menu);
  // vbox will contain all the buttons we will use
  vbox_menu.pack_start(log_out);
  vbox_menu.pack_start(delete_account);
  vbox_menu.pack_start(list_account);
  vbox_menu.pack_start(creat_meeting);
  vbox_menu.pack_start(list_meetings);
  vbox_menu.pack_start(list_s_meetings);
  vbox_menu.pack_start(list_p_meetings);
  vbox_menu.pack_start(query_title);
  vbox_menu.pack_start(query_time);
  vbox_menu.pack_start(delete_meeting);
  vbox_menu.pack_start(delete_a_meetings);

  log_out.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_logout));
  delete_account.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_deleteaccount));
  list_account.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_listuser));
  creat_meeting.signal_clicked().connect(sigc::mem_fun(*this,
	      &MWin::on_button_createmeeting));
  list_meetings.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_listmeetings));
  list_s_meetings.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_listsmeetings));
  list_p_meetings.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_listpmeetings));
  query_title.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_querytitle));
  query_time.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_querytime));
  delete_meeting.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_deletemeeting));
  delete_a_meetings.signal_clicked().connect(sigc::mem_fun(*this,
		      &MWin::on_button_deleteameeting));

  frame_show.unset_label();
  frame_show.set_size_request(600, 400);
  frame_show.set_shadow_type(Gtk::SHADOW_OUT);

  // in the right part, we also put a vbox there
  frame_show.add(vbox_show);

  // vbox will contain the two frame we need
  vbox_show.pack_start(frame_account);
  vbox_show.pack_start(frame_meeting);

  // this is used to show the list 'account'
  frame_account.set_label("___________________________________________________________________________");
  frame_account.set_size_request(600, 109);
  frame_account.add(vbox_account);
  vbox_account.pack_start(scrolledwindow_account);
  scrolledwindow_account.add(treeview_account);

  // Only show the scrollbars when they are necessary
  scrolledwindow_account.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  // Create the Tree model
  refptr_account = Gtk::ListStore::create(m_account_columns);
  treeview_account.set_model(refptr_account);

  // set the title of the column
  treeview_account.append_column("username", m_account_columns.col_username);
  treeview_account.append_column("email", m_account_columns.col_email);
  treeview_account.append_column("phone", m_account_columns.col_phone);

  // this is used to show the list 'meeting'
  frame_meeting.set_label("___________________________________________________________________________");
  frame_meeting.set_size_request(600, 291);
  frame_meeting.add(vbox_meeting);
  vbox_meeting.pack_start(scrolledwindow_meeting);
  scrolledwindow_meeting.add(treeview_meeting);
  // Only show the scrollbars when they are necessary
  scrolledwindow_meeting.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  // Create the Tree model
  refptr_meeting = Gtk::ListStore::create(m_meeting_columns);
  treeview_meeting.set_model(refptr_meeting);

  // set the title of the column
  treeview_meeting.append_column("sponsor", m_meeting_columns.col_sponsor);
  treeview_meeting.append_column("participator", m_meeting_columns.col_participator);
  treeview_meeting.append_column("title", m_meeting_columns.col_title);
  treeview_meeting.append_column("stime", m_meeting_columns.col_stime);
  treeview_meeting.append_column("etime", m_meeting_columns.col_etime);

  show_all_children();
}

MWin::~MWin() {

}

void MWin::on_button_logout() {
  clientservice.quitAgenda();
  refptr_account->clear();
  refptr_meeting->clear();
  hide();
}

void MWin::on_button_deleteaccount() {
  clientservice.deleteUser(user_logined, password_logined);
  hide();
}

void MWin::on_button_listuser() {
  refptr_account->clear();

  std::list<User> li = clientservice.listAllUsers();
  for (std::list<User>::iterator it = li.begin();
       it != li.end(); ++it) {
    Gtk::TreeModel::Row row = *(refptr_account->append());
    row[m_account_columns.col_username] = it->getName();
    row[m_account_columns.col_email] = it->getEmail();
    row[m_account_columns.col_phone] = it->getEmail();
  }

}

void MWin::on_button_createmeeting() {

  //  Glib::ustring u = string("ss");
  //  string s = Glib::ustring("ldd");

  dialog_comman.set_title_text(string("Create_Meeting"));
  dialog_comman.set_label_text(Glib::ustring("[title] [participator] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)"));

  Gtk::Main::run(dialog_comman);

  std::string str = dialog_comman.get_entry_text();
  dialog_comman.clear_entry_text();
  vector<string> vec;
  StringSplit(str, ' ', vec);
  std::string str1 = vec[0];
  std::string str2 = vec[1];
  std::string str3 = vec[2];
  std::string str4 = vec[3];
  clientservice.createMeeting(user_logined, str1, str2, str3, str4);
  on_button_listmeetings();
}


void MWin::on_button_listmeetings() {
  std::list<Meeting> m = clientservice.listAllMeetings(user_logined);
  print_meetings(m);
}

void MWin::on_button_listsmeetings() {
  std::list<Meeting> m = clientservice.listAllSponsorMeetings(user_logined);
  print_meetings(m);
}
void MWin::on_button_listpmeetings() {
  std::list<Meeting> m = clientservice.listAllParticipateMeetings(user_logined);
  print_meetings(m);
}

void MWin::on_button_querytitle() {
  dialog_comman.set_title_text(Glib::ustring("Query_Meeting_By_Title"));
  dialog_comman.set_label_text(Glib::ustring("[title]"));
  Gtk::Main::run(dialog_comman);
  std::string t = dialog_comman.get_entry_text();
  dialog_comman.clear_entry_text();
  std::list<Meeting> m = clientservice.meetingQuery(user_logined, t);
  print_meetings(m);
}

void MWin::on_button_querytime() {
  dialog_comman.set_title_text(Glib::ustring("Query_Meeting_By_Time"));
  dialog_comman.set_label_text(Glib::ustring(" [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"));
  Gtk::Main::run(dialog_comman);
  std::string twotime = dialog_comman.get_entry_text();
  dialog_comman.clear_entry_text();
  vector<string> vec;
  StringSplit(twotime, ' ', vec);
  std::string str1 = vec[0];
  std::string str2 = vec[1];
  std::list<Meeting> m = clientservice.meetingQuery(user_logined, str1, str2);
  print_meetings(m);
}

void MWin::on_button_deletemeeting() {
  dialog_comman.set_title_text(Glib::ustring("Delete_Meeting"));
  dialog_comman.set_label_text(Glib::ustring("[title]"));
  Gtk::Main::run(dialog_comman);
  clientservice.deleteMeeting(user_logined, dialog_comman.get_entry_text());
  dialog_comman.clear_entry_text();
  on_button_listmeetings();
}

void MWin::on_button_deleteameeting() {
  clientservice.deleteAllMeetings(user_logined);
  on_button_listmeetings();
}

void MWin::print_meetings(std::list<Meeting> meetings) {
  refptr_meeting->clear();

  for (std::list<Meeting>::iterator it = meetings.begin(); it != meetings.end();
       ++it) {
    Gtk::TreeModel::Row row = *(refptr_meeting->append());
    row[m_meeting_columns.col_sponsor] = it->getSponsor();
    row[m_meeting_columns.col_participator] = it->getParticipator();
    row[m_meeting_columns.col_title] = it->getTitle();
    row[m_meeting_columns.col_stime] = Date::dateToString(it->getStartDate());
    row[m_meeting_columns.col_etime] = Date::dateToString(it->getEndDate());
  }
}

void MWin::StringSplit(string s,char splitchar,vector<string>& vec) {
  if(vec.size()>0)//保证vec是空的  
    vec.clear();
  int length = s.length();
  int start=0;
  for(int i=0;i<length;i++) {
    if(s[i] == splitchar && i == 0) { //第一个就遇到分割符    
       start += 1;  
    }  
    else if(s[i] == splitchar) {
       vec.push_back(s.substr(start,i - start));
       start = i+1;
     }
    else if(i == length-1) {//到达尾部
      vec.push_back(s.substr(start,i+1 - start));
    }
  }
}
