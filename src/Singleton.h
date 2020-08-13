#pragma once
#include <memory>

template <class T>
class Singleton{
public:
  static T& inst()
  {
    return getReference(uptr());
  }

  static void reset()
  {
    uptr().reset(T::createInstance());
  }

private:
  using singleton_pointer_type = std::unique_ptr<T>;

  inline static singleton_pointer_type& uptr() {
    static typename T::singleton_pointer_type s_singleton(T::createInstance());
    return s_singleton;
  }
  inline static T* createInstance() { return new T(); }
  inline static T& getReference(const singleton_pointer_type& ptr) { return *ptr; }

protected:
  Singleton() {}

private:
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;
};
