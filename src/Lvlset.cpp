#include "lvlset.h"
#include "util.h"

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
#define BLOCK_X_MIN        0
#define BLOCK_X_MAX        31
#define BLOCK_Y_MIN        0
#define BLOCK_Y_MAX        25

#define PATCH_LEVEL_SIZE 451
#define PATCH_VERSION 1

#define PROTECT_OFFSET (0x18c*8)
#define SCREEN_OFFSET (0xcfc*8)
#define SCREEN_LENGTH (32*24*3)
#define LEVEL_OFFSET (0x673a*8)
#define LEVEL_LENGTH (43*8)

#define NORMAL_PALETTE_OFFSET (0x00*0)
#define POINTS_Y_OFFSET       (0x01*8)
#define POINTS_X_OFFSET       (0x02*8)
#define TOP_OFFSET            (0x03*8)
#define BOTTOM_OFFSET         (0x03*8+4)
#define MONSTER_TYPE_OFFSET   (0x04*8)
#define BREAK_SPEED_OFFSET    (0x07*8)
#define MONSTER_SPEED_OFFSET  (0x08*8)
#define BUBBLE_SPEED_OFFSET   (0x09*8)
#define WEAPON_Y_OFFSET       (0x0a*8)
#define WEAPON_X_OFFSET       (0x0b*8)
#define ALIEN_FLAG_OFFSET     (0x0c*8)
#define TIMER_OFFSET          (0x0d*8)
#define SUPER_PALETTE_OFFSET  (0x0e*8)
#define BARON_TIME_OFFSET     (0x0f*8)
#define BARON_SPEED_OFFSET    (0x10*8)
#define MONSTER_DROP_OFFSET   (0x11*8)
#define MONSTER_X_OFFSET      (0x12*8)
#define MONSTER_Y_OFFSET      (0x13*8)
#define LIGHTNING_RATE_OFFSET (0x26*8)
#define WATER_RATE_OFFSET     (0x27*8)
#define FIRE_RATE_OFFSET      (0x28*8)
#define MONSTER_DIR_OFFSET    (0x2a*8)

#define MONSTER_TYPE_LENGTH   4
#define MONSTER_LENGTH        24
#define MONSTER_DIR_LENGTH    1

#define MONSTERX_OFFSET       MONSTER_X_OFFSET
#define MONSTERY_OFFSET       MONSTER_Y_OFFSET


int topbot_data[9][32] =
{
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int topbot_encode[16] = {10, 6, 2, 9, 8, 5, 4, 1, 0};
int topbot_decode[16] = {8, 7, 2, 0, 6, 5, 1, 0, 4, 3, 0};

int mnum_filter_super[8]  = {3, 1, 6, 0, 5, 4, 2, 3};

LvlSet::LvlSet(int nlevels)
: m_info(new struct levelinfo[nlevels])
, m_orig(new struct levelinfo[nlevels])
, m_nlevels(nlevels)
, m_super(FALSE)
{
   memset(m_author, 0, 21);
   memset(m_desc, 0, 31);
   clear_all();
   make_default();
   set_level(1);
}

BOOL LvlSet::set_level(int newlevel)
{
   if(newlevel < LEVEL_MIN || newlevel > m_nlevels || m_level == newlevel-1)
      return FALSE;
   m_level = newlevel-1;
   calc_screen();
   return TRUE;
}

void LvlSet::optimize_pattern()
{
   int i;
   int min_drop = MONSTER_DROP_MAX;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      if(m_info[m_level].monster_drop[i] < min_drop)
         min_drop = m_info[m_level].monster_drop[i];

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] -= min_drop;
}

void LvlSet::drop_pattern_wipe()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = MONSTER_DROP_MIN;
}

void LvlSet::drop_pattern_slide()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = win2rom_y(m_info[m_level].monster_y[i]);
   optimize_pattern();
}

void LvlSet::drop_pattern_roll()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = MONSTER_DROP_MAX - win2rom_y(m_info[m_level].monster_y[i]);
   optimize_pattern();
}

void LvlSet::drop_pattern_stretch()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = win2rom_y(m_info[m_level].monster_y[i])*2;
   optimize_pattern();
}

void LvlSet::drop_pattern_shrink()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = win2rom_y(m_info[m_level].monster_y[i])/2;
   optimize_pattern();
}

#define MDROP_ANGLE_LEFT(Y, X) (Y + X/2)
#define MDROP_ANGLE_RIGHT(Y, X) (Y + (30*8-X)/2)

void LvlSet::drop_pattern_angle_left()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = MDROP_ANGLE_LEFT(win2rom_y(m_info[m_level].monster_y[i]), win2rom_x(m_info[m_level].monster_x[i]));
   optimize_pattern();
}

void LvlSet::drop_pattern_angle_right()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      m_info[m_level].monster_drop[i] = MDROP_ANGLE_RIGHT(win2rom_y(m_info[m_level].monster_y[i]), win2rom_x(m_info[m_level].monster_x[i]));
   optimize_pattern();
}

void LvlSet::drop_pattern_angle_inside()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      if(win2rom_x(m_info[m_level].monster_x[i]) < 15*8)
         m_info[m_level].monster_drop[i] = MDROP_ANGLE_RIGHT(win2rom_y(m_info[m_level].monster_y[i]), win2rom_x(m_info[m_level].monster_x[i]));
      else
         m_info[m_level].monster_drop[i] = MDROP_ANGLE_LEFT(win2rom_y(m_info[m_level].monster_y[i]), win2rom_x(m_info[m_level].monster_x[i]));
   optimize_pattern();
}

void LvlSet::drop_pattern_angle_outside()
{
   int i;

   for(i= MONSTER_NUM_MIN;i < m_info[m_level].nmonsters;i++)
      if(win2rom_x(m_info[m_level].monster_x[i]) < 15*8)
         m_info[m_level].monster_drop[i] = MDROP_ANGLE_LEFT(win2rom_y(m_info[m_level].monster_y[i]), win2rom_x(m_info[m_level].monster_x[i]));
      else
         m_info[m_level].monster_drop[i] = MDROP_ANGLE_RIGHT(win2rom_y(m_info[m_level].monster_y[i]), win2rom_x(m_info[m_level].monster_x[i]));
   optimize_pattern();
}


