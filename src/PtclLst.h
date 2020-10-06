#pragma once
#include "Vec2.h"
#include "Singleton.h"

class PtclLst : public Singleton<PtclLst> {
public:
  struct Ptcl {
    Ptcl(Vec2f pos, Vec2f dir, uint8_t col)
      : m_pos(pos)
      , m_dir(dir)
      , m_col(col)
    {}
    Vec2f m_pos;
    Vec2f m_dir;
    float m_lifetm = 1.0f;
    uint8_t m_col;
    void update(float dt) {
      m_lifetm -= dt;
      if (m_lifetm > 0.0f) {
        m_pos += m_dir * dt;
      }
    }
  };
  struct Sqptcl {
    Sqptcl(Vec2f pos, Vec2f dir, uint8_t col, float side, float side_ed, float lifetm)
      : m_pos(pos)
      , m_dir(dir)
      , m_lifetm(lifetm)
      , m_lifetm_base(lifetm)
      , m_wh(side)
      , m_whhalf(side/2.0f)
      , m_whst(side)
      , m_whed(side_ed)
      , m_col(col)
    {}
    Vec2f m_pos;
    Vec2f m_dir;
    float m_lifetm;
    float m_lifetm_base;
    float m_wh;
    float m_whhalf;
    float m_whst;
    float m_whed;
    uint8_t m_col;
    void update(float dt) {
      m_lifetm -= dt;
      if (m_lifetm > 0.0f) {
        m_pos += m_dir * dt;
        m_wh = fw::lerp(m_whed, m_whst, m_lifetm / m_lifetm_base);
        m_whhalf = m_wh / 2.f;
      }
    }
  };
  PtclLst();
  ~PtclLst();
  void update(float dt);
  void draw(sf::RenderWindow& window);
  void clear() {
    m_ptcl_lst.clear();
    m_sqp_lst.clear();
  }
  size_t count() const { return m_ptcl_lst.size() + m_sqp_lst.size(); }
  static void add(Vec2f p, int32_t col) {
    inst().add_col(p, col);
  }
  static void add_sqr(Vec2f p, int32_t num, float radius) {
    inst().add_sq(p, num, radius);
  }
private:
  void add_col(Vec2f p, int32_t col);
  void add_sq(Vec2f p, int32_t num, float radius);
  //void add_bossap();
  std::vector<Ptcl> m_ptcl_lst;
  std::vector<Sqptcl> m_sqp_lst;
  sf::Sprite m_spr;
};