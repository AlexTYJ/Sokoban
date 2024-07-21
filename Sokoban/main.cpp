/*
路                                 0
墙壁                               1
目的地                             3
箱子                               4
箱子到达目的地                     3+4=7
*/
#include<stdio.h>
#include <cstdlib>
#include<time.h>
#include<queue>
#include <vector>
#include<windows.h>
#include <conio.h>//控制台输入输出头文件
#include <stdlib.h>//标准库头文件
#include <graphics.h>//加载图形库，图片显示要用到
#include <mmsystem.h>//多媒体 API 实现声音输出
#include<iostream>
#pragma comment(lib,"winmm.lib")  //静态库资源
IMAGE img[6];		//6张图片，6个名字
void loadResource()
{
    loadimage(img + 0, "0.bmp", 50, 50);//50表示图片长宽，可以自己调的
    loadimage(img + 1, "1.bmp", 50, 50);
    loadimage(img + 2, "2.bmp", 50, 50);
    loadimage(img + 3, "3.bmp", 50, 50);
    loadimage(img + 4, "4.bmp", 50, 50);
    loadimage(img + 5, "5.bmp", 50, 50);
}
using std::queue;
using std::vector;
const int maxm = 8;//maxm行
const int maxn = 9;//maxn列
const int levelnum = 20;//levelnum关
const int max_box = 25;//最多25个箱子
const int p = 1919081;
//char pic[9]={' ','■','?','☆','○','♀','?','★','♀'};
char pic[9] = { ' ','W','?','X','B','I','?','B','I' };

int Random(int start, int end) {
    int dis = end - start;
    return rand() % dis + start;
}

int m_init[levelnum][maxm][maxn] = {
1,1,1,1,1,1,1,1,1,//1
1,3,0,4,0,3,1,1,1,
1,0,4,0,4,0,1,1,1,
1,3,0,4,0,3,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//2
1,0,0,3,3,4,0,1,1,
1,0,4,0,0,4,0,1,1,
1,0,4,3,3,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//3
1,1,0,0,0,0,0,1,1,
1,1,0,3,4,3,0,1,1,
1,0,0,4,0,4,0,1,1,
1,0,0,3,4,3,0,1,1,
1,1,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//4
1,1,0,0,0,1,1,1,1,
1,0,0,1,0,0,0,1,1,
1,0,1,0,0,1,0,1,1,
1,0,4,0,4,1,0,1,1,
1,3,1,0,0,0,0,1,1,
1,3,0,0,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//5
1,0,4,3,0,4,3,1,1,
1,0,3,4,0,4,3,1,1,
1,0,4,3,0,4,3,1,1,
1,0,3,4,0,4,3,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//6
1,0,0,0,7,0,0,0,1,
1,0,4,3,1,3,4,0,1,
1,0,0,4,3,4,0,0,1,
1,1,1,3,4,3,1,1,1,
1,1,1,1,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//7
1,1,1,0,0,0,1,1,1,
1,0,0,3,4,3,1,1,1,
1,0,0,4,0,4,1,1,1,
1,1,1,3,4,3,1,1,1,
1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//8
1,0,0,0,0,0,0,1,1,
1,0,4,3,3,0,0,1,1,
1,1,1,3,3,1,0,0,1,
1,1,1,1,4,4,4,0,1,
1,1,1,1,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//9
1,0,0,0,0,0,0,0,1,
1,0,4,4,1,0,0,1,1,
1,1,0,0,4,0,1,1,1,
1,1,3,4,0,0,1,1,1,
1,1,1,3,0,1,1,1,1,
1,1,1,3,3,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//10
1,1,3,0,3,3,1,1,1,
1,1,3,0,4,3,1,1,1,
1,1,1,0,0,4,1,1,1,
1,0,4,0,0,4,0,1,1,
1,0,1,4,1,1,0,1,1,
1,0,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,


1,1,1,1,1,1,1,1,1,//11
1,1,1,0,3,0,0,1,1,
1,1,0,4,0,4,0,1,1,
1,1,3,0,7,0,3,1,1,
1,1,0,4,0,4,0,1,1,
1,1,1,0,3,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//12
1,1,1,0,0,0,0,1,1,
1,1,1,4,4,4,0,1,1,
1,0,0,4,3,3,0,1,1,
1,0,4,3,3,3,1,1,1,
1,1,1,1,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//13
1,0,0,1,0,0,0,1,1,
1,0,4,3,3,4,0,1,1,
1,0,4,3,7,0,1,1,1,
1,0,4,3,3,4,0,1,1,
1,0,0,4,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//14
1,1,0,0,0,1,1,1,1,
1,1,4,1,0,0,3,1,1,
1,0,0,4,4,1,3,1,1,
1,0,1,4,0,0,3,1,1,
1,0,0,0,0,0,3,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//15
1,1,0,0,0,0,1,1,1,
1,0,4,0,4,4,0,1,1,
1,3,3,3,3,3,3,1,1,
1,0,4,4,0,4,0,1,1,
1,1,1,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//16
1,1,3,0,3,0,3,1,1,
1,1,0,4,4,4,0,1,1,
1,1,3,4,0,4,3,1,1,
1,1,0,4,4,4,0,1,1,
1,1,3,0,3,0,3,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//17
1,0,0,0,7,0,0,0,1,
1,0,4,0,3,4,0,0,1,
1,3,7,7,7,3,3,7,1,
1,0,4,0,7,0,4,0,1,
1,0,0,0,7,0,0,0,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//18
1,1,1,0,0,0,1,1,1,
1,0,4,3,4,3,4,0,1,
1,0,0,3,7,3,0,0,1,
1,0,4,3,4,3,4,0,1,
1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//19
1,1,0,0,3,0,0,1,1,
1,1,4,4,3,4,4,1,1,
1,0,3,3,7,3,3,0,1,
1,0,4,4,3,4,4,0,1,
1,0,0,0,3,0,0,0,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//20
1,1,0,0,0,0,1,1,1,
1,1,4,4,7,0,0,1,1,
1,1,3,3,0,3,3,1,1,
1,1,0,0,7,4,4,1,1,
1,1,1,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1

/*1,1,1,1,1,1,1,1,1,//debug关卡
1,1,0,0,0,0,0,1,1,
1,1,0,3,0,7,0,1,1,
1,0,0,4,0,4,0,1,1,
1,0,0,3,4,3,0,1,1,
1,1,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,*/
};

