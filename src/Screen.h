#ifndef SCREEN__HEADER
#define SCREEN__HEADER

#include <windows.h>
#include "lvlset.h"
#include "gfxset.h"
#include "limits.h"

#define STATE_POINT       0
#define STATE_PAINT_SOLID 1
#define STATE_PAINT_UP    2
#define STATE_PAINT_LEFT  3
#define STATE_PAINT_RIGHT 4
#define STATE_PAINT_DOWN  5
#define STATE_MOVE        6

#define MOVE_NONE         -1
#define MOVE_M1           0
#define MOVE_M2           1
#define MOVE_M3           2
#define MOVE_M4           3
#define MOVE_M5           4
#define MOVE_M6           5
#define MOVE_M7           6
#define MOVE_WEAPON       7
#define MOVE_POINTS       8

#include <stdio.h>
#include <stdarg.h>
void msgbox(char* msg, ...);
BOOL has_common_area(int x1s, int y1s, int x1e, int y1e,
                     int x2s, int y2s, int x2e, int y2e);
BOOL is_in_region(int x, int y, int xs, int ys, int xe, int ye);


class Screen
{
   public:
      Screen(LvlSet* lvl, GfxSet* gfx, HWND hOper_Button, int basex, int basey);

      void    set_lvlset      (LvlSet* newset) {m_lvlset = newset;update_all();}
      LvlSet* get_lvlset      ()               {return m_lvlset;}
      void    set_gfxset      (GfxSet* newset) {m_gfxset = newset;update_all();}
      GfxSet* get_gfxseta     ()               {return m_gfxset;}
      void    set_grid        (BOOL val)       {m_grid = val;}
      BOOL    get_grid        ()               {return m_grid;}
      void    toggle_grid     ()               {m_grid = !m_grid;}
      BOOL    get_smoothing   ()               {return m_gfxset->get_smoothing();}
      void    toggle_smoothing()               {m_gfxset->toggle_smoothing();}
      BOOL    get_drop_mode   ()               {return m_mode_drop;}
      void    toggle_drop_mode()               {m_mode_drop = !m_mode_drop;update_all();}

      void start_drop_anim();
      BOOL pulse_drop_anim();
      void stop_drop_anim () {if(m_drop_anim){ m_drop_anim = FALSE;update_all();}}

      HBITMAP get_oper_bmp    ();
      void    draw_oper_button(HDC hdc) {m_gfxset->draw_bitmap(get_oper_bmp(), 0, 0, hdc);}

      BOOL invert_selected_monster  ();
      BOOL new_monster              ();
      BOOL del_selected_monster     ();
      BOOL select_monster           (int monster_num) {drop_item(FALSE);return pickup_item(monster_num);}
      BOOL set_selected_monster_type(int newtype);
      BOOL set_selected_monster_drop(int newdrop);
      int  get_selected_monster     ()                {return m_move_item >= 0 && m_move_item <= 6 ? m_move_item : -1;}
      int  get_selected_monster_type()                {return m_lvlset->get_monster_type(m_move_item);}
      int  get_selected_monster_drop()                {return (m_lvlset->get_monster_drop(m_move_item)*2)+16;}

      void update_all       () {m_gfxset->set_level(m_lvlset->get_level(), m_lvlset->get_palette()); draw_blocks(0, 0, 32, 26, m_bg_dc); update(0, 0, 32*16, 26*16);}
      void invalidate_screen() {invalidate_blocks(BLOCK_X_MIN, BLOCK_Y_MIN, BLOCK_X_MAX, BLOCK_Y_MAX);}

