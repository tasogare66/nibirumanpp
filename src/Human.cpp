﻿#include "stdafx.h"

#include "Random.h"
#include "ConstParam.h"
#include "GameSeq.h"
#include "Player.h"
#include "Pop2D.h"
#include "Sound.h"

#include "Human.h"

Human::Human(const EntityArgs& args)
  : Enemy(EntityType::Human, args, 404)
{
  m_flag.set(EntityFlag::Ally);
  m_colli_attr.reset();
  m_score = 1000;
}

Human::~Human()
{
}

void Human::appear()
{
  this->attr_px();
  this->spr8x8(m_spr_ene);
}

void Human::upd_ene(float dt)
{
  if (m_hit_mask.test(HitMask::PlayerAll)) {
    this->del();
    return;
  }

  if (m_timer < 0.0f) {
    auto s = rng::range_int(1, 4);
    m_animdir = s - 1;
    if (s == 1) m_dir.set(20.0f, 0.f);
    else if (s == 2) m_dir.set(-20.0f, 0.f);
    else if (s == 3) m_dir.set(0.f, 20.f);
    else m_dir.set(0.f, -20.f);
    m_timer = rng::range(1.0f, 3.0f);
  }
  else {
    m_timer -= dt;
  }
  m_mov = m_dir * dt;
  auto cnt = static_cast<uint32_t>(m_elapsed / (const_param::FRAME2SEC * 10)) % 4;
  this->spr8x8(m_spr_ene + m_animdir * 16 + cnt);
}

void Human::dead()
{
  if (auto* player = this->check_kill_by_player_random()) {
    GameSeq::add_score(m_score);
    GameSeq::inst().reduceDiff(30);
    player->add_armslv();
    new Pop2D(m_pos);
    Sound::psfx(SfxId::Human, SndChannel::SFX3);
  } else {
    FW_ASSERT(0);
  }
}
