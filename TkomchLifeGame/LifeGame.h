#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "SDL.h"
#include "SDL_ttf.h"

#define isCellLive(x, y) &cell->old_cell_store[x][y] && cell->old_cell_store[x][y]

typedef struct Cells {
	SDL_Rect cell_panel[24][24];
	Uint8 old_cell_store[24][24];
	Uint8 cell_store[24][24];
} Cell;

typedef struct Control {
	SDL_Rect reset_rect;    //重置
	SDL_Rect next_step_rect;    //开始||下一步
	SDL_Rect switch_rect;   //切换
	SDL_Rect mode_display;   //显示模式
	Uint8 mode;
	Uint8 alone;
	SDL_Rect alone_choose[8];
	Uint8 hunger;
	SDL_Rect hunger_choose[9];
	float speed;
	SDL_Rect speed_control[3];
} Controler;

//显示模块
void DrawCellPanel(SDL_Window* w, SDL_Renderer* r, Cell* c);
void DrawControlPanel(SDL_Window* w, SDL_Renderer* r, Controler* c);

//刷新面板
void CellToStore(SDL_Window* w, SDL_Renderer* r, Cell* c);
void RedrawControlPanel(SDL_Window* w, SDL_Renderer* r, Controler* c);
void RedrawAlonePanel(SDL_Window* w, SDL_Renderer* r, Controler* c);
void RedrawHungerPanel(SDL_Window* w, SDL_Renderer* r, Controler* c);
//元胞衍化
void CellReproduct(Cell* cell, Controler* control);

void SetTextLocation(SDL_Window* w, SDL_Renderer* r, TTF_Font* font, SDL_Color color, 
	wchar_t* text, int x, int y);
Uint8 IsInRect(SDL_Event* e, SDL_Rect* r);


SDL_Color color = { 0xaa, 0x70, 0xff };
SDL_Color button_color = { 0x22, 0x33, 0x22 };
TTF_Font* font = NULL;
TTF_Font* button_font = NULL;
TTF_Font* num_font = NULL;
//字符串数据
wchar_t *auto_mode_text = L"自动模式";
wchar_t *manual_mode_text = L"手动模式";
wchar_t *title_text = L"生命游戏";
wchar_t *switch_text = L"切  换";
wchar_t *next_text = L"下一步";
wchar_t *reset_text = L"重  置";
wchar_t *start_text = L"开  始";
wchar_t *number[11] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
wchar_t *speed_text = L"速  度:";
wchar_t *control_icon[3] = { "", "<", ">" };
wchar_t *des_alone_text = L"周围此数量时孤独死";
wchar_t *des_hunger_text = L"周围此数量时饥饿死";

