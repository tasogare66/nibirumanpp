class CobraParts : public BossParts {
public:
  CobraParts(const EntityArgs& args)
    : BossParts(args)
  {
    m_flag.set(EntityFlag::IgnoreCollisionSameRoot);
    //circle
    this->set_circle_radius(m_radius);
  }
  virtual ~CobraParts() = default;
protected:
  void draw_common(sf::RenderWindow& window) {
    //this->Enemy::draw(window);
    this->draw_circle(window);
  }
  void upd_ene_common(float dt) {
    this->upd_blink(dt);
  }
};

class CobraHeadParts final : public CobraParts {
public:
  CobraHeadParts(const EntityArgs& args)
    : CobraParts(args)
  {
  }
  virtual ~CobraHeadParts() = default;
private:
  void appear() override {
    this->attr_px();
  }
  void upd_ene(float dt) override {
    this->upd_ene_common(dt);
  }
  void draw(sf::RenderWindow& window) override final {
    if (m_appear_flag) return;

    this->draw_common(window);
  }
};

//ene_bullet属性,forceでvel入れない
class CobraBodyParts final : public CobraParts {
public:
  CobraBodyParts(const EntityArgs& args)
    : CobraParts(args)
  {
    m_flag.reset(EntityFlag::ForceAddVelEnabled);
  }
  virtual ~CobraBodyParts() = default;
private:
  void appear() override {
    this->attr_ene_bullet();
  }
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override final;
};
void CobraBodyParts::upd_ene(float dt)
{
  this->upd_ene_common(dt);
}
void CobraBodyParts::draw(sf::RenderWindow& window) {
  if (m_appear_flag) return;

  this->draw_common(window);
}

//root
class SingleBossCobra final : public CobraParts {
public:
  SingleBossCobra(const EntityArgs& args);
  virtual ~SingleBossCobra() = default;
private:
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override final;

  fabrik::IK m_ik;
};
SingleBossCobra::SingleBossCobra(const EntityArgs& args)
  : CobraParts(args)
{
  //child
  Entity* parent = this;
  constexpr size_t parts_num = 6;
  for (size_t i = 1; i <= parts_num; ++i) {
    auto p = args.m_pos + Vec2f(-(m_radius*2.f-2.f) * i, 0.f);

    CobraParts* child = nullptr;
    if (i == parts_num) child = new CobraHeadParts({ EntityDataId::CobraHeadParts, p-Vec2f(8.f,0.f) });
    else child = new CobraBodyParts({ EntityDataId::CobraBodyParts, p });

    Entity::set_hierarchy(parent, child);
    parent = child;
  }
  //ik
  m_ik.awake(this);
}
void SingleBossCobra::upd_ene(float dt)
{
  this->upd_ene_common(dt);
  //ik
  if (m_effector) m_effector->apply_effector();
  m_ik.update();

  if (!m_appear_flag) {
    PtclLst::add_sqr(m_pos, 1, m_radius + 6.0f);
  }
}
void SingleBossCobra::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

  this->draw_common(window);
}

BossCobra::BossCobra(const EntityArgs& args)
  : Boss(args)
{
  this->set_health_max();
  //circle
  this->set_circle_radius(m_radius);

  //child
  Entity* parent = this;
  {
    EntityArgs args(EntityDataId::CobraBodyParts, args.m_pos+Vec2f(2.f,2.f));
    auto child = new SingleBossCobra(args);
    //Entity::set_hierarchy(parent, child);
  }
}

void BossCobra::appear()
{
}

void BossCobra::update(float dt)
{
  this->update_dt(dt);
  Boss::update(dt);
}

void BossCobra::upd_ene(float dt)
{
  m_arms_timer += dt;
  this->upd_ene_base(dt);
}

void BossCobra::draw(sf::RenderWindow& window)
{
  this->draw_circle(window);
}