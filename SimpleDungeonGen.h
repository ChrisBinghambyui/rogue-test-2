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

    // finds the center of the room, used for hallways
    int centerX() const { return x + width / 2; }
    int centerY() const { return y + height / 2; }

    // checks if rooms are overlapping
    bool intersects(const Room& other) const
    {
        return (x <= other.x + other.width && x + width >= other.x &&
            y <= other.y + other.height && y + height >= other.y);
    }
};

inline void BuildDungeonLayout(int width, int height, int maxRooms, std::vector<TileType>& outTiles, std::vector<Room>& outRooms)
{
    // fills the whole map with walls first
    outTiles.assign(width * height, TILE_WALL);
    outRooms.clear();

    // limits to 300 tries so the game doesnt freeze forever
    for (int attempts = 0; attempts < 300 && outRooms.size() < maxRooms; attempts++)
    {
        // change these numbers to make rooms wider or thinner
        int roomW = rand() % 9 + 8;
        // change these numbers to make rooms taller or shorter
        int roomH = rand() % 7 + 7;

        // picks a random spot that fits on the map
        int roomX = rand() % (width - roomW - 3) + 2;
        int roomY = rand() % (height - roomH - 3) + 2;

        Room candidate = { roomX, roomY, roomW, roomH };
        bool blocked = false;

        // padding for rooms. makes sure they're not too close and don't overlap.
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

    // connects all the rooms together with hallways
    for (size_t i = 1; i < outRooms.size(); i++)
    {
        int cx = outRooms[i].centerX();
        int cy = outRooms[i].centerY();
        int targetX = outRooms[i - 1].centerX();
        int targetY = outRooms[i - 1].centerY();

        // digs a hallway left or right until aligned
        while (cx != targetX)
        {
            outTiles[cx * height + cy] = TILE_FLOOR;
            cx += (targetX > cx) ? 1 : -1;
        }

        // digs a hallway up or down to finish the connection
        while (cy != targetY)
        {
            outTiles[cx * height + cy] = TILE_FLOOR;
            cy += (targetY > cy) ? 1 : -1;
        }
    }
}