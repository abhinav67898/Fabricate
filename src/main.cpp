#include <raylib.h>

int windowHeight = 800;
int windowWidth = 600;

int main() {
    //Initialise Window 
    InitWindow(windowHeight, windowWidth, "Fabricate");

    //Game Loop
    while(WindowShouldClose() == false) {
        // Event Handling

        // Updating Positions

        // Drawings
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}