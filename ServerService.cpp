#include "ServerService.h"

ServerService::ServerService() {
  clear();
}

ServerService::~ServerService() {

}

void ServerService::clear() {
  meetingReceieved.clear();
  meetingToSend.clear();
  userReceieved.clear();
  userToSend.clear();
  op.clear();
  flag = false;
}
string ServerService::intToString(int a) {
  string str;
  stringstream ss;
  ss << a;
  ss >> str;
  return str;
}

void ServerService::devideReceievedString(string str) {
  op = str[0];
  flag = str[1] - '0';
  str = str.substr(2); // cut off the two charactor

  string strForReadIn;

  // get the first line
  int pos = str.find("\n");
  strForReadIn = str.substr(0, pos);
  if (pos + 1 != str.length()) str = str.substr(pos + 1);

  int found = strForReadIn.find("total");
  int numOfUser = strForReadIn[found + 6] - '0';

  for (int i = 0; i < numOfUser; ++i) {
    // get line
    int pos = str.find("\n");
    strForReadIn = str.substr(0, pos);
    if (pos + 1 != str.length()) str = str.substr(pos + 1);

    string name, password, email, phone;
    int posOfLeft = 0;
    int posOfRight = 0;
    for (int j = 0; j < 4; ++j) {
      posOfLeft = strForReadIn.find("\"", posOfRight + 1);
      posOfRight = strForReadIn.find("\"", posOfLeft + 1);
      string subStr = strForReadIn.substr(posOfLeft + 1, posOfRight - posOfLeft - 1);
      if (j == 0)
	name = subStr;
      else if (j == 1)
	password = subStr;
      else if (j == 2)
	email = subStr;
      else
	phone = subStr;
    }
    User u(name, password, email, phone);
    userReceieved.push_back(u);
  }

  // get line
  pos = str.find("\n");
  strForReadIn = str.substr(0, pos);
  if (pos + 1 != str.length()) str = str.substr(pos + 1);


  found = strForReadIn.find("total");
  int numOfMeeting = strForReadIn[found + 6] - '0';

  for (int i = 0; i < numOfMeeting; ++i) {
    // get line
    int pos = str.find("\n");
    strForReadIn = str.substr(0, pos);
    if (pos + 1 != str.length()) str = str.substr(pos + 1);

    string sponsor, participator, startDate, endDate, title;
    int posOfLeft = 0;
    int posOfRight = 0;
    for (int j = 0; j < 5; ++j) {
      posOfLeft = strForReadIn.find("\"", posOfRight + 1);
      posOfRight = strForReadIn.find("\"", posOfLeft + 1);
            string subStr = strForReadIn.substr(posOfLeft + 1, posOfRight- posOfLeft - 1);
      if (j == 0)
	sponsor = subStr;
      else if (j == 1)
	participator = subStr;
      else if (j == 2)
	startDate = subStr;
      else if (j == 3)
	endDate = subStr;
      else
	title = subStr;

    }
    Date s = Date::stringToDate(startDate);
    Date e = Date::stringToDate(endDate);
    meetingReceieved.push_back(Meeting(sponsor, participator, s, e, title));

  }
}

string ServerService::mergeToBeString() {
  string strToBeSend;
  strToBeSend += op + intToString(flag);

  strToBeSend += "{collection:\"User\",total:" + intToString(userToSend.size()) + "}\n";
  for (list<User>::iterator it = userToSend.begin(); it != userToSend.end();
       ++it) {
    strToBeSend += string("{")
      + string("name:\"") + it->getName()
      + string("\",password:\"") + it->getPassword()
      + string("\",email:\"") + it->getEmail()
      + string("\",phone:\"") + it->getPhone()
      + string("\"}\n");
  }

  strToBeSend += string("{collection:\"Meeting\",total:") + intToString(meetingToSend.size()) + string("}\n");
  for (list<Meeting>::iterator it = meetingToSend.begin();
       it != meetingToSend.end(); ++it) {
    strToBeSend += string("{")
      + string("sponsor:\"") + it->getSponsor()
      + string("\",participator:\"") + it->getParticipator()
      + string("\",sdate:\"") + Date::dateToString(it->getStartDate())
      + string("\",edate:\"") + Date::dateToString(it->getEndDate())
      + string("\",title:\"") + it->getTitle()
      + string("\"}\n");
  }
  return strToBeSend;
}

