#pragma once

#include "main.h"
#include "dynArray.h"

#define MAX_COMPONENTS 8

typedef struct Entity Entity;
/* Components have suffix C. */
typedef struct HitboxC HitboxC;
typedef struct TransformC TransformC;
typedef struct BehaviorC BehaviorC;
typedef struct SpriteC SpriteC;

/* Component header. Must always be the first member of component struct. */
typedef struct {
	int id; /* Negative id for free component. */
	int entityId;
} ComHeader;

/* If has signature, contains at least one such component. */
typedef struct {
	size_t hitbox : 1;
	size_t transform : 1;
	size_t behavior : 1;
	size_t sprite : 1;
} Signature;

struct Entity {
	int id;
	Signature signature;
};

typedef struct {
	DynArray entities;
	/* Components. */
	DynArray transforms;
	DynArray hitboxes;
	DynArray behaviors;
	DynArray sprites;
	// PhysicsManager physicsManager (also tile collider)
	// HealthManager healthManager
	// DamageManager damageManager
	// ParticleManager particleManager
	// TimerManager timerManager ?
	// StateManager stateManager ?
} EntityManager;

extern EntityManager* entityManager;

void entityFree( void* elementP );
bool entityIsFree( void* elementP );
void entityManagerInit();
void entityManagerFreeEntity( int id );
void entityManagerFree();
Entity* entityNew();
Entity* entityGet( int id );
