//------------------------------------------------------------------------
// 五目ならべAI
//  ここを作る．
//  下記のサンプルは，直前に自分が置いた石の8近傍のどこかにランダムに石を置くだけ
//
//  引数説明
//    board[BOARD_SIZE][BORARD_SIZE]：五目並べの盤
//    com ： コンピュータが白石(STONE_WHITE)か黒石(STONE_BLACK)かを表す．
//    pos_x, pos_y：石を置く場所のx座標，y座標 両方出力用
//------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gomoku.h"

#define PRI_VERY_HIGH 4
#define PRI_HIGH 3
#define PRI_MID 2
#define PRI_LOW 1
#define PRI_ZERO 0

#define ARRAY 10

void check_around(int board[][BOARD_SIZE], int com, int pos_x, int pos_y);
void checkenemy(int com);
void Reset(int Row[],int Total[],int x[],int y[]);
int max_element(int Total[],int x[],int y[]);
int max_value(int row[]);



int hoge = 0;

static int answer[2]= {
        // 適当に初期化
        BOARD_SIZE / 2,
        BOARD_SIZE / 2,
    };


int atack_bottom_right_flag_3 = 0;
int atack_bottom_flag_3 = 0;
int atack_bottom_left_flag_3 = 0;
int atack_right_flag_3 = 0;

int atack_bottom_right_flag_4 = 0;
int atack_bottom_flag_4 = 0;
int atack_bottom_left_flag_4 = 0;
int atack_right_flag_4 = 0;


int defence_bottom_right_flag_3 = 0;
int defence_bottom_flag_3 = 0;
int defence_bottom_left_flag_3 = 0;
int defence_right_flag_3 = 0;

int defence_bottom_right_flag_4 = 0;
int defence_bottom_flag_4 = 0;
int defence_bottom_left_flag_4 = 0;
int defence_right_flag_4 = 0;




int atack_bottom_right_count = 0;   //右斜
int atack_bottom_count = 0;         //縦
int atack_bottom_left_count = 0;    //左斜
int atack_right_count = 0;          //横

int defence_bottom_right_count = 0;
int defence_bottom_count = 0;
int defence_bottom_left_count = 0;
int defence_right_count = 0;

static int enemy;




typedef struct stone{
    int start_x;            //先頭のx座標
    int start_y;            //先頭のy座標

    int end_x;              //終端のx座標
    int end_y;              //終端のy座標

    int before_start_x;     //先頭の前の石のx座標
    int before_start_y;     //先頭の前の石のy座標

    int after_end_x;        //終端の後の石のx座標
    int after_end_y;        //終端の後の石のy座標

    int direction;           //方向：1->縦, 2->横, 3->右斜, 4->左斜

    int stone_count;        //石の数

    int valid;              //優先度
}STONE;

void search_stone_tail(int board[][BOARD_SIZE], int com, STONE *stone);
void init_stone(STONE *stone,int i);

int out_board_judge(int i, int j){
    if( ( 0 <= i && i < BOARD_SIZE ) && ( 0 <= j && j < BOARD_SIZE ) ){
        return 1;
    }
    else{
        return 0;
    }
}

