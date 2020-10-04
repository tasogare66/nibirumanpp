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
  };
  PtclLst();
  ~PtclLst();
  void update(float dt);
  void draw(sf::RenderWindow& window);
  void clear() { m_lst.clear(); }
  size_t count() const { return m_lst.size(); }
  static void add(Vec2f p, int32_t col) {
    inst().add_col(p, col);
  }
private:
  void add_col(Vec2f p, int32_t col);
  std::vector<Ptcl> m_lst;
  sf::Sprite m_spr;
};