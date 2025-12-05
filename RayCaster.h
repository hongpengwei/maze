#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "Constants.h"
#include "Player.h"
#include <vector>
#include <SDL2/SDL.h>

// 地圖數據定義 (在地圖中的數字代表牆壁的種類)
extern int worldMap[MAP_WIDTH][MAP_HEIGHT];

class RayCaster {
public:
    // 建構函數 (不需要複雜初始化，但保留以便未來擴充資源管理)
    RayCaster() = default;

    /**
     * @brief 執行核心 Ray Casting 邏輯並更新像素緩衝區
     * @param player 玩家物件，提供位置和方向
     * @param pixels 待寫入的像素緩衝區
     * @param texture 最終渲染的 SDL 紋理
     * @param renderer 渲染器
     */
    void renderFrame(const Player& player, std::vector<Uint32>& pixels, 
                     SDL_Texture* texture, SDL_Renderer* renderer);
};

#endif // RAYCASTER_H