void board_search_atack_bottom_right_3(int board[][BOARD_SIZE], int com, STONE *line){  //右斜
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                           //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j + 1 ] == com && out_board_judge( i + 1, j + 1 ) ){           //右下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j + 2 ] == com && out_board_judge( i + 2, j + 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;
                        atack_bottom_right_flag_3 = 1;

                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j + 2;
                        res.end_y = i + 2;

                        res.stone_count = 3;

                        res.direction = 3;

                        search_stone_tail(board,com,&res);

                        if( board[ i - 1 ][ j - 1 ] == STONE_SPACE && board[ i + 3 ][ j + 3 ] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[ i - 1 ][ j - 1 ] == STONE_SPACE || board[ i + 3 ][ j + 3 ] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }

                        for(p = line - atack_bottom_right_count; p < p + atack_bottom_right_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            atack_bottom_right_count++;
                        }
                        
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
    
}
void board_search_atack_right_3(int board[][BOARD_SIZE], int com, STONE *line){         //右
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                       //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[i][ j + 1 ] == com && out_board_judge( i, j + 1 ) ){       //右に同じ色の石があるか
                    pos[1] = 1;
                    if( board[i][ j + 2 ] == com && out_board_judge( i, j + 2 ) ){   //3つ並んでいるか
                        pos[2] = 1;
                        atack_right_flag_3 = 1;
                        
                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j + 2;
                        res.end_y = i;

                        res.stone_count = 3;

                        res.direction = 2;

                        search_stone_tail(board,com,&res);

                        if( board[i][ j - 1 ] == STONE_SPACE && board[i][ j + 3 ] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[i][ j - 1 ] == STONE_SPACE || board[i][ j + 3 ] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }

                        for(p = line - atack_right_count; p < p + atack_right_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            atack_right_count++;
                        }
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
    
}
void board_search_atack_bottom_3(int board[][BOARD_SIZE], int com, STONE *line){        //下
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                       //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][j] == com && out_board_judge( i + 1, j ) ){       //下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][j] == com && out_board_judge( i + 2, j ) ){   //3つ並んでいるか
                        pos[2] = 1;
                        atack_bottom_flag_3 = 1;

                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j;
                        res.end_y = i + 2;

                        res.direction = 1;

                        res.stone_count = 3;


                        search_stone_tail(board,com,&res);

                        if( board[ i - 1 ][j] == STONE_SPACE && board[ i + 3 ][j] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[ i - 1 ][j] == STONE_SPACE || board[ i + 3 ][j] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }

                        for(p = line - atack_bottom_count; p < p + atack_bottom_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            atack_bottom_count++;
                        }
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }

            }
        }
    }
    
}
void board_search_atack_bottom_left_3(int board[][BOARD_SIZE], int com, STONE *line){   //左斜
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                       //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j - 1 ] == com && out_board_judge( i + 1, j - 1 ) ){       //下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j - 2 ] == com && out_board_judge( i + 2, j - 2 ) ){   //3つ並んでいるか
                        pos[2] = 1;
                        atack_bottom_left_flag_3 = 1;

                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j - 2;
                        res.end_y = i + 2;

                        res.direction = 4;

                        res.stone_count = 3;

                        search_stone_tail(board,com,&res);

                        if( board[ i - 1 ][ j + 1 ] == STONE_SPACE && board[ i + 3 ][ j - 3 ] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[ i - 1 ][ j + 1 ] == STONE_SPACE || board[ i + 3 ][ j - 3 ] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }

                        for(p = line - atack_bottom_left_count; p < p + atack_bottom_left_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            atack_bottom_left_count++;
                        }
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
    
}

