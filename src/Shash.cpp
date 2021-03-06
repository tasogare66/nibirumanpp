﻿#include "stdafx.h"

#include "Shash.h"

Shash::Shash(uint32_t cellsize)
  : m_cellsize(cellsize)
{
}

Vec2<int32_t> Shash::cell_position(float x, float y)
{
  auto ix = static_cast<int32_t>(std::floor(x / m_cellsize));
  auto iy = static_cast<int32_t>(std::floor(y / m_cellsize));
  return Vec2<int32_t>(ix, iy);
}

void Shash::each_overlapping_cell(ShaEntity& e, CallbackFunc fn) {
  auto sxy = this->cell_position(e.m_left, e.m_top);
  auto exy = this->cell_position(e.m_right, e.m_bottom);
  for (auto y = sxy.y; y <= exy.y; ++y) {
    for (auto x = sxy.x; x <= exy.x; ++x) {
      int64_t idx = Shash::coord_to_key(x, y);
      fn(idx);
    }
  }
}

void Shash::add_entity_to_cell(int64_t idx, ShaEntity* e) {
  m_cells[idx].insert({ e,0 });
}

void Shash::remove_entity_from_cell(int64_t idx, ShaEntity* e)
{
  auto it = m_cells.find(idx);
  if (it == m_cells.end()) {
    FW_ASSERT(0);
    return;
  }
  it->second.erase(e);
}

void Shash::add(Entity* obj, float x, float y, float w, float h, const Entity* root)
{
  // Create entity.The table is used as an array as this offers a noticable
  // performance increase on LuaJIT; the indices are as follows :
  // [1] = left, [2] = top, [3] = right, [4] = bottom, [5] = object
  ShaEntity e = { x, y, x + w, y + h, obj, root };
  // Add to main entities table
  auto p = m_entities.emplace(obj, e);
  //self.numentities = self.numentities + 1
  // Add to cells
  this->each_overlapping_cell(e, [this,&p](int64_t idx) { this->add_entity_to_cell(idx, &(p.first->second)); });
}

void Shash::remove(Entity* obj)
{
  // Get entity of obj
  auto it = m_entities.find(obj);
  if (it == m_entities.end()) {
    FW_ASSERT(0); //unregistered
    return;
  }
  //Remove from cells
  auto& e = it->second;
  this->each_overlapping_cell(e, [this, &e](int64_t idx) { this->remove_entity_from_cell(idx, &e); });
  // Remove from main entities table
  m_entities.erase(it);
}

void Shash::update(Entity* obj, float x, float y, float w, float h)
{
  // Get entity from obj
  auto it = m_entities.find(obj);
  if (it == m_entities.end()) {
    FW_ASSERT(0);
    return;
  }
  auto& e = it->second;
  // No width / height specified ? Get width / height from existing bounding box
  if (w <= 0.0f) w = e.m_right - e.m_left;
  if (h <= 0.0f) h = e.m_bottom - e.m_top;
  // Check the entity has actually changed cell - position, if it hasn't we don't
  // need to touch the cells at all
  auto a1 = this->cell_position(e.m_left, e.m_top);
  auto a2 = this->cell_position(e.m_right, e.m_bottom);
  auto b1 = this->cell_position(x, y);
  auto b2 = this->cell_position(x + w, y + h);
  bool dirty = (a1.x != b1.x || a1.y != b1.y || a2.x != b2.x || a2.y != b2.y);
  // Remove from old cells
  if (dirty) {
    this->each_overlapping_cell(e, [this, &e](int64_t idx) { this->remove_entity_from_cell(idx, &e); });
  }
  // Update entity
  e.m_left = x;
  e.m_top = y;
  e.m_right = x + w;
  e.m_bottom = y + h;
  // Add to new cells
  if (dirty) {
    this->each_overlapping_cell(e, [this, &e](int64_t idx) { this->add_entity_to_cell(idx, &e); });
  }
}

void Shash::clear() {
  // Clear all cellsand entities
  m_cells.clear();
  m_entities.clear();
}

static inline bool overlaps(const ShaEntity& e1, const ShaEntity& e2) {
  //return e1[3] > e2[1] and e1[1] < e2[3] and e1[4] > e2[2] and e1[2] < e2[4]
  return e1.m_right > e2.m_left && e1.m_left < e2.m_right&& e1.m_bottom > e2.m_top && e1.m_top < e2.m_bottom;
}

static inline bool is_other_root(const ShaEntity& e1, const ShaEntity& e2) {
  return (!e1.m_root || (e1.m_root != e2.m_root));
}

void Shash::each_overlapping_in_cell(int64_t idx, ShaEntity& e, std::set<const ShaEntity*>& sets, HitCallbackFunc& hcb)
{
  const auto it = m_cells.find(idx);
  if (it == m_cells.end()) return;
  for (auto& p : it->second) {
    auto v = p.first;
    if (&e != v && is_other_root(e, *v) && overlaps(e, *v) && sets.count(v) <= 0) {
      hcb(v->m_e);
      sets.insert(v);
    }
  }
}

void Shash::each_overlapping_entity(ShaEntity& e, HitCallbackFunc& hcb)
{
  //Init set for keeping track of which entities have already been handled
  auto& sets = m_tablepool;
  sets.clear();
  //Do overlap checks
  this->each_overlapping_cell(e, [&](int64_t idx) { this->each_overlapping_in_cell(idx, e, sets, hcb); });
  //Clear setand return to pool
  //for v in pairs(set) do
  //  set[v] = nil
  //end
  //insert(self.tablepool, set)
}

void Shash::each(float x, float y, float w, float h, HitCallbackFunc hcb)
{
  // Got bounding box, make temporary entity
  auto tmp_e = ShaEntity(x, y, x + w, y + h);
  this->each_overlapping_entity(tmp_e, hcb);
}

void Shash::each(Entity* obj, HitCallbackFunc hcb)
{
  // Got object, use its entity
  auto it = m_entities.find(obj);
  if (it == m_entities.end()) {
    FW_ASSERT(0);
    return;
  }
  this->each_overlapping_entity(it->second, hcb);
}