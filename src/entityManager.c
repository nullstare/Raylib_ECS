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
	memset( &entity->signature, 0, sizeof( Signature ) );
}

bool entityIsFree( void* elementP ) {
	Entity* entity = (Entity*)elementP;

	return entity->id < 0;
}

void entityManagerInit() {
	entityManager = malloc( sizeof( EntityManager ) );
	dynArrayInit( &entityManager->entities, sizeof( Entity ), entityFree, entityIsFree );
	dynArrayInit( &entityManager->behaviors, sizeof( TransformC ), behaviorFree, behaviorIsFree );
	dynArrayInit( &entityManager->transforms, sizeof( TransformC ), transformFree, transformIsFree );
	dynArrayInit( &entityManager->hitboxes, sizeof( HitboxC ), hitboxFree, hitboxIsFree );
	dynArrayInit( &entityManager->sprites, sizeof( SpriteC ), spriteFree, spriteIsFree );
}

void entityManagerFreeEntity( int id ) {
	Entity* entity = entityGet( id );

	if ( 0 <= entity->id ) {
		if ( entity->signature.behavior ) dynArrayFreeByEntityId( &entityManager->behaviors, entity->id );
		if ( entity->signature.transform ) dynArrayFreeByEntityId( &entityManager->transforms, entity->id );
		if ( entity->signature.hitbox )	dynArrayFreeByEntityId( &entityManager->hitboxes, entity->id );
		if ( entity->signature.sprite )	dynArrayFreeByEntityId( &entityManager->sprites, entity->id );
	}
	entityFree( entity );
}

void entityManagerFree() {
	dynArrayFree( &entityManager->entities );
	dynArrayFree( &entityManager->behaviors );
	dynArrayFree( &entityManager->transforms );
	dynArrayFree( &entityManager->hitboxes );
	dynArrayFree( &entityManager->sprites );

	free( entityManager );

	TraceLog( LOG_INFO, "Entity System Freed" );
}

Entity* entityNew() {
	void* elementP = NULL;
	int id = dynArrayAddElement( &entityManager->entities, &elementP );
	Entity* entity = (Entity*)elementP;

	entity->id = id;

	return entity;
}

Entity* entityGet( int id ) {
	return (Entity*)dynArrayGetElement( &entityManager->entities, id );
}
