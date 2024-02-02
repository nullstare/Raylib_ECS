#include "main.h"
#include "resources.h"
#include "entityManager.h"
#include "player.h"
#include "enemy.h"
#include "behavior.h"
#include "hitbox.h"
#include "transform.h"
#include "sprite.h"

static void centerWindow( Vector2 windowSize, int monitor ) {
	Vector2 monitorSize = (Vector2){ GetMonitorWidth( monitor ), GetMonitorHeight( monitor ) };
	Vector2 monitorPos = GetMonitorPosition( monitor );

	SetWindowPosition(
		monitorPos.x + monitorSize.x / 2 - windowSize.x / 2,
		monitorPos.y + monitorSize.y / 2 - windowSize.y / 2
	);
}

static void addPlayer( Vector2 pos ) {
	Entity* player = entityNew();
	behaviorNew( player, playerProcess );
	transformNew( player, pos, (Vector2){ 1.0, 1.0 }, 0.0 );
	hitboxNew( player, (Rectangle){ -16, -16, 32, 32 }, playerCollision );
	spriteNew( player, (Vector2){ -16, -16 },
		resourcesGetTextureByName( "player" ),
		resourcesGetAnimationByName( "idle" ),
		BLUE, 1.0
	);
}

static void addEnemy( Vector2 pos ) {
	Entity* enemy = entityNew();
	// behaviorNew( enemy, enemyProcess );
	transformNew( enemy, pos, (Vector2){ 1.0, 1.0 }, 0.0 );
	hitboxNew( enemy, (Rectangle){ -16, -16, 32, 32 }, NULL );
	spriteNew( enemy, (Vector2){ -16, -16 },
		resourcesGetTextureByName( "enemy" ),
		resourcesGetAnimationByName( "angry" ),
		RED, (float)rand() / (float)RAND_MAX
	);
}

static void addEnemies() {
	// addEnemy( (Vector2){ 96, 200 } );

	for ( int x = 0; x < 20; x++ ) {
		addEnemy( (Vector2){ 96 + x * 40, 200 } );
	}

	// for ( int x = 0; x < 32; x++ ) {
	// 	for ( int y = 0; y < 22; y++ ) {
	// 		addEnemy( (Vector2){ 16 + x * 32, 16 + y * 32 } );
	// 	}
	// }

	// for ( int x = 0; x < 100; x++ ) {
	// 	for ( int y = 0; y < 100; y++ ) {
	// 		addEnemy( (Vector2){ 16 + x * 10, 16 + y * 7 } );
	// 	}
	// }
}

int main() {
	/* Window setup. */
	Vector2 windowSize = (Vector2){ 1024, 720 };
	int monitor = 0;

	InitWindow( windowSize.x, windowSize.y, "ECS" );
	centerWindow( windowSize, monitor );
	SetWindowState( FLAG_VSYNC_HINT );

	/* Initialice systems. */
	entityManagerInit();
	resourcesInit();

	/* Load resources. */
	resourcesLoadTexture( "../images/player.png" );
	resourcesLoadTexture( "../images/enemy.png" );

	Rectangle idle[1] = { { 0.0, 0.0, 32.0, 32.0 } };
	resourcesLoadAnimation( "idle", 1, idle );
	Rectangle angry[2] = { { 0.0, 0.0, 32.0, 32.0 }, { 32.0, 0.0, 32.0, 32.0 } };
	resourcesLoadAnimation( "angry", 2, angry );

	/* Create entities. */
	addPlayer( (Vector2){ 200, 96 } );
	addEnemies();

	while ( !WindowShouldClose() ) {
		/* Process components. */
		behaviorProcess();
		hitboxProcess();
		spriteProcess();

		BeginDrawing();
			ClearBackground( RAYWHITE );
			/* Draw components. */
			spriteDraw();
			// transformDraw();
			// hitboxDraw();

			/* Draw info. */
			DrawRectangle( 16, 16, 200, 56, (Color){ 0, 0, 0, 200 } );
			DrawText( TextFormat( "Entity count %d", entityManager->entities.size ), 20, 46, 20, GREEN );
			DrawFPS( 20, 20 );
		EndDrawing();
	}
	/* Free systems. */
	entityManagerFree();
	resourcesFree();

	CloseWindow();

	return 0;
}
