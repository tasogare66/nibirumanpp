﻿#pragma once
#include "Entity.h"

class Player;
class Enemy :public Entity {
public:
  Enemy(EntityType type, const EntityArgs& args, uint32_t spr_ene=0);
  Enemy(const EntityArgs& args, uint32_t spr_ene=0);
  virtual ~Enemy() = default;
  virtual void init() override;
  virtual void update(float dt) override;
  virtual void dead() override;
  float get_elapsed() const { return m_elapsed; }
protected:
  virtual void appear() {}
  virtual void upd_ene(float dt) = 0;
  virtual bool set_blink(float blink_tm) override;
  virtual void play_blink_sfx() const override;
  void upd_blink(float dt) { if (m_blink > 0.f) { m_blink -= dt; } }
  bool is_blink() const { return (m_blink >= m_blinktm / 2.f); }
  void end_appear_state() {
    this->appear();
    m_appear_flag = false;
  }
  void ene_spr8x8(float elp, SprFlag spr_flag=SPRFLAG_NON);
  bool m_appear_flag = true; //出現中の場合,true
  float m_appear_sec = 2.0f; //出現の時間
  float m_elapsed = 0.0f;
  float m_blink = 0.0f;
  float m_blinktm=m_common_blinktm;
  uint32_t m_spr_ene = Entity::m_dummy_spr_id;
  uint16_t m_spr_num = 1;
  float m_anim_wait_frame = 4.0f;
  static constexpr uint32_t m_common_blink_spr = 267;
};

class EneSnake : public Enemy {
public:
  EneSnake(const EntityArgs& args);
  ~EneSnake() = default;
private:
  void appear() override final;
  void upd_ene(float dt) override final;
  const Player* m_tgt = nullptr;
};

class EneGrunt : public Enemy {
public:
  EneGrunt(const EntityArgs& args);
  ~EneGrunt() = default;
private:
  void appear() override final;
  void upd_ene(float dt) override final;
  float m_rad = 0.f;
};

class EneHulk : public Enemy {
public:
  EneHulk(const EntityArgs& args);
  ~EneHulk() = default;
private:
  void appear() override final;
  void upd_ene(float dt) override final;
  void setmvtm();
  float m_mvtm = 0.0f;
  uint32_t m_animdir = 0;
};

class EneArrow : public Enemy {
public:
  EneArrow(const EntityArgs& args, uint32_t spr_ene);
  EneArrow(const EntityArgs& args) : EneArrow(args, 368){}
  virtual ~EneArrow() = default;
protected:
  void appear() override;
  void upd_ene(float dt) override;
  bool hit_wall(const Vec2f& dir) override;
  float m_speed;
};

//arrow2,反射が違う
class EneArrow2 final : public EneArrow {
public:
  EneArrow2(const EntityArgs& args);
  ~EneArrow2() = default;
protected:
  bool hit_wall(const Vec2f& dir) override;
};

//life timeがある
class BossArrow final : public EneArrow {
public:
  BossArrow(const EntityArgs& args);
  ~BossArrow() = default;
protected:
  void init() override;
  void update(float dt) override;
  float m_lifetime=8.0f;
};

class EneSphe : public Enemy {
public:
  EneSphe(const EntityArgs& args);
  ~EneSphe() = default;
private:
  void appear() override;
  void upd_ene(float dt) override;
  float m_rotr;
  float m_rdir = 1.f;
  float m_speed;
};

class BossBullet : public Enemy {
public:
  BossBullet(const EntityArgs& args);
  virtual ~BossBullet() = default;
protected:
  void init() override;
  void update(float dt) override;
  void upd_ene(float) override {}
  bool hit_wall(const Vec2f& dir) override;
  float m_speed;
};