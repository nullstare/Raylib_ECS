#pragma once

#include "entityManager.h"

struct TransformC{
	ComHeader header;
	Vector2 position;
	Vector2 scale;
	float rotation;
};

void transformFree( void* elementP );
bool transformIsFree( void* elementP );
TransformC* transformNew( Entity* entity, Vector2 position, Vector2 scale, float rotation );
TransformC* transformGet( int id );
TransformC* transformGetByEntityId( int id );
void transformDraw();
