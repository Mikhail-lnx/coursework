#include <stdio.h>
#include <graphics.h>

/*==========Constant==========*/
#define MENU_WIDTH 400
#define MENU_HEIGHT 580
#define BUTTON_WIDTH 350
#define BUTTON_HEIGHT 50
/*==========Window game==========*/

typedef struct menu_item{
  int x,y;
  const char *text;
} menu_item;

menu_item main_menu[]={
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 50, "Сложность 1 (поле 5x6)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 110, "Сложность 2 (поле 6x7)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 170, "Сложность 3 (поле 7x8)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 230, "Сложность 4 (поле 8x9)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 290, "Сложность 5 (поле 9x10)"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 350, "Правила игры"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 410, "О программе"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 470, "Выход"}
};

int waitclick(int *x1, int *y1, int *x2, int *y2);
void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game();
void about();
void rules();

int main(){
   draw_main_menu();
   getch();
   closegraph();
   return 0;
}


int waitclick(int *x1, int *y1, int *x2, int *y2){
  int b;
  while((b=mousebuttons())==0) { // ждем нажатия кнопки
     if(kbhit()) return 0; // если нажата клавиша - выйти
  }
  *x1=mousex();
  *y1=mousey();
  while(mousebuttons()==0); // ждем отпускания кнопки
  *x2=mousex();
  *y2=mousey();
  return b;
}

void draw_main_menu(){
   initwindow(MENU_WIDTH, MENU_HEIGHT, "The Slider");
   setbkcolor(WHITE);
   clearviewport();
   for(int i = 0; i < 8; i++){
	   draw_menu_item(&main_menu[i]);
   }
}

void draw_menu_item(menu_item *main_menu){
  setfillstyle(SOLID_FILL, COLOR(255,165,0));
  for(int i = 0; i < 8; i++){
	bar(main_menu->x,main_menu->y,main_menu->x+BUTTON_WIDTH,main_menu->y+BUTTON_HEIGHT);
	moveto(main_menu->x+BUTTON_WIDTH/2,main_menu->y+BUTTON_HEIGHT*2/3);
        setbkcolor(COLOR(255,165,0));
	setcolor(BLACK);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
	outtext(main_menu->text);
  } 
}

void new_game(int n){
   switch(n){
         case 1:
            return;
         case 2:
            return;
         case 3:
            return;
         case 4:
            return;
         case 5:
            return;
   }
}
