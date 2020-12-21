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
    float aT = i * pi2 / m_legs_num;
    float apv = aT, l = lb;
    Vec2f vDT(std::cos(aT), std::sin(aT)), vpv = vP;

    for (int32_t j = 0; j < m_node_num; ++j)
    {
      auto& n{m_all_nodes[i][j]};
      auto& vC = n.m_p;
      Vec2f vab = vC - vpv;
      Vec2f vAB = vab * InvLength(vab, 1.f);
      float aS = atan2(vAB.y, vAB.x);
      float cl = std::clamp(lambda_NA(aS - apv), -pi, pi);
      aS = apv + std::clamp(cl - cl * .1f, -pi, pi);
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
  Vec2f vP = m_pos;
  for (int32_t i = 0; i < m_legs_num; ++i) {
    Vec2f vpv = vP;

    for (int32_t j = 0; j < m_node_num; ++j)
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
