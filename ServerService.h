#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include "SocketException.h"
#include "ServerSocket.h"
#include "AgendaService.h"

#include <string>
#include <sstream>

using namespace std;

class ServerService {
 private:
  AgendaService agendaService;
  list<Meeting> meetingReceieved, meetingToSend;
  list<User> userReceieved, userToSend;
  string op;
  bool flag;
 public:
  ServerService();
  ~ServerService();
  void clear();
  string intToString(int a);
  void devideReceievedString(string str);
  string mergeToBeString();
  void listenAndDeal();

  bool userLogIn(std::string userName, std::string password);
  bool userRegister(std::string userName, std::string password,
                    std::string email, std::string phone);
  bool deleteUser(std::string userName, std::string password);
	// a user can only delete itself
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
