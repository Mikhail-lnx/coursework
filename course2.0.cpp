#include <stdio.h>
#include <graphics.h>

/*==========Константы==========*/
#define MENU_WIDTH 250
#define MENU_HEIGHT 350
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define INDENT_FIELD 30
 
typedef struct menu_item {
   int x,y;
   const char *text;
} menu_item;

 
menu_item main_menu[]= { // Массив структур menu item, содержащих информацию о кнопках главного меню
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 60, "Новая игра"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 120, "Правила игры"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 180, "О программе"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 240, "Выход"}
};

int field[17][15]; // Массив, содержащий информацию о поле игры
int ends[2][2] = {-1}; // Массив, содержащий координаты концов траектории
int move; // Переменная, фиксирующая очередь хода

int waitclick(int *x1, int *y1, int *x2, int *y2);
int main_menu_button();
void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game(int n);
void init_game();
void game_begin(int i, int dx, int dy);
void draw_segment();

int main(){
   draw_main_menu();
   new_game(main_menu_button());
   return 0;
}


int waitclick(int *x1, int *y1) {// определение нажатия кнопки на мыши
   int mouse;
   while ((mouse=mousebuttons())==0) { 
      if (kbhit()) return 0; 
   }
   *x1=mousex();
   *y1=mousey();
   while (mousebuttons()==0);
   return mouse;
} 

void draw_main_menu() {// рисование главного меню
   initwindow(MENU_WIDTH, MENU_HEIGHT, "The Slider");
   setbkcolor(WHITE);
   clearviewport();
   for (int i = 0; i < 4; i++) {
      draw_menu_item(&main_menu[i]);
   }
} 

void draw_menu_item(menu_item *main_menu) {// рисование кнопки главного меню
   setfillstyle(SOLID_FILL, COLOR(255,165,0));
   for (int i = 0; i < 4; i++) {
      bar(main_menu->x, main_menu->y, main_menu->x+BUTTON_WIDTH, main_menu->y+BUTTON_HEIGHT);
      moveto(main_menu->x+BUTTON_WIDTH/2, main_menu->y+BUTTON_HEIGHT*2/3);
      setbkcolor(COLOR(255,165,0));
      setcolor(BLACK);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
      outtext(main_menu->text);
   }
} 

int main_menu_button() {// определение нажатия левой кнопки мыши по одной из кнопок в главном меню
   int x1, y1;
   while(1){
      while(waitclick(&x1, &y1) != 1);
      for(int i = 0; i < 4; ++i){
         if (x1 >= main_menu[0].x && x1 <= main_menu[0].x + BUTTON_WIDTH && y1 >= main_menu[i].y && y1 <= main_menu[i].y + BUTTON_HEIGHT) {
            return i+1;
         }
      }
   }
} 


void new_game(int n) { // загрузка  новой игры, правил, информации о программе или выход из программы (в зависимости от того, какую кнопку нажал пользователь)
   closegraph();
   switch(n) {
      case 1:
         init_game();
         return;
      case 2:
         return;
      case 3:
         return;
      case 4:
         return;
   }
}



