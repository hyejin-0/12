#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "board.c"

#define MAX_CHARNAME   200
#define N_PLAYER       3
#define MAX_DIE        6

#define PLAYERSTATUS_LIVE    0
#define PLAYERSTATUS_DIE     1
#define PLAYERSTATUS_END     2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = {"LIVE", "DIE", "END"};

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void opening(void)
{
	printf("======\n");
	printf(" S.G. \n");
	printf("======\n");
}

int rolldie(void)
{
	return rand()%MAX_DIE+1;
}


void printPlayerPosition(int player)
{
	int i;
	for(i=0;i<N_BOARD;i++)
	{
		printf("|");
		if (i == player_position[player])
		   printf("%c", player_name[player][0]);
		else
		{
			if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
			   printf("X");
			else 
			   printf(" ");
		}
	}
	printf("|\n");
} 

void printPlayerStatus(void)
{
	int i;
	printf("player status ---\n");
	for (i=0;i<N_PLAYER;i++)
	{
		printf("%s : pos %i, coin %i, status %s\n", player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
	}
	printf("-----------------\n");
}

void checkDie(void)
{
	int i;
	for (i=0;i<N_PLAYER;i++)
	   if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
	      player_status[i] = PLAYERSTATUS_DIE;
}

int main(int argc, char *argv[]) 
{
	int pos = 0;
	int i;
	int turn = 0;
	
	srand((unsigned)time(NULL));
	
	//0. opening
	opening();
	
	//1. 초기화&플레이어 이름 결정
	board_initboard();
	//1-2. initialize player
	for (i=0;i<N_PLAYER;i++)
	{
		player_position[i] = 0;
		player_coin[i] = 0;
		player_status[i] = PLAYERSTATUS_LIVE;
		//player name
		printf("Player %i's name: ", i);
		scanf("%s", player_name[i]);
	}
	
	//2. 반복문(플레이어 턴) 
	do {
		int step;
		int coinResult;
		char c;
		
		if (player_status[turn] != PLAYERSTATUS_LIVE)
		{
			turn = (turn + 1)%N_PLAYER;
			continue;
		}
		
		//2-1. 보드 & 플레이어의 상태 출력
		board_printBoardStatus();
	    for (i=0;i<N_PLAYER;i++)
	        printPlayerPosition(i);
	    printPlayerStatus();
		
		
		//2-2. 주사위 던지기
		printf("%s turn!! ", player_name[turn]);
		printf("Press any key to roll a die!\n");
		scanf("%d", &c);
		fflush(stdin);
		step = rolldie();
		
		//2-3. 이동
		player_position[turn] += step;
		if (player_position[turn] >= N_BOARD)
		   player_position[turn] = N_BOARD-1;
		
		if (player_position[turn] == N_BOARD-1)
		   player_status[turn] = PLAYERSTATUS_END;
		//printf()
		printf("Die result : %i, %s moved to %i\n", player_position[turn], player_name[turn]);
		
		//2-4. 동전 줍기
		coinResult = board_getBoardCoin(pos);
		player_coin[turn] += coinResult;
		//printf()
		printf("Lucky! %s got %i coins\n", player_name[turn], player_coin[turn]);
		
	    
	    //2-5. 다음턴
	    turn = (turn + 1)%N_PLAYER; //wrap around
	    
	    //2-6. if (조건: 모든 플레이어가 한 번씩 턴을 돎)
	    if (turn == 0)
	    {
	    	//상어 동작 
	    	int shark_pos = board_stepShark();
			printf("Shark moved to %i\n", shark_pos);
			checkDie(); 
		}
	} while(player_status[i] == PLAYERSTATUS_DIE);
	    
	//3. 정리(승자 계산, 출력 등) 
	int game_end(void)
	{
		int i;
		int flag_end = 1;
		
		//if all the players are died?
		for (i=0;i<N_PLAYER;i++)
		{
			if (player_status[i] == PLAYERSTATUS_LIVE)
			{
				flag_end = 0;
				break;
			}
		}
		return flag_end;
	}
	
	int getAlivePlayer(void)
	{
		int i;
		int cnt = 0;
		for (i=0;i<N_PLAYER;i++)
		{
			if (player_status[i] == PLAYERSTATUS_END)
			   cnt++;
		}
		return cnt;
	}
	
	int getWinner(void)
	{
		int i;
		int winner = 0;
		int max_coin = -1;
		
		for (i=0;i<N_PLAYER;i++)
		{
			if (player_coin[i] > max_coin)
			{
				max_coin = player_coin[i];
				winner = i;
			}
		}
		return winner;
	}
	
	return 0;
}
