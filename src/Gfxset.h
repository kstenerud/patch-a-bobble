#ifndef GFXSET__HEADER
#define GFXSET__HEADER

#include <windows.h>
#include "limits.h"

#define FILESIZE 0x8000

#define T_SOLID        0
#define T_BOTTOM_LEFT  1
#define T_BOTTOM_RIGHT 2
#define T_BOTTOM_BEGIN 3
#define T_BOTTOM       4
#define T_LEFT_BEGIN   5
#define T_LEFT         6
#define T_BLANK        7
#define T_LARGE1       8
#define T_LARGE2       9
#define T_LARGE3      10
#define T_LARGE4      11
#define T_MAX         11


void invert_x(char* data, int x, int y);
void invert_y(char* data, int x, int y);
void doublesprite(char* data, int width, int length, BOOL smoothing);
void smooth_sprite(char* data, int width, int length);
void get_8_pixels(char* buff1, char* buff2, char* disp);
HBITMAP make_mask(char* data, int x, int y);


class GfxSet
{
public:
   GfxSet               (HWND hwnd);
   ~GfxSet              ();
   BOOL load_roms       (char* rom1a_name,  char* rom1b_name,
                         char* rom2a_name,  char* rom2b_name,
                         char* rom3a_name,  char* rom3b_name,
                         char* rombsa_name, char* rombsb_name,
                         char* rombga_name, char* rombgb_name);
   BOOL set_level       (int level, int palette);
   BOOL set_palette     (int palette);
   HWND get_hwnd        () {return m_hwnd;}
   void toggle_smoothing() {m_smoothing = !m_smoothing;make_all_gfx();}
   BOOL get_smoothing   () {return m_smoothing;}

   BOOL draw_bitmap            (HBITMAP hBitmap, int x, int y, HDC hdc = NULL);
   BOOL draw_bitmap_transparent(HBITMAP hBitmap, HBITMAP hMask, int x, int y, HDC hdc = NULL);
   BOOL draw_icon              (HICON hIcon, int x, int y, HDC hdc = NULL);
   BOOL draw_block             (int x, int y, int adata, int bdata, BOOL show_arrows, HDC hdc = NULL);
   BOOL draw_button            (HBITMAP hbmp, BOOL is_up, HDC hdc);

   BOOL draw_up_button         (HDC hdcButton, BOOL is_up) {return draw_button(m_up_bmp, is_up, hdcButton);}
   BOOL draw_down_button       (HDC hdcButton, BOOL is_up) {return draw_button(m_down_bmp, is_up, hdcButton);}
   BOOL draw_left_button       (HDC hdcButton, BOOL is_up) {return draw_button(m_left_bmp, is_up, hdcButton);}
   BOOL draw_right_button      (HDC hdcButton, BOOL is_up) {return draw_button(m_right_bmp, is_up, hdcButton);}
   BOOL draw_block_button      (HDC hdcButton, BOOL is_up) {return draw_button(m_bgs_bmp[T_SOLID][0], is_up, hdcButton);}
   BOOL draw_palette_button    (HDC hdcButton)             {return draw_bitmap_transparent(m_palette_bmp, m_palette_mask, 0, 0, hdcButton);}
   BOOL draw_small_alien_button(HDC hdcButton)             {return draw_bitmap(m_small_alien_bmp, 0, 0, hdcButton);}

   HICON get_monster_icon  (int mnum, int dir) {RETURN_IF_VALID(mnum, 0, 7, m_monsters_icon[mnum][dir != 0]);}
   HICON get_weapon_icon   ()  {return m_weapon_icon;}
   HICON get_points_icon   ()  {return m_points_icon;}
   HICON get_bubble_icon   ()  {return m_bubble_icon;}
   HICON get_burst_icon    ()  {return m_burst_icon;}
   HICON get_hurry_icon    ()  {return m_hurry_icon;}
   HICON get_baron_icon    ()  {return m_baron_icon;}
   HICON get_fire_icon     ()  {return m_fire_icon;}
   HICON get_water_icon    ()  {return m_water_icon;}
   HICON get_lightning_icon()  {return m_lightning_icon;}

