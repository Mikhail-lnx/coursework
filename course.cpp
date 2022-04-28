#include <stdio.h>
#include "graphics.h"

/*==========Constant==========*/
#define WIDTH 800
#define HEIGHT 600
#define BUTTON_WIDTH 
#define BUTTON_HEIGTH
/*==========Window game==========*/

typedef struct menu_item{ 
  int x,y,w,h;
  const char *text;
} menu_item;

menu_item main_menu[5]={
  {(MAXWIDTH-MENU_WIDTH)/2,70,MENU_WIDTH,40,"Игра с компьютером"},
  {(MAXWIDTH-MENU_WIDTH)/2,130,MENU_WIDTH,40,"Игра по сети(два человека)"},
  {(MAXWIDTH-MENU_WIDTH)/2,190,MENU_WIDTH,40,"Правила игры"},
  {(MAXWIDTH-MENU_WIDTH)/2,250,MENU_WIDTH,40,"О программе"},
  {(MAXWIDTH-MENU_WIDTH)/2,310,MENU_WIDTH,40,"Выход"}
};

void field(int n);
void init();
void menu();
void new_game();
void about();
void rules();

int main() {
   initwindow(WIDTH, HEIGHT, "The Slider");
   menu();
   getch();
   closegraph();
   return 0;
}

void menu() {
   setbkcolor(WHITE);
   clearviewport();
   setbkcolor(GREEN);
   setfillstyle(EMPTY_FILL, GREEN);
   bar(450, 100, 650, 200);
   bar(450, 250, 650, 350);
   bar(450, 400, 650, 500);
}

