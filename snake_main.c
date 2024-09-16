#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
int field_x=75;
int field_y=50;
int score=0;
int scorex;
int scorey;
int body_parts=5;
int alive = 1;
int cur_time = 250;
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
void draw_map(snake *head);
char is_body_render(snake *head, int x, int y);
void randomize_item();
void score_item();
int main(){
	start_game();
	return 0;
}

void start_game(){
	srand(time(NULL));
	randomize_item();
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
	printf("Game over, you got a score of:%d \n", score);
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
		if((head->dir==0 && head->y-1 == scorey && head->x==scorex)||
				(head->dir==1 && head->x+1==scorex && head->y==scorey)||
				(head->dir==2 && head->y+1==scorey && head->x==scorex)||
				(head->dir==3 && head->x-1==scorex && head->y==scorey)){
			score_item();
			make_body(head);
		}
		move_snake(head);
	} else {
		printf("HIT SOMETHING\n");
		alive=0;
	}
	return;
}

void move_snake(snake *head){
	snake *tmp;
	tmp=malloc(sizeof(snake));
	int x=head->x;
	int y=head->y;
	int dir=head->dir;
	tmp = head->next_body;
	int x2=tmp->x;
	int y2=tmp->y;
	int dir2=tmp->dir;
	while(tmp != NULL){
		x2=tmp->x;
		y2=tmp->y;
		dir2=tmp->dir;
		tmp->x=x;
		tmp->y=y;
		tmp->dir=dir;
		tmp=tmp->next_body;
		x=x2;
		y=y2;
		dir=dir2;
	}
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
	return;
}

void user_input(snake *head){
	WINDOW *w;
	char input;
	w=initscr();
	refresh();
	timeout(cur_time);
	input=getch();
	draw_map(head);
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

void draw_map(snake *head){
	clear();
	for(int i=0; i <= field_y; i++){
		for(int j=0; j<=field_x;j++){
			if(i==0 || i== field_y || j==0 || j== field_x){
				printw("#");
			} else {
				if(i==head->y && j==head->x){
					switch(head->dir){
						case 0:
							printw("^");
							break;
						case 1:
							printw(">");
							break;
						case 2:
							printw("v");
							break;
						case 3:
							printw("<");
							break;
						default:
							printw("ERR");
					}
				} else if(j==scorex && i==scorey) {
					printw("*");
				} else {
					printw("%c", is_body_render(head, j, i));
				}
			}
		}
		printw("\n");
	}
	printf("\n");
	return;
}

char is_body_render(snake *head, int x, int y){
	snake *tmp;
	tmp = head->next_body;
	while(tmp != NULL){
		if(x==tmp->x && y==tmp->y){
			if(tmp->dir%2==0){
				return 'I'; 
			} else {
				return '-';
			}
		}
		tmp=tmp->next_body;
	}
	return ' ';
}

void randomize_item(){
	scorex=rand()%field_x;
	scorey=rand()%field_y;
	return;
}

void score_item(){
	score+=1;
	cur_time-=10;
	randomize_item();
}
