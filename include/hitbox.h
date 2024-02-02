#pragma once

#include "entityManager.h"

struct HitboxC{
	ComHeader header;
	Rectangle rect;
	Vector2 offset;
	void (*collisionCallback)( HitboxC*, HitboxC* );
};

/* Dynamic array callbacks. */
void hitboxFree( void* elementP, bool init );
/* Hitbox functions. */
HitboxC* hitboxNew( Entity* entity, Rectangle rect, void (*collisionCallback)( HitboxC*, HitboxC* ) );
HitboxC* hitboxGet( int id );
HitboxC* hitboxGetByEntityId( int id );
void hitboxProcess();
void hitboxDraw();
