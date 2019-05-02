//#define RELEASE_VERSION

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "bitbuff.h"
#include "screen.h"
#include "menu.h"
#include "gfxset.h"
#include "lvlset.h"
#include "util.h"

#define CTRL_LEFT (16*32)
#define CTRL_TOP 0
#define B_BASEX (CTRL_LEFT+68)
#define B_BASEY (CTRL_TOP+64)
#define SPEED_BASEX (CTRL_LEFT+2)
#define SPEED_BASEY (CTRL_TOP+60+32)
#define TIME_BASEX (CTRL_LEFT+2)
#define TIME_BASEY (CTRL_TOP+178)
#define RATE_BASEX (CTRL_LEFT+2)
#define RATE_BASEY (CTRL_TOP+296)
#define FRAME_WIDTH 99
#define FRAME_HEIGHT 128
#define TRACK_WIDTH (FRAME_WIDTH-39)
#define TRACK_HEIGHT 18
#define OPER_BASEX (CTRL_LEFT+4)
#define OPER_BASEY (CTRL_TOP+48)
#define PAL_BASEX (CTRL_LEFT+66)
#define PAL_BASEY (CTRL_TOP+26)
#define LEVEL_BASEX CTRL_LEFT+52
#define LEVEL_BASEY CTRL_TOP
#define SUPER_BASEX CTRL_LEFT
#define SUPER_BASEY CTRL_TOP
#define ALIEN_BASEX CTRL_LEFT
#define ALIEN_BASEY (CTRL_TOP+24)

#define BUFFA_TITLE "Patch-A-Bobble: Buffer A"
#define BUFFB_TITLE "Patch-A-Bobble: Buffer B"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AboutPatchDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

void set_level_controls();
void enable_controls(BOOL enable);


HWND hPalette_ud;
HWND hLevel_ud;
HWND hLevel_edit;
HWND hTop_ud;
HWND hBottom_ud;
HWND hOper_button;
HWND hMtype_ud;
HWND hUp_button;
HWND hDown_button;
HWND hLeft_button;
HWND hRight_button;
HWND hBlock_button;
HWND hBubbleSpeed_track;
HWND hBubbleSpeed_text;
HWND hMonsterSpeed_track;
HWND hMonsterSpeed_text;
HWND hBreakSpeed_track;
HWND hBreakSpeed_text;
HWND hTimer_track;
HWND hTimer_text;
HWND hBaronTime_track;
HWND hBaronTime_text;
HWND hLightningRate_track;
HWND hLightningRate_text;
HWND hWaterRate_track;
HWND hWaterRate_text;
HWND hFireRate_track;
HWND hFireRate_text;
HWND hSuper_check;
HWND hAlien_check;

HWND hMonster_icon;
HWND hWeapon_icon;
HWND hPoints_icon;
HWND hBubble_icon;
HWND hBurst_icon;
HWND hHurry_icon;
HWND hBaron_icon;
HWND hFire_icon;
HWND hWater_icon;
HWND hLightning_icon;
HWND hPalette_icon;
HWND hSmallAlien_icon;

//HWND hToolTips;

//TOOLINFO BubbleSpeed_tinfo;
//char* BubbleSpeed_string = "TESTING";

Screen* scrn;
GfxSet* gfxset;
LvlSet A_buff(102);
LvlSet B_buff(102);
LvlSet C_buff(1);


HINSTANCE hInst;
HWND      hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{
   static char szAppName[] = "Patch-A-Bobble";
   WNDCLASSEX  wndclass;
   MSG         msg;
   HACCEL      haccel;
   int         len;
   char        current_dir[1000] = {0};

   hPrevInstance;
   szCmdLine;

   wndclass.cbSize        = sizeof (wndclass);
   wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
   wndclass.lpfnWndProc   = WndProc;
   wndclass.cbClsExtra    = 0;
   wndclass.cbWndExtra    = 0;
   wndclass.hInstance     = hInstance;
   wndclass.hIcon         = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_PBOBBLE));
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
   wndclass.hbrBackground = CreateSolidBrush(GetSysColor(CTLCOLOR_DLG));
   wndclass.lpszMenuName  = "PBOBBLE_MENU";
   wndclass.lpszClassName = szAppName;
   wndclass.hIconSm       = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_BUBBLE));

   RegisterClassEx(&wndclass);

   hwnd = CreateWindowEx(0,            // ext style
                       szAppName,      // window class name
                       BUFFA_TITLE,      // window caption
                       WS_CAPTION | WS_DLGFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX |
                       WS_OVERLAPPED | WS_SYSMENU,   // window style
                       CW_USEDEFAULT,  // initial x position
                       CW_USEDEFAULT,  // initial y position
                       620,            // initial x size
                       470,            // initial y size
                       NULL,            // parent window handle
                       NULL,            // window menu handle
                       hInstance,      // program instance handle
                       NULL);            // creation parameters

   if(hwnd == NULL)
   {
      errorbox("Error %d\n", GetLastError());
      ExitProcess(0);
   }

