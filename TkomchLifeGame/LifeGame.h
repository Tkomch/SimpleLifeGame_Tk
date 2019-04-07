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
	SDL_Rect reset_rect;    //����
	SDL_Rect next_step_rect;    //��ʼ||��һ��
	SDL_Rect switch_rect;   //�л�
	SDL_Rect mode_display;   //��ʾģʽ
	Uint8 mode;
	Uint8 alone;
	SDL_Rect alone_choose[8];
	Uint8 hunger;
	SDL_Rect hunger_choose[9];
	float speed;
	SDL_Rect speed_control[3];
} Controler;

//��ʾģ��
void DrawCellPanel(SDL_Window* w, SDL_Renderer* r, Cell* c);
void DrawControlPanel(SDL_Window* w, SDL_Renderer* r, Controler* c);

//ˢ�����
void CellToStore(SDL_Window* w, SDL_Renderer* r, Cell* c);
void RedrawControlPanel(SDL_Window* w, SDL_Renderer* r, Controler* c);
void RedrawAlonePanel(SDL_Window* w, SDL_Renderer* r, Controler* c);
void RedrawHungerPanel(SDL_Window* w, SDL_Renderer* r, Controler* c);
//Ԫ���ܻ�
void CellReproduct(Cell* cell, Controler* control);

void SetTextLocation(SDL_Window* w, SDL_Renderer* r, TTF_Font* font, SDL_Color color, 
	wchar_t* text, int x, int y);
Uint8 IsInRect(SDL_Event* e, SDL_Rect* r);


SDL_Color color = { 0xaa, 0x70, 0xff };
SDL_Color button_color = { 0x22, 0x33, 0x22 };
TTF_Font* font = NULL;
TTF_Font* button_font = NULL;
TTF_Font* num_font = NULL;
//�ַ�������
wchar_t *auto_mode_text = L"�Զ�ģʽ";
wchar_t *manual_mode_text = L"�ֶ�ģʽ";
wchar_t *title_text = L"������Ϸ";
wchar_t *switch_text = L"��  ��";
wchar_t *next_text = L"��һ��";
wchar_t *reset_text = L"��  ��";
wchar_t *start_text = L"��  ʼ";
wchar_t *number[11] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
wchar_t *speed_text = L"��  ��:";
wchar_t *control_icon[3] = { "", "<", ">" };
wchar_t *des_alone_text = L"��Χ������ʱ�¶���";
wchar_t *des_hunger_text = L"��Χ������ʱ������";

