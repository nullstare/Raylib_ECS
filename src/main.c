#include "main.h"
#include "resources.h"
#include "entityManager.h"
#include "player.h"
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
	transformNew( player, (Vector2){ 200, 128 }, (Vector2){ 1.0, 1.0 }, 0.0 );
	hitboxNew( player, (Rectangle){ -16, -16, 32, 32 } );
	spriteNew( player, (Vector2){ -16, -16 }, resGetTexture( "player" ), resGetAnimation( "idle" ), BLUE );

	Entity* enemy = entityNew();
	hitboxNew( enemy, (Rectangle){ 128, 80, 32, 32 } );
}

int main() {
	Vector2 windowSize = (Vector2){ 1024, 720 };

	InitWindow( windowSize.x, windowSize.y, "ECS");
	centerWindow( windowSize, 0 );
	SetTargetFPS( 60 );

	entityManagerInit();
	resInit();
	resLoadTexture( "../images/player.png" );
	resLoadTexture( "../images/enemy.png" );

	Rectangle idle[1] = { { 0.0, 0.0, 32.0, 32.0 } };
	resLoadAnimation( "idle", 1, idle );
	Rectangle angry[2] = { { 0.0, 0.0, 32.0, 32.0 }, { 32.0, 0.0, 32.0, 32.0 } };
	resLoadAnimation( "angry", 2, angry );

	addPlayer( (Vector2){ 20, 96 } );

	while ( !WindowShouldClose() ) {
		behaviorProcess();
		hitboxProcess();

		BeginDrawing();
			ClearBackground( RAYWHITE );

			if ( IsKeyPressed( KEY_ONE ) ) {
				entityManagerFreeEntity( 0 );
			}
			if ( IsKeyPressed( KEY_TWO ) ) {
				entityManagerFreeEntity( 1 );
			}
			spriteDraw();
			transformDraw();
			hitboxDraw();
		EndDrawing();
	}
	entityManagerFree();
	resFree();
	CloseWindow();

	return 1;
}
