#include "gfxset.h"
#include "util.h"
#include <windowsx.h>

#define BUBBLE_GFX_OFFSET     100 // 7/13
#define M1_GFX_OFFSET         356 // 7/13
#define M2_GFX_OFFSET         932 // 7/13
#define M5_GFX_OFFSET         876 // 7/13
#define ALIEN_GFX_OFFSET      376 // 7/13
#define DINO_GFX_OFFSET       588 // 7/13
#define WEAPON_GFX_OFFSET    1364 // 7/13
#define POINTS_GFX_OFFSET    1980 // 7/13
#define HURRY_OFFSET          380 // 7/13

#define M3_GFX_OFFSET           8 // 8/14
#define M6_GFX_OFFSET        1228 // 8/14
#define FIRE_GFX_OFFSET      1028 // 8/14
#define WATER_GFX_OFFSET     1076 // 8/14
#define LIGHTNING_GFX_OFFSET 1124 // 8/14
#define BARON_GFX_OFFSET     1256 // 8/14
#define BURST_GFX_OFFSET     1312 // 8/14  1312 or 1324

#define M4_GFX_OFFSET        1544  // 9/15
#define M7_GFX_OFFSET        1800  // 9/15

#define ROMBUFF_1  0
#define ROMBUFF_2  1
#define ROMBUFF_3  2
#define ROMBUFF_BG 3
#define ROMBUFF_SEC 4
#define ROMBUFF_MIN 0
#define ROMBUFF_MAX 4

#define BBIT0 0x80
#define BBIT1 0x40
#define BBIT2 0x20
#define BBIT3 0x10
#define BBIT4 0x08
#define BBIT5 0x04
#define BBIT6 0x02
#define BBIT7 0x01

#define LBIT0 0x01
#define LBIT1 0x02
#define LBIT2 0x04
#define LBIT3 0x08
#define LBIT4 0x10
#define LBIT5 0x20
#define LBIT6 0x40
#define LBIT7 0x80

#define OFF_T_LARGE        516
#define OFF_T_SMALL        520
#define T_LEN                5
#define OFF_T_BOTTOM_LEFT  240
#define OFF_T_BOTTOM_RIGHT 243
#define OFF_T_BOTTOM_BEGIN 245
#define OFF_T_BOTTOM       242
#define OFF_T_LEFT_BEGIN   241
#define OFF_T_LEFT         244

#define OFF_T_SECRET_LARGE 1173 // 10/16
#define OFF_T_SECRET_SMALL 1172 // 10/16

#define MAX_CHAR 2048



GfxSet::GfxSet(HWND hwnd)
: m_level(0)
, m_smoothing(TRUE)
, m_weapon_icon(NULL)
, m_points_icon(NULL)
, m_bubble_icon(NULL)
, m_burst_icon(NULL)
, m_hurry_icon(NULL)
, m_baron_icon(NULL)
, m_fire_icon(NULL)
, m_water_icon(NULL)
, m_lightning_icon(NULL)
, m_up_bmp(NULL)
, m_down_bmp(NULL)
, m_left_bmp(NULL)
, m_right_bmp(NULL)
, m_up32_bmp(NULL)
, m_down32_bmp(NULL)
, m_left32_bmp(NULL)
, m_right32_bmp(NULL)
, m_blank32_bmp(NULL)
, m_block32_bmp(NULL)
, m_weapon_bmp(NULL)
, m_points_bmp(NULL)
, m_bubble_bmp(NULL)
, m_burst_bmp(NULL)
, m_hurry_bmp(NULL)
, m_baron_bmp(NULL)
, m_fire_bmp(NULL)
, m_water_bmp(NULL)
, m_lightning_bmp(NULL)
, m_palette_bmp(NULL)
, m_palette_mask(NULL)
, m_small_alien_bmp(NULL)
, m_small_alien_mask(NULL)
{
   int i;
   int j;

   m_hwnd = hwnd;
   m_hdc = GetDC(hwnd);

   for(i=MONSTER_NUM_MIN;i<=MONSTER_NUM_MAX;i++)
   {
      m_monsters_icon[i][0] = NULL;
      m_monsters_icon[i][1] = NULL;
      m_monsters_bmp[i][0] = NULL;
      m_monsters_bmp[i][1] = NULL;
   }

   for(i=0;i<5;i++)
      for(j=0;j<=T_MAX;j++)
            m_bgs_bmp[j][i] = NULL;


   load_roms(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   set_level(1, 1);
}

GfxSet::~GfxSet()
{
   delete_all_gfx();
}

BOOL GfxSet::load_roms(char* rom1a_name,  char* rom1b_name,
                       char* rom2a_name,  char* rom2b_name,
                       char* rom3a_name,  char* rom3b_name,
                       char* rombga_name, char* rombgb_name,
                       char* rombsa_name, char* rombsb_name)
{
   if(rom1a_name == NULL ||
      !(load_file(rom1a_name, m_roma[0], FILESIZE) &&
        load_file(rom1b_name, m_romb[0], FILESIZE) &&
        load_file(rom2a_name, m_roma[1], FILESIZE) &&
        load_file(rom2b_name, m_romb[1], FILESIZE) &&
        load_file(rom3a_name, m_roma[2], FILESIZE) &&
        load_file(rom3b_name, m_romb[2], FILESIZE) &&
        load_file(rombga_name, m_roma[3], FILESIZE) &&
        load_file(rombgb_name, m_romb[3], FILESIZE) &&
        load_file(rombsa_name, m_roma[4], FILESIZE) &&
        load_file(rombsb_name, m_romb[4], FILESIZE)
      ))
   {
      memset(m_roma[0], 0, FILESIZE);
      memset(m_romb[0], 0, FILESIZE);
      memset(m_roma[1], 0, FILESIZE);
      memset(m_romb[1], 0, FILESIZE);
      memset(m_roma[2], 0, FILESIZE);
      memset(m_romb[2], 0, FILESIZE);
      memset(m_roma[3], 0, FILESIZE);
      memset(m_romb[3], 0, FILESIZE);
      return FALSE;
   }
   make_all_gfx();
   return TRUE;
}

void GfxSet::make_fg_gfx()
{
   char arrow_up[32] =
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   };
   char arrow_right[32] =
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
      0x00, 0x00, 0x80, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x01, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   };
   char arrow_left[32] =
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x80, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x80,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   };
   char arrow_down[32] =
   {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x04, 0x20, 0x02, 0x40, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   };
