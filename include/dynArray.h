#pragma once

#define ALLOC_PAGE_SIZE 32

typedef struct {
	int size;
	int capacity;
	size_t elementSize;
	void* elements;
	void (*freeCallback)( void* );
	bool (*isFreeCallback)( void* );
} DynArray;

int dynArrayAddElement( DynArray* arr, void** elementP );
void* dynArrayGetElement( DynArray* arr, int id );
int dynArrayGetElementByEntityId( DynArray* arr, int id );
bool dynArrayIsFreeElement( DynArray* arr, int id );
void dynArrayFreeElement( DynArray* arr, int id );
void dynArrayFreeByEntityId( DynArray* arr, int id );
void dynArrayInit( DynArray* arr, size_t elementSize, void (*freeCallback)( void* ),bool (*isFreeCallback)( void* ) );
void dynArrayFree( DynArray* arr );
