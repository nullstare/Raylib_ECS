#include "main.h"
#include "entityManager.h"
#include "dynArray.h"
#include "behavior.h"

void behaviorFree( void* elementP ) {
	BehaviorC* behavior = (BehaviorC*)elementP;

	behavior->header = (ComHeader){ .id = -1, .entityId = -1 };
	behavior->process = NULL;
}

bool behaviorIsFree( void* elementP ) {
	BehaviorC* behavior = (BehaviorC*)elementP;

	return behavior->header.id < 0;
}

BehaviorC* behaviorNew( Entity* entity, void (*process)( BehaviorC* behavior ) ) {
	void* elementP = NULL;
	int id = dynArrayAddElement( &entityManager->behaviors, &elementP );
	BehaviorC* behavior = (BehaviorC*)elementP;

	behavior->header = (ComHeader){ .id = id, .entityId = entity->id };
	behavior->process = process;

	entity->signature.behavior = true;

	return behavior;
}

BehaviorC* behaviorGet( int id ) {
	return (BehaviorC*)dynArrayGetElement( &entityManager->behaviors, id );
}

/* Return NULL of not found. */
BehaviorC* behaviorGetByEntityId( int id ) {
	for ( int i = 0; i < entityManager->behaviors.size; i++ ) {
		BehaviorC* behavior = behaviorGet( i );

		if ( behavior->header.entityId == id ) {
			return behavior;
		}
	}
	return NULL;
}

void behaviorProcess() {
	for ( int i = 0; i < entityManager->behaviors.size; i++ ) {
		BehaviorC* behavior = behaviorGet( i );

		if ( 0 <= behavior->header.id ) {
			behavior->process( behavior );
		}
	}
}
