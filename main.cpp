#include "raylib.h"
#include <iostream>

bool running = true;
bool pieceSetDown = false;
short LastInputTime = 0;
unsigned int frameCount = 0;
unsigned int points = 0;

char Board[22][13] = {
    "############",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "#          #",
    "############"
};

char Tetrominos[7][4][5] = {
    {
        "0   ",
        "0   ",
        "0   ",
        "0   "
    },
    {
        "4   ",
        "44  ",
        " 4  ",
        "    "
    },
    {
        " 6  ",
        "66  ",
        "6   ",
        "    "
    },
    {
        "33  ",
        "33  ",
        "    ",
        "    "
    },
    {
        "22  ",
        " 2  ",
        " 2  ",
        "    "
    },
    {
        "11  ",
        "1   ",
        "1   ",
        "    "
    },
    {
        "5   ",
        "55  ",
        "5   ",
        "    "
    },
};

struct TertrominoVertex{
    short y, x;
};

class Tertromino{
    public:
    short x = 5, y = 1;
    int TetroNum; // The array index of the tetromino
    char disp;
    TertrominoVertex vertexes[4];
    Tertromino(int _TetroNum){
        TetroNum = _TetroNum;
        short numOfNonWhiteSpace = 0;
        for (short i = 0; i < 4; i++){
            for (short j = 0; j < 4; j++){
                if (numOfNonWhiteSpace >= 4){return;}
                if (Tetrominos[TetroNum][i][j] != ' '){
                    numOfNonWhiteSpace++;
                    disp = Tetrominos[TetroNum][i][j];                      
                    vertexes[numOfNonWhiteSpace-1] = {i, j};
                }
            }
        }
    }
};

void ClearLines(){
    int LinesCleared = 0; // For scoring points
    for (int i = 20; i > 0; i--){ // Looping through the lines (Bottom up)
        bool full = true;
        for (int j = 1; j < 12; j++){ // Looping through the line
            if (Board[i][j] == ' '){ // Empty space
                full = false;
                break;
            }
        }
        if (full){// The line is full
            LinesCleared++;
            for (int k = 1; k < 11; k++){Board[i][k] = ' ';} // Clear the line
            for (int k = i; k > 1; k--){ // Looping through the list backwards
                auto temp = Board[k];
                for (int l = 1; l < 11; l++){Board[k][l] = Board[k-1][l];} // Copy the line above to the line below
                for (int l = 1; l < 11; l++){Board[k-1][l] = temp[l];} // Vice versa
            }
            i++; // So the line above isnt skipped
        }
    }
    switch(LinesCleared){
        case 1:
            points += 40;
            break;
        case 2:
            points += 100;
            break;
        case 3:
            points += 300;
            break;
        case 4:
            points += 1200;
            break;
    }
}

