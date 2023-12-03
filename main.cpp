#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <iostream>

int main(int, char**)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return -1;
    }

    // ==========================================
    // Setup SDL
    SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);
    if (!window) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // ==========================================
    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup ImGui for SDL2
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // ==========================================
    // Create a red 64x32 texture
    constexpr int TEXTURE_WIDTH = 64;
    constexpr int TEXTURE_HEIGHT = 32;
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    Uint32 redColor = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), 255, 0, 0);
    Uint32* pixels = new Uint32[TEXTURE_WIDTH * TEXTURE_HEIGHT];

    for (int i = 0; i < TEXTURE_WIDTH * TEXTURE_HEIGHT; ++i) {
        pixels[i] = redColor;
    }

    SDL_UpdateTexture(texture, nullptr, pixels, TEXTURE_WIDTH * sizeof(Uint32));
    delete[] pixels;

    // Render loop
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        SDL_WaitEvent(&event);
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
            case SDLK_q:
                quit = true;
                break;
            }
            break;
        }
        }

        // Render SDL
        SDL_RenderClear(renderer);
        SDL_Rect destRect = { 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);

        // Render ImGui
        ImGui_ImplSDL2_NewFrame();
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is a sample ImGui window.");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        // Submit to render
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
