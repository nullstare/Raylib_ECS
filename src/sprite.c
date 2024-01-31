#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "sprite.h"
#include "transform.h"

void spriteFree( void* elementP ) {
	SpriteC* sprite = (SpriteC*)elementP;

	sprite->header = (ComHeader){ .id = -1, .entityId = -1 };
	sprite->offset = (Vector2){ 0, 0 };
	sprite->texture = NULL;
	sprite->animation = NULL;
	sprite->tint = BLACK;
	sprite->animPos = 0.0;
}

bool spriteIsFree( void* elementP ) {
	SpriteC* sprite = (SpriteC*)elementP;

	return sprite->header.id < 0;
}

SpriteC* spriteNew( Entity* entity, Vector2 offset, Texture* texture, SpriteAnimation* animation, Color tint ) {
	void* elementP = NULL;
	int id = dynArrayAddElement( &entityManager->sprites, &elementP );
	SpriteC* sprite = (SpriteC*)elementP;

	sprite->header = (ComHeader){ .id = id, .entityId = entity->id };
	sprite->offset = offset;
	sprite->texture = texture;
	sprite->animation = animation;
	sprite->tint = tint;
	sprite->animPos = 0.0;

	entity->signature.sprite = true;

	return sprite;
}

SpriteC* spriteGet( int id ) {
	return (SpriteC*)dynArrayGetElement( &entityManager->sprites, id );
}

/* Return NULL of not found. */
SpriteC* spriteGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->sprites.size; i++ ) {
		SpriteC* sprite = spriteGet( i );

		if ( sprite->header.entityId == id ) {
			return sprite;
		}
	}
	return NULL;
}

static void drawSprite( SpriteC* sprite, Rectangle dst, Color color ) {
	int frame = floor( Lerp( 0.0, (float)sprite->animation->frameCount, sprite->animPos ) );

	Rectangle src = sprite->animation->src[ frame ];

	DrawTexturePro(
		*sprite->texture,
		src,
		dst,
		(Vector2){ 0.0, 0.0 },
		0.0,
		color
	);
}

void spriteDraw() {
	for ( int i = 0; i < entityManager->sprites.size; i++ ) {
		SpriteC* sprite = spriteGet( i );
		TransformC* transform = transformGetByEntityId( sprite->header.entityId );
		Vector2 position = { sprite->offset.x, sprite->offset.y };
		Vector2 scale = { 1, 1 };

		if ( transform != NULL ) {
			position.x += transform->position.x;
			position.y += transform->position.y;
			scale = transform->scale;
		}

		if ( 0 <= sprite->header.id && sprite->animation != NULL ) {
			int frame = floor( Lerp( 0.0, (float)sprite->animation->frameCount, sprite->animPos ) );
			Rectangle src = sprite->animation->src[ frame ];

			DrawTexturePro(
				*sprite->texture,
				src,
				(Rectangle){
					position.x,
					position.y,
					sprite->texture->width * scale.x,
					sprite->texture->height * scale.y
				},
				(Vector2){ 0.0, 0.0 },
				0.0,
				sprite->tint
			);
		}
	}
}