BOOL LvlSet::set_normal_palette(int val)
{
   if(val < PALETTE_MIN || val > PALETTE_MAX || m_info[m_level].normal_palette == val)
      return FALSE;
   m_info[m_level].normal_palette = val;
   return TRUE;
}

BOOL LvlSet::set_super_palette(int val)
{
   if(val < PALETTE_MIN || val > PALETTE_MAX || m_info[m_level].super_palette == (val-3)%8)
      return FALSE;
   m_info[m_level].super_palette = (val-3)%8;
   return TRUE;
}

BOOL LvlSet::set_top(int val)
{
   if(val < TOPBOT_MIN || val > TOPBOT_MAX || m_info[m_level].top_type == val)
      return FALSE;
   m_info[m_level].top_type = val;
   decode_top();
   calc_top();
   return TRUE;
}

BOOL LvlSet::set_bottom(int val)
{
   if(val < TOPBOT_MIN || val > TOPBOT_MAX || m_info[m_level].bottom_type == val)
      return FALSE;
   m_info[m_level].bottom_type = val;
   decode_bottom();
   calc_bottom();
   return TRUE;
}

BOOL LvlSet::set_break_speed(int val)
{
   if(val < BREAK_SPEED_MIN || val > BREAK_SPEED_MAX || m_info[m_level].break_speed == val)
      return FALSE;
   m_info[m_level].break_speed = val;
   return TRUE;
}

BOOL LvlSet::set_monster_speed(int val)
{
   if(val < MONSTER_SPEED_MIN || val > MONSTER_SPEED_MAX || m_info[m_level].monster_speed == val)
      return FALSE;
   m_info[m_level].monster_speed = val;
   return TRUE;
}

BOOL LvlSet::set_bubble_speed(int val)
{
   if(val < BUBBLE_SPEED_MIN || val > BUBBLE_SPEED_MAX || m_info[m_level].bubble_speed == val)
      return FALSE;
   m_info[m_level].bubble_speed = val;
   return TRUE;
}

BOOL LvlSet::set_timer(int val)
{
   if(val < TIMER_MIN || val > TIMER_MAX || m_info[m_level].timer == val)
      return FALSE;
   m_info[m_level].timer = val;
   return TRUE;
}

BOOL LvlSet::set_baron_time(int val)
{
   if(val < BARON_TIME_MIN || val > BARON_TIME_MAX || m_info[m_level].baron_time == val)
      return FALSE;
   m_info[m_level].baron_time = val;
   return TRUE;
}

BOOL LvlSet::set_baron_speed(int val)
{
   if(val < BARON_SPEED_MIN || val > BARON_SPEED_MAX || m_info[m_level].baron_speed == val)
      return FALSE;
   m_info[m_level].baron_speed = val;
   return TRUE;
}

BOOL LvlSet::set_lightning_rate(int val)
{
   if(val < BUBBLE_RATE_MIN)
      return FALSE;

   if(val + m_info[m_level].water_rate + m_info[m_level].fire_rate > BUBBLE_RATE_MAX)
      val = BUBBLE_RATE_MAX - m_info[m_level].water_rate - m_info[m_level].fire_rate;

   if(m_info[m_level].lightning_rate == val)
      return FALSE;

   m_info[m_level].lightning_rate = val;
   return TRUE;
}

BOOL LvlSet::set_water_rate(int val)
{
   if(val < BUBBLE_RATE_MIN)
      return FALSE;

   if(val + m_info[m_level].lightning_rate + m_info[m_level].fire_rate > BUBBLE_RATE_MAX)
      val = BUBBLE_RATE_MAX - m_info[m_level].lightning_rate - m_info[m_level].fire_rate;

   if(m_info[m_level].water_rate == val)
      return FALSE;

   m_info[m_level].water_rate = val;
   return TRUE;
}

BOOL LvlSet::set_fire_rate(int val)
{
   if(val < BUBBLE_RATE_MIN)
      return FALSE;

   if(val + m_info[m_level].water_rate + m_info[m_level].lightning_rate > BUBBLE_RATE_MAX)
      val = BUBBLE_RATE_MAX - m_info[m_level].water_rate - m_info[m_level].lightning_rate;

   if(m_info[m_level].fire_rate == val)
      return FALSE;

   m_info[m_level].fire_rate = val;
   return TRUE;
}

BOOL LvlSet::set_weapon_x(int val)
{
   if(val < X32_MIN)
      val = X32_MIN;
   else if(val > X32_MAX)
      val = X32_MAX;
   if(m_info[m_level].weapon_x == val)
      return FALSE;

   m_info[m_level].weapon_x = val;
   return TRUE;
}

BOOL LvlSet::set_weapon_y(int val)
{
   if(val < Y32_MIN)
      val = Y32_MIN;
   else if(val > Y32_MAX)
      val = Y32_MAX;
   if(m_info[m_level].weapon_y == val)
      return FALSE;

   m_info[m_level].weapon_y = val;
   return TRUE;
}

BOOL LvlSet::set_points_x(int val)
{
   if(val < X32_MIN)
      val = X32_MIN;
   else if(val > X32_MAX)
      val = X32_MAX;
   if(m_info[m_level].points_x == val)
      return FALSE;

   m_info[m_level].points_x = val;
   return TRUE;
}

BOOL LvlSet::set_points_y(int val)
{
   if(val < Y32_MIN)
      val = Y32_MIN;
   else if(val > Y32_MAX)
      val = Y32_MAX;
   if(m_info[m_level].points_y == val)
      return FALSE;

   m_info[m_level].points_y = val;
   return TRUE;
}


