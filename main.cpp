#include "raylib.h"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;
struct HexCell
{
    Vector2 center;
    bool isLand;
    Color fill;
};
struct Cities
{
    const char* name;
    int x;
    int y;
};

Cities cities[] {
    {"London", 286, 269},
    {"Paris", 343, 322},
    {"Bruges", 359, 288},
    {"Ghent", 366, 294},
    {"Amsterdam", 389, 276},
    {"Cologne", 420, 289},
    {"Hamburg", 455, 242},
    {"Lubeck", 500, 228},
    {"Lisbon", 257, 441},
    {"Seville", 302, 463},
    {"Barcelona", 403, 426},
    {"Marseille", 461, 386},
    {"Genoa", 501, 376},
    {"Milan", 515, 349},
    {"Florence", 542, 386},
    {"Venice", 584, 368},
    {"Naples", 578, 442},
    {"Vienna", 635, 318},
    {"Prague", 592, 290},
    {"Constantinople", 845, 430}

    
}; 

static void DrawHexCell(Vector2 center, float radius, Color fill, Color outline)
{
    DrawPoly(center, 6, radius, 30.0f, fill);
    DrawPolyLinesEx(center, 6, radius, 30.0f, 1.0f, outline);
}

static bool IsWaterSample(Color color)
{
    return color.b > color.g + 12 && color.b > color.r + 18;
}

static Color ClampLandColor(Color color)
{
    if (IsWaterSample(color))
    {
        return (Color){52, 163, 235, 255};
    }

    if (color.r > 150 && color.g > 140)
    {
        return (Color){201, 188, 104, 255};
    }

    if (color.g > 95)
    {
        return (Color){58, 148, 42, 255};
    }

    return (Color){112, 132, 108, 255};
}

static bool IsLandAt(const Image &image, int x, int y)
{
    int landVotes = 0;
    int waterVotes = 0;

    const int offsets[5][2] = {
        {0, 0},
        {-4, 0},
        {4, 0},
        {0, -4},
        {0, 4}
    };

    for (int i = 0; i < 5; i++)
    {
        int sampleX = x + offsets[i][0];
        int sampleY = y + offsets[i][1];

        if (sampleX < 0 || sampleY < 0 || sampleX >= image.width || sampleY >= image.height)
        {
            waterVotes++;
            continue;
        }

        Color sample = GetImageColor(image, sampleX, sampleY);
        if (IsWaterSample(sample))
        {
            waterVotes++;
        }
        else
        {
            landVotes++;
        }
    }

    return landVotes > waterVotes;
}

int main()
{
    const int screenWidth = 980;
    const int screenHeight = 547;
    const float hexRadius = 2.0f;
    const float hexWidth = sqrtf(3.0f) * hexRadius;
    const float rowStep = hexRadius * 1.5f;
    const float mapOffsetX = 7.0f;
    const float mapOffsetY = 7.0f;
    const int mapCols = 678;
    const int mapRows = 510;

    InitWindow(screenWidth, screenHeight, "Europe Tile Map");

    Image mapImage = LoadImage("europe_map.png");
    std::vector<HexCell> cells;
    cells.reserve(mapCols * mapRows);

    for (int row = 0; row < mapRows; row++)
    {
        for (int col = 0; col < mapCols; col++)
        {
            float x = mapOffsetX + col * hexWidth + ((row % 2) ? hexWidth * 0.5f : 0.0f);
            float y = mapOffsetY + row * rowStep;

            if (x < 0 || y < 0 || x >= screenWidth || y >= screenHeight)
            {
                continue;
            }

            Color sample = GetImageColor(mapImage, (int)x, (int)y);
            bool isLand = IsLandAt(mapImage, (int)x, (int)y);
            Color fill = isLand ? ClampLandColor(sample) : (Color){52, 152, 226, 255};

            cells.push_back({{x, y}, isLand, fill});
        }
    }

    UnloadImage(mapImage);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        
        BeginDrawing();
        ClearBackground((Color){44, 130, 204, 255});
        ClearBackground(RAYWHITE);
        
        for (const HexCell &cell : cells)
        {
            Color outline = cell.isLand ? (Color){18, 48, 14, 255} : (Color){13, 58, 103, 255};
            DrawHexCell(cell.center, hexRadius, cell.fill, outline);
        }
        int cityCount = sizeof(cities) / sizeof(cities[0]);

        for(int i = 0; i < cityCount; i++){
            cout << cities[i].x << endl;
            DrawRectangle(cities[i].x, cities[i].y, 10 ,10, RED);

        }
        DrawRectangle(500, 50, 10, 10, BLACK);
        Vector2 mousePosition = GetMousePosition();
        string positionText = "Mouse: (" + std::to_string((int)mousePosition.x) + ", " + std::to_string((int)mousePosition.y) + ")";
        DrawText(positionText.c_str(), 50, 40, 40, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
