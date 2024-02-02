#include "main.h"
#include "resources.h"

Resources* resources;

/* Dynamic array callbacks. */

void textureFree( void* elementP, bool init ) {
	TextureR* textureR = (TextureR*)elementP;

	if ( !init ) {
		free( textureR->header.name );
		UnloadTexture( textureR->texture );
	}
	textureR->header = (ResHeader){ .id = -1, .name = NULL, .type = RES_TYPE_NULL };
}

void animationFree( void* elementP, bool init ) {
	AnimationR* animationR = (AnimationR*)elementP;

	if ( !init ) {
		free( animationR->header.name );
		free( animationR->animation.source );
	}
	animationR->header = (ResHeader){ .id = -1, .name = NULL, .type = RES_TYPE_NULL };
	animationR->animation.frameCount = 0;
}

bool resDefaultIsFree( void* elementP ) {
	ResHeader* resHeader = (ResHeader*)elementP;
	return resHeader->id < 0;
}

/* Resources functions. */

void resourcesInit() {
	resources = malloc( sizeof( Resources ) );

	dynArrayInit( &resources->assets[ RES_TYPE_TEXTURE ], sizeof( TextureR ), textureFree, resDefaultIsFree );
	dynArrayInit( &resources->assets[ RES_TYPE_ANIMATION ], sizeof( AnimationR ), animationFree, resDefaultIsFree );
}

void resourcesFree() {
	for ( int i = 0; i < RES_TYPE_COUNT; i++ ) {
		dynArrayFree( &resources->assets[i] );
	}
	free( resources );
	TraceLog( LOG_INFO, "Resources Freed" );
}

void resourcesLoadTexture( const char* path ) {
	TextureR* textureR = NULL;
	int id = dynArrayAddElement( &resources->assets[ RES_TYPE_TEXTURE ], (void*)&textureR );

	textureR->header = (ResHeader){ .id = id, .type = RES_TYPE_TEXTURE };
	const char* fileName = GetFileNameWithoutExt( path );
	textureR->header.name = malloc( ( TextLength( fileName ) + 1 ) * sizeof( char ) );
	TextCopy( textureR->header.name, fileName );
	textureR->texture = LoadTexture( path );
}

void resourcesLoadAnimation( const char* name, int frameCount, Rectangle* source ) {
	AnimationR* animationR = NULL;
	int id = dynArrayAddElement( &resources->assets[ RES_TYPE_ANIMATION ], (void*)&animationR );

	animationR->header = (ResHeader){ .id = id, .type = RES_TYPE_ANIMATION };
	animationR->header.name = malloc( ( TextLength( name ) + 1 ) * sizeof( char ) );
	TextCopy( animationR->header.name, name );
	animationR->animation.frameCount = frameCount;
	animationR->animation.source = malloc( animationR->animation.frameCount * sizeof( Rectangle ) );

	for ( int i = 0; i < animationR->animation.frameCount; i++ ) {
		animationR->animation.source[i] = source[i];
	}
}

/* Return NULL if not found. */
Texture* resourcesGetTextureByName( const char* name ) {
	for ( int i = 0; i < resources->assets[ RES_TYPE_TEXTURE ].size; i++ ) {
		TextureR* textureR = (TextureR*)dynArrayGetElement( &resources->assets[ RES_TYPE_TEXTURE ], i );

		if ( TextIsEqual( textureR->header.name, name ) ) {
			return &textureR->texture;
		}
	}
	return NULL;
}

/* Return NULL if not found. */
SpriteAnimation* resourcesGetAnimationByName( const char* name ) {
	for ( int i = 0; i < resources->assets[ RES_TYPE_ANIMATION ].size; i++ ) {
		AnimationR* animationR = (AnimationR*)dynArrayGetElement( &resources->assets[ RES_TYPE_ANIMATION ], i );

		if ( TextIsEqual( animationR->header.name, name ) ) {
			return &animationR->animation;
		}
	}
	return NULL;
}