   BOOL draw_monster_icon  (int mnum, int dir, int x, int y, HDC hdc = NULL) {RETURN_IF_VALID(mnum, 0, 7, draw_icon(m_monsters_icon[mnum][dir != 0], x, y, hdc));}
   BOOL draw_weapon_icon   (int x, int y, HDC hdc = NULL)  {return draw_icon(m_weapon_icon, x, y, hdc);}
   BOOL draw_points_icon   (int x, int y, HDC hdc = NULL)  {return draw_icon(m_points_icon, x, y, hdc);}
   BOOL draw_bubble_icon   (int x, int y, HDC hdc = NULL)  {return draw_icon(m_bubble_icon, x, y, hdc);}
   BOOL draw_burst_icon    (int x, int y, HDC hdc = NULL)  {return draw_icon(m_burst_icon, x, y, hdc);}
   BOOL draw_hurry_icon    (int x, int y, HDC hdc = NULL)  {return draw_icon(m_hurry_icon, x, y, hdc);}
   BOOL draw_baron_icon    (int x, int y, HDC hdc = NULL)  {return draw_icon(m_baron_icon, x, y, hdc);}
   BOOL draw_fire_icon     (int x, int y, HDC hdc = NULL)  {return draw_icon(m_fire_icon, x, y, hdc);}
   BOOL draw_water_icon    (int x, int y, HDC hdc = NULL)  {return draw_icon(m_water_icon, x, y, hdc);}
   BOOL draw_lightning_icon(int x, int y, HDC hdc = NULL)  {return draw_icon(m_lightning_icon, x, y, hdc);}

   HBITMAP get_bg_bmp     (int bgnum, int dir) {RETURN_IF_VALID(bgnum, 0, T_MAX, m_bgs_bmp[bgnum][dir]);}
   HBITMAP get_up_bmp     ()  {return m_up_bmp;}
   HBITMAP get_down_bmp   ()  {return m_down_bmp;}
   HBITMAP get_left_bmp   ()  {return m_left_bmp;}
   HBITMAP get_right_bmp  ()  {return m_right_bmp;}
   HBITMAP get_up32_bmp   ()  {return m_up32_bmp;}
   HBITMAP get_down32_bmp ()  {return m_down32_bmp;}
   HBITMAP get_left32_bmp ()  {return m_left32_bmp;}
   HBITMAP get_right32_bmp()  {return m_right32_bmp;}
   HBITMAP get_blank32_bmp()  {return m_blank32_bmp;}
   HBITMAP get_block32_bmp()  {return m_block32_bmp;}

   BOOL draw_bg_bmp     (int bgnum, int dir, int x, int y, HDC hdc = NULL) {RETURN_IF_VALID(bgnum, 0, T_MAX, draw_bitmap(m_bgs_bmp[bgnum][dir], x, y, hdc));}
   BOOL draw_up_bmp     (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_up_bmp, x, y, hdc);}
   BOOL draw_down_bmp   (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_down_bmp, x, y, hdc);}
   BOOL draw_left_bmp   (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_left_bmp, x, y, hdc);}
   BOOL draw_right_bmp  (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_right_bmp, x, y, hdc);}
   BOOL draw_up32_bmp   (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_up32_bmp, x, y, hdc);}
   BOOL draw_down32_bmp (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_down32_bmp, x, y, hdc);}
   BOOL draw_left32_bmp (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_left32_bmp, x, y, hdc);}
   BOOL draw_right32_bmp(int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_right32_bmp, x, y, hdc);}
   BOOL draw_blank32_bmp(int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_blank32_bmp, x, y, hdc);}
   BOOL draw_block32_bmp(int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_block32_bmp, x, y, hdc);}

   HBITMAP get_monster_bmp  (int mnum, int dir) {RETURN_IF_VALID(mnum, 0, 7, m_monsters_bmp[mnum][dir != 0]);}
   HBITMAP get_weapon_bmp   ()  {return m_weapon_bmp;}
   HBITMAP get_points_bmp   ()  {return m_points_bmp;}
   HBITMAP get_bubble_bmp   ()  {return m_bubble_bmp;}
   HBITMAP get_burst_bmp    ()  {return m_burst_bmp;}
   HBITMAP get_hurry_bmp    ()  {return m_hurry_bmp;}
   HBITMAP get_baron_bmp    ()  {return m_baron_bmp;}
   HBITMAP get_fire_bmp     ()  {return m_fire_bmp;}
   HBITMAP get_water_bmp    ()  {return m_water_bmp;}
   HBITMAP get_lightning_bmp()  {return m_lightning_bmp;}

   BOOL draw_monster_bmp  (int mnum, int dir, int x, int y, HDC hdc = NULL) {RETURN_IF_VALID(mnum, 0, 7, draw_bitmap(m_monsters_bmp[mnum][dir != 0], x, y, hdc));}
   BOOL draw_weapon_bmp   (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_weapon_bmp, x, y, hdc);}
   BOOL draw_points_bmp   (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_points_bmp, x, y, hdc);}
   BOOL draw_bubble_bmp   (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_bubble_bmp, x, y, hdc);}
   BOOL draw_burst_bmp    (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_burst_bmp, x, y, hdc);}
   BOOL draw_hurry_bmp    (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_hurry_bmp, x, y, hdc);}
   BOOL draw_baron_bmp    (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_baron_bmp, x, y, hdc);}
   BOOL draw_fire_bmp     (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_fire_bmp, x, y, hdc);}
   BOOL draw_water_bmp    (int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_water_bmp, x, y, hdc);}
   BOOL draw_lightning_bmp(int x, int y, HDC hdc = NULL)  {return draw_bitmap(m_lightning_bmp, x, y, hdc);}
