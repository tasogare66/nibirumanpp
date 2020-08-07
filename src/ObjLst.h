#pragma once
#include <functional>
#include "Vec2.h"
#include "Singleton.h"
class Entity;

class ObjLst : public Singleton<ObjLst> {
public:
  ObjLst() = default;
  ~ObjLst() = default;
  void request(Entity* o);
  void update(float dt);
  void draw(sf::RenderWindow& window);
private:
  void upd_add();
  void upd_del();
  void upd_move(float dt);
  void upd_colliders(std::vector<Entity*>&, std::function<bool(Entity*,Vec2f)> func=nullptr);
  void upd_verlet(float dt);
  friend class Entity;
  std::vector<Entity*> m_pxs;
  std::vector<Entity*> m_bullets;
  std::vector<Entity*> m_ene_bullets;
  std::vector<Entity*> m_ene_dot;
  std::vector<Entity*> m_verlets;
  std::vector<Entity*> m_objs;
  std::vector<Entity*> m_request;
  float m_prev_dt = 1.0f / 60.0f;
};