#include "main.h"
#include "dynArray.h"
#include "entityManager.h"
#include "behavior.h"
#include "transform.h"
#include "hitbox.h"
#include "sprite.h"

EntityManager* entityManager;

/* Dynamic array callbacks. */

void entityFree( void* elementP, bool init ) {
	Entity* entity = (Entity*)elementP;
	entity->id = -1;
}

bool entityIsFree( void* elementP ) {
	Entity* entity = (Entity*)elementP;
	return entity->id < 0;
}

void entityComFree( void* elementP, bool init ) {
	Ref* ref = (Ref*)elementP;
	ref->id = -1;
	ref->type = COM_TYPE_NULL;
}

bool entityComIsFree( void* elementP ) {
	Ref* ref = (Ref*)elementP;
	return ref->id < 0;
}

bool comDefaultIsFree( void* elementP ) {
	ComHeader* comHeader = (ComHeader*)elementP;
	return comHeader->id < 0;
}

/* Entity manager functions. */

void entityManagerInit() {
	entityManager = malloc( sizeof( EntityManager ) );
	dynArrayInit( &entityManager->entities, sizeof( Entity ), entityFree, entityIsFree );

	dynArrayInit( &entityManager->components[ COM_TYPE_BEHAVIOR ], sizeof( BehaviorC ), behaviorFree, comDefaultIsFree );
	dynArrayInit( &entityManager->components[ COM_TYPE_TRANSFORM ], sizeof( TransformC ), transformFree, comDefaultIsFree );
	dynArrayInit( &entityManager->components[ COM_TYPE_HITBOX ], sizeof( HitboxC ), hitboxFree, comDefaultIsFree );
	dynArrayInit( &entityManager->components[ COM_TYPE_SPRITE ], sizeof( SpriteC ), spriteFree, comDefaultIsFree );
}

void entityManagerFree() {
	dynArrayFree( &entityManager->entities );

	for ( int i = 0; i < COM_TYPE_COUNT; i++ ) {
		dynArrayFree( &entityManager->components[i] );
	}
	free( entityManager );
	TraceLog( LOG_INFO, "Entity System Freed" );
}

/* Entity functions. */

Entity* entityNew() {
	Entity* entity = NULL;
	int id = dynArrayAddElement( &entityManager->entities, (void*)&entity );

	entity->id = id;
	dynArrayInit( &entity->components, sizeof( Ref ), entityComFree, entityComIsFree );

	return entity;
}

void entityRemove( int entityId ) {
	Entity* entity = entityGet( entityId );

	for ( int entityComId = 0; entityComId < entity->components.size; entityComId++ ) {
		Ref* ref = (Ref*)dynArrayGetElement( &entity->components, entityComId );

		dynArrayFreeElement( &entityManager->components[ ref->type ], ref->id, false );
	}
	dynArrayClear( &entity->components );
	entityFree( entity, false );
}

Entity* entityGet( int id ) {
	return (Entity*)dynArrayGetElement( &entityManager->entities, id );
}

void* entityGetComponentByType( int entityId, int type ) {
	if ( entityId < 0 ) {
		return NULL;
	}
	Entity* entity = entityGet( entityId );

	for ( int i = 0; i < entity->components.size; i++ ) {
		Ref* ref = (Ref*)dynArrayGetElement( &entity->components, i );

		if ( 0 <= ref->id && ref->type == type ) {
			return dynArrayGetElement( &entityManager->components[ ref->type ], ref->id );
		}
	}
	return NULL;
}