BOOL LvlSet::set_monster_drop(int mnum, int val)
{
   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX || m_info[m_level].monster_drop[mnum] == val)
      return FALSE;

   if(val < MONSTER_DROP_MIN)
      val = MONSTER_DROP_MIN;
   else if(val > MONSTER_DROP_MAX)
      val = MONSTER_DROP_MAX;

   m_info[m_level].monster_drop[mnum] = val;
   return TRUE;
}

BOOL LvlSet::set_monster_x(int mnum, int val)
{
   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX)
      return FALSE;

   if(val < X32_MIN)
      val = X32_MIN;
   else if(val > X32_MAX)
      val = X32_MAX;
   if(m_info[m_level].monster_x[mnum] == val)
      return FALSE;

   m_info[m_level].monster_x[mnum] = val;
   return TRUE;
}

BOOL LvlSet::set_monster_y(int mnum, int val)
{
   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX)
      return FALSE;

   if(val < Y32_MIN)
      val = Y32_MIN;
   else if(val > Y32_MAX)
      val = Y32_MAX;
   if(m_info[m_level].monster_y[mnum] == val)
      return FALSE;

   m_info[m_level].monster_y[mnum] = val;
   return TRUE;
}

BOOL LvlSet::set_monster_dir(int mnum, int val)
{
   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX || val < MONSTER_DIR_MIN || val > MONSTER_DIR_MAX || m_info[m_level].monster_dir[mnum] == val)
      return FALSE;
   m_info[m_level].monster_dir[mnum] = val;
   return TRUE;
}

BOOL LvlSet::set_monster_type(int mnum, int val)
{
   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX || val < MONSTER_TYPE_MIN || val > MONSTER_TYPE_MAX || m_info[m_level].monster_type[mnum] == val)
      return FALSE;
   m_info[m_level].monster_type[mnum] = val;
   return TRUE;
}

int  LvlSet::get_disp_monster_type(int mnum)
{
   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX)
      return -1;

   if(!m_super && m_info[m_level].alien_flag && m_info[m_level].monster_type[mnum] == 0)
      return 7;
   if(m_super)
      return mnum_filter_super[m_info[m_level].monster_type[mnum]];
   else
      return m_info[m_level].monster_type[mnum];
}


int  LvlSet::insert_monster(int type, int x, int y, int drop, int dir)
{
   int mnum = m_info[m_level].nmonsters;

   if(m_info[m_level].nmonsters >= 7)
      return -1;

   m_info[m_level].nmonsters++;

   m_info[m_level].monster_drop[mnum] = MONSTER_DROP_MIN;
   m_info[m_level].monster_x[mnum]    = X32_MIN;
   m_info[m_level].monster_y[mnum]    = Y32_MIN;
   m_info[m_level].monster_dir[mnum]  = MONSTER_DIR_MIN;
   m_info[m_level].monster_type[mnum] = MONSTER_TYPE_MIN;

   set_monster_drop(mnum, drop);
   set_monster_x(mnum, x);
   set_monster_y(mnum, y);
   set_monster_dir(mnum, dir);
   set_monster_type(mnum, type);

   return mnum;
}

BOOL LvlSet::delete_monster(int mnum)
{
   int i;

   if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX || m_info[m_level].nmonsters <= 1)
      return FALSE;

   for(i=mnum;i<6;i++)
      copy_monster(m_level, i+1, i);

   m_info[m_level].monster_drop[i] =
   m_info[m_level].monster_x[i]    =
   m_info[m_level].monster_y[i]    =
   m_info[m_level].monster_dir[i]  =
   m_info[m_level].monster_type[i] = 0;

   m_info[m_level].nmonsters--;

   return TRUE;
}

BOOL LvlSet::data_valid(int start_level, int end_level)
{
   int i;
   int x;
   int y;

   if(start_level < LEVEL_MIN || start_level > LEVEL_MAX ||
      end_level   < LEVEL_MIN || end_level   > LEVEL_MAX ||
      start_level > end_level
     )
      return FALSE;

   start_level--;
   end_level--;

   for(i=start_level;i<=end_level;i++)
   {
      if(
         m_info[i].normal_palette < PALETTE_MIN       || m_info[i].normal_palette > PALETTE_MAX       ||
         m_info[i].super_palette  < PALETTE_MIN       || m_info[i].super_palette  > PALETTE_MAX       ||
//         _info[i].points_x       < X32_MIN           || _info[i].points_x       > X32_MAX           ||
//         _info[i].points_y       < Y32_MIN           || _info[i].points_y       > Y32_MAX           ||
//         _info[i].weapon_x       < X32_MIN           || _info[i].weapon_x       > X32_MAX           ||
//         _info[i].weapon_y       < Y32_MIN           || _info[i].weapon_y       > Y32_MAX           ||
         m_info[i].top_type       < TOPBOT_MIN        || m_info[i].top_type       > 0x0a              ||
         m_info[i].bottom_type    < TOPBOT_MIN        || m_info[i].bottom_type    > 0x0a              ||
         m_info[i].monster_speed  < MONSTER_SPEED_MIN || m_info[i].monster_speed  > MONSTER_SPEED_MAX ||
         m_info[i].bubble_speed   < BUBBLE_SPEED_MIN  || m_info[i].bubble_speed   > BUBBLE_SPEED_MAX  ||
         m_info[i].break_speed    < BREAK_SPEED_MIN   || m_info[i].break_speed    > BREAK_SPEED_MAX   ||
//         _info[i].alien_flag     < 0                 || _info[i].alien_flag     > 1                 ||
//         _info[i].timer          < TIMER_MIN         || _info[i].timer          > TIMER_MAX         ||
         m_info[i].baron_time     < BARON_TIME_MIN    || m_info[i].baron_speed    > BARON_SPEED_MAX   ||
         m_info[i].lightning_rate < BUBBLE_RATE_MIN   || m_info[i].lightning_rate > BUBBLE_RATE_MAX   ||
         m_info[i].water_rate     < BUBBLE_RATE_MIN   || m_info[i].water_rate     > BUBBLE_RATE_MAX   ||
         m_info[i].fire_rate      < BUBBLE_RATE_MIN   || m_info[i].fire_rate      > BUBBLE_RATE_MAX   ||
         m_info[i].nmonsters      < 1                 || m_info[i].nmonsters      > 7
        )
         return FALSE;

      for(x=0;x<6;x++)
      {
         if(
//            _info[i].monster_drop[x] < MONSTER_DROP_MIN+1 || _info[i].monster_drop[x] > MONSTER_DROP_MAX+1 ||
//            _info[i].monster_x[x]    < X32_MIN          || _info[i].monster_x[x]    > X32_MAX          ||
//            _info[i].monster_y[x]    < Y32_MIN          || _info[i].monster_y[x]    > Y32_MAX          ||
            m_info[i].monster_type[x] < MONSTER_TYPE_MIN || m_info[i].monster_type[x] > MONSTER_TYPE_MAX
           )
            return FALSE;
      }
      for(y=1;y<25;y++)
         for(x=0;x<32;x++)
            if(m_info[i].arrow_data[y][x] < 0 || m_info[i].arrow_data[y][x] > 4)
               return FALSE;
   }
   return TRUE;
}

