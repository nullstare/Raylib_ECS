#pragma once

#include "entityManager.h"

struct BehaviorC{
	ComHeader header;
	void (*process)( BehaviorC* behavior );
};

void behaviorFree( void* elementP );
bool behaviorIsFree( void* elementP );
BehaviorC* behaviorNew( Entity* entity, void (*process)( BehaviorC* behavior ) );
BehaviorC* behaviorGet( int id );
BehaviorC* behaviorGetByEntityId( int id );
void behaviorProcess();
