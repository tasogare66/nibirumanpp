#include "stdafx.h"

#include "Random.h"
#include "Resource.h"
#include "ConstParam.h"

#include "PtclLst.h"

PtclLst::PtclLst()
{
  m_ptcl_lst.reserve(2048);
  m_sqp_lst.reserve(512);
  m_spr.setTexture(Resource::inst().get_pix_tex());
  m_spr.setOrigin(0.5f,0.5f);
}

PtclLst::~PtclLst()
{
}

void PtclLst::update(float dt)
{
  auto lambda_update_list = [&dt](auto& lst) {
    if (!lst.empty()) {
      auto* ptcl = &lst[0];
      for (size_t i = 0; i < lst.size(); ++i, ++ptcl) {
        ptcl->update(dt);
      }
    }
    //remove
    lst.erase(std::remove_if(lst.begin(), lst.end(), [](const auto& p)->bool {
      return (p.m_lifetm <= 0.0f);
    }), lst.end());
  };
  lambda_update_list(m_ptcl_lst);
  lambda_update_list(m_sqp_lst);
}

void PtclLst::draw(sf::RenderWindow& window)
{
  if (!m_sqp_lst.empty()) {
    auto* ptcl = &m_sqp_lst[0];
    for (size_t i = 0; i < m_sqp_lst.size(); ++i, ++ptcl) {
      m_spr.setScale(ptcl->m_wh, ptcl->m_wh);
      m_spr.setPosition(ptcl->m_pos);
      m_spr.setColor(const_param::ticcol(ptcl->m_col));
      window.draw(m_spr);
    }
  }
  if (!m_ptcl_lst.empty()) {
    auto* ptcl = &m_ptcl_lst[0];
    m_spr.setScale(1.0f, 1.0f);
    for (size_t i = 0; i < m_ptcl_lst.size(); ++i, ++ptcl) {
      m_spr.setPosition(ptcl->m_pos);
      m_spr.setColor(const_param::ticcol(ptcl->m_col));
      window.draw(m_spr);
    }
  }
}

void PtclLst::add_col(Vec2f p, int32_t col)
{
  for (int i = 0; i < 6; ++i) {
    auto theta = rng::range(static_cast<float>(-M_PI),static_cast<float>(M_PI), rng::Type::EFFECT);
    m_ptcl_lst.emplace_back(p, Vec2f(std::cos(theta), std::sin(theta))*rng::range(15.0f,24.0f), col);
  }
}

void PtclLst::add_sq(Vec2f p, int32_t num, float radius)
{
  for (int i = 0; i < num; ++i) {
    auto r = std::sqrt(rng::randf(rng::Type::EFFECT)) * radius;
    auto theta = rng::range(static_cast<float>(-M_PI), static_cast<float>(M_PI), rng::Type::EFFECT);
    auto dir = Vec2f(std::cos(theta), std::sin(theta));
    auto ofs = dir * r;
    dir *= rng::range(4.0f, 12.0f);
    float side = rng::range(3.0f, 10.0f, rng::Type::EFFECT);
    uint8_t cols[] = {3,7};
    m_sqp_lst.emplace_back(p+ofs, dir, cols[rng::rand_int(static_cast<int32_t>(fw::array_size(cols)-1),rng::Type::EFFECT)], side, side, 0.7f);
  }
}
