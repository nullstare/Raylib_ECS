#include "main.h"
#include "dynArray.h"
#include "entityManager.h"

static bool checkArrayRealloc( DynArray* arr, int id ) {
	if ( id == arr->size ) {
		arr->size++;
	}
	if ( arr->size == arr->capacity ) {
		arr->capacity += ALLOC_PAGE_SIZE;
		arr->elements = realloc( arr->elements, arr->capacity * arr->elementSize );

		for ( id = arr->size; id < arr->capacity; id++ ) {
			dynArrayFreeElement( arr, id, true );
		}
		return true;
	}
	return false;
}

/* elementP will be set to first free element slot. */
int dynArrayAddElement( DynArray* arr, void** elementP ) {
	int id = 0;
	*elementP = arr->elements;

	for ( id = 0; id < arr->size; id++ ) {
		if ( dynArrayIsFreeElement( arr, id ) ) {
			break;
		}
		*elementP += arr->elementSize;
	}
	/* Important! After realloc our elements live in new address so we need to reassing our pointer. */
	if ( checkArrayRealloc( arr, id ) ) {
		*elementP = arr->elements + ( arr->size - 1 ) * arr->elementSize;
	}

	return id;
}

/* Returns element handle. */
void* dynArrayGetElement( DynArray* arr, int id ) {
	return arr->elements + id * arr->elementSize;
}

/* Note! Call only for components. Element need to have ComHeader. */
int dynArrayGetElementEntityId( DynArray* arr, int id ) {
	ComHeader* headerP = (ComHeader*)dynArrayGetElement( arr, id );
	return headerP->entityId;
}

bool dynArrayIsFreeElement( DynArray* arr, int id ) {
	if ( arr->isFreeCallback != NULL ) {
		return arr->isFreeCallback( dynArrayGetElement( arr, id ) );
	}
}
/* Pass init bool variable so that it is known when free element so we can also free the possible memory it has. */
void dynArrayFreeElement( DynArray* arr, int id, bool init ) {
	if ( arr->freeCallback != NULL ) {
		arr->freeCallback( dynArrayGetElement( arr, id ), init );
	}
}

/* Free all components with entity id. Note! Call only for components. */
void dynArrayFreeByEntityId( DynArray* arr, int id ) {
	for ( int i = 0; i < arr->size; i++ ) {
		if ( dynArrayGetElementEntityId( arr, i ) == id ) {
			dynArrayFreeElement( arr, i, false );
		}
	}
}

void dynArrayInit( DynArray* arr, size_t elementSize,
	void (*freeCallback)( void*, bool ), bool (*isFreeCallback)( void* ) )
{
	arr->size = 0;
	arr->capacity = ALLOC_PAGE_SIZE;
	arr->elementSize = elementSize;
	arr->elements = malloc( arr->capacity * arr->elementSize );
	arr->freeCallback = freeCallback;
	arr->isFreeCallback = isFreeCallback;

	for ( int id = 0; id < arr->capacity; id++ ) {
		dynArrayFreeElement( arr, id, true );
	}
}

void dynArrayClear( DynArray* arr ) {
	for ( int id = 0; id < arr->size; id++ ) {
		dynArrayFreeElement( arr, id, false );
	}
	arr->size = 0;
}

void dynArrayFree( DynArray* arr ) {
	if ( 0 < arr->capacity ) {
		dynArrayClear( arr );
		free( arr->elements );
	}
}