int map[levelnum][maxm][maxn] = {
1,1,1,1,1,1,1,1,1,//1
1,3,0,4,0,3,1,1,1,
1,0,4,0,4,0,1,1,1,
1,3,0,4,0,3,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//2
1,0,0,3,3,4,0,1,1,
1,0,4,0,0,4,0,1,1,
1,0,4,3,3,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//3
1,1,0,0,0,0,0,1,1,
1,1,0,3,4,3,0,1,1,
1,0,0,4,0,4,0,1,1,
1,0,0,3,4,3,0,1,1,
1,1,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//4
1,1,0,0,0,1,1,1,1,
1,0,0,1,0,0,0,1,1,
1,0,1,0,0,1,0,1,1,
1,0,4,0,4,1,0,1,1,
1,3,1,0,0,0,0,1,1,
1,3,0,0,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//5
1,0,4,3,0,4,3,1,1,
1,0,3,4,0,4,3,1,1,
1,0,4,3,0,4,3,1,1,
1,0,3,4,0,4,3,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//6
1,0,0,0,7,0,0,0,1,
1,0,4,3,1,3,4,0,1,
1,0,0,4,3,4,0,0,1,
1,1,1,3,4,3,1,1,1,
1,1,1,1,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//7
1,1,1,0,0,0,1,1,1,
1,0,0,3,4,3,1,1,1,
1,0,0,4,0,4,1,1,1,
1,1,1,3,4,3,1,1,1,
1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//8
1,0,0,0,0,0,0,1,1,
1,0,4,3,3,0,0,1,1,
1,1,1,3,3,1,0,0,1,
1,1,1,1,4,4,4,0,1,
1,1,1,1,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//9
1,0,0,0,0,0,0,0,1,
1,0,4,4,1,0,0,1,1,
1,1,0,0,4,0,1,1,1,
1,1,3,4,0,0,1,1,1,
1,1,1,3,0,1,1,1,1,
1,1,1,3,3,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//10
1,1,3,0,3,3,1,1,1,
1,1,3,0,4,3,1,1,1,
1,1,1,0,0,4,1,1,1,
1,0,4,0,0,4,0,1,1,
1,0,1,4,1,1,0,1,1,
1,0,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,


1,1,1,1,1,1,1,1,1,//11
1,1,1,0,3,0,0,1,1,
1,1,0,4,0,4,0,1,1,
1,1,3,0,7,0,3,1,1,
1,1,0,4,0,4,0,1,1,
1,1,1,0,3,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//12
1,1,1,0,0,0,0,1,1,
1,1,1,4,4,4,0,1,1,
1,0,0,4,3,3,0,1,1,
1,0,4,3,3,3,1,1,1,
1,1,1,1,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//13
1,0,0,1,0,0,0,1,1,
1,0,4,3,3,4,0,1,1,
1,0,4,3,7,0,1,1,1,
1,0,4,3,3,4,0,1,1,
1,0,0,1,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//14
1,1,0,0,0,1,1,1,1,
1,1,4,1,0,0,3,1,1,
1,0,0,4,4,1,3,1,1,
1,0,1,4,0,0,3,1,1,
1,0,0,0,0,0,3,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//15
1,1,0,0,0,0,1,1,1,
1,0,4,0,4,4,0,1,1,
1,3,3,3,3,3,3,1,1,
1,0,4,4,0,4,0,1,1,
1,1,1,0,0,1,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//16
1,1,3,0,3,0,3,1,1,
1,1,0,4,4,4,0,1,1,
1,1,3,4,0,4,3,1,1,
1,1,0,4,4,4,0,1,1,
1,1,3,0,3,0,3,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//17
1,0,0,0,7,0,0,0,1,
1,0,4,0,3,4,0,0,1,
1,3,7,7,7,3,3,7,1,
1,0,4,0,7,0,4,0,1,
1,0,0,0,7,0,0,0,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//18
1,1,1,0,0,0,1,1,1,
1,0,4,3,4,3,4,0,1,
1,0,0,3,7,3,0,0,1,
1,0,4,3,4,3,4,0,1,
1,1,1,0,0,0,1,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//19
1,1,0,0,3,0,0,1,1,
1,1,4,4,3,4,4,1,1,
1,0,3,3,7,3,3,0,1,
1,0,4,4,3,4,4,0,1,
1,0,0,0,3,0,0,0,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1,1,//20
1,1,0,0,0,0,1,1,1,
1,1,4,4,7,0,0,1,1,
1,1,3,3,0,3,3,1,1,
1,1,0,0,7,4,4,1,1,
1,1,1,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1

/*1,1,1,1,1,1,1,1,1,//debug关卡
1,1,0,0,0,0,0,1,1,
1,1,0,3,0,7,0,1,1,
1,0,0,4,0,0,0,1,1,
1,0,4,3,0,7,0,1,1,
1,1,0,0,0,0,0,1,1,
1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,*/
};


