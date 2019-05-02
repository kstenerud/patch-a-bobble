#ifndef LVLSET__HEADER
#define LVLSET__HEADER

#include "limits.h"
#include "bitbuff.h"


struct levelinfo
{
   int arrow_data[26][32];
   int normal_palette;
   int super_palette;
   int points_x;
   int points_y;
   int weapon_x;
   int weapon_y;
   int top_type;
   int bottom_type;
   int monster_speed;
   int bubble_speed;
   int break_speed;
   int alien_flag;
   int timer;
   int baron_time;
   int baron_speed;
   int lightning_rate;
   int water_rate;
   int fire_rate;
   int monster_x[7];
   int monster_y[7];
   int monster_drop[7];
   int monster_type[7];
   int monster_dir[7];
   int nmonsters;
};

class LvlSet
{
public:
   LvlSet           (int nlevels);
   ~LvlSet          ()             {delete [] m_info; delete[] m_orig;}
   BOOL set_super   (BOOL val)     {if(m_super == (val!=0)) return FALSE; m_super = (val!=0); return TRUE;}
   BOOL get_super   ()             {return m_super;}
   void toggle_super()             {m_super = !m_super;}

   void drop_pattern_wipe         ();
   void drop_pattern_slide        ();
   void drop_pattern_roll         ();
   void drop_pattern_stretch      ();
   void drop_pattern_shrink       ();
   void drop_pattern_angle_left   ();
   void drop_pattern_angle_right  ();
   void drop_pattern_angle_inside ();
   void drop_pattern_angle_outside();

   BOOL load_roms  (char* srom_name, char* lrom1_name, char* lrom2_name, char* lrom3_name, char* lrom4_name, int start_level=LEVEL_MIN, int end_level=0);
   BOOL save_roms  (char* srom_name, char* lrom1_name, char* lrom2_name, char* lrom3_name, char* lrom4_name, int start_level=LEVEL_MIN, int end_level=0);
   BOOL data_valid (int start_level, int end_level);
   BOOL load_patch (char* filename);
   BOOL save_patch (char* filename);

   BOOL set_level         (int newlevel);
   int  get_level         ()         {return m_level+1;}
   int  insert_monster    (int type, int x, int y, int drop, int dir);
   BOOL delete_monster    (int mnum);
   int  get_normal_palette()         {return m_info[m_level].normal_palette;}
   BOOL set_normal_palette(int val);
   int  get_super_palette ()         {return (m_info[m_level].super_palette+3)%8;}
   BOOL set_super_palette (int val);
   int  get_palette       ()         {return m_super ? get_super_palette() : get_normal_palette();}
   BOOL set_palette       (int val)  {return m_super ? set_super_palette(val) : set_normal_palette(val);}
   int  get_points_y      ()         {return m_info[m_level].points_y;}
   BOOL set_points_y      (int val);
   int  get_points_x      ()         {return m_info[m_level].points_x;}
   BOOL set_points_x      (int val);
   int  get_top           ()         {return m_info[m_level].top_type;}
   BOOL set_top           (int val);
   int  get_bottom        ()         {return m_info[m_level].bottom_type;}
   BOOL set_bottom        (int val);
   int  get_break_speed   ()         {return m_info[m_level].break_speed;}
   BOOL set_break_speed   (int val);
   int  get_monster_speed ()         {return m_info[m_level].monster_speed;}
   BOOL set_monster_speed (int val);
   int  get_bubble_speed  ()         {return m_info[m_level].bubble_speed;}
   BOOL set_bubble_speed  (int val);
   int  get_weapon_y      ()         {return m_info[m_level].weapon_y;}
   BOOL set_weapon_y      (int val);
   int  get_weapon_x      ()         {return m_info[m_level].weapon_x;}
   BOOL set_weapon_x      (int val);
   int  get_alien_flag    ()         {return m_info[m_level].alien_flag;}
   BOOL set_alien_flag    (int val)  {if(m_info[m_level].alien_flag == (val!=0)) return FALSE; m_info[m_level].alien_flag = (val!=0); return TRUE;}
   void toggle_alien_flag ()         {m_info[m_level].alien_flag = !m_info[m_level].alien_flag;}
   int  get_timer         ()         {return m_info[m_level].timer;}
   BOOL set_timer         (int val);
   int  get_baron_time    ()         {return m_info[m_level].baron_time;}
   BOOL set_baron_time    (int val);
   int  get_baron_speed   ()         {return m_info[m_level].baron_speed;}
   BOOL set_baron_speed   (int val);
   int  get_lightning_rate()         {return m_info[m_level].lightning_rate;}
   BOOL set_lightning_rate(int val);
   int  get_water_rate    ()         {return m_info[m_level].water_rate;}
   BOOL set_water_rate    (int val);
   int  get_fire_rate     ()         {return m_info[m_level].fire_rate;}
   BOOL set_fire_rate     (int val);

