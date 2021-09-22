#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <conio.h>

using namespace std;

/* 
    可以自定义的项：
        1. 棋盘大小 4*4 ==> N*M（命令行输入）
        2. 填充的内容 2^1...2^10 ==> 自定义内容和长度（从文件导入）
        3. 游戏终止的条件 2048出现 ==> 第几个长度的数字出现的时候（从文件导入）
        4. - ==> 统计分数
        5. - ==> 连击奖励（连续多次操作都有合并）
        6. - ==> 多消奖励（单次操作连续消除了多少个）
        7. - ==> 记录操作数（将操作序列导出到文件中）
        8. - ==> 随机运行x步（命令行输入x）
*/

const int DEFAULT_SIZE = 4;
int fill_list[11] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
int N;

void delay_time(double t)
{
	clock_t start = clock();
	clock_t delay = (clock_t)t * CLOCKS_PER_SEC;
	while (clock() - start < delay)
		continue;
}

class GAME2048 { 
    private:
        int chess_board[DEFAULT_SIZE][DEFAULT_SIZE];
        int count_empty;
        int successive_elimination;
        int score;
        bool game_status;

    public:
        GAME2048() { // 构建一个空棋盘，并向其中添加两个2
            // for (int i = 0; i < N; ++i)
            //     for (int j = 0; j < N; ++j) 
            //         chess_board[i][j] = 0;
            // count_empty = N * N;
            // srand((unsigned)time(NULL));

            // add(0);
            // add(0);

            // show();
            init();
        } 
        void init();
        void show();
        void add(int type); // 随机增加一个数字, type=-1表示随机，=0表示第一个，=1表示第二个。
        void gameover(int type);// 游戏结束, type=-1表示异常，type=0表示失败，type=1表示成功
        void restart();
        void up();
        void down();
        void left();
        void right();
        bool check(int level); // 判断是否出现level x的结果，出现认为游戏结束。默认为level=10
};

void GAME2048::init() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) 
            chess_board[i][j] = 0;
    count_empty = N * N;
    game_status = 1;
    srand((unsigned)time(NULL));

    add(0);
    add(0);

    show();
}

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
        count_empty == 0? gameover(0): gameover(-1);
        game_status = 0;
        return;
    }
    int x = rand() % N, y = rand() % N, fill_type;
    fill_type = type==-1? (rand() % 2): type;
    while(chess_board[x][y]) {
        x = rand() % N, y = rand() % N;
    }
    chess_board[x][y] = fill_list[fill_type];
    --count_empty;
}

void GAME2048::up() {
    count_empty = N * N;
    for (int j = 0; j < N; ++j)  {
        int temp[DEFAULT_SIZE];
        int cnt_temp = 0;
        // 从下到上取出非空位
        for (int i = N - 1; i >= 0; --i)
            if (chess_board[i][j])
                temp[cnt_temp++] = chess_board[i][j];
        // 两两合并
        for (int k = 0; k < cnt_temp - 1; ++k) 
            if (temp[k] && temp[k] == temp[k + 1]) {
                temp[k + 1] *= 2;
                temp[k] = 0;
                ++k; // 如果合并后，不能连续合并
            }
        // 合并后，从上到下填充
        int t = 0;
        for (int k = cnt_temp - 1; k >= 0; --k) 
            if (temp[k]) {
                chess_board[t][j] = temp[k];
                ++t;
                --count_empty;
            }
        for(;t < N; ++t) 
            chess_board[t][j] = 0;
    }   
    add(-1);
}

void GAME2048::down() {
    count_empty = N * N;
    for (int j = 0; j < N; ++j)  {
        int temp[DEFAULT_SIZE];
        int cnt_temp = 0;
        // 从上到下取出非空位
        for (int i = 0; i < N; ++i)
            if (chess_board[i][j])
                temp[cnt_temp++] = chess_board[i][j];
        // 两两合并
        for (int k = 0; k < cnt_temp - 1; ++k) 
            if (temp[k] && temp[k] == temp[k + 1]) {
                temp[k + 1] *= 2;
                temp[k] = 0;
                ++k; // 如果合并后，不能连续合并
            }
        // 合并后，从下到上填充
        int t = N - 1;
        for (int k = cnt_temp - 1; k >= 0; --k) 
            if (temp[k]) {
                chess_board[t][j] = temp[k];
                --t;
                --count_empty;
            }
        for(;t >= 0; --t) 
            chess_board[t][j] = 0;
    }   
    add(-1);
}

void GAME2048::left() {
    count_empty = N * N;
    for (int i = 0; i < N; ++i)  {
        int temp[DEFAULT_SIZE];
        int cnt_temp = 0;
        // 从右到左取出非空位
        for (int j = N - 1; j >= 0; --j)
            if (chess_board[i][j])
                temp[cnt_temp++] = chess_board[i][j];
        // 两两合并
        for (int k = 0; k < cnt_temp - 1; ++k) 
            if (temp[k] && temp[k] == temp[k + 1]) {
                temp[k + 1] *= 2;
                temp[k] = 0;
                ++k; // 如果合并后，不能连续合并
            }
        // 合并后，从左到右填充
        int t = 0;
        for (int k = cnt_temp - 1; k >= 0; --k) 
            if (temp[k]) {
                chess_board[i][t] = temp[k];
                ++t;
                --count_empty;
            }
        for(;t < N; ++t) 
            chess_board[i][t] = 0;
    }   
    add(-1);
}

