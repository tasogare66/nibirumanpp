#pragma once
#include "Singleton.h"

class Mode;
class ModeMng : public Singleton<ModeMng> {
public:
  ModeMng();
  virtual ~ModeMng();
  bool update(float dt);
  void update_post();
  void draw0(sf::RenderWindow& window);
  void draw1(sf::RenderWindow& window);
private:
  void init();
  void dest();
  bool ctrl(float dt);
  Mode* m_cur_work = nullptr;
  Mode* m_req_work = nullptr;
};