#include <stdio.h>
#include <unistd.h>
//0kK 1kQ 2kR 3kB 4kN 5kP 6K 7Q 8R 9B 10N 11P 12 BLANK
 
// program nie robi rozszady, en passant 
const int BLANK = 12;
int SCORE[]={-10000, -90, -50, -30, -30, -10, 10000, 90, 50, 30, 30,10 , 0};
int MAX_KIER[]={8,8,4,4,8,3,8,8,4,4,8,3,0};
int MAX_ODL[]={2,8,8,8,2,2,2,8,8,8,2,2,0};
int WX[12][8]={{0,1,1,1,0,-1,-1,-1},{0,1,1,1,0,-1,-1,-1},{0,1,0,-1},{1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1},{0,1,1,1,0,-1,-1,-1},{0,1,1,1,0,-1,-1,-1},{0,1,0,-1},{1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1} };
 
int WY[12][8]={{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},{-1,0,1,0},{-1,1,1,-1},{-2,-1,1,2,2,1,-1,-2},{1,1,1},{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},{-1,0,1,0},{-1,1,1,-1},{-2,-1,1,2,2,1,-1,-2},{-1,-1,-1} };
 
 
int max(int a, int b){
    if(a>b) return a;
    return b;
}
 
int min(int a, int b){
    if(a<b) return a;
    return b;
}
 
int evaluate(int b[8][8])
{
    int score = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            score+= SCORE[b[i][j]];
        }
    }
    return score;
}
 
 
void PrintBoard(int board[8][8]){
    printf("\n");
    char fig[] = "kqrbnpKQRBNP_";
    for(int i = 0 ; i < 8; i++){
 
        for(int j = 0; j < 8; j++){
            printf(j == 7 ? "%c\n":"%c ", fig[board[i][j]]);
        }
    }
    printf("\n");  
}
 
int MoveGenerator(int board[8][8], int mode, int depth, int best_move[4])    //mode 1 = komputer- maxujemy score, mode 0 = gracz- minujemy score
{
    if(depth < 2)
        return(evaluate(board));
    int new_x,new_y, was_on_new,score,best_score=(mode?10000:-10000);
 
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            if(mode == 1 && board[x][y]<6){     //0-5 figura komputera
                for(int kier = 0; kier < MAX_KIER[board[x][y]]; kier++){
                    int IsNextPossible = 1;
                    for(int odl = 1; odl < MAX_ODL[board[x][y]]; odl++){
 
                        if(IsNextPossible == 0)
                            break;
 
                        new_x = x + WX[board[x][y]][kier]*odl;
                        new_y = y + WY[board[x][y]][kier]*odl;
                        if(new_x < 8 && new_x > -1 && new_y < 8 && new_y > -1){
                            if(board[new_x][new_y] < 6) //jeżeli stoi tu figura komputera
                                break;
 
                            if(board[new_x][new_y] > 5 && board[new_x][new_y] < 12){ //jeżeli stoi tu figura gracza
                                IsNextPossible = 0;
                            }
 
                            if(board[x][y] == 5 && board[new_x][new_y] == 12 && kier != 1)//warunek pionka
                                break;
                            was_on_new = board[new_x][new_y];   //na przesuniętym była
                            board[new_x][new_y] = board[x][y];  //teraz jest
                            board[x][y] = 12;                   //a stare pole jest puste
 
                            score = MoveGenerator(board, 0, depth - 1,best_move);
                            if(score < best_score){
                                best_score = score;
                                best_move[0]=x;
                                best_move[1]=y;
                                best_move[2]=new_x;
                                best_move[3]=new_y;
                            }
 
                            board[x][y] = board[new_x][new_y]; //cofnąć figurę
                            board[new_x][new_y] = was_on_new; //oddać zbitą
                        }
                        else break;
                       /**   
                jeżeli ruch jest możliwy,czyli nie stoi nasza figura albo nie wychodzi poza szachownicę
                    wykonaj ruch
                    sprawdź ocenę
                    cofnij ruch
                    jeżeli ocena jest najlepsza
                        najlepsza ocena = ta ocena
                        najlepszy ruch = ten ruch
    zwróć najlepszy ruch
    **/
                    }
                }
            }
            if(mode == 0 && board[x][y]>5 && board[x][y]<12){     //6-11 figura gracza
                for(int kier = 0; kier < MAX_KIER[board[x][y]]; kier++){
                    int IsNextPossible = 1;
                    for(int odl = 1; odl < MAX_ODL[board[x][y]]; odl++){
 
                        if(IsNextPossible == 0)
                            break;
 
                        new_x = x + WX[board[x][y]][kier]*odl;
                        new_y = y + WY[board[x][y]][kier]*odl;
                        if(new_x < 8 && new_x > -1 && new_y < 8 && new_y > -1){
                            if(board[new_x][new_y] > 5 && board[new_x][new_y] < 12) //jeżeli stoi tu figura gracza
                                break;
 
                            if(board[new_x][new_y] < 6){ //jeżeli stoi tu figura komputera
                                IsNextPossible = 0;
                            }
 
                            if(board[x][y] == 11 && board[new_x][new_y] == 12 && kier != 1)//warunek pionka
                                break;
                            was_on_new = board[new_x][new_y];   //na przesuniętym była
                            board[new_x][new_y] = board[x][y];  //teraz jest
                            board[x][y] = 12;                   //a stare pole jest puste
 
                            score = MoveGenerator(board, 1, depth - 1,best_move);
                            if(score > best_score){
                                best_score = score;
                                best_move[0]=x;
                                best_move[1]=y;
                                best_move[2]=new_x;
                                best_move[3]=new_y;
                            }
 
                            board[x][y] = board[new_x][new_y]; //cofnąć figurę
                            board[new_x][new_y] = was_on_new; //oddać zbitą
                        }
                        else break;
                    }
                }
            }
        }
    }
    return best_score;
 
}
 
