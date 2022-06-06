#pragma once

enum DIRECTION { DIR_LEFT, DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_END };

enum OBJ_LIST { OBJ_PLAYER, OBJ_MONSTER, OBJ_BLOCK, OBJ_ITEM, OBJ_BULLET, OBJ_END };

enum MONSTER_LIST { MONSTER_MUSHROOM, MONSTER_TURTLE, MONSTER_TURTLEBACK, MONSTER_JUMPING, MONSTER_FLYING, MONSTER_BOSS, MONSTER_END };

enum BLOCK_LIST { BLOCK_NORMAL, BLOCK_ITEM, BLOCK_COIN, BLOCK_FLAG, BLOCK_END };

enum ITEM_TYPE { ITEM_COIN, ITEM_MUSHROOM, ITEM_STAR, ITEM_FLOWER, ITEM_END };

enum STAGE_LIST { STAGE_1, STAGE_2, STAGE_3, STAGE_4, STAGE_EDITOR, STAGE_END };

enum BOSS_STATE { LEVEL1, LEVEL2, LEVEL3 };

enum PLAYER_LIST { PLAYER_NORMAL, PLAYER_END };

enum DIRTEMP { LEFT, RIGHT, END };