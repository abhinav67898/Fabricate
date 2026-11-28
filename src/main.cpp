#include <raylib.h>
#include "../assets/Sprites/characterSprite.c"
#include "../assets/Sprites/drillSprite.c"
#include "../assets/Sprites/Conveyor_UPSprite.c"
#include "../assets/Sprites/oresSprites.c"
#include <iostream>
#include <math.h>
#include <raymath.h>

const int windowHeight = 800;
const int windowWidth = 800;
const int frameRate = 24;
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
    DRILL,
    CONVEYOR
};

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum class Items {
    NONE,
    IRON,
    COPPER,
    COAL
};

struct ItemData {
    Items type = Items::NONE;
    float progress = 0.0f;
};

struct MachineData {
    Machines type = Machines::NONE;
    float timer = 0.0f;
    int storedResources = 0;
    Direction direction = Direction::RIGHT;

    ItemData item;
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
        MachineData machinesTiles[100][100];
        Texture2D drillTexture;

        Texture2D conveyorTexture[15];
        int conveyorFrame = 0;
        float conveyorTimer = 0.0f;
        float conveyorFPS = 24.0f;

        Texture2D oresTextures[3];


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
                    machinesTiles[i][j].type = Machines::NONE;

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
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
                    if(machinesTiles[y][x].type == Machines::DRILL) {
                        Vector2 positionDrill = {
                            (float)(x*tileSize), 
                            (float)(y*tileSize)
                        };
                        float scale = (float)tileSize/drillTexture.width;
                        DrawTextureEx(drillTexture, positionDrill, 0.0f, scale, WHITE);
                    }

