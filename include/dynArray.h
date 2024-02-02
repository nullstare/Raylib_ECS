#pragma once

typedef struct {
	int size;
	int capacity;
	size_t elementSize;
	void* elements;
	void (*freeCallback)( void*, bool ); /* Components provide callback to free themselves. */
	bool (*isFreeCallback)( void* ); /* Components provide callback to determine when free. */
} DynArray;

int dynArrayAddElement( DynArray* arr, void** elementP );
void* dynArrayGetElement( DynArray* arr, int id );
int dynArrayGetElementEntityId( DynArray* arr, int id );
bool dynArrayIsFreeElement( DynArray* arr, int id );
void dynArrayFreeElement( DynArray* arr, int id, bool init );
void dynArrayFreeByEntityId( DynArray* arr, int id );
void dynArrayInit( DynArray* arr, size_t elementSize,
	void (*freeCallback)( void*, bool ), bool (*isFreeCallback)( void* )
);
void dynArrayClear( DynArray* arr );
void dynArrayFree( DynArray* arr );
