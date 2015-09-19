#include "AgendaUI.h"
#include <iomanip>

AgendaUI::AgendaUI() {

}

void AgendaUI::OperationLoop(void) {
  startAgenda();
  bool continueToLoop = true;
  string op;
  while(continueToLoop) {
    op = getOperation();
    continueToLoop = executeOperation(op);
  }
}

void AgendaUI::startAgenda(void) {
  userName_.clear();
  userPassword_.clear();
}

std::string AgendaUI::getOperation() {
  std::string op;
  
  bool operationInValid = true;
  while (operationInValid) {
    inform();
    cin >> op;
    if (userName_.empty()) {
      if (op == "l" || op == "r" || op == "q") {
	operationInValid = false;
      }
    } else if (!userName_.empty()) {
      if ( op == "o" || op == "dc" || op == "lu" || op == "cm" || op == "la"
                   || op == "las" || op == "lap" || op == "qm" || op == "qt"
                                                 || op == "dm" || op == "da") {
	operationInValid = false;
      }
    }
  }
  
  return op;
}

bool AgendaUI::executeOperation(std::string op) {
  int f;
  if (op == "l")
    f = 1;
  else if (op == "r")
    f = 2;
  else if (op == "q")
    f = 3;
  else if (op == "o")
    f = 4;
  else if (op == "dc")
    f = 5;
  else if (op == "lu")
    f = 6;
  else if (op == "cm")
    f = 7;
  else if (op == "la")
    f = 8;
  else if (op == "las")
    f = 9;
  else if (op == "lap")
    f = 10;
  else if (op == "qm")
    f = 11;
  else if (op == "qt")
    f = 12;
  else if (op == "dm")
    f = 13;
  else if (op == "da")
    f = 14;

  switch(f) {
  case 1 : userLogIn(); break;
  case 2 : userRegister(); break;
    //if f is 'q', then we should tell the server we have to quit
  case 3 : quitAgenda(); break;
  case 4 : userLogOut(); break;
  case 5 : deleteUser(); break;
  case 6 : listAllUsers(); break;
  case 7 : createMeeting(); break;
  case 8 : listAllMeetings(); break;
  case 9 : listAllSponsorMeetings(); break;
  case 10 : listAllParticipateMeetings(); break;
  case 11 : queryMeetingByTitle(); break;
  case 12 : queryMeetingByTimeInterval(); break;
  case 13 : deleteMeetingByTitle(); break;
  case 14 : deleteAllMeetings(); break;
  }
  // if we input "q", then we should not 'continueToLoop'
  if (op == "q")
    return false;
  else
    return true;
}

void AgendaUI::userLogIn(void) {
  cout << "[log in] [user name] [password]" << endl;
  cout << "[log in] ";
  string name, password;
  cin >> name >> password;

  // y*****the name is " << name << "the password is " << 
  //  password << endl;


  if (agendaService_.userLogIn(name, password)) {
    cout << "[log in] succeed!" << endl;
    userName_ = name;
    userPassword_ = password;
  } else {
    cout << "[error] log in fail!" << endl;
  }
}

void AgendaUI::userRegister(void) {
  cout << "[register] [user name] [password] [email] [phone]" << endl;
  cout << "[register] ";
  string name, password, email, phone;

  cin >> name >> password >> email >> phone;
  if (agendaService_.userRegister(name, password, email, phone)) {
    cout << "[register] succeed!" << endl;
  } else {
    cout << "[error] register fail!" << endl;
  }
}

void AgendaUI::quitAgenda(void) {
  agendaService_.quitAgenda();
  userName_.clear();
  userPassword_.clear();
}

void AgendaUI::userLogOut(void) {
  quitAgenda();
  startAgenda();
}

void AgendaUI::deleteUser(void) {
  agendaService_.deleteUser(userName_, userPassword_);
  cout << "[delete agenda account] succeed!" << endl;
  userLogOut();
}

void AgendaUI::listAllUsers(void) {
  cout << "[list all users]" << endl;
  cout << "name      email               phone" << endl;
  std::list<User> li = agendaService_.listAllUsers();
  for (std::list<User>::iterator it = li.begin();
       it != li.end(); ++it) {
    cout << setw(10) << left << it->getName() << setw(20) << left << it->getEmail()
	 << setw(20) << left << it->getPhone() << endl;
  }
}

