#pragma once

#include "main.h"
#include "dynArray.h"

enum ComponentTypes{
	COM_TYPE_NULL = -1,
	COM_TYPE_BEHAVIOR = 0,
	COM_TYPE_TRANSFORM,
	COM_TYPE_HITBOX,
	COM_TYPE_SPRITE
};

#define COM_TYPE_COUNT COM_TYPE_SPRITE + 1 /* We don't count COM_TYPE_NULL here. */

typedef struct Entity Entity;
/* Components have suffix C. */
typedef struct BehaviorC BehaviorC;
typedef struct TransformC TransformC;
typedef struct HitboxC HitboxC;
typedef struct SpriteC SpriteC;

/* Component reference for entities. */
typedef struct {
	int id;
	int type;
} Ref;

/* Component header. Must always be the first member of component struct. */
typedef struct {
	int id; /* Negative id for free component. */
	int entityId;
	int type;
} ComHeader;

struct Entity {
	int id;
	DynArray components; /* References (Ref) to components. */
	// char* name;
};

typedef struct {
	DynArray entities;
	DynArray components[ COM_TYPE_COUNT ];
} EntityManager;

extern EntityManager* entityManager;

/* Dynamic array callbacks. */
void entityFree( void* elementP, bool init );
bool entityIsFree( void* elementP );
void entityComFree( void* elementP, bool init );
bool entityComIsFree( void* elementP );
bool comDefaultIsFree( void* elementP );
/* Entity manager functions. */
void entityManagerInit();
void entityManagerFree();
/* Entity functions. */
Entity* entityNew();
void entityRemove( int id );
Entity* entityGet( int id );
void* entityGetComponentByType( int entityId, int type );