//   char blank[32] =
//   {
//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//   };

   char palette[128] =
   {
     0x00, 0x00, 0xe8, 0x88, 0x88, 0xee, 0x00, 0x00,
     0x00, 0x08, 0x87, 0xb7, 0xb7, 0xb7, 0xee, 0x00,
     0x00, 0x8b, 0x7b, 0x7b, 0x66, 0x6b, 0x7b, 0xe0,
     0x08, 0xb7, 0x22, 0xb7, 0x66, 0x66, 0xb7, 0xbe,
     0x88, 0x71, 0x11, 0x2b, 0x7b, 0x66, 0x7b, 0x7e,
     0x87, 0xb7, 0x11, 0xb7, 0xb7, 0xb7, 0xb7, 0xbe,
     0x8b, 0x7b, 0x7b, 0x7b, 0x7e, 0xee, 0xeb, 0x8e,
     0x87, 0x44, 0x47, 0xb7, 0xbe, 0xf6, 0xeb, 0xe0,
     0x8b, 0x33, 0x4b, 0x7b, 0x7e, 0xee, 0x6e, 0x00,
     0x87, 0x33, 0xb7, 0xb7, 0xb7, 0xb8, 0x06, 0xe0,
     0x8b, 0x7b, 0x7a, 0xaa, 0x7b, 0x8e, 0x00, 0xce,
     0x8f, 0xb7, 0xaa, 0xaa, 0xb7, 0xe0, 0x00, 0xee,
     0x88, 0xfb, 0x7a, 0xab, 0x78, 0xe0, 0x00, 0xee,
     0x08, 0xff, 0xb7, 0xb7, 0xbe, 0x00, 0x00, 0x0e,
     0x00, 0x88, 0xff, 0xf8, 0xe0, 0x00, 0x00, 0x00,
     0x00, 0x08, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00,
   };

   delete_fg_gfx();

   m_palette_bmp  = make_bitmap(16, 16, 4, -3, palette);
   invert_y(palette, 16, 16);
   m_palette_mask = make_mask(palette, 16, 16);
   invert_y(palette, 16, 16);

   m_up_bmp      = CreateBitmap(16, 16, 1, 1, arrow_up);
   m_down_bmp    = CreateBitmap(16, 16, 1, 1, arrow_down);
   m_left_bmp    = CreateBitmap(16, 16, 1, 1, arrow_left);
   m_right_bmp   = CreateBitmap(16, 16, 1, 1, arrow_right);
   m_up32_bmp    = map_16_32(m_up_bmp);
   m_down32_bmp  = map_16_32(m_down_bmp);
   m_left32_bmp  = map_16_32(m_left_bmp);
   m_right32_bmp = map_16_32(m_right_bmp);
   m_blank32_bmp = map_16_32(NULL);

   m_weapon_icon         = make_icon(ROMBUFF_1, WEAPON_GFX_OFFSET,        -2);
   m_points_icon         = make_icon(ROMBUFF_1, POINTS_GFX_OFFSET,        -2);
   m_monsters_icon[0][0] = make_icon(ROMBUFF_1, M1_GFX_OFFSET,            -1);
   m_monsters_icon[0][1] = make_icon(ROMBUFF_1, M1_GFX_OFFSET,            -1, 1);
   m_monsters_icon[7][0] = make_icon(ROMBUFF_1, ALIEN_GFX_OFFSET,         -1);
   m_monsters_icon[7][1] = make_icon(ROMBUFF_1, ALIEN_GFX_OFFSET,         -1, 1);
   m_monsters_icon[4][0] = make_icon(ROMBUFF_1, M5_GFX_OFFSET,            -1);
   m_monsters_icon[4][1] = make_icon(ROMBUFF_1, M5_GFX_OFFSET,            -1, 1);
   m_monsters_icon[1][0] = make_icon(ROMBUFF_1, M2_GFX_OFFSET,            -1);
   m_monsters_icon[1][1] = make_icon(ROMBUFF_1, M2_GFX_OFFSET,            -1, 1);
   m_bubble_icon         = make_icon(ROMBUFF_1, BUBBLE_GFX_OFFSET,        -1);
   m_hurry_icon          = make_icon(ROMBUFF_1, HURRY_OFFSET,             -1, 1);

   m_weapon_bmp          = make_bitmap32(ROMBUFF_1, WEAPON_GFX_OFFSET,    -2);
   m_points_bmp          = make_bitmap32(ROMBUFF_1, POINTS_GFX_OFFSET,    -2);
   m_monsters_bmp[0][0]  = make_bitmap32(ROMBUFF_1, M1_GFX_OFFSET,        -1);
   m_monsters_bmp[0][1]  = make_bitmap32(ROMBUFF_1, M1_GFX_OFFSET,        -1, 1);
   m_monsters_bmp[7][0]  = make_bitmap32(ROMBUFF_1, ALIEN_GFX_OFFSET,     -1);
   m_monsters_bmp[7][1]  = make_bitmap32(ROMBUFF_1, ALIEN_GFX_OFFSET,     -1, 1);
   m_monsters_bmp[4][0]  = make_bitmap32(ROMBUFF_1, M5_GFX_OFFSET,        -1);
   m_monsters_bmp[4][1]  = make_bitmap32(ROMBUFF_1, M5_GFX_OFFSET,        -1, 1);
   m_monsters_bmp[1][0]  = make_bitmap32(ROMBUFF_1, M2_GFX_OFFSET,        -1);
   m_monsters_bmp[1][1]  = make_bitmap32(ROMBUFF_1, M2_GFX_OFFSET,        -1, 1);
   m_bubble_bmp          = make_bitmap32(ROMBUFF_1, BUBBLE_GFX_OFFSET,    -1);
   m_hurry_bmp           = make_bitmap32(ROMBUFF_1, BUBBLE_GFX_OFFSET,    -1, 1);

   m_small_alien_bmp     = make_small_bitmap(ROMBUFF_1, ALIEN_GFX_OFFSET,     -1);
   m_small_alien_mask    = make_small_mask(ROMBUFF_1, ALIEN_GFX_OFFSET,     -1);

   m_monsters_icon[2][0] = make_icon(ROMBUFF_2, M3_GFX_OFFSET,            -1);
   m_monsters_icon[2][1] = make_icon(ROMBUFF_2, M3_GFX_OFFSET,            -1, 1);
   m_monsters_icon[5][0] = make_icon(ROMBUFF_2, M6_GFX_OFFSET,            -1);
   m_monsters_icon[5][1] = make_icon(ROMBUFF_2, M6_GFX_OFFSET,            -1, 1);
   m_fire_icon           = make_icon(ROMBUFF_2, FIRE_GFX_OFFSET,          -1, 1);
   m_water_icon          = make_icon(ROMBUFF_2, WATER_GFX_OFFSET,         -1, 1);
   m_lightning_icon      = make_icon(ROMBUFF_2, LIGHTNING_GFX_OFFSET,     -1, 1);
   m_baron_icon          = make_icon(ROMBUFF_2, BARON_GFX_OFFSET,         -1, 1);
   m_burst_icon          = make_icon(ROMBUFF_2, BURST_GFX_OFFSET,         -1, 1);

   m_monsters_bmp[2][0]  = make_bitmap32(ROMBUFF_2, M3_GFX_OFFSET,        -1);
   m_monsters_bmp[2][1]  = make_bitmap32(ROMBUFF_2, M3_GFX_OFFSET,        -1, 1);
   m_monsters_bmp[5][0]  = make_bitmap32(ROMBUFF_2, M6_GFX_OFFSET,        -1);
   m_monsters_bmp[5][1]  = make_bitmap32(ROMBUFF_2, M6_GFX_OFFSET,        -1, 1);
   m_fire_bmp            = make_bitmap32(ROMBUFF_2, FIRE_GFX_OFFSET,      -1, 1);
   m_water_bmp           = make_bitmap32(ROMBUFF_2, WATER_GFX_OFFSET,     -1, 1);
   m_lightning_bmp       = make_bitmap32(ROMBUFF_2, LIGHTNING_GFX_OFFSET, -1, 1);
   m_baron_bmp           = make_bitmap32(ROMBUFF_2, BARON_GFX_OFFSET,     -1, 1);
   m_burst_bmp           = make_bitmap32(ROMBUFF_2, BURST_GFX_OFFSET,     -1, 1);

   m_monsters_icon[3][0] = make_icon(ROMBUFF_3, M4_GFX_OFFSET,            -1);
   m_monsters_icon[3][1] = make_icon(ROMBUFF_3, M4_GFX_OFFSET,            -1, 1);
   m_monsters_icon[6][0] = make_icon(ROMBUFF_3, M7_GFX_OFFSET,            -1);
   m_monsters_icon[6][1] = make_icon(ROMBUFF_3, M7_GFX_OFFSET,            -1, 1);

   m_monsters_bmp[3][0] = make_bitmap32(ROMBUFF_3, M4_GFX_OFFSET,         -1);
   m_monsters_bmp[3][1] = make_bitmap32(ROMBUFF_3, M4_GFX_OFFSET,         -1, 1);
   m_monsters_bmp[6][0] = make_bitmap32(ROMBUFF_3, M7_GFX_OFFSET,         -1);
   m_monsters_bmp[6][1] = make_bitmap32(ROMBUFF_3, M7_GFX_OFFSET,         -1, 1);
}

