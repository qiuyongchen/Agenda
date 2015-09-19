#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include "Date.h"
#include "User.h"
#include "Meeting.h"

#include "ClientSocket.h"
#include "SocketException.h"

#include <sstream>
#include <list>
#include <string>
#include <fstream>

class ClientService {
 private:
  list<Meeting> meetingReceieved, meetingToSend;
  list<User> userReceieved, userToSend;
  string op;
  bool flag;
  string ipOfServer;
 public:
  ClientService();
  ~ClientService();
  void clear();
  string intToString(int a);
  void devideReceievedString(string str);
  string mergeToBeString();
  bool userLogIn(std::string userName, std::string password);
  bool userRegister(std::string userName, std::string password,
                    std::string email, std::string phone);
  bool deleteUser(std::string userName, std::string password);
  std::list<User> listAllUsers(void);
  bool createMeeting(std::string userName, std::string title,
                     std::string participator,
                     std::string startDate, std::string endDate);
  std::list<Meeting> meetingQuery(std::string userName, std::string title);
  std::list<Meeting> meetingQuery(std::string userName, std::string startDate,
                                  std::string endDate);
  std::list<Meeting> listAllMeetings(std::string userName);
  std::list<Meeting> listAllSponsorMeetings(std::string userName);
  std::list<Meeting> listAllParticipateMeetings(std::string userName);
  bool deleteMeeting(std::string userName, std::string title);
  bool deleteAllMeetings(std::string userName);
  void startAgenda(void);
  void quitAgenda(void);

};

#endif