void board_search_atack_bottom_right_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                           //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j + 1 ] == com && out_board_judge( i + 1, j + 1 ) ){           //右下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j + 2 ] == com && out_board_judge( i + 2, j + 2 )){       //3つ並んでいるか
                        pos[2] = 1;

                        if( board[ i + 3 ][ j + 3 ] == com && out_board_judge( i + 3, j + 3 ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            atack_bottom_right_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j + 3;
                            res.end_y = i + 3;

                            res.direction = 3;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[ i - 1 ][ j - 1 ] == STONE_SPACE && board[ i + 4 ][ j + 4 ] == STONE_SPACE ){
                                res.valid = PRI_VERY_HIGH;
                            }
                            else if ( board[ i - 1 ][ j - 1 ] == STONE_SPACE || board[ i + 4 ][ j + 4 ] == STONE_SPACE ) {
                                res.valid = PRI_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            *line = res;
                            atack_bottom_right_count++;
                            line++;                         
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                            *line = res;
                        }
                    }
                    else{
                        *line = res;
                        //
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                    //
                }
            }
        }
    }
   
}
void board_search_atack_right_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                       //(自分の)1個目の石を見つけた時
                pos[0] = 1;

                if( board[i][ j + 1 ] == com && out_board_judge( i, j + 1 ) ){       //右に同じ色の石があるか
                    pos[1] = 1;

                    if( board[i][ j + 2 ] == com && out_board_judge( i, j + 2 ) ){   //3つ並んでいるか
                        pos[2] = 1;

                        if( board[i][ j + 3 ] == com && out_board_judge( i, j + 3 ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            atack_right_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j + 3;
                            res.end_y = i ;

                            res.direction = 2;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[i][ j - 1 ] == STONE_SPACE && board[i][ j + 4 ] == STONE_SPACE ){
                                res.valid = PRI_VERY_HIGH;
                            }
                            else if ( board[i][ j - 1 ] == STONE_SPACE || board[i][ j + 4 ] == STONE_SPACE ) {
                                res.valid = PRI_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            *line = res;
                            atack_right_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                            *line = res;
                        }
                    }
                    else{
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
    
}
void board_search_atack_bottom_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};

    init_stone(&res,1);

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                       //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][j] == com && out_board_judge( i + 1, j ) ){       //下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][j] == com && out_board_judge( i + 2, j ) ){   //3つ並んでいるか
                        pos[2] = 1;

                        if( board[ i + 3 ][j] == com && out_board_judge( i + 3, j ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            atack_bottom_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j;
                            res.end_y = i + 3;

                            res.direction = 1;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[ i - 1 ][j] == STONE_SPACE && board[ i + 4 ][j] == STONE_SPACE ){
                                res.valid = PRI_VERY_HIGH;
                            }
                            else if ( board[ i - 1 ][j] == STONE_SPACE || board[ i + 4 ][j] == STONE_SPACE ) {
                                res.valid = PRI_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            *line = res;
                            atack_bottom_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                            *line = res;
                        }
                    }
                    else{
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
   printf("start(%d %d) end(%d %d) before(%d %d) after(%d %d) cnt_%d dic_%d val_%d atackcnt_%d \n",line->start_x,line->start_y,line->end_x,line->end_y,line->before_start_x,line->before_start_y,line->after_end_x,line->after_end_y,line->stone_count,line->direction,line->valid,atack_bottom_count);
}
void board_search_atack_bottom_left_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};

    res.valid = -1;

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == com ){                       //(自分の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j - 1 ] == com && out_board_judge( i + 1, j - 1 ) ){       //下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j - 2 ] == com && out_board_judge( i + 2, j - 2 ) ){   //3つ並んでいるか
                        pos[2] = 1;

                        if( board[ i + 3 ][ j - 3 ] == com && out_board_judge( i + 3, j - 3 ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            atack_bottom_left_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j - 3;
                            res.end_y = i + 3;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[ i - 1 ][ j + 1 ] == STONE_SPACE && board[ i + 4 ][ j - 4 ] == STONE_SPACE ){
                                res.valid = PRI_VERY_HIGH;
                            }
                            else if ( board[ i - 1 ][ j + 1 ] == STONE_SPACE || board[ i + 4 ][ j - 4 ] == STONE_SPACE ) {
                                res.valid = PRI_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            res.direction = 4;

                            *line = res;
                            atack_bottom_left_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                            *line = res;
                        }
                    }
                    else{
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
    
}



void board_search_defence_bottom_right_3(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j + 1 ] == enem && out_board_judge( i + 1, j + 1 ) ){           //右下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j + 2 ] == enem && out_board_judge( i + 2, j + 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;
                        defence_bottom_right_flag_3 = 1;

                        
                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j + 2;
                        res.end_y = i + 2;

                        res.stone_count = 3;

                        res.direction = 3;

                        search_stone_tail(board,com,&res);

                        if( board[ i - 1 ][ j - 1 ] == STONE_SPACE && board[ i + 3 ][ j + 3 ] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[ i - 1 ][ j - 1 ] == STONE_SPACE || board[ i + 3 ][ j + 3 ] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }


                        for(p = line - defence_bottom_right_count; p < p + defence_bottom_right_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            defence_bottom_right_count++;
                        }

                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }
   
}
void board_search_defence_right_3(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[i][ j + 1 ] == enem && out_board_judge( i, j + 1 ) ){           //右に同じ色の石があるか
                    pos[1] = 1;
                    if( board[i][ j + 2 ] == enem && out_board_judge( i, j + 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;
                        defence_right_flag_3 = 1;

                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j + 2;
                        res.end_y = i;
                        
                        res.direction = 2;

                        res.stone_count = 3;

                        search_stone_tail(board,com,&res);

                        if( board[i][ j - 1 ] == STONE_SPACE && board[i][ j + 3 ] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[i][ j - 1 ] == STONE_SPACE || board[i][ j + 3 ] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }


                        for(p = line - defence_right_count; p < p + defence_right_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            defence_right_count++;
                        }
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }
    
}
void board_search_defence_bottom_3(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][j] == enem && out_board_judge( i + 1, j ) ){           //下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][j] == enem && out_board_judge( i + 2, j ) ){       //3つ並んでいるか
                        pos[2] = 1;
                        defence_bottom_flag_3 = 1;

                    
                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j;
                        res.end_y = i + 2;

                        res.direction = 1;

                        res.stone_count = 3;

                        search_stone_tail(board,com,&res);

                        if( board[ i - 1 ][j] == STONE_SPACE && board[ i + 3 ][j] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[ i - 1 ][j] == STONE_SPACE || board[ i + 3 ][j] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }



                        for(p = line - defence_bottom_count; p < p + defence_bottom_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            defence_bottom_count++;
                        }
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }
    
}
void board_search_defence_bottom_left_3(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;
    STONE *p,tmp;
    int f = 0;

    int pos[3] = {0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j - 1 ] == enem && out_board_judge( i + 1, j - 1 ) ){           //左下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j - 2 ] == enem && out_board_judge( i + 2, j - 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;
                        defence_bottom_left_flag_3 = 1;

                        res.start_x = j;
                        res.start_y = i;
                        res.end_x = j - 2;
                        res.end_y = i + 2;
                        
                        res.direction = 4;

                        res.stone_count = 3;

                        search_stone_tail(board,com,&res);

                        if( board[ i - 1 ][ j + 1 ] == STONE_SPACE && board[ i + 3 ][ j - 3 ] == STONE_SPACE ){
                            res.valid = PRI_MID;
                        }
                        else if ( board[ i - 1 ][ j + 1 ] == STONE_SPACE || board[ i + 3 ][ j - 3 ] == STONE_SPACE ) {
                            res.valid = PRI_LOW;
                        }
                        else{
                            res.valid = PRI_ZERO;
                        }


                        for(p = line - defence_bottom_left_count; p < p + defence_bottom_left_count; p++){
                            if( (res.end_x == p->end_x && res.end_y == p->end_y ) || ( res.start_x == p->start_x && res.start_y == p->start_y ) ){
                                //4列の先頭座標か終端座標が同じであれば保存しない
                                f = 1;
                                break;
                            }
                            else{
                                tmp = res;
                            }
                        }

                        if( f == 0 ){
                            *line = tmp;
                            line++;
                            defence_bottom_left_count++;
                        }
                    }
                    else{
                        pos[0] = 0; pos[1] = 0;
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }
    
}