void ServerService::listenAndDeal() {
  cout << "the ServerService is running..." << endl << endl; 
  try
    {
    ServerSocket server_socket(30000);
    while (true) {
      ServerSocket newSocket;
      server_socket.accept(newSocket);
      try {
        while (true) {
	  clear();
          string strForReceieved;
          newSocket >> strForReceieved;
	  cout << "the server receieve :\n" << strForReceieved;
	  cout << "(-_-) : ";
          devideReceievedString(strForReceieved);

	  int opp= op[0] - 'a' + 1;
        switch(opp) {
	case 1 :
	  cout << "we had delt with login request" << endl << endl;
          flag = userLogIn(userReceieved.begin()->getName(),
			    userReceieved.begin()->getPassword());
	  break;
	case 2 :
	  cout << "we had delt with register request" << endl << endl;
          flag = userRegister(userReceieved.begin()->getName(),
			    userReceieved.begin()->getPassword(), 
			    userReceieved.begin()->getEmail(),
			      userReceieved.begin()->getPhone());
	  break;
	case 3 :
	  cout << "we had delt with deleteUser request" << endl << endl;
          flag = deleteUser(userReceieved.begin()->getName(),
			    userReceieved.begin()->getPassword());
	  break;
	case 4 :
	  cout << "we had delt with listAllUsers request" << endl << endl;
	  userToSend = listAllUsers();
	  break;
	case 5 :
	  cout << "we had delt with createMeeting request" << endl << endl;
          flag = createMeeting(meetingReceieved.begin()->getSponsor(), meetingReceieved.begin()->getTitle(),
			       meetingReceieved.begin()->getParticipator(), Date::dateToString(meetingReceieved.begin()->getStartDate()), Date::dateToString(meetingReceieved.begin()->getEndDate()));
	  break;
	case 6 :
	  cout << "we had delt with meetingQueryByTitle request" << endl << endl;
	  meetingToSend = meetingQuery(meetingReceieved.begin()->getSponsor(), meetingReceieved.begin()->getTitle());
	  break;
	case 7 :
	  cout << "we had delt with meetingQueryByDate request" << endl << endl;
	  meetingToSend = meetingQuery(meetingReceieved.begin()->getSponsor(), Date::dateToString(meetingReceieved.begin()->getStartDate()), Date::dateToString(meetingReceieved.begin()->getEndDate()));
	  break;
	case 8:
	  cout << "we had delt with listAllMeetings request" << endl << endl;
	  meetingToSend = listAllMeetings(meetingReceieved.begin()->getSponsor());
	  break;
	case 9 :
	  cout << "we had delt with listAllSponsorMeetings request" << endl << endl;
	  meetingToSend = listAllSponsorMeetings(meetingReceieved.begin()->getSponsor());
	  break;
	case 10 :
	  cout << "we had delt with listAllParticipateMeetings request" << endl << endl;
	  meetingToSend = listAllParticipateMeetings(meetingReceieved.begin()->getSponsor());
	  break;
	case 11 :
	  cout << "we had delt with deleteMeeting request" << endl << endl;
	  flag = deleteMeeting(meetingReceieved.begin()->getSponsor(), meetingReceieved.begin()->getTitle());
	  break;
	case 12 :
	  cout << "we had delt with deleteAllMeetings request" << endl << endl;
	  flag = deleteAllMeetings(meetingReceieved.begin()->getSponsor());
	  break;
	case 13 :
	  cout << "we had delt with startAgenda() request" << endl << endl;
	  startAgenda();
	  break;
	case 14 :
	  cout << "we had delt with quitAgenda() request" << endl << endl;
	  quitAgenda();
	  break;
	}
	string strForSend = mergeToBeString();
	newSocket << strForSend;
	clear();
	}
      }
      catch (SocketException& ) {}
    }
    }
  catch (SocketException & e) {
    cout << "Exception was caugth:" << e.description() << endl;
  }
}

bool ServerService::userLogIn(std::string userName, std::string password) {
  return agendaService.userLogIn(userName, password);
}

bool ServerService::userRegister(std::string userName, std::string password,
				 std::string email, std::string phone) {
  return agendaService.userRegister(userName, password, email, phone);
}

bool ServerService::deleteUser(std::string userName, std::string password) {
  return agendaService.deleteUser(userName, password);
}

	// a user can only delete itself
std::list<User> ServerService::listAllUsers(void) {
  return agendaService.listAllUsers();
}

bool ServerService::createMeeting(std::string userName, std::string title,
                     std::string participator,
				  std::string startDate, std::string endDate) {
  return agendaService.createMeeting(userName, title, participator, startDate, endDate);
}

std::list<Meeting> ServerService::meetingQuery(std::string userName, std::string title) {
  return agendaService.meetingQuery(userName, title);
}

std::list<Meeting> ServerService::meetingQuery(std::string userName, std::string startDate,
						 std::string endDate) {
  return agendaService.meetingQuery(userName, startDate, endDate);
}

std::list<Meeting> ServerService::listAllMeetings(std::string userName) {
  return agendaService.listAllMeetings(userName);
}
std::list<Meeting> ServerService::listAllSponsorMeetings(std::string userName) {
  return agendaService.listAllSponsorMeetings(userName);
}

std::list<Meeting> ServerService::listAllParticipateMeetings(std::string userName) {
  return agendaService.listAllParticipateMeetings(userName);
}

bool ServerService::deleteMeeting(std::string userName, std::string title) {
  return agendaService.deleteMeeting(userName, title);
}

bool ServerService::deleteAllMeetings(std::string userName) {
  return agendaService.deleteAllMeetings(userName);
}

void ServerService::startAgenda(void) {

}

void ServerService::quitAgenda(void) {

}
