#include <raylib.h>
#include "../assets/Sprites/characterSprite.c"
#include <iostream>
#include <math.h>
#include <raymath.h>

const int windowHeight = 800;
const int windowWidth = 800;
const int frameRate = 60;
int cellSize = 20;
const int tileSize = 32;
const int worldWidth = 100;
const int worldHeight = 100;

using namespace std;

enum class Terrain {
    GRASS, 
    DIRT,
    STONE,
    WATER,
    MUD
};

class World {
    public: 
        int width;
        int height;
        int tileSize;
        int number;
        Color tileColor;

        Terrain tiles[100][100];

        void Generate() {
            for(int i = 0; i < worldHeight; i++) {
                for (int j = 0; j < worldWidth; j++) {
                    number = GetRandomValue(1, 100);

                    if(number <= 60) {
                        tiles[i][j] = Terrain::GRASS;
                    } else if(number > 60 && number <=80) {
                        tiles[i][j] = Terrain::DIRT;
                    } else if(number > 80 && number <=90) {
                        tiles[i][j] = Terrain::STONE;
                    } else if(number > 90 && number <=95) {
                        tiles[i][j] = Terrain::STONE;
                    } else if(number > 95 && number <=100) {
                        tiles[i][j] = Terrain::MUD;
                    };
                }
            }    
        }

        void Draw() {
            for(int x = 0; x < 100; x++) {
                for(int y = 0; y < 100; y++) {
                    switch (tiles[y][x]) {
                        case Terrain::GRASS:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, GREEN);
                            break;

                        case Terrain::STONE:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, GRAY);
                            break;

                        case Terrain::DIRT:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, BROWN);
                            break;

                        case Terrain::WATER:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, BLUE);
                            break;

                        case Terrain::MUD:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, DARKBROWN);
                            break;
                    }
                }
            }
        }
};

class Player {
    
    public: 
        Texture2D texture;
        float x;
        float y;
        float speed;
        int health;

    void Draw() {
        DrawTextureEx(texture, {x,y}, 0.0f, 3.0f, WHITE);
    }

    void Update() {
        float dt = GetFrameTime(); 
        Vector2 direction = {0.0f, 0.0f};

        if(IsKeyDown(KEY_W)) {
            direction.y -= 1.0f;
        };

        if(IsKeyDown(KEY_A)) {
            direction.x -= 1.0f;
        };

        if(IsKeyDown(KEY_S)) {
            direction.y += 1.0f;
        };

        if(IsKeyDown(KEY_D)) {
            direction.x += 1.0f;
        }

        direction = Vector2Normalize(direction);
        
        x += direction.x * speed * dt;
        y += direction.y * speed * dt;
    }
};

int main() {
    Player player;
    cout << " >> Player Intialized" << endl;

    Image image = {(void*)charactersprite_data[0], 16, 16, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    cout << ">> Image Object Intialized and Populated" << endl;

    World world = {worldWidth, worldHeight, tileSize};

    //Initialise Window   
    InitWindow(cellSize*40 , cellSize*40, "Fabricate");
    SetTargetFPS(frameRate);
    cout << ">> Window Created" << endl;
    
    player = {LoadTextureFromImage(image), (float)cellSize*10, (float)cellSize*10, 100.00f, 10};
    
    Camera2D camera = {{(float)cellSize*20, (float)cellSize*20}, {player.x, player.y}, 0.0f, 1.0f};

    world.Generate();
    while(WindowShouldClose() == false) {

        //Updates
        player.Update();

        camera.target = {player.x, player.y};

        //Making the Character
        BeginDrawing();
            DrawFPS(10,10);

            ClearBackground(GREEN);
        
            BeginMode2D(camera);
                world.Draw();
                player.Draw();
            EndMode2D();
            DrawText(TextFormat("X : %.2f | Y: %.2f", player.x, player.y), 10, 10, 20, BLACK);
        EndDrawing();
        
    };

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}