void board_search_defence_bottom_right_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j + 1 ] == enem && out_board_judge( i + 1, j + 1 ) ){           //右下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j + 2 ] == enem && out_board_judge( i + 2, j + 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;

                        if( board[ i + 3 ][ j + 3 ] == enem && out_board_judge( i + 3, j + 3 ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            defence_bottom_right_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j + 3;
                            res.end_y = i + 3;

                            res.stone_count = 4;

                            res.direction = 3;

                            search_stone_tail(board,com,&res);

                            if( board[ i - 1 ][ j - 1 ] == STONE_SPACE && board[ i + 4 ][ j + 4 ] == STONE_SPACE ){
                                res.valid = PRI_ZERO;
                            }
                            else if ( board[ i - 1 ][ j - 1 ] == STONE_SPACE || board[ i + 4 ][ j + 4 ] == STONE_SPACE ) {
                                res.valid = PRI_VERY_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            *line = res;
                            defence_bottom_right_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                        }
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }
    
}
void board_search_defence_right_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[i][ j + 1 ] == enem && out_board_judge( i, j + 1 ) ){           //右に同じ色の石があるか
                    pos[1] = 1;
                    if( board[i][ j + 2 ] == enem && out_board_judge( i, j + 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;

                        if( board[i][ j + 3 ] == enem && pos[3] != 1 && out_board_judge( i, j + 3 ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            defence_right_flag_3 = 0;
                            defence_right_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j + 3;
                            res.end_y = i;

                            res.direction = 2;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[i][ j - 1 ] == STONE_SPACE && board[i][ j + 4 ] == STONE_SPACE ){
                                res.valid = PRI_ZERO;
                            }
                            else if ( board[i][ j - 1 ] == STONE_SPACE || board[i][ j + 4 ] == STONE_SPACE ) {
                                res.valid = PRI_VERY_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            

                            *line = res;
                            defence_right_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                        }
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }
    
}
void board_search_defence_bottom_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][j] == enem && out_board_judge( i + 1, j ) ){           //下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][j] == enem && out_board_judge( i + 2, j ) ){       //3つ並んでいるか
                        pos[2] = 1;

                        if( board[ i + 3 ][j] == enem && out_board_judge( i + 3, j ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            defence_bottom_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j;
                            res.end_y = i + 3;

                            res.direction = 1;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[ i - 1 ][j] == STONE_SPACE && board[ i + 4 ][j] == STONE_SPACE ){
                                res.valid = PRI_ZERO;
                            }
                            else if ( board[ i - 1 ][j] == STONE_SPACE || board[ i + 4 ][j] == STONE_SPACE ) {
                                res.valid = PRI_VERY_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            *line = res;
                            defence_bottom_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                            *line = res;
                        }
                    }
                    else{
                        *line = res;
                    }
                }
                else{
                    pos[0] = 0;
                    *line = res;
                }
            }
        }
    }
    
}
void board_search_defence_bottom_left_4(int board[][BOARD_SIZE], int com, STONE *line){
    STONE res;

    int pos[4] = {0,0,0,0};
    int enem;

    res.valid = -1;

    if(com == STONE_BLACK){
        enem = STONE_WHITE;
    }
    else{
        enem = STONE_BLACK;
    }

    for(int i = 0; i < BOARD_SIZE; i++){
        for(int j = 0; j < BOARD_SIZE; j++){
            if( board[i][j] == enem ){                           //(敵の)1個目の石を見つけた時

                pos[0] = 1;

                if( board[ i + 1 ][ j - 1 ] == enem && out_board_judge( i + 1, j - 1 ) ){           //左下に同じ色の石があるか
                    pos[1] = 1;
                    if( board[ i + 2 ][ j - 2 ] == enem && out_board_judge( i + 2, j - 2 ) ){       //3つ並んでいるか
                        pos[2] = 1;

                        if( board[ i + 3 ][ j - 3 ] == enem && out_board_judge( i + 3, j - 3 ) ){   //4つ並んでいるか
                            pos[3] = 1;
                            defence_bottom_left_flag_4 = 1;

                            res.start_x = j;
                            res.start_y = i;
                            res.end_x = j - 3;
                            res.end_y = i + 3;

                            res.direction = 4;

                            res.stone_count = 4;

                            search_stone_tail(board,com,&res);

                            if( board[ i - 1 ][ j + 1 ] == STONE_SPACE && board[ i + 4 ][ j - 4 ] == STONE_SPACE ){
                                res.valid = PRI_ZERO;
                            }
                            else if ( board[ i - 1 ][ j + 1 ] == STONE_SPACE || board[ i + 4 ][ j - 4 ] == STONE_SPACE ) {
                                res.valid = PRI_VERY_HIGH;
                            }
                            else{
                                res.valid = PRI_ZERO;
                            }

                            *line = res;
                            defence_bottom_left_count++;
                            line++;
                        }
                        else{
                            pos[0] = 0; pos[1] = 0; pos[2] = 0;
                        }
                    }
                }
                else{
                    pos[0] = 0;
                }
            }
        }
    }

}