int person_x[levelnum] = { 2,2,3,6,2,5,2,1,1,6,1,3,3,2,5,3,4,3,1,3 };
int person_y[levelnum] = { 3,3,1,3,1,4,1,5,4,4,6,1,1,4,4,4,3,7,6,4 };

int boxnum[levelnum];

struct MOVE {
    int x, y;
    int direction;
}ans[1000];

struct Node {
    int x, y, level;
    Node(int _x, int _y) : x(_x), y(_y) {}
};

int step;
int win(int map[levelnum][maxm][maxn], int level) {//胜利判断：当期仅当有箱子不在正确的位置上是失败
    int flag = 1;
    for (int i = 0; i < maxm; i++) {
        for (int j = 0; j < maxn; j++) {
            if (map[level][i][j] == 4) {
                flag = 0;
                break;
            }
        }
    }
    return flag;
}

int permutation[24][4] = {
    0,1,2,3,
    0,1,3,2,
    0,2,1,3,
    0,2,3,1,
    0,3,1,2,
    0,3,2,1,
    1,0,2,3,
    1,0,3,2,
    1,2,0,3,
    1,2,3,0,
    1,3,0,2,
    1,3,2,0,
    2,0,1,3,
    2,0,3,1,
    2,1,0,3,
    2,1,3,0,
    2,3,0,1,
    2,3,1,0,
    3,0,1,2,
    3,0,2,1,
    3,1,0,2,
    3,1,2,0,
    3,2,0,1,
    3,2,1,0 };