private:
   BOOL    load_data        (char* roma_name, char* romb_name);
   HICON   make_icon        (int romnum, int charnum, int palette, int inv_x = 0) {return make_icon(make_bitmap32(romnum, charnum, palette, inv_x), make_mask32(romnum, charnum, inv_x));}
   HICON   make_icon        (HBITMAP hBmap, HBITMAP hMask);
   void    combine_bmp16    (HBITMAP bg_bmp, HBITMAP fg_bmp);
   HBITMAP make_bitmap16    (int romnum, int charnum, int palette, int inv_x = 0);
   HBITMAP make_bitmap32    (int romnum, int charnum, int palette, int inv_x = 0);
   HBITMAP map_16_32        (HBITMAP bmp16);
   HBITMAP make_small_bitmap(int romnum, int charnum, int palette, int inv_x = 0);
   HBITMAP make_mask16      (int romnum, int charnum, int inv_x = 0);
   HBITMAP make_mask32      (int romnum, int charnum, int inv_x = 0);
   HBITMAP make_small_mask  (int romnum, int charnum, int inv_x = 0);
   HBITMAP make_bitmap      (int height, int width, int depth, int palette, char* data);
   void    get_sprite16     (int romnum, int charnum, char* disp);
   void    get_sprite32     (int romnum, int charnum, char* disp);
   void    delete_all_gfx   () {delete_fg_gfx();delete_bg_gfx();}
   void    delete_fg_gfx    ();
   void    delete_bg_gfx    ();
   void    make_all_gfx     () {make_fg_gfx();make_bg_gfx();}
   void    make_fg_gfx      ();
   void    make_bg_gfx      ();
   void    setpalette       (RGBQUAD* rgb, int palette);


   int     m_level;
   BOOL    m_smoothing;
   HWND    m_hwnd;
   HDC     m_hdc;
   int     m_palette;
   char    m_roma[5][FILESIZE];
   char    m_romb[5][FILESIZE];
   HICON   m_monsters_icon[8][2];
   HICON   m_weapon_icon;
   HICON   m_points_icon;
   HICON   m_bubble_icon;
   HICON   m_burst_icon;
   HICON   m_hurry_icon;
   HICON   m_baron_icon;
   HICON   m_fire_icon;
   HICON   m_water_icon;
   HICON   m_lightning_icon;

   HBITMAP m_bgs_bmp[T_MAX+1][5];
   HBITMAP m_up_bmp;
   HBITMAP m_down_bmp;
   HBITMAP m_left_bmp;
   HBITMAP m_right_bmp;
   HBITMAP m_up32_bmp;
   HBITMAP m_down32_bmp;
   HBITMAP m_left32_bmp;
   HBITMAP m_right32_bmp;
   HBITMAP m_blank32_bmp;
   HBITMAP m_block32_bmp;

   HBITMAP m_monsters_bmp[8][2];
   HBITMAP m_weapon_bmp;
   HBITMAP m_points_bmp;
   HBITMAP m_bubble_bmp;
   HBITMAP m_burst_bmp;
   HBITMAP m_hurry_bmp;
   HBITMAP m_baron_bmp;
   HBITMAP m_fire_bmp;
   HBITMAP m_water_bmp;
   HBITMAP m_lightning_bmp;

   HBITMAP m_palette_bmp;
   HBITMAP m_palette_mask;
   HBITMAP m_small_alien_bmp;
   HBITMAP m_small_alien_mask;
};
#endif
