#pragma once

#include "entityManager.h"

struct HitboxC{
	ComHeader header;
	Rectangle rect;
	Vector2 offset;
	// TransformC* transform;
};

void hitboxFree( void* elementP );
bool hitboxIsFree( void* elementP );
HitboxC* hitboxNew( Entity* entity, Rectangle rect );
HitboxC* hitboxGet( int id );
HitboxC* hitboxGetByEntityId( int id );
void hitboxProcess();
void hitboxDraw();
