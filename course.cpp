#include <stdio.h>
#include "graphics.h"

/*==========Constant==========*/
#define MAXWIDTH 800
#define HEIGHT 600
#define MENU_WIDTH 600
/*==========Window game==========*/

typedef struct menu_item{ 
  int x,y,w,h;
  const char *text;
} menu_item;

menu_item main_menu[]={
  {(MAXWIDTH-MENU_WIDTH)/2,70,MENU_WIDTH,40,"Сложность 1\n(поле 5x6)"},
  {(MAXWIDTH-MENU_WIDTH)/2,130,MENU_WIDTH,40,"Сложность 2\n(поле 6x7)"},
  {(MAXWIDTH-MENU_WIDTH)/2,190,MENU_WIDTH,40,"Сложность 3\n(поле 7x8)"},
  {(MAXWIDTH-MENU_WIDTH)/2,250,MENU_WIDTH,40,"Сложность 4\n(поле 8x9)"},
  {(MAXWIDTH-MENU_WIDTH)/2,310,MENU_WIDTH,40,"Сложность 5\n(поле 9x10)"},
  {(MAXWIDTH-MENU_WIDTH)/2,370,MENU_WIDTH,40,"Правила игры"},
  {(MAXWIDTH-MENU_WIDTH)/2,430,MENU_WIDTH,40,"О программе"},
  {(MAXWIDTH-MENU_WIDTH)/2,490,MENU_WIDTH,40,"Выход"}
};

void draw_menu_item();
void draw_main_menu();
void new_game();
void about();
void rules();

int main() {
   initwindow(MAXWIDTH, HEIGHT, "The Slider");
   draw_main_menu();
   getch();
   closegraph();
   return 0;
}

void draw_main_menu() {
   setbkcolor(WHITE);
   clearviewport();
   setbkcolor(GREEN);
   setfillstyle(EMPTY_FILL, GREEN);
   bar(450, 100, 650, 200);
   bar(450, 250, 650, 350);
   bar(450, 400, 650, 500);
}

void new_game(int n){
   return;
}
