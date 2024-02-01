#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "transform.h"

void transformFree( void* elementP ) {
	TransformC* transform = (TransformC*)elementP;

	transform->header = (ComHeader){ .id = -1, .entityId = -1, .type = COM_TYPE_NULL };
	transform->position = (Vector2){ 0, 0 };
	transform->scale = (Vector2){ 0, 0 };
	transform->rotation = 0.0;
}

bool transformIsFree( void* elementP ) {
	TransformC* transform = (TransformC*)elementP;
	return transform->header.id < 0;
}

TransformC* transformNew( Entity* entity, Vector2 position, Vector2 scale, float rotation ) {
	TransformC* transform = NULL;
	int id = dynArrayAddElement( &entityManager->components[ COM_TYPE_TRANSFORM ], (void*)&transform );

	transform->header = (ComHeader){ .id = id, .entityId = entity->id, .type = COM_TYPE_TRANSFORM };
	transform->position = position;
	transform->scale = scale;
	transform->rotation = rotation;

	/* Add component to entity component list. */
	Ref* ref = NULL;
	dynArrayAddElement( &entity->components, (void*)&ref );
	ref->type = COM_TYPE_TRANSFORM;
	ref->id = id;

	return transform;
}

TransformC* transformGet( int id ) {
	return (TransformC*)dynArrayGetElement( &entityManager->components[ COM_TYPE_TRANSFORM ], id );
}

/* Return NULL if not found. */
TransformC* transformGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_TRANSFORM ].size; i++ ) {
		TransformC* transform = transformGet( i );

		if ( transform->header.entityId == id ) {
			return transform;
		}
	}
	return NULL;
}

void transformDraw() {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_TRANSFORM ].size; i++ ) {
		TransformC* transform = transformGet( i );

		if ( 0 <= transform->header.id ) {
			Vector2 position = transform->position;
			DrawCircle( position.x, position.y, 2.0, GREEN );
		}
	}
}
