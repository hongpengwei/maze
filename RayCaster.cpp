#include "RayCaster.h"
#include <cmath>
#include <iostream>
#include <algorithm>

// ...existing code...
int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,6,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, // 6 是出口
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

Uint32 getColor(int wallType, int side) {
    Uint32 color;
    switch (wallType) {
        case 1: color = 0xFFFF0000; break; // 紅色
        case 2: color = 0xFF00FF00; break; // 綠色
        case 3: color = 0xFF0000FF; break; // 藍色
        case 4: color = 0xFFFFFF00; break; // 黃色
        case 5: color = 0xFFFF7F00; break; // 橘色
        case 6: color = 0xFFFFFFFF; break; // 白色 (出口)
        default: color = 0xFFFF00FF; break; // 預設：洋紅色
    }
    if (side == 1) {
        color = (color >> 1) & 0x7F7F7F7F; 
        color |= 0xFF000000;
    }
    return color;
}
// ...existing code...

void RayCaster::renderFrame(const Player& player, std::vector<Uint32>& pixels, 
                            SDL_Texture* texture, SDL_Renderer* renderer) {
    
    // --- Ray Casting 核心邏輯 ---
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        // 1. 初始化射線和攝像機平面
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
        double rayDirX = player.dirX + player.planeX * cameraX;
        double rayDirY = player.dirY + player.planeY * cameraX;

        int mapX = (int)player.posX;
        int mapY = (int)player.posY;

        double sideDistX, sideDistY;
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        int stepX, stepY; 
        int hit = 0;      
        int side;         

        // 2. 計算初始 sideDist (Siddhart-Yadav's method)
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
        }

        // 3. DDA 迴圈 (找牆壁)
        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0; // 垂直牆 (X 軸)
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1; // 水平牆 (Y 軸)
            }
            if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT && worldMap[mapX][mapY] > 0) {
                hit = 1;
            }
        }

        // 4. 牆壁高度投影 (避免魚眼)
        double perpWallDist;
        if (side == 0) {
            perpWallDist = (mapX - player.posX + (1 - stepX) / 2) / rayDirX;
        } else {
            perpWallDist = (mapY - player.posY + (1 - stepY) / 2) / rayDirY;
        }
        
        if (perpWallDist < 0.001) perpWallDist = 0.001; // 防止除以零或過近
        
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        // 計算繪製範圍
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // 獲取牆壁顏色
        Uint32 color = getColor(worldMap[mapX][mapY], side);
        
        // 5. 繪製像素緩衝區
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            // 繪製天花板
            if (y < drawStart) {
                pixels[y * SCREEN_WIDTH + x] = 0xFF555555; 
            }
            // 繪製牆壁
            else if (y >= drawStart && y <= drawEnd) {
                pixels[y * SCREEN_WIDTH + x] = color;
            }
            // 繪製地板
            else {
                pixels[y * SCREEN_WIDTH + x] = 0xFF888888;
            }
        }
    }

    // 將像素數據傳輸到 SDL 紋理並繪製
    SDL_UpdateTexture(texture, NULL, pixels.data(), SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}