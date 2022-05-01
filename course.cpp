#include <stdio.h>
#include <graphics.h>

/*==========Constant==========*/
#define MENU_WIDTH 250 
#define MENU_HEIGHT 580
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
/*==========Window game==========*/

typedef struct menu_item{
  int x,y;
  const char *text;
} menu_item;

menu_item main_menu[]={
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 50, "поле 5x6"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 110, "поле 6x7"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 170, "поле 7x8"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 230, "поле 8x9"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 290, "поле 9x10"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 350, "Правила игры"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 410, "О программе"},
  {(MENU_WIDTH-BUTTON_WIDTH)/2, 470, "Выход"}
};


int waitclick(int *x1, int *y1, int *x2, int *y2);
int press_left_button();
void draw_main_menu();
void draw_menu_item(menu_item *main_menu);
void new_game(int n);
void about();
void rules();

int main(){
   draw_main_menu();
   new_game(press_left_button());
   return 0;
}


int waitclick(int *x1, int *y1, int *x2, int *y2){
  int mouse;
  while((mouse=mousebuttons())==0) { // ждем нажатия кнопки
     if(kbhit()) return 0; // если нажата клавиша - выйти
  }
  *x1=mousex();
  *y1=mousey();
  while(mousebuttons()==0); // ждем отпускания кнопки
  *x2=mousex();
  *y2=mousey();
  return mouse;
} // определение нажатия кнопки на мыши

void draw_main_menu(){
   initwindow(MENU_WIDTH, MENU_HEIGHT, "The Slider");
   setbkcolor(WHITE);
   clearviewport();
   for(int i = 0; i < 8; i++){
	   draw_menu_item(&main_menu[i]);
   }
} // рисование главного меню

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
} // рисование кнопки

int press_left_button(){
	int x1, x2, y1, y2;
	while(1){
		while(waitclick(&x1, &y1, &x2, &y2) != 1 || x1 != x2 || y1 != y2);
		for(int i = 0; i < 8; ++i){
			if(x1 >= main_menu[0].x && x1 <= main_menu[0].x + BUTTON_WIDTH && y1 >= main_menu[i].y && y1 <= main_menu[i].y + BUTTON_HEIGHT){
				return i+1;
			}
		}
	} 
} // определение нажатия левой кнопки мыши по одной из кнопок в главном меню


void new_game(int n){
   closegraph();
   IMAGE *pic1, *pic2, *pic3, *pic4, *pic5;
   switch(n){
         case 1:
            initwindow(480, 320);
            pic1 = loadBMP("field1.bmp");
            putimage(10, 10, pic1, OR_PUT);
            return;
         case 2:
			initwindow(580, 370);
            return;
         case 3:
			initwindow(680, 420);
            return;
         case 4:
			initwindow(780, 470);
            return;
         case 5:
			initwindow(880, 520);
            return;
         case 6:
            return;
         case 7:
            return;
         case 8:
            closegraph();
            return;
   }
}