void AgendaUI::createMeeting(void) {
  cout << "[create meeting] [title] [participator] [start time(";
  cout << "yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
  cout << "[create meeting]";
  string participator, startTime, endTime, title;
  cin >> title >> participator >> startTime >> endTime;
  if (agendaService_.createMeeting(userName_, title, participator, startTime, endTime))
    cout << "[create meeting] succeed!" << endl;
  else
    cout << "[error] create meeting fail!" << endl;
}

void AgendaUI::listAllMeetings(void) {
  cout << "[list all meetings]" << endl;
  std::list<Meeting> m = agendaService_.listAllMeetings(userName_);
  printMeetings(m);
}

void AgendaUI::listAllSponsorMeetings(void) {
  cout << "[list all sponsor meetings]" << endl;
  std::list<Meeting> m = agendaService_.listAllSponsorMeetings(userName_);
  printMeetings(m);
}

void AgendaUI::listAllParticipateMeetings(void) {
  cout << "[list all participate meetings]" << endl;
  std::list<Meeting> m = agendaService_.listAllParticipateMeetings(userName_);
  printMeetings(m);
}

void AgendaUI::queryMeetingByTitle(void) {
  cout << "[query meeting] [title]:" << endl;
  cout << "[query meeting] ";
  string title;
  cin >> title;
  std::list<Meeting> m = agendaService_.meetingQuery(userName_, title);
  printMeetings(m);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
  cout << "[query meetings] [start time(";
  cout << "yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
  cout << "[query meetings] ";
  string s, e;
  cin >> s >> e;
  std::list<Meeting> m = agendaService_.meetingQuery(userName_, s, e);
  cout << "[query meetings]" << endl;
  printMeetings(m);
}

void AgendaUI::deleteMeetingByTitle(void) {
  cout << "[delete meeting] [title]:" << endl;
  cout << "[delete meeting] ";
  string title;
  cin >> title;
  cout << "what you input is " << title << " and " << userName_ <<endl;

  if (agendaService_.deleteMeeting(userName_, title))
    cout << "[delete meeting by title] succed!" << endl;
  else
    cout << "[error] delete meeting fail!" << endl;
}

void AgendaUI::deleteAllMeetings(void) {
  agendaService_.deleteAllMeetings(userName_);
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
  cout << "title               sponsor            participator        start time          end time" << endl;
  for (std::list<Meeting>::iterator it = meetings.begin(); it != meetings.end();
       ++it) {
    cout << setw(20) << left << it ->getTitle() << setw(20) << left << it ->getSponsor()
	 << setw(20) << left << it ->getParticipator() << setw(20) << left << Date::dateToString(it ->getStartDate())
	 << setw(20) << left << Date::dateToString(it ->getEndDate()) << endl;
  }
}

void AgendaUI::inform() {
  if (userName_.empty()) {
    cout << endl;
    cout << "------------------------------ Agenda ------------------------------" << endl;
    cout << "Action : " << endl;
    cout << "l   - log in Agenda by user name and password" << endl;
    cout << "r   - register an Agenda account" << endl;
    cout << "q   - quit Agenda" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    cout << "Agenda : ~$";
  } else  {
    cout << endl;
    cout << "------------------------------ Agenda ------------------------------" << endl;
    cout << "Action : " << endl;
    cout << "o   - log out Agenda" << endl;
    cout << "dc  - delete Agenda account" << endl;
    cout << "lu  - list all Agenda user" << endl;
    cout << "cm  - create a meeting" << endl;
    cout << "la  - list all meetings" << endl;
    cout << "las - list all sponsor meetings" << endl;
    cout << "lap - list all participate meetings" << endl;
    cout << "qm  - query meeting by title" << endl;
    cout << "qt  - query meeting by time interval" << endl;
    cout << "dm  - delete meeting by title" << endl;
    cout << "da  - delete all meetings" << endl;
    cout << "--------------------------------------------------------------------" << endl;
    cout << "Agenda@" << userName_ << " : # ";
  }
}
