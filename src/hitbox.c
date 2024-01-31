#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "hitbox.h"
#include "transform.h"

void hitboxFree( void* elementP ) {
	HitboxC* hitbox = (HitboxC*)elementP;

	hitbox->header = (ComHeader){ .id = -1, .entityId = -1 };
	hitbox->rect = (Rectangle){ 0, 0, 0, 0 };
	hitbox->offset = (Vector2){ 0, 0 };
	// hitbox->transform = NULL;
}

bool hitboxIsFree( void* elementP ) {
	HitboxC* hitbox = (HitboxC*)elementP;

	return hitbox->header.id < 0;
}

HitboxC* hitboxNew( Entity* entity, Rectangle rect ) {
	void* elementP = NULL;
	int id = dynArrayAddElement( &entityManager->hitboxes, &elementP );
	HitboxC* hitbox = (HitboxC*)elementP;

	hitbox->header = (ComHeader){ .id = id, .entityId = entity->id };
	hitbox->rect = rect;
	hitbox->offset = (Vector2){ rect.x, rect.y };

// 	printf( "id %d entityId %d Array entityId %d\n", hitbox->header.id, hitbox->header.entityId,
// dynArrayGetElementByEntityId( &entityManager->hitboxes, hitbox->header.id ) );

	entity->signature.hitbox = true;

	return hitbox;
}

HitboxC* hitboxGet( int id ) {
	return (HitboxC*)dynArrayGetElement( &entityManager->hitboxes, id );
}

/* Return NULL of not found. */
HitboxC* hitboxGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->hitboxes.size; i++ ) {
		HitboxC* hitbox = hitboxGet( i );

		if ( hitbox->header.entityId == id ) {
			return hitbox;
		}
	}
	return NULL;
}

void hitboxProcess() {
	for ( int i = 0; i < entityManager->hitboxes.size; i++ ) {
		HitboxC* hitbox = hitboxGet( i );

		if ( 0 <= hitbox->header.id ) {
			TransformC* transform = transformGetByEntityId( hitbox->header.entityId );

			if ( transform != NULL ) {
				hitbox->rect.x = transform->position.x + hitbox->offset.x;
				hitbox->rect.y = transform->position.y + hitbox->offset.y;
			}
		}
	}
}

void hitboxDraw() {
	for ( int i = 0; i < entityManager->hitboxes.size; i++ ) {
		HitboxC* hitbox = hitboxGet( i );

		if ( 0 <= hitbox->header.id ) {
			Rectangle rect = hitbox->rect;
			DrawRectangleLines( rect.x, rect.y, rect.width, rect.height, RED );
		}
	}
}
