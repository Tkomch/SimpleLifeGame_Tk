#include "LifeGame.h"

int main(int arc, char** arv) {
	Uint8 running = 1;
	Uint8 switch_auto = 0;
	Uint8 switch_manual = 0;
	Uint32 time = 500;
	Cell* cells = NULL;
	Controler* controler = NULL;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	int i, j;
	SDL_Rect title_rect; title_rect.x = 200; title_rect.y = 200;
	title_rect.w = 200; title_rect.h = 200;
	cells = (Cell*)malloc(sizeof(Cell));
	controler = (Controler*)malloc(sizeof(Controler));
	//init
	controler->mode = 0; controler->alone = 0; controler->hunger = 3; controler->speed = 0;
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	if (!(font = TTF_OpenFont("C:/Windows/Fonts/msyhbd.ttc", 50))) printf("font NULL!");
	if (!(button_font = TTF_OpenFont("C:/Windows/Fonts/msyhbd.ttc", 33))) printf("font NULL!");
	if (!(num_font = TTF_OpenFont("C:/Windows/Fonts/msyhbd.ttc", 15))) printf("font NULL!");
	window = SDL_CreateWindow("Tkomch Life Game", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			1000, 600,
			SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderClear(renderer);
	//绘制cell面板
	DrawCellPanel(window, renderer, cells);
	//绘制control面板
	DrawControlPanel(window, renderer, controler);
	//事件处理
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT){
					//点击元胞面板流程图
					for (i = 1; i < 24; i++) {
						for (j = 1; j < 24; j++) {
							if (IsInRect(&event, &cells->cell_panel[i][j])) {
								cells->cell_store[i][j] = !cells->cell_store[i][j];
								CellToStore(window, renderer, cells);
							}
						}
					}
					//点击切换流程
					if (IsInRect(&event, &controler->switch_rect)) {
						controler->mode = !controler->mode;
						if (!controler->mode) switch_auto = 0;
						RedrawControlPanel(window, renderer, controler);
					}
					//点击开始的流程
					if (IsInRect(&event, &controler->next_step_rect)) {
						if (!controler->mode) {
							switch_manual = 1;
						}
						else switch_auto = 1;
					}
					//点击孤独数字
					for (i = 0; i < 7; i++) {
						if (IsInRect(&event, &controler->alone_choose[i])) {
							controler->alone = i;
							RedrawAlonePanel(window, renderer, controler);
						}
					}
					//点击饥饿数字
					for (i = 2; i < 9; i++) {
						if (IsInRect(&event, &controler->hunger_choose[i])) {
							controler->hunger = i;
							RedrawHungerPanel(window, renderer, controler);
						}
					}
					//点击速度
					//-
					if (IsInRect(&event, &controler->speed_control[1])) {
						if ((time + 50) <= 500) time += 50;
					}
					//+
					if (IsInRect(&event, &controler->speed_control[2])) {
						if ((time - 50) >= 50) time -= 50;
					}
				}
				break;
			default:
				break;
			}
			if (switch_manual) {
				CellReproduct(cells, controler);
				CellToStore(window, renderer, cells);
				switch_manual = 0;
			}
			if (switch_auto) {
				CellReproduct(cells, controler);
				CellToStore(window, renderer, cells);
				SDL_Delay(time);
			}
			
			SDL_RenderPresent(renderer);
		}
	}
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}

