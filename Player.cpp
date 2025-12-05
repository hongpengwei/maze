#include "Player.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// 從 Constants.h 引入地圖
extern int worldMap[MAP_WIDTH][MAP_HEIGHT];

Player::Player(double startX, double startY, double startDirX, double startDirY)
    : posX(startX), posY(startY), dirX(startDirX), dirY(startDirY) {
    // 根據方向向量計算攝像機平面 (垂直於 dir 向量，66度 FOV)
    if (dirX == -1.0 && dirY == 0.0) { // 初始朝西
        planeX = 0.0;
        planeY = 0.66;
    } else {
        // 這裡可以寫通用的垂直向量計算，但為簡潔，僅處理初始值
        planeX = -dirY * 0.66;
        planeY = dirX * 0.66;
    }
}

void Player::move(double frameTime, bool forward, double speedFactor) {
    double actualMoveSpeed = MOVE_SPEED * frameTime * 30.0 * speedFactor;
    double moveStep = forward ? actualMoveSpeed : -actualMoveSpeed;
    
    // 碰撞檢測：允許進入空地 (0) 或出口 (6)
    // 檢查 X 軸
    int nextMapX = worldMap[(int)(posX + dirX * moveStep)][(int)posY];
    if (nextMapX == 0 || nextMapX == 6) {
        posX += dirX * moveStep;
    }
    // 檢查 Y 軸
    int nextMapY = worldMap[(int)posX][(int)(posY + dirY * moveStep)];
    if (nextMapY == 0 || nextMapY == 6) {
        posY += dirY * moveStep;
    }
}

void Player::rotate(double frameTime, bool right) {
    double actualRotSpeed = ROT_SPEED * frameTime * 30.0;
    double rotAngle = right ? -actualRotSpeed : actualRotSpeed;
    
    double oldDirX = dirX;
    // 旋轉方向向量
    dirX = dirX * std::cos(rotAngle) - dirY * std::sin(rotAngle);
    dirY = oldDirX * std::sin(rotAngle) + dirY * std::cos(rotAngle);
    
    double oldPlaneX = planeX;
    // 旋轉攝像機平面向量
    planeX = planeX * std::cos(rotAngle) - planeY * std::sin(rotAngle);
    planeY = oldPlaneX * std::sin(rotAngle) + planeY * std::cos(rotAngle);
}

// 處理滑鼠旋轉邏輯 <--- [新增實作]
void Player::rotateMouse(double deltaX, double sensitivity) {
    // 角度計算：位移 * 靈敏度
    // 這裡通常加負號，讓滑鼠向右移動時，視角向右旋轉 (符合FPS遊戲習慣)
    double rotAngle = -deltaX * sensitivity;

    double oldDirX = dirX;
    // 旋轉方向向量
    dirX = dirX * std::cos(rotAngle) - dirY * std::sin(rotAngle);
    dirY = oldDirX * std::sin(rotAngle) + dirY * std::cos(rotAngle);

    double oldPlaneX = planeX;
    // 旋轉攝像機平面向量
    planeX = planeX * std::cos(rotAngle) - planeY * std::sin(rotAngle);
    planeY = oldPlaneX * std::sin(rotAngle) + planeY * std::cos(rotAngle);
}

void Player::strafe(double frameTime, bool right, double speedFactor) { // <--- [新增實作]
    // 速度計算 (與 move 函數相同)
    double actualMoveSpeed = MOVE_SPEED * frameTime * 30.0 * speedFactor;
    
    // 如果 'right' 為真 (D鍵)，則 moveStep 為正 (右平移)；否則為負 (左平移)
    double moveStep = right ? actualMoveSpeed : -actualMoveSpeed;
    
    // --- 碰撞及位置更新 ---
    
    // 檢查 X 軸：沿著 planeX 方向移動 (側向移動)，允許進入空地 (0) 或出口 (6)
    int nextMapX = worldMap[(int)(posX + planeX * moveStep)][(int)posY];
    if (nextMapX == 0 || nextMapX == 6) {
        posX += planeX * moveStep;
    }
    
    // 檢查 Y 軸：沿著 planeY 方向移動 (側向移動)，允許進入空地 (0) 或出口 (6)
    int nextMapY = worldMap[(int)posX][(int)(posY + planeY * moveStep)];
    if (nextMapY == 0 || nextMapY == 6) {
        posY += planeY * moveStep;
    }
}
bool Player::checkExit(int mapValue) {
    // 如果踩到出口標記 (6)，設置通關狀態
    if (mapValue == MAP_EXIT) {
        hasReachedExit = true;
        return true;
    }
    return false;
}