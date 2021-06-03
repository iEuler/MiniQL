// g++ example07.cpp -o out
// test date, calendar, dayCount

#include <exception>
#include <iostream>

// #include "../time/date.hpp"
#include "../error.hpp"
#include "../interestrate.hpp"
#include "../quote.hpp"
#include "../termstructure.hpp"
#include "../termstructures/yieldtermstructure.hpp"

#define QL_ERROE_FUNCTIONS
#define QL_ERROE_LINES

int main() {
  if (0) 
  {
    try 
    {
      QL_FAIL("Test ql fail.");
    }
    catch (std::exception &e)
    {
      std::cout << e.what() << std::endl;
    }
    QL_FAIL("Test ql fail 2nd.")
    QL_ENSURE(5<3, "Test ensure fail.");
  }


}