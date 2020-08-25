#include "stdafx.h"

#include <iostream>
#include <iomanip>
#include <string>
//#include <strstream>

extern uint64_t GetNumOfHeapAllocations();
#if DEBUG&0
class CExternalBuffer : public std::streambuf
{
public:
  CExternalBuffer() = delete;
  CExternalBuffer(char* buffer, size_t bufferLength) {
    char* bufferEnd = buffer + bufferLength;
    setp(buffer, bufferEnd);
    setg(buffer, buffer, bufferEnd);
  }
};
class StreamTest {
public:
  StreamTest() {
    printf("%lld\n", GetNumOfHeapAllocations());
    char buffer[1000];
    CExternalBuffer streamBuffer(buffer, 1000);
    std::ostream stream(&streamBuffer);
    stream << "TEST_dATA" << std::ends;
    printf("%s\n",buffer);
    printf("%lld\n", GetNumOfHeapAllocations());
  }
private:
};
static StreamTest m_stream_test_inst;
#endif

#if DEBUG&0
//test
using namespace std;
class MovTest {
public:
  MovTest(int i)
    : m_val(i)
  {
    cout << "construct:" << m_val << endl;
  }
  ~MovTest() {
    uint64_t p = (uint64_t)(this);
    cout << "destruct:" << m_val << ":" << p << endl;
  }
  MovTest(const MovTest& obj) { m_val = obj.m_val; cout << "copy const:" << m_val << endl; }
  MovTest(MovTest&& obj) { m_val = obj.m_val;  cout << "move const:" << m_val << endl; }
  //MovTest(MovTest&& obj) = default;
private:
  int m_val;
};
class Annotation {
public:
  explicit Annotation(const MovTest text)
    : val(std::move(text)) //text constだとcopyこんになる
  {}
  MovTest val;
};

void process(const string& lvalarg) {
  cout << "call lval:" << lvalarg << endl;
}
void process(string&& rvalarg) {
  cout << "call rval:" << rvalarg << endl;
}

template<typename T>
void logAndProcess(T&& param) {
  cout << "call log process" << endl;
  process(std::forward<T>(param)); //paramが右辺値の場合右辺値にキャスト
  //process(std::move<T>(param));
  //process(param);
}

class Widget {
public:
  Widget(MovTest s)
    : m_s(s)
  {}
  Widget(Widget&& rhs)
    : m_s(std::move(rhs.m_s))
  {
    ++movCtroCalls;
  }
  static size_t movCtroCalls;
  MovTest m_s;
};

void f(int&& param) {
}

class IntVector
{
  int* mData;
  std::size_t mSize;
  std::string mName;
public:
  IntVector(std::size_t iSize, int iFuctor, char const* iName) :
    mData(new int[iSize]), mSize(iSize), mName(iName)
  {
    for (std::size_t i = 0; i < mSize; ++i) mData[i] = static_cast<int>(i * iFuctor);
    print("constuctor");
  }
#if 1
  IntVector(IntVector&& iRhs) :
    mData(iRhs.mData), mSize(iRhs.mSize), mName(std::move(iRhs.mName))
  {
    iRhs.mData = nullptr;
    iRhs.mSize = 0;
    print("move constuctor");
  }
#else
  IntVector(IntVector const& iRhs) = delete;
  IntVector(IntVector&& iRhs) = delete;
#endif
  ~IntVector() { delete[] mData; }

  int& operator[](std::size_t i) { return mData[i]; }
  std::size_t size() const { return mSize; }
  void print(std::string const& iTitle) const
  {
    std::cout << iTitle << " " << mName << "\n"
      << "    mData(" << &mData << ") = " << mData << "\n"
      << "    mSize(" << &mSize << ") = " << mSize;
    for (std::size_t i = 0; i < mSize; ++i)
    {
      if ((i % 8) == 0)   std::cout << "\n";
      std::cout << std::setw(5) << mData[i];
    }
    std::cout << "\n";
  }
};

IntVector nrvo()
{
  IntVector   vec(4, 10, "vec");
  return vec;
}

class TestExec {
public:
  TestExec() {
    set_func("aiueo");

    std::vector<int> tmp;
    tmp.reserve(100);

    //int v = 5;
    //f(v);

    {
      Annotation aaa(MovTest(1));
    }
    {
      string w = "hoge";
      logAndProcess(w);
      logAndProcess(std::move(w));
    }
    {
      MovTest mt(3);
      Widget w(std::move(mt));
    }
    {
      Widget w(4);
      MovTest d = makeMovTest();

      IntVector   vec1(nrvo());
    }
    cout << "aaa" << endl;
  }
  void set_func(const string& in_str) {
    m_str = in_str;
    set2_func(in_str);
  }
  void set2_func(const string& in_str) {
    m_str = in_str;
  }
  MovTest makeMovTest() {
    MovTest ret(5);
    return ret;
  }

  string m_str;
};
static TestExec m_inst;
#endif