void put_stone_to_board(int board[][BOARD_SIZE], int com, int *pos_x, int *pos_y, STONE put_stone,int pre[]){
    *pos_x = put_stone.before_start_x;
    *pos_y = put_stone.before_start_y;

    printf("put(%d %d)\n",*pos_x,*pos_y);

    if( *pos_x > 0 && *pos_y > 0 ){
        *pos_x = put_stone.after_end_x;
        *pos_y = put_stone.after_end_y;
    }
    else{
        *pos_x = 0;
        *pos_y = hoge;
    }

    printf("valid %d\n",put_stone.valid);

    if( put_stone.valid == -1 ){
        check_around(board,com,pre[1],pre[0]); 
        pre[0]=answer[0];
		pre[1]=answer[1];
		*pos_y=answer[0];
		*pos_x=answer[1];
    }
    else{
        *pos_x = 0;
        *pos_y = hoge;
    }
    hoge++;

    printf("(%d  %d)\n",*pos_x,*pos_y);

}

void search_stone_tail(int board[][BOARD_SIZE], int com, STONE *stone){

    switch (stone->direction){

    case 1:             //縦
        stone->before_start_x = stone->start_x;
        stone->before_start_y = stone->start_y - 1;

        stone->after_end_x = stone->end_x;
        stone->after_end_y = stone->end_y + 1;
        break;

    case 2:             //横
        stone->before_start_x = stone->start_x - 1;
        stone->before_start_y = stone->start_y;

        stone->after_end_x = stone->end_x + 1;
        stone->after_end_y = stone->end_y;
        break;

    case 3:             //右斜
        stone->before_start_x = stone->start_x - 1;
        stone->before_start_y = stone->start_y - 1;

        stone->after_end_x = stone->end_x + 1;
        stone->after_end_y = stone->end_y + 1;
        break;
    
    case 4:             //左斜
        stone->before_start_x = stone->start_x + 1;
        stone->before_start_y = stone->start_y - 1;

        stone->after_end_x = stone->end_x - 1;
        stone->after_end_y = stone->end_y + 1;
        break;
    
    default:
        break;
    }

    if( !(0 <= stone->before_start_x && stone->before_start_x < BOARD_SIZE) || !(0 <= stone->before_start_y && stone->before_start_y < BOARD_SIZE) ){
        stone->before_start_x = -1; stone->before_start_y = -1;   //先頭の前が範囲外であれば，-1を格納(有効な座標として扱わない)
    }

    if( !(0 <= stone->after_end_x && stone->after_end_x < BOARD_SIZE) || !(0 <= stone->after_end_y && stone->after_end_y < BOARD_SIZE) ){
        stone->after_end_x = -1; stone->after_end_y = -1;         //最後尾の後が範囲外であれば，-1を格納(有効な座標として扱わない)
    }
}