                    if(machinesTiles[y][x].type == Machines::CONVEYOR) {
                        Vector2 positionConveyor = {
                            (float)(x*tileSize),
                            (float)(y*tileSize)
                        };
                        float scale = (float)tileSize / 16;

                        Rectangle source = {0, 0, 16, 16};
                        Rectangle dest = {
                            x * tileSize + tileSize / 2.0f,
                            y * tileSize + tileSize / 2.0f,
                            (float)tileSize,
                            (float)tileSize
                        };
                        Vector2 origin = { tileSize / 2.0f, tileSize/ 2.0f};
                        float rotation = static_cast<int>(machinesTiles[y][x].direction) * 90.0f;
                        DrawTexturePro(conveyorTexture[conveyorFrame], source, dest, origin, rotation, WHITE);
                    }
                }
            }
        };

        void UpdateMachines(float dt) {
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
                    if(machinesTiles[y][x].type == Machines::DRILL) {
                        if(machinesTiles[y][x].storedResources < 10) {
                            machinesTiles[y][x].timer +=dt;
                            if(machinesTiles[y][x].timer >= 2.0f) {
                                machinesTiles[y][x].storedResources += 1;
                                machinesTiles[y][x].timer -= 2;
                                cout << ">> Storage of machine at " << y << ", " << x << "is: " << machinesTiles[y][x].storedResources << "\n";
                            }
                        }
                    }
                }
            }
        };

        void LoadConveyorTextures() {
            for(int i = 0; i < 15; i++) {
                Image frame = {
                    (void*)Conveyor_UpSprite_data[i],
                    16, 16, 1,
                    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
                };

                conveyorTexture[i] = LoadTextureFromImage(frame);
                SetTextureFilter(conveyorTexture[i], TEXTURE_FILTER_POINT);
            }
        }

        void UpdateConveyorAnimation(float dt) {
            conveyorTimer += dt;
            while(conveyorTimer >= 1.0f / conveyorFPS) {
                conveyorFrame = (conveyorFrame + 1) % 15;
                conveyorTimer -= 1.0f / conveyorFPS;
            }
        }

        void LoadOresTextures() {
            for(int i = 0; i < 3; i++) {
                Image frameOres = {
                    (void*)oresSprites_data[i],
                    16, 16, 1,
                    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
                };

                oresTextures[i] = LoadTextureFromImage(frameOres);
                SetTextureFilter(oresTextures[i], TEXTURE_FILTER_POINT);
            }
        }

        void drawItems() {
            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
                    if(machinesTiles[y][x].item.type == Items::IRON) {
                        Vector2 positionOres = {
                            x * tileSize + 8.0f,
                            y * tileSize + 8.0f
                        };

                        DrawTextureEx(oresTextures[0], positionOres, 0.0f, 1.0f, WHITE);
                    }
                }
            }
        }
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

    Image drillImage = {(void*)drillSprite_data[0], 16, 16, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    cout << ">> Drill Object Intialized and Populated" << endl;

    world = {worldWidth, worldHeight, tileSize};

    //Initialise Window   
    InitWindow(cellSize*40 , cellSize*40, "Fabricate");
    SetTargetFPS(frameRate);
    cout << ">> Window Created" << endl;

    player = {LoadTextureFromImage(image), {0} , (float)cellSize*10, (float)cellSize*10, 200.00f, 10};
    world.drillTexture = LoadTextureFromImage(drillImage);
    SetTextureFilter(world.drillTexture, TEXTURE_FILTER_POINT);

    world.LoadConveyorTextures();
    world.LoadOresTextures();
    
    Camera2D camera = {{(float)cellSize*20, (float)cellSize*20}, {player.x, player.y}, 0.0f, 1.0f};

    Machines selectedMachine = Machines::DRILL;
    Direction selectedDirection = Direction::RIGHT;

    world.Generate();
    //GAME LOOP STARS HERE
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

        world.UpdateMachines(GetFrameTime());
        world.UpdateConveyorAnimation(GetFrameTime());

        BeginDrawing();
            ClearBackground(GREEN);
        
            BeginMode2D(camera);
                world.Draw();
                world.DrawMachines();
                world.drawItems();
                player.Draw();

                //Select Drill
                if(IsKeyPressed(KEY_ONE)) {
                    selectedMachine = Machines::DRILL;
                };

                //Select Conveyor
                if(IsKeyPressed(KEY_TWO)) {
                    selectedMachine = Machines::CONVEYOR;
                }

                if(IsKeyPressed(KEY_R)) {
                    selectedDirection = static_cast<Direction>((static_cast<int>(selectedDirection) + 1) % 4);
                };
                
                //Tile Validator
                if(validTile) {
                    DrawRectangleLines(mouseTileX * tileSize, mouseTileY * tileSize, tileSize, tileSize, RED);
                };

                if(validTile && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(world.machinesTiles[mouseTileY][mouseTileX].type != Machines::DRILL) {
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
                        };
                    };
                };

                if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                    // cout << ">> Right Mouse Cick detected \n";
                    if(validTile) {
                        // cout << ">> Tile is Valid \n";
                        if(player.inventory.iron >=5) {
                            // cout << "Player Has Sufficient Iron: " << player.inventory.iron <<"\n";
                            if(world.machinesTiles[mouseTileY][mouseTileX].type == Machines::NONE && selectedMachine == Machines::DRILL) {
                                switch(world.tiles[mouseTileY][mouseTileX]) {
                                    case Terrain::IRON:
                                        player.inventory.iron -= 5;
                                        world.machinesTiles[mouseTileY][mouseTileX].type = Machines::DRILL;
                                        break;
                                    case Terrain::COAL:
                                        player.inventory.iron -= 5;
                                        world.machinesTiles[mouseTileY][mouseTileX].type = Machines::DRILL;
                                        break;
                                    case Terrain::COPPER:
                                        player.inventory.iron -= 5;
                                        world.machinesTiles[mouseTileY][mouseTileX].type = Machines::DRILL;
                                        break;
                                    default:
                                        break;
                                }
                            };
                        };
                    };
                };

                
                if(IsKeyPressed(KEY_E) && world.machinesTiles[mouseTileY][mouseTileX].type == Machines::DRILL && validTile) {
                    int temp = world.machinesTiles[mouseTileY][mouseTileX].storedResources;
                    switch(world.tiles[mouseTileY][mouseTileX]) {
                        case Terrain::IRON:
                            player.inventory.iron += temp;
                            world.machinesTiles[mouseTileY][mouseTileX].storedResources -= temp;
                            break;
                        case Terrain::COAL:
                            player.inventory.coal += temp;
                            world.machinesTiles[mouseTileY][mouseTileX].storedResources -= temp;
                            break;
                        case Terrain::COPPER:
                            player.inventory.copper += temp;
                            world.machinesTiles[mouseTileY][mouseTileX].storedResources -= temp;
                            break;                            
                        default:
                            break;
                    }
                }

                if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && validTile && player.inventory.iron >= 1 && world.machinesTiles[mouseTileY][mouseTileX].type == Machines::NONE && selectedMachine == Machines::CONVEYOR) {
                    // switch(world.tiles[mouseTileY][mouseTileX]) {
                    //     case Terrain::IRON:
                    //         player.inventory.iron -= 1;
                    //         world.machinesTiles[mouseTileY][mouseTileX].direction = selectedDirection;
                    //         world.machinesTiles[mouseTileY][mouseTileX].type = Machines::CONVEYOR;
                    //         break;
                    //     case Terrain::COAL:
                    //         player.inventory.iron -= 1;
                    //         world.machinesTiles[mouseTileY][mouseTileX].type = Machines::CONVEYOR;
                    //         break;
                    //     case Terrain::COPPER:
                    //         player.inventory.iron -= 1;
                    //         world.machinesTiles[mouseTileY][mouseTileX].type = Machines::CONVEYOR;
                    //         break;
                    //     case Terrain::GRASS:
                    //         player.inventory.iron -= 1;
                    //         world.machinesTiles[mouseTileY][mouseTileX].type = Machines::CONVEYOR;
                    //     default:
                    //         break;
                    // }
                    if(world.tiles[mouseTileY][mouseTileX] != Terrain::WATER) {
                        player.inventory.iron -= 1;
                        world.machinesTiles[mouseTileY][mouseTileX].type = Machines::CONVEYOR;
                        world.machinesTiles[mouseTileY][mouseTileX].direction = selectedDirection;
                    }
                }

                //Conveyor Rotation 
                if(validTile && IsKeyPressed(KEY_R)) {
                    if(world.machinesTiles[mouseTileY][mouseTileX].type == Machines::CONVEYOR) {
                        world.machinesTiles[mouseTileY][mouseTileX].direction = static_cast<Direction>((static_cast<int>(world.machinesTiles[mouseTileY][mouseTileX].direction) + 1 ) %4);
                    }
                }

                if(IsKeyPressed(KEY_F)) {
                    if(validTile) {
                        if(world.machinesTiles[mouseTileY][mouseTileX].type == Machines::CONVEYOR) {
                            if(world.machinesTiles[mouseTileY][mouseTileX].item.type == Items::NONE) {
                                world.machinesTiles[mouseTileY][mouseTileX].item.type = Items::IRON;
                                world.machinesTiles[mouseTileY][mouseTileX].item.progress = 0.0f;
                            }
                        }
                    }
                }

                
            EndMode2D();
            if(validTile && world.machinesTiles[mouseTileY][mouseTileX].type == Machines::DRILL) {
                DrawText(
                    TextFormat("Drill Storage: %d / 10", world.machinesTiles[mouseTileY][mouseTileX].storedResources),
                    10, 160, 20, WHITE
                );
            }
            const char* machineName = selectedMachine == Machines::DRILL ? "Drill" : "Conveyor";
            DrawText(TextFormat("Selected: %s", machineName), 10, 190, 20, WHITE);
            DrawText(TextFormat("X: %.2f | Y: %2.f", mousePosition.x, mousePosition.y), 10, 40, 20, BLACK);
            DrawText(TextFormat("X : %.2f | Y: %.2f", player.x, player.y), 10, 10, 20, BLACK);
            DrawText(TextFormat("Iron: %d", player.inventory.iron), 10, 70, 20, WHITE);
            DrawText(TextFormat("Copper: %d", player.inventory.copper), 10, 100, 20, WHITE);
            DrawText(TextFormat("Coal: %d", player.inventory.coal), 10, 130, 20, WHITE);
        EndDrawing();
    };

    UnloadTexture(player.texture);
    for(int i = 0; i < 15; i ++) {
        UnloadTexture(world.conveyorTexture[i]);
    }
    UnloadTexture(world.drillTexture);
    CloseWindow();
    return 0;
}