BOOL LvlSet::load_roms(char* srom_name, char* lrom1_name, char* lrom2_name, char* lrom3_name, char* lrom4_name, int start_level, int end_level)
{
   BitBuff sdata(3);
   BitBuff ldata(8);
   int i;
   int x;
   int y;
   int last;
   int loffset = LEVEL_OFFSET;
   int soffset = SCREEN_OFFSET;

   if(end_level <= 0)
      end_level = m_nlevels;

   if(start_level < LEVEL_MIN || start_level > m_nlevels ||
      end_level   < LEVEL_MIN || end_level   > m_nlevels ||
      start_level > end_level
     )
      return FALSE;

   if(!sdata.load(srom_name))
      return FALSE;
   if(!ldata.load(lrom1_name))
      if(!ldata.load(lrom2_name))
         if(!ldata.load(lrom3_name))
            if(!ldata.load(lrom4_name))
               return FALSE;

   start_level--;
   end_level--;

   for(i=start_level;i<=end_level;i++)
   {
      m_info[i].top_type       = topbot_decode[ldata.get(loffset+TOP_OFFSET, 4)];
      m_info[i].bottom_type    = topbot_decode[ldata.get(loffset+BOTTOM_OFFSET, 4)];
      m_info[i].lightning_rate = ldata.get(loffset+LIGHTNING_RATE_OFFSET, 8);
      m_info[i].water_rate     = ldata.get(loffset+WATER_RATE_OFFSET, 8);
      m_info[i].fire_rate      = ldata.get(loffset+FIRE_RATE_OFFSET, 8);
      m_info[i].normal_palette = ldata.get(loffset+NORMAL_PALETTE_OFFSET, 8);
      m_info[i].super_palette  = ldata.get(loffset+SUPER_PALETTE_OFFSET, 8);
      m_info[i].points_x       = rom2win_x(ldata.get(loffset+POINTS_X_OFFSET, 8));
      m_info[i].points_y       = rom2win_y(ldata.get(loffset+POINTS_Y_OFFSET, 8));
      m_info[i].weapon_x       = rom2win_x(ldata.get(loffset+WEAPON_X_OFFSET, 8));
      m_info[i].weapon_y       = rom2win_y(ldata.get(loffset+WEAPON_Y_OFFSET, 8));
      m_info[i].monster_speed  = ldata.get(loffset+MONSTER_SPEED_OFFSET, 8);
      m_info[i].bubble_speed   = ldata.get(loffset+BUBBLE_SPEED_OFFSET, 8);
      m_info[i].break_speed    = ldata.get(loffset+BREAK_SPEED_OFFSET, 8);
      m_info[i].timer          = ldata.get(loffset+TIMER_OFFSET, 8);
      m_info[i].baron_time     = ldata.get(loffset+BARON_TIME_OFFSET, 8);
      m_info[i].baron_speed    = ldata.get(loffset+BARON_SPEED_OFFSET, 8);
      m_info[i].alien_flag     = ldata.get(loffset+ALIEN_FLAG_OFFSET, 8);

      m_info[i].nmonsters = 0;
      last = 0;
      for(x=0;x<6;x++)
      {
         m_info[i].nmonsters += ldata.get(loffset+MONSTER_TYPE_OFFSET+(4*x), 4);
         for(y=last;y<m_info[i].nmonsters;y++)
            m_info[i].monster_type[y] = x;
         last = m_info[i].nmonsters;
      }

      for(x=0;x<m_info[i].nmonsters;x++)
      {
         m_info[i].monster_dir[x]  = ldata.get(loffset+MONSTER_DIR_OFFSET+(7-x), 1);
         m_info[i].monster_x[x]    = rom2win_x(ldata.get(loffset+MONSTER_X_OFFSET+(3*8*x), 8));
         m_info[i].monster_y[x]    = rom2win_y(ldata.get(loffset+MONSTER_Y_OFFSET+(3*8*x), 8));
         m_info[i].monster_drop[x] = ldata.get(loffset+MONSTER_DROP_OFFSET+(3*8*x), 8)-1;
      }

// get monster types

      for(;x<7;x++)
      {
         m_info[i].monster_dir[x]  = 1;
         m_info[i].monster_x[x]    = 0;
         m_info[i].monster_y[x]    = 0;
         m_info[i].monster_drop[x] = 0;
         m_info[i].monster_type[x] = 0;
      }

      for(y=1;y<25;y++)
         for(x=0;x<32;x++)
         {
            m_info[i].arrow_data[y][x] = sdata.get(soffset, 3);
            soffset += 3;
         }
      loffset += LEVEL_LENGTH;
   }
   if(m_level >= start_level && m_level <= end_level)
      calc_screen();

   if(!data_valid(start_level+1, end_level+1))
   {
      revert_all();
      return FALSE;
   }
   return TRUE;
}