int flag[maxm][maxn];
int h[p + 1];
int direction_x[4] = { -1,1,0,0 };
int direction_y[4] = { 0,0,-1,1 };
int connect(int map[levelnum][maxm][maxn], int x_1, int y_1, int x_2, int y_2, int level) {  //(x_1,y_1)与(x_2,y_2)是否是可达的
    if ((x_1 == -1) && (y_1 == -1)) return 0;
    if ((x_2 == -1) && (y_2 == -1)) return 0;
    if (map[level][x_1][y_1] == 1 || map[level][x_1][y_1] == 4 || map[level][x_1][y_1] == 7) return 0;
    if (map[level][x_2][y_2] == 1 || map[level][x_2][y_2] == 4 || map[level][x_2][y_2] == 7) return 0;
    if (x_1 == x_2 && y_1 == y_2) return 1;
    for (int d = 0; d <= 3; d++) {
        if (map[level][x_1 + direction_x[d]][y_1 + direction_y[d]] != 1 && map[level][x_1 + direction_x[d]][y_1 + direction_y[d]] != 4 && map[level][x_1 + direction_x[d]][y_1 + direction_y[d]] != 7 && !flag[x_1 + direction_x[d]][y_1 + direction_y[d]]) {
            flag[x_1 + direction_x[d]][y_1 + direction_y[d]] = 1;
            if (connect(map, x_1 + direction_x[d], y_1 + direction_y[d], x_2, y_2, level)) return 1;
            else flag[x_1 + direction_x[d]][y_1 + direction_y[d]] = 0;
        }
    }
    return 0;
}

void init_flag() {
    for (int i = 0; i < maxm; i++)
        for (int j = 0; j < maxn; j++)
            flag[i][j] = 0;
}

int legal(int i, int j, int x, int y, int d, int level) {//(x,y)为人的位置，(i,j)为箱子位置，d为方向，上下左右为0123
    if (map[level][i + direction_x[d]][j + direction_y[d]] == 1 || map[level][i + direction_x[d]][j + direction_y[d]] == 4 || map[level][i + direction_x[d]][j + direction_y[d]] == 7) return 0;
    if (map[level][i - direction_x[d]][j - direction_y[d]] == 1 || map[level][i - direction_x[d]][j - direction_y[d]] == 4 || map[level][i - direction_x[d]][j - direction_y[d]] == 7) return 0;
    init_flag();
    if (!connect(map, x, y, i - direction_x[d], j - direction_y[d], level)) return 0;
    else {
        //printf("(%d,%d)and(%d,%d)is connected\n",x,y,i+direction_x[d],j+direction_y[d]);
        //getchar();
    }
    return 1;
}

int hash(int map[levelnum][maxm][maxn], int level, int x, int y) {
    int ans = 0;
    int represent_x = -1, represent_y = -1;
    for (int i = 0; i < maxm && represent_x == -1; i++) {
        for (int j = 0; j < maxm && represent_x == -1; j++) {
            init_flag();
            if (connect(map, i, j, x, y, level)) {
                represent_x = i;
                represent_y = j;
                break;
            }
        }
    }
    //print_map(map,level,x,y);
    //printf("represent(%d,%d)\n",represent_x,represent_y);
    //getchar();
    for (int i = 0; i < maxm; i++) {
        for (int j = 0; j < maxn; j++) {
            if (map[level][i][j] == 4 || map[level][i][j] == 7) ans = (ans * 3 + 1) % p;
            else if (i == represent_x && j == represent_y) ans = (ans * 3 + 2) % p;
            else ans = (ans * 3) % p;
            //printf("%d\n",ans);
        }
    }
    return ans;
}

void print_map(int map[levelnum][maxm][maxn], int level, int x, int y) {//(x,y)为人的坐标，由于地图中没有人，所以要单独处理
    //system("cls");
    for (int i = 0; i < maxm; i++) {
        for (int j = 0; j < maxn; j++) {
            //if ((i == x) && (j == y)) printf("I");
            //else printf("%c", pic[map[level][i][j]]);
            if ((i == x) && (j == y)) {
                putimage(50*y, 50*x, img + 2);
            }
            else {
                int x_begin = 50 * j;
                int y_begin = 50 * i;
                switch (map[level][i][j])
                {
                case 0:
                    putimage(x_begin, y_begin, img + 0);
                    break;
                case 1:
                    putimage(x_begin, y_begin, img + 1);
                    break;
                case 3:
                    putimage(x_begin, y_begin, img + 3);
                    break;
                case 4:
                    putimage(x_begin, y_begin, img + 4);
                    break;
                case 7:
                    putimage(x_begin, y_begin, img + 5);
                    break;
                }
            }
        }
        //printf("\n");
    }
    //printf("hash:%d\n",hash(map,level,x,y));
    //printf("-----------------\n");
    getchar();
}