//   msgbox(szCmdLine);

   haccel = LoadAccelerators(hInstance, "PBOBBLE_ACCEL");

   hInst = hInstance;

   gfxset = new GfxSet(hwnd);

   GetCurrentDirectory(1000, current_dir);
   len = strlen(current_dir);
   current_dir[len] = '\\';
   current_dir[len+1] = 0;

   if(! (gfxset->load_roms(rom_names[ROMS_GFX1A][1], rom_names[ROMS_GFX1B][1], rom_names[ROMS_GFX2A][1], rom_names[ROMS_GFX2B][1],
                           rom_names[ROMS_GFX3A][1], rom_names[ROMS_GFX3B][1], rom_names[ROMS_GFXBA][1], rom_names[ROMS_GFXBB][1],
                           rom_names[ROMS_GFXSA][1], rom_names[ROMS_GFXSB][1])
         && A_buff.load_roms(rom_names[ROMS_SCREEN][1], rom_names[ROMS_LEVEL_BUBLBOBL][1], rom_names[ROMS_LEVEL_BUBLBOBL_UMS][1], rom_names[ROMS_LEVEL_BUBLBOBL_U][1], rom_names[ROMS_LEVEL_BOBLBOBL][1])))
   {
      errorbox("There was an error loading the Patch-A-Bobble data files from %s", current_dir);
      while(!(gfxset->load_roms(rom_names[ROMS_GFX1A][1], rom_names[ROMS_GFX1B][1], rom_names[ROMS_GFX2A][1], rom_names[ROMS_GFX2B][1],
                                rom_names[ROMS_GFX3A][1], rom_names[ROMS_GFX3B][1], rom_names[ROMS_GFXBA][1], rom_names[ROMS_GFXBB][1],
                                rom_names[ROMS_GFXSA][1], rom_names[ROMS_GFXSB][1])
              && A_buff.load_roms(rom_names[ROMS_SCREEN][1], rom_names[ROMS_LEVEL_BUBLBOBL][1], rom_names[ROMS_LEVEL_BUBLBOBL_UMS][1], rom_names[ROMS_LEVEL_BUBLBOBL_U][1], rom_names[ROMS_LEVEL_BOBLBOBL][1])
            ))
      {
         switch(DialogBox(hInst, "DLG_DATA_NOT_FOUND", hwnd, (DLGPROC)DatDlgProc))
         {
            case IDC_USE_QUIT:
               ExitProcess(0);
               return 0;
            case IDC_USE_BUBLBOBL:
               make_data_files(current_dir);
               break;
         }
      }
   }

   A_buff.make_default();
   B_buff.load_roms(rom_names[ROMS_SCREEN][1], rom_names[ROMS_LEVEL_BUBLBOBL][1], rom_names[ROMS_LEVEL_BUBLBOBL_UMS][1], rom_names[ROMS_LEVEL_BUBLBOBL_U][1], rom_names[ROMS_LEVEL_BOBLBOBL][1]);
   B_buff.make_default();
   C_buff.clear(1);

   InitCommonControls();

   hOper_button = CreateWindow("button", "", BS_OWNERDRAW | BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE, OPER_BASEX, OPER_BASEY, 32, 32, hwnd, (HMENU)IDM_INVERT, hInstance, 0);
   hMtype_ud    = CreateUpDownControl(WS_CHILD | WS_VISIBLE | UDS_HORZ, OPER_BASEX, OPER_BASEY+32, 32, 10, hwnd, -1, hInstance, NULL, MONSTER_TYPE_MAX, MONSTER_TYPE_MIN, MONSTER_TYPE_MIN);

   scrn = new Screen(&A_buff, gfxset, hOper_button, 0, 0);

   hSuper_check     = CreateWindow("button", "Sup", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, SUPER_BASEX, SUPER_BASEY, 50, 20, hwnd, (HMENU)IDM_SUPER, hInstance, 0);
   hAlien_check     = CreateWindow("button", "", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, ALIEN_BASEX, ALIEN_BASEY, 16, 20, hwnd, (HMENU)IDM_ALIENS, hInstance, 0);
   hSmallAlien_icon = CreateWindow("static", "", SS_OWNERDRAW | WS_CHILD | WS_VISIBLE, ALIEN_BASEX+16, ALIEN_BASEY+2, 16, 16, hwnd, (HMENU)IDM_SMALL_ALIEN_ICON, hInstance, 0);

   hLevel_edit = CreateWindow("edit", "1", WS_CHILD | WS_VISIBLE | WS_BORDER, LEVEL_BASEX, LEVEL_BASEY, 50, 20, hwnd, (HMENU)IDM_LEVEL, hInstance, 0);
   hLevel_ud   = CreateUpDownControl(WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT, 0, 0, 0, 0, hwnd, -1, hInstance, hLevel_edit, LEVEL_MAX, LEVEL_MIN, LEVEL_MIN);
   SendMessage(hLevel_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_lvlset()->get_level(), 0));

   hPalette_icon = CreateWindow("static", "", SS_OWNERDRAW | WS_CHILD | WS_VISIBLE, PAL_BASEX, PAL_BASEY, 16, 16, hwnd, (HMENU)IDM_PALETTE_ICON, hInstance, 0);
   hPalette_ud = CreateUpDownControl(WS_CHILD | WS_VISIBLE, PAL_BASEX+18, PAL_BASEY, 8, 16, hwnd, -1, hInstance, NULL, PALETTE_MAX, PALETTE_MIN, PALETTE_MIN);

   hBlock_button = CreateWindow("button", "", BS_OWNERDRAW | BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE, B_BASEX, B_BASEY, 16, 16, hwnd, (HMENU)IDM_BLOCK, hInstance, 0);
   hLeft_button  = CreateWindow("button", "", BS_OWNERDRAW | BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE, B_BASEX-16, B_BASEY, 16, 16, hwnd, (HMENU)IDM_LEFT, hInstance, 0);
   hRight_button = CreateWindow("button", "", BS_OWNERDRAW | BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE, B_BASEX+16, B_BASEY, 16, 16, hwnd, (HMENU)IDM_RIGHT, hInstance, 0);
   hUp_button    = CreateWindow("button", "", BS_OWNERDRAW | BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE, B_BASEX, B_BASEY-16, 16, 16, hwnd, (HMENU)IDM_UP, hInstance, 0);
   hDown_button  = CreateWindow("button", "", BS_OWNERDRAW | BS_PUSHBUTTON | BS_BITMAP | WS_CHILD | WS_VISIBLE, B_BASEX, B_BASEY+16, 16, 16, hwnd, (HMENU)IDM_DOWN, hInstance, 0);

   hTop_ud    = CreateUpDownControl(WS_CHILD | WS_VISIBLE, CTRL_LEFT-16, CTRL_TOP, 8, 16, hwnd, -1, hInstance, NULL, TOPBOT_MAX, TOPBOT_MIN, TOPBOT_MIN);
   hBottom_ud = CreateUpDownControl(WS_CHILD | WS_VISIBLE, CTRL_LEFT-16, CTRL_TOP+25*16, 8, 16, hwnd, -1, hInstance, NULL, TOPBOT_MAX, TOPBOT_MIN, TOPBOT_MIN);


   CreateWindow("button", "Speed", BS_GROUPBOX | WS_CHILD | WS_VISIBLE, SPEED_BASEX, SPEED_BASEY, FRAME_WIDTH, FRAME_HEIGHT-32, hwnd, (HMENU)-1, hInstance, 0);

   hBubble_icon       = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, SPEED_BASEX+4, SPEED_BASEY+18, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hBubbleSpeed_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, SPEED_BASEX+36, SPEED_BASEY+32, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   hBubbleSpeed_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER, SPEED_BASEX+36, SPEED_BASEY+18, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hBubble_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_bubble_icon(), 0);
   SendMessage(hBubbleSpeed_track, TBM_SETRANGE, TRUE, MAKELPARAM(BUBBLE_SPEED_MIN, BUBBLE_SPEED_MAX));
   SendMessage(hBubbleSpeed_track, TBM_SETPAGESIZE, 0, 1);

   hMonster_icon       = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, SPEED_BASEX+4, SPEED_BASEY+52, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hMonsterSpeed_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, SPEED_BASEX+36, SPEED_BASEY+66, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   hMonsterSpeed_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER, SPEED_BASEX+36, SPEED_BASEY+52, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hMonster_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_monster_icon(0, 1), 0);
   SendMessage(hMonsterSpeed_track, TBM_SETRANGE, TRUE, MAKELPARAM(MONSTER_SPEED_MIN, MONSTER_SPEED_MAX));
   SendMessage(hMonsterSpeed_track, TBM_SETPAGESIZE, 0, 1);


   CreateWindow("button", "Time Until", BS_GROUPBOX | WS_CHILD | WS_VISIBLE, TIME_BASEX, TIME_BASEY, FRAME_WIDTH, FRAME_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);

   hBurst_icon       = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, TIME_BASEX+4, TIME_BASEY+18, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hBreakSpeed_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER,   TIME_BASEX+36, TIME_BASEY+18, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   hBreakSpeed_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, TIME_BASEX+36, TIME_BASEY+32, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hBurst_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_burst_icon(), 0);
   SendMessage(hBreakSpeed_track, TBM_SETRANGE, TRUE, MAKELPARAM(BREAK_SPEED_MIN, BREAK_SPEED_MAX));
   SendMessage(hBreakSpeed_track, TBM_SETPAGESIZE, 0, 1);

   hHurry_icon  = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, TIME_BASEX+4, TIME_BASEY+52, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hTimer_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER, TIME_BASEX+36, TIME_BASEY+52, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   hTimer_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, TIME_BASEX+36, TIME_BASEY+66, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hHurry_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_hurry_icon(), 0);
   SendMessage(hTimer_track, TBM_SETRANGE, TRUE, MAKELPARAM(TIMER_MIN, TIMER_MAX));
   SendMessage(hTimer_track, TBM_SETPAGESIZE, 0, 1);

   hBaron_icon      = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, TIME_BASEX+4, TIME_BASEY+86, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hBaronTime_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER, TIME_BASEX+36, TIME_BASEY+86, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   hBaronTime_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, TIME_BASEX+36, TIME_BASEY+100, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hBaron_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_baron_icon(), 0);
   SendMessage(hBaronTime_track, TBM_SETRANGE, TRUE, MAKELPARAM(BARON_TIME_MIN, BARON_TIME_MAX));
   SendMessage(hBaronTime_track, TBM_SETPAGESIZE, 0, 1);


   CreateWindow("button", "Rate", BS_GROUPBOX | WS_CHILD | WS_VISIBLE, RATE_BASEX, RATE_BASEY, FRAME_WIDTH, FRAME_HEIGHT-8, hwnd, (HMENU)-1, hInstance, 0);

   hWater_icon      = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, RATE_BASEX+4, RATE_BASEY+18, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hWaterRate_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER, RATE_BASEX+36, RATE_BASEY+18, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   hWaterRate_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, RATE_BASEX+36, RATE_BASEY+32, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hWater_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_water_icon(), 0);
   SendMessage(hWaterRate_track, TBM_SETRANGE, TRUE, MAKELPARAM(BUBBLE_RATE_MIN, BUBBLE_RATE_MAX));
   SendMessage(hWaterRate_track, TBM_SETPAGESIZE, 0, 1);

   hFire_icon = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, RATE_BASEX+4, RATE_BASEY+52, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hFireRate_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER, RATE_BASEX+36, RATE_BASEY+52, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   hFireRate_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, RATE_BASEX+36, RATE_BASEY+66, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hFire_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_fire_icon(), 0);
   SendMessage(hFireRate_track, TBM_SETRANGE, TRUE, MAKELPARAM(BUBBLE_RATE_MIN, BUBBLE_RATE_MAX));
   SendMessage(hFireRate_track, TBM_SETPAGESIZE, 0, 1);

   hLightning_icon = CreateWindow("static", "", SS_ICON | WS_CHILD | WS_VISIBLE, RATE_BASEX+4, RATE_BASEY+86, 32, 32, hwnd, (HMENU)-1, hInstance, 0);
   hLightningRate_text  = CreateWindow("static", "", WS_CHILD | WS_VISIBLE | SS_CENTER,   RATE_BASEX+36, RATE_BASEY+86, TRACK_WIDTH, 14, hwnd, (HMENU)-1, hInstance, 0);
   hLightningRate_track = CreateWindow(TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE, RATE_BASEX+36, RATE_BASEY+100, TRACK_WIDTH, TRACK_HEIGHT, hwnd, (HMENU)-1, hInstance, 0);
   SendMessage(hLightning_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_lightning_icon(), 0);
   SendMessage(hLightningRate_track, TBM_SETRANGE, TRUE, MAKELPARAM(BUBBLE_RATE_MIN, BUBBLE_RATE_MAX));
   SendMessage(hLightningRate_track, TBM_SETPAGESIZE, 0, 1);

