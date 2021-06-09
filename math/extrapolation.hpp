#ifndef MINIQL_EXTRAPOLATION_HPP
#define MINIQL_EXTRAPOLATION_HPP

namespace MiniQL {
  class Extrapolator {
    public:
      Extrapolator() = default;
      virtual ~Extrapolator() = default;
    
    private:
      bool extrapolate_ = false;

  };
  
}  // namespace MiniQL

#endif