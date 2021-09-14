#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

const int DEFAULT_SIZE = 4;
int fill_list[11] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
int N;

class GAME2048 {
    private:
        int chess_board[DEFAULT_SIZE][DEFAULT_SIZE];
        // int temp_chess_board[DEFAULT_SIZE][DEFAULT_SIZE];
        int count_empty;
        int score;

    public:
        GAME2048() { // 构建一个空棋盘，并向其中添加两个2
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j) 
                    chess_board[i][j] = 0;
            count_empty = N * N;
            srand((unsigned)time(NULL));

            add(0);
            add(0);

            show();
        } 
        void show();
        void add(int type); // 随机增加一个数字, type=-1表示随机，=0表示第一个，=1表示第二个。
        void gameover();// 游戏结束
        void up();
        void down();
        void left();
        void right();
};

void GAME2048::show() {
    cout << "=====================" << endl;
    cout << "---------------------" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) 
            cout << "|" << setw(4) << chess_board[i][j];
        cout << "|" << endl;
        cout << "---------------------" << endl;
    }
    cout << "=====================" << endl;
}

void GAME2048::add(int type=-1) {
    if (count_empty <= 0) {
        gameover();
        return;
    }
    int x = rand() % N, y = rand() % N, fill_type;
    fill_type = type==-1? (rand() % 2): type;
    while(chess_board[x][y]) {
        x = rand() % N, y = rand() % N;
    }
    chess_board[x][y] = fill_list[fill_type];
    count_empty--;
}

void GAME2048::up() {
    count_empty = N * N;
    for (int j = 0; j < N; ++j)  {
        int temp[DEFAULT_SIZE];
        int cnt_temp = 0;
        // 自下而上取出非空位
        for (int i = N - 1; i >= 0; --i)
            if (temp_chess_board[i][j])
                temp[cnt_temp++] = chess_board[i][j];
        // 两两合并
        for (int k = 0; k < cnt_temp; ++k) 
            if (temp[k] == temp[k + 1]) {
                temp[k + 1] *= 2;
                ++k; // 如果合并后，不能连续合并
            }
        // 合并后，从上到下填充
        int t = 0;
        for (int k = cnt_temp - 1; k >= 0; --k) 
            if (temp[k]) {
                chess_board[t][j] = temp[k];
                ++count_empty;
            }
        for(;t < N; ++t) 
            chess_board[t][j] = 0;
    }   
    // show();
}

void GAME2048::down() {
    count_empty = N * N;
    for (int j = 0; j < N; ++j)  {
        int temp[DEFAULT_SIZE];
        int cnt_temp = 0;
        // 自上而下取出非空位
        for (int i = 0; i < N; ++i)
            if (temp_chess_board[i][j])
                temp[cnt_temp++] = chess_board[i][j];
        // 两两合并
        for (int k = 0; k < cnt_temp; ++k) 
            if (temp[k] == temp[k + 1]) {
                temp[k + 1] *= 2;
                ++k; // 如果合并后，不能连续合并
            }
        // 合并后，从下到上填充
        int t = N - 1;
        for (int k = cnt_temp - 1; k >= 0; --k) 
            if (temp[k]) {
                chess_board[t][j] = temp[k];
                ++count_empty;
            }
        for(;t >= 0; --t) 
            chess_board[t][j] = 0;
    }   
    // show();
}

void GAME2048::left() {

}

void GAME2048::right() {

}

void GAME2048::gameover() {
    
}

int main() {
    N = DEFAULT_SIZE;
    GAME2048 game;
    return 0;
}
