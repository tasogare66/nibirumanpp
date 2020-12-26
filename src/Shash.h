#pragma once
#include <unordered_map>
#include <functional>
#include <set>
#include "Vec2.h"
class Entity;

struct ShaEntity {
  ShaEntity(float l,float t, float r, float b, Entity* e=nullptr, const Entity* root=nullptr)
    : m_left(l), m_top(t)
    , m_right(r), m_bottom(b)
    , m_e(e)
    , m_root(root)
  {}
  float m_left, m_top;
  float m_right, m_bottom;
  Entity* m_e;
  const Entity* const m_root;
};

class Shash {
public:
  using HitCallbackFunc = std::function<void(Entity* o)>;

  Shash(uint32_t cellsize);
  ~Shash() = default;
  void add(Entity* obj, float x, float y, float w, float h, const Entity* root);
  void remove(Entity* obj);
  void update(Entity* obj, float x, float y, float w=0.0f, float h=0.0f);
  void clear();
  void each(float x, float y, float w, float h, HitCallbackFunc hcb);
  void each(Entity* obj, HitCallbackFunc hcb);

private:
  using CallbackFunc = std::function<void(int64_t)>;

  static int64_t coord_to_key(int64_t x, int64_t y) {
    return x + y * static_cast<int64_t>(1e+7);
  }
  Vec2<int32_t> cell_position(float x, float y);
  void each_overlapping_cell(ShaEntity& e, CallbackFunc fn);
  void add_entity_to_cell(int64_t idx, ShaEntity* e);
  void remove_entity_from_cell(int64_t idx, ShaEntity* e);
  void each_overlapping_in_cell(int64_t idx, ShaEntity& e, std::set<const ShaEntity*>& sets, HitCallbackFunc& hcb);
  void each_overlapping_entity(ShaEntity& e, HitCallbackFunc& hcb);

  uint32_t m_cellsize;
  std::unordered_map<Entity*, ShaEntity> m_entities;
  std::unordered_map<int64_t, std::unordered_map<ShaEntity*,int>> m_cells;
  std::set<const ShaEntity*> m_tablepool;
};