class UrchinNode final : public BossParts {
public:
  UrchinNode(const EntityArgs& args)
    : BossParts(args,327)
  {
    m_health = m_health_max;
    m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
    //circle
    this->set_circle_radius(m_radius);
  }
  virtual ~UrchinNode() = default;
  void draw(sf::RenderWindow& window) override final{
    if (m_appear_flag) return;

    if (this->is_blink()) {
      this->spr8x8(m_common_blink_spr);
    } else {
      this->spr8x8(m_spr_ene);
    }
    this->Enemy::draw(window);
    //this->draw_circle(window);
  }
private:
  void appear() {
    this->attr_px();
  }
  void upd_ene(float dt) override{
    this->upd_damage();
    this->upd_blink(dt);
  }
  void upd_damage() {
    //parentにblink伝搬
    if (!m_parent) return;
    if (m_boss_flag.m_blink && !this->is_blink()) {
      auto* parent_parts = static_cast<BossParts*>(m_parent);
      parent_parts->set_sub_blink(m_short_blinktm);
      m_boss_flag.m_blink = false;
    }
  }
  bool sub_health_by_player(int32_t dmg, FwFlag<HitMask> mask, float blink_tm) override {
    if (m_root) {
      bool set_blink = this->set_blink(m_short_blinktm);
      m_boss_flag.m_blink |= set_blink;
      m_root->sub_health_by_player(dmg, mask, -1.0f); //blink設定しない
      return set_blink; //blink設定した
    }
    return Entity::sub_health_by_player(dmg, mask, m_common_blinktm);
  }
  static constexpr float m_short_blinktm = 0.05f;
  const int32_t m_health_max = std::numeric_limits<int32_t>::max();
};

BossUrchin::BossUrchin(const EntityArgs& args)
  : Boss(args, 340)
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
    auto dir = (n.m_p - parent->get_estimate_pos()).normalize_lax();
    n.m_e->set_dir(dir);
    n.m_e->apply_angle(90.0);
  };

  Vec2f vP = this->get_estimate_pos();
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

      lambda_apply_entity(n, parent_entity); //pos -> entity->m_mov

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

  if (this->is_blink()) {
    this->spr8x8(332, 4, 4);
  } else {
    this->spr8x8(m_spr_ene, 4, 4);
  }
  this->Enemy::draw(window);
  //circle
  //this->draw_circle(window);
#if DEBUG&0
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

void BossUrchin::set_sub_dmg(bool is_del, int32_t dmg)
{
  if (is_del) {
    this->detach_all_or_lower();
  }
}

void BossUrchin::dead()
{
  auto num = 20;
  this->dead_dot_base(num, m_radius * 3.f);
  this->dead_efc_base();
  Boss::dead_base();
}

void BossUrchin::use_arms(int type, const LuaIntf::LuaRef& tbl)
{
  float t = tbl["t"].value<float>();
  if (m_arms_timer <= t) return;

  switch (type) {
  case 0: //bullet,arrow
    this->arms0();
    break;
  case 1: //bullet
    this->arms1();
    break;
  case 2: //spawn
    this->arms2_spawn();
    break;
  case 3: //spawn
    this->arms3_spawn();
    break;
  default:
    FW_ASSERT(0);
    break;
  }

  if (t>0.f) m_arms_timer = fmod(m_arms_timer, t);
}

void BossUrchin::arms0()
{
  //末端nodeから
  for (int32_t i = 0; i < m_legs_num; ++i) {
    auto& node = m_all_nodes[i].back();

    const auto* e = node.m_e;
    auto dir(e->get_dir());
    if (dir.sqr_magnitude() <= const_param::EPSILON) continue;
    new BossArrow(EntityArgs(EntityDataId::BossArrow, e->get_pos(), dir));
  }
}
void BossUrchin::arms1()
{
  float ofsdir = fw::PI * 2.0f / static_cast<float>(m_legs_num);
  for (int32_t i = 0; i < m_legs_num; ++i) {
    Vec2f d(1.0f, 0.0f);
    d.set_rotate(ofsdir * i + ofsdir * 0.5f + m_legs_rot);
    Vec2f p = this->get_estimate_pos() + d * (m_radius);
    //EntityArgs args(p);
    //scr::spawn(EnemyType::URCHIN, args);
    //new BossArrow(EntityArgs(EntityDataId::BossArrow, e->get_pos(), dir));
    new BossBullet(EntityArgs(p, d));
  }
}
void BossUrchin::arms2_spawn()
{
  this->get_estimate_pos();
  EntityArgs args(this->get_estimate_pos());
  scr::spawn(EnemyType::URCHIN, args);
}
void BossUrchin::arms3_spawn()
{
  scr::spawn_for_boss_urchin(0);
}