BOOL LvlSet::save_roms(char* srom_name, char* lrom1_name, char* lrom2_name, char* lrom3_name, char* lrom4_name, int start_level, int end_level)
{
   BitBuff sdata(3);
   BitBuff ldata(8);
   int i;
   int j;
   int x;
   int y;
   int mtypes[6];
   int loffset = LEVEL_OFFSET;
   int soffset = SCREEN_OFFSET;
   char* lroms[4] = {lrom1_name, lrom2_name, lrom3_name, lrom4_name};
   BOOL lroms_saved = FALSE;

   if(end_level <= 0)
      end_level = m_nlevels;

   if(start_level < LEVEL_MIN || start_level > m_nlevels ||
      end_level   < LEVEL_MIN || end_level   > m_nlevels ||
      start_level > end_level
     )
      return FALSE;

   if(!sdata.load(srom_name))
      return FALSE;

   start_level--;
   end_level--;

   for(i=start_level;i<=end_level;i++)
   {
      for(y=1;y<25;y++)
      {
         for(x=0;x<32;x++)
         {
            sdata.put(soffset, m_info[i].arrow_data[y][x], 3);
            soffset += 3;
         }
      }
   }
   sdata.put(PROTECT_OFFSET, 0xc9, 8);
   if(!sdata.save(srom_name))
      return FALSE;


   for(j=0;j<4;j++)
   {
      if(!ldata.load(lroms[j]))
         continue;
      for(i=start_level;i<=end_level;i++)
      {
         if(i != 99 && i != 101)
         {
            ldata.put(loffset+TOP_OFFSET, topbot_encode[m_info[i].top_type], 4);
            ldata.put(loffset+BOTTOM_OFFSET, topbot_encode[m_info[i].bottom_type], 4);
            ldata.put(loffset+LIGHTNING_RATE_OFFSET, m_info[i].lightning_rate, 8);
            ldata.put(loffset+WATER_RATE_OFFSET, m_info[i].water_rate, 8);
            ldata.put(loffset+FIRE_RATE_OFFSET, m_info[i].fire_rate, 8);
            ldata.put(loffset+NORMAL_PALETTE_OFFSET, m_info[i].normal_palette, 8);
            ldata.put(loffset+SUPER_PALETTE_OFFSET, m_info[i].super_palette, 8);
            ldata.put(loffset+POINTS_X_OFFSET, win2rom_x(m_info[i].points_x), 8);
            ldata.put(loffset+POINTS_Y_OFFSET, win2rom_y(m_info[i].points_y), 8);
            ldata.put(loffset+WEAPON_X_OFFSET, win2rom_x(m_info[i].weapon_x), 8);
            ldata.put(loffset+WEAPON_Y_OFFSET, win2rom_y(m_info[i].weapon_y), 8);
            ldata.put(loffset+MONSTER_SPEED_OFFSET, m_info[i].monster_speed, 8);
            ldata.put(loffset+BUBBLE_SPEED_OFFSET, m_info[i].bubble_speed, 8);
            ldata.put(loffset+BREAK_SPEED_OFFSET, m_info[i].break_speed, 8);
            ldata.put(loffset+TIMER_OFFSET, m_info[i].timer, 8);
            ldata.put(loffset+BARON_TIME_OFFSET, m_info[i].baron_time, 8);
            ldata.put(loffset+BARON_SPEED_OFFSET, m_info[i].baron_speed, 8);
            ldata.put(loffset+ALIEN_FLAG_OFFSET, m_info[i].alien_flag, 8);
      
            sort_monsters(i);
            mtypes[0] = mtypes[1] = mtypes[2] = mtypes[3] = mtypes[4] = mtypes[5] = 0;
            for(x=0;x<m_info[i].nmonsters;x++)
            {
               mtypes[m_info[i].monster_type[x]]++;
      
               ldata.put(loffset+MONSTER_DIR_OFFSET+(7-x), m_info[i].monster_dir[x], 1);
               ldata.put(loffset+MONSTER_X_OFFSET+(3*8*x), win2rom_x(m_info[i].monster_x[x]), 8);
               ldata.put(loffset+MONSTER_Y_OFFSET+(3*8*x), win2rom_y(m_info[i].monster_y[x]), 8);
               ldata.put(loffset+MONSTER_DROP_OFFSET+(3*8*x), m_info[i].monster_drop[x]+1, 8);
            }
      
            for(;x<7;x++)
            {
               ldata.put(loffset+MONSTER_DIR_OFFSET+(7-x), 1, 1);
               ldata.put(loffset+MONSTER_X_OFFSET+(3*8*x), 0, 8);
               ldata.put(loffset+MONSTER_Y_OFFSET+(3*8*x), 0, 8);
               ldata.put(loffset+MONSTER_DROP_OFFSET+(3*8*x), 0, 8);
            }
      
            for(x=0;x<6;x++)
               ldata.put(loffset+MONSTER_TYPE_OFFSET+(4*x), mtypes[x], 4);
         }
         loffset += LEVEL_LENGTH;
      }
      if(ldata.save(lroms[j]))
         lroms_saved = TRUE;
   }
   return lroms_saved;
}


void LvlSet::sort_monsters(int level)
{
   int dirty = 1;
   int i;

   while(dirty)
   {
      dirty = 0;
      for(i=0;i<m_info[level].nmonsters-1;i++)
         if(m_info[level].monster_type[i] > m_info[level].monster_type[i+1])
         {
            swap_monsters(level, i, i+1);
            dirty = 1;
         }
   }
}

void LvlSet::swap_monsters(int level, int a, int b)
{
   int swap;

   swap = m_info[level].monster_type[a];
   m_info[level].monster_type[a] = m_info[level].monster_type[b];
   m_info[level].monster_type[b] = swap;

   swap = m_info[level].monster_dir[a];
   m_info[level].monster_dir[a] = m_info[level].monster_dir[b];
   m_info[level].monster_dir[b] = swap;

   swap = m_info[level].monster_drop[a];
   m_info[level].monster_drop[a] = m_info[level].monster_drop[b];
   m_info[level].monster_drop[b] = swap;

   swap = m_info[level].monster_x[a];
   m_info[level].monster_x[a] = m_info[level].monster_x[b];
   m_info[level].monster_x[b] = swap;

   swap = m_info[level].monster_y[a];
   m_info[level].monster_y[a] = m_info[level].monster_y[b];
   m_info[level].monster_y[b] = swap;
}

