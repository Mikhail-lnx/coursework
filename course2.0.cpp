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
#define SQUARE_SIDE 20

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

typedef struct vertex{ // структура, содержащая координаты вершины и соответствующие ей номера строки и столбца в массиве field
	int x, y, row, col;
} vertex;

typedef struct edge{ // структура для отрезка траектории, содержащая две вершины и координаты точки для 
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
int is_equal(vertex* node1, vertex* node2);
int is_possible_move(vertex* node1, vertex* node2);
int end_game();
void game_over();

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
         clearviewport();
	for(int i = 1; i <= 8; i++){ // отрисовка поля
		for(int j = 1; j <= 7; j++){
			setcolor(BLACK);
			setlinestyle(SOLID_LINE, 0, 2);
			setfillstyle(SOLID_FILL,BLACK);
			fillellipse(j*INDENT, i*INDENT, 3, 3);
		}
	}
	for(int i = 0; i < 15; i++){ // инициализация массива segment
		for(int j = 0; j < 6 + i%2; j++){
			segment[i][j].v1 = {(j+1)*INDENT, (i/2+1)*INDENT, i+1-i%2, 2*j+1};
			segment[i][j].v2 = {(j+2-(i%2))*INDENT, (i/2+1+i%2)*INDENT, i+1+i%2, 2*j+3-2*(i%2)};
			segment[i][j].left = (segment[i][j].v1.x + segment[i][j].v2.x)/2 - SQUARE_SIDE/2;
			segment[i][j].top = (segment[i][j].v1.y + segment[i][j].v2.y)/2 - SQUARE_SIDE/2;

		}
	}
	setlinestyle(SOLID_LINE, 0, 2);
	start_game();
}

void start_game(){
	edge current_edge;
	while(!end_game()){
		if(check_press(&current_edge)){
			if(beg.x == -1){
				beg = current_edge.v1;
				end = current_edge.v2;
			}
			else if(is_possible_move(&current_edge.v1, &current_edge.v2))
				(is_equal(&current_edge.v1, &beg) ? beg : end) = current_edge.v2;
			else if(is_possible_move(&current_edge.v2, &current_edge.v1))
				(is_equal(&current_edge.v2, &beg) ? beg : end) = current_edge.v1;
            else 
                continue;
			if(move){
				setcolor(RED);
				setfillstyle(SOLID_FILL, RED);
			}
			else{
				setcolor(GREEN);
				setfillstyle(SOLID_FILL, GREEN);
			}
			move = !move;
			line(current.v1.x, current.v1.y, current.v2.x, current.v2.y);
			field[(current.v1.row + current.v2.row)/2][(current.v1.col + current.v2.col)/2] = 1;
		}
	}
	if(move){
		printf("Победил зеленый\n");
	}
	else{
		printf("Победил красный\n");
	}
}

int check_press(edge *current){
	int x, y;
	while(waitclick(&x, &y) != 1)
		;
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 6 + i%2; j++){
			if(x >= segment[i][j].left && x <= segment[i][j].left + SQUARE_SIDE && y >= segment[i][j].top && y <= segment[i][j].top + SQUARE_SIDE){
				*current = segment[i][j];
				return 1;
			}
		}
	}
	return 0;
}

int is_equal(vertex* node1, vertex* node2){ // проверяет, равны ли вершины
	return node1->x == node2->x && node1->y == node2->y;
}

int is_possible_move(vertex* node1, vertex* node2){ // проверяет, является ли вершина node1 крайней точкой траектории и является ли node2 изолированной
	int i = node2->row, j = node2->col;
	return (is_equal(node1, &end) || is_equal(node1, &beg)) && !field[i+1][j] && !field[i-1][j] && !field[i][j-1] && !field[i][j+1];
}

int end_game(){
	int flag = 1;
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 6 + i%2; j++){
			if(beg.x == -1 || is_possible_move(segment[i][j].v1, segment[i][j].v2) || is_possible_move(segment[i][j].v2, segment[i][j].v1))
				flag = 0;
		}
	}
	return flag;
}

void game_over(){
	char s[] = (move ? "зелёный" : "красный");
	setfillstyle(SOLID_FILL, COLOR(255,165,0));
		bar(main_menu->x, main_menu->y, main_menu->x+BUTTON_WIDTH, main_menu->y+BUTTON_HEIGHT);
		moveto(main_menu->x+BUTTON_WIDTH/2, main_menu->y+BUTTON_HEIGHT*2/3);
		setbkcolor(COLOR(255,165,0));
		setcolor(BLACK);
		settextjustify(CENTER_TEXT, CENTER_TEXT);
		settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
		outtext(main_menu->text);
   }

}