//   hToolTips = CreateWindow(TOOLTIPS_CLASS, NULL, 0, 0, 0, 0, 0, hwnd, (HMENU)-1, hInstance, 0);
//   set_tooltip(hToolTips, hBubbleSpeed_track, &BubbleSpeed_tinfo, BubbleSpeed_string);

   if(*szCmdLine != 0)
      if(!scrn->get_lvlset()->load_patch(szCmdLine))
         errorbox("Error loading %s", szCmdLine);
   scrn->update_all();
   set_level_controls();

   ShowWindow(hwnd, iCmdShow);
   UpdateWindow(hwnd);

   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateAccelerator(hwnd, haccel, &msg);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   PAINTSTRUCT ps;
   DRAWITEMSTRUCT* lpdis;
   char patch_name[1000] = {0};
   char patch_path[1000] = {0};

   if(iMsg != WM_DRAWITEM)
      scrn->change_cursor();
   switch (iMsg)
   {
      case WM_CREATE:
         break;
      case WM_DRAWITEM:
         lpdis = (DRAWITEMSTRUCT*) lParam;
         switch(lpdis->CtlID)
         {
            case IDM_UP:
               gfxset->draw_up_button(lpdis->hDC, lpdis->itemAction & ODS_SELECTED);
               break;
            case IDM_DOWN:
               gfxset->draw_down_button(lpdis->hDC, lpdis->itemAction & ODS_SELECTED);
               break;
            case IDM_LEFT:
               gfxset->draw_left_button(lpdis->hDC, lpdis->itemAction & ODS_SELECTED);
               break;
            case IDM_RIGHT:
               gfxset->draw_right_button(lpdis->hDC, lpdis->itemAction & ODS_SELECTED);
               break;
            case IDM_BLOCK:
               gfxset->draw_block_button(lpdis->hDC, lpdis->itemAction & ODS_SELECTED);
               break;
            case IDM_INVERT:
               SendMessage(hMtype_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_selected_monster_type(), 0));
               scrn->draw_oper_button(lpdis->hDC);
               break;
            case IDM_PALETTE_ICON:
               gfxset->draw_palette_button(lpdis->hDC);
               break;
            case IDM_SMALL_ALIEN_ICON:
               gfxset->draw_small_alien_button(lpdis->hDC);
               break;
         }
         break;
      case WM_COMMAND:
         switch(LOWORD(wParam))
         {
            case IDM_BUFF_A:
               scrn->cancel();
               scrn->set_lvlset(&A_buff);
               SetWindowText(hwnd, BUFFA_TITLE);
               SendMessage(hLevel_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_lvlset()->get_level(), 0));
               scrn->update_all();
               set_level_controls();
               break;
            case IDM_BUFF_B:
               scrn->cancel();
               scrn->set_lvlset(&B_buff);
               SetWindowText(hwnd, BUFFB_TITLE);
               SendMessage(hLevel_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_lvlset()->get_level(), 0));
               scrn->update_all();
               set_level_controls();
               break;
            case IDM_COPY:
               scrn->cancel();
               scrn->get_lvlset()->copy(scrn->get_lvlset()->get_level(), &C_buff, 1);
               break;
            case IDM_CUT:
               scrn->cancel();
               scrn->get_lvlset()->copy(scrn->get_lvlset()->get_level(), &C_buff, 1);
               scrn->get_lvlset()->clear(scrn->get_lvlset()->get_level());
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_PASTE:
               scrn->cancel();
               C_buff.copy(1, scrn->get_lvlset(), scrn->get_lvlset()->get_level());
               scrn->update_all();
               set_level_controls();
               break;
            case IDM_XCHG:
               scrn->cancel();
               scrn->get_lvlset()->xchg(scrn->get_lvlset()->get_level(), &C_buff, 1);
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_REVERT:
               scrn->cancel();
               scrn->get_lvlset()->revert_level();
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_REVERT_ALL:
               scrn->cancel();
               scrn->get_lvlset()->revert_all();
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_CLEAR:
               scrn->cancel();
               scrn->get_lvlset()->clear(scrn->get_lvlset()->get_level());
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_CLEAR_ALL:
               scrn->cancel();
               scrn->get_lvlset()->clear_all();
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_LOADPATCH:
               scrn->cancel();
               if(load_filerequestor(patch_path, patch_name, "Load Patchfile", "bub", "Bubble Patch"))
                  if(!scrn->get_lvlset()->load_patch(patch_path))
                     errorbox("Error loading %s", patch_path);
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_SAVEPATCH:
               scrn->cancel();
               if(save_filerequestor(patch_path, patch_name, "Save Patchfile As", "bub", "Bubble Patch"))
                  if(!scrn->get_lvlset()->save_patch(patch_path))
                     errorbox("Error saving %s", patch_path);
               break;
            case IDM_UP:
               scrn->cancel();
               scrn->paint_up();
               break;
            case IDM_DOWN:
               scrn->cancel();
               scrn->paint_down();
               break;
            case IDM_LEFT:
               scrn->cancel();
               scrn->paint_left();
               break;
            case IDM_RIGHT:
               scrn->cancel();
               scrn->paint_right();
               break;
            case IDM_BLOCK:
               scrn->cancel();
               scrn->paint_solid();
               break;
            case IDM_LEVEL:
               if(scrn->get_lvlset()->set_level((short)SendMessage(hLevel_ud, UDM_GETPOS, 0, 0)))
               {
                  scrn->cancel(FALSE);
                  set_level_controls();
                  scrn->update_all();
               }
               break;
            case IDM_LOAD_BUBLBOBL:
               scrn->cancel();
               load_roms(scrn->get_lvlset());
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_SAVE_BUBLBOBL:
               scrn->cancel();
               save_roms(scrn->get_lvlset());
               break;
            case IDM_EXIT:
               scrn->cancel();
