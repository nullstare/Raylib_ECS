#pragma once

#include "dynArray.h"

enum ResourceTypes{
	RES_TYPE_NULL = -1,
	RES_TYPE_TEXTURE = 0,
	RES_TYPE_ANIMATION
};

#define RES_TYPE_COUNT RES_TYPE_ANIMATION + 1 /* We don't count RES_TYPE_NULL here. */

/* Resource header. Must always be the first member of resource struct. */
typedef struct {
	int id;
	int type;
	char* name;
} ResHeader;

typedef struct {
	int frameCount;
	Rectangle* source;
} SpriteAnimation;

/* Resources have suffix R. */
typedef struct {
	ResHeader header;
	Texture texture;
} TextureR;

typedef struct {
	ResHeader header;
	SpriteAnimation animation;
} AnimationR;

typedef struct {
	DynArray assets[ RES_TYPE_COUNT ];
} Resources;

extern Resources* resources;

/* Dynamic array callbacks. */
void textureFree( void* elementPbool, bool init );
void animationFree( void* elementP, bool init );
bool resDefaultIsFree( void* elementP );
/* Resources functions. */
void resourcesInit();
void resourcesFree();
void resourcesLoadTexture( const char* path );
void resourcesLoadAnimation( const char* name, int frameCount, Rectangle* source );
Texture* resourcesGetTextureByName( const char* name );
SpriteAnimation* resourcesGetAnimationByName( const char* name );
