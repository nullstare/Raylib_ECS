#pragma once

#include "entityManager.h"

struct BehaviorC{
	ComHeader header;
	void (*process)( BehaviorC* behavior );
};

/* Dynamic array callbacks. */
void behaviorFree( void* elementP, bool init );
/* Behavior functions. */
BehaviorC* behaviorNew( Entity* entity, void (*process)( BehaviorC* behavior ) );
BehaviorC* behaviorGet( int id );
BehaviorC* behaviorGetByEntityId( int id );
void behaviorProcess();
