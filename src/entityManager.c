#include "main.h"
#include "dynArray.h"
#include "entityManager.h"
#include "behavior.h"
#include "transform.h"
#include "hitbox.h"
#include "sprite.h"

EntityManager* entityManager;

void entityFree( void* elementP ) {
	Entity* entity = (Entity*)elementP;
	entity->id = -1;
}

bool entityIsFree( void* elementP ) {
	Entity* entity = (Entity*)elementP;
	return entity->id < 0;
}

void entityComFree( void* elementP ) {
	Ref* ref = (Ref*)elementP;
	ref->type = COM_TYPE_NULL;
	ref->id = -1;
}

bool entityComIsFree( void* elementP ) {
	Ref* ref = (Ref*)elementP;
	return ref->id < 0;
}

void entityManagerInit() {
	entityManager = malloc( sizeof( EntityManager ) );
	dynArrayInit( &entityManager->entities, sizeof( Entity ), entityFree, entityIsFree );

	dynArrayInit( &entityManager->components[ COM_TYPE_BEHAVIOR ], sizeof( BehaviorC ), behaviorFree, behaviorIsFree );
	dynArrayInit( &entityManager->components[ COM_TYPE_TRANSFORM ], sizeof( TransformC ), transformFree, transformIsFree );
	dynArrayInit( &entityManager->components[ COM_TYPE_HITBOX ], sizeof( HitboxC ), hitboxFree, hitboxIsFree );
	dynArrayInit( &entityManager->components[ COM_TYPE_SPRITE ], sizeof( SpriteC ), spriteFree, spriteIsFree );
}

void entityManagerFree() {
	dynArrayFree( &entityManager->entities );

	for ( int i = 0; i < COMPONENT_TYPE_COUNT; i++ ) {
		dynArrayFree( &entityManager->components[i] );
	}
	free( entityManager );

	TraceLog( LOG_INFO, "Entity System Freed" );
}

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

		dynArrayFreeElement( &entityManager->components[ ref->type ], ref->id );
	}
	dynArrayClear( &entity->components );
	entityFree( entity );
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
