#ifndef MINIQL_DATE_HPP
#define MINIQL_DATE_HPP

#include <iostream>
#include <string>

#include "../typedef.hpp"

namespace MiniQL {

class Date {
    // friend Time operator-(const Date& date1, const Date& date2);
    // friend Date operator-(const Date& date1, const Time& years);
    // friend Date operator+(const Date& date1, const Time& years);
    // friend Date operator+(const Time& years, const Date& date1);
    // friend std::ostream& operator<<(std::ostream& os, const Date& date) {
    //   return os << date.date_;
    // }
  public:
    Date() = default;
    Date(double date) : date_(date) {}
    ~Date() = default;

    Time date() const { return date_; }

    Date& operator+=(Time years) {
      date_ += years;
      return *this;
    }
    Date& operator-=(Time years) {
      date_ -= years;
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
    operator Time() const { return date_; }
    
    
  protected:
    Time date_ = 0.0;

};

inline Time operator-(const Date& date1, const Date& date2) {
  return date1.date() - date2.date();
}

inline Date operator-(const Date& date1, const Time& years) {  
  return Date(date1.date() - years);
}

inline Date operator+(const Date& date1, const Time& years) {  
  return Date(date1.date() + years);
}

inline Date operator+(const Time& years, const Date& date1) {  
  return Date(date1.date() + years);
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
      return date1 - date2;
    }


  protected:
    std::string name_;
};

}  // namespace MiniQL

#endif 
