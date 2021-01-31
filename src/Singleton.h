#pragma once
#include <memory>

class SingletonsAdapter {
public:
  using ResetFunc = void(*)();
  static void add_reset_func(ResetFunc func);
  static void invoke_resets();
};

template <class T>
class Singleton{
public:
  static T& inst()
  {
    return getReference(uptr());
  }

  static void reset_inst()
  {
    uptr().reset(T::createInstance());
  }

private:
  using singleton_pointer_type = std::unique_ptr<T>;

  inline static singleton_pointer_type& uptr() {
    static typename T::singleton_pointer_type s_singleton(T::createInstance(true));
    return s_singleton;
  }
  inline static T* createInstance(bool first_time=false) {
    if (first_time) { //初回だけ登録
      SingletonsAdapter::add_reset_func(&Singleton<T>::reset_inst);
    }
    return new T();
  }
  inline static T& getReference(const singleton_pointer_type& ptr) { return *ptr; }

protected:
  Singleton() {}

private:
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;
};
