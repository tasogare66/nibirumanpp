class CobraParts : public BossParts {
public:
  CobraParts(const EntityArgs& args)
    : BossParts(args)
  {
    //circle
    this->set_circle_radius(m_radius);
  }
  virtual ~CobraParts() = default;
protected:
  void appear() override {
    this->attr_px();
  }
  void draw_common(sf::RenderWindow& window) {
    //this->Enemy::draw(window);
    this->draw_circle(window);
  }
};

class CobraChild final : public CobraParts {
public:
  CobraChild(const EntityArgs& args)
    : CobraParts(args)
  {
  }
  virtual ~CobraChild() = default;
private:
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override final;
};
void CobraChild::upd_ene(float dt)
{
}
void CobraChild::draw(sf::RenderWindow& window) {
  if (m_appear_flag) return;

  this->draw_common(window);
}

class SingleBossCobra final : public CobraParts {
public:
  SingleBossCobra(const EntityArgs& args);
  virtual ~SingleBossCobra() = default;
private:
  void upd_ene(float dt) override;
  void draw(sf::RenderWindow& window) override final;
};
SingleBossCobra::SingleBossCobra(const EntityArgs& args)
  : CobraParts(args)
{
  //m_flag.set(EntityFlag::IgnoreCollisionSameRoot);

  //child
  Entity* parent = this;
  for (size_t i = 1; i <= 10; ++i) {
    auto p = args.m_pos + Vec2f(-18.f * i, 0.f);
    auto child = new CobraChild({ EntityDataId::CobraChild, p });
    Entity::set_hierarchy(parent, child);
    parent = child;
  }
  //ik
}
void SingleBossCobra::upd_ene(float dt)
{
}
void SingleBossCobra::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

  this->draw_common(window);
}

BossCobra::BossCobra(const EntityArgs& args)
  : Boss(args, 340)
{
  this->set_health_max();
  //circle
  this->set_circle_radius(m_radius);

  //child
  Entity* parent = this;
  {
    EntityArgs args(EntityDataId::SingleBossCobra, args.m_pos+Vec2f(2.f,2.f));
    auto child = new SingleBossCobra(args);
    Entity::set_hierarchy(parent, child);
  }
}

void BossCobra::appear()
{
  this->attr_px();
}

void BossCobra::update(float dt)
{
  this->update_dt(dt);
  Boss::update(dt);
  this->upd_blink(dt);
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