#include <raylib.h>
#include <vector>
#include "SimpleDungeonGen.h"

// change these to make the map bigger or add more rooms
const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 70;
const int MAX_ROOMS = 15;

int main()
{
    // window size
    InitWindow(1280, 720, "Dungeon Generation");
    SetTargetFPS(60);

    std::vector<TileType> layout;
    std::vector<Room> rooms;

    auto generate = [&]() {
        BuildDungeonLayout(MAP_WIDTH, MAP_HEIGHT, MAX_ROOMS, layout, rooms);
        };

    generate();

    // camera
    Camera3D camera3d = { 0 };
    camera3d.position = { MAP_WIDTH / 2.0f + 15.0f, 20.0f, MAP_HEIGHT / 2.0f + 15.0f };
    camera3d.target = { MAP_WIDTH / 2.0f, 0.0f, MAP_HEIGHT / 2.0f };
    camera3d.up = { 0.0f, 1.0f, 0.0f };
    // change to CAMERA_PERSPECTIVE for standard 3d
    camera3d.projection = CAMERA_ORTHOGRAPHIC;
    camera3d.fovy = 60.0f;

    while (!WindowShouldClose())
    {
        // press n to generate a new map
        if (IsKeyPressed(KEY_N))
        {
            generate();
        }

        // adjust camera panning speed
        float panSpeed = 1.0f;

        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) camera3d.target.x += panSpeed;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) camera3d.target.x -= panSpeed;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) camera3d.target.z += panSpeed;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) camera3d.target.z -= panSpeed;
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

                    // change numbers (red, green, blue, transparency) to edit tile color
                    DrawCube(pos, 1.0f, 1.0f, 1.0f, Color{ 92, 84, 96, 255 });
                    // change to edit tile outline color
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