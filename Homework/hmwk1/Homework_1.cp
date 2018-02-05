#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;



int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 360);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    projectionMatrix.SetOrthoProjection(-3.55 * 2.0f, 3.55 * 2.0f, -2.0f * 2.0f, 2.0f * 2.0f, -1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    
    //modelMatrix.Scale(2.0f, 2.0f, 1.0f);
    //modelMatrix.Rotate(45.0f * (3.1415926f/180.0f));
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.SetModelMatrix(modelMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        modelMatrix.Identity();
        modelMatrix.Translate(0.0f, 0.0f, 0.0f);
        modelMatrix.Scale(50.0f, 50.0f, 0.0f);
        program.SetModelMatrix(modelMatrix);
        program.SetColor(0.3f, 0.7f, 0.0f, 0.0f);
        float vertices3[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices3);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        modelMatrix.Identity();
        modelMatrix.Translate(0.0f, 23.2f, 0.0f);
        modelMatrix.Scale(50.0f, 50.0f, 50.0f);
        program.SetModelMatrix(modelMatrix);
        program.SetColor(0.0f, 0.6f, 1.0f, 0.3f);
        
        float vertices[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        
        glDisableVertexAttribArray(program.positionAttribute);
        
        
        modelMatrix.Identity();
        modelMatrix.Translate(0.0f, 0.0f, 0.0f);
        modelMatrix.Scale(6.6f, 8.0f, 10.0f);
        program.SetModelMatrix(modelMatrix);
        program.SetColor(0.47f, 0.45f, 0.46f, -0.5);
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        modelMatrix.Identity();
        modelMatrix.Translate(-3.0f, -0.50f, 0.0f);
        modelMatrix.Scale(5.0f, 7.0f, 10.0f);
        program.SetModelMatrix(modelMatrix);
        //program.SetColor(0.001f, 0.2f, 0.1f, 0.1f);//tree color
        program.SetColor(0.47f, 0.45f, 0.46f, 0.1f);
        float vertices4[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices4);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        modelMatrix.Identity();
        modelMatrix.Translate(3.0f, -1.0f, 0.0f);
        modelMatrix.Scale(4.0f, 6.0f, 10.0f);
        program.SetModelMatrix(modelMatrix);
        //program.SetColor(0.001f, 0.2f, 0.1f, 0.1f);//tree color
        program.SetColor(0.47f, 0.45f, 0.46f, 0.1f);
        float vertices5[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices5);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        modelMatrix.Identity();
        modelMatrix.Translate(3.0f, 0.6f, 0.0f);
        modelMatrix.Scale(2.0f, 3.0f, 10.0f);
        program.SetModelMatrix(modelMatrix);
        //program.SetColor(0.001f, 0.2f, 0.1f, 0.1f);//tree color
        program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        float vertices6[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices6);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        modelMatrix.Identity();
        modelMatrix.Translate(-3.0f, 1.3f, 0.0f);
        modelMatrix.Scale(2.5f, 3.5f, 10.0f);
        program.SetModelMatrix(modelMatrix);
        //program.SetColor(0.001f, 0.2f, 0.1f, 0.1f);//tree color
        program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        float vertices7[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices7);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        modelMatrix.Identity();
        modelMatrix.Translate(0.0f, 2.0f, 0.0f);
        modelMatrix.Scale(3.3f, 4.0f, 10.0f);
        program.SetModelMatrix(modelMatrix);
        //program.SetColor(0.001f, 0.2f, 0.1f, 0.1f);//tree color
        program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        float vertices8[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT,  false,  0,  vertices8);
        glEnableVertexAttribArray(program.positionAttribute);
        glDrawArrays(GL_TRIANGLES, 0 , 3);
        glDisableVertexAttribArray(program.positionAttribute);
        
        
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
