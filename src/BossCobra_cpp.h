BossCobra::BossCobra(const EntityArgs& args)
  : Boss(args, 340)
{
  this->set_health_max();
  //circle
  this->set_circle_radius(m_radius);
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