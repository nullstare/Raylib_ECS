#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "behavior.h"

/* Dynamic array callbacks. */

void behaviorFree( void* elementP, bool init ) {
	BehaviorC* behavior = (BehaviorC*)elementP;

	behavior->header = (ComHeader){ .id = -1, .entityId = -1, .type = COM_TYPE_NULL };
	behavior->process = NULL;
}

/* Behavior functions. */

BehaviorC* behaviorNew( Entity* entity, void (*process)( BehaviorC* behavior ) ) {
	BehaviorC* behavior = NULL;
	int id = dynArrayAddElement( &entityManager->components[ COM_TYPE_BEHAVIOR ], (void*)&behavior );

	behavior->header = (ComHeader){ .id = id, .entityId = entity->id, .type = COM_TYPE_BEHAVIOR };
	behavior->process = process;

	/* Add component to entity component list. */
	Ref* ref = NULL;
	dynArrayAddElement( &entity->components, (void*)&ref );
	ref->id = id;
	ref->type = COM_TYPE_BEHAVIOR;

	return behavior;
}

BehaviorC* behaviorGet( int id ) {
	return (BehaviorC*)dynArrayGetElement( &entityManager->components[ COM_TYPE_BEHAVIOR ], id );
}

/* Return NULL if not found. */
BehaviorC* behaviorGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_BEHAVIOR ].size; i++ ) {
		BehaviorC* behavior = behaviorGet( i );

		if ( behavior->header.entityId == id ) {
			return behavior;
		}
	}
	return NULL;
}

void behaviorProcess() {
	for ( int i = 0; i < entityManager->components[ COM_TYPE_BEHAVIOR ].size; i++ ) {
		BehaviorC* behavior = behaviorGet( i );

		if ( 0 <= behavior->header.id ) {
			behavior->process( behavior );
		}
	}
}
