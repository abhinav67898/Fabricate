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
    WATER,
    IRON,
    COPPER,
    COAL
};

enum class Machines {
    NONE,
    DRILL
};

struct playerInventory {
    int iron = 0;
    int copper = 0;
    int coal = 0;
};

class World {
    public: 
        int width;
        int height;
        int tileSize;
        int number;
        Color tileColor;
        Terrain tiles[100][100];
        Machines machinesTiles[100][100];
        Texture2D drillTexture;

        void Generate() {
            Image perlinNoise = GenImagePerlinNoise(
                worldWidth, worldHeight, 0, 0, 2.0f
            );

            for(int i = 0; i < worldHeight; i++) {
                for (int j = 0; j < worldWidth; j++) {
                    Color pixel = GetImageColor(perlinNoise, j, i);

                    number = pixel.r;

                    // 225 / 50% = 112
                    // 225 / 20% = 45 
                    // 225 / 10% = 23
                    if(number <= 147) {
                        tiles[i][j] = Terrain::GRASS;
                    } else if(number > 147 && number <= 157) {
                        tiles[i][j] = Terrain::WATER;
                    } else if(number > 157 && number <= 180) {
                        tiles[i][j] = Terrain::IRON;
                    } else if(number > 180 && number <= 203) {
                        tiles[i][j] = Terrain::COPPER;
                    } else if(number > 203 && number <= 225) {
                        tiles[i][j] = Terrain::COAL;
                    };

                    //Machine Generation
                    machinesTiles[i][j] = Machines::NONE;

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

                        case Terrain::WATER:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, BLUE);
                            break;

                        case Terrain::IRON:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, GRAY);
                            break;

                        case Terrain::COPPER:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, ORANGE);
                            break;

                        case Terrain::COAL:
                            DrawRectangle(x*tileSize, y*tileSize, tileSize, tileSize, BLACK);
                            break;
                    }
                }
            }
        }

        void DrawMachines() {
            
        };
};

class Player {
    public: 
        Texture2D texture;
        playerInventory inventory; // To add an item simply player.investory.iron++;
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
    World world;
    cout << " >> Player Intialized" << endl;

    Image image = {(void*)charactersprite_data[0], 16, 16, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    cout << ">> Image Object Intialized and Populated" << endl;

    world = {worldWidth, worldHeight, tileSize};

    //Initialise Window   
    InitWindow(cellSize*40 , cellSize*40, "Fabricate");
    SetTargetFPS(frameRate);
    cout << ">> Window Created" << endl;

    player = {LoadTextureFromImage(image), {0} , (float)cellSize*10, (float)cellSize*10, 200.00f, 10};
    
    Camera2D camera = {{(float)cellSize*20, (float)cellSize*20}, {player.x, player.y}, 0.0f, 1.0f};

    world.Generate();
    while(WindowShouldClose() == false) {

        //Updates
        player.Update();
        camera.target = {player.x, player.y};

        Vector2 mousePosition = GetScreenToWorld2D(GetMousePosition(), camera);
        int mouseTileX = (int)floor(mousePosition.x/tileSize);
        int mouseTileY = (int)floor(mousePosition.y/tileSize);


        bool validTile = {
            mouseTileX >=0 &&
            mouseTileX < world.width &&
            mouseTileY >=0 &&
            mouseTileY < world.height
        };

        //Making the Character
        BeginDrawing();
            ClearBackground(GREEN);
        
            BeginMode2D(camera);
                world.Draw();
                player.Draw();
                if(validTile) {
                    DrawRectangleLines(mouseTileX * tileSize, mouseTileY * tileSize, tileSize, tileSize, RED);
                }

                if(validTile && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    switch(world.tiles[mouseTileY][mouseTileX]) {
                        case Terrain::IRON: 
                            player.inventory.iron++;
                            break;

                        case Terrain::COAL: 
                            player.inventory.coal++;
                            break;

                        case Terrain::COPPER:
                            player.inventory.copper++;
                            break;

                        default:
                            break;
                    }
                }

                if(validTile && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && player.inventory.iron > 5) {
                    switch(world.tiles[mouseTileY][mouseTileX]) {
                        case Terrain::IRON:
                            
                            
                    };
                };

            EndMode2D();
            DrawText(TextFormat("X: %.2f | Y: %2.f", mousePosition.x, mousePosition.y), 10, 40, 20, BLACK);
            DrawText(TextFormat("X : %.2f | Y: %.2f", player.x, player.y), 10, 10, 20, BLACK);
            DrawText(TextFormat("Iron: %d", player.inventory.iron), 10, 70, 20, WHITE);
            DrawText(TextFormat("Copper: %d", player.inventory.copper), 10, 100, 20, WHITE);
            DrawText(TextFormat("Coal: %d", player.inventory.coal), 10, 130, 20, WHITE);
        EndDrawing();
    };

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}