void GfxSet::make_bg_gfx()
{
   HBITMAP hbmps[4] = {m_up_bmp, m_right_bmp, m_left_bmp, m_down_bmp};
   int i;
   int j;
   int offset = T_LEN*(m_level-1);

   delete_bg_gfx();

   if(m_level == 102)
   {
      for(i=0;i<5;i++)
      {
         m_bgs_bmp[T_SOLID][i]        = make_bitmap16(ROMBUFF_SEC, OFF_T_SECRET_SMALL,   -4);
         m_bgs_bmp[T_BOTTOM_LEFT][i]  = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_BOTTOM_RIGHT][i] = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_BOTTOM_BEGIN][i] = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_BOTTOM][i]       = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_LEFT_BEGIN][i]   = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_LEFT][i]         = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_BLANK][i]        = make_bitmap16(ROMBUFF_SEC, -1,                   -4);
         m_bgs_bmp[T_LARGE1][i]       = make_bitmap16(ROMBUFF_SEC, OFF_T_SECRET_LARGE,   -4);
         m_bgs_bmp[T_LARGE2][i]       = make_bitmap16(ROMBUFF_SEC, OFF_T_SECRET_LARGE+1, -4);
         m_bgs_bmp[T_LARGE3][i]       = make_bitmap16(ROMBUFF_SEC, OFF_T_SECRET_LARGE+2, -4);
         m_bgs_bmp[T_LARGE4][i]       = make_bitmap16(ROMBUFF_SEC, OFF_T_SECRET_LARGE+3, -4);
      }
   }
   else
   {
      for(i=0;i<5;i++)
      {
         m_bgs_bmp[T_SOLID][i]        = make_bitmap16(ROMBUFF_BG, OFF_T_SMALL+offset,   m_palette);
         m_bgs_bmp[T_BOTTOM_LEFT][i]  = make_bitmap16(ROMBUFF_BG, OFF_T_BOTTOM_LEFT,    m_palette);
         m_bgs_bmp[T_BOTTOM_RIGHT][i] = make_bitmap16(ROMBUFF_BG, OFF_T_BOTTOM_RIGHT,   m_palette);
         m_bgs_bmp[T_BOTTOM_BEGIN][i] = make_bitmap16(ROMBUFF_BG, OFF_T_BOTTOM_BEGIN,   m_palette);
         m_bgs_bmp[T_BOTTOM][i]       = make_bitmap16(ROMBUFF_BG, OFF_T_BOTTOM,         m_palette);
         m_bgs_bmp[T_LEFT_BEGIN][i]   = make_bitmap16(ROMBUFF_BG, OFF_T_LEFT_BEGIN,     m_palette);
         m_bgs_bmp[T_LEFT][i]         = make_bitmap16(ROMBUFF_BG, OFF_T_LEFT,           m_palette);
         m_bgs_bmp[T_BLANK][i]        = make_bitmap16(ROMBUFF_BG, -1,                   m_palette);
         m_bgs_bmp[T_LARGE1][i]       = make_bitmap16(ROMBUFF_BG, OFF_T_LARGE+offset,   m_palette);
         m_bgs_bmp[T_LARGE2][i]       = make_bitmap16(ROMBUFF_BG, OFF_T_LARGE+offset+1, m_palette);
         m_bgs_bmp[T_LARGE3][i]       = make_bitmap16(ROMBUFF_BG, OFF_T_LARGE+offset+2, m_palette);
         m_bgs_bmp[T_LARGE4][i]       = make_bitmap16(ROMBUFF_BG, OFF_T_LARGE+offset+3, m_palette);
      }
   }
   
   for(i=0;i<4;i++)
      for(j=0;j<T_MAX;j++)
         combine_bmp16(m_bgs_bmp[j][i+1], hbmps[i]);

   m_block32_bmp = map_16_32(m_bgs_bmp[T_SOLID][0]);
}