STONE judge(int board[][BOARD_SIZE], STONE *my_tate, STONE *my_yoko, STONE *my_l_naname, STONE *my_r_naname, STONE *ene_tate, STONE *ene_yoko, STONE *ene_l_naname, STONE *ene_r_naname){
    STONE tmp;
    STONE all_result[100];
    STONE *p;
    tmp.valid = -1;
    int i = 0,j = 0;

    printf("judge_start\n");

    for(i = 0; i < 100; i++){
        all_result[i].valid = -1;
        tmp.valid = -1;
    }


    for(p = my_tate;p < p + atack_bottom_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            p->valid++;
            all_result[i] = *p;
            i++;
        }
    }

    for(p = my_yoko;p < p + atack_right_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            p->valid++;
            all_result[i] = *p;
            i++;
        }
    }

    for(p = my_l_naname;p < p + atack_bottom_left_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            p->valid++;
            all_result[i] = *p;
            i++;
        }
    }

    for(p = my_r_naname;p < p + atack_bottom_right_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            p->valid++;
            all_result[i] = *p;
            i++;
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    for(p = ene_tate;p < p + defence_bottom_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            all_result[i] = *p;
            i++;
        }
    }

    for(p = ene_yoko;p < p + defence_right_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            all_result[i] = *p;
            i++;
        }
    }

    for(p = ene_l_naname;p < p + defence_bottom_left_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            all_result[i] = *p;
            i++;
        }
    }

    for(p = ene_r_naname;p < p + defence_bottom_right_count; p++ ){
        if(p->valid > PRI_ZERO && p->valid <= PRI_VERY_HIGH){
            all_result[i] = *p;
            i++;
        }
    }




    tmp = all_result[0];
    for( j = 1 ; j < i ; j++ ){
        if( tmp.valid < all_result[i].valid ){
            tmp = all_result[i];
        }
    }

    /*
    for(int i = 0; i < 50; i++){
        printf("%d %d %d %d  \n",all_result[i].before_start_x,all_result[i].after_end_y,all_result[i].after_end_x,all_result[i].after_end_y);
    }
    */
    printf("judge_end\n");
    return tmp;

}

void init_stone(STONE *stone,int i){
    STONE *p;
    for(p = stone; p < p + i; p++){

        p->start_x = -1;
        p->start_y = -1;

        p->end_x = -1;
        p->end_y = -1;

        p->stone_count = -1;
        p->valid = -1;
        p->direction = -1;
    }
}

