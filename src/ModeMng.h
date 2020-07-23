#pragma once
class Mode;
class ModeMng {
public:
  ModeMng();
  virtual ~ModeMng();
  void update();
  void update_post();
  void draw0();
  void draw1();
private:
  Mode* m_work=nullptr;
};