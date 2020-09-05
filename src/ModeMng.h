#pragma once
#include "Singleton.h"
#include "ModeType.h"
class Mode;
class ModeMng : public Singleton<ModeMng> {
public:
  ModeMng();
  virtual ~ModeMng();

  void request(ModeType req);

  bool update(float dt);
  void update_post();
  void draw0(sf::RenderWindow& window);
  void draw1(sf::RenderWindow& window);

  ModeType get_current_mode_type() const;
  template<typename T>
  T* get_current_mode();
private:
  void init();
  void dest();
  bool ctrl(float dt);
  std::unique_ptr<Mode> m_cur_work = nullptr;
  std::unique_ptr<Mode> m_req_work = nullptr;
};