int calcPutPos(int board[][BOARD_SIZE], int com, int *pos_x, int *pos_y)
{
    static int pre[2] = {
        // 直前の石の場所を覚えておくための変数 適当に初期化
        BOARD_SIZE / 2,
        BOARD_SIZE / 2,
    };
    static int flag = 0;
    int count = 0;

    STONE my_stone_vertical[ARRAY];             //縦
    STONE my_stone_horizontal[ARRAY];           //横
    STONE my_stone_left_diagonal[ARRAY];        //左斜め
    STONE my_stone_right_diagonal[ARRAY];       //右斜め

    STONE enemy_stone_vertical[ARRAY];             //縦
    STONE enemy_stone_horizontal[ARRAY];           //横
    STONE enemy_stone_left_diagonal[ARRAY];        //左斜め
    STONE enemy_stone_right_diagonal[ARRAY];       //右斜め

    

    //同じ個数で同じ列の石の列を複数保存したい

    if(com == STONE_BLACK){
        enemy = STONE_WHITE;
    }
    else{
        enemy = STONE_BLACK;
    }

    //後攻の場合・・・最初の石は相手の石のとなり

    // コンピュータが最初の1手の場合は，(7, 7)に石を置く
    if (flag == 0)
    {
        srand((unsigned int)time(NULL));
        flag = 1;
        if (com == STONE_BLACK)
        {
            *pos_y = BOARD_SIZE / 2;
            *pos_x = BOARD_SIZE / 2;
            return 0;
        }
    }
    while (1)
    {
        /*
        //乱数で前に置いた場所の8近傍に置く
        if (count > 5)
        { // 5回置こうとして，どこにも置けなくなった場合はどこでもよいからおいてみる
            *pos_y = (int)(rand() % 15);
            *pos_x = (int)(rand() % 15);
            printf("%d\n", count);
        }
        else
        {
            *pos_y = pre[0] + (int)(rand() % 3 - 1);
            *pos_x = pre[1] + (int)(rand() % 3 - 1);
        }
        */

        board_search_atack_bottom_4(board,com,my_stone_vertical+atack_bottom_count);
       
        put_stone_to_board(board,com,pos_x,pos_y,judge(board,my_stone_vertical,my_stone_horizontal,my_stone_left_diagonal,my_stone_right_diagonal,enemy_stone_vertical,enemy_stone_horizontal,enemy_stone_left_diagonal,enemy_stone_right_diagonal),pre);

        if (checkOutPos(*pos_x, *pos_y)){
            if (board[*pos_y][*pos_x] == STONE_SPACE)
            {
                break;
            }
            count++;
        }
        else{ // bug出し
            printf("out of board(%d %d)\n", *pos_x, *pos_y);
            count++;
        }
    }
    pre[0] = *pos_y;
    pre[1] = *pos_x;

    return 0;
}

