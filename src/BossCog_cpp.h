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
    this->draw_circle(window);
  }
  void upd_ene(float dt) override {
    this->upd_blink(dt);
  }
};

//root
class CogRootParts : public CogParts {
public:
  CogRootParts(const EntityArgs& args)
    : CogParts(args)
  {
    m_flag.reset(EntityFlag::ForceAddVelEnabled);

    //child
    Entity* parent = this;
    constexpr size_t parts_num = 5;
    for (size_t i = 1; i <= parts_num; ++i) {
      auto p = args.m_pos + Vec2f(-(m_radius * 2.f - 2.f) * i, 0.f);
      auto* child = new CogParts({ EntityDataId::CobraBodyParts, p });

      Entity::set_hierarchy(parent, child);
      parent = child;
    }
    //ik
    m_ik.awake(this);
  }
  virtual ~CogRootParts() = default;
  void upd_ene(float dt) override final {
#if DEBUG
    const auto& input = Input::inst().input_data(0);
    m_ik.get_root_chain()->set_target_position(input.mouse());
#endif
    CogParts::upd_ene(dt);
    //ik
    if (m_effector) m_effector->apply_effector();
    m_ik.update();
  }
protected:
  void appear() override {
    this->attr_ene_bullet(); //FIXME:どうする 
  }
  fabrik::IK m_ik;
};

BossCog::BossCog(const EntityArgs& args)
  : Boss(args)
{
  this->set_health_max();
  //circle
  this->set_circle_radius(m_radius);

  //child
  {
    constexpr size_t parts_num = 8;
    constexpr float prad = 2.0f*fw::PI / parts_num;
    Vec2f vec{const_param::LvRadius-m_radius*0.4f, 0.f};
    for (size_t i = 0; i < parts_num; ++i) {
      const auto p = vec.rotate(prad * i);
      new CogRootParts({EntityDataId::CobraBodyParts, p});
    }
  }
}

void BossCog::appear()
{
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
}

void BossCog::draw(sf::RenderWindow& window)
{
  this->draw_circle(window);
}