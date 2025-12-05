#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <vector>
#include <SDL2/SDL.h>

// --- 專案參數設定 ---
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double PI = 3.14159265358979323846;
// ...existing code...
const int MAP_EXIT = 6;        // 出口標記
const double EXIT_CHECK_DISTANCE = 0.5; // 靠近出口的距離閾值
// ...existing code...
// 地圖設定
const int MAP_WIDTH = 24;
const int MAP_HEIGHT = 24;

// 速度
const double MOVE_SPEED = 0.05; 
const double ROT_SPEED = 0.03;  
const double MOUSE_SENSITIVITY = 0.003; // 滑鼠靈敏度 <--- [新增]
const double SPRINT_FACTOR = 2.0; // 加速倍率 (兩倍速度) <--- [新增]


// 外部宣告地圖（實體在 RayCaster.cpp 中定義）
extern int worldMap[MAP_WIDTH][MAP_HEIGHT];

// 顏色定義 (ARGB 格式)
Uint32 getColor(int wallType, int side);

#endif // CONSTANTS_H