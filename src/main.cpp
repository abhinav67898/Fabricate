#include <raylib.h>
#include "../assets/Sprites/characterSprite.c"
#include <iostream>

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

        if(IsKeyDown(KEY_W)) {
            y -= speed * dt;
        } else if(IsKeyDown(KEY_A)) {
            x -= speed * dt;
        } else if(IsKeyDown(KEY_S)) {
            y += speed * dt;
        } else if(IsKeyDown(KEY_D)) {
            x += speed * dt;
        }
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

    while(WindowShouldClose() == false) {

        //Updates
        player.Update();

        //Making the Character
        BeginDrawing();
        ClearBackground(GREEN);
        player.Draw();
        EndDrawing();
        
    };

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}