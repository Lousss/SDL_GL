//
//  main.cpp
//  SDL_GL
//
//  Created by Sergey Apenkov on 22/08/2021.
//  Copyright © 2021 Sergey Apenkov. All rights reserved.
//
#define GL_SILENCE_DEPRECATION
#define GLEW_STATIC

#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <GLKit/GLKMath.h>

static const GLfloat g_vertex_buffer_data[] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

static const char* vertex_shader_text =
    "#version 410\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "out vec4 a_color;\n"
    "void main()\n"
    "{\n"
    "    a_color = vec4(1.0f,1.0f,1.0f,1.0f);\n"
    "    gl_Position.xyz = vertexPosition_modelspace;\n"
    "    gl_Position.w = 1.0;\n"
    "}\n";

static const char* fragment_shader_text =
    "#version 410\n"
    "in vec4 a_color;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "     color = a_color;\n"
    "}\n";

GLuint LoadShaders(const char * vertex_shader, const char * fragment_shader){
    
    // Создаем шейдеры
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Загружаем код Вершинного Шейдера из файла
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    // Компилируем Вершинный шейдер
    glShaderSource(VertexShaderID, 1, &vertex_shader , NULL);
    glCompileShader(VertexShaderID);
    
    // Выполняем проверку Вершинного шейдера
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    
    if ( InfoLogLength > 0 ){
        std::cout << "\nVertex Shader\n";
        char message[InfoLogLength+1];
        glGetShaderInfoLog(VertexShaderID,InfoLogLength, NULL, message);
        std::cout << message;
    }
    else  std::cout << "\nVertex done\n";
    
    // Компилируем Фрагментный шейдер
    glShaderSource(FragmentShaderID, 1, &fragment_shader , NULL);
    glCompileShader(FragmentShaderID);
    
    // Проверяем Фрагментный шейдер
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0 ){
        std::cout << "Fragment Shader\n";
        char message[InfoLogLength+1];
        glGetShaderInfoLog(FragmentShaderID,InfoLogLength, NULL, message);
        std::cout << message;
    }
    else {
        std::cout << "Fragment done\n";
    }
    
    // Создаем шейдерную программу и привязываем шейдеры к ней
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    
    // Проверяем шейдерную программу
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::cout << "Compile \n";
        char message[InfoLogLength+1];
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, message);
        std::cout << message;
        
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        glDeleteProgram(ProgramID);
    }
    else std::cout << "Compile done\n";
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    
    return ProgramID;
}

int main(int argc, const char * argv[]) {
    
    bool run;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_Window *win = SDL_CreateWindow("Hello World!", 0, 0, 640, 480, SDL_WINDOW_OPENGL);
   
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GLContext glContext = SDL_GL_CreateContext(win);
    
    if (glContext == NULL) {
        std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    //glewExperimental=GL_TRUE;
    glewInit();
    std::cout << "\nGL Verion " << glGetString(GL_VERSION);
    
    GLuint VertexArrayID;
    GLuint vertexbuffer;
    
    glGenVertexArrays(1, &VertexArrayID);
    glGenBuffers(1, &vertexbuffer);
    
    glBindVertexArray(VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,  GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)(0*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    glViewport(0, 0, 640, 480);
    GLuint programID = LoadShaders(vertex_shader_text, fragment_shader_text );
    
    glClearColor(0.3f, 0.2f, 0.3f, 1.0f);
    
    run = true;
    SDL_Event event;
    
    while (run) {
        
        while ( SDL_PollEvent ( &event ) )  {
            if (event.type == SDL_QUIT) {
                run = false;
            }
            
            else if ( event.type == SDL_KEYDOWN ) {
                switch (event.key.keysym.sym ) {
                    case SDLK_0:
                        run = false;
                        break;
                        
                    default:
                        break;
                }
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0, 0.0, 1.0f, 1);
        
        glUseProgram(programID);
        glBindVertexArray(VertexArrayID);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);// Указываем, что первым буфером атрибутов будут вершины
        
        glBindVertexArray(0);
        
        SDL_GL_SwapWindow(win);
    }
    
    
    
    // После окончания работы программы
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
