﻿#ifndef AGENDA_STORAGE_H
#define AGENDA_STORAGE_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

#include <list>
#include <string>
#include <functional>

#include "User.h"
#include "Meeting.h"
#include <iostream>
using namespace std;

class Storage {
  private:
    static Storage *instance_;
    DISALLOW_COPY_AND_ASSIGN(Storage);
    Storage();
    // storage structure with list, or you have better structures
    // e.g. balanced tree
    list<User> userList_;
    list<Meeting> meetingList_;
    // File IO
    bool readFromFile(const char *fpath);
    bool writeToFile(const char *fpath);
  public:
    // singleton
    static Storage *getInstance(void);
    ~Storage();
    // CRUD for User & Meeting
    // using C++11 Function Template and Lambda Expressions
    void createUser(const User&);
    list<User> queryUser(function<bool(const User&)> filter);

    // return found users
    int updateUser(function<bool(const User&)> filter,
        function<void(User&)> switcher);
    // return the number of updated users
    int deleteUser(function<bool(const User&)> filter);
    // return the number of deleted users
    void createMeeting(const Meeting&);
    list<Meeting> queryMeeting(
                       function<bool(const Meeting&)> filter);
    // return found meetings
    int updateMeeting(function<bool(const Meeting&)> filter,
        function<void(Meeting&)> switcher);
    // return the number of updated meetings
    int deleteMeeting(function<bool(const Meeting&)> filter);
    // return the number of deleted meetings
    // File IO
    bool sync(void);
};

#endif
