#pragma once
#include <functional>
#include "Vec2.h"
#include "Singleton.h"
class Entity;
class Shash;

class ObjLst : public Singleton<ObjLst> {
public:
  ObjLst();
  ~ObjLst();
  uint32_t request(Entity* o);
  void update(float dt);
  void draw(sf::RenderWindow& window);
  uint32_t get_spawn_num() const { return m_spawn_num; }
  uint32_t get_spawn_ttl() const { return m_spawn_ttl; }
private:
  void upd_add();
  void upd_del();
  void upd_prepare();
  void upd_move(float dt);
  void upd_reciprocal();
  static bool intersect_circle_vs_circle(const Entity* p1, const Entity* p2);
  static void reciprocal_each(Entity* p1, Entity* p2);
  static void blt_vs_ene(Entity* o, Entity* b);
  static void force_vs_ene(Entity* o, Entity* f);
  template<typename T>
  void upd_colliders(std::vector<T*>&, std::function<bool(Entity*,Vec2f)> func=nullptr);
  void upd_verlet(float dt);
  friend class Entity;
  std::vector<Entity*> m_pxs;
  std::vector<Entity*> m_bullets;
  std::vector<Entity*> m_ene_bullets;
  std::vector<Entity*> m_ene_dot;
  std::vector<Entity*> m_forces;
  std::vector<Entity*> m_verlets;
  std::vector<Entity*> m_objs;
  std::vector<Entity*> m_request;
  std::unique_ptr<Shash> m_px_sha;
  std::unique_ptr<Shash> m_enblt_sha;
  std::unique_ptr<Shash> m_endot_sha;
  float m_prev_dt = 1.0f / 60.0f;
  uint32_t m_cumulative_no = 0;
  uint32_t m_spawn_ttl = 0;
  uint32_t m_spawn_num = 0;
};