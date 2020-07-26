#pragma once
#include "Singleton.h"
class Entity;

class ObjLst : public Singleton<ObjLst> {
public:
  ObjLst() = default;
  ~ObjLst() = default;
  void add(Entity* o);
  void update(float dt);
  void draw(sf::RenderWindow& window);
private:
  void upd_move(float dt);
  void upd_verlet(float dt);
  std::vector<Entity*> m_pxs;
  std::vector<Entity*> m_bullets;
  std::vector<Entity*> m_ene_bullets;
  std::vector<Entity*> m_ene_dot;
  std::vector<Entity*> m_verlets;
  std::vector<Entity*> m_objs;
  float m_prev_dt = 1.0f / 60.0f;
};