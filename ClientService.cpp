#include "ClientService.h"

ClientService::ClientService() {
  //  client_socket = ClientSocket("localhost", 30000);
  ifstream in("ip.txt");
  if (!in) {
    cerr << "open file fail" << endl;
    exit(1);
  }
  getline(in, ipOfServer);
  in.close();
}

ClientService::~ClientService() {

}

void ClientService::clear() {
  meetingReceieved.clear();
  meetingToSend.clear();
  userReceieved.clear();
  userToSend.clear();
  op.clear();
  flag = false;
}

string ClientService::intToString(int a) {
  string str;
  stringstream ss;
  ss << a;
  ss >> str;
  return str;
}

void ClientService::devideReceievedString(string str) {
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

string ClientService::mergeToBeString() {
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

bool ClientService::userLogIn(std::string userName, std::string password) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("a");
	  userToSend.push_back(User(userName, password, "", ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return flag;
}

bool ClientService::userRegister(std::string userName, std::string password,
				 std::string email, std::string phone) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("b");
	  userToSend.push_back(User(userName, password, email, phone));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return flag;

}

bool ClientService::deleteUser(std::string userName, std::string password) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("c");
	  userToSend.push_back(User(userName, password, "", ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);

	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return flag;
}

std::list<User> ClientService::listAllUsers(void) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;

      try
	{
	  op = string("d");
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return userReceieved;
}

bool ClientService::createMeeting(std::string userName, std::string title,
                               std::string participator,
                             std::string startDate, std::string endDate) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("e");
	  meetingToSend.push_back(Meeting(userName, participator, Date::stringToDate(startDate), Date::stringToDate(endDate), title));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);

	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }
  return flag;
}

std::list<Meeting> ClientService::meetingQuery(std::string userName,
					       std::string title) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("f");
	  meetingToSend.push_back(Meeting(userName, "", Date(), Date(), title));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);

	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return meetingReceieved;
}

std::list<Meeting> ClientService::meetingQuery(std::string userName,
					       std::string startDate, 
                                               std::string endDate) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("g");
	  meetingToSend.push_back(Meeting(userName, "", Date::stringToDate(startDate), Date::stringToDate(endDate), ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	  client_socket << "Test message.";
	  client_socket >> reply;
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return meetingReceieved;
}

std::list<Meeting> ClientService::listAllMeetings(std::string userName) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("h");
	  meetingToSend.push_back(Meeting(userName, "", Date(), Date(), ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return meetingReceieved;
}

std::list<Meeting> ClientService::listAllSponsorMeetings(std::string userName) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("i");
	  meetingToSend.push_back(Meeting(userName, "", Date(), Date(), ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return meetingReceieved;

}

std::list<Meeting> ClientService::listAllParticipateMeetings(std::string userName) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("j");
	  meetingToSend.push_back(Meeting(userName, "", Date(), Date(), ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return meetingReceieved;

}

bool ClientService::deleteMeeting(std::string userName, std::string title) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("k");
	  meetingToSend.push_back(Meeting(userName, "", Date(), Date(), title));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return flag;
}

bool ClientService::deleteAllMeetings(std::string userName) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;
      try
	{
	  op = string("l");
	  meetingToSend.push_back(Meeting(userName, "", Date(), Date(), ""));
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return flag;

}

void ClientService::startAgenda(void) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;

      try
	{
	  op = string("m");
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }
}

void ClientService::quitAgenda(void) {
  clear();
  try
    {
      ClientSocket client_socket ( ipOfServer, 30000 );
      std::string reply;

      try
	{
	  op = string("n");
	  client_socket << mergeToBeString();
	  client_socket >> reply;
	  devideReceievedString(reply);
	}
      catch ( SocketException& ) {}

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }
}
