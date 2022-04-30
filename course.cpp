#include <stdio.h>
#include <graphics.h>

/*==========Constant==========*/
#define MENU_WIDTH 400
#define MENU_HEIGHT 600
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 30
/*==========Window game==========*/

typedef struct menu_item{
  int x,y;
  const char *text;
} menu_item;

menu_item main_menu[]={
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 70, "Сложность 1\n(поле 5x6)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 130, "Сложность 2\n(поле 6x7)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 190, "Сложность 3\n(поле 7x8)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 250, "Сложность 4\n(поле 8x9)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 310, "Сложность 5\n(поле 9x10)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 370, "Правила игры"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 430, "О программе"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 490, "Выход"}
};


void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game();
void about();
void rules();

int main(){
   initwindow(MENU_WIDTH, MENU_HEIGHT, "The Slider");
   draw_main_menu();
   getch();
   closegraph();
   return 0;
}

void draw_main_menu(){
   setbkcolor(WHITE);
   clearviewport();
   setbkcolor(GREEN);
   setfillstyle(EMPTY_FILL, GREEN);
   for(int i = 0; i < 8; i++){
	   draw_menu_item(&main_menu[i]);
   }
}

void draw_menu_item(menu_item *main_menu){
  setfillstyle(SOLID_FILL,MAGENTA);
  for(int i = 0; i < 8; i++){
	bar(main_menu->x,main_menu->y,main_menu->x+BUTTON_WIDTH,main_menu->y+BUTTON_HEIGHT);
	moveto(main_menu->x+5,main_menu->y);
	setcolor(YELLOW);
	outtext(main_menu->text);
  } 
}

void new_game(int n){
   return;
}
