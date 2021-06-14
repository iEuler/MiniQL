#ifndef MINIQL_DATE_HPP
#define MINIQL_DATE_HPP

#include <iostream>
#include <string>

#include "../typedef.hpp"

namespace MiniQL {

class Date {
    // friend date_type operator-(const Date& date1, const Date& date2);
    // friend Date operator-(const Date& date1, const date_type& days);
    // friend Date operator+(const Date& date1, const date_type& days);
    // friend Date operator+(const date_type& days, const Date& date1);
    // friend std::ostream& operator<<(std::ostream& os, const Date& date) {
    //   return os << date.date_;
    // }
    
  public:
    typedef long date_type;
    typedef long date_difference_type;

    Date() = default;
    Date(double date) : date_(date) {}
    ~Date() = default;

    date_type date() const { return date_; }

    date_type addYears(Time t) const { return Date(date_ + date_type(t*365)); }
    date_type addDays(date_difference_type d) const { return Date(date_ + d); }

    Date& operator+=(date_difference_type days) {
      date_ += days;
      return *this;
    }
    Date& operator-=(date_difference_type days) {
      date_ -= days;
      return *this;
    }
    bool operator<(const Date& date) {      
      return date_ < date.date_;
    }
    bool operator<=(const Date& date) {      
      return date_ <= date.date_;
    }
    bool operator>(const Date& date) {      
      return date_ > date.date_;
    }
    bool operator>=(const Date& date) {      
      return date_ >= date.date_;
    }
    bool operator==(const Date& date) {      
      return date_ == date.date_;
    }
    operator long() const { return date_; }
    
    
  protected:
    date_type date_ = 0.0;

};

inline Date::date_difference_type operator-(const Date& date1, const Date& date2) {
  return date1.date() - date2.date();
}

inline Date operator-(const Date& date1, const Date::date_difference_type& days) {  
  Date tmp(date1);
  return tmp -= days;
}

inline Date operator+(const Date& date1, const Date::date_difference_type& days) {  
  Date tmp(date1);
  return tmp += days;
}

inline Date operator+(const Date::date_difference_type& days, const Date& date1) {  
  Date tmp(date1);
  return tmp += days;
}


class Calendar {
  public:
    Calendar() = default;
    Calendar(std::string name) : name_(name) {}

    std::string name() const { return name_; }

  protected:
    std::string name_;
};

class DayCounter {
  public:
    DayCounter() = default;
    
    std::string name() const { return name_; }
    // Time yearFraction(const Date& date1, const Date& date2) const;
    Time yearFraction(const Date& date1, const Date& date2) const {
      return 1.0 * (date2 - date1) / 365.0;
    }


  protected:
    std::string name_;
};

}  // namespace MiniQL

#endif 
