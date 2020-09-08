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

#endif // ENEMY_TYPE_DECL