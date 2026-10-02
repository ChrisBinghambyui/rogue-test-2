#pragma once
#include <vector>
#include <cstdlib>

enum TileType
{
    TILE_WALL,
    TILE_FLOOR
};

struct Room
{
    int x, y, width, height;

    int centerX() const { return x + width / 2; }
    int centerY() const { return y + height / 2; }

    bool intersects(const Room& other) const
    {
        return (x <= other.x + other.width && x + width >= other.x &&
            y <= other.y + other.height && y + height >= other.y);
    }
};

inline void BuildDungeonLayout(int width, int height, int maxRooms, std::vector<TileType>& outTiles, std::vector<Room>& outRooms)
{
    outTiles.assign(width * height, TILE_WALL);
    outRooms.clear();

    for (int attempts = 0; attempts < 300 && outRooms.size() < maxRooms; attempts++)
    {
        int roomW = rand() % 9 + 8;
        int roomH = rand() % 7 + 7;
        int roomX = rand() % (width - roomW - 3) + 2;
        int roomY = rand() % (height - roomH - 3) + 2;

        Room candidate = { roomX, roomY, roomW, roomH };
        bool blocked = false;

        for (const auto& room : outRooms)
        {
            Room padded = { room.x - 3, room.y - 3, room.width + 6, room.height + 6 };
            if (candidate.intersects(padded))
            {
                blocked = true;
                break;
            }
        }

        if (blocked)
        {
            continue;
        }

        for (int x = candidate.x; x < candidate.x + candidate.width; x++)
        {
            for (int y = candidate.y; y < candidate.y + candidate.height; y++)
            {
                outTiles[x * height + y] = TILE_FLOOR;
            }
        }
        outRooms.push_back(candidate);
    }

    for (size_t i = 1; i < outRooms.size(); i++)
    {
        int cx = outRooms[i].centerX();
        int cy = outRooms[i].centerY();
        int targetX = outRooms[i - 1].centerX();
        int targetY = outRooms[i - 1].centerY();

        while (cx != targetX)
        {
            outTiles[cx * height + cy] = TILE_FLOOR;
            cx += (targetX > cx) ? 1 : -1;
        }
        while (cy != targetY)
        {
            outTiles[cx * height + cy] = TILE_FLOOR;
            cy += (targetY > cy) ? 1 : -1;
        }
    }
}