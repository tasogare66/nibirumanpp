#ifndef __ENEMY_TYPE_H__
#define __ENEMY_TYPE_H__

#undef ENEMY_TYPE_DECL
#define ENEMY_TYPE_DECL(_id,_cls,_edid)	_id,
enum class EnemyType {
  #include "EnemyType.h"
};
#undef ENEMY_TYPE_DECL


#endif // __ENEMY_TYPE_H__


#ifdef ENEMY_TYPE_DECL

ENEMY_TYPE_DECL(SNAKE, EneSnake, EntityDataId::EneSnake)
ENEMY_TYPE_DECL(URCHIN, EneSnake, EntityDataId::EneUrchin)
ENEMY_TYPE_DECL(GRUNT, EneGrunt, EntityDataId::None)
ENEMY_TYPE_DECL(HULK, EneHulk, EntityDataId::None)
ENEMY_TYPE_DECL(ARROW, EneArrow, EntityDataId::EneArrow)
ENEMY_TYPE_DECL(ARROW2, EneArrow2, EntityDataId::EneArrow)
ENEMY_TYPE_DECL(SPHE, EneSphe, EntityDataId::None)
ENEMY_TYPE_DECL(HUMAN, Human, EntityDataId::None)
ENEMY_TYPE_DECL(BOSS_BABY, BossBaby, EntityDataId::None)
ENEMY_TYPE_DECL(BOSS_WORM, BossWorm, EntityDataId::None)
ENEMY_TYPE_DECL(BOSS_URCHIN, BossUrchin, EntityDataId::BossUrchin)

#endif // ENEMY_TYPE_DECL