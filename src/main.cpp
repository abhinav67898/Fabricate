#include <raylib.h>

const int windowHeight = 800;
const int windowWidth = 800;
const int frameRate = 60;
int ballX = 400;
int ballY = 400;

int main() {
    //Initialise Window 
    InitWindow(windowWidth, windowHeight, "Fabricate");
    SetTargetFPS(frameRate);

    while(WindowShouldClose() == false) {
        
    }

    CloseWindow();
    return 0;
}