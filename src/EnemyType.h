#ifndef __ENEMY_TYPE_H__
#define __ENEMY_TYPE_H__

#undef ENEMY_TYPE_DECL
#define ENEMY_TYPE_DECL(_id,_cls)	_id,
enum class EnemyType {
  #include "EnemyType.h"
};
#undef ENEMY_TYPE_DECL


#endif // __ENEMY_TYPE_H__


#ifdef ENEMY_TYPE_DECL

ENEMY_TYPE_DECL(SNAKE, EneSnake)
ENEMY_TYPE_DECL(GRUNT, EneGrunt)
ENEMY_TYPE_DECL(HULK, EneHulk)
ENEMY_TYPE_DECL(ARROW, EneArrow)
ENEMY_TYPE_DECL(ARROW2, EneArrow2)
ENEMY_TYPE_DECL(SPHE, EneSphe)
ENEMY_TYPE_DECL(HUMAN, Human)
ENEMY_TYPE_DECL(BOSS_BABY, BossBaby)
ENEMY_TYPE_DECL(BOSS_CELLS, BossCells)

#endif // ENEMY_TYPE_DECL