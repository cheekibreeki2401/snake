#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include "kbhit.h"
int field_x=20;
int field_y=30;
int score=0;
int body_parts=5;
int alive = 1;
typedef enum status{MOVE, CRASH, SCORE, WAIT}status;
typedef struct snake{
	int x;
	int y;
	int dir;//0=N, 1=E, 2=S, 3=W
	struct snake *next_body;
}snake;


void start_game();
void make_body(snake *head);
void next_frame(snake *head);
void move_snake(snake *head);
void user_input(snake *head);
int collide_with_self(snake *head);
int main(){
	start_game();
	return 0;
}

void start_game(){
	snake *head;
	head=malloc(sizeof(snake));
	if(head == NULL){
		return;
	}
	head->x = field_x/2;
	head->y = field_y/2;
	head->dir = 3;
	for(int i = 0; i < body_parts; i++){
		make_body(head);
	}
	snake *tmp=malloc(sizeof(snake));
	tmp = head;
	while(tmp != NULL){
		printf("The position of this body part is %d and %d. \n", tmp->x, tmp->y);
		tmp = tmp->next_body;
	}
	while(alive==1){
		user_input(head);
		next_frame(head);
	}
	tmp = head->next_body;
	free(head);
	while(tmp != NULL)
	{
		snake *tmp2 = tmp;
		tmp = tmp->next_body;
	}
	printf("Game over\n");
	return;
}

void make_body(snake *head){
	snake *new_snake;
	snake *tmp;
	new_snake=malloc(sizeof(snake));
	tmp=malloc(sizeof(snake));
	tmp = head;
	while(tmp->next_body != NULL){
		tmp = tmp->next_body;
	}
	tmp->next_body = new_snake;
	new_snake->dir = tmp->dir;
	switch(new_snake->dir){
		case 0:
			new_snake->x = tmp->x;
			new_snake->y = tmp->y+1;
			break;
		case 1:
			new_snake->x = tmp->x-1;
			new_snake->y = tmp->y;
			break;
		case 2:
			new_snake->x = tmp->x;
			new_snake->y = tmp->y-1;
			break;
		case 3:
			new_snake->x = tmp->x+1;
			new_snake->y = tmp->y;
			break;
		default:
			printf("We done fucked");
	}
	return;
}

void next_frame(snake *head){
	status cur_stat = MOVE;
	if((head->dir==0 && head->y-1 == 0)||
			(head->dir==1 && head->x+1==field_x)||
			(head->dir==2 && head->y+1==field_y)||
			(head->dir==3 && head->x-1==0)||
			collide_with_self(head)==1){
		cur_stat = CRASH;
	}
	if(cur_stat != CRASH){
		move_snake(head);
		printf("Can move forward!\n");
	} else {
		printf("HIT SOMETHING\n");
		alive=0;
	}
	return;
}

void move_snake(snake *head){
	snake *tmp;
	snake *tmp2;
	tmp=malloc(sizeof(snake));
	tmp2=malloc(sizeof(snake));
	tmp=head;
	tmp2=head->next_body;
	switch(head->dir)
	{
		case 0:
			head->y -= 1;
			break;
		case 1:
			head->x += 1;
			break;
		case 2:
			head->y += 1;
			break;
		case 3:
			head->x -=1;
			break;
		default:
			printf("We should not be here");
	}
	while(tmp->next_body != NULL){
		tmp->next_body->x = tmp->x;
		tmp->next_body->y = tmp->y;
		tmp->next_body->dir = tmp->dir;
		tmp = tmp2;
		tmp2 = tmp2->next_body;
	}
	return;
}

void user_input(snake *head){
	WINDOW *w;
	char input;
	w=initscr();
	refresh();
	printw("The snake is currently at x: %d and y: %d \n", head->x, head->y);
	timeout(500);
	input=getch();
	endwin();
	switch(input){
		case 'a':
			if(head->dir != 1){
			head->dir = 3;
			}
			break;
		case 's':
			if(head->dir != 0){
				head->dir=2;
			}
			break;
		case 'd':
			if(head->dir != 3){
				head->dir=1;
			}
			break;
		case 'w':
			if(head->dir != 2){
			head->dir=0;
			}
			break;
		default:
			return;
	}
	return;
}

int collide_with_self(snake *head){
	snake *tmp;
	tmp = head->next_body;
	while(tmp != NULL){
		if((head->dir==0 && head->y-1==tmp->y && head->x == tmp->x)||
				(head->dir==1 && head->x+1==tmp->x && head->y == tmp->y)||
				(head->dir==2 && head->y+1 == tmp->y && head->x == tmp->x)||
				(head->dir==3 && head->x-1 == tmp->x && head->y == tmp->y)){
			return 1;
		}
		tmp=tmp->next_body;
	}
	return 0;
}
