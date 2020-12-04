#include "stdafx.h"

#include "Entity.h"

#include "FABRIK.h"

namespace fabrik {
  Effector::Effector(Entity* entity)
    : m_entity(entity)
  {}

  Effector* Effector::constructor(Effector * parent)
  {
    m_position = m_entity->get_pos();

    if (parent)
    {
      m_distance = (parent->m_position - m_position).magnitude();
    }

    return this;
  }

  void Effector::apply_effector()
  {
    m_position = m_entity->get_pos();
  }

  void Effector::apply_entity()
  {
    auto mov = m_entity->calc_mov(m_position);
    m_entity->set_mov(mov);
    m_entity->set_dir(mov);
  }

  Chain::Chain(int32_t layer, Chain* parent, std::vector<Effector*>& effectors)
  {
    m_layer = layer;

    m_end_effector = static_cast<int32_t>(effectors.size()) - 1;

    //FIXME:  
    m_effectors.insert(m_effectors.end(), effectors.begin(), effectors.end());

    m_parent = parent;

    if (parent)
    {
      parent->m_children.push_back(this);
    }
  }
  void Chain::forward()
  {
    if (not m_target_enabled) return;

    auto root_position = m_effectors[0]->m_position;

    // Sub-base, average for centroid
    if (m_children.size() > 0)
    {
      m_target_position /= static_cast<float>(m_children.size());
    }

    m_effectors[m_end_effector]->m_position = m_target_position;

    for (uint32_t i = m_end_effector; i > 0; i--)
    {
      m_effectors[i]->m_direction = (m_effectors[i - 1]->m_position - m_effectors[i]->m_position).normalize();
      m_effectors[i - 1]->m_position = m_effectors[i]->m_position + m_effectors[i]->m_direction * m_effectors[i]->m_distance;
    }

    // Increment parent sub-base's target, to be averaged as above
    if (m_parent)
    {
      m_parent->m_target_position += m_effectors[0]->m_position;
    }

    m_effectors[0]->m_position = root_position;

  }
  void Chain::backward()
  {
    for (int32_t i = 0; i < m_end_effector; ++i)
    {
      m_effectors[i]->m_direction = (m_effectors[i + 1]->m_position - m_effectors[i]->m_position).normalize();
      m_effectors[i + 1]->m_position = m_effectors[i]->m_position + m_effectors[i]->m_direction * m_effectors[i + 1]->m_distance;
    }

    if (m_children.size() > 0)
    {
      m_target_position.set(0.f,0.f);
    }
  }

  void Chain::backward_multi()
  {
    this->backward();

    // Sub-bases share transforms with parent chains' end effectors, update only as the end effector
    for (int i = 1; i <= m_end_effector; ++i)
    {
      //m_effectors[i].transform.localPosition = effectors[i - 1].transform.InverseTransformPoint(effectors[i].position);
      //m_effectors[i].transform.localRotation = Quaternion.LookRotation(effectors[i - 1].transform.InverseTransformDirection(effectors[i].direction));
      m_effectors[i]->apply_entity();
    }

    for (auto child : m_children) {
      child->backward();
    }
  }

  void Chain::apply_effectors()
  {
    for (auto* eff: m_effectors) {
      eff->apply_effector();
    }
  }

  void IK::update()
  {
    this->apply_all_effectors();
    this->solve();
  }
  void IK::solve()
  {
    // We must iterate by layer in the first stage, working from target(s) to root
    for (auto& chain :m_chains) {
      chain->forward();
    }
    // Provided our hierarchy, the second stage doesn't directly require an iterator
    m_root_chain->backward_multi();
  }
  void IK::apply_all_effectors()
  {
    for (auto& chain : m_chains) {
      chain->apply_effectors();
    }
  }

  void IK::awake(Entity* root)
  {
    m_root_chain = this->create_system(root);

    // Inversely sort by layer, greater-first
    std::sort(m_chains.begin(), m_chains.end(),
      [](const auto& x, const auto& y) { return x->get_layer() > y->get_layer(); });
  }

  Chain* IK::create_system(Entity* transform, Chain* parent, int32_t layer)
  {
    std::vector<Effector*> effectors;

    Effector* effector = nullptr;

    // Use parent chain's end effector as our sub-base effector
    if (parent)
    {
      effector = parent->get_end_effector();

      effectors.push_back(effector);
    }

    // childCount > 1 is a new sub-base, childCount = 0 is an end chain (added to our list below), childCount = 1 is continuation of chain
    while (transform)
    {
      effector = transform->fetch_effector()->constructor(effector);

      effectors.push_back(effector);

      if (transform->get_child_count() != 1) //FIXME: 
      {
        break;
      }

      transform = transform->get_child_w(0);
    }

    m_chains.emplace_back(std::make_unique<Chain>(layer, parent, effectors));
    Chain* chain = m_chains.back().get();

    if (transform->get_child_count() <= 0)
    {
      m_end_chains.push_back(chain);
    }
    else for(auto& child : transform->get_children_w())
    {
      this->create_system(child, chain, layer + 1);
    }

    return chain;
  }
}