void GfxSet::delete_fg_gfx()
{
   int i;

   if(m_block32_bmp    != NULL) {DeleteObject(m_block32_bmp);    m_block32_bmp    = NULL;}

   if(m_palette_bmp    != NULL) {DeleteObject(m_palette_bmp);    m_palette_bmp    = NULL;}
   if(m_palette_mask   != NULL) {DeleteObject(m_palette_mask);   m_palette_mask   = NULL;}

   if(m_weapon_icon    != NULL) {DeleteObject(m_weapon_icon);    m_weapon_icon    = NULL;}
   if(m_points_icon    != NULL) {DeleteObject(m_points_icon);    m_points_icon    = NULL;}
   if(m_bubble_icon    != NULL) {DeleteObject(m_bubble_icon);    m_bubble_icon    = NULL;}
   if(m_burst_icon     != NULL) {DeleteObject(m_burst_icon);     m_burst_icon     = NULL;}
   if(m_baron_icon     != NULL) {DeleteObject(m_baron_icon);     m_baron_icon     = NULL;}
   if(m_fire_icon      != NULL) {DeleteObject(m_fire_icon);      m_fire_icon      = NULL;}
   if(m_water_icon     != NULL) {DeleteObject(m_water_icon);     m_water_icon     = NULL;}
   if(m_lightning_icon != NULL) {DeleteObject(m_lightning_icon); m_lightning_icon = NULL;}

   if(m_weapon_bmp     != NULL) {DeleteObject(m_weapon_bmp);     m_weapon_bmp     = NULL;}
   if(m_points_bmp     != NULL) {DeleteObject(m_points_bmp);     m_points_bmp     = NULL;}
   if(m_bubble_bmp     != NULL) {DeleteObject(m_bubble_bmp);     m_bubble_bmp     = NULL;}
   if(m_burst_bmp      != NULL) {DeleteObject(m_burst_bmp);      m_burst_bmp      = NULL;}
   if(m_baron_bmp      != NULL) {DeleteObject(m_baron_bmp);      m_baron_bmp      = NULL;}
   if(m_fire_bmp       != NULL) {DeleteObject(m_fire_bmp);       m_fire_bmp       = NULL;}
   if(m_water_bmp      != NULL) {DeleteObject(m_water_bmp);      m_water_bmp      = NULL;}
   if(m_lightning_bmp  != NULL) {DeleteObject(m_lightning_bmp);  m_lightning_bmp  = NULL;}
   if(m_up_bmp         != NULL) {DeleteObject(m_up_bmp);         m_up_bmp         = NULL;}
   if(m_down_bmp       != NULL) {DeleteObject(m_down_bmp);       m_down_bmp       = NULL;}
   if(m_left_bmp       != NULL) {DeleteObject(m_left_bmp);       m_left_bmp       = NULL;}
   if(m_right_bmp      != NULL) {DeleteObject(m_right_bmp);      m_right_bmp      = NULL;}
   if(m_up32_bmp       != NULL) {DeleteObject(m_up32_bmp);       m_up32_bmp       = NULL;}
   if(m_down32_bmp     != NULL) {DeleteObject(m_down32_bmp);     m_down32_bmp     = NULL;}
   if(m_left32_bmp     != NULL) {DeleteObject(m_left32_bmp);     m_left32_bmp     = NULL;}
   if(m_right32_bmp    != NULL) {DeleteObject(m_right32_bmp);    m_right32_bmp    = NULL;}
   if(m_blank32_bmp    != NULL) {DeleteObject(m_blank32_bmp);    m_blank32_bmp    = NULL;}

   for(i=MONSTER_NUM_MIN;i<=MONSTER_NUM_MAX;i++)
   {
      if(m_monsters_icon[i][0] != NULL) {DeleteObject(m_monsters_icon[i][0]);m_monsters_icon[i][0] = NULL;}
      if(m_monsters_icon[i][1] != NULL) {DeleteObject(m_monsters_icon[i][1]);m_monsters_icon[i][1] = NULL;}
      if(m_monsters_bmp[i][0]  != NULL) {DeleteObject(m_monsters_bmp[i][0]);m_monsters_bmp[i][0] = NULL;}
      if(m_monsters_bmp[i][1]  != NULL) {DeleteObject(m_monsters_bmp[i][1]);m_monsters_bmp[i][1] = NULL;}
   }
   if(m_small_alien_bmp  != NULL) {DeleteObject(m_small_alien_bmp);m_small_alien_bmp = NULL;}
   if(m_small_alien_mask != NULL) {DeleteObject(m_small_alien_mask);m_small_alien_mask = NULL;}
}

void GfxSet::delete_bg_gfx()
{
   int i;
   int j;

   if(m_block32_bmp    != NULL) {DeleteObject(m_block32_bmp);m_block32_bmp = NULL;}

   for(i=0;i<5;i++)
      for(j=0;j<=T_MAX;j++)
         if(m_bgs_bmp[j][i] != NULL)
            {DeleteObject(m_bgs_bmp[j][i]);m_bgs_bmp[j][i] = NULL;}
}


BOOL GfxSet::set_level(int level, int palette)
{
   if(level == m_level && palette == m_palette)
      return FALSE;

   m_level = level;
   m_palette = palette;
   make_bg_gfx();
   return TRUE;
}

BOOL GfxSet::set_palette(int palette)
{
   if(palette == m_palette)
      return FALSE;

   m_palette = palette;
   make_bg_gfx();
   return TRUE;
}

void GfxSet::combine_bmp16(HBITMAP bg_bmp, HBITMAP fg_bmp)
{
   HDC bg_dc = CreateCompatibleDC(m_hdc);
   HDC fg_dc = CreateCompatibleDC(m_hdc);

   SelectObject(bg_dc, bg_bmp);
   SelectObject(fg_dc, fg_bmp);
   BitBlt(bg_dc, 0, 0, 16, 16, fg_dc, 0, 0, SRCPAINT);

   DeleteDC(bg_dc);
   DeleteDC(fg_dc);
}

HICON GfxSet::make_icon(HBITMAP hBmap, HBITMAP hMask)
{
   ICONINFO iinfo;

   iinfo.fIcon = FALSE;
   iinfo.xHotspot = 0;
   iinfo.yHotspot = 0;
   iinfo.hbmMask = hMask;
   iinfo.hbmColor = hBmap;
   return CreateIconIndirect(&iinfo);
}

HBITMAP GfxSet::make_bitmap16(int romnum, int charnum, int palette, int inv_x)
{
   char data[256] = {0};

   if(charnum >= 0)
   {
      get_sprite16(romnum, charnum, data);
      doublesprite(data, 8, 8, m_smoothing);
      if(inv_x)
         invert_x(data, 16, 16);
   }

   return make_bitmap(16, 16, 4, palette, data);
}

