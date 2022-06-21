#include <stdio.h>
#include <graphics.h>

/*==========Константы==========*/
#define MENU_WIDTH 220
#define MENU_HEIGHT 190
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define INDENT 50
#define FIELD_WIDTH 400
#define FIELD_HEIGHT 450
#define SQUARE_SIDE 20
#define END_WIDTH 300
#define END_HEIGHT 150
#define RULES_WIDTH 700
#define RULES_HEIGHT 200

typedef struct menu_item {
   int x,y;
   const char *text;
} menu_item;

 
menu_item buttons[]= { // Массив структур menu item, содержащих информацию о  кнопках
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 10, "Новая игра"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 70, "Правила игры"},
   {(MENU_WIDTH-BUTTON_WIDTH)/2, 130, "Выход"},
   {(END_WIDTH-BUTTON_WIDTH)/2, END_HEIGHT-BUTTON_HEIGHT-10, "ОК"},
   {(RULES_WIDTH-BUTTON_WIDTH)/2, RULES_HEIGHT-BUTTON_HEIGHT-10, "ОК"}
};

typedef struct vertex{ // структура, содержащая координаты вершины и соответствующие ей номера строки и столбца в массиве field
	int x, y, row, col;
} vertex;

typedef struct edge{ // структура для отрезка траектории, содержащая две вершины и координаты точки для 
	vertex v1;
	vertex v2;
	int left, top;
} edge;

edge segment[15][7]; // 

int field[17][15]; // массив, содержащий информацию о поле игры
vertex beg = {-1}, end = {-1}; // структуры, содержащие информацию о крайних точках траектории
int move; // переменная, фиксирующая очередь хода
int x, y; // переменные, отвечающие за положение мыши
int is_exit = 0; // Переменная для проверки, произошел выход из программы или нет

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
void show_rules();
void okay_button();

int main(){
   while(!is_exit){
      draw_main_menu();
      new_game(main_menu_button());
   }
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
   for (int i = 0; i < 3; i++) {
      draw_menu_item(&buttons[i]);
   }
} 

void draw_menu_item(menu_item *buttons) {// рисование кнопки главного меню
   setfillstyle(SOLID_FILL, COLOR(255,165,0));
   for (int i = 0; i < 3; i++) {
      bar(buttons->x, buttons->y, buttons->x+BUTTON_WIDTH, buttons->y+BUTTON_HEIGHT);
      moveto(buttons->x+BUTTON_WIDTH/2, buttons->y+BUTTON_HEIGHT*2/3);
      setbkcolor(COLOR(255,165,0));
      setcolor(BLACK);
      settextjustify(CENTER_TEXT, CENTER_TEXT);
      settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
      outtext(buttons->text);
   }
} 

int main_menu_button() {// определение нажатия левой кнопки мыши по одной из кнопок в главном меню
   while(1){
      while(waitclick(&x, &y) != 1);
      for(int i = 0; i < 3; ++i){
         if (x >= buttons[0].x && x <= buttons[0].x + BUTTON_WIDTH && y >= buttons[i].y && y <= buttons[i].y + BUTTON_HEIGHT) {
            return i+1;
         }
      }
   }
} 


void new_game(int n) { // загрузка  новой игры, правил  или выход из программы (в зависимости от того, какую кнопку нажал пользователь)
   closegraph();
   switch(n) {
      case 1:
         init_game();
         return;
      case 2:
         show_rules();
         return;
      case 3:
         is_exit = 1;
         return;
   }
}

void init_game(){ // инициализация параметров игры
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

void start_game(){ // процесс игры(передача хода, рисование отрезка)
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
			line(current_edge.v1.x, current_edge.v1.y, current_edge.v2.x, current_edge.v2.y);
			field[(current_edge.v1.row + current_edge.v2.row)/2][(current_edge.v1.col + current_edge.v2.col)/2] = 1;
		}
	}
	game_over();
}

int check_press(edge *current){ // проверяет, есть ли нажатие между двумя соседними точками
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

int end_game(){ // проверяет, остались ли возможные ходы
	int flag = 1;
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 6 + i%2; j++){
			if(beg.x == -1 || is_possible_move(&segment[i][j].v1, &segment[i][j].v2) || is_possible_move(&segment[i][j].v2, &segment[i][j].v1))
				flag = 0;
		}
	}
	return flag;
}

void game_over(){ // вызов окна окончания игры
   initwindow(END_WIDTH, END_HEIGHT, "The Slider");
   setbkcolor(WHITE);
   clearviewport();
   setcolor(BLACK);
   settextjustify(CENTER_TEXT, CENTER_TEXT);
   settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
   move ? outtextxy(END_WIDTH/2, END_HEIGHT/2-15,"Победил зелёный!") : outtextxy(END_WIDTH/2, END_HEIGHT/2-15,"Победил красный!");
   setfillstyle(SOLID_FILL, COLOR(255,165,0));
   bar(buttons[3].x, buttons[3].y, buttons[3].x+BUTTON_WIDTH, buttons[3].y+BUTTON_HEIGHT);
   setbkcolor(COLOR(255,165,0));
   outtextxy(buttons[3].x+BUTTON_WIDTH/2, buttons[3].y+BUTTON_HEIGHT*2/3, "ОК");
   while(!(waitclick(&x, &y) == 1 && x >= buttons[3].x && x <= buttons[3].x+BUTTON_WIDTH && y >= buttons[3].y && y <= buttons[3].y + BUTTON_HEIGHT))
      ;
   closegraph();
}

void show_rules(){ // вызов окна с правилами игры
   initwindow(RULES_WIDTH, RULES_HEIGHT, "The Slider");
   setbkcolor(WHITE);
   clearviewport();
   setcolor(BLACK);
   settextjustify(LEFT_TEXT,  BOTTOM_TEXT);
   settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
   outtextxy(100, 50, "Игроки по очереди проводят вертикальные или горизонтальные");
   outtextxy(51, 80, "«единичные» отрезки, которые образуют непрерывную ломаную линию.");
   outtextxy(51, 110, "Проигрывает тот игрок, который своим ходом вынужден замкнуть траекторию.");
   setfillstyle(SOLID_FILL, COLOR(255,165,0));
   bar(buttons[4].x, buttons[4].y, buttons[4].x+BUTTON_WIDTH, buttons[4].y+BUTTON_HEIGHT);
   setbkcolor(COLOR(255,165,0));
   settextjustify(CENTER_TEXT,  BOTTOM_TEXT);
   settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
   outtextxy(buttons[4].x+BUTTON_WIDTH/2, buttons[4].y+BUTTON_HEIGHT*2/3, "ОК");
   while(!(waitclick(&x, &y) == 1 && x >= buttons[4].x && x <= buttons[4].x+BUTTON_WIDTH && y >= buttons[4].y && y <= buttons[4].y + BUTTON_HEIGHT))
      ;
   closegraph();
}
