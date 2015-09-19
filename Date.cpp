#include "Date.h"
#include <iostream>
#include <sstream>
#include <string>

Date::Date() {
  setYear(0);
  setMonth(0);
  setDay(0);
  setHour(0);
  setMinute(0);

}
Date::Date(int y, int m, int d, int h, int mi) {
  setYear(y);
  setMonth(m);
  setDay(d);
  setHour(h);
  setMinute(mi);
}

int Date::getYear(void) const {
  return year_;
}

void Date::setYear(int year) {
  year_ = year;
}

int Date::getMonth(void) const {
  return month_;
}

void Date::setMonth(int month) {
  month_ = month;
}

int Date::getDay(void) const {
  return day_;
}

void Date::setDay(int day) {
  day_ = day;
}

int Date::getHour(void) const {
  return hour_;
}

void Date::setHour(int hour) {
  hour_ = hour;
}

int Date::getMinute(void) const {
  return minute_;
}

void Date::setMinute(int minute) {
  minute_ = minute;
}

bool Date::isValid(Date date) {

  // only like this can we continue to do some stupid things, or it is
  // just false.
  if (date.getYear() >= 1000 && date.getYear() <= 9999 &&
      date.getMonth() >= 1 && date.getMonth() <= 12 &&
      date.getDay() >= 1 && date.getDay() <= 31 &&
      date.getHour() >= 0 && date.getHour() < 24 &&
      date.getMinute() >= 0 && date.getMinute() < 60) {
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // if the year is leapyear
    if ((date.getYear() % 400 == 0) || (date.getYear() % 4 == 0 &&
					date.getYear() % 100 != 0)) {
      days[1] = 29;
    }

    // unfortunately the day is not correct yet.
    if (date.getDay() > days[date.getMonth() - 1])
      return false;

    return true;
  }

  return false;
}

// the str should be like yyyy-mm-dd/hh:mm, or there will
// be some bugs which will be attacked by those asshore.
Date Date::stringToDate(std::string str) {
  int y, m, d, h, mi;
  y = (str[0] - '0') * 1000 + (str[1] - '0') * 100 + (str[2] - '0') * 10 + str[3] - '0';
  m = (str[5] - '0') * 10 + str[6] - '0';
  d = (str[8] - '0') * 10 + str[9] - '0';
  h = (str[11] - '0') * 10 + str[12] - '0';
  mi = (str[14] - '0') * 10 + str[15] - '0';
  Date temp(y, m, d, h, mi);
  return temp;
}

std::string Date::dateToString(Date date) {
  string str;
  str = intToString(date.getYear()) + "-" + intToString(date.getMonth());
  str += "-" + intToString(date.getDay()) + "/";
  str += intToString(date.getHour()) + ":" + intToString(date.getMinute());
  return str;							
}

string Date::intToString(int a) {
  stringstream ss;
  string tempStr;
  ss << a;
  ss >> tempStr;
  if (tempStr.length() == 0) tempStr = "00";
  else if (tempStr.length() == 1) tempStr = "0" + tempStr;
  return tempStr;
}

Date& Date::operator=(const Date& date) {
  setYear(date.getYear());
  setMonth(date.getMonth());
  setDay(date.getDay());
  setHour(date.getHour());
  setMinute(date.getMinute());
  return *this;
}

bool Date::operator==(const Date& date) const {
  return ((getYear() == date.getYear()) && (getMonth() == date.getMonth()) &&
	  (getDay() == date.getDay()) && (getHour() == date.getHour()) &&
	  (getMinute() == date.getMinute()));
}

bool Date::operator>(const Date& date) const {
  return Date::dateToString(*this) > Date::dateToString(date);
}

bool Date::operator<(const Date& date) const {
  return Date::dateToString(*this) < Date::dateToString(date);
}

bool Date::operator>=(const Date& date) const {
  return ((*this == date) || (*this > date));
}

bool Date::operator<=(const Date& date) const {
  return ((*this == date) || (*this < date));
}
