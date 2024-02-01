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
		int iters = 0;

		for ( id = arr->size; id < arr->capacity; id++ ) {
			dynArrayFreeElement( arr, id );
			iters++;
		}
		return true;
	}
	return false;
}

/* elementP will be set to free element slot. */
int dynArrayAddElement( DynArray* arr, void** elementP ) {
	int id = 0;
	*elementP = arr->elements;

	for ( id = 0; id < arr->size; id++ ) {
		if ( dynArrayIsFreeElement( arr, id ) ) {
			break;
		}
		*elementP += arr->elementSize;
	}
	/* Important!! After realloc our elements live in new address so we need to reassing our pointer. */
	if ( checkArrayRealloc( arr, id ) ) {
		*elementP = arr->elements + ( arr->size - 1 ) * arr->elementSize;
	}

	return id;
}

void* dynArrayGetElement( DynArray* arr, int id ) {
	void* elementP = arr->elements;
	return elementP + id * arr->elementSize;
}

int dynArrayGetElementByEntityId( DynArray* arr, int id ) {
	void* elementP = dynArrayGetElement( arr, id );
	ComHeader* headerP = (ComHeader*)elementP;
	return headerP->entityId;
}

bool dynArrayIsFreeElement( DynArray* arr, int id ) {
	if ( arr->isFreeCallback != NULL ) {
		return arr->isFreeCallback( dynArrayGetElement( arr, id ) );
	}
}

void dynArrayFreeElement( DynArray* arr, int id ) {
	if ( arr->freeCallback != NULL ) {
		arr->freeCallback( dynArrayGetElement( arr, id ) );
	}
}

/* Free all components with entity id. */
void dynArrayFreeByEntityId( DynArray* arr, int id ) {
	for ( int i = 0; i < arr->size; i++ ) {
		void* elementP = dynArrayGetElement( arr, i );

		if ( dynArrayGetElementByEntityId( arr, i ) == id ) {
			dynArrayFreeElement( arr, i );
		}
	}
}

void dynArrayInit( DynArray* arr, size_t elementSize, void (*freeCallback)( void* ), bool (*isFreeCallback)( void* ) ) {
	arr->size = 0;
	arr->capacity = ALLOC_PAGE_SIZE;
	arr->elementSize = elementSize;
	arr->elements = malloc( arr->capacity * arr->elementSize );
	arr->freeCallback = freeCallback;
	arr->isFreeCallback = isFreeCallback;

	dynArrayClear( arr );
}

void dynArrayClear( DynArray* arr ) {
	void* elementP = arr->elements;

	for ( int id = 0; id < arr->capacity; id++ ) {
		dynArrayFreeElement( arr, id );
		elementP += arr->elementSize;
	}
	arr->size = 0;
}

void dynArrayFree( DynArray* arr ) {
	if ( 0 < arr->capacity ) {
		free( arr->elements );
	}
}