void LvlSet::copy_monster(int level, int src, int dst)
{
   m_info[level].monster_type[dst] = m_info[level].monster_type[src];
   m_info[level].monster_dir[dst]  = m_info[level].monster_dir[src];
   m_info[level].monster_drop[dst] = m_info[level].monster_drop[src];
   m_info[level].monster_x[dst]    = m_info[level].monster_x[src];
   m_info[level].monster_y[dst]    = m_info[level].monster_y[src];
}

int LvlSet::get_arrow(int x, int y)
{
   if(x < BLOCK_X_MIN || x > BLOCK_X_MAX || y < BLOCK_Y_MIN || y > BLOCK_Y_MAX)
      return -1;
   return m_info[m_level].arrow_data[y][x];
}

int LvlSet::get_block(int x, int y)
{
   if(x < BLOCK_X_MIN || x > BLOCK_X_MAX || y < BLOCK_Y_MIN || y > BLOCK_Y_MAX)
      return -1;
   return m_block_data[y][x];
}

void LvlSet::decode_top()
{
   int x;

   for(x=BLOCK_X_MIN;x<= BLOCK_X_MAX;x++)
      m_info[m_level].arrow_data[BLOCK_Y_MIN][x] = topbot_data[m_info[m_level].top_type][x];
}

void LvlSet::calc_top()
{
   int x;

   for(x=BLOCK_X_MIN;x<= BLOCK_X_MAX;x++)
   {
      calc_block(x, BLOCK_Y_MIN);
      calc_block(x, BLOCK_Y_MIN+1);
   }
}

void LvlSet::decode_bottom()
{
   int x;

   for(x=BLOCK_X_MIN;x<= BLOCK_X_MAX;x++)
      m_info[m_level].arrow_data[BLOCK_Y_MAX][x] = topbot_data[m_info[m_level].bottom_type][x];
}

void LvlSet::calc_bottom()
{
   int x;

   for(x=BLOCK_X_MIN;x<= BLOCK_X_MAX;x++)
      calc_block(x, BLOCK_Y_MAX);
}

void LvlSet::calc_block(int x, int y)
{
   if(x < BLOCK_X_MIN || x > BLOCK_X_MAX || y < BLOCK_Y_MIN || y > BLOCK_Y_MAX)
      return;

   if(m_info[m_level].arrow_data[y][x] == 0) // solid
   {
      if(x < 2 || x >= 30)
         m_block_data[y][x] = T_LARGE1 + x%2 + (y%2)*2; // large block
      else
         m_block_data[y][x] = T_SOLID; // small block
      return;
   }

   if(y == 0)
   {
      if(x == 0)
         m_block_data[0][0] = T_BLANK; // default value
      else
      {
         if(m_info[m_level].arrow_data[0][x-1] == 0)
            m_block_data[0][x] = T_LEFT_BEGIN; // left_begin
         else
            m_block_data[0][x] = T_BLANK; // default value
      }
   }
   else
   {
      if(x == 0)
      {
         if(m_info[m_level].arrow_data[y-1][0] == 0)
            m_block_data[y][0] = T_BOTTOM; // bottom
         else
            m_block_data[y][0] = T_BLANK; // default value
      }
      else
      {
         if(m_info[m_level].arrow_data[y][x-1] != 0 && m_info[m_level].arrow_data[y-1][x] != 0 && m_info[m_level].arrow_data[y-1][x-1] != 0)
            m_block_data[y][x] = T_BLANK; // default value
         else
         {
            if(m_info[m_level].arrow_data[y][x-1] == 0)
            {
               if(m_info[m_level].arrow_data[y-1][x] == 0)
                  m_block_data[y][x] = T_BOTTOM_LEFT; // bottom_left
               else
               {
                  if(m_info[m_level].arrow_data[y-1][x-1] == 0)
                     m_block_data[y][x] = T_LEFT; // left
                  else
                     m_block_data[y][x] = T_LEFT_BEGIN; // left_begin
               }
            }
            else
            {
               if(m_info[m_level].arrow_data[y-1][x] != 0)
                  m_block_data[y][x] = T_BOTTOM_RIGHT; // bottom_right
               else
               {
                  if(m_info[m_level].arrow_data[y-1][x-1] == 0)
                     m_block_data[y][x] = T_BOTTOM; // bottom
                  else
                     m_block_data[y][x] = T_BOTTOM_BEGIN; // bottom_begin
               }
            }
         }
      }
   }
}

void LvlSet::calc_screen()
{
   decode_top();
   decode_bottom();
   for(int y=0;y<26;y++)
      for(int x=0;x<32;x++)
         calc_block(x, y);
}

BOOL LvlSet::change_block(int x, int y, int val)
{
   if(x < 0 || x > 31 || y < 1 || y > 24)
      return FALSE;
   if(m_info[m_level].arrow_data[y][x] == val || val < 0 || val > 4 || m_info[m_level].arrow_data[y][x] == val)
      return FALSE;

   m_info[m_level].arrow_data[y][x] = val;
   calc_block(x, y);
   calc_block(x+1, y);
   calc_block(x, y+1);
   calc_block(x+1, y+1);

   return TRUE;
}

BOOL LvlSet::same_as(LvlSet* dbuff, int level)
{
   level--;
   if(level < 0 || level > m_nlevels || level > dbuff->m_nlevels)
      return FALSE;
   return same_as(&(m_info[level]), &(dbuff->m_info[level]));
}

BOOL LvlSet::is_default(int level)
{
   if(level < LEVEL_MIN || level > LEVEL_MAX)
      return FALSE;

   level--;

   return same_as(&(m_info[level]), &(m_orig[level]));
}

BOOL LvlSet::same_as(levelinfo* one, levelinfo* two)
{
   int x;
   int y;

   for(y=1;y<25;y++)
      for(x=0;x<32;x++)
         if(one->arrow_data[y][x] != two->arrow_data[y][x])
            return FALSE;
   for(x=0;x<one->nmonsters;x++)
         if(!(
            (one->monster_x[x]    == two->monster_x[x]) &&
            (one->monster_y[x]    == two->monster_y[x]) &&
            (one->monster_drop[x] == two->monster_drop[x]) &&
            (one->monster_type[x] == two->monster_type[x]) &&
            (one->monster_dir[x]  == two->monster_dir[x])
           ))
            return FALSE;
   return (one->normal_palette == two->normal_palette) &&
          (one->super_palette  == two->super_palette) &&
          (one->points_x       == two->points_x) &&
          (one->points_y       == two->points_y) &&
          (one->weapon_x       == two->weapon_x) &&
          (one->weapon_y       == two->weapon_y) &&
          (one->top_type       == two->top_type) &&
          (one->bottom_type    == two->bottom_type) &&
          (one->bubble_speed   == two->bubble_speed) &&
          (one->break_speed    == two->break_speed) &&
          (one->alien_flag     == two->alien_flag) &&
          (one->timer          == two->timer) &&
          (one->baron_time     == two->baron_time) &&
          (one->lightning_rate == two->lightning_rate) &&
          (one->water_rate     == two->water_rate) &&
          (one->fire_rate      == two->fire_rate) &&
          (one->nmonsters      == two->nmonsters);
}

BOOL LvlSet::copy(int mylevel, LvlSet* dest, int destlevel)
{
   mylevel--;
   destlevel--;
   if(mylevel < 0 || mylevel > m_nlevels || destlevel < 0 || destlevel > dest->m_nlevels)
      return FALSE;

   memcpy(&(dest->m_info[destlevel]), &(m_info[mylevel]), sizeof(m_info[mylevel]));

   if(dest->m_level == destlevel)
      dest->calc_screen();

   return TRUE;
}

BOOL LvlSet::xchg(int mylevel, LvlSet* them, int theirlevel)
{
   char* buff;
   int size = sizeof(m_info[mylevel]);

   mylevel--;
   theirlevel--;
   if(mylevel < 0 || mylevel > m_nlevels || theirlevel < 0 || theirlevel > them->m_nlevels)
      return FALSE;

   buff = new char[size];
   memcpy(buff, &(m_info[mylevel]), size);
   memcpy(&(m_info[mylevel]), &(them->m_info[theirlevel]), size);
   memcpy(&(them->m_info[theirlevel]), buff, size);
   if(them->m_level == theirlevel)
      them->calc_screen();

   if(m_level == mylevel)
      calc_screen();

   delete [] buff;
   return TRUE;
}

BOOL LvlSet::clear(int level)
{
   int x;
   int y;

   level--;
   if(level < 0 || level > m_nlevels)
      return FALSE;

   memset(&(m_info[level]), 0, sizeof(m_info[level]));

   m_info[level].nmonsters       = 1;
   m_info[level].monster_drop[0] = MONSTER_DROP_MIN;
   m_info[level].monster_dir[0]  = 1;
   m_info[level].monster_x[0]    = X32_MIN;
   m_info[level].monster_y[0]    = Y32_MIN;
   m_info[level].points_x        = X32_MIN;
   m_info[level].points_y        = Y32_MIN;
   m_info[level].weapon_x        = X32_MIN;
   m_info[level].weapon_y        = Y32_MIN;
   m_info[level].bubble_speed    = 4;
   m_info[level].monster_speed   = 10;
   m_info[level].break_speed     = 30;
   m_info[level].timer           = 30;
   m_info[level].baron_time      = 30;
   m_info[level].baron_speed     = 1;

   for(y=1;y<25;y++)
   {
      m_info[level].arrow_data[y][0]  =
      m_info[level].arrow_data[y][1]  =
      m_info[level].arrow_data[y][30] =
      m_info[level].arrow_data[y][31] = 0;
      for(x=2;x<30;x++)
         m_info[level].arrow_data[y][x] = 1;
   }
   if(level == m_level)
      calc_screen();

   return TRUE;
}

void LvlSet::clear_all()
{
   int i;

   for(i=1;i<=m_nlevels;i++)
      clear(i);
}

BOOL LvlSet::revert(int level)
{
   level--;
   if(level < 0 || level > m_nlevels)
      return FALSE;

   memcpy(&(m_info[m_level]), &(m_orig[m_level]), sizeof(m_info[m_level]));
   if(level == m_level)
      calc_screen();
   return TRUE;
}

BOOL LvlSet::save_patch(char* filename)
{
   HANDLE fd;
   DWORD len;
   unsigned char buff[PATCH_LEVEL_SIZE];
   int i;
   int j;
   int x;
   int y;

   if(filename == NULL || filename[0] == '\0')
      return FALSE;

   if( (fd=CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
      return FALSE;

   memcpy(buff, "Patch-A-Bobble", 14);
   buff[14] = (unsigned char) PATCH_VERSION;
   memcpy(buff+15, m_author, 20);
   memcpy(buff+35, m_desc, 30);
   if(!WriteFile(fd, buff, 65, &len, NULL))
   {
      CloseHandle(fd);
      return FALSE;
   }
   for(i=0;i<m_nlevels;i++)
   {
      if(!same_as(&(m_info[i]), &(m_orig[i])))
      {
         buff[0]  = (unsigned char) i;
         buff[1]  = (unsigned char) m_info[i].normal_palette;
         buff[2]  = (unsigned char) m_info[i].super_palette;
         buff[3]  = (unsigned char) win2rom_x(m_info[i].points_x);
         buff[4]  = (unsigned char) win2rom_y(m_info[i].points_y);
         buff[5]  = (unsigned char) win2rom_x(m_info[i].weapon_x);
         buff[6]  = (unsigned char) win2rom_y(m_info[i].weapon_y);
         buff[7]  = (unsigned char) topbot_encode[m_info[i].top_type];
         buff[8]  = (unsigned char) topbot_encode[m_info[i].bottom_type];
         buff[9]  = (unsigned char) m_info[i].monster_speed;
         buff[10] = (unsigned char) m_info[i].bubble_speed;
         buff[11] = (unsigned char) m_info[i].break_speed;
         buff[12] = (unsigned char) m_info[i].alien_flag;
         buff[13] = (unsigned char) m_info[i].timer;
         buff[14] = (unsigned char) m_info[i].baron_time;
         buff[15] = (unsigned char) m_info[i].baron_speed;
         buff[16] = (unsigned char) m_info[i].lightning_rate;
         buff[17] = (unsigned char) m_info[i].water_rate;
         buff[18] = (unsigned char) m_info[i].fire_rate;
         buff[19] = (unsigned char) m_info[i].nmonsters;
         for(j=0;j<7;j++)
         {
            buff[20+5*j]   = (unsigned char) win2rom_x(m_info[i].monster_x[j]);
            buff[20+5*j+1] = (unsigned char) win2rom_y(m_info[i].monster_y[j]);
            buff[20+5*j+2] = (unsigned char) m_info[i].monster_drop[j];
            buff[20+5*j+3] = (unsigned char) m_info[i].monster_type[j];
            buff[20+5*j+4] = (unsigned char) m_info[i].monster_dir[j];
         }
         for(y=1;y<25;y++)
            for(x=0;x<32;x++)
                nibble_encode((char*)(buff+55), 32*(y-1)+x, m_info[i].arrow_data[y][x]);
         if(!WriteFile(fd, buff, PATCH_LEVEL_SIZE, &len, NULL))
         {
            CloseHandle(fd);
            return FALSE;
         }
      }
   }
   CloseHandle(fd);
   return TRUE;
}

BOOL LvlSet::load_patch(char* filename)
{
   HANDLE fd;
   DWORD len;
   unsigned char buff[PATCH_LEVEL_SIZE];
   int i;
   int j;
   int x;
   int y;

   if(filename == NULL || filename[0] == '\0')
      return FALSE;

   if( (fd=CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
      return FALSE;

   if(!ReadFile(fd, buff, 65, &len, NULL))
   {
      CloseHandle(fd);
      return FALSE;
   }

   if(memcmp(buff, "Patch-A-Bobble", 14) != 0)
      return FALSE;
   if( buff[14] != (unsigned char) PATCH_VERSION)
      return FALSE;

   memcpy(m_author, buff+15, 20);
   memcpy(m_desc, buff+35, 30);

   if(!ReadFile(fd, buff, PATCH_LEVEL_SIZE, &len, NULL))
   {
      CloseHandle(fd);
      return FALSE;
   }

   for(i=0;i<m_nlevels;i++)
      memcpy(&(m_info[i]), &(m_orig[i]), sizeof(m_info[i]));

   while(len == PATCH_LEVEL_SIZE)
   {
      i = (int) buff[0];
      if(i >= m_nlevels)
      {
         if(!ReadFile(fd, buff, PATCH_LEVEL_SIZE, &len, NULL))
            break;
         continue;
      }
      m_info[i].normal_palette = (int) buff[1];
      m_info[i].super_palette  = (int) buff[2];
      m_info[i].points_x       = rom2win_x((int) buff[3]);
      m_info[i].points_y       = rom2win_y((int) buff[4]);
      m_info[i].weapon_x       = rom2win_x((int) buff[5]);
      m_info[i].weapon_y       = rom2win_y((int) buff[6]);
      m_info[i].top_type       = topbot_decode[(int) buff[7]];
      m_info[i].bottom_type    = topbot_decode[(int) buff[8]];
      m_info[i].monster_speed  = (int) buff[9];
      m_info[i].bubble_speed   = (int) buff[10];
      m_info[i].break_speed    = (int) buff[11];
      m_info[i].alien_flag     = (int) buff[12];
      m_info[i].timer          = (int) buff[13];
      m_info[i].baron_time     = (int) buff[14];
      m_info[i].baron_speed    = (int) buff[15];
      m_info[i].lightning_rate = (int) buff[16];
      m_info[i].water_rate     = (int) buff[17];
      m_info[i].fire_rate      = (int) buff[18];
      m_info[i].nmonsters      = (int) buff[19];
      for(j=0;j<7;j++)
      {
         m_info[i].monster_x[j]    = rom2win_x((int) buff[20+5*j]);
         m_info[i].monster_y[j]    = rom2win_y((int) buff[20+5*j+1]);
         m_info[i].monster_drop[j] = (int) buff[20+5*j+2];
         m_info[i].monster_type[j] = (int) buff[20+5*j+3];
         m_info[i].monster_dir[j]  = (int) buff[20+5*j+4];
      }
      for(y=1;y<25;y++)
         for(x=0;x<32;x++)
             m_info[i].arrow_data[y][x] =  nibble_decode((char*)(buff+55), 32*(y-1)+x);

      if(!ReadFile(fd, buff, PATCH_LEVEL_SIZE, &len, NULL))
         break;
   }
   CloseHandle(fd);
   calc_screen();
   if(!data_valid(1, 102))
   {
      revert_all();
      return FALSE;
   }
   return TRUE;
}

void nibble_encode(char* buff, int offset, int val)
{
   if(offset%2)
   {
      buff[offset/2] &= 0xf0;
      buff[offset/2] |= (char)(val & 0x0f);
   }
   else
   {
      buff[offset/2] &= 0x0f;
      buff[offset/2] |= (char)((val<<4) & 0xf0);
   }
}

int nibble_decode(char* buff, int offset)
{
   if(offset%2)
      return buff[offset/2] & 0x0f;
   else
      return (buff[offset/2]>>4) & 0x0f;
}