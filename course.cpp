#include <stdio.h>
#include <graphics.h>

/*==========Константы==========*/
#define MENU_WIDTH 250
#define MENU_HEIGHT 580
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define INDENT_FIELD 20
 
typedef struct menu_item {
   int x,y;
   const char *text;
} menu_item;

menu_item main_menu[]= {
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 50, "пj�ле 5x6"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 110, "поле 6x7"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 170, "поле 7x8"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 230, "поле 8x9"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 290, "поле 9x10"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 350, "Правила игры"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 410, "О программе"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 470, "Выход"}
}; // Массив структур menu item, содержащих информацию о кнопках главного меню


int waitclick(int *x1, int *y1, int *x2, int *y2);
int press_left_button();
void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game(int n);
void init_game(int i);


int main() {
   draw_main_menu();
   new_game(press_left_button());
   while (1);
   return 0;
}


int waitclick(int *x1, int *y1, int *x2, int *y2) {
   int mouse;
   while ((mouse=mousebuttons())==0) { 
      if (kbhit()) return 0; 
   }
   *x1=mousex();
   *y1=mousey();
   while (mousebuttons()==0); 
   *x2=mousex();
   *y2=mousey();
   return mouse;
} // определение нажатия кнопки на мыши

void draw_main_menu() {
   initwindow(MENU_WIDTH, MENU_HEIGHT, "The Slider");
   setbkcolor(WHITE);
   clearviewport();
   for (int i = 0; i < 8; i++) {
      draw_menu_item(&main_menu[i]);
   }
} // рисование главного меню

void draw_menu_item(menu_item *main_menu) {
   setfillstyle(SOLID_FILL, COLOR(255,165,0));
   for (int i = 0; i < 8; i++) {
      bar(main_menu->x,main_menu->y,main_menu->x+BUTTON_WIDTH,main_menu->y+BUTTON_HEIGHT);
      moveto(main_menu->x+BUTTON_WIDTH/2,main_menu->y+BUTTON_HEIGHT*2/3);
      setbkcolor(COLOR(255,165,0));
      setcolor(BLACK);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
      outtext(main_menu->text);
   }
} // рисование кнопки главного меню

int press_left_button() {
   int x1, x2, y1, y2;
   while (1) {
      while (waitclick(&x1, &y1, &x2, &y2) != 1 || x1 != x2 || y1 != y2);
      for (int i = 0; i < 8; ++i) {
         if (x1 >= main_menu[0].x && x1 <= main_menu[0].x + BUTTON_WIDTH && y1 >= main_menu[i].y && y1 <= main_menu[i].y + BUTTON_HEIGHT) {
            return i+1;
         }
      }
   }
} // определение нажатия левой кнопки мыши по одной из кнопок в главном меню


void new_game(int n) {
   closegraph();
   IMAGE *pic;
   switch (n) {
   case 1:
	  init_game(1);
      return;
   case 2:
	  init_game(2);
      return;
   case 3:
      init_game(3);
	  return;
   case 4:
      init_game(4);
	  return;
   case 5:
      init_game(5);
	  return;
   case 6:
      return;
   case 7:
      return;
   case 8:
      return;
   }
} // загрузка  новой игры, правил, информации о программе или выход из программы (в зависимости от того, какую кнопку нажал пользователь)
void init_game(int i){
	initwindow(430+50*i, 270+50*i);
	setbkcolor(BLUE);
	clearviewport();
	bar(10, 10, 210+50*i, 260+50*i);
	setbkcolor(WHITE);	
	for(int j = 0; j < i+5; j++){
		for(int k = 0; k < i+4; k++){
			setcolor(BLACK);
			setlinestyle(SOLID_LINE, 0, 2);
			setfillstyle(SOLID_FILL,BLACK);
			fillellipse(10 + INDENT_FIELD + k*(200+50*i-2*INDENT_FIELD)/(i+3), 10 + INDENT_FIELD + j*(250+50*i-2*INDENT_FIELD)/(i+4), 3, 3);
		}
	 
	} // инициализация параметров игры
}