   int  get_monster_drop_win (int mnum)          {return rom2win_drop(get_monster_drop(mnum));}
   int  get_monster_drop     (int mnum)          {if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX) return -1; return m_info[m_level].monster_drop[mnum];}
   BOOL set_monster_drop_win (int mnum, int val) {return set_monster_drop(mnum, win2rom_drop(val));}
   BOOL set_monster_drop     (int mnum, int val);
   int  get_monster_y        (int mnum)          {if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX) return -1; return m_info[m_level].monster_y[mnum];}
   BOOL set_monster_y        (int mnum, int val);
   int  get_monster_x        (int mnum)          {if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX) return -1; return m_info[m_level].monster_x[mnum];}
   BOOL set_monster_x        (int mnum, int val);
   int  get_monster_dir      (int mnum)          {if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX) return -1; return m_info[m_level].monster_dir[mnum];}
   BOOL set_monster_dir      (int mnum, int val);
   int  get_monster_type     (int mnum)          {if(mnum < MONSTER_NUM_MIN || mnum > MONSTER_NUM_MAX) return -1; return m_info[m_level].monster_type[mnum];}
   int  get_disp_monster_type(int mnum);
   BOOL set_monster_type     (int mnum, int val);

   int  num_monsters()              {return m_info[m_level].nmonsters;}
   BOOL change_block(int x, int y, int val);
   int  get_arrow   (int x, int y);
   int  get_block   (int x, int y);
   BOOL same_as     (LvlSet* dbuff, int level);
   BOOL same_as     (levelinfo* one, levelinfo* two);
   BOOL copy        (int mylevel, LvlSet* dest, int destlevel);
   BOOL xchg        (int mylevel, LvlSet* them, int theirlevel);
   BOOL clear       (int level);
   BOOL revert      (int level);
   void revert_level()             {revert(m_level+1);}
   void clear_level ()             {clear(m_level+1);}
   void revert_all  ()             {memcpy(m_info, m_orig, sizeof(m_info[0])*m_nlevels);}
   void clear_all   ();
   void make_default()             {memcpy(m_orig, m_info, sizeof(m_info[0])*m_nlevels);}
   BOOL is_default  (int level);
   void set_author  (char* author) {memset(m_author, 0, 21); strncpy(m_author, author, 20);}
   char* get_author ()             {return m_author;}
   void set_desc    (char* desc)   {memset(m_desc, 0, 31); strncpy(m_desc, desc, 30);}
   char* get_desc   ()             {return m_desc;}
private:
   void optimize_pattern();
   void decode_top      ();
   void decode_bottom   ();
   void sort_monsters   (int level);
   void swap_monsters   (int level, int a, int b);
   void copy_monster    (int level, int src, int dst);
   int  rom2win_x       (int x)         {return x*2;}
   int  rom2win_y       (int y)         {return (26*8-1-y)*2;}
   int  rom2win_drop    (int drop)      {return ((26*8-1-(drop))*2)-48;}
   int  win2rom_x       (int x)         {return x/2;}
   int  win2rom_y       (int y)         {return 26*8-1-(y/2);}
   int  win2rom_drop    (int drop)      {return (26*8-1-((drop+48)/2));}
   void calc_block      (int x, int y);
   void calc_top        ();
   void calc_bottom     ();
   void calc_screen     ();

   char              m_author[21];
   char              m_desc[31];
   int               m_block_data[26][32];
   struct levelinfo* m_info;
   struct levelinfo* m_orig;
   int               m_level;
   int               m_nlevels;
   BOOL              m_super;
};

#endif
