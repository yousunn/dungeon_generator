#include <iostream>
#include <random>
#include <tuple>
#include <vector>

class DungeonGenerator
{
public:
    enum class Tile
    {
        EMPTY = 0,
        HALL = 1,
        ROOM = 2,
        END = 3,
        START = 4,
    };

    DungeonGenerator(unsigned int dungeonSize)
    {
        m_dungeonSize = dungeonSize;
        m_dungeon = std::vector<std::vector<Tile>>(dungeonSize, std::vector<Tile>(dungeonSize, Tile::EMPTY));
        std::random_device rd;
        m_rng = std::mt19937(rd());
        m_minHallways = m_dungeonSize / 2;
    }

    // generates a 2D dungeon with a START tile that is lead to an END tile by hallways and rooms
    void generate()
    {
        auto nextTile = Tile::START;

        //generate start location
        std::uniform_int_distribution<int> indexDist(0, m_dungeonSize - 1);
        int currentX = indexDist(m_rng);
        int currentY = indexDist(m_rng);

        int nextX;
        int nextY;

        m_dungeon[currentX][currentY] = nextTile;

        // generate tiles until END tile is encountered
        while (true)
        {
            auto nextLocation = generateTile(currentX, currentY);
            nextX = std::get<0>(nextLocation);
            nextY = std::get<1>(nextLocation);
            nextTile = m_dungeon[nextX][nextY];

            switch (nextTile)
            {
            case Tile::ROOM:
            {
                // rooms shouldn't have more than one connection, don't update current location
                break;
            }
            case Tile::END:
            {
                // we're done generating
                return;
            }
            case Tile::HALL:
            {
                // hallways can generate more rooms off of it, so update current location
                currentX = nextX;
                currentY = nextY;
                m_hallwayCount++;
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }

    void print()
    {
        for (auto row : m_dungeon)
        {
            for (auto tile : row)
            {
                char tileChar = '*';
                switch (tile)
                {
                case Tile::EMPTY:
                {
                    break;
                }
                case Tile::HALL:
                {
                    tileChar = 'H';
                    break;
                }
                case Tile::ROOM:
                {
                    tileChar = 'R';
                    break;
                }
                case Tile::END:
                {
                    tileChar = 'E';
                    break;
                }
                case Tile::START:
                {
                    tileChar = 'S';
                    break;
                }
                default:
                {
                    break;
                }
                }
                std::cout << tileChar << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    // places a random tile at a random point to the given point
    // returns the point where the tile was placed
    std::tuple<int, int> generateTile(int& currentX, int& currentY)
    {
        if (isForcedEnd(currentX, currentY))
        {
            m_dungeon[currentX][currentY] = Tile::END;
            return std::tuple<int, int>(currentX, currentY);
        }

        int nextX;
        int nextY;
        bool isValidTile = false;
        // find a valid place to put a tile
        while (!isValidTile)
        {
            nextX = currentX;
            nextY = currentY;

            //generate directional number
            std::uniform_int_distribution<int> directionDist(1, 4);
            auto direction = directionDist(m_rng);

            switch (direction)
            {
            case 1: //north
            {
                nextY--;
                break;
            }
            case 2: // south
            {
                nextY++;
                break;
            }
            case 3: // west
            {
                nextX--;
                break;
            }
            case 4: // east
            {
                nextX++;
                break;
            }
            default:
            {
                break;
            }
            }

            isValidTile = canPlaceTile(nextX, nextY);
        }

        //generate tile type, can't be END if we don't have minimum amount of HALL's
        std::uniform_int_distribution<int> tileDist(1, 100);
        auto tileSelector = tileDist(m_rng);
        Tile selectedTile;
        if (m_hallwayCount < m_minHallways)
        {
            if (tileSelector < ROOM_MAX_RAND_NO_END) // %30
            {
                selectedTile = Tile::ROOM;
            }
            else // %70
            {
                selectedTile = Tile::HALL;
            }
        }
        else
        {
            if (tileSelector < END_MAX_RAND) // %10
            {
                selectedTile = Tile::END;
            }
            else if (tileSelector >= END_MAX_RAND && tileSelector < ROOM_MAX_RAND) // %30
            {
                selectedTile = Tile::ROOM;
            }
            else // %60
            {
                selectedTile = Tile::HALL;
            }
        }
        //place tile
        m_dungeon[nextX][nextY] = selectedTile;

        return std::tuple<int, int>(nextX, nextY);
    }

    // returns true if each adjacent tile to the given point is either non-existant
    // or is not EMPTY
    bool isForcedEnd(int x, int y)
    {
        if (!canPlaceTile(x, y - 1) &&
            !canPlaceTile(x, y + 1) &&
            !canPlaceTile(x - 1, y) &&
            !canPlaceTile(x + 1, y))
        {
            return true;
        }
        return false;
    }

    // returns true if the given point is within the boundaries of the dungeon and
    // is an EMPTY tile
    bool canPlaceTile(int x, int y)
    {
        if (x < 0 || x >= m_dungeonSize || y < 0 || y >= m_dungeonSize)
        {
            return false;
        }
        if (m_dungeon[x][y] != Tile::EMPTY)
        {
            return false;
        }
        return true;
    }

    const int END_MAX_RAND = 10;
    const int ROOM_MAX_RAND_NO_END = 30;
    const int ROOM_MAX_RAND = 40;

    unsigned int m_dungeonSize;
    std::vector<std::vector<Tile>> m_dungeon;
    std::mt19937 m_rng;
    unsigned int m_minHallways;
    unsigned int m_hallwayCount = 0;
};

int main()
{
    unsigned int size;
    std::cout << "Enter Dungeon Grid Square Length" << std::endl;
    std::cin >> size;
    DungeonGenerator gen(size);
    gen.generate();
    gen.print();
    return 0;
}