void movePiece(Tertromino * t){
    // Clearing where the piece was
    int targx = t->x, targy = t->y;
    for (int i = 0; i < 4; i++){
        Board[t->vertexes[i].y + t->y][t->vertexes[i].x + t->x] = ' ';
    }
    /* Proccess the tetronimo 
    // Move it down every period of time
    if (clock() - TetrisTime > 750){
        targy++;
        TetrisTime = clock();
    }*/
    // Input
    
    if (frameCount >= 40){
        frameCount = 0;
        targy++;
    }
    
    if (LastInputTime >= 3){
        LastInputTime = 0;
        if(IsKeyDown(KEY_S)){
            if (targy == t->y){ // If the tetris timer didnt go off
                targy++;
            }
        } else if (IsKeyDown(KEY_A)){
            targx--;
        } else if (IsKeyDown(KEY_D)){
            targx++;
        } else if (IsKeyDown(KEY_R)){
            TertrominoVertex * vtemp = new TertrominoVertex[4];
            for (int i = 0; i < 4; i++){ // Rotate the vertexes 90 degrees
                auto temp = t->vertexes[i].x;
                vtemp[i].x = -t->vertexes[i].y; 
                vtemp[i].y = temp;
            }
            int min = 0;
            for (int i = 0; i < 4; i++){
                if (vtemp[i].x < min){
                    min = vtemp[i].x;
                }
            }
            for (int i = 0; i < 4; i++){
                vtemp[i].x -= min;
            }
            bool fine = true;
            for (int i = 0; i < 4; i++){
                if (Board[vtemp[i].y + t->y][vtemp[i].x + t->x] != ' '){ // Rotation causes collision
                    fine = false;
                    break;
                }
            }
            if (fine){
                for (int i = 0; i < 4; i++){
                    t->vertexes[i] = vtemp[i];
                }
            }
            delete[] vtemp;
        } else if (IsKeyDown(KEY_Q)){
            for (int i = 1; i < 21; i++){
                for (int j = 1; j < 11; j++){
                    Board[i][j] = ' ';
                }
            }
        }
    }
    /* Collision */
    if (targx != t->x){ // Moved horizontally
        bool collided = false;
        for (int i = 0; i < 4; i++){
            if (Board[t->vertexes[i].y + targy][t->vertexes[i].x + targx] != ' '){
                collided = true;
                break;
            }
        }
        if (!collided){
            t->x = targx;
        }
    }
    if (targy != t->y){ // Moved vertically
        bool collided = false;
        for (int i = 0; i < 4; i++){
            if (Board[t->vertexes[i].y + targy][t->vertexes[i].x + t->x] != ' '){
                collided = true;
                pieceSetDown = true; // The piece has hit the ground
                break;
            }
        }
        if (!collided){
            t->y = targy;
        }
    }
    // Draw the tetronimo to the screen
    for (int i = 0; i < 4; i++){
        Board[t->vertexes[i].y + t->y][t->vertexes[i].x + t->x] = t->disp;
    }
}

int main(void){
    const int screenWidth = 650;
    const int screenHeight = 850;
    
    const int BlockSize = 40;
    
    Color CurrentColor;
    
    Tertromino * CurrentTetronimo = new Tertromino(GetRandomValue(0, 6));

    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose() && running){
        LastInputTime++;
        frameCount++;
        movePiece(CurrentTetronimo);
        if (pieceSetDown){
            ClearLines();
            pieceSetDown = false;
            delete CurrentTetronimo;
            CurrentTetronimo = new Tertromino(GetRandomValue(0, 6));
            for (int i = 0; i < 4; i++){ // Checking if where the block spawned is colliding
                if (Board[CurrentTetronimo->y + CurrentTetronimo->vertexes[i].y][CurrentTetronimo->x + CurrentTetronimo->vertexes[i].x] != ' '){
                    running = false;
                }
            }
        }
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawRectangle(40, 10, 420, 820, GRAY);
        DrawText(TextFormat("Score: %08i", points), 470, 10, 20, RED);
        
        for (int i = 1; i < 21; i++){
            for (int j = 1; j < 11; j++){
                /*bool black = true;*/
                switch(Board[i][j]){
                    case '0':
                        CurrentColor = SKYBLUE;
                        break;
                    case '1':
                        CurrentColor = DARKBLUE;
                        break;
                    case '2':
                        CurrentColor = ORANGE;
                        break;
                    case '3':
                        CurrentColor = YELLOW;
                        break;
                    case '4':
                        CurrentColor = GREEN;
                        break;
                    case '5':
                        CurrentColor = VIOLET;
                        break;
                    case '6':
                        CurrentColor = RED;
                        break;
                    default:
                        CurrentColor = RAYWHITE;
                        /*black = false;*/
                        break;
                }
                /*if (black)
                DrawRectangle(50 + (j-1) * BlockSize, 20 + (i-1) * BlockSize, BlockSize, BlockSize, BLACK);
                else*/
                DrawRectangle(50 + (j-1) * BlockSize, 20 + (i-1) * BlockSize, BlockSize, BlockSize, CurrentColor);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
