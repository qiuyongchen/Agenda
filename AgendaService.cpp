#include "AgendaService.h"

AgendaService::AgendaService() {
  storage_ = NULL;
  storage_ = Storage::getInstance();
 }

AgendaService::~AgendaService() {
  delete storage_;
  storage_ = NULL;
}

// only the name and password are both right that can we login
bool AgendaService::userLogIn(std::string userName, std::string password) {

  return (storage_->queryUser([&](const User& u) {
	return (u.getName() == userName && u.getPassword() == password);} )).size();

}

// we can register those name that hadn't been used
bool AgendaService::userRegister(std::string userName, std::string password,
				 std::string email, std::string phone) {
  if (storage_->queryUser([&](const User& u) {
	return u.getName() == userName;}).size()) {
    return false;
  } else {
    storage_->createUser(User(userName, password, email, phone));
    return true;
  }
}

bool AgendaService::deleteUser(std::string userName, std::string password) {
  // only the user that can login can be deleted by our human animal, or
  // what the bird egg can we delete
  if (userLogIn(userName, password)) {
    // delete the meeting created by this user
    storage_->deleteMeeting([&](const Meeting& m) {
	return m.getSponsor() == userName;});
    // delete the user now
    storage_->deleteUser([&](const User & u) {
	return (u.getName() == userName && u.getPassword() == password);});
    return true;
  } else {

    return false;
  }
}

// just need to return the list of User, that is all
std::list<User> AgendaService::listAllUsers(void) {
  return storage_->queryUser([](const User& u) {return true;});
}

bool AgendaService::createMeeting(std::string userName, std::string title,
                               std::string participator,
                             std::string startDate, std::string endDate) {
  // if the sponsor and the participator are both alive
  if (storage_->queryUser([&](const User& u) {
	return (u.getName() == userName
		|| u.getName() == participator);}).size() == 2) {
    Date sDate = Date::stringToDate(startDate);
    Date eDate = Date::stringToDate(endDate);
    // if the title hadn't been used
    if (storage_->queryMeeting([&](const Meeting& m) {
	  return (m.getTitle() == title);
	    }).size() == 0) {
      std::list<Meeting> m1, m2;
      m1 = meetingQuery(userName, startDate, endDate);
      m2 = meetingQuery(participator, startDate, endDate);
      // if both of the sponsor and the participator have time between the
      // startDate and the endDate
      if (m1.size() == 0 && m2.size() == 0) {
	if (Date::isValid(sDate) && Date::isValid(eDate))
          if (sDate < eDate) {
	    storage_->createMeeting(Meeting(userName, participator, sDate, eDate, title));
	    return true;
	  }
      }
    }
  }
  return false;
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName,
					       std::string title) {
  std::list<Meeting> m = listAllMeetings(userName);
  std::list<Meeting> re;
  for (std::list<Meeting>::iterator it = m.begin(); it != m.end(); ++it) {
    if (it->getTitle() == title)
      re.push_back(*it);
  }
  return re;

}

std::list<Meeting> AgendaService::meetingQuery(std::string userName,
					       std::string startDate, 
                                               std::string endDate) {
  std::list<Meeting> m = listAllMeetings(userName);
  Date sDate = Date::stringToDate(startDate);
  Date eDate = Date::stringToDate(endDate);
  std::list<Meeting> re;
  for (std::list<Meeting>::iterator it = m.begin(); it != m.end(); ++it) {
    if ((it->getStartDate() <= sDate && it->getEndDate() > sDate)
	|| (it->getStartDate() < eDate && it->getEndDate() >= eDate)
	|| (it->getStartDate() >= sDate && it->getEndDate() <= eDate)) {
      re.push_back(*it);
    }
  }
  return re;
}

std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
  std::list<Meeting> m1 = listAllSponsorMeetings(userName);
  std::list<Meeting> m2 = listAllParticipateMeetings(userName);
  for (std::list<Meeting>::iterator it = m2.begin(); it != m2.end(); ++it) {
    m1.push_back(*it);
  }
  return m1;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
  return storage_->queryMeeting([&](const Meeting& m) {
      return m.getSponsor() == userName;});
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(std::string userName) {
  return storage_->queryMeeting([&](const Meeting& m) {
      return m.getParticipator() == userName;});
}

bool AgendaService::deleteMeeting(std::string userName, std::string title) {
  return storage_->deleteMeeting([&](const Meeting& m) {
      return (m.getSponsor() == userName && m.getTitle() == title);});
}

bool AgendaService::deleteAllMeetings(std::string userName) {
  return storage_->deleteMeeting([&](const Meeting& m) {
      return m.getSponsor() == userName;});
}

void AgendaService::startAgenda(void) {
 
}

void AgendaService::quitAgenda(void) {

}
