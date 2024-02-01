#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "hitbox.h"
#include "transform.h"

void hitboxFree( void* elementP ) {
	HitboxC* hitbox = (HitboxC*)elementP;

	hitbox->header = (ComHeader){ .id = -1, .entityId = -1, .type = COM_TYPE_NULL };
	hitbox->rect = (Rectangle){ 0, 0, 0, 0 };
	hitbox->offset = (Vector2){ 0, 0 };
}

bool hitboxIsFree( void* elementP ) {
	HitboxC* hitbox = (HitboxC*)elementP;
	return hitbox->header.id < 0;
}

HitboxC* hitboxNew( Entity* entity, Rectangle rect, void (*collisionCallback)( HitboxC*, HitboxC* ) ) {
	HitboxC* hitbox = NULL;
	int id = dynArrayAddElement( &entityManager->components[ COM_TYPE_HITBOX ], (void*)&hitbox );

	hitbox->header = (ComHeader){ .id = id, .entityId = entity->id, .type = COM_TYPE_HITBOX };
	hitbox->rect = rect;
	hitbox->offset = (Vector2){ rect.x, rect.y };
	hitbox->collisionCallback = collisionCallback;

	/* Add component to entity component list. */
	Ref* ref = NULL;
	dynArrayAddElement( &entity->components, (void*)&ref );
	ref->type = COM_TYPE_HITBOX;
	ref->id = id;

	return hitbox;
}

HitboxC* hitboxGet( int id ) {
	return (HitboxC*)dynArrayGetElement( &entityManager->components[ COM_TYPE_HITBOX ], id );
}

/* Return NULL if not found. */
HitboxC* hitboxGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_HITBOX ].size; i++ ) {
		HitboxC* hitbox = hitboxGet( i );

		if ( hitbox->header.entityId == id ) {
			return hitbox;
		}
	}
	return NULL;
}

void hitboxProcess() {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_HITBOX ].size; i++ ) {
		HitboxC* hitbox = hitboxGet( i );

		if ( 0 <= hitbox->header.id ) {
			TransformC* transform = entityGetComponentByType( hitbox->header.entityId, COM_TYPE_TRANSFORM );

			if ( transform != NULL ) {
				hitbox->rect.x = transform->position.x + hitbox->offset.x;
				hitbox->rect.y = transform->position.y + hitbox->offset.y;
			}
			/* Don't check collision if doesn't do anything. */
			if ( hitbox->collisionCallback != NULL ) {
				/* Note! Slow collision cheking. Grows exponentially. */
				for ( int j = 0; j < entityManager->components[ COM_TYPE_HITBOX ].size; j++ ) {
					HitboxC* hitbox2 = hitboxGet( j );
					/* Should not collide with itself. */
					if ( 0 <= hitbox2->header.id && i != j && CheckCollisionRecs( hitbox->rect, hitbox2->rect ) ) {
						hitbox->collisionCallback( hitbox, hitbox2 );

						if ( hitbox2->collisionCallback != NULL ) {
							hitbox2->collisionCallback( hitbox2, hitbox );
						}
					}
				}
			}
		}
	}
}

void hitboxDraw() {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_HITBOX ].size; i++ ) {
		HitboxC* hitbox = hitboxGet( i );

		if ( 0 <= hitbox->header.id ) {
			Rectangle rect = hitbox->rect;
			DrawRectangleLines( rect.x, rect.y, rect.width, rect.height, RED );
		}
	}
}
