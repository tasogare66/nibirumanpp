﻿#pragma once
#include "Vec2.h"

namespace fabrik{
  class Effector {
  public:
    Effector(Entity*);
    virtual ~Effector() = default;
    Effector* constructor(Effector* parent = nullptr);
    void apply_effector(); //entity->effector
    void apply_entity(const Effector* parent); //effector->entity
  protected:
    friend class Chain;
    Vec2f m_position;
    Vec2f m_direction;
    float m_distance = 0.0f;
    Entity* m_entity;
  };

  class Chain {
  public:
    Chain(int32_t layer, Chain* parent, std::vector<Effector*>& effectors);
    virtual ~Chain() = default;
    void forward();
    void backward();
    void backward_multi();
    void apply_effectors();

    Effector* get_base_effector() {
      return m_effectors[0];
    }
    Effector* get_end_effector() {
      return m_effectors[m_end_effector];
    }
    int32_t get_layer() const { return m_layer; }
    bool target_enabled() const { return m_target_enabled; }
    void set_target_enabled(bool in_flag) { m_target_enabled = in_flag; }
    const Vec2f& get_target_position() const { return m_target_position; }
    void set_target_position(const Vec2f& v) { m_target_position = v; }
  protected:
    Chain* m_parent=nullptr;
    std::vector<Chain*> m_children;
    std::vector<Effector*> m_effectors;
    int32_t m_layer = 0;
    int32_t m_end_effector = 0;
    Vec2f m_target_position;
    bool m_target_enabled = true;;
  };
 
  class IK {
  public:
    IK() = default;
    virtual ~IK() = default;
    void update();
    void awake(Entity* entity);
    Chain* create_system(Entity* transform, Chain* parent = nullptr, int32_t layer = 0);
    Chain* get_root_chain() { return m_root_chain; }
    void apply_all_effectors();
  protected:
    void solve();
    Chain* m_root_chain=nullptr;
    std::vector<std::unique_ptr<Chain>> m_chains;
    std::vector<Chain*> m_end_chains;
  };
}