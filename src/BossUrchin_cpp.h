BossUrchin::BossUrchin(const EntityArgs& args)
  : Boss(args, 456)
{
  m_health_max = m_health;
  //circle
  this->set_circle_radius(m_radius);
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
  this->upd_ene_base(dt);
}

void BossUrchin::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

  //circle
  this->draw_circle(window);
}
