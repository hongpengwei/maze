#include <iostream>
#include <SDL2/SDL.h>
#include "Constants.h"
#include "Player.h"
#include "RayCaster.h"

// SDL 資源 (原始指標，建議面試時替換為 std::unique_ptr)
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
std::vector<Uint32> pixels(SCREEN_WIDTH * SCREEN_HEIGHT); // 像素緩衝區

// 函數前置宣告
bool initializeSDL();
void cleanupSDL();

int main(int argc, char* argv[]) {
    // 檢查 SDL 初始化
    if (!initializeSDL()) {
        return 1;
    }
    // --- 啟用相對滑鼠模式 (鎖定滑鼠並隱藏) --- <--- [新增]
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // 實例化 Player 和 RayCaster
    Player player(10.0, 10.0, 0.0, 1.0); 
    RayCaster caster;
    
    // --- 主遊戲迴圈 ---
    bool running = true;
    Uint32 oldTime = SDL_GetTicks();
    
    while (running) {
        Uint32 newTime = SDL_GetTicks();
        double frameTime = (newTime - oldTime) / 1000.0; 
        oldTime = newTime;

        // 1. 事件處理
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            // --- 處理滑鼠移動事件 (只在鎖定時旋轉) --- [修改]
            else if (e.type == SDL_MOUSEMOTION) {
                // 只有在 SDL 處於相對滑鼠模式 (鎖定中) 才執行旋轉
                if (SDL_GetRelativeMouseMode()) { 
                    player.rotateMouse((double)e.motion.xrel, MOUSE_SENSITIVITY); 
                }
            }
            
            // --- 處理按鍵按下事件 (用於切換鎖定) --- <--- [新增]
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    // 切換滑鼠相對模式：
                    // !SDL_GetRelativeMouseMode() 會獲取當前狀態的相反值 (TRUE 變 FALSE, FALSE 變 TRUE)
                    SDL_SetRelativeMouseMode((SDL_bool)!SDL_GetRelativeMouseMode());
                }
                
                // 如果您用 Esc 退出程式，請將原本的退出邏輯也放在這裡
                // 舉例： if (e.key.keysym.sym == SDLK_q) running = false;
            }
        }

        // 2. 輸入處理 (鍵盤狀態)
        const Uint8* state = SDL_GetKeyboardState(NULL);
        
        // --- 偵測 Shift 鍵並計算速度倍率 --- <--- [新增]
        double speedFactor = 1.0; 
        if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT]) {
            speedFactor = SPRINT_FACTOR; 
        }

        // 使用 speedFactor 呼叫移動函數 <--- [修改]
        if (state[SDL_SCANCODE_W]) player.move(frameTime, true, speedFactor); 
        if (state[SDL_SCANCODE_S]) player.move(frameTime, false, speedFactor); 
        // --- A/D 鍵：側向平移 (Strafe) --- <--- [修改/替換]
        // A 鍵 (左平移): right = false
        if (state[SDL_SCANCODE_A]) player.strafe(frameTime, false, speedFactor); 
        // D 鍵 (右平移): right = true
        if (state[SDL_SCANCODE_D]) player.strafe(frameTime, true, speedFactor);
        // 旋轉速度不變 (如果您也想加速旋轉，可以自行調整 rotSpeed)
        // if (state[SDL_SCANCODE_A]) player.rotate(frameTime, false);
        // if (state[SDL_SCANCODE_D]) player.rotate(frameTime, true);

        // 3. 核心渲染
        caster.renderFrame(player, pixels, texture, renderer);

        // --- 檢查玩家是否到達出口 --- <--- [新增]
        int playerMapX = (int)player.posX;
        int playerMapY = (int)player.posY;
        if (playerMapX >= 0 && playerMapX < MAP_WIDTH && playerMapY >= 0 && playerMapY < MAP_HEIGHT) {
            player.checkExit(worldMap[playerMapX][playerMapY]);
        }

        // --- 如果通關，停止遊戲迴圈 --- <--- [新增]
        if (player.hasReachedExit) {
            std::cout << "🎉 恭喜！你已通關迷宮！" << std::endl;
            running = false;
        }

        // 可選：顯示 FPS
        std::string title = "C++ Ray Caster | FPS: " + std::to_string(1.0 / frameTime);
        SDL_SetWindowTitle(window, title.c_str());
    }
    
    // 清理資源
    cleanupSDL();
    return 0;
}


// --- SDL 初始化與清理的實作 ---

bool initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("C++ Ray Caster Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 
                                SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!texture) {
        std::cerr << "Texture could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void cleanupSDL() {
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}