int cut(int map[levelnum][maxm][maxn], int level, int x_box, int y_box) {
    if (map[level][x_box][y_box] == 4) {
        if (map[level][x_box - 1][y_box] == 1 && map[level][x_box][y_box - 1] == 1) return 1;
        if (map[level][x_box - 1][y_box] == 1 && map[level][x_box][y_box + 1] == 1) return 1;
        if (map[level][x_box + 1][y_box] == 1 && map[level][x_box][y_box - 1] == 1) return 1;
        if (map[level][x_box + 1][y_box] == 1 && map[level][x_box][y_box + 1] == 1) return 1;//返回1表示不可能，要剪枝
    }

    return 0;
}

int dfs(int map[levelnum][maxm][maxn], int level, int x, int y, int depth, int maxdepth) {
    //print_map();
    if (win(map, level)) {
        return 1;
    }
    if (depth >= maxdepth) return 0;
    for (int i = 0; i < maxm; i++) {
        for (int j = 0; j <= maxn; j++) {
            if (map[level][i][j] == 4 || map[level][i][j] == 7) {//(i,j)是箱子
                int index = Random(0, 4);
                for (int k = 0; k <= 3; k++) {
                    int d = permutation[index][k];
                    if (legal(i, j, x, y, d, level)) {
                        map[level][i + direction_x[d]][j + direction_y[d]] += 4;
                        map[level][i][j] -= 4;
                        int h_new = hash(map, level, i, j);
                        /*if (h_new==hash(map,20,3,1)){
                            printf("searched\n");
                            print_map(map,level,x,y);
                            printf("(x,y)=(%d,%d),depth=%d\n",x,y,depth);
                            getchar();

                        }*/
                        if ((h[h_new] == 0) && (!cut(map, level, i + direction_x[d], j + direction_y[d]))) {
                            //printf("(%d,%d)move%d\n",i,j,d);
                            //print_map(map,level,x,y);
                            //getchar();
                            h[h_new] = 1;
                            if (dfs(map, level, i, j, depth + 1, maxdepth)) {
                                ans[++step].x = i;
                                ans[step].y = j;
                                ans[step].direction = d;
                                return 1;
                            }
                        }
                        map[level][i + direction_x[d]][j + direction_y[d]] -= 4;
                        map[level][i][j] += 4;
                    }
                }
            }
        }
    }
    return 0;
}

void print_way(int map[levelnum][maxm][maxn], int level, int x_start, int y_start, int x_end, int y_end) {

    bool visited[levelnum][maxm][maxn] = { false };
    queue<Node> q;
    vector<Node> path[levelnum][maxm][maxn];

    q.push(Node(x_start, y_start));
    visited[level][x_start][y_start] = true;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();
        //print_map(map, level, current.x, current.y);

        if (current.x == x_end && current.y == y_end) {
            vector<Node> result = path[level][x_end][y_end];
            for (Node node : result) {
                print_map(map, level, node.x, node.y);
                //printf("Node: (%d, %d)\n", node.x, node.y);
            }
            print_map(map, level, current.x, current.y);
            return;
        }

        for (int i = 0; i < 4; ++i) {
            int newX = current.x + direction_x[i];
            int newY = current.y + direction_y[i];

            if ((map[level][newX][newY] == 0 || map[level][newX][newY] == 3) && !visited[level][newX][newY]) {
                q.push(Node(newX, newY));
                visited[level][newX][newY] = true;
                path[level][newX][newY] = path[level][current.x][current.y];
                path[level][newX][newY].push_back(current);
            }
        }
    }
}


