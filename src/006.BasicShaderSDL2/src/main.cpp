#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#ifdef _EMSCRIPTEN
#include <emscripten.h>
#endif

struct Context {
    SDL_Window* window;
    GLuint shader;
    GLuint vao;
    int currentWidth, currentHeight;
    std::chrono::high_resolution_clock::time_point startTime;
};

#ifdef _EMSCRIPTEN
void mainLoop(void* arg) {
    Context *ctx = static_cast<Context *>(arg);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            emscripten_cancel_main_loop();
        }

        // Handle window resize event
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_GetWindowSize(ctx->window, &ctx->currentWidth, &ctx->currentHeight);
            glViewport(0, 0, ctx->currentWidth, ctx->currentHeight);
        }
    }

    float time = std::chrono::duration<float>(
                     std::chrono::high_resolution_clock::now() - ctx->startTime)
                     .count();

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(ctx->shader);
    glUniform2f(glGetUniformLocation(ctx->shader, "iResolution"), 
                static_cast<float>(ctx->currentWidth), 
                static_cast<float>(ctx->currentHeight));
    glUniform1f(glGetUniformLocation(ctx->shader, "iTime"), time);

    glBindVertexArray(ctx->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(ctx->window);
}
#endif

std::string loadFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(const std::string& src, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compile error (" << src << "):\n" << log << std::endl;
    }
    return shader;
}

GLuint createProgram(const std::string& vsText, const std::string& fsText) {
    GLuint vs = compileShader(vsText, GL_VERTEX_SHADER);
    GLuint fs = compileShader(fsText, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main() {

    Context ctx;
    SDL_Init(SDL_INIT_VIDEO);
    ctx.window = SDL_CreateWindow("Shader Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!ctx.window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    #ifdef _EMSCRIPTEN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    #endif
    SDL_GLContext context = SDL_GL_CreateContext(ctx.window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    #ifndef _EMSCRIPTEN
    glewInit();
    #endif

// For shaders, version needs to be changed (i think)
// to 330 core for desktop OpenGL and 300 es for WebGL
// Also fragColor needs to be changed to out_FragColor
// and the fragment shader needs to specify the precision for WebGL
// Example: --> precision mediump float;

    const std::string shaderVert = R"""(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)""";

    const std::string shaderVertWeb = 
R"""(#version 300 es
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)""";
    const std::string shaderFrag = R"""(
#version 330 core
out vec4 FragColor;
uniform float iTime;
uniform vec2 iResolution;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec3 col = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0,2,4));
    FragColor = vec4(col, 1.0);
}
)""";
    const std::string shaderFragWeb = 
R"""(#version 300 es
precision mediump float;

out vec4 out_FragColor;
uniform float iTime;
uniform vec2 iResolution;

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec3 col = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0,2,4));
    out_FragColor = vec4(col, 1.0);
}
)""";
    const std::string shaderFragX = R"""(
#version 330 core
out vec4 FragColor;
uniform float iTime;
uniform vec2 iResolution;

// Palette function from ShaderToy
vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

// Main image function from ShaderToy
void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;
        float d = length(uv) * exp(-length(uv0));
        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);
        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);
        d = pow(0.01 / d, 1.2);
        finalColor += col * d;
    }

    fragColor = vec4(finalColor, 1.0);
}

// Fragment shader main function
void main() {
    mainImage(FragColor, gl_FragCoord.xy);
}
)""";
    const std::string shaderFragXWeb = 
R"""(#version 300 es
precision mediump float;
out vec4 out_FragColor;
uniform float iTime;
uniform vec2 iResolution;

// Palette function from ShaderToy
vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

// Main image function from ShaderToy
void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;
        float d = length(uv) * exp(-length(uv0));
        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);
        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);
        d = pow(0.01 / d, 1.2);
        finalColor += col * d;
    }

    fragColor = vec4(finalColor, 1.0);
}

// Fragment shader main function
void main() {
    mainImage(out_FragColor, gl_FragCoord.xy);
}
)""";

#ifdef _EMSCRIPTEN
    ctx.shader = createProgram(shaderVertWeb, shaderFragXWeb);
#else
    ctx.shader = createProgram(shaderVert, shaderFragX);
#endif

    float vertices[] = {
        -1.0f, -1.0f,
         3.0f, -1.0f,
        -1.0f,  3.0f
    };

    GLuint vbo;
    glGenVertexArrays(1, &ctx.vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(ctx.vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    bool running = true;
    SDL_Event event;

    // Set the initial viewport size
    SDL_GetWindowSize(ctx.window, &ctx.currentWidth, &ctx.currentHeight);
    glViewport(0, 0, ctx.currentWidth, ctx.currentHeight);

    // Start the timer
    ctx.startTime = std::chrono::high_resolution_clock::now(); 


    #ifdef _EMSCRIPTEN
    emscripten_set_main_loop_arg(mainLoop, &ctx, 0, 1);
    #endif
    #ifndef _EMSCRIPTEN
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            // Handle window resize event
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(ctx.window, &ctx.currentWidth, &ctx.currentHeight);
                glViewport(0, 0, ctx.currentWidth, ctx.currentHeight);
            }
        }

        float time = std::chrono::duration<float>(
        std::chrono::high_resolution_clock::now() - ctx.startTime).count();

        // Crear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ctx.shader);

        glUniform2f(glGetUniformLocation(ctx.shader, "iResolution"), static_cast<float>(ctx.currentWidth), static_cast<float>(ctx.currentHeight));
        glUniform1f(glGetUniformLocation(ctx.shader, "iTime"), time);
        
        glBindVertexArray(ctx.vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(ctx.window);
    }
    #endif

    glDeleteProgram(ctx.shader);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &ctx.vao);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
    return 0;
}
