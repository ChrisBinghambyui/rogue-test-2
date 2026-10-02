#include <raylib.h>
#include <vector>
#include "SimpleDungeonGen.h"

const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 70;
const int MAX_ROOMS = 15;

int main()
{
    InitWindow(1280, 720, "Dungeon Generation");
    SetTargetFPS(60);

    std::vector<TileType> layout;
    std::vector<Room> rooms;

    auto generate = [&]() {
        BuildDungeonLayout(MAP_WIDTH, MAP_HEIGHT, MAX_ROOMS, layout, rooms);
        };

    generate();

    Camera3D camera3d = { 0 };
    camera3d.position = { MAP_WIDTH / 2.0f + 15.0f, 20.0f, MAP_HEIGHT / 2.0f + 15.0f };
    camera3d.target = { MAP_WIDTH / 2.0f, 0.0f, MAP_HEIGHT / 2.0f };
    camera3d.up = { 0.0f, 1.0f, 0.0f };
    camera3d.projection = CAMERA_ORTHOGRAPHIC;
    camera3d.fovy = 60.0f;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_N))
        {
            generate();
        }

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) camera3d.target.x += 1.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) camera3d.target.x -= 1.0f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) camera3d.target.z += 1.0f;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) camera3d.target.z -= 1.0f;
        camera3d.position.x = camera3d.target.x + 15.0f;
        camera3d.position.z = camera3d.target.z + 15.0f;

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera3d);

        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                TileType tile = layout[x * MAP_HEIGHT + y];
                Vector3 pos = { x + 0.5f, 0.0f, y + 0.5f };

                if (tile == TILE_WALL)
                {
                    pos.y = 0.5f;
                    DrawCube(pos, 1.0f, 1.0f, 1.0f, Color{ 92, 84, 96, 255 });
                    DrawCubeWires(pos, 1.0f, 1.0f, 1.0f, Color{ 40, 34, 44, 255 });
                }
            }
        }

        EndMode3D();
        DrawText("Press 'N' to generate  |  WASD to pan", 10, 10, 20, GOLD);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}