#ifndef MINIQL_SCHEDULE_HPP
#define MINIQL_SCHEDULE_HPP

// #include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "date.hpp"
#include "../error.hpp"
#include "../typedef.hpp"

namespace MiniQL {

class Schedule {
  public:
    Schedule() = default;
    Schedule(const std::vector<Date>& dates,
             Period tenor = 1.0,
             Calendar calendar = Calendar())
    : dates_(dates), tenor_(tenor), calendar_(calendar) {}
    Schedule(const Date& effectiveDate,
            //  const Date& terminationDate,
             Period tenor = 1.0,
             Frequency freq = Frequency::Quarterly,
             Calendar calendar = Calendar())
    : tenor_(tenor), calendar_(calendar), 
      effectiveDate_(effectiveDate) //, terminationDate_(terminationDate)
    {
      Date prev20th = previousTwentieth(effectiveDate_);      
      Date exitDate = effectiveDate_.addYears(tenor_);
      Date next20th = prev20th;
      while (next20th < exitDate) {        
        dates_.push_back(next20th);
        Date next20th = nextTwentieth(next20th);
      }      
    } // This is simplied CDS schedule
    
    
    Size size() const { return dates_.size(); }
    const std::vector<Date>& dates() const { return dates_; }
    const Date& operator[] (Size i) const { return dates_[i]; }
    
    // Date previousDate(const Date& refDate) const;
    // Date nextDate(const Date& refDate) const;
    Date startDate() const { return dates_.front(); }
    Date endDate() const { return dates_.back(); }

    bool empty() const { return dates_.empty(); }
    const Period& tenor() const { return tenor_; }

    // Iterators
    typedef std::vector<Date>::const_iterator const_iterator;
    const_iterator begin() const { return dates_.begin(); }
    const_iterator end() const { return dates_.end(); }
    const_iterator lower_bound(const Date& d = Date()) const;

    Schedule after(const Date& truncationDate) const;
    Schedule until(const Date& truncationDate) const;
  
  private:
    Period tenor_;
    Calendar calendar_;
    Date effectiveDate_, terminationDate_;
    // Date firstDate_, nextToLastDate_;
    std::vector<Date> dates_;

};

class MakeSchedule {
  public:
    MakeSchedule() = default;
    MakeSchedule& from(const Date& effectiveDate) {
      effectiveDate_ = effectiveDate;
      return *this;
    }
    MakeSchedule& to(const Date& terminationDate) {
      terminationDate_ = terminationDate;
      return *this;
    }
    MakeSchedule& withTenor(const Period& tenor) {
      tenor_ = tenor;
      return *this;
    }
    MakeSchedule& withFrequency(Frequency freq) {
      freq_ = freq;
      return *this;
    }
    operator Schedule() const {
      return Schedule(effectiveDate_, tenor_, freq_, calendar_);
    }
    
  private:
    Period tenor_ = 1.0;
    Frequency freq_ = Frequency::Quarterly;
    Calendar calendar_ = Calendar();
    Date effectiveDate_ = Date(), terminationDate_ = Date();
    // Date firstDate_, nextToLastDate_;
    std::vector<Date> dates_;

};

Date previousTwentieth(const Date& date) {
  long d = long(date);
  d %= 365;
  d %= 91;
  if (d>=20)
    return date.addDays(20 - d);
  else
    return date.addDays(20 - d - 91);
}

Date nextTwentieth(const Date& date) {
  return previousTwentieth(date).addDays(91);
}

inline Schedule Schedule::after(const Date& truncationDate) const
{
  Schedule result = *this;

  QL_REQUIRE(truncationDate < result.dates_.back(),
             "trucation date " << truncationDate << 
             " must be before the last schedule date " <<
             result.dates_.back());
  if (truncationDate > result.dates_[0]) {
    while (result.dates_[0] < truncationDate)
      result.dates_.erase(result.dates_.begin());
    
    if (truncationDate != result.dates_.front())
      result.dates_.insert(result.dates_.begin(), truncationDate);
  }
  return result;
}

inline Schedule Schedule::until(const Date& truncationDate) const
{
  Schedule result = *this;

  QL_REQUIRE(truncationDate > result.dates_.front(),
             "trucation date " << truncationDate << 
             " must be later the first schedule date " <<
             result.dates_.front());
  if (truncationDate < result.dates_.back()) {
    while (result.dates_.back() > truncationDate)
      result.dates_.pop_back();
    
    if (truncationDate != result.dates_.back())
      result.dates_.push_back(truncationDate);
  }
  return result;
}

inline std::vector<Date>::const_iterator
Schedule::lower_bound(const Date& refDate) const
{
  return std::lower_bound(dates_.begin(), dates_.end(), refDate);
}

}  // namespace MiniQL

#endif 
