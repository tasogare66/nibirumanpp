class CogParts : public BossParts {
public:
  CogParts(const EntityArgs& args)
    : BossParts(args)
  {
    m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
    //circle
    this->set_circle_radius(m_radius);
  }
  virtual ~CogParts() = default;
protected:
  void appear() override {
    this->attr_px();
  }
  void draw(sf::RenderWindow& window) override final {
    if (m_appear_flag) return;

    this->spr8x8(m_spr_ene, 2, 2);
    this->Enemy::draw(window);
    //this->draw_circle(window);
  }
  void upd_ene(float dt) override {
    this->upd_blink(dt);
  }
};

//root
class CogRootParts : public CogParts {
public:
  CogRootParts(const EntityArgs& args, float in_central_rad)
    : CogParts(args)
    , m_parts_central_rad(in_central_rad)
  {
    m_flag.reset(EntityFlag::ForceAddVelEnabled);
    m_flag.set(EntityFlag::IgnoreWallCollision);

    //child
    CogParts* parent = this;
    constexpr size_t parts_num = 6;
    for (size_t i = 1; i <= parts_num; ++i) {
      auto p = args.m_pos + Vec2f(-(m_radius * 2.f - 2.f) * i, 0.f);
      const auto is_top_parts = (i==parts_num);
      auto* child = new CogParts({ is_top_parts ? EntityDataId::CogTopParts : EntityDataId::CogParts, p });

      Entity::set_hierarchy(parent, child);
      parent = child;
    }
    m_top_parts = parent;
    //ik
    m_ik.awake(this);
  }
  virtual ~CogRootParts() = default;
  void upd_ene(float dt) override final {
    CogParts::upd_ene(dt);
  }
  void upd_node(float parts_rot_deg) {
#if DEBUG&0
    const auto& input = Input::inst().input_data(0);
    m_ik.get_root_chain()->set_target_position(input.mouse());
#endif
    //ik
    if (m_effector) m_effector->apply_effector();
    m_ik.update();

    this->exec_or_lower([&parts_rot_deg](Entity* e) {
      float rotdir = 0.0f; //top_partsは回転しない
      float deg_ofs = 0.0f;
      if (e->has_children()) {
        if (e->get_hierarchy_level() % 2 == 0) {
          rotdir = 1.0f;
        } else {
          rotdir = -1.0f;
          deg_ofs = 30.0f; //歯車合わせでずらす
        }
      }
      e->apply_angle(-90.0f + deg_ofs + rotdir*parts_rot_deg);
    });
  }
  void set_ik_target_position(const Vec2f& p) {
    Vec2f lpos = this->get_estimate_pos();
    Vec2f vec = p - lpos;
    vec.set_rotate(fw::deg2rad(m_ik_target_ofs_deg.get()));
    m_ik.get_root_chain()->set_target_position(lpos+vec);
  }
  const CogParts* get_top_parts() const { return m_top_parts; }
  CogParts* get_top_parts_s() { return m_top_parts; }

  static Vec2f calc_central_rad_parts_posiiotn(float rad) {
    Vec2f vec{ const_param::LvRadius - 3 * 0.75f, 0.f };
    vec.set_rotate(rad);
    return vec;
  }
  void  upd_position(float dt, float rot_rad) {
    m_parts_central_rad.update(dt);
    m_ik_target_ofs_deg.update(dt);
    float rad = m_parts_central_rad.get() + rot_rad;
    auto p = this->calc_central_rad_parts_posiiotn(rad);
    this->set_calc_mov(p);
  }
  void request_parts_central_rad(float v, float dur) {
    m_parts_central_rad.request(v, dur);
  }
  void request_target_ofs_deg(float deg) {
    m_ik_target_ofs_deg.request(deg, 3.0f);
  }
protected:
  void appear() override {
    this->attr_ene_bullet(); //FIXME:どうする 
  }
  fabrik::IK m_ik;
  CogParts* m_top_parts; //先っぽ
  EaseParam m_parts_central_rad;
  EaseParam m_ik_target_ofs_deg{0.0f};
};

BossCog::BossCog(const EntityArgs& args)
  : Boss(args)
{
  this->set_health_max();
  //circle
  this->set_circle_radius(m_radius);

  //child
  {
    for (size_t i = 0; i < m_parts_num; ++i) {
      const auto rad = this->calc_parts_central_rad(i);
      const auto p = CogRootParts::calc_central_rad_parts_posiiotn(rad+ m_parts_pos_rot);
      auto* child = new CogRootParts({EntityDataId::CogParts, p}, rad);
 
      m_root_parts[i] = child;
      Entity::set_hierarchy(this, child);
    }
  }
  //script
  m_script.reset(scr::create_lua_boss_sequence(std::string_view("update_cog"), this));
  m_script->reset_thread();
}