HBITMAP GfxSet::map_16_32(HBITMAP bmp16)
{
   HDC memdc32 = CreateCompatibleDC(m_hdc);
   HDC memdc16 = CreateCompatibleDC(m_hdc);
   HBITMAP bmp32 = CreateCompatibleBitmap(m_hdc, 32, 32);

   SetMapMode(memdc32, GetMapMode(m_hdc));
   SetMapMode(memdc16, GetMapMode(m_hdc));
   SelectObject(memdc32, bmp32);
   BitBlt(memdc32, 0, 0, 32, 32, NULL, 0, 0, BLACKNESS);

   if(bmp16 != NULL)
   {
      SelectObject(memdc16, bmp16);
      BitBlt(memdc32, 8, 8, 16, 16, memdc16, 0, 0, SRCCOPY);
   }

   DeleteDC(memdc16);
   DeleteDC(memdc32);
   return bmp32;
}

HBITMAP GfxSet::make_bitmap32(int romnum, int charnum, int palette, int inv_x)
{
   char data[1024] = {0};

   get_sprite32(romnum, charnum, data);
   doublesprite(data, 16, 16, m_smoothing);
   if(inv_x)
      invert_x(data, 32, 32);

   return make_bitmap(32, 32, 4, palette, data);
}

HBITMAP GfxSet::make_small_bitmap(int romnum, int charnum, int palette, int inv_x)
{
   char data[256] = {0};

   get_sprite32(romnum, charnum, data);
   if(inv_x)
      invert_x(data, 16, 16);

   return make_bitmap(16, 16, 4, palette, data);
}

HBITMAP GfxSet::make_mask16(int romnum, int charnum, int inv_x)
{
   char data[256] = {0};

   if(charnum >= 0)
   {
      get_sprite16(romnum, charnum, data);
      doublesprite(data, 8, 8, m_smoothing);
      if(inv_x)
         invert_x(data, 16, 16);
   }

   return make_mask(data, 16, 16);
}

HBITMAP GfxSet::make_mask32(int romnum, int charnum, int inv_x)
{
   char data[1024] = {0};

   get_sprite32(romnum, charnum, data);
   doublesprite(data, 16, 16, m_smoothing);
   invert_y(data, 32, 32);
   if(inv_x)
      invert_x(data, 32, 32);

   return make_mask(data, 32, 32);
}

HBITMAP GfxSet::make_small_mask(int romnum, int charnum, int inv_x)
{
   char data[256] = {0};

   get_sprite32(romnum, charnum, data);
   if(inv_x)
      invert_x(data, 16, 16);

   return make_mask(data, 16, 16);
}


HBITMAP GfxSet:: make_bitmap(int height, int width, int depth, int palette, char* data)
{
   char buff[sizeof(BITMAPINFO)+sizeof(RGBQUAD)*15];
   BITMAPINFO* bmi = (BITMAPINFO*)buff;

   bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bmi->bmiHeader.biWidth = width;
   bmi->bmiHeader.biHeight = height;
   bmi->bmiHeader.biPlanes = 1;
   bmi->bmiHeader.biBitCount = (WORD)depth;
   bmi->bmiHeader.biCompression = 0;
   bmi->bmiHeader.biSizeImage = 0;
   bmi->bmiHeader.biXPelsPerMeter = 0;
   bmi->bmiHeader.biYPelsPerMeter = 0;
   bmi->bmiHeader.biClrUsed = 0;
   bmi->bmiHeader.biClrImportant = 0;
   setpalette(bmi->bmiColors, palette);

   return CreateDIBitmap(m_hdc, &(bmi->bmiHeader), CBM_INIT, data, bmi, DIB_RGB_COLORS);
}


BOOL GfxSet::draw_bitmap(HBITMAP hBitmap, int x, int y, HDC hdcDst)
{
   BITMAP bm;
   HDC hdcBmp;
   POINT size;
   POINT bmp;
   BOOL retval = FALSE;

   if(hBitmap == NULL)
      return FALSE;

   if(hdcDst == NULL)
      hdcDst = m_hdc;

   if( (hdcBmp = CreateCompatibleDC(hdcDst)) == NULL)
      return FALSE;
   if(SelectObject(hdcBmp, hBitmap) != NULL)
   {
      SetMapMode(hdcBmp, GetMapMode(hdcDst));

      if(GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm) != 0)
      {
         size.x = bm.bmWidth;
         size.y = bm.bmHeight;
         DPtoLP(hdcDst, &size, 1);

         bmp.x = 0;
         bmp.y = 0;
         DPtoLP(hdcBmp, &bmp, 1);

         retval = BitBlt(hdcDst, x, y, size.x, size.y, hdcBmp, bmp.x, bmp.y, SRCCOPY);
      }
   }
   DeleteDC(hdcBmp);
   return retval;
}

BOOL GfxSet::draw_bitmap_transparent(HBITMAP hBitmap, HBITMAP hMask, int x, int y, HDC hdcDst)
{
   BITMAP bm;
   HDC hdcBmp;
   POINT size;
   POINT bmp;
   BOOL retval = FALSE;

   if(hBitmap == NULL || hMask == NULL)
      return FALSE;

   if(hdcDst == NULL)
      hdcDst = m_hdc;

   if( (hdcBmp = CreateCompatibleDC(hdcDst)) == NULL)
      return FALSE;
   if(SelectObject(hdcBmp, hMask) != NULL)
   {
      SetMapMode(hdcBmp, GetMapMode(hdcDst));

      if(GetObject(hMask, sizeof(BITMAP), (LPVOID)&bm) != 0)
      {
         size.x = bm.bmWidth;
         size.y = bm.bmHeight;
         DPtoLP(hdcDst, &size, 1);

         bmp.x = 0;
         bmp.y = 0;
         DPtoLP(hdcBmp, &bmp, 1);

         if((retval = BitBlt(hdcDst, x, y, size.x, size.y, hdcBmp, bmp.x, bmp.y, SRCAND)) == TRUE)
         {
            SelectObject(hdcBmp, hBitmap);
            retval = BitBlt(hdcDst, x, y, size.x, size.y, hdcBmp, bmp.x, bmp.y, SRCPAINT);
         }
      }
   }
   DeleteDC(hdcBmp);
   return retval;
}

BOOL GfxSet::draw_icon(HICON hIcon, int x, int y, HDC hdc)
{
   if(hIcon == NULL)
      return FALSE;

   if(hdc == NULL)
      hdc = m_hdc;

   return DrawIcon(hdc, x, y, hIcon);
}

BOOL GfxSet::draw_block(int x, int y, int adata, int bdata, BOOL show_arrows, HDC hdc)
{
   if(show_arrows)
      return draw_bitmap(m_bgs_bmp[bdata][adata], x, y, hdc);
   else
      return draw_bitmap(m_bgs_bmp[bdata][0], x, y, hdc);
}