      void set_top      (int num)     {if(m_lvlset->set_top(num)) update_top();}
      void set_bottom   (int num)     {if(m_lvlset->set_bottom(num)) update_bottom();}
      void redraw       (RECT* area)  {BitBlt(m_hdc, area->left+m_base_x, area->top+m_base_y, area->right-area->left, area->bottom-area->top, m_work_dc, area->left, area->top, SRCCOPY);}
      void cancel       (BOOL set_focus = TRUE);
      void toggle_arrows()            {m_show_arrows = !m_show_arrows;update_all();}
      BOOL get_arrows   ()            {return m_show_arrows;}
      void paint_solid  ()            {cancel();m_state = STATE_PAINT_SOLID;newoper();}
      void paint_up     ()            {cancel();m_state = STATE_PAINT_UP;newoper();}
      void paint_left   ()            {cancel();m_state = STATE_PAINT_LEFT;newoper();}
      void paint_right  ()            {cancel();m_state = STATE_PAINT_RIGHT;newoper();}
      void paint_down   ()            {cancel();m_state = STATE_PAINT_DOWN;newoper();}
      void change_cursor();
      BOOL pickup_item  (int item);
      void drop_item    (BOOL abort);
      void set_mouse    (int x, int y);
      BOOL mouse_l_down (int x, int y);
      BOOL mouse_r_down (int x, int y);
      BOOL mouse_l_up   (int x, int y);
      BOOL mouse_r_up   (int x, int y);
      BOOL mouse_move   (int x, int y);
      void clip_mouse_x (int x);
      void newoper      ()            {InvalidateRect(m_hoper_button, NULL, FALSE);}
   private:
      void draw_blocks        (int startx, int starty, int endx, int endy, HDC hdc);
      void draw_monsters      (int startx, int starty, int endx, int endy, HDC hdc);
      void draw_items         (int startx, int starty, int endx, int endy, HDC hdc);
      void invalidate_blocks  (int startx, int starty, int endx, int endy) {invalidate_region(startx<<4, starty<<4, (endx<<4)+15, (endy<<4)+15);}
      void invalidate_region  (int startx, int starty, int endx, int endy);
      void invalidate_sprite32(int x, int y)   {invalidate_blocks(x>>4, y>>4, (x>>4)+2, (y>>4)+2);}
      BOOL in_client_area     (int x, int y)   {return x >= PIXEL_X_MIN && x <= PIXEL_X_MAX && y >= PIXEL_Y_MIN && y <= PIXEL_Y_MAX;}
      BOOL is_valid_region32  (int x, int y)   {return x >= REGION32_X_MIN && x <= REGION32_X_MAX && y >= REGION32_Y_MIN && y <= REGION32_Y_MAX;}
      void relative_xy        (int* x, int* y) {*x -= m_base_x;*y -= m_base_y;}
      void real_xy            (int* x, int* y) {*x += m_base_x;*y += m_base_y;}
      void screen_xy          (int* x, int* y);
      void make_valid_xy32    (int* x, int* y);
      void paint_block        (int x, int y, int direction);
      void update             (int xs, int ys, int xe, int ye);
      void update_sprite32    (int x, int y)   {update(x, y, x+32, y+32);}
      void update_top()                        {draw_blocks(0, 0, 31, 1, m_bg_dc); update(0, 0, 32*16, 32);}
      void update_bottom()                     {draw_blocks(0, 24, 31, 25, m_bg_dc); update(0, 25*16, 32*16, 26*16);}

      GfxSet* m_gfxset;
      LvlSet* m_lvlset;
      int     m_crsr_x;
      int     m_crsr_y;
      int     m_move_item;
      BOOL    m_mouse_l_down;
      BOOL    m_mouse_r_down;
      int     m_state;
      int     m_base_x;
      int     m_base_y;
      BOOL    m_show_arrows;
      HWND    m_hwnd;
      HWND    m_hoper_button;
      HDC     m_hdc;
      HDC     m_bg_dc;
      HDC     m_work_dc;
      BOOL    m_grid;
      HBITMAP m_work_bmp;
      HBITMAP m_bg_bmp;
      BOOL    m_mode_drop;
      BOOL    m_drop_anim;
      int     m_drop_pulse_clk;
};
#endif
