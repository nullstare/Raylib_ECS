#include "main.h"
#include "dynArray.h"
#include "entityManager.h"

static void checkArrayRealloc( DynArray* arr, int id ) {
	if ( id == arr->size ) {
		arr->size++;
	}
	if ( arr->size == arr->capacity ) {
		arr->capacity += ALLOC_PAGE_SIZE;
		arr->elements = realloc( arr->elements, arr->capacity * arr->elementSize );

		void* elementP = arr->elements + id * arr->elementSize;

		for ( id = arr->size; id < arr->capacity; id++ ) {
			dynArrayFreeElement( arr, id );
			elementP += arr->elementSize;
		}
	}
}

int dynArrayAddElement( DynArray* arr, void** elementP ) {
	int id = 0;
	*elementP = arr->elements;

	for ( id = 0; id < arr->size; id++ ) {
		if ( dynArrayIsFreeElement( arr, id ) ) {
			break;
		}
		*elementP += arr->elementSize;
	}
	checkArrayRealloc( arr, id );

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
	return arr->isFreeCallback( dynArrayGetElement( arr, id ) );
}

void dynArrayFreeElement( DynArray* arr, int id ) {
	arr->freeCallback( dynArrayGetElement( arr, id ) );
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

	void* elementP = arr->elements;

	for ( int id = arr->size; id < arr->capacity; id++ ) {
		dynArrayFreeElement( arr, id );
		elementP += arr->elementSize;
	}
}

void dynArrayFree( DynArray* arr ) {
	free( arr->elements );
}