#ifdef RELEASE_VERSION
               if(MessageBox(NULL, "Quit Patch-A-Bobble?", "Quit Verify", MB_OKCANCEL) != IDOK)
                  break;
#endif
               DestroyWindow(hwnd);
               break;
            case IDM_ABOUT:
               scrn->cancel();
               DialogBox(hInst, "DLG_ABOUT", hwnd, (DLGPROC)AboutDlgProc);
               break;
            case IDM_ABOUT_PATCH:
               scrn->cancel();
               DialogBox(hInst, "DLG_ABOUT_PATCH", hwnd, (DLGPROC)AboutPatchDlgProc);
               break;
            case IDM_INSERT:
               scrn->cancel();
               scrn->new_monster();
               break;
            case IDM_REMOVE:
               scrn->del_selected_monster();
               break;
            case IDM_ALIENS:
               scrn->get_lvlset()->toggle_alien_flag();
               SendMessage(hAlien_check, BM_SETCHECK, scrn->get_lvlset()->get_alien_flag(), 0);
               CheckMenuItem(GetMenu(hwnd), IDM_ALIENS, scrn->get_lvlset()->get_alien_flag() ? MF_CHECKED : MF_UNCHECKED);
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_DROP_MODE:
               scrn->toggle_drop_mode();
               CheckMenuItem(GetMenu(hwnd), IDM_DROP_MODE, scrn->get_drop_mode() ? MF_CHECKED : MF_UNCHECKED);
               break;
            case IDM_TEST_DROP:
               SetTimer(hwnd, 0, 30, NULL);
               scrn->start_drop_anim();
               ShowWindow(hTop_ud, SW_HIDE);
               ShowWindow(hBottom_ud, SW_HIDE);
               enable_controls(FALSE);
               break;
            case IDM_INVERT:
               scrn->invert_selected_monster();
               break;
            case IDM_ARROWS:
               scrn->toggle_arrows();
               CheckMenuItem(GetMenu(hwnd), IDM_ARROWS, scrn->get_arrows() ? MF_CHECKED : MF_UNCHECKED);
               break;
            case IDM_SMOOTHING:
               scrn->toggle_smoothing();
               scrn->update_all();
               scrn->invalidate_screen();
               CheckMenuItem(GetMenu(hwnd), IDM_SMOOTHING, scrn->get_smoothing() ? MF_CHECKED : MF_UNCHECKED);
               SendMessage(hBubble_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_bubble_icon(), 0);
               SendMessage(hMonster_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_monster_icon(0, 1), 0);
               SendMessage(hBurst_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_burst_icon(), 0);
               SendMessage(hHurry_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_hurry_icon(), 0);
               SendMessage(hBaron_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_baron_icon(), 0);
               SendMessage(hWater_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_water_icon(), 0);
               SendMessage(hFire_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_fire_icon(), 0);
               SendMessage(hLightning_icon, STM_SETICON, (WPARAM) (HICON) gfxset->get_lightning_icon(), 0);
               scrn->newoper();
               break;
            case IDM_SUPER:
               scrn->get_lvlset()->toggle_super();
               set_level_controls();
               scrn->update_all();
               break;
            case IDM_GRID:
               scrn->toggle_grid();
               CheckMenuItem(GetMenu(hwnd), IDM_GRID, scrn->get_grid() ? MF_CHECKED : MF_UNCHECKED);
               break;
            case IDM_DROP_WIPE:
               scrn->get_lvlset()->drop_pattern_wipe();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_SLIDE:
               scrn->get_lvlset()->drop_pattern_slide();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_ROLL:
               scrn->get_lvlset()->drop_pattern_roll();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_STRETCH:
               scrn->get_lvlset()->drop_pattern_stretch();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_SHRINK:
               scrn->get_lvlset()->drop_pattern_shrink();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_ANGLE_LEFT:
               scrn->get_lvlset()->drop_pattern_angle_left();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_ANGLE_RIGHT:
               scrn->get_lvlset()->drop_pattern_angle_right();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_ANGLE_INSIDE:
               scrn->get_lvlset()->drop_pattern_angle_inside();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
            case IDM_DROP_ANGLE_OUTSIDE:
               scrn->get_lvlset()->drop_pattern_angle_outside();
               if(scrn->get_drop_mode())
                  scrn->update_all();
               break;
         }
         break;
      case WM_VSCROLL:
      case WM_HSCROLL:
         if((HWND)lParam == hPalette_ud)
         {
            if(scrn->get_lvlset()->set_palette((short)SendMessage(hPalette_ud, UDM_GETPOS, 0, 0)))
            {
               set_level_controls();
               scrn->update_all();
            }
         }
         else if((HWND)lParam == hBubbleSpeed_track)
         {
            scrn->get_lvlset()->set_bubble_speed((short)SendMessage(hBubbleSpeed_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hBubbleSpeed_track, hBubbleSpeed_text, scrn->get_lvlset()->get_bubble_speed(), -1);
         }
         else if((HWND)lParam == hMonsterSpeed_track)
         {
            scrn->get_lvlset()->set_monster_speed((short)SendMessage(hMonsterSpeed_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hMonsterSpeed_track, hMonsterSpeed_text, scrn->get_lvlset()->get_monster_speed(), -1);
         }
         else if((HWND)lParam == hBreakSpeed_track)
         {
            scrn->get_lvlset()->set_break_speed((short)SendMessage(hBreakSpeed_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hBreakSpeed_track, hBreakSpeed_text, scrn->get_lvlset()->get_break_speed(), -1);
         }
         else if((HWND)lParam == hTimer_track)
         {
            scrn->get_lvlset()->set_timer((short)SendMessage(hTimer_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hTimer_track, hTimer_text, scrn->get_lvlset()->get_timer(), TIMER_MAX);
         }
         else if((HWND)lParam == hBaronTime_track)
         {
            scrn->get_lvlset()->set_baron_time((short)SendMessage(hBaronTime_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hBaronTime_track, hBaronTime_text, scrn->get_lvlset()->get_baron_time(), BARON_TIME_MAX);
         }
         else if((HWND)lParam == hWaterRate_track)
         {
            scrn->get_lvlset()->set_water_rate((short)SendMessage(hWaterRate_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hWaterRate_track, hWaterRate_text, scrn->get_lvlset()->get_water_rate(), BUBBLE_RATE_MIN);
         }
         else if((HWND)lParam == hFireRate_track)
         {
            scrn->get_lvlset()->set_fire_rate((short)SendMessage(hFireRate_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hFireRate_track, hFireRate_text, scrn->get_lvlset()->get_fire_rate(), BUBBLE_RATE_MIN);
         }
         else if((HWND)lParam == hLightningRate_track)
         {
            scrn->get_lvlset()->set_lightning_rate((short)SendMessage(hLightningRate_track, TBM_GETPOS, 0, 0));
            set_track_and_text(hLightningRate_track, hLightningRate_text, scrn->get_lvlset()->get_lightning_rate(), BUBBLE_RATE_MIN);
         }
         else if((HWND)lParam == hTop_ud)
         {
            scrn->set_top((short)SendMessage(hTop_ud, UDM_GETPOS, 0, 0));
         }
         else if((HWND)lParam == hBottom_ud)
         {
            scrn->set_bottom((short)SendMessage(hBottom_ud, UDM_GETPOS, 0, 0));
         }
         else if((HWND)lParam == hMtype_ud)
            if(scrn->set_selected_monster_type((short)SendMessage(hMtype_ud, UDM_GETPOS, 0, 0)))
               scrn->newoper();
         break;
      case WM_MOUSEMOVE:
         scrn->mouse_move(LOWORD(lParam), HIWORD(lParam));
         break;
      case WM_LBUTTONDOWN:
         scrn->mouse_l_down(LOWORD(lParam), HIWORD(lParam));
         break;
      case WM_LBUTTONUP:
         scrn->mouse_l_up(LOWORD(lParam), HIWORD(lParam));
         break;
      case WM_RBUTTONDOWN:
         scrn->mouse_r_down(LOWORD(lParam), HIWORD(lParam));
         break;
      case WM_RBUTTONUP:
         scrn->mouse_r_up(LOWORD(lParam), HIWORD(lParam));
         break;
      case WM_CHAR:
         switch((TCHAR) wParam)
         {
            case 's':
               scrn->toggle_smoothing();
               scrn->update_all();
               scrn->invalidate_screen();
               break;
         }
         break;
      case WM_TIMER:
         if(!scrn->pulse_drop_anim())
         {
            KillTimer(hwnd, 0);
            scrn->stop_drop_anim();
            ShowWindow(hTop_ud, SW_SHOW);
            ShowWindow(hBottom_ud, SW_SHOW);
            enable_controls(TRUE);
         }
         break;
      case WM_PAINT:
         BeginPaint(hwnd, &ps);
         scrn->redraw(&(ps.rcPaint));
         EndPaint(hwnd, &ps);
         break;
      case WM_CLOSE:
         scrn->cancel();
#ifdef RELEASE_VERSION
         if(MessageBox(NULL, "Quit Patch-A-Bobble?", "Quit Verify", MB_OKCANCEL) != IDOK)
            break;
#endif
         DestroyWindow(hwnd);
         break;
      case WM_DESTROY:
         if(scrn != NULL)
            delete scrn;
         if(gfxset != NULL)
            delete gfxset;
         scrn = NULL;
         gfxset = NULL;
         PostQuitMessage(0);
         break;
      default:
         return DefWindowProc(hwnd, iMsg, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   lParam;
   switch(iMsg)
   {
      case WM_INITDIALOG:
         break;
      case WM_COMMAND:
         switch(LOWORD(wParam))
         {
            case IDOK:
               EndDialog(hDlg, IDOK);
               break;
         }
         break;
      default:
         return FALSE;
   }
   return TRUE;
}

LRESULT CALLBACK DatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   lParam;
   switch(iMsg)
   {
      case WM_INITDIALOG:
         break;
      case WM_COMMAND:
         switch(LOWORD(wParam))
         {
            case IDC_USE_QUIT:
               EndDialog(hDlg, IDC_USE_QUIT);
               break;
            case IDC_USE_BOBLBOBL:
               EndDialog(hDlg, IDC_USE_BOBLBOBL);
               break;
            case IDC_USE_SBOBLBOB:
               EndDialog(hDlg, IDC_USE_SBOBLBOB);
               break;
            case IDC_USE_BUBLBOBL:
               EndDialog(hDlg, IDC_USE_BUBLBOBL);
               break;
         }
         break;
      default:
         return FALSE;
   }
   return TRUE;
}

LRESULT CALLBACK AboutPatchDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   int command;
   int i;
   char buff[40] = {0};

   lParam;
   switch(iMsg)
   {
      case WM_INITDIALOG:
         for(i=IDC_PATCH_C1;i<=IDC_PATCH_C102;i++)
            CheckDlgButton(hDlg, i, scrn->get_lvlset()->is_default(i-IDC_PATCH_C1+1) ? 0 : 2);
         SetDlgItemText(hDlg, IDC_PATCH_AUTHOR, scrn->get_lvlset()->get_author());
         SetDlgItemText(hDlg, IDC_PATCH_DESC, scrn->get_lvlset()->get_desc());
         break;
      case WM_COMMAND:
         command = LOWORD(wParam);

         if(command >= IDC_PATCH_C1 && command <= IDC_PATCH_C102)
         {
            CheckDlgButton(hDlg, command, scrn->get_lvlset()->is_default(command-IDC_PATCH_C1+1) ? 0 : 2);
         }
         else
         {
            switch(LOWORD(wParam))
            {
               case IDC_PATCH_AUTHOR:
                  memset(buff, 0, 40);
                  GetDlgItemText(hDlg, IDC_PATCH_AUTHOR, buff, 20);
                  scrn->get_lvlset()->set_author(buff);
                  break;
               case IDC_PATCH_DESC:
                  memset(buff, 0, 40);
                  GetDlgItemText(hDlg, IDC_PATCH_DESC, buff, 30);
                  scrn->get_lvlset()->set_desc(buff);
                  break;
               case IDOK:
                  EndDialog(hDlg, IDOK);
                  break;
            }
         }
         break;
      default:
         return FALSE;
   }
   return TRUE;
}


void enable_controls(BOOL enable)
{
   EnableWindow(hPalette_ud, enable);
   EnableWindow(hLevel_ud, enable);
   EnableWindow(hLevel_edit, enable);
   EnableWindow(hTop_ud, enable);
   EnableWindow(hBottom_ud, enable);
   EnableWindow(hOper_button, enable);
   EnableWindow(hMtype_ud, enable);
   EnableWindow(hUp_button, enable);
   EnableWindow(hDown_button, enable);
   EnableWindow(hLeft_button, enable);
   EnableWindow(hRight_button, enable);
   EnableWindow(hBlock_button, enable);
   EnableWindow(hBubbleSpeed_track, enable);
   EnableWindow(hMonsterSpeed_track, enable);
   EnableWindow(hBreakSpeed_track, enable);
   EnableWindow(hTimer_track, enable);
   EnableWindow(hBaronTime_track, enable);
   EnableWindow(hLightningRate_track, enable);
   EnableWindow(hWaterRate_track, enable);
   EnableWindow(hFireRate_track, enable);
   EnableWindow(hSuper_check, enable);
   EnableWindow(hAlien_check, enable);

   EnableMenuItem(GetMenu(hwnd), IDM_LOADPATCH, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_SAVEPATCH, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_LOAD_BOBLBOBL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_SAVE_BOBLBOBL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_LOAD_SBOBLBOBL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_SAVE_SBOBLBOBL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_LOAD_BUBLBOBL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_SAVE_BUBLBOBL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_CUT, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_PASTE, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_COPY, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_XCHG, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_CLEAR, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_REVERT, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_CLEAR_ALL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_REVERT_ALL, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_INSERT, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_REMOVE, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_INVERT, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_ALIENS, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_DROP_MODE, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_ARROWS, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_SUPER, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_GRID, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_BUFF_A, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_BUFF_B, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_ABOUT, enable ? MF_ENABLED : MF_GRAYED);
   EnableMenuItem(GetMenu(hwnd), IDM_ABOUT_PATCH, enable ? MF_ENABLED : MF_GRAYED);
}

void set_level_controls()
{
   CheckMenuItem(GetMenu(hwnd), IDM_ARROWS, scrn->get_arrows() ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(GetMenu(hwnd), IDM_SMOOTHING, scrn->get_smoothing() ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(GetMenu(hwnd), IDM_GRID, scrn->get_grid() ? MF_CHECKED : MF_UNCHECKED);
   SendMessage(hSuper_check, BM_SETCHECK, scrn->get_lvlset()->get_super(), 0);
   CheckMenuItem(GetMenu(hwnd), IDM_ALIENS, scrn->get_lvlset()->get_alien_flag() ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(GetMenu(hwnd), IDM_SUPER, scrn->get_lvlset()->get_super() ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(GetMenu(hwnd), IDM_BUFF_A, scrn->get_lvlset() == &A_buff ? MF_CHECKED : MF_UNCHECKED);
   CheckMenuItem(GetMenu(hwnd), IDM_BUFF_B, scrn->get_lvlset() == &B_buff ? MF_CHECKED : MF_UNCHECKED);
   SendMessage(hAlien_check, BM_SETCHECK, scrn->get_lvlset()->get_alien_flag(), 0);
   SendMessage(hTop_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_lvlset()->get_top(), 0));
   SendMessage(hBottom_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_lvlset()->get_bottom(), 0));
   SendMessage(hPalette_ud, UDM_SETPOS, 0, MAKELPARAM(scrn->get_lvlset()->get_palette(), 0));
   set_track_and_text(hBubbleSpeed_track, hBubbleSpeed_text, scrn->get_lvlset()->get_bubble_speed(), -1);
   set_track_and_text(hMonsterSpeed_track, hMonsterSpeed_text, scrn->get_lvlset()->get_monster_speed(), -1);
   set_track_and_text(hBreakSpeed_track, hBreakSpeed_text, scrn->get_lvlset()->get_break_speed(), -1);
   set_track_and_text(hTimer_track, hTimer_text, scrn->get_lvlset()->get_timer(), TIMER_MAX);
   set_track_and_text(hBaronTime_track, hBaronTime_text, scrn->get_lvlset()->get_baron_time(), BARON_TIME_MAX);
   set_track_and_text(hWaterRate_track, hWaterRate_text, scrn->get_lvlset()->get_water_rate(), BUBBLE_RATE_MIN);
   set_track_and_text(hFireRate_track, hFireRate_text, scrn->get_lvlset()->get_fire_rate(), BUBBLE_RATE_MIN);
   set_track_and_text(hLightningRate_track, hLightningRate_text, scrn->get_lvlset()->get_lightning_rate(), BUBBLE_RATE_MIN);

   scrn->newoper();
   InvalidateRect(hBlock_button, NULL, NULL);
}