void BossCog::appear()
{
  this->attr_verlet();
}

void BossCog::update(float dt)
{
  this->update_dt(dt);
  Boss::update(dt);
}

void BossCog::upd_ene(float dt)
{
  m_arms_timer += dt;
  this->upd_ene_base(dt);

  m_rot_speed.update(dt);
  //upd parts
  m_parts_pos_rot += fw::deg2rad(this->get_rot_speed())*dt;
  m_parts_pos_rot = fw::wrap_radian(m_parts_pos_rot);
  for (size_t i = 0; i < m_parts_num; ++i) {
    m_root_parts[i]->upd_position(dt, m_parts_pos_rot);
  }

  //ik target
  for (auto* rp : m_root_parts) {
    rp->set_ik_target_position(this->get_pos());
  }

  this->upd_nodes(dt);
}

void BossCog::upd_nodes(float dt)
{
  m_parts_rot += fw::deg2rad(60.f) * dt;
  m_parts_rot = fw::wrap_radian(m_parts_rot);
  auto parts_rot_deg = fw::rad2deg(m_parts_rot);
  for (auto* parts : m_root_parts) {
    parts->upd_node(parts_rot_deg);
  }
}

void BossCog::draw(sf::RenderWindow& window)
{
  this->draw_circle(window);
}

float BossCog::calc_parts_central_rad(size_t n) const {
  constexpr float prad = 2.0f * fw::PI / m_parts_num;
  return prad * n;
}

void BossCog::use_arms(int type, const LuaIntf::LuaRef& tbl)
{
  float t = tbl["t"].value<float>();
  if (m_arms_timer <= t) return;

  switch (type) {
  case 0:
    this->arms0();
    break;
  case 1:
    this->arms1();
    break;
  default:
    FW_ASSERT(0);
    break;
  }

  if (t > 0.f) m_arms_timer = fmod(m_arms_timer, t);
}

void BossCog::arms0()
{
  for (auto* parts : m_root_parts) {
    auto* e = parts->get_top_parts(); //先端
    auto dir(-e->get_dir());
    if (dir.sqr_magnitude() <= const_param::EPSILON) continue;
    Vec2f p = e->get_estimate_pos() + dir * e->get_radius();
    new BossBullet(EntityArgs(EntityDataId::BossBullet, p, dir, 20.0f));
  }
}

void BossCog::arms1()
{
  for (auto* parts : m_root_parts) {
    auto* e = parts->get_top_parts(); //先端
    auto dir(-e->get_dir());
    if (dir.sqr_magnitude() <= const_param::EPSILON) continue;
    Vec2f p = e->get_estimate_pos() + dir * e->get_radius();
    new BossArrow(EntityArgs(EntityDataId::BossArrow, p, dir, 20.0f));
  }
}

void BossCog::set_formation(int type, const LuaIntf::LuaRef& tbl)
{
  float dur = tbl["dur"].value<float>();

  switch (type) {
  case 0:
    this->formation0(dur);
    break;
  case 1:
    this->formation1(dur);
    break;
  case 2:
    this->formation2(dur);
    break;
  default:
    FW_ASSERT(0);
    break;
  }
}
void BossCog::formation0(float dur)
{
  for (size_t i = 0; i < m_root_parts.size(); ++i) {
    const auto rad = this->calc_parts_central_rad(i);
    auto& parts{ m_root_parts[i] };
    parts->request_parts_central_rad(rad, dur);
    parts->request_target_ofs_deg(0.0f);
  }
}
//one place
void BossCog::formation1(float dur)
{
  for (size_t i = 0; i < m_root_parts.size(); ++i) {
    const auto rad = fw::deg2rad(180.0f + 6.0f * i);
    m_root_parts[i]->request_parts_central_rad(rad, dur);
    float mul = 0.5f + i - m_root_parts.size()/2.0f;
    m_root_parts[i]->request_target_ofs_deg(-5.0f*mul);
  }
}
//two places
void BossCog::formation2(float dur)
{
  for (size_t i = 0; i < m_root_parts.size()/2; ++i) {
    const auto rad = fw::deg2rad(90.0f + 6.0f * i);
    m_root_parts[i]->request_parts_central_rad(rad, dur);
    float mul = 0.5f + i - m_root_parts.size() / 4.0f;
    m_root_parts[i]->request_target_ofs_deg(-5.0f * mul);
  }
  for (size_t i = m_root_parts.size()/2; i < m_root_parts.size(); ++i) {
    const auto rad = fw::deg2rad(270.0f + 6.0f * i);
    m_root_parts[i]->request_parts_central_rad(rad, dur);
    float mul = 0.5f + i - m_root_parts.size() / 2 - m_root_parts.size() / 4.0f;
    m_root_parts[i]->request_target_ofs_deg(-5.0f * mul);
  }
}