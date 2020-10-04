#include "stdafx.h"

#include "Random.h"
#include "Resource.h"
#include "ConstParam.h"

#include "PtclLst.h"

PtclLst::PtclLst()
{
  m_lst.reserve(2048);
  m_spr.setTexture(Resource::inst().get_pix_tex());
}

PtclLst::~PtclLst()
{
}

void PtclLst::update(float dt)
{
  if (!m_lst.empty()) {
    auto* ptcl = &m_lst[0];
    for (size_t i = 0; i < m_lst.size(); ++i,++ptcl) {
      ptcl->m_lifetm -= dt;
      if (ptcl->m_lifetm > 0.0f) {
        ptcl->m_pos += ptcl->m_dir * dt;
      }
    }
  }
  //remove
  m_lst.erase(std::remove_if(m_lst.begin(), m_lst.end(), [](auto& p)->bool {
    return (p.m_lifetm <= 0.0f);
  }), m_lst.end());

}

void PtclLst::draw(sf::RenderWindow& window)
{
  if (!m_lst.empty()) {
    auto* ptcl = &m_lst[0];
    for (size_t i = 0; i < m_lst.size(); ++i, ++ptcl) {
      m_spr.setPosition(ptcl->m_pos);
      m_spr.setColor(const_param::ticcol(ptcl->m_col));
      window.draw(m_spr);
    }
  }
}

void PtclLst::add_col(Vec2f p, int32_t col)
{
  for (int i = 0; i < 6;++i) {
    auto theta = rng::range(static_cast<float>(-M_PI),static_cast<float>(M_PI));
    m_lst.emplace_back(p, Vec2f(std::cos(theta), std::sin(theta))*rng::range(15.0f,24.0f), col);
  }
}
