#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "player.h"
#include "behavior.h"
#include "transform.h"
#include "hitbox.h"

void playerProcess( BehaviorC* behavior ) {
	TransformC* transform = entityGetComponentByType( behavior->header.entityId, COM_TYPE_TRANSFORM );
	Vector2 dir = { 0, 0 };
	float delta = GetFrameTime();
	float speed = 200.0;

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

/* collided is players hitbox. */
void playerCollision( HitboxC* collided, HitboxC* with ) {
	if ( IsKeyPressed( KEY_SPACE ) ) {
		entityRemove( with->header.entityId );
	}
}