void check_around(int board[][BOARD_SIZE], int com, int pos_x, int pos_y) {
	static int i = 0;
	static int Row[8] = { 1,1,1,1,1,1,1,1 };//8方向の自石の数をここに格納
	static int Total[4] = {0,0,0,0};//8方向を4方向に変換するTotal
	static int _x[8] = { 15,15,15,15,15,15,15,15 };//8近傍のxパターン　（相手石がある場合は15のままという処理）
	static int _y[8] = { 15,15,15,15,15,15,15,15 };//8近傍のyパターン　（相手石～略）
	static int max_val=0;

	while (i < 8) {
		switch (i)
		{
		case 0: //上,ここだけ詳しく書くけど、case1~7は方向が変わるだけで処理は一緒！
			if (board[pos_y - 1][pos_x] == com) {//自分の上に自石があったとき
				
				Row[i]++;//上方向に1足すという意味
				check_around(board, com, pos_x, pos_y-1);//ここがポイント再帰します.staticを使っているのでiは不変であるから,そこの列だけみることができる
				return;
			}
			else if(board[pos_y - 1][pos_x] == enemy) {//自分の上に相手の石だったとき　（ここに壁の時も入れたい！）
				pos_y += Row[i]-1;//リセットだけしてくれればいい
			}
			else {//そこにスペースが有る時は
				_y[i] = pos_y-1;//そのスペースの座標を入れる
				_x[i] = pos_x;//上記と同じ
				pos_y += Row[i]-1;//再帰した時用に,直前の一手にリセットする
			}
			break;

		case 1: //左上
			if (board[pos_y - 1][pos_x - 1] == com) {
				Row[i]++;
				check_around(board, com, pos_x - 1,pos_y - 1);
				return;
			}
			else if (board[pos_y - 1][pos_x - 1] == enemy) {
				pos_y += Row[i]-1;
				pos_x += Row[i]-1;
			}
			else {
				_x[i] = pos_x-1;
				_y[i] = pos_y -1;

				pos_y += Row[i]-1;
				pos_x += Row[i]-1;

			}
			break;

		case 2: //左
			if (board[pos_y][pos_x - 1] == com) {
				Row[i]++;
				
				check_around(board, com,pos_x-1,pos_y);
				return;
			}
			else if (board[pos_y][pos_x - 1] == enemy) {
				pos_x += Row[i]-1;				
			}
			else {
				_x[i] = pos_x -1;
				_y[i]=pos_y;
				pos_x += Row[i]-1;
			}
			break;

		case 3: //左下
			if (board[pos_y + 1][pos_x - 1] == com) {
				Row[i]++;
				check_around(board, com, pos_x-1, pos_y+1);
				return;
			}
			else if (board[pos_y + 1][pos_x - 1] == enemy) {

				pos_y -= Row[i]-1;
				pos_x += Row[i]-1;
			}
			else {
				_x[i] = pos_x - 1;
				_y[i] = pos_y + 1;

				pos_y -= Row[i]-1;
				pos_x += Row[i]-1;
			}
			break;

		case 4: //下
			if (board[pos_y + 1][pos_x] == com) {
				Row[i]++;
				check_around(board, com, pos_x, pos_y+1);
				return;
			}
			else if (board[pos_y + 1][pos_x] == enemy) {
				pos_y -= Row[i]-1;
			}
			else {
				_x[i]=pos_x;
				_y[i] = pos_y+1;
				pos_y -= Row[i]-1;
			}
			break;

		case 5: //右下
			if (board[pos_y + 1][pos_x + 1] == com) {
				Row[i]++;
				check_around(board, com,pos_x+1,pos_y+1);
				return;
			}
			else if (board[pos_y + 1][pos_x + 1] == enemy) {
				pos_y -= Row[i]-1;
				pos_x -= Row[i]-1;
			}
			else {
				_x[i] = pos_x +1;
				_y[i] = pos_y + 1;
				pos_y -= Row[i]-1;
				pos_x -= Row[i]-1;
			}
			break;

		case 6: //右
			if (board[pos_y][pos_x + 1] == com) {
				Row[i]++;
				check_around(board, com, pos_x+1,pos_y);
				return;
			}
			else if (board[pos_y][pos_x + 1] == enemy) {
				pos_x -= Row[i]-1;
			}
			else {
				_x[i] = pos_x + 1;
				_y[i]=pos_y;
				pos_x -= Row[i]-1;
			}
			break;

		case 7: //右上
			if (board[pos_y - 1][pos_x + 1] == com) {
				Row[i]++;
				check_around(board, com,pos_x+1,pos_y-1);
				return;
			}
			else if (board[pos_y - 1][pos_x + 1] == enemy) {
				pos_y += Row[i]-1;
				pos_x -= Row[i]-1;
			}
			else {
				_x[i] = pos_x + 1;
				_y[i] = pos_y - 1;

				pos_y += Row[i]-1;
				pos_x -= Row[i]-1;
			}
			break;

		default:
			break;
		}
		
		i++;
	}


		i=0;//staticを使っているので初期化します
		
		int j;
	for (j = 0; j < 4; j++) {//Totalを作ることで、縦,横,左斜,右斜の数に変換
		Total[j] = Row[j]+Row[j+4];

	}

	max_val = max_element(Total,_x,_y);//最適な添字を探す

	if(_x[max_val]!=15&&_y[max_val]!=15){//xとyが15の時は,相手石があるということになる.(基本的に最初の位置を優先してます)
		answer[1]=_x[max_val];
		answer[0] = _y[max_val];
	}
	else{
		answer[1]=_x[max_val+4];
		answer[0] = _y[max_val+4];
	}

		Reset(Row,Total,_x,_y);//初期化する
		return;
}

void Reset(int Row[],int Total[],int x[],int y[]){//staticを多様してるので、初期化しないといけない
	int i;
	for(i=0;i<8;i++){
		x[i]=15;
		y[i]=15;
		Row[i]=1;
		if(i<4){
			Total[i]=0;
		}
	}
}
void checkenemy(int com) {//自分が白か黒か判別する
	if (com == 1) {
		enemy = 2;
	}
	else if (com == 2) {
		enemy = 1;
	}
	else {
		printf("error");
	}
}

int max_element(int Total[],int x[],int y[]) {//添字探し
	int i;
	int max_element = 0; //配列内の最大値の添字
	int max_value;//配列内の最大値

	max_value = -1;//絶対ないけど、添字が-は周りが全部障害物の時　エラー時で使える？

	for (i = 0; i < 3; i++) {
		if (max_value < Total[i]) {
			if((x[i]!=15&&y[i]!=15)||(x[i+4]!=15&&y[i+4]!=15)){//両端が相手石で塞がってる場合は添字を取らなくしてます.
			max_value = Total[i];
			max_element = i;
			}
		}
	}
	return max_element;
}
