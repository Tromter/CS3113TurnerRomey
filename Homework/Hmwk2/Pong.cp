#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

using namespace std;

SDL_Window* displayWindow;

class Paddle{
public:
    Paddle(float left, float right, float top, float bottom) : left(left), right(right), top(top), bottom(bottom){}
    
    float getLeft(){
        return left;
    }
    
    float getRight(){
        return right;
    }
    
    float getTop(){
        return top;
    }
    
    float getBottom(){
        return bottom;
    }
    
    void newTop(float new_top){
        top = new_top;
    }
    
    void newBottom(float new_bot){
        bottom = new_bot;
    }
    
private:
    float left;
    float right;
    float top;
    float bottom;
};


int main(int argc, char *argv[])
{

    float gameball_x = 0.0f;
    float gameball_y = 0.0f;
    
    float ball_Speed = 0.5f;
    
    double angleX = cos(0.4);
    double angleY = cos(-0.2);
    
    float ypos1 = 0;
    float ypos2 = 0;
    
    float boost = 0.1f;

    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    
    Matrix left_paddle_m;
    Matrix right_paddle_m;
    Matrix ball;
    Matrix view;
    Matrix project;

    
    project.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    
    glUseProgram(program.programID);
    glClearColor(0.0,0.0,0.0,0.0);
    SDL_Event event;
    bool done = false;
    
    Paddle left_player(-3.5f, -3.4f, 0.5f, -0.5f);
    Paddle right_player(3.4f, 3.5f, 0.5f, -0.5f);
    
    float lastFrameTicks = 0;
    float distance_to_travel_in_one_second = 2;
    
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        float tickytok = (float)SDL_GetTicks() /1000.0f;
        
        float elapsed = tickytok - lastFrameTicks;
        lastFrameTicks = tickytok;
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        
        if(keys[SDL_SCANCODE_W]){
                if(left_player.getTop() < 2.0f){
                    float new_top = left_player.getTop() + (elapsed * distance_to_travel_in_one_second);
                    left_player.newTop(new_top);
                    float new_bot = left_player.getBottom() + (elapsed * distance_to_travel_in_one_second);
                    left_player.newBottom(new_bot);
                    
                    cout << ypos1 << endl;
                    
                    ypos1 += elapsed * distance_to_travel_in_one_second;
                    
                    
                }
            }
            if(keys[SDL_SCANCODE_S]){
                if(left_player.getBottom() > -2.0f){
                    float new_top = left_player.getTop() - (elapsed * distance_to_travel_in_one_second);
                    left_player.newTop(new_top);
                    float new_bot = left_player.getBottom() - (elapsed * distance_to_travel_in_one_second);
                    left_player.newBottom(new_bot);
                    
                    ypos1 -= elapsed * distance_to_travel_in_one_second;
                    
                     cout << ypos1 << endl;
                    
                }
            }
            
            if(keys[SDL_SCANCODE_UP]){
                if(right_player.getTop() < 2.0f){
                    float new_top = right_player.getTop() + (elapsed * distance_to_travel_in_one_second);
                    right_player.newTop(new_top);
                    float new_bot = right_player.getBottom() + (elapsed * distance_to_travel_in_one_second);
                   right_player.newBottom(new_bot);
                    
                    ypos2 += elapsed * distance_to_travel_in_one_second;
                    
                    cout << ypos2 << endl;
                    
                }
            }
            if(keys[SDL_SCANCODE_DOWN]){
                if(right_player.getBottom() > -2.0f){
                    float new_top = right_player.getTop() - (elapsed * distance_to_travel_in_one_second);
                    right_player.newTop(new_top);
                    float new_bot = right_player.getBottom() - (elapsed * distance_to_travel_in_one_second);
                    right_player.newBottom(new_bot);
                    
                    ypos2 -= elapsed * distance_to_travel_in_one_second;
                    
                    cout << ypos2 << endl;
                    
                }
            }

        
        if(gameball_x - 0.1f <= left_player.getLeft()){
            glClearColor(0.0f, 0.4f, 1.0f, 0.3f);
            gameball_x = 0;
            gameball_y = 0;
            ball_Speed = 0;
            std::cout << "Right Player Wins" << endl;
        }
        
        if (gameball_x + 0.1f >= right_player.getRight()){
            glClearColor(0.3f, 0.5f, 0.0f, 0.0f);
            gameball_x = 0;
            gameball_y = 0;
            ball_Speed = 0;
            std::cout << "Left Player Wins" << endl;
        }
        
        if((gameball_x - 0.1f) <= -3.05 && (gameball_y - 0.1f) <= left_player.getTop() && (gameball_y + 0.1f) >= left_player.getBottom()){
            angleX = -angleX;
            ball_Speed += boost;
        }
        
        else if(gameball_x <= -3.05 && (gameball_y - 0.1f) <= left_player.getTop() && (gameball_y + 0.1f) >= left_player.getBottom()){
            angleX = -angleX;
            angleY = -angleY;
            ball_Speed += boost;
        }
        
        if((gameball_x + 0.1f) >= 3.05 && (gameball_y - 0.1f) <= right_player.getTop() && (gameball_y+0.1f) >= right_player.getBottom()){
            angleX = -angleX;
            ball_Speed += boost;
        }
        
        else if((gameball_x) >= 3.05 && (gameball_y - 0.1f) <= right_player.getTop() && (gameball_y+0.1f) >= right_player.getBottom()){
            angleX = -angleX;
            ball_Speed += boost;
        }
        
        
        
        if (gameball_y + 0.1f >= 2.0f || gameball_y - 0.1f <= -2.0f){
            angleY = -angleY;
            ball_Speed += boost;
        }
        
        gameball_y += angleY * ball_Speed * elapsed;
        gameball_x += angleX * ball_Speed * elapsed;
        
        
        
        program.SetViewMatrix(view);
        program.SetProjectionMatrix(project);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        //left
        Matrix modelMatrix;
        modelMatrix.Translate(-2.75, ypos1, 1);
        program.SetModelMatrix(modelMatrix);
        float paddle_left_coords[] = {-0.5, -0.4, -0.3, -0.4, -0.3, 0.4, -0.5, -0.4, -0.3, 0.4, -0.5, 0.4};
        program.SetColor(0.3f, 0.7f, 0.0f, 0.0f);
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle_left_coords);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //right
        modelMatrix.Identity();
        modelMatrix.Translate(2.75, ypos2,1);
        program.SetModelMatrix(modelMatrix);
        float paddle_right_coords[] = { 0.3, -0.4, 0.5, -0.4, 0.5, 0.4, 0.3, -0.4, 0.5, 0.4, 0.3, 0.4 };
        program.SetColor(0.0f, 0.6f, 1.0f, 0.3f);
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle_right_coords);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //ball
        Matrix ball;
        ball.Translate(gameball_x, gameball_y, 0);
        program.SetModelMatrix(ball);
        float ball_coords[] = { -0.1f, -0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, 0.1f, -0.1f, 0.1f, -0.1f, -0.1f };
        program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball_coords);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        glDisableVertexAttribArray(program.positionAttribute);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
