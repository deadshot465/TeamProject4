// TeamProject5.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <raylib.h>

int main()
{
    InitWindow(1024, 768, "Hello");

    SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("FUCK OFF!!!", 190, 200, 20, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 380, 300, 30, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 570, 400, 40, LIGHTGRAY);
		DrawText("FUCK OFF!!!", 760, 500, 50, LIGHTGRAY);

		RED

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
