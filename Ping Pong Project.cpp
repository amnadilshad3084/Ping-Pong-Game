#include <raylib.h>
#include <iostream>
#include <fstream>
using namespace std;

int playerscore = 0;//tracks current score
int highestscore = 0;

// wall position and size
const int wallx = 5;  // x coordinate of wall
const int wally = 0;   // y coordinate of top of wall
const int wallwidth = 10;  // width of  wall
int wallheight = 0;        // height of wall dynamically set in the main(i.e set during runtime)

// ball variables
float ballx, bally;      //balls position on the screen.
int ballspeedx, ballspeedy;
int ballradius;  // ball size

// paddle variables
float paddlex, paddley;   //paddles position
float paddlewidth, paddleheight;  //paddles size
int paddlespeed;         //how fast paddle moves

// functions used
void drawball();
void drawpaddle();
void updateball(bool& gameover);//updates balls position
void updatepaddle();
void loadhighestscore();
void savescore();


void drawball() {
    DrawCircle((int)ballx, (int)bally, ballradius, WHITE);
}

void drawpaddle() {
    DrawRectangle((int)paddlex, (int)paddley, (int)paddlewidth, (int)paddleheight, WHITE);
}

void updateball(bool& gameover) {//updates balls position
    //horizontal position of ball.
    ballx = ballx + ballspeedx;//if speed +ve ball shifts right, if speed -ve ball shifts to left
    //for vertical position
    bally = bally + ballspeedy;


    //BALL COLLISION WITH TOP AND BOTTOM OF SCREEN
//    TOP EDGE                 // BOTTOM EDGE
    if (bally - ballradius <= 0 || bally + ballradius >= GetScreenHeight()) {//if this value is less than/equal to 0 
        ballspeedy = ballspeedy * -1;                        // it means top edge of the ball has touched screen.
        //sign of ballspeedy is flipped
        // -ve = up   +ve = up
    }

    //                   BALL COLLISION WITH PADDLE
    // right edge of ball>=leftside of paddle                                 (bottom of paddle):             
    if ((ballx + ballradius >= paddlex) && (bally >= paddley && bally <= paddley + paddleheight)) {
        //ball collides                   //ball is at/below top of paddle && ball at/above bottom of paddle

        ballspeedx = ballspeedx * -1;// so ball will bounce back
        playerscore++; // incrementing score after successful hit
        if (playerscore > highestscore) {
            highestscore = playerscore;
        }
    }

    //BALL COLLISON WITH WALL
    if (ballx - ballradius <= wallx + wallwidth) {// left side of ball has passed right side of wall
        ballspeedx = ballspeedx * -1; // bounce back
    }

    // check if ball misses paddle so game over
    if (ballx + ballradius >= GetScreenWidth()) {
        gameover = true; // end game
    }
}


void updatepaddle() {
    if (IsKeyDown(KEY_UP) && paddley > 0) {//paddley > 0 ensures paddle stays within screen
        paddley = paddley - paddlespeed;//y coordinate decreases to move up
    }
    if (IsKeyDown(KEY_DOWN) && paddley + paddleheight < GetScreenHeight()) {
        paddley = paddley + paddlespeed;//y coordinate increases to move down
    }
}

void savescore() {
    ofstream file("scores.txt");
    if (file.is_open()) {
        file << highestscore;// writes highest score in file
        file.close();
    }
}

void loadhighestscore() {
    ifstream file("scores.txt");//reading file
    if (file.is_open()) {
        file >> highestscore;//reads value from file and stores in variable highestscore
        file.close();
    }
}

int main() {
    const int screenwidth = 1280;  // screen size
    const int screenheight = 720;
    wallheight = screenheight;     // set wall height same as screen height

    loadhighestscore(); // load highest score at start

    InitWindow(screenwidth, screenheight, "Ping Pong Game");//part of raylib library,initializes window for game
    SetTargetFPS(60);

    // initializing ball
    ballx = screenwidth / 2.0f;//divide by 2 bcz ensures ball is in centre at start
    bally = screenheight / 2.0f;//2.0f means floating point precision
    ballspeedx = 5;
    ballspeedy = 5;
    ballradius = 15;//ball size


    // initialize player paddle
    paddlex = screenwidth - 30;// paddle starts 30 pixels away from right boundary
    paddley = screenheight / 2 - 50;//  /2 ensure paddle is in centre each time and paddle vertically on screen
    paddlewidth = 20;
    paddleheight = 100;
    paddlespeed = 6;

    bool gameover = false;// false means game is ongoing

    while (WindowShouldClose() == false) {
        BeginDrawing();
        Color darkblue = { 0, 0, 50, 255 }; // custom color made (red green blue opacity)
        ClearBackground(darkblue);

        if (gameover == false) {
            // update game elements
            updateball(gameover);
            updatepaddle();
        }
        else {
            // game over screen
            DrawText("GAME OVER", screenwidth / 2 - 150, screenheight / 2 - 50, 50, RED);//2-150 moves in center & left
            DrawText("Press SPACE to Restart", screenwidth / 2 - 150, screenheight / 2 + 20, 20, WHITE);
            if (IsKeyPressed(KEY_SPACE)) {
                savescore();           // save score
                playerscore = 0;       // reset score to 0
                ballx = screenwidth / 2.0f;  // reset ball position in centre of screen
                bally = screenheight / 2.0f;

                //function selects any random number from 0 or 1 of direction of ball
                if (GetRandomValue(0, 1) == 0) {  //if random no. is 0
                    ballspeedx = -5;   //set horizontal speed to -ve, ball moves left
                }
                else {
                    ballspeedx = 5;  //set horizontal speed to +ve
                }

                if (GetRandomValue(0, 1) == 0) {
                    ballspeedy = -5; //set vertical speed to -ve
                }
                else {
                    ballspeedy = 5;  //set vertical speed to +ve
                }
                gameover = false;    // resume game
            }
        }

        // draw game elements
        drawball();
        drawpaddle();

        // draw wall
        DrawRectangle(wallx, wally, wallwidth, wallheight, WHITE);

        // draw score
        DrawText(TextFormat("Score: %d", playerscore), 60, 30, 45, WHITE);//(x,y coordinate, size,colour )
        DrawText(TextFormat("Highest: %d", highestscore), 60, 70, 35, RED);

        EndDrawing();
    }

    savescore(); // save score on exit
    CloseWindow();
    return 0;
}