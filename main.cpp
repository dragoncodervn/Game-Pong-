#include <iostream>
#include <raylib.h> //From Github
#define endl "\n"

//Colour
Color LightRed = Color{255,128,128,255};
Color Yellow = Color{255,255,0,255};
Color LightBlue = Color{0,204,255,255};
Color LightGreen = Color{0,161,98,255};
Color DarkGreen = Color{0,100,0,255};
Color KellyGreen = Color{76,187,23,255};

int player_score = 0;
int cpu_score = 0;

class Paddle{
protected:
    void LimitMovement(){
        if(y <= 0){
            y = 0;
        }
        if(y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }
public:
    float x,y;
    float width,height;
    int speed;
    void Draw(){
        //DrawRectangle(x,y,width,height,WHITE);
        DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,LightBlue);
    }
    void Update(){
        if(IsKeyDown(KEY_UP)){
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN)){
            y += speed;
        }
        LimitMovement();
    }
    void ResetPaddle(){
        width = 25;
        height = 120;
        x = GetScreenWidth() - width - 10;
        y = GetScreenHeight()/2 - height/2;

    }
};
class CpuPaddle : public Paddle{
    public:
        void Update(int ball_y){
            if(y + height > ball_y){
                y -= speed;
            }
            else if(y + height <= ball_y){
                y += speed;
            }
             if(y <= 0){
                y = 0;
            }
            if(y + height >= GetScreenHeight()){
                y = GetScreenHeight() - height;
            }
            LimitMovement();
    }
    void Draw(){
            DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,RED);
        }
    void ResetPaddle(){
        /*
        cpu.width = 25;
        cpu.height = 120;
        cpu.x = 10;
        cpu.y = screen_height/2 - cpu.height/2;
        */
        width = 25;
        height = 120;
        x = 10;
        y = GetScreenHeight()/2 - height/2;

    }
};


Paddle player;
CpuPaddle cpu;

class Ball{
public:
    float x,y;
    int speed_x,speed_y;
    int radius;
    void Draw(){
        DrawCircle(x,y,radius,YELLOW);
    }
    void Update(){

        x += speed_x;

        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }
        if(x + radius >= GetScreenWidth()){
            cpu_score++;
            player.ResetPaddle();
            cpu.ResetPaddle();
            ResetBall();
        }
        if(x - radius <= 0){
            player_score++;
            player.ResetPaddle();
            cpu.ResetPaddle();
            ResetBall();
        }
    }
    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        int speed_choices[2] = {1,-1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

Ball ball;

void GameplayDisplay(int screen_width,int screen_height){
    ClearBackground(KellyGreen);
        //DrawRectangle(screen_width/2,0,screen_width/2,screen_height,KellyGreen);
        //DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);
       
    DrawCircle(screen_width/2,screen_height/2,120,WHITE);
    DrawCircle(screen_width/2,screen_height/2,110,KellyGreen);
    DrawRectangle(screen_width/2 - 5,0,10,screen_height,WHITE);
    DrawCircle(screen_width/2,screen_height/2,20,WHITE);
    DrawText("10 points to win",screen_width/2 - 150,20,40,DARKPURPLE);
}

void CheckWin(int screen_width,int screen_height){
    if(player_score >= 10){
        DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),LightBlue);
        DrawText("Player Wins",screen_width/2 - 305,screen_height/2 - 40,100,WHITE);
        return;
    }
    if(cpu_score >= 10){
        DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),LightRed);
        DrawText("Computer Wins",screen_width/2 - 360,screen_height/2 - 40,100,WHITE);
        return;
    }
}

void CustomLog(int msgType, const char *text, va_list args)
{ 
  return;
}


using namespace std;

signed main () {
    //cout<<"Starting the game"<< endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    SetTraceLogCallback(CustomLog); // call this before InitWindow() 
    InitWindow(screen_width,screen_height,"Pong!");
    SetTargetFPS(60);

    //Ball
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 8;
    ball.speed_y = 8;

    //Player
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 7;

    //Computer
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 7;

    while(!WindowShouldClose()){
        //system("cls");
    
        //Update
        ball.Update();
        player.Update();
        cpu.Update(ball.y);
        //Checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})){
            ball.speed_x *= -1;
        }

        //Drawing
        BeginDrawing();
        GameplayDisplay(screen_width,screen_height);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%d",cpu_score),screen_width/4 - 20,20,80,RED);
        DrawText(TextFormat("%d",player_score),3 * screen_width/4 - 20,20,80,LightBlue);
        CheckWin(screen_width,screen_height);
        EndDrawing();
    }
    CloseWindow();
    return (0^0);
}