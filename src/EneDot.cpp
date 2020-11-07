#include "stdafx.h"

#include "Player.h"
#include "GameSeq.h"
#include "Random.h"
#include "Sound.h"

#include "EneDot.h"

EneDot::EneDot(const EntityArgs& args)
  : Entity(EntityType::Dot, args)
{
  m_score = 300;
  this->spr8x8(m_ene_spr);
}

EneDot::~EneDot()
{
}

void EneDot::init()
{
  m_flag.on(EntityFlag::Ally);
  this->attr_ene_dot();
}

void EneDot::update(float dt)
{
  if (!m_capture_pl) {
    m_capture_pl = this->check_kill_by_player_random();
  }

  if (m_capture_pl) {
    auto t = m_captime * m_captime * m_captime;
    this->set_position(fw::lerp(m_pos, m_capture_pl->get_pos(), t));
    auto ttl = 1.f;
    m_captime = std::min(m_captime + dt / ttl, 1.f);
  }
  else if (m_timer < 0) {
    this->del();
    return;
  }
  this->spr8x8(m_ene_spr + static_cast<uint32_t>(m_timer / 0.333f) % 2);
  m_timer -= dt;
}

void EneDot::hitcb(const Entity* o, const Vec2f& dir, float dist)
{
  m_hit_mask.on(o->get_colli_attr());

  auto player = o->cast_to<Player>();
  if (!player) return;
  if (m_capture_pl && m_capture_pl != player) return; //capture済ならcapture_playerのみ

  if (dist < m_radius) {
    //capture済ならそのplayer
    //そうでないなら各playerに入れる(同フレームでは複数入る)
    auto idx = m_capture_pl ? m_capture_pl->get_index() : player->get_index();
    GameSeq::add_score(idx, m_score);
    GameSeq::add_multiplier(idx);
    this->del();
    m_acquired = true;
  }
}

void EneDot::dead()
{
  if (m_acquired) {
    // GAME:reduceDiff(0.5)
    Sound::psfx(SfxId::Dot, SndChannel::SFX2);
  }
}