void DrawCellPanel(SDL_Window* w, SDL_Renderer* r, Cell* c) {
	int i, j, loc_x, loc_y;
	SDL_Texture* cell_panel;
	SDL_Surface* screen = SDL_GetWindowSurface(w);
	for (i = 1, loc_x = 100; i < 24; i++, loc_x+=20) 
		for (j = 1, loc_y = 100; j < 24; j++, loc_y+=20) {
			c->cell_panel[i][j].w = 15; c->cell_panel[i][j].h = 15;
			c->cell_panel[i][j].x = loc_x; c->cell_panel[i][j].y = loc_y;
			SDL_FillRect(screen, &c->cell_panel[i][j], 
				SDL_MapRGB(screen->format, 0, 200, 0));
			c->cell_store[i][j] = 0;
		}
	cell_panel = SDL_CreateTextureFromSurface(r, screen);
	SDL_FreeSurface(screen);
	SDL_RenderCopy(r, cell_panel, NULL, NULL);
	SDL_DestroyTexture(cell_panel);
}
//Display ControlPanel
void DrawControlPanel(SDL_Window* w, SDL_Renderer* r, Controler* c) {
	int i, x;
	SDL_Surface* screen = SDL_GetWindowSurface(w);
	SDL_Texture* control_panel = NULL;
	SDL_Texture* mode_text_texture = NULL;
	//绘制Rect
	c->mode_display.w = 200; c->mode_display.h = 70;
	c->mode_display.x = 770; c->mode_display.y = 90;
	c->switch_rect.w = 100; c->switch_rect.h = 50;
	c->switch_rect.x = 630; c->switch_rect.y = 85;
	c->next_step_rect.w = 100; c->next_step_rect.h = 50;
	c->next_step_rect.x = 630; c->next_step_rect.y = 180;
	c->reset_rect.w = 100; c->reset_rect.h = 50;
	c->reset_rect.x = 820; c->reset_rect.y = 180;
	for (i = 0, x = 620; i < 7; i++, x += 50) {
		//set孤独选择Rect location
		c->alone_choose[i].x = x; c->alone_choose[i].y = 340;
		c->alone_choose[i].w = 20; c->alone_choose[i].h = 20;
	}
	for (i = 2, x = 620; i < 9; i++, x += 50) {
		c->hunger_choose[i].x = x; c->hunger_choose[i].y = 440;
		c->hunger_choose[i].w = 20; c->hunger_choose[i].h = 20;
	}
	for (i = 1, x = 750; i < 3; i++, x += 100) {
		c->speed_control[i].w = 40; c->speed_control[i].h = 40;
		c->speed_control[i].x = x; c->speed_control[i].y = 500;
	}
	//数字板绘制
	SDL_FillRects(screen, &c->alone_choose[1], 7,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->alone_choose[c->alone],
		SDL_MapRGB(screen->format, 0x77, 0x00, 0x22));
	SDL_FillRects(screen, &c->hunger_choose[2], 7,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->hunger_choose[c->hunger],
		SDL_MapRGB(screen->format, 0x77, 0x00, 0x22));

	SDL_FillRects(screen, &c->speed_control[1], 2,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->switch_rect,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->next_step_rect,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->reset_rect,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->mode_display,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	control_panel = SDL_CreateTextureFromSurface(r, screen);
	SDL_FreeSurface(screen);
	SDL_RenderCopy(r, control_panel, NULL, NULL);
	//绘制标题
	SetTextLocation(w, r, font, color, title_text, 5, 5);
	SetTextLocation(w, r, button_font, button_color, switch_text, 633, 85);  //切换
	//手动模式
	SetTextLocation(w, r, button_font, button_color, next_text, 630, 180);   //下一步
	SetTextLocation(w, r, font, color, manual_mode_text, 770, 90);
	SetTextLocation(w, r, button_font, button_color, reset_text, 823, 180);  //重置
	for (i = 0, x = 622; i < 7; i++, x += 50) {
		SetTextLocation(w, r, num_font, button_color, number[i], x, 340);
	}
	for (i = 2, x = 622; i < 9; i++, x += 50) {
		SetTextLocation(w, r, num_font, button_color, number[i], x, 440);
	}
	for (i = 1, x = 755; i < 3; i++, x += 100) {
		SetTextLocation(w, r, button_font, button_color, control_icon[i], x, 500);
	}
	SetTextLocation(w, r, button_font, color, des_alone_text, 580, 290);
	SetTextLocation(w, r, button_font, color, des_hunger_text, 580, 390);
	SetTextLocation(w, r, button_font, color, speed_text, 620, 500);
	SDL_DestroyTexture(control_panel);
}

void CellToStore(SDL_Window* w, SDL_Renderer* r, Cell* c) {
	int i, j, loc_x, loc_y;
	SDL_Texture* cell_panel;
	SDL_Surface* screen = SDL_GetWindowSurface(w);
	for (i = 1, loc_x = 100; i < 24; i++, loc_x += 20)
		for (j = 1, loc_y = 100; j < 24; j++, loc_y += 20) {
			c->cell_panel[i][j].w = 15; c->cell_panel[i][j].h = 15;
			c->cell_panel[i][j].x = loc_x; c->cell_panel[i][j].y = loc_y;
			if (c->cell_store[i][j]) {
				SDL_FillRect(screen, &c->cell_panel[i][j], 
					SDL_MapRGB(screen->format, 0, 0, 200));
			}
			else{
				SDL_FillRect(screen, &c->cell_panel[i][j], 
					SDL_MapRGB(screen->format, 0, 200, 0));
			}
		}
	cell_panel = SDL_CreateTextureFromSurface(r, screen);
	SDL_FreeSurface(screen);
	SDL_RenderCopy(r, cell_panel, NULL, NULL);
	SDL_DestroyTexture(cell_panel);
}

void RedrawControlPanel(SDL_Window* w, SDL_Renderer* r, Controler* c) {
	//判断mode来重绘控制面板
	SDL_Surface* screen = SDL_GetWindowSurface(w);
	SDL_FillRect(screen, &c->mode_display,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->next_step_rect,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	if (!c->mode) {
		SetTextLocation(w, r, font, color, manual_mode_text, 770, 90);
		SetTextLocation(w, r, button_font, button_color, next_text, 630, 180);   //下一步
	}
	else {
		SetTextLocation(w, r, font, color, auto_mode_text, 770, 90);
		SetTextLocation(w, r, button_font, button_color, start_text, 630, 180);
	}
}

void SetTextLocation(SDL_Window* w, SDL_Renderer* r, TTF_Font* font, SDL_Color color,
	wchar_t* text, int x, int y) {
	SDL_Surface* screen = NULL;
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect rect;
	rect.x = x; rect.y = y;
	rect.w = 300; rect.h = 150;
	screen = SDL_GetWindowSurface(w);
	SDL_BlitSurface((surface = TTF_RenderUNICODE_Solid(font, text, color)), NULL, screen, &rect);
	texture = SDL_CreateTextureFromSurface(r, screen);
	SDL_RenderCopy(r, texture, NULL, NULL);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(screen);
}
//Rect x on upper left corner, y too.
Uint8 IsInRect(SDL_Event* e, SDL_Rect* r) {
	Uint8 i;
	if ((e->button.x >= (r->x)) &&
		(e->button.x <= (r->x + r->w)) &&
		(e->button.y >= (r->y)) &&
		(e->button.y <= (r->y + r->h)))
		i = 1;
	else i = 0;
	return i;
}
//元胞衍化函数
void CellReproduct(Cell* cell, Controler* control) {
	int i, j, account = 0;
	//将值赋给老cell
	for (i = 1; i < 24; i++) {
		for (j = 1; j < 24; j++) {
			cell->old_cell_store[i][j] = cell->cell_store[i][j];
		}
	}
	//判断老cell绘制新cell
	for (i = 1; i < 24; i++) {
		for (j = 1; j < 24; j++, account = 0) {
			//upperleft
			if (isCellLive(i - 1, j - 1)) account++;
			if (isCellLive(i - 1, j)) account++;
			if (isCellLive(i - 1, j + 1)) account++;
			if (isCellLive(i + 1, j)) account++;
			if (isCellLive(i + 1, j + 1)) account++;
			if (isCellLive(i + 1, j - 1)) account++;
			if (isCellLive(i, j + 1)) account++;
			if (isCellLive(i, j - 1)) account++;
			if ((account > control->alone) && (account < control->hunger))
				cell->cell_store[i][j] = 1;
			else cell->cell_store[i][j] = 0;
		}
	}
}
//重绘孤独板
void RedrawAlonePanel(SDL_Window* w, SDL_Renderer* r, Controler* c) {
	int i, x;
	SDL_Surface* screen = SDL_GetWindowSurface(w);
	SDL_FillRects(screen, &c->alone_choose[0], 7,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->alone_choose[c->alone],
		SDL_MapRGB(screen->format, 0x77, 0x00, 0x22));
	for (i = 0, x = 622; i < 7; i++, x += 50) {
		SetTextLocation(w, r, num_font, button_color, number[i], x, 340);
	}
}
//重绘饥饿板
void RedrawHungerPanel(SDL_Window* w, SDL_Renderer* r, Controler* c) {
	int i, x;
	SDL_Surface* screen = SDL_GetWindowSurface(w);
	SDL_FillRects(screen, &c->hunger_choose[2], 7,
		SDL_MapRGB(screen->format, 0x22, 0xff, 0x22));
	SDL_FillRect(screen, &c->hunger_choose[c->hunger],
		SDL_MapRGB(screen->format, 0x77, 0x00, 0x22));
	SDL_RenderCopy(r, screen, NULL, NULL);
	for (i = 2, x = 622; i < 9; i++, x += 50) {
		SetTextLocation(w, r, num_font, button_color, number[i], x, 440);
	}
}