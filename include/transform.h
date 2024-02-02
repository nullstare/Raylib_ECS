#pragma once

#include "entityManager.h"

struct TransformC{
	ComHeader header;
	Vector2 position;
	Vector2 scale;
	float rotation;
};

/* Dynamic array callbacks. */
void transformFree( void* elementP, bool init );
/* Transform functions. */
TransformC* transformNew( Entity* entity, Vector2 position, Vector2 scale, float rotation );
TransformC* transformGet( int id );
TransformC* transformGetByEntityId( int id );
void transformDraw();