int main() {
    int mode;
    printf("print 1 to begin normal mode, print 2 to begin random-generat-map mode,press enter to continue in the game\n");
    std::cin >> mode;
    //mciSendString("open 1.mp3", 0, 0, 0);//加载音乐
    //mciSendString("play 1.mp3 repeat", 0, 0, 0);//循环播放音乐资源
    if (mode == 1) {
        loadResource();
        initgraph(50 * 8, 50 * 9);
        for (int l = 0; l < levelnum; l++) {
            //printf("level=%d\n", l + 1);
            print_map(map, l, person_x[l], person_y[l]);
            //getchar();
            int if_win = 0;
            for (int maxd = 5; maxd <= 150 && if_win == 0; maxd += 2) {
                //printf("maxdepth=%d\n",maxd);
                //getchar();
                step = -1;
                for (int i = 0; i <= p; i++) {
                    h[i] = 0;
                }
                h[hash(map, l, person_x[l], person_y[l])] = 1;
                if (dfs(map, l, person_x[l], person_y[l], 0, maxd)) {
                    if_win = 1;
                    //printf("win\n");
                    //printf("step=%d\n,press enter to see the path", step);
                    //getchar();
                    ans[step + 1].x = person_x[l];
                    ans[step + 1].y = person_y[l];
                    while (step != -1) {
                        // printf("print way from (%d,%d)to(%d,%d)\n",ans[step+1].x,ans[step+1].y,ans[step].x,ans[step].y);
                         //getchar();
                        print_way(m_init, l, ans[step + 1].x, ans[step + 1].y, ans[step].x - direction_x[ans[step].direction], ans[step].y - +direction_y[ans[step].direction]);
                        //printf("position(%d,%d) ,direction:%d\n",ans[step].x,ans[step].y,ans[step].direction);
                        m_init[l][ans[step].x][ans[step].y] -= 4;
                        m_init[l][ans[step].x + direction_x[ans[step].direction]][ans[step].y + direction_y[ans[step].direction]] += 4;
                        //print_map(m_init,l,ans[step].x,ans[step].y);
                        //printf("printed\n");
                        //getchar();
                        step--;
                    }
                    print_map(map, l, ans[0].x, ans[0].y);
                }
            }
            if (!if_win) {
                printf("lose\n");
                //getchar();
            }
        }
    }
    else {
        int height, width, box_num;
        printf("please type height(3-7), width(3-7) and box_num(1-4), using space between them\n");
        std::cin >> height >> width >> box_num;
        loadResource();
        initgraph(50 * 8, 50 * 9);
        while (1) {
            for (int i = 0; i < maxm; i++) {
                for (int j = 0; j < maxn; j++) {
                    map[0][i][j] = 0;
                }
            }
            for (int i = 0; i < height; i++)  map[0][i][0] = 1;
            for (int j = 0; j < width; j++) map[0][0][j] = 1;
            for (int i = height - 1; i < maxm; i++)
                for (int j = 0; j < maxn; j++)
                    map[0][i][j] = 1;
            for (int j = width - 1; j < maxn; j++)
                for (int i = 0; i < maxm; i++)
                    map[0][i][j] = 1;
            for (int i = 0; i < box_num; i++) {
                int pos_x = Random(1, height - 1);
                int pos_y = Random(1, width - 1);
                if (map[0][pos_x][pos_y] == 4) i--;
                else map[0][pos_x][pos_y] = 4;
            }
            for (int i = 0; i < box_num; i++) {
                int pos_x = Random(1, height - 1);
                int pos_y = Random(1, width - 1);
                if ((map[0][pos_x][pos_y] % 4) == 3) i--;
                else map[0][pos_x][pos_y] += 3;
            }
            int p_x = Random(1, height - 1);
            int p_y = Random(1, width - 1);
            while (map[0][p_x][p_y] != 0) {
                p_x = Random(1, height - 1);
                p_y = Random(1, width - 1);
            }
            for (int i = 0; i < maxm; i++) {
                for (int j = 0; j < maxn; j++) {
                    m_init[0][i][j] = map[0][i][j];
                }
            }
            //print_map(random_init_map, 0, p_x, p_y);
            int if_win = 0;
            for (int maxd = 5; maxd <= 150 && if_win == 0; maxd += 2) {
                //printf("maxdepth=%d\n",maxd);
                //getchar();
                
                step = -1;
                for (int i = 0; i <= p; i++) {
                    h[i] = 0;
                }
                h[hash(map, 0, p_x, p_y)] = 1;
                if (dfs(map, 0, p_x, p_y, 0, maxd)) {
                    if_win = 1;
                    print_map(m_init, 0, p_x, p_y);
                    ans[step + 1].x = p_x;
                    ans[step + 1].y = p_y;
                    while (step != -1) {
                        // printf("print way from (%d,%d)to(%d,%d)\n",ans[step+1].x,ans[step+1].y,ans[step].x,ans[step].y);
                         //getchar();
                        print_way(m_init, 0, ans[step + 1].x, ans[step + 1].y, ans[step].x - direction_x[ans[step].direction], ans[step].y - direction_y[ans[step].direction]);
                        //printf("position(%d,%d) ,direction:%d\n",ans[step].x,ans[step].y,ans[step].direction);
                        m_init[0][ans[step].x][ans[step].y] -= 4;
                        m_init[0][ans[step].x + direction_x[ans[step].direction]][ans[step].y + direction_y[ans[step].direction]] += 4;
                        //print_map(m_init,l,ans[step].x,ans[step].y);
                        //printf("printed\n");
                        //getchar();
                        step--;
                    }
                    print_map(map, 0, ans[0].x, ans[0].y);
                }
            }
            //getchar();
           // closegraph();//关闭窗口
        }
    }
}