void GAME2048::right() {
    count_empty = N * N;
    for (int i = 0; i < N; ++i)  {
        int temp[DEFAULT_SIZE];
        int cnt_temp = 0;
        // 从左到右取出非空位
        for (int j = 0; j < N; ++j)
            if (chess_board[i][j])
                temp[cnt_temp++] = chess_board[i][j];
        // 两两合并
        for (int k = 0; k < cnt_temp - 1; ++k) 
            if (temp[k] && temp[k] == temp[k + 1]) {
                temp[k + 1] *= 2;
                temp[k] = 0;
                ++k; // 如果合并后，不能连续合并
            }
        // 合并后，从右到左填充
        int t = N - 1;
        for (int k = cnt_temp - 1; k >= 0; --k) 
            if (temp[k]) {
                chess_board[i][t] = temp[k];
                --t;
                --count_empty;
            }
        for(;t >= 0; --t) 
            chess_board[i][t] = 0;
    }   
    add(-1);
}

bool GAME2048::check(int level=10) { // 判断是否可继续运行，可以为true，不可以为false，并保证不可以时跳到gameover.
    if (count_empty < 0) {
        gameover(-1);
        return 0;
    }

    if (game_status == 0) {
        gameover(0);
        return 0;
    }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (chess_board[i][j] == fill_list[level]) {
                gameover(1);
                return 0;
            }

    bool flag = false;
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j) {
            if (i < N - 1 && chess_board[i][j] == chess_board[i + 1][j])
                flag = true;
            if (j < N - 1 && chess_board[i][j] == chess_board[i][j + 1])
                flag = true;
        }
    
    if (!flag) {
        gameover(0);
        return 0;
    }
    return 1;   
}

void GAME2048::gameover(int type=-1) {  // type=-1表示异常，type=0表示失败，type=1表示成功，
    if (type == -1) {
        cout << "=====================" << endl;

        cout << "游戏异常结束，请检查代码或操作" << endl;
        cout << "=====================" << endl;
    }
    else if (type == 0) {
        cout << "=====================" << endl;
        cout << "游戏失败，请重新尝试" << endl;
        cout << "=====================" << endl;
    }
    else if (type == 1) {
        cout << "=====================" << endl;
        cout << "恭喜你成功通关，合成出了2048" << endl; 
        cout << "=====================" << endl;
    }
}

void GAME2048::restart() {
    system("cls");
    init();
}

int main() {
    N = DEFAULT_SIZE;
    cout << "是否开始游戏，请输入[Y/N]" << endl;
    char input_operation;
    while(input_operation = getch()) {
        input_operation = toupper(input_operation);
        if (input_operation == 'N') {
            cout << "退出游戏" << endl;
            delay_time(3);
            exit(0);
        }
        else if (input_operation == 'Y') {
            cout << "游戏开始！按H进入帮助界面" << endl;
            delay_time(1);
            break;
        }
        else {
            cout << "请输入[Y/N]" << endl;
        }
    }
    system("cls");
    GAME2048 game;
    delay_time(1);
    
    while(true) {
        input_operation = getch();
        if (input_operation == '\n' || input_operation == ' ')
            continue;
        
        input_operation = toupper(input_operation); //输入转化为大写
        // cout << "input: " << input_operation << endl;
        // cout << "xx: " << input_operation << endl;
        bool is_direction_input = true;
        switch (input_operation)
		{
            case 'W':
                game.up();
                break;
            case 'A':
                game.left();
                break;
            case 'S':
                game.down();
                break;
            case 'D':
                game.right();
                break;
            default:
                is_direction_input = false;
                break;
		}

        // system("pause");
        // return 0;

        if (input_operation == 'R') {
            // while(cin.get() != '\n');
            cout << "是否重新开始游戏？请输入[Y/N]" << endl;
            char temp_input = getch();
            if (temp_input == 'Y') {
                system("cls");
                cout << "游戏重新开始，祝您玩得开心！" << endl;
                cout << "倒计时 3" << endl;
                delay_time(1);
                cout << "倒计时 2" << endl;
                delay_time(1);
                cout << "倒计时 1" << endl;
                delay_time(1);
                system("cls");
                game.restart();

            }
            continue;
        }
        if (input_operation == 'Q') {
            system("cls");
            cout << "退出游戏" << endl;
            delay_time(3);
            exit(0);
        }

        if (input_operation == 'H') {
            cout << "输入W/A/S/D进行上/下/左/右操作，输入R重新开始游戏，输入Q退出游戏。" << endl;
            delay_time(3);
            system("cls");
            game.show();
            continue;
        }

        if (is_direction_input) {
            system("cls");
            game.show();
		}

		if (!game.check())	{ //检查
            cout << "游戏结束，请输入R重新开始，或者输入Q退出游戏" << endl;
            delay_time(1);

            input_operation = getch();
            if (input_operation == 'R') {
                cout << "是否重新开始游戏？请输入[Y/N]" << endl;
                char temp_input = getch();
                if (temp_input == 'Y') {
                    system("cls");
                    cout << "游戏重新开始，祝您玩得开心！" << endl;
                    cout << "倒计时 3" << endl;
                    delay_time(1);
                    cout << "倒计时 2" << endl;
                    delay_time(1);
                    cout << "倒计时 1" << endl;
                    delay_time(1);
                    system("cls");
                    game.restart();

                }
            }
            else if (input_operation == 'Q') {
                system("cls");
                cout << "退出游戏" << endl;
                delay_time(3);
                exit(0);
            }
            else {
                system("cls");
                cout << "输入错误, 游戏结束" << endl;
            }

			break;
        }
        
    }

    return 0;
}
