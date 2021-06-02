#ifndef MINIQL_EXERCISE_HPP
#define MINIQL_EXERCISE_HPP

#include <vector>

#include "time/date.hpp"
#include "typedef.hpp"

namespace MiniQL {

class Exercise {
  public:
    enum Type {
        American, Bermudan, European
    };

    Exercise() = default;
    explicit Exercise(Type type) : type_(type) {}    
    Exercise(Type type, const std::vector<Date>& dates) : type_(type), dates_(dates) {}    
    virtual ~Exercise() = default;

    Type type() const { return type_; }
    Date date(size_t index) const { return dates_[index]; }
    Date lastDate() const { return dates_.back(); }
    const std::vector<Date>& dates() const { return dates_; }
    
  protected:
    std::vector<Date> dates_;
    Type type_;
    
};

}  // namespace MiniQL

#endif 
