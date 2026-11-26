#include <raylib.h>
#include "../assets/Sprites/characterSprite.c"
#include <iostream>
#include <math.h>
#include <raymath.h>

const int windowHeight = 800;
const int windowWidth = 800;
const int frameRate = 60;
int cellSize = 20;

using namespace std;

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

    
    //Initialise Window   
    InitWindow(cellSize*40 , cellSize*40, "Fabricate");
    SetTargetFPS(frameRate);
    cout << ">> Window Created" << endl;
    
    player = {LoadTextureFromImage(image), (float)cellSize*10, (float)cellSize*10, 100.00f, 10};
    
    Camera2D camera = {{(float)cellSize*20, (float)cellSize*20}, {player.x, player.y}, 0.0f, 1.0f};

    while(WindowShouldClose() == false) {

        //Updates
        player.Update();

        camera.target = {player.x, player.y};

        //Making the Character
        BeginDrawing();
        ClearBackground(GREEN);
        
            BeginMode2D(camera);
                DrawRectangle(100, 100, 40, 40, RED);
                player.Draw();
            EndMode2D();
            DrawText(TextFormat("X : %.2f | Y: %.2f", player.x, player.y), 10, 10, 20, BLACK);
        EndDrawing();
        
    };

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}