BOOL GfxSet::draw_button(HBITMAP hbmp, BOOL is_up, HDC hdcButton)
{
   HPEN pen_lt = CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNHIGHLIGHT));
   HPEN pen_dk = CreatePen(PS_SOLID, 2, GetSysColor(COLOR_BTNSHADOW));
   HPEN pen_old = SelectPen(hdcButton, pen_lt);

   if(is_up)
   {
      SelectObject(hdcButton, pen_dk);
      LineTo(hdcButton, 15, 0);
      draw_bitmap(hbmp, 0, 0, hdcButton);

//      MoveTo(hdcButton, 15, 0);
      LineTo(hdcButton, 15, 15);
      LineTo(hdcButton, 0, 15);

      SelectObject(hdcButton, pen_lt);
//      MoveTo(hdcButton, 0, 15);
      LineTo(hdcButton, 0, 0);
      LineTo(hdcButton, 15, 0);
   }
   else
   {
      SelectObject(hdcButton, pen_lt);
      LineTo(hdcButton, 15, 0);
      draw_bitmap(hbmp, 1, 1, hdcButton);

//      MoveTo(hdcButton, 15, 0);
      LineTo(hdcButton, 15, 15);
      LineTo(hdcButton, 0, 15);

      SelectObject(hdcButton, pen_dk);
//      MoveTo(hdcButton, 0, 15);
      LineTo(hdcButton, 0, 0);
      LineTo(hdcButton, 15, 0);
   }

   SelectObject(hdcButton, pen_old);
   DeleteObject(pen_lt);
   DeleteObject(pen_dk);
   return TRUE;
}


void GfxSet::setpalette(RGBQUAD* rgb, int palnum)
{
   int i;

   char pal_idx[12] = {0,  32,  68, 104, 148, 184, 220, 252,
                       85, 119, 136, 170};
   char palette[12][16][3] =
   {
      {  // -4: secret stage
         {0,0,0}, {0,1,1}, {0,11,11}, {0,0,0}, {0,0,0}, {0,0,0}, {0,9,9}, {0,0,0},
         {0,8,8}, {0,0,0}, {0,10,10}, {0,0,0}, {0,3,3}, {0,0,0}, {0,2,2}, {0,0,0}
      },
      {  // -3: palette
         {0,0,0}, {7,0,0}, {3,0,0}, {0,7,0}, {0,3,0}, {0,0,7}, {0,0,3}, {7,7,0},
         {3,3,0}, {7,0,7}, {3,0,3}, {5,5,5}, {3,3,3}, {0,0,0}, {0,0,0}, {7,7,7}
      },
      {  // -2: weapon/points
         {0,0,0}, {4,0,0}, {6,7,0}, {7,5,0}, {0,7,6}, {5,0,4}, {7,0,0}, {0,0,0},
         {5,0,0}, {3,0,6}, {0,7,0}, {0,5,0}, {7,4,0}, {7,3,2}, {7,0,2}, {7,7,6}
      },
      {  // -1: monsters
         {0,0,0}, {0,4,6}, {7,7,0}, {7,5,4}, {7,4,2}, {4,4,4}, {7,0,2}, {0,0,0},
         {6,2,0}, {5,5,6}, {0,7,0}, {0,5,0}, {7,4,0}, {4,0,6}, {0,5,6}, {7,7,6}
      },
      {  // 0
         {0,0,0}, {4,0,0}, {7,6,4}, {0,0,0}, {7,3,0}, {4,4,4}, {3,0,0}, {0,0,0},
         {5,1,0}, {5,5,6}, {4,0,0}, {0,0,0}, {7,4,2}, {3,3,4}, {7,0,0}, {0,0,0}
      },
      {  // 1
         {0,0,0}, {4,0,0}, {7,7,6}, {0,0,0}, {4,4,4}, {0,5,0}, {2,2,2}, {0,0,0},
         {3,3,4}, {3,3,6}, {3,3,4}, {0,0,0}, {5,5,6}, {0,7,6}, {7,0,0}, {0,0,0}
      },
      {  // 2
         {0,0,0}, {3,3,4}, {7,7,6}, {0,0,0}, {0,5,6}, {0,0,0}, {2,2,2}, {0,0,0},
         {0,4,6}, {0,0,0}, {3,3,4}, {0,0,0}, {0,7,6}, {0,0,0}, {3,3,6}, {0,0,0}
      },
      {  // 3
         {0,0,0}, {4,0,0}, {7,7,6}, {0,0,0}, {4,4,4}, {0,0,4}, {3,3,2}, {0,0,0},
         {4,4,4}, {0,3,6}, {3,3,2}, {0,0,0}, {6,6,6}, {0,0,0}, {7,0,0}, {0,0,0},
      },
      {  // 4
         {0,0,0}, {4,0,0}, {7,7,6}, {0,0,0}, {7,5,0}, {0,0,0}, {3,0,0}, {0,0,0},
         {7,0,2}, {0,0,0}, {4,0,0}, {0,0,0}, {7,7,0}, {0,0,0}, {0,7,0}, {0,0,0}
      },
      {  // 5
         {0,0,0}, {0,0,0}, {7,7,6}, {0,0,0}, {7,3,6}, {0,0,0}, {3,0,0}, {0,0,0},
         {7,0,4}, {0,0,0}, {4,0,2}, {0,0,0}, {7,5,6}, {0,0,0}, {7,0,0}, {0,0,0}
      },
      {  // 6
         {0,0,0}, {0,4,0}, {7,7,4}, {0,0,0}, {5,5,2}, {0,7,0}, {2,2,0}, {0,0,0},
         {4,4,2}, {0,6,0}, {3,3,0}, {0,0,0}, {6,6,4}, {7,7,6}, {7,0,0}, {0,0,0}
      },
      {  // 7
         {0,0,0}, {0,0,0}, {7,7,6}, {0,0,0}, {6,5,2}, {0,0,0}, {2,2,0}, {0,0,0},
         {5,4,2}, {0,0,0}, {3,3,0}, {0,0,0}, {7,6,4}, {7,7,6}, {7,0,0}, {0,0,0}
      }
   };

   if(palnum < -4 || palnum > 7)
      return;

   for(i=0;i<16;i++)
   {
      rgb[i].rgbRed   = pal_idx[palette[palnum+4][i][0]];
      rgb[i].rgbGreen = pal_idx[palette[palnum+4][i][1]];
      rgb[i].rgbBlue  = pal_idx[palette[palnum+4][i][2]];
      rgb[i].rgbReserved = 0;
   }
}

// 1 2x2 sprite = 128 bytes
void GfxSet::get_sprite32(int romnum, int charnum, char* disp)
{
   int i;
   char* ptr1;
   char* ptr2;

   if(romnum < ROMBUFF_MIN || romnum > ROMBUFF_MAX)
      return;

   ptr1 = m_roma[romnum] + (charnum<<4);
   ptr2 = m_romb[romnum] + (charnum<<4);

   if(charnum+4 >= MAX_CHAR)
   {
      for(i=0;i<128;i++)
         disp[i] = (char)0;
      return;
   }

   for(i=0;i<8;i++)
      get_8_pixels(ptr1+(i*2), ptr2+(i*2), disp-(i*8)+120);

   ptr1 += 16;
   ptr2 += 16;
   for(i=0;i<8;i++)
      get_8_pixels(ptr1+(i*2), ptr2+(i*2), disp-(i*8)+124);

   ptr1 += 16;
   ptr2 += 16;
   for(i=0;i<8;i++)
      get_8_pixels(ptr1+(i*2), ptr2+(i*2), disp-(i*8)+56);

   ptr1 += 16;
   ptr2 += 16;
   for(i=0;i<8;i++)
      get_8_pixels(ptr1+(i*2), ptr2+(i*2), disp-(i*8)+60);
}


// 1 char = 32 bytes
void GfxSet::get_sprite16(int romnum, int charnum, char* disp)
{
   int i;
   char* ptr1;
   char* ptr2;

   if(romnum < ROMBUFF_MIN || romnum > ROMBUFF_MAX)
      return;

   ptr1 = m_roma[romnum] + (charnum<<4);
   ptr2 = m_romb[romnum] + (charnum<<4);

   if(charnum+4 >= MAX_CHAR)
   {
      for(i=0;i<128;i++)
         disp[i] = (char)0;
      return;
   }

   for(i=0;i<8;i++)
      get_8_pixels(ptr1+(i*2), ptr2+(i*2), disp-(i*4)+28);
}


/*                       PIXEL
 *              0  1  2  3   4  5  6  7
 *
 *                 byte0        byte1
 * bitplane  0: 3, 2, 1, 0,  3, 2, 1, 0
 *           1: 7, 6, 5, 4,  7, 6, 5, 4
 *
 *                 byte2        byte3
 *           2: 3, 2, 1, 0,  3, 2, 1, 0
 *           3: 7, 6, 5, 4,  7, 6, 5, 4
 */
void get_8_pixels(char* buff1, char* buff2, char* disp)
{
   char inbuff[4];

   inbuff[0] = buff1[0];
   inbuff[1] = buff1[1];
   inbuff[2] = buff2[0];
   inbuff[3] = buff2[1];


   disp[0] = (char)((
                    (((inbuff[0] & BBIT3) != 0)*0x10) |
                    (((inbuff[0] & BBIT7) != 0)*0x20) |
                    (((inbuff[2] & BBIT3) != 0)*0x40) |
                    (((inbuff[2] & BBIT7) != 0)*0x80) |
                    (((inbuff[0] & BBIT2) != 0)*0x01) |
                    (((inbuff[0] & BBIT6) != 0)*0x02) |
                    (((inbuff[2] & BBIT2) != 0)*0x04) |
                    (((inbuff[2] & BBIT6) != 0)*0x08)
                   ) & 0xff);

   disp[1] = (char)((
                    (((inbuff[0] & BBIT1) != 0)*0x10) |
                    (((inbuff[0] & BBIT5) != 0)*0x20) |
                    (((inbuff[2] & BBIT1) != 0)*0x40) |
                    (((inbuff[2] & BBIT5) != 0)*0x80) |
                    (((inbuff[0] & BBIT0) != 0)*0x01) |
                    (((inbuff[0] & BBIT4) != 0)*0x02) |
                    (((inbuff[2] & BBIT0) != 0)*0x04) |
                    (((inbuff[2] & BBIT4) != 0)*0x08)
                   ) & 0xff);

   disp[2] = (char)((
                    (((inbuff[1] & BBIT3) != 0)*0x10) |
                    (((inbuff[1] & BBIT7) != 0)*0x20) |
                    (((inbuff[3] & BBIT3) != 0)*0x40) |
                    (((inbuff[3] & BBIT7) != 0)*0x80) |
                    (((inbuff[1] & BBIT2) != 0)*0x01) |
                    (((inbuff[1] & BBIT6) != 0)*0x02) |
                    (((inbuff[3] & BBIT2) != 0)*0x04) |
                    (((inbuff[3] & BBIT6) != 0)*0x08)
                   ) & 0xff);

   disp[3] = (char)((
                    (((inbuff[1] & BBIT1) != 0)*0x10) |
                    (((inbuff[1] & BBIT5) != 0)*0x20) |
                    (((inbuff[3] & BBIT1) != 0)*0x40) |
                    (((inbuff[3] & BBIT5) != 0)*0x80) |
                    (((inbuff[1] & BBIT0) != 0)*0x01) |
                    (((inbuff[1] & BBIT4) != 0)*0x02) |
                    (((inbuff[3] & BBIT0) != 0)*0x04) |
                    (((inbuff[3] & BBIT4) != 0)*0x08)
                   ) & 0xff);
}


char swap(char val)
{
   return (char)(((val & 0x0f)<<4) | ((val & 0xf0)>>4));
}

void invert_x(char* data, int x, int y)
{
   int cx;
   int cy;
   char* tmp = new char[x];

   x /=2;

   for(cy=0;cy<y;cy++)
   {
      for(cx=0;cx<x;cx++)
         tmp[(x-1)-cx] = data[cy*x+cx];
      for(cx=0;cx<x;cx++)
         data[cy*x+cx] = swap(tmp[cx]);
   }

   delete [] tmp;
}

void invert_y(char* data, int x, int y)
{
   int cx;
   int cy;
   char tmp;
   int first;
   int last;

   x/=2;

   for(cy=0;cy<y/2;cy++)
   {
      for(cx=0;cx<x;cx++)
      {
         first = cy*x+cx;
         last  = (y-cy-1)*x+cx;
         tmp         = data[first];
         data[first] = data[last];
         data[last]  = tmp;
      }
   }
}

//00 00 00
//00 00 00

char get_pixel(int y, int x, int width, char* data)
{
   int idx = y*width+x;

   if(!(idx % 2))
      return (char)((data[idx/2] & 0xf0) >> 4);
   return (char)(data[idx/2] & 0x0f);
}

void set_pixel(int y, int x, int width, char* data, int value)
{
   int idx = (y*width+x)/2;
   value &= 0x0f;

//   width /= 2;

   if(!((y*width+x) % 2))
   {
      data[idx] &= 0x0f;
      data[idx] |= (char)(value << 4);
   }
   else
   {
      data[idx] &= 0xf0;
      data[idx] |= (char)value;
   }
}


