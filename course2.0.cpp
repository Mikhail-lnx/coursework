#include <stdio.h>
#include <graphics.h>

/*==========Константы==========*/
#define MENU_WIDTH 220
#define MENU_HEIGHT 250
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define INDENT 50
#define FIELD_WIDTH 400
#define FIELD_HEIGHT 450
#define SQUARE_SIDE 10

typedef struct menu_item {
   int x,y;
   const char *text;
} menu_item;

 
menu_item main_menu[]= { // Массив структур menu item, содержащих информацию о кнопках главного меню
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 10, "Новая игра"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 70, "Правила игры"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 130, "О программе"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 190, "Выход"}
};

typedef struct vertex{
	int x, y, row, col;
} vertex;

typedef struct edge{
	vertex v1;
	vertex v2;
	int left, top;
} edge;

edge segment[15][7];

int field[17][15]; // Массив, содержащий информацию о поле игры
vertex beg = {-1}, end = {-1}; // Структуры, содержащие информацию о крайних точках траектории
int move; // Переменная, фиксирующая очередь хода

int waitclick(int *x, int *y);
int main_menu_button();
void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game(int n);
void init_game();
void start_game();
int check_press(edge *current);
int is_isolated(vertex node);
int is_equal(vertex node1, vertex node2);
void make_move(edge current);

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

void init_game(){
	for(int j = 0; j < 17; j++) for(int k = 0; k < 15; k++) field[j][k] = ((j >= 1 && j < 16 && k >= 1 && k < 14 && j % 2 == k % 2) ? 1 : 0);
	move = 0;
	initwindow(FIELD_WIDTH, FIELD_HEIGHT, "The Slider");
	setbkcolor(WHITE);
	for(int i = 1; i <= 8; i++){ // отрисовка поля
		for(int j = 1; j <= 7; j++){
			setcolor(BLACK);
			setlinestyle(SOLID_LINE, 0, 2);
			setfillstyle(SOLID_FILL,BLACK);
			fillellipse(j*INDENT, i*INDENT, 2, 2);
		}
	}
	for(int i = 0; i < 15; i++){ // инициализация массива segment
		for(int j = 0; j < 6 + i%2; j++){
			segment[i][j].v1 = {(j+1)*INDENT, (i/2+1)*INDENT, i+1, 2*j+1};
			segment[i][j].v2 = {(j+2-(i%2))*INDENT, (i/2+1+i%2)*INDENT, i+1+2*(i%2), 2*j+3-2*(i%2)};
			segment[i][j].left = (j+1+0.5*!(i%2))*INDENT-SQUARE_SIDE/2;
			segment[i][j].top = (i%2 ? i+1.5 : i/2+1)*INDENT-SQUARE_SIDE/2;
		}
	}
	setcolor(GREEN);
	setlinestyle(SOLID_LINE, 0, 2);
	setfillstyle(SOLID_FILL, GREEN);
	start_game();
}

void start_game(){
	int x, y;
	edge current_edge;
	while(1){
		if(check_press(&current_edge)){
			if(beg.x == -1 || (is_equal(current_edge.v1, beg) || is_equal(current_edge.v1, end)) && is_isolated(current_edge.v2) || (is_equal(current_edge.v2, beg) || is_equal(current_edge.v2, end)) && is_isolated(current_edge.v1))
				make_move(current_edge);
			if(beg.x == -1){
				beg = current_edge.v1;
				end = current_edge.v2;
			}
			else if((is_equal(current_edge.v1, beg) || is_equal(current_edge.v1, end)) && is_isolated(current_edge.v2)){
				(is_equal(current_edge.v1, beg) ? beg : end) = current_edge.v2;
			}
			else if((is_equal(current_edge.v2, beg) || is_equal(current_edge.v2, end)) && is_isolated(current_edge.v1)){
				(is_equal(current_edge.v2, beg) ? beg : end) = current_edge.v1;
			}
		}
	}	
}

int check_press(edge *current){
	int x, y;
	while(waitclick(&x, &y) != 1)
		;
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 6 + i%2; j++){
			if(x >= segment[i][j].left && x <= segment[i][j].left + SQUARE_SIDE && y >= segment[i][j].top && y <= segment[i][j].top + SQUARE_SIDE){
				current = &segment[i][j];
				return 1;
			}
		}
	}
	return 0;
}

int is_isolated(vertex node){ // проверяет, изолирована ли вершина
	int i = node.row, j = node.col;
	return !field[i+1][j] && !field[i-1][j] && !field[i][j-1] && !field[i][j+1] ? 1 : 0;
}

int is_equal(vertex node1, vertex node2){ // проверяет, равны ли вершины
	return node1.x == node2.x && node1.y == node2.y ? 1 : 0;
}

void make_move(edge current){ // рисование отрезка и передача хода
	if(move)
		setfillstyle(SOLID_FILL, RED);
	else
		setfillstyle(SOLID_FILL, GREEN);
	move = !move;
	line(current.v1.x, current.v1.y, current.v2.x, current.v2.y);
	field[(current.v1.row + current.v2.row)/2][(current.v1.col + current.v2.col)/2] = 1;
}
