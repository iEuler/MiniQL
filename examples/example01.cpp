#include <iostream>
#include <memory>

template <typename T>
class Singleton {
  public:
    static T& instance()
    {
      static std::shared_ptr<T> instance_(new T);
      return *instance_;
    }

  protected:
    Singleton();  
};

class Settings : public Singleton<Settings> {
    friend class Singleton<Settings>;
  public:    
    int& flag() { return flag_; }
  private:
    Settings();    
    int flag_ = 0;
};

class AnotherSetting {
  public:
    AnotherSetting(const AnotherSetting&) = delete;
    AnotherSetting& operator=(const AnotherSetting&) = delete;
    AnotherSetting(AnotherSetting&&) = delete;
    AnotherSetting& operator=(AnotherSetting&&) = delete;
    
    static AnotherSetting& instance() {
      static AnotherSetting ins;
      return ins;
    }

    int& flag() { return flag_; }
  
  private:
    AnotherSetting() = default;
    int flag_ = 0;
  
};

class Test{
  private:
    Test() = default; // Disallow instantiation outside of the class.
  public:
    Test(const Test&) = delete;
    Test& operator=(const Test &) = delete;
    Test(Test &&) = delete;
    Test & operator=(Test &&) = delete;        

    static auto& instance(){
        static Test test;
        return test;
    }
}; 

int main() {
  // auto x = Singleton<int>::instance();
  // auto y = Singleton<Settings>::instance();  // not compile
  // auto z = Settings::instance();             // not compile
  // auto &a = AnotherSetting::instance();
  AnotherSetting::instance().flag() = 7;
  std::cout << AnotherSetting::instance().flag() << std::endl;
  Test::instance();
  // Test x;
}