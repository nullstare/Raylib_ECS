#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "enemy.h"
#include "behavior.h"
#include "transform.h"

void enemyProcess( BehaviorC* behavior ) {
	TransformC* transform = entityGetComponentByType( behavior->header.entityId, COM_TYPE_TRANSFORM );
	Vector2 dir = { 1, 0 };
	float delta = GetFrameTime();
	double time = GetTime();
	float speed = 20.0;

	// printf( "time %f\n", time );

	if ( transform != NULL ) {
		transform->position.x += dir.x * speed * delta;
		transform->position.y += dir.y * speed * delta;
	}
}
