#ifndef UTIL__HEADER
#define UTIL__HEADER

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdarg.h>
#include "lvlset.h"
#include "screen.h"

#define ABS_INT(X) (X < 0 ? -X : X)
#define ROMS_GFX1A 0
#define ROMS_GFX1B 1
#define ROMS_GFX2A 2
#define ROMS_GFX2B 3
#define ROMS_GFX3A 4
#define ROMS_GFX3B 5
#define ROMS_GFXBA 6
#define ROMS_GFXBB 7
#define ROMS_GFXSA 8
#define ROMS_GFXSB 9
#define ROMS_SCREEN 10
#define ROMS_LEVEL 11
#define ROMS_LEVEL_BUBLBOBL 11
#define ROMS_LEVEL_BUBLBOBL_UMS 12
#define ROMS_LEVEL_BUBLBOBL_U 13
#define ROMS_LEVEL_BOBLBOBL 14

extern char* rom_names[15][2];

BOOL load_file(char* filename, char* buff, int len);
void set_track_and_text(HWND track, HWND text, int val, int off_val);
BOOL load_filerequestor(char* filepath, char* filename, char* title, char* ext, char* desc);
BOOL save_filerequestor(char* filepath, char* filename, char* title, char* ext, char* desc);
BOOL get_load_files(char* name_set[15][4], int type, char* level, char* screen);
BOOL get_save_files(char* name_set[15][4], int type, char* level, char* screen);
BOOL make_data_files(char* dir);
void strip_filename(char* str);
BOOL load_roms(LvlSet* lset);
BOOL save_roms(LvlSet* lset);
void set_tooltip(HINSTANCE hinst, HWND hwnd, HWND htooltip, HWND htool, TOOLINFO* tinfo, char* text);

void msgbox(char* msg, ...);
void errorbox(char* msg, ...);

#endif
