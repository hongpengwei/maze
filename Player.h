#ifndef PLAYER_H
#define PLAYER_H

#include "Constants.h"

class Player {
public:
    // 玩家狀態變數 (為方便 RayCaster 存取，設為 public)
    double posX, posY;    // 玩家在世界座標上的位置
    double dirX, dirY;    // 玩家的視野方向向量
    double planeX, planeY;  // 攝像機平面向量
    bool hasReachedExit = false; // 檢查是否到達出口
    // 建構函數：設定初始位置和方向
    Player(double startX, double startY, double startDirX, double startDirY);
    // 處理滑鼠旋轉邏輯： deltaX 是滑鼠的水平位移 <--- [新增]
    void rotateMouse(double deltaX, double sensitivity);
    // 處理側向平移邏輯 <--- [新增]
    void strafe(double frameTime, bool right, double speedFactor);
    // 處理移動邏輯: 新增 speedFactor 參數 <--- [修改]
    void move(double frameTime, bool forward, double speedFactor);
    // 處理旋轉邏輯
    void rotate(double frameTime, bool right);
    // 檢查玩家是否到達出口
    bool checkExit(int mapValue);
};

#endif // PLAYER_H