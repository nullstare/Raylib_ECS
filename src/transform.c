#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "transform.h"

void transformFree( void* elementP ) {
	TransformC* transform = (TransformC*)elementP;

	transform->header = (ComHeader){ .id = -1, .entityId = -1 };
	transform->position = (Vector2){ 0, 0 };
	transform->scale = (Vector2){ 0, 0 };
	transform->rotation = 0.0;
}

bool transformIsFree( void* elementP ) {
	TransformC* transform = (TransformC*)elementP;

	return transform->header.id < 0;
}

TransformC* transformNew( Entity* entity, Vector2 position, Vector2 scale, float rotation ) {
	void* elementP = NULL;
	int id = dynArrayAddElement( &entityManager->transforms, &elementP );
	TransformC* transform = (TransformC*)elementP;

	transform->header = (ComHeader){ .id = id, .entityId = entity->id };
	transform->position = position;
	transform->scale = scale;
	transform->rotation = rotation;

	entity->signature.transform = true;

	return transform;
}

TransformC* transformGet( int id ) {
	return (TransformC*)dynArrayGetElement( &entityManager->transforms, id );
}

/* Return NULL of not found. */
TransformC* transformGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->transforms.size; i++ ) {
		TransformC* transform = transformGet( i );

		if ( transform->header.entityId == id ) {
			return transform;
		}
	}
	return NULL;
}

void transformDraw() {
	for ( int i = 0; i < entityManager->transforms.size; i++ ) {
		TransformC* transform = transformGet( i );

		if ( 0 <= transform->header.id ) {
			Vector2 position = transform->position;
			DrawCircle( position.x, position.y, 2.0, GREEN );
		}
	}
}
