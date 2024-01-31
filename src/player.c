#include "main.h"
#include "entityManager.h"
// #include "dynArray.h"
#include "player.h"
#include "behavior.h"
#include "transform.h"

void playerProcess( BehaviorC* behavior ) {
	TransformC* transform = transformGetByEntityId( behavior->header.entityId );
	Vector2 dir = { 0, 0 };
	float delta = GetFrameTime();
	float speed = 120.0;

	if ( IsKeyDown( KEY_RIGHT ) ) {
		dir.x = 1;
	}
	else if ( IsKeyDown( KEY_LEFT ) ) {
		dir.x = -1;
	}
	if ( IsKeyDown( KEY_DOWN ) ) {
		dir.y = 1;
	}
	else if ( IsKeyDown( KEY_UP ) ) {
		dir.y = -1;
	}

	if ( transform != NULL ) {
		transform->position.x += dir.x * speed * delta;
		transform->position.y += dir.y * speed * delta;
	}
}