int MovesPossible(int board[8][8], int mode)
{
    int new_x,new_y, moves = 0;
 
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            if(mode == 1 && board[x][y]<6){     //0-5 figura komputera
                for(int kier = 0; kier < MAX_KIER[board[x][y]]; kier++){
                    int IsNextPossible = 1;
                    for(int odl = 1; odl < MAX_ODL[board[x][y]]; odl++){
 
                        if(IsNextPossible == 0)
                            break;
 
                        new_x = x + WX[board[x][y]][kier]*odl;
                        new_y = y + WY[board[x][y]][kier]*odl;
                        if(new_x < 8 && new_x > -1 && new_y < 8 && new_y > -1){
                            if(board[new_x][new_y] < 6) //jeżeli stoi tu figura komputera
                                break;
 
                            if(board[new_x][new_y] > 5 && board[new_x][new_y] < 12){ //jeżeli stoi tu figura gracza
                                IsNextPossible = 0;
                            }
 
                            if(board[x][y] == 5 && board[new_x][new_y] == 12 && kier != 1)//warunek pionka 1
                                break;
 
                            moves += 1;
                        }
                        else break;
                    }
                }
            }
            if(mode == 0 && board[x][y]>5 && board[x][y]<12){     //6-11 figura gracza
                for(int kier = 0; kier < MAX_KIER[board[x][y]]; kier++){
                    int IsNextPossible = 1;
                    for(int odl = 1; odl < MAX_ODL[board[x][y]]; odl++){
 
                        if(IsNextPossible == 0)
                            break;
 
                        new_x = x + WX[board[x][y]][kier]*odl;
                        new_y = y + WY[board[x][y]][kier]*odl;
                        if(new_x < 8 && new_x > -1 && new_y < 8 && new_y > -1){
                            if(board[new_x][new_y] > 5 && board[new_x][new_y] < 12) //jeżeli stoi tu figura gracza
                                break;
 
                            if(board[new_x][new_y] < 6){ //jeżeli stoi tu figura komputera
                                IsNextPossible = 0;
                            }
 
                            if(board[x][y] == 11 && board[new_x][new_y] == 12 && kier != 1)//warunek pionka
                                break;
 
                           moves+=1;
                        }
                        else break;
                    }
                }
            }
        }
    }
    if(moves > 0) return 1;
    else return 0;
 
}
 
void Do_move(int board[8][8], int best_move[4]){
    board[best_move[2]][best_move[3]] = board[best_move[0]][best_move[1]];
    board[best_move[0]][best_move[1]] = 12;
}
 
int main()
{
    int best_move[4];
 
    int board[8][8] =
/**    {        //szachownica na początku gry
        {  2,  5, 12, 12, 12, 12, 11,  8},
        {  4,  5, 12, 12, 12, 12, 11, 10},
        {  3,  5, 12, 12, 12, 12, 11,  9},
        {  1,  5, 12, 12, 12, 12, 11,  6},
        {  0,  5, 12, 12, 12, 12, 11,  7},
        {  3,  5, 12, 12, 12, 12, 11,  9},
        {  4,  5, 12, 12, 12, 12, 11, 10},
        {  2,  5, 12, 12, 12, 12, 11,  8}
   };
 **/ 
    {        //szachownica zmodyfikowana
        {  2,  5, 8, 12, 12, 12, 11,  12},
        {  4,  5, 12, 10, 12, 12, 11, 12},
        {  3,  5, 12, 12, 12, 12, 11,  9},
        {  1,  5, 12, 12, 12, 12, 11,  6},
        {  0,  5, 12, 12, 12, 12, 11,  7},
        {  3,  5, 12, 12, 12, 12, 11,  9},
        {  4,  5, 12, 12, 12, 12, 11, 10},
        {  2,  5, 12, 12, 12, 12, 11,  8}
  };
 
 
/** blok do gry K vs K
    for(int x = 0; x < 20; x++){
        PrintBoard(board);
        MoveGenerator(board,1,6,best_move);
        Do_move(board,best_move);
        sleep(1);
 
        PrintBoard(board);
        MoveGenerator(board,0,6,best_move);
        Do_move(board,best_move);
        sleep(1);
    }
    **/
 
    //blok do gry K vs P
    int my_move[4];
    while(evaluate(board) < 9000 && evaluate(board) > -9000 && MovesPossible(board, 1) > 0 && MovesPossible(board,0) > 0){
        PrintBoard(board);
        if(MovesPossible(board, 0) == 0){
            printf("pat");
            break;
        }
        scanf("%d %d %d %d", &my_move[0], &my_move[1], &my_move[2], &my_move[3]);
        Do_move(board, my_move);
        MoveGenerator(board,1,6,best_move);
        Do_move(board, best_move);
        PrintBoard(board);
    }
 
    return 0;
}