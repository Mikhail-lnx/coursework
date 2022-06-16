#include <stdio.h>
#include <graphics.h>

/*==========Константы==========*/
#define MENU_WIDTH 250
#define MENU_HEIGHT 580
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define INDENT_FIELD 30
 
typedef struct menu_item {
   int x,y;
   const char *text;
} menu_item;

menu_item main_menu[]= { // Массив структур menu item, содержащих информацию о кнопках главного меню
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 50, "Поле 5x6"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 110, "Поле 6x7"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 170, "Поле 7x8"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 230, "Поле 8x9"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 290, "Поле 9x10"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 350, "Правила игры"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 410, "О программе"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 470, "Выход"}
};

int field[21][19]; // Массив, содержащий информацию о поле игры
int ends[2][2] = {-1}; // Массив, содержащий координаты концов траектории
int move; // Переменная, фиксирующая очередь хода

int waitclick(int *x1, int *y1, int *x2, int *y2);
int main_menu_button();
void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game(int n);
void init_game(int i);
void game_begin(int i, int dx, int dy, int *field);
void draw_segment();
void between_dots(int *x, int *y, int dx, int dy);

int main() {
   draw_main_menu();
   new_game(main_menu_button());
   while (1);
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
   for (int i = 0; i < 8; i++) {
      draw_menu_item(&main_menu[i]);
   }
} 

void draw_menu_item(menu_item *main_menu) {// рисование кнопки главного меню
   setfillstyle(SOLID_FILL, COLOR(255,165,0));
   for (int i = 0; i < 8; i++) {
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
      for(int i = 0; i < 8; ++i){
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
}
void init_game(int i){ // инициализация параметров игры
	for(int j = 0; j < 2*i+11; j++) for(int k = 0; k < 2*i+9; k++) field[j][k] = ((j >= 1 && j < 2*i+10 && k >= 1 && k < 2*i+8 && j % 2 == k % 2) ? 1 : 0);
	int dx = (220+50*i-2*INDENT_FIELD)/(i+3), dy = (270+50*i-2*INDENT_FIELD)/(i+4);
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
			fillellipse(INDENT_FIELD + k*dx, INDENT_FIELD + j*dy, 3, 3);
		}
	}
    game_begin(i, dx, dy);
	
}

void game_begin(int i, int dx, int dy){ // процесс игры(проверка и выделение хода); если ход невозможен, функция завершит работу; возвращает 1 или 0, в зависимости от того, какой игрок победил
	int x, y, vertices[2][2];
	move = 0;
	while(1){
		if(waitclick(x, y) == 1 && !((x-INDENT_FIELD) % dx) != !((y-INDENT_FIELD) % dy) && x > INDENT_FIELD && x < INDENT_FIELD + dx*(i+3) && y > INDENT_FIELD && y < INDENT_FIELD + dy*(i+4)){
			if((x-INDENT_FIELD)%dx){
				vertices[0][0] = x-(x-INDENT_FIELD)%dx;
				vertices[0][1] = y;
				vertices[1][0] = vertices[0][0] + dx;
				vertices[1][1] = y;
			}
			else{
				vertices[0][0] = x;
				vertices[0][1] = y-(y-INDENT_FIELD)%dy;
				vertices[1][0] = x;
				vertices[1][1] = vertices[0][1] + dy;
			}
			if(ends[0][0] == -1){
				field[x][y] = 1;
				ends[0][0] = x - (x%dx != 0);
				ends[0][1] = y - (y%dy != 0);
				ends[1][0] = x + (x%dx != 0);
				ends[1][1] = y + (y%dy != 0);
				setcolor(GREEN);
				setlinestyle(SOLID_LINE, 0, 2);
				setfillstyle(SOLID_FILL, GREEN);
				line();
			}

		}		
	}
}

void between_dots(int *x, int *y, int dx, int dy){
	while(!(waitclick(x, y) == 1 && !((x-INDENT_FIELD) % dx) != !((y-INDENT_FIELD) % dy) && x > INDENT_FIELD && x < INDENT_FIELD + dx*(i+3) && y > INDENT_FIELD && y < INDENT_FIELD + dy*(i+4)));
}
