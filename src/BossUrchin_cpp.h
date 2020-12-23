BossUrchin::BossUrchin(const EntityArgs& args)
  : Boss(args, 456)
{
  m_health_max = m_health;
  //circle
  this->set_circle_radius(m_radius);
  //script
  m_script.reset(scr::create_lua_boss_sequence(std::string_view("update_baby"), this));
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
  this->upd_ene_base(dt);
  //upd legs
  m_legs_rot += fw::deg2rad(60.f)*dt;
  m_legs_rot = atan2(sin(m_legs_rot), cos(m_legs_rot));
  this->upd_nodes(dt);
}

static inline float InvLength(const Vec2f& lhs, float fail_value) {
  float d = lhs.sqr_magnitude();
  if (d > 0.0f) return 1.0f / std::sqrtf(d);
  return fail_value;
}

void BossUrchin::upd_nodes(float dt)
{
  constexpr float lb = 10.f;
  constexpr float pi = static_cast<float>(M_PI);
  constexpr float pi2 = 2.f * pi;

  auto lambda_NA = [this,pi,pi2](float f)->float {
    return f - pi2 * std::floor((f + pi) / pi2);
  };

  Vec2f vP = m_pos;
  for (int32_t i = 0; i < m_legs_num; ++i) {
    float aT = i * pi2 / m_legs_num + m_legs_rot;
    float apv = aT;
    float l = lb;
    Vec2f vDT(std::cos(aT), std::sin(aT));

    Vec2f vpv = vP+vDT*m_radius;
    m_all_nodes[i][0].m_p = vpv;

    for (int32_t j = 1; j < m_node_num; ++j)
    {
      auto& n{m_all_nodes[i][j]};
      auto& vC = n.m_p;
      Vec2f vab = vC - vpv;
      Vec2f vAB = vab * InvLength(vab, 1.f);
      float aS = atan2(vAB.y, vAB.x);
      float cl = std::clamp(lambda_NA(aS - apv), -pi, pi);
      aS = apv + std::clamp(cl - cl * .2f, -pi, pi);
      Vec2f vD(std::cos(aS), std::sin(aS));
      vC = vpv + vD * l;

      apv = aS;
      vpv = vC;
      l *= i & 1 ? .9f : .85f;
    }
  }
}

void BossUrchin::draw(sf::RenderWindow& window)
{
  if (m_appear_flag) return;

  //circle
  this->draw_circle(window);

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
}
