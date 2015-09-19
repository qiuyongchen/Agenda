#include "Storage.h"
#include <iostream>
#include <string>
#include <fstream>
#include <list>

using namespace std;

Storage* Storage::instance_ = NULL;

Storage::Storage() {
  userList_.clear();
  meetingList_.clear();
  readFromFile("agenda.data");
}

bool Storage::readFromFile(const char *fpath) {
  string strForReadIn;
  ifstream in(fpath);
  if (!in) {
    return false;
  }
    getline(in, strForReadIn);
  //  in >> strForReadIn;
  int found = strForReadIn.find("total");
  int numOfUser = strForReadIn[found + 6] - '0';

  for (int i = 0; i < numOfUser; ++i) {
    getline(in, strForReadIn);
    //    in >> strForReadIn;

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
    User u = User(name, password, email, phone);
    createUser(u);
  }

  getline(in, strForReadIn);

  found = strForReadIn.find("total");
  int numOfMeeting = strForReadIn[found + 6] - '0';

  for (int i = 0; i < numOfMeeting; ++i) {
    getline(in, strForReadIn);
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
    createMeeting(Meeting(sponsor, participator, s, e, title));

  }
  in.close();
  return true;
}

bool Storage::writeToFile(const char *fpath) {
  ofstream out(fpath);
  if (!out) {
    return false;
  }

  out << "{collection:\"User\",total:" << userList_.size() << "}\n";
  for (list<User>::iterator it = userList_.begin(); it != userList_.end();
       ++it) {
    out << "{"
	<< "name:\"" << it->getName()
        << "\",password:\"" << it->getPassword()
	<< "\",email:\"" << it->getEmail()
        << "\",phone:\"" << it->getPhone()
	<< "\"}\n";
  }

  out << "{collection:\"Meeting\",total:" << meetingList_.size() << "}\n";
  for (list<Meeting>::iterator it = meetingList_.begin();
       it != meetingList_.end(); ++it) {
    out << "{"
	<< "sponsor:\"" << it->getSponsor()
        << "\",participator:\"" << it->getParticipator()
        << "\",sdate:\"" << Date::dateToString(it->getStartDate())
        << "\",edate:\"" << Date::dateToString(it->getEndDate())
        << "\",title:\"" << it->getTitle()
        << "\"}\n";
  }

  out.close();

  return true;
}

Storage * Storage::getInstance(void) {
  if (instance_ == NULL) {
    instance_ = new Storage();
  }
  return instance_;
}

Storage::~Storage() {
  writeToFile("agenda.data");
  instance_ = NULL;
}

void Storage::createUser(const User& u) {
  userList_.push_back(u);
  sync();
}

list<User> Storage::queryUser(function<bool(const User&)> filter) {
  list<User> temp;

  // avoid to query the empty list
  if (!userList_.empty()) {
    for (list<User>::iterator it = userList_.begin(); it != userList_.end();
         ++it) {
      if (filter(*it)) {
        temp.push_back(*it);
      }
    }
  }
  return temp;
}

int Storage::updateUser(function<bool(const User&)> filter,
			    function<void(User&)> switcher) {
  int num = 0;
  for (list<User>::iterator it = userList_.begin(); it != userList_.end();
       ++it) {
    if (filter(*it)) {
      switcher(*it);
      ++num;
    }
  }
  sync();
  return num;
}

int Storage::deleteUser(function<bool(const User&)> filter) {
  int num = 0;
  for (list<User>::iterator it = userList_.begin(); it != userList_.end();) {
    if (filter(*it)) {
      userList_.erase(it);
      ++num;
      return num;
    } else {
      ++it;
    }
  }
  sync();
  return num;
}

void Storage::createMeeting(const Meeting& m) {
  meetingList_.push_back(m);
  sync();
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting&)> filter) {
  list<Meeting> temp;
  for (list<Meeting>::iterator it = meetingList_.begin();
       it != meetingList_.end(); ++it) {
    if (filter(*it)) {
      temp.push_back(*it);
    }
  }
  
  return temp;
}

int Storage::updateMeeting(function<bool(const Meeting&)> filter,
			   function<void(Meeting&)> switcher) {
  int num = 0;
  for (list<Meeting>::iterator it = meetingList_.begin();
       it != meetingList_.end(); ++it) {
    if (filter(*it)) {
      switcher(*it);
      ++num;
    }
  }
  sync();
  return num;
}

int Storage::deleteMeeting(function<bool(const Meeting&)> filter) {
  int num = 0;
  for (list<Meeting>::iterator it = meetingList_.begin();
       it != meetingList_.end();) {
      if (filter(*it)) {
	  meetingList_.erase(it);
	  it = meetingList_.begin();
	  ++num;
	  if (meetingList_.size() == 0)	  return num;
      } else {
	  ++it;
      }
  }
  sync();
  return num;
}

bool Storage::sync(void) {
  return writeToFile("agenda.data");
}
