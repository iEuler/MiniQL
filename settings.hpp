#ifndef MINIQL_SETTINGS_HPP
#define MINIQL_SETTINGS_HPP

#include "patterns/singleton.hpp"
#include "time/date.hpp"
#include "typedef.hpp"

namespace MiniQL {

class Settings : public Singleton<Settings> {
    friend class Singleton<Settings>;
  private:
    Settings() = default;
  public:    
    Date& evaluationDate();            
  private:    
    Date evaluationDate_;
    
    
    
};

inline Date& Settings::evaluationDate() {
  return evaluationDate_;
}

}  // namespace MiniQL

#endif