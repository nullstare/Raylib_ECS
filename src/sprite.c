#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "sprite.h"
#include "transform.h"

/* Dynamic array callbacks. */

void spriteFree( void* elementP, bool init ) {
	SpriteC* sprite = (SpriteC*)elementP;

	sprite->header = (ComHeader){ .id = -1, .entityId = -1, .type = COM_TYPE_NULL };
	sprite->offset = (Vector2){ 0, 0 };
	sprite->texture = NULL;
	sprite->animation = NULL;
	sprite->tint = BLACK;
	sprite->animPos = 0.0;
}

/* Sprite functions. */

SpriteC* spriteNew( Entity* entity, Vector2 offset, Texture* texture, SpriteAnimation* animation,
	Color tint, float animSpeed )
{
	SpriteC* sprite = NULL;
	int id = dynArrayAddElement( &entityManager->components[ COM_TYPE_SPRITE ], (void*)&sprite );

	sprite->header = (ComHeader){ .id = id, .entityId = entity->id, .type = COM_TYPE_SPRITE };
	sprite->offset = offset;
	sprite->texture = texture;
	sprite->animation = animation;
	sprite->tint = tint;
	sprite->animPos = 0.0;
	sprite->animSpeed = animSpeed;

	/* Add component to entity component list. */
	Ref* ref = NULL;
	dynArrayAddElement( &entity->components, (void*)&ref );
	ref->id = id;
	ref->type = COM_TYPE_SPRITE;

	return sprite;
}

SpriteC* spriteGet( int id ) {
	return (SpriteC*)dynArrayGetElement( &entityManager->components[ COM_TYPE_SPRITE ], id );
}

/* Return NULL if not found. */
SpriteC* spriteGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_SPRITE ].size; i++ ) {
		SpriteC* sprite = spriteGet( i );

		if ( sprite->header.entityId == id ) {
			return sprite;
		}
	}
	return NULL;
}

void spriteProcess() {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_SPRITE ].size; i++ ) {
		SpriteC* sprite = spriteGet( i );
		
		if ( 0 <= sprite->header.id ) {
			sprite->animPos += sprite->animSpeed * GetFrameTime();

			if ( 1.0 < sprite->animPos ) {
				sprite->animPos -= 1.0;
			}
		}
	}
}

void spriteDraw() {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_SPRITE ].size; i++ ) {
		SpriteC* sprite = spriteGet( i );

		if ( 0 <= sprite->header.id && sprite->animation != NULL ) {
			TransformC* transform = entityGetComponentByType( sprite->header.entityId, COM_TYPE_TRANSFORM );
			Vector2 position = { sprite->offset.x, sprite->offset.y };
			Vector2 scale = { 1, 1 };

			if ( transform != NULL ) {
				position.x += transform->position.x;
				position.y += transform->position.y;
				scale = transform->scale;
			}

			int frame = floor( Lerp( 0.0, (float)sprite->animation->frameCount, sprite->animPos ) );
			Rectangle source = sprite->animation->source[ frame ];

			DrawTexturePro(
				*sprite->texture,
				source,
				(Rectangle){
					position.x,
					position.y,
					source.width * scale.x,
					source.height * scale.y
				},
				(Vector2){ 0.0, 0.0 },
				0.0,
				sprite->tint
			);
		}
	}
}