void smooth_sprite(char* data, int width, int length)
{
   int x;
   int y;
   char base_pixel;
   char tl_pixel;
   char t_pixel;
   char tr_pixel;
   char r_pixel;
   char br_pixel;
   char b_pixel;
   char bl_pixel;
   char l_pixel;

   width *= 2;
   length *= 2;

   for(y=0;y<length;y++)
      for(x=0;x<width;x++)
         if(x > 0 && x < width-1 && y > 0 && y < length-1)
         {
            base_pixel = get_pixel(y, x, width, data);
            tl_pixel = get_pixel(y+1, x-1, width, data);
            t_pixel  = get_pixel(y+1, x  , width, data);
            tr_pixel = get_pixel(y+1, x+1, width, data);
            r_pixel  = get_pixel(y  , x+1, width, data);
            br_pixel = get_pixel(y-1, x+1, width, data);
            b_pixel  = get_pixel(y-1, x  , width, data);
            bl_pixel = get_pixel(y-1, x-1, width, data);
            l_pixel  = get_pixel(y  , x-1, width, data);


            if(x > 1 && x < width-2 && y > 1 && y < length-2 &&
               (
                (   base_pixel == r_pixel
                 && base_pixel != l_pixel
                 && l_pixel    == tl_pixel
                 && l_pixel    == t_pixel
                 && l_pixel    == tr_pixel
                 && l_pixel    == get_pixel(y  , x+2, width, data)
                 && l_pixel    == get_pixel(y+1, x+2, width, data)
                 && l_pixel    == get_pixel(y-1, x+2, width, data)
                 && l_pixel    == bl_pixel
                 && l_pixel    == get_pixel(y-2, x+2, width, data)
                 && l_pixel    == get_pixel(y-2, x-1, width, data)
                ) ||
                (   base_pixel == l_pixel
                 && base_pixel != r_pixel
                 && r_pixel    == tr_pixel
                 && r_pixel    == t_pixel
                 && r_pixel    == tl_pixel
                 && r_pixel    == get_pixel(y  , x-2, width, data)
                 && r_pixel    == get_pixel(y+1, x-2, width, data)
                 && r_pixel    == get_pixel(y-1, x-2, width, data)
                 && r_pixel    == br_pixel
                 && r_pixel    == get_pixel(y-2, x-2, width, data)
                 && r_pixel    == get_pixel(y-2, x+1, width, data)
                ) ||
                (   base_pixel == r_pixel
                 && base_pixel != l_pixel
                 && l_pixel    == bl_pixel
                 && l_pixel    == b_pixel
                 && l_pixel    == br_pixel
                 && l_pixel    == get_pixel(y  , x+2, width, data)
                 && l_pixel    == get_pixel(y-1, x+2, width, data)
                 && l_pixel    == get_pixel(y+1, x+2, width, data)
                 && l_pixel    == tl_pixel
                 && l_pixel    == get_pixel(y+2, x+2, width, data)
                 && l_pixel    == get_pixel(y+2, x-1, width, data)
                ) ||
                (   base_pixel == l_pixel
                 && base_pixel != r_pixel
                 && r_pixel    == br_pixel
                 && r_pixel    == b_pixel
                 && r_pixel    == bl_pixel
                 && r_pixel    == get_pixel(y  , x-2, width, data)
                 && r_pixel    == get_pixel(y-1, x-2, width, data)
                 && r_pixel    == get_pixel(y+1, x-2, width, data)
                 && r_pixel    == tr_pixel
                 && r_pixel    == get_pixel(y+2, x-2, width, data)
                 && r_pixel    == get_pixel(y+2, x+1, width, data)
                )
               )
              )
               continue;

            if     (   base_pixel == t_pixel
                    && base_pixel == r_pixel

                    && bl_pixel == b_pixel
                    && bl_pixel == br_pixel
                    && bl_pixel == l_pixel
                    && bl_pixel == tl_pixel
                   )
                      set_pixel(y, x, width, data, bl_pixel);

            else if(   base_pixel == b_pixel
                    && base_pixel == r_pixel

                    && tl_pixel == t_pixel
                    && tl_pixel == tr_pixel
                    && tl_pixel == l_pixel
                    && tl_pixel == bl_pixel
                   )
                      set_pixel(y, x, width, data, tl_pixel);

            else if(   base_pixel == b_pixel
                    && base_pixel == l_pixel

                    && tr_pixel == t_pixel
                    && tr_pixel == tl_pixel
                    && tr_pixel == r_pixel
                    && tr_pixel == br_pixel
                    )
                       set_pixel(y, x, width, data, tr_pixel);

            else if(   base_pixel == b_pixel
                    && base_pixel == l_pixel

                    && br_pixel == b_pixel
                    && br_pixel == bl_pixel
                    && br_pixel == r_pixel
                    && br_pixel == tr_pixel
                   )
                      set_pixel(y, x, width, data, br_pixel);
         }
}

void doublesprite(char* data, int width, int length, BOOL smoothing)
{
   int i;
   int j;

   width/=2;

   for(i=length-1;i>=0;i--)
   {
      for(j=width-1;j>=0;j--)
      {
         data[i*2*width*2+j*2+1] =
         data[(i*2+1)*width*2+j*2+1] = (char)
         (((data[i*width+j] & 0x0f) |
         ((data[i*width+j] & 0x0f) * 16)) & 0xff);

         data[i*2*width*2+j*2] =
         data[(i*2+1)*width*2+j*2] = (char)
         (((data[i*width+j] & 0xf0) |
         ((data[i*width+j] & 0xf0) / 16)) & 0xff);
      }
   }

   if(smoothing)
      smooth_sprite(data, width*2, length);
}


HBITMAP make_mask(char* data, int x, int y)
{
   char maskdata[1024];
   int i;
   int j;
   int len = x*y/8;

   for(i=0;i<len;i++)
   {
      maskdata[i] = 0;
      j = i*4;
      if((data[j]   & 0xf0) == 0) maskdata[i] |= 0x80;
      if((data[j]   & 0x0f) == 0) maskdata[i] |= 0x40;
      if((data[j+1] & 0xf0) == 0) maskdata[i] |= 0x20;
      if((data[j+1] & 0x0f) == 0) maskdata[i] |= 0x10;
      if((data[j+2] & 0xf0) == 0) maskdata[i] |= 0x08;
      if((data[j+2] & 0x0f) == 0) maskdata[i] |= 0x04;
      if((data[j+3] & 0xf0) == 0) maskdata[i] |= 0x02;
      if((data[j+3] & 0x0f) == 0) maskdata[i] |= 0x01;
   }
   return CreateBitmap(x, y, 1, 1, maskdata);
}

