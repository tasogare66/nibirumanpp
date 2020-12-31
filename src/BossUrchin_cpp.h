﻿class UrchinNode final : public BossParts {
public:
  UrchinNode(const EntityArgs& args)
    : BossParts(args,460)
  {
    m_health = m_health_max;
    m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
    //circle
    this->set_circle_radius(m_radius);
  }
  virtual ~UrchinNode() = default;
  void draw(sf::RenderWindow& window) override final{
    if (m_appear_flag) return;
    this->draw_circle(window);
  }
private:
  void appear() {
    this->attr_px();
  }
  void upd_ene(float dt) override{
    this->upd_blink(dt);
  }
  const int32_t m_health_max = std::numeric_limits<int32_t>::max();
};

BossUrchin::BossUrchin(const EntityArgs& args)
  : Boss(args, 456)
{
  m_health_max = m_health;
  m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
  //circle
  this->set_circle_radius(m_radius);

  //child
  for(int i=0;i<m_legs_num;++i){
    Entity* parent = this;
    for(int j=0;j<m_node_num;++j){
      auto p = args.m_pos;
      EntityArgs args(p);
      args.m_radius = m_node_radisu;
      auto child = new UrchinNode(args);
      m_all_nodes[i][j].m_e = child;
      Entity::set_hierarchy(parent, child);
      parent = child;
    }
  }

  //script
  m_script.reset(scr::create_lua_boss_sequence(std::string_view("update_urchin"), this));
  m_script->reset_thread();
}

void BossUrchin::appear()
{
  this->attr_px();
}

void BossUrchin::update(float dt)
{
  this->update_dt(dt);
  Boss::update(dt);
  this->upd_blink(dt);
}

void BossUrchin::upd_ene(float dt)
{
  m_arms_timer += dt;
  this->upd_ene_base(dt);

  m_stiffness.update(dt);
  m_rot_speed.update(dt);

  //upd legs
  m_legs_rot += fw::deg2rad(this->get_rot_speed())*dt;
  m_legs_rot = atan2(sin(m_legs_rot), cos(m_legs_rot));

  this->upd_nodes();
}

static inline float InvLength(const Vec2f& lhs, float fail_value) {
  float d = lhs.sqr_magnitude();
  if (d > 0.0f) return 1.0f / std::sqrtf(d);
  return fail_value;
}

void BossUrchin::upd_nodes(bool is_reset)
{
  constexpr float lb = m_node_radisu*2.0f;
  constexpr float pi = static_cast<float>(M_PI);
  constexpr float pi2 = 2.f * pi;

  auto lambda_NA = [this,pi,pi2](float f)->float {
    return f - pi2 * std::floor((f + pi) / pi2);
  };
  auto lambda_apply_entity = [&is_reset](Node& n, const Entity* parent) {
    if (is_reset) {
      n.m_e->set_position(n.m_p);
    } else {
      n.m_e->set_calc_mov(n.m_p);
    }
    auto dir = (parent->get_pos() - n.m_p).normalize_lax();
    n.m_e->set_dir(dir);
  };

  Vec2f vP = this->get_estimate_pos();
  const Entity* eP = this;
  for (int32_t i = 0; i < m_legs_num; ++i) {
    float aT = i * pi2 / m_legs_num + m_legs_rot;
    float apv = aT;
    float l = lb;
    Vec2f vDT(std::cos(aT), std::sin(aT));

    Vec2f vpv = vP+vDT*(m_radius+m_node_radisu*0.5f);
    m_all_nodes[i][0].m_p = vpv; //entity->m_pos -> pos
    lambda_apply_entity(m_all_nodes[i][0], this); //pos -> entity->m_mov

    const Entity* parent_entity = m_all_nodes[i][0].m_e;
    const float stiffness = get_stiffness();
    for (int32_t j = 1; j < m_node_num; ++j)
    {
      auto& n{m_all_nodes[i][j]};
      n.m_p = n.m_e->get_pos(); //entity->m_pos -> pos
      auto& vC = n.m_p;
      Vec2f vab = vC - vpv;
      Vec2f vAB = vab * InvLength(vab, 1.f);
      float aS = atan2(vAB.y, vAB.x);
      float cl = std::clamp(lambda_NA(aS - apv), -pi, pi);
      aS = apv + std::clamp(cl - cl * stiffness, -pi, pi);
      Vec2f vD(std::cos(aS), std::sin(aS));
      vC = vpv + vD * l;

      lambda_apply_entity(n, eP); //pos -> entity->m_mov

      apv = aS;
      vpv = vC;
      parent_entity = n.m_e;
      //l *= i & 1 ? .9f : .85f;
    }
  }
}

void BossUrchin::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

  //circle
  this->draw_circle(window);
#if DEBUG
  std::vector<sf::Vertex> vertices;
  for (int32_t i = 0; i < m_legs_num; ++i) {
    Vec2f vpv = m_all_nodes[i][0].m_p;

    for (int32_t j = 1; j < m_node_num; ++j)
    {
      const auto& n{ m_all_nodes[i][j] };
      auto& vC = n.m_p;

      vertices.emplace_back(Vec2f(vpv.x, vpv.y));
      vertices.emplace_back(Vec2f(vC.x, vC.y));
      vpv = vC;
    }
  }
  if (vertices.size() > 0) {
    window.draw(&vertices[0], vertices.size(), sf::Lines);
  }
#endif
}

void BossUrchin::use_arms(int type, const LuaIntf::LuaRef& tbl)
{
  switch (type) {
  case 0: //bullet
  {
    float t = tbl["t"].value<float>();
    this->arms0(t);
  }
  break;
  default:
    FW_ASSERT(0);
    break;
  }
}

void BossUrchin::arms0(float t)
{
  if (m_arms_timer > t) {
    //末端nodeから
    for (int32_t i = 0; i < m_legs_num; ++i) {
      auto& node = m_all_nodes[i].back();

      const auto* e = node.m_e;
      auto dir(e->get_dir());
      if (dir.sqr_magnitude() <= const_param::EPSILON) continue;
      new BossArrow(EntityArgs(EntityDataId::BossArrow, e->get_pos(), -dir));
    }
    m_arms_timer = fmod(m_arms_timer, t);
  }
}
