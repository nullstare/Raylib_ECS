#pragma once

#include "entityManager.h"
#include "resources.h"

struct SpriteC{
	ComHeader header;
	Vector2 offset;
	Texture* texture;
	SpriteAnimation* animation;
	Color tint;
	float animPos;
	float animSpeed;
};

void spriteFree( void* elementP );
bool spriteIsFree( void* elementP );
SpriteC* spriteNew( Entity* entity, Vector2 offset, Texture* texture, SpriteAnimation* animation, Color tint, float animSpeed );
SpriteC* spriteGet( int id );
SpriteC* spriteGetByEntityId( int id );
void spriteProcess();
void spriteDraw();
