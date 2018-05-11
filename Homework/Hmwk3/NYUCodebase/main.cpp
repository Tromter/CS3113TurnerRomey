#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include "Matrix.h"

#include <math.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define MAX_ENEMIES 20
#define MAX_BULLETS 40
#define spriteCountX 16
#define spriteCountY 8
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;


bool gameOver = false;

void DrawSprite(ShaderProgram* program, int index){
    
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
    float spriteWidth = 1.0f / (float)spriteCountX;
    float spriteHeight = 1.0f / (float)spriteCountY;
    
    GLfloat texCoords[] = {
        u, v + spriteHeight,
        u + spriteWidth, v,
        u, v,
        u + spriteWidth, v,
        u, v + spriteHeight,
        u + spriteWidth, v + spriteHeight
    };
    
    float vertices[] = { -0.35f, -0.35f,
        0.35f,  0.35f,
        -0.35f,  0.35f,
        
        0.35f,  0.35f,
        -0.35f, -0.35f,
        0.35f, -0.35f
    };
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

class Vector3 {
public:
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {};
    
    float x;
    float y;
    float z;
    
};

class Entity {
public:
    Entity(){};
    Entity(Vector3 pos, Vector3 mysize, int sprite_num): position(pos), size(mysize), sprite(sprite_num) {};
    void Draw(ShaderProgram* program){
        if(alive){
            Matrix modelMatrix;
            modelMatrix.Translate(position.x, position.y, 0.0f);
            program->SetModelMatrix(modelMatrix);
            DrawSprite(program, sprite);
        }
    }
    void Update(float elapsed){
        position.x += elapsed * velocity.x;
        position.y += elapsed * velocity.y;
        
        if (position.y > 3.55){
            velocity.y = 0;
        }
        
        if(enemy){
            if((position.x + size.x) > 3.95){
                position.x -= .01f;
                position.y -= .6f;
                velocity.x = -velocity.x;
            }
            
            else if ((position.x -size.x) < -3.95f){
                position.x += .001f;
                position.y -= .6f;
                velocity.x = -velocity.x;
            }
            
            if(position.y < -1.0f){
                gameOver = true;
            }
            
            
        }
        
    }
    
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    
    float rotation;
    int sprite;
    
    bool enemy = false;
    
    bool alive = true;
};


class GameState{
public:
    GameState(Entity player) : player(player) {};
    
    Entity player;
    //Entity enemies[MAX_ENEMIES];
    std::vector<Entity> enemies;
    int myEnemies = 0;
    Entity bullets[MAX_BULLETS];
    int score = 0;
    int bulletIndex = 0;
};



void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
    
    float texture_size = 1.0/16.0f;
    
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    
    for(int i=0; i < text.size(); i++) {
        int spriteIndex = (int)text[i];
        float texture_x = (float)(spriteIndex % 16) / 16.0f;
        float texture_y = (float)(spriteIndex / 16) / 16.0f;
        
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    // draw this data (use the .data() method of std::vector to get pointer to data)
    // draw this yourself, use text.size() * 6 or vertexData.size()/2 to get number of vertices
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6 * text.size());
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

GLuint LoadTexture(const char *filePath) {
    int w,h,comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    if(image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(image);
    return retTexture;
}

void SetUp(GameState* myGame){
    float x = -2.1f;
    float y = 1.4f;
    
    myGame->score = 0;
    gameOver = false;
    
    for(int i = 0; i < MAX_ENEMIES; i++){
        myGame->myEnemies += 1;
        if(x > 2.4f){
            x = -2.1f;
            y -= 0.7f;
        }
        
        Vector3 myEnemy_pos(x, y);
        Vector3 myEnemy_size(.7, .7);
        int myEnemy_sprite = 84;
        
        Entity enemy(myEnemy_pos, myEnemy_size, myEnemy_sprite);
        enemy.velocity.x = 0.25f;
        enemy.enemy = true;
        
        myGame->enemies.push_back(enemy);
        x += .6f;
    }
    
    for (int i = 0; i < MAX_BULLETS; i++){
        myGame->bullets[i].position.x = 100;
        myGame->bullets[i].position.y = 100;
        myGame->bullets[i].sprite = 22;
    }
}


void shootBullets(GameState* myGame, Entity* player){
    myGame->bullets[myGame->bulletIndex].position.x = player->position.x;
    myGame->bullets[myGame->bulletIndex].position.y = player->position.y + .35f;
    myGame->bullets[myGame->bulletIndex].velocity.y = 2.0f;
}

void Render_Game(ShaderProgram* program, Entity* player, GameState* myGame){
    player->Draw(program);
    for(Entity& enemy : myGame->enemies){
        enemy.Draw(program);
    }
    
    for (size_t i = 0; i < MAX_BULLETS; i++){
        myGame->bullets[i].Draw(program);
    }
}

void Render_Menu(ShaderProgram *program, int textSprite){
    
    Matrix modelMatrix;
    
    modelMatrix.Identity();
    modelMatrix.Translate(-1, 1.0f, 0.0f);
    program->SetModelMatrix(modelMatrix);
    
    DrawText(program, textSprite, "Space Invaders", .30f, 0);
    
    modelMatrix.Identity();
    modelMatrix.Translate(-1, -1.0f, 0.0f);
    program->SetModelMatrix(modelMatrix);
    DrawText(program, textSprite, "press [space] to start", .17f, 0);
}


bool collider(float x1, float y1, float x2, float y2, float width1, float height1, float width2, float height2){
    if(y1+height1 / 2.0f < y2 - height2 / 2.0f || y1 - height1 /2.0f > y2 + height2 / 2.0f || x1 - width1 / 2.0f > x2 + width2 /2.0f || x1 + width1 / 2.0f < x2 - width2 /2.0f){
        return false;
    }
    return true;
}


void Update(float elapsed, GameState* myGame){
    for (size_t i = 0; i < myGame->enemies.size(); i++){
        myGame->enemies[i].Update(elapsed);
    }
    
    for (size_t i = 0; i < MAX_BULLETS; i++){
        myGame->bullets[i].Update(elapsed);
        for(size_t j = 0; j < myGame->enemies.size(); j++){
            bool isColliding = collider(
                                        myGame->bullets[i].position.x, myGame->bullets[i].position.y,
                                        myGame->enemies[j].position.x, myGame->enemies[j].position.y,
                                        myGame->bullets[i].size.x, myGame->bullets[i].size.y,
                                        myGame->enemies[j].size.x,  myGame->enemies[j].size.y
                                        );
            
            if(isColliding){
                if((myGame->enemies[j].alive)){
                    myGame->enemies[j].alive = false;
                    myGame->bullets[i].position.x = 100;
                    myGame->bullets[i].position.y = 100;
                    myGame->myEnemies -= 1;
                    myGame->score += 1;
                }
            }
            
        }
    }
    
    
    
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    SDL_Event event;
    bool done = false;
    glViewport(0,0, 640, 360);
    
    enum GameMode {TITLE_SCREEN, GAME_01};
    GameMode state = TITLE_SCREEN;
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program.programID);
    
    GLuint textSprite = LoadTexture(RESOURCE_FOLDER"font1.png");
    GLuint characters = LoadTexture(RESOURCE_FOLDER"mySprites.png");
    
    glBindTexture(GL_TEXTURE_2D, characters);
    
    Matrix projectionMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    float lastFrameTicks = 0.0f;
    
    Vector3 main_player;
    main_player.x = 0.0f;
    main_player.y = -1.7f;
    
    Vector3 size;
    size.x = .7f;
    size.y = .7f;
    
    Entity player(main_player, size, 89);
    player.velocity.x = 1.5f;
    player.velocity.y = 0;
    
    GameState myGame(player);
    
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        
            else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                    if(state == TITLE_SCREEN){
                        state = GAME_01;
                        SetUp(&myGame);
                    }
                    else if(state == GAME_01){
                        if(myGame.bulletIndex > MAX_BULLETS - 1){
                            myGame.bulletIndex = 0;
                        }
                        
                        shootBullets(&myGame, &player);
                        myGame.bulletIndex++;
                        
                    }
                }
            }
        }
        
        float ticks = float(SDL_GetTicks() / 1000.0f);
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        glClear(GL_COLOR_BUFFER_BIT);
        
        switch(state){
            case TITLE_SCREEN:
                Render_Menu(&program, textSprite);
                break;
                
            case GAME_01:
                glBindTexture(GL_TEXTURE_2D, characters);
                
                if(keys[SDL_SCANCODE_RIGHT]){
                    if(!((player.position.x + (player.size.y / 2)) > 3.55)){
                        player.position.x += elapsed * player.velocity.x;
                    }
                }
                else if (keys[SDL_SCANCODE_LEFT]){
                    if(!((player.position.x - (player.size.y / 2)) < -3.55)){
                        player.position.x -= elapsed * player.velocity.x;
                    }
                }
                
                Update(elapsed, &myGame);
                
                Render_Game(&program, &player, &myGame);
                
                if(myGame.myEnemies == 0 || gameOver){
                    myGame.enemies.clear();
                    state = TITLE_SCREEN;
                }
                
                Matrix modelMatrix;
                modelMatrix.Translate(0.0, 1.75f, 0.0f);
                program.SetModelMatrix(modelMatrix);
                DrawText(&program, textSprite, std::to_string(myGame.score), .25f, -0.05f);
                
                
                break;
                
        }
        
        glDisableVertexAttribArray(program.positionAttribute);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
