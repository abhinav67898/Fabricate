#include <raylib.h>
#include "../assets/Sprites/characterSprite.c"

const int windowHeight = 800;
const int windowWidth = 800;
const int frameRate = 60;
int cellSize = 20;

class Player {
    
    public: 
        Texture2D texture;
        float x;
        float y;
        float speed;
        int health;
};

int main() {
    Player player;
    Image image = {(void*)charactersprite_data[0], 16, 16, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};
    
    //Initialise Window   
    InitWindow(cellSize*40 , cellSize*40, "Fabricate");
    SetTargetFPS(frameRate);

    player = {LoadTextureFromImage(image), (float)cellSize*10, (float)cellSize*10, 10.00f, 10};

    while(WindowShouldClose() == false) {

        //Making the Character
        BeginDrawing();
        ClearBackground(GREEN);
        EndDrawing();
        
    };

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}