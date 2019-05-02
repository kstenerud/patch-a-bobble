#include "screen.h"

Screen::Screen(LvlSet* lvl, GfxSet* gfx, HWND hOper_Button, int basex, int basey)
: m_gfxset(gfx)
, m_lvlset(lvl)
, m_crsr_x(-1)
, m_crsr_y(-1)
, m_move_item(MOVE_NONE)
, m_mouse_l_down(FALSE)
, m_mouse_r_down(FALSE)
, m_state(STATE_POINT)
, m_base_x(basex)
, m_base_y(basey)
, m_show_arrows(TRUE)
, m_hoper_button(hOper_Button)
, m_grid(FALSE)
, m_mode_drop(FALSE)
, m_drop_anim(FALSE)
{
   m_hwnd = gfx->get_hwnd();
   m_hdc = GetDC(m_hwnd);
   m_bg_dc = CreateCompatibleDC(m_hdc);
   m_bg_bmp = CreateCompatibleBitmap(m_hdc, 32*16, 26*16);
   SelectObject(m_bg_dc, m_bg_bmp);
   m_work_dc = CreateCompatibleDC(m_hdc);
   m_work_bmp = CreateCompatibleBitmap(m_hdc, 32*16, 26*16);
   SelectObject(m_work_dc, m_work_bmp);
}


void Screen::cancel(BOOL set_focus)
{
   drop_item(FALSE);
   m_state = STATE_POINT;
   if(set_focus)
      SetFocus(m_hwnd);
   clip_mouse_x(-1);
   newoper();
   stop_drop_anim();
}

BOOL Screen::set_selected_monster_type(int newtype)
{
   int retval;
   if(m_state != STATE_MOVE)
      return FALSE;
   if( (retval = m_lvlset->set_monster_type(m_move_item, newtype)) == TRUE)
      newoper();
   return retval;
}

BOOL Screen::set_selected_monster_drop(int num)
{
   if(m_state != STATE_MOVE)
      return FALSE;
   return m_lvlset->set_monster_drop_win(m_move_item, num);
}

BOOL Screen::invert_selected_monster()
{
   int retval;
   if(m_state != STATE_MOVE)
      return FALSE;

   if( (retval = m_lvlset->set_monster_dir(m_move_item, !m_lvlset->get_monster_dir(m_move_item))) == TRUE)
      newoper();
   return retval;
}

BOOL Screen::new_monster()
{
   int newmnum;

   if(m_state == STATE_MOVE || (newmnum = m_lvlset->insert_monster(0, 0, 0, 1, 0)) == -1)
      return FALSE;

   m_state = STATE_MOVE;
   m_move_item = newmnum;

   newoper();
   return TRUE;
}

BOOL Screen::del_selected_monster()
{
   int retval;
   if(m_state != STATE_MOVE)
      return FALSE;

   if( (retval = m_lvlset->delete_monster(m_move_item)) == TRUE)
   {
      m_state = STATE_POINT;
      m_move_item = MOVE_NONE;
      newoper();
   }
   return retval;
}

BOOL Screen::mouse_l_down(int x, int y)
{
   int ox;
   int oy;
   int block_x;
   int block_y;
   int i;

   if(!in_client_area(x, y))
      return FALSE;

   relative_xy(&x, &y);
   block_x = x>>4;
   block_y = y>>4;

   m_mouse_l_down = TRUE;

   switch(m_state)
   {
         case STATE_PAINT_SOLID:
            paint_block(block_x, block_y, 0);
            break;
         case STATE_PAINT_UP:
            paint_block(block_x, block_y, 1);
            break;
         case STATE_PAINT_LEFT:
            paint_block(block_x, block_y, 3);
            break;
         case STATE_PAINT_RIGHT:
            paint_block(block_x, block_y, 2);
            break;
         case STATE_PAINT_DOWN:
            paint_block(block_x, block_y, 4);
            break;
      case STATE_POINT:
         ox = m_lvlset->get_weapon_x();
         oy = m_lvlset->get_weapon_y();
         if(is_in_region(x, y, ox, oy, ox+31, oy+31))
         {
            pickup_item(MOVE_WEAPON);
            break;
         }
         ox = m_lvlset->get_points_x();
         oy = m_lvlset->get_points_y();
         if(is_in_region(x, y, ox, oy, ox+31, oy+31))
         {
            pickup_item(MOVE_POINTS);
            break;
         }
         for(i=0;i<=MOVE_M7;i++)
         {
            ox = m_lvlset->get_monster_x(i);
            oy = m_mode_drop ? m_lvlset->get_monster_drop_win(i) : m_lvlset->get_monster_y(i);
            if(is_in_region(x, y, ox, oy, ox+31, oy+31))
            {
               pickup_item(i);
               break;
            }
         }
         break;
      case STATE_MOVE:
         drop_item(TRUE);
         break;
   }
   return TRUE;
}

BOOL Screen::mouse_l_up(int x, int y)
{
   if(!in_client_area(x, y))
      return FALSE;

   m_mouse_l_down = FALSE;
   return TRUE;
}

BOOL Screen::mouse_r_down(int x, int y)
{
   if(!in_client_area(x, y))
      return FALSE;

   m_mouse_r_down = TRUE;
   cancel();
   return TRUE;
}

BOOL Screen::mouse_r_up(int x, int y)
{
   if(!in_client_area(x, y))
      return FALSE;

   m_mouse_r_down = FALSE;
   return TRUE;
}

BOOL Screen::mouse_move(int x, int y)
{
   int block_x;
   int block_y;

   relative_xy(&x, &y);

   m_crsr_x = x;
   m_crsr_y = y;

   if(!in_client_area(x, y))
      return FALSE;

   block_x = x>>4;
   block_y = y>>4;

   if(m_mouse_l_down)
   {
      switch(m_state)
      {
         case STATE_PAINT_SOLID:
            paint_block(block_x, block_y, 0);
            break;
         case STATE_PAINT_UP:
            paint_block(block_x, block_y, 1);
            break;
         case STATE_PAINT_LEFT:
            paint_block(block_x, block_y, 3);
            break;
         case STATE_PAINT_RIGHT:
            paint_block(block_x, block_y, 2);
            break;
         case STATE_PAINT_DOWN:
            paint_block(block_x, block_y, 4);
            break;
         case STATE_POINT:
            break;
         case STATE_MOVE:
            break;
      }
   }
   return TRUE;
}

void Screen::paint_block(int x, int y, int direction)
{
   if(direction < 0 || direction > 4)
      return;

   if(m_lvlset->change_block(x, y, direction))
   {
     draw_blocks(x, y, x+1, y+1, m_bg_dc);
     update(x<<4, y<<4, ((x+2)<<4)-1, ((y+2)<<4)-1);
   }
}

void Screen::set_mouse(int x, int y)
{
   POINT itempoint;
   itempoint.x = x;
   itempoint.y = y;
   ClientToScreen(m_hwnd, &itempoint);
   SetCursorPos(itempoint.x, itempoint.y);
}


void Screen::screen_xy(int* x, int* y)
{
   POINT pt;

   real_xy(x, y);
   pt.x = *x;
   pt.y = *y;
   ClientToScreen(m_hwnd, &pt);
   *x = pt.x;
   *y = pt.y;
}

void Screen::clip_mouse_x(int x)
{
   RECT rect;
   int x_val = x;
   int y_val = Y32_MIN;

   if(x < 0)
   {
      ClipCursor(NULL);
      return;
   }

   screen_xy(&x_val, &y_val);
   rect.left = x_val;
   rect.right = x_val;
   rect.top = y_val;

   x_val = x;
   y_val = Y32_MAX;
   screen_xy(&x_val, &y_val);
   rect.bottom = y_val;

   ClipCursor(&rect);
}

BOOL Screen::pickup_item(int item)
{
   int x;
   int y;

   switch(item)
   {
      case MOVE_POINTS:
         if(m_mode_drop)
            return FALSE;
         m_state = STATE_MOVE;
         m_move_item = MOVE_POINTS;
         x = m_lvlset->get_points_x();
         y = m_lvlset->get_points_y();
         set_mouse(x, y);
         update_sprite32(x, y);
         break;
      case MOVE_WEAPON:
         if(m_mode_drop)
            return FALSE;
         m_state = STATE_MOVE;
         m_move_item = MOVE_WEAPON;
         x = m_lvlset->get_weapon_x();
         y = m_lvlset->get_weapon_y();
         set_mouse(x, y);
         update_sprite32(x, y);
         break;
      case MOVE_M1:
      case MOVE_M2:
      case MOVE_M3:
      case MOVE_M4:
      case MOVE_M5:
      case MOVE_M6:
      case MOVE_M7:
         if(item >= m_lvlset->num_monsters())
            return FALSE;
         m_state = STATE_MOVE;
         m_move_item = item;
         x = m_lvlset->get_monster_x(item);
         y = m_mode_drop ? m_lvlset->get_monster_drop_win(item) : m_lvlset->get_monster_y(item);
         set_mouse(x, y);
         update_sprite32(x, y);
         if(m_mode_drop)
            clip_mouse_x(x);
         break;
      default:
         return FALSE;
   }
   newoper();
   return TRUE;
}

void Screen::drop_item(BOOL really_drop)
{
   int x = m_crsr_x;
   int y = m_crsr_y;
   int tmp;

   clip_mouse_x(-1);

   if(m_state != STATE_MOVE)
      return;

   if(m_grid)
   {
      tmp = x%16;
      if(tmp >= 8)
         x = x - tmp + 16;
      else
         x = x - tmp;

      tmp = y%16;
      if(tmp >= 8)
         y = y - tmp + 16;
      else
         y = y - tmp;
   }

   switch(m_move_item)
   {
      case MOVE_WEAPON:
         if(really_drop)
         {
            m_lvlset->set_weapon_x(x);
            m_lvlset->set_weapon_y(y);
         }
         x = m_lvlset->get_weapon_x();
         y = m_lvlset->get_weapon_y();
         break;
      case MOVE_POINTS:
         if(really_drop)
         {
            m_lvlset->set_points_x(x);
            m_lvlset->set_points_y(y);
         }
         x = m_lvlset->get_points_x();
         y = m_lvlset->get_points_y();
         break;
      case MOVE_M1:
      case MOVE_M2:
      case MOVE_M3:
      case MOVE_M4:
      case MOVE_M5:
      case MOVE_M6:
      case MOVE_M7:
         if(really_drop)
         {
            if(m_mode_drop)
               m_lvlset->set_monster_drop_win(m_move_item, y);
            else
            {
               m_lvlset->set_monster_y(m_move_item, y);
               m_lvlset->set_monster_x(m_move_item, x);
            }
         }
         x = m_lvlset->get_monster_x(m_move_item);
         y = m_mode_drop ? m_lvlset->get_monster_drop_win(m_move_item) : m_lvlset->get_monster_y(m_move_item);
         break;
   }
   m_state = STATE_POINT;
   m_move_item = MOVE_NONE;
   update_sprite32(x, y);
   newoper();
}

HBITMAP Screen::get_oper_bmp()
{
   switch(m_state)
   {
      case STATE_PAINT_SOLID:
         return m_gfxset->get_block32_bmp();
      case STATE_PAINT_UP:
         return m_gfxset->get_up32_bmp();
      case STATE_PAINT_DOWN:
         return m_gfxset->get_down32_bmp();
      case STATE_PAINT_LEFT:
         return m_gfxset->get_left32_bmp();
      case STATE_PAINT_RIGHT:
         return m_gfxset->get_right32_bmp();
      case STATE_MOVE:
         switch(m_move_item)
         {
            case MOVE_M1:
            case MOVE_M2:
            case MOVE_M3:
            case MOVE_M4:
            case MOVE_M5:
            case MOVE_M6:
            case MOVE_M7:
               return m_gfxset->get_monster_bmp(m_lvlset->get_disp_monster_type(m_move_item), m_lvlset->get_monster_dir(m_move_item));
            case MOVE_WEAPON:
               return m_gfxset->get_weapon_bmp();
            case MOVE_POINTS:
               return m_gfxset->get_points_bmp();
            default:
               return m_gfxset->get_blank32_bmp();
         }
      default:
         return m_gfxset->get_blank32_bmp();
   }
}

void Screen::change_cursor()
{
   if(this == NULL)
      return;

   if(!(m_state == STATE_MOVE) || !in_client_area(m_crsr_x, m_crsr_y))
      return;

   switch(m_move_item)
   {
      case MOVE_WEAPON:
         SetCursor(m_gfxset->get_weapon_icon());
         break;
      case MOVE_POINTS:
         SetCursor(m_gfxset->get_points_icon());
         break;
      case MOVE_M1:
      case MOVE_M2:
      case MOVE_M3:
      case MOVE_M4:
      case MOVE_M5:
      case MOVE_M6:
      case MOVE_M7:
         SetCursor(m_gfxset->get_monster_icon(m_lvlset->get_disp_monster_type(m_move_item), m_lvlset->get_monster_dir(m_move_item)));
         break;
   }
}

void Screen::start_drop_anim()
{
   cancel();
   m_drop_anim = TRUE;
   m_drop_pulse_clk = 0;
   pulse_drop_anim();
}

BOOL Screen::pulse_drop_anim()
{
   int i;
   int x;
   int y;
   int drop_y;
   int nmonsters = m_lvlset->num_monsters();
   BOOL change = FALSE;

   if(nmonsters <= 0 || !m_drop_anim)
      return FALSE;

   BitBlt(m_work_dc, 0, 0, 32*16, 26*16, m_bg_dc, 0, 0, SRCCOPY);

   for(i=nmonsters-1;i>=MONSTER_NUM_MIN;i--)
   {
      x = m_lvlset->get_monster_x(i);
      y = m_lvlset->get_monster_y(i);
      drop_y = m_drop_pulse_clk - ((m_lvlset->get_monster_drop(i))*2);
      if(y >= drop_y)
         y = drop_y;
      if(y+6 >= drop_y)
         change = TRUE;
      real_xy(&x, &y);
      m_gfxset->draw_icon(m_gfxset->get_monster_icon(m_lvlset->get_disp_monster_type(i), m_lvlset->get_monster_dir(i)), x, y, m_work_dc);
   }

   invalidate_screen();
   m_drop_pulse_clk += 8;
   return change;
}

void Screen::update(int xs, int ys, int xe, int ye)
{
   if(xs < 0)
      xs = 0;
   if(ys < 0)
      ys = 0;

   BitBlt(m_work_dc, xs, ys, xe-xs, ye-ys, m_bg_dc, xs, ys, SRCCOPY);
   draw_monsters(xs, ys, xe, ye, m_work_dc);
   draw_items(xs, ys, xe, ye, m_work_dc);
   invalidate_region(xs, ys, xe, ye);
}

void Screen::draw_blocks(int startx, int starty, int endx, int endy, HDC hdc)
{
   int x;
   int y;

   if(startx < BLOCK_X_MIN)
      startx = BLOCK_X_MIN;
   if(starty < BLOCK_Y_MIN)
      starty = BLOCK_Y_MIN;
   if(endx > BLOCK_X_MAX)
      endx = BLOCK_X_MAX;
   if(endy > BLOCK_Y_MAX)
      endy = BLOCK_Y_MAX;

   for(y=starty;y<=endy;y++)
      for(x=startx;x<=endx;x++)
         m_gfxset->draw_block(x<<4, y<<4, m_lvlset->get_arrow(x,y), m_lvlset->get_block(x,y), m_show_arrows, hdc);
}


void Screen::draw_items(int startx, int starty, int endx, int endy, HDC hdc)
{
   int x;
   int y;

   if(m_mode_drop)
      return;

   x = m_lvlset->get_weapon_x();
   y = m_lvlset->get_weapon_y();
   if(m_move_item != MOVE_WEAPON && has_common_area(startx, starty, endx, endy, x, y, x+31, y+31))
   {
      real_xy(&x, &y);
      m_gfxset->draw_icon(m_gfxset->get_weapon_icon(), x, y, hdc);
   }

   x = m_lvlset->get_points_x();
   y = m_lvlset->get_points_y();
   if(m_move_item != MOVE_POINTS && has_common_area(startx, starty, endx, endy, x, y, x+31, y+31))
   {
      real_xy(&x, &y);
      m_gfxset->draw_icon(m_gfxset->get_points_icon(), x, y, hdc);
   }
}

void Screen::draw_monsters(int startx, int starty, int endx, int endy, HDC hdc)
{
   int i;
   int x;
   int y;
   int nmonsters = m_lvlset->num_monsters();

   if(nmonsters <= 0)
      return;

   for(i=nmonsters-1;i>=MONSTER_NUM_MIN;i--)
   {
      x = m_lvlset->get_monster_x(i);
      y = m_mode_drop ? m_lvlset->get_monster_drop_win(i) : m_lvlset->get_monster_y(i);
      if(m_move_item != i && has_common_area(startx, starty, endx, endy, x, y, x+31, y+31))
      {
         real_xy(&x, &y);
         m_gfxset->draw_icon(m_gfxset->get_monster_icon(m_lvlset->get_disp_monster_type(i), m_lvlset->get_monster_dir(i)), x, y, hdc);
      }
   }
}

void Screen::invalidate_region(int startx, int starty, int endx, int endy)
{
   RECT rect = {startx+m_base_x,
                starty+m_base_y,
                endx+m_base_x,
                endy+m_base_y};

   InvalidateRect(m_hwnd, &rect, FALSE);
}

void Screen::make_valid_xy32(int* x, int* y)
{
   if(*x < REGION32_X_MIN) *x = REGION32_X_MIN;
   else if(*x > REGION32_X_MAX) *x = REGION32_X_MAX;

   if(*y < REGION32_Y_MIN) *y = REGION32_Y_MIN;
   else if(*y > REGION32_Y_MAX) *y = REGION32_Y_MAX;
}

BOOL has_common_area(int x1s, int y1s, int x1e, int y1e,
                     int x2s, int y2s, int x2e, int y2e)
{
   return ( ((x2s >= x1s && x2s <= x1e) ||
             (x2e >= x1s && x2e <= x1e) ||
             (x1s >= x2s && x1s <= x2e) ||
             (x1e >= x2s && x1e <= x2e)) &&
            ((y2s >= y1s && y2s <= y1e) ||
             (y2e >= y1s && y2e <= y1e) ||
             (y1s >= y2s && y1s <= y2e) ||
             (y1e >= y2s && y1e <= y2e)) );
}

BOOL is_in_region(int x, int y, int xs, int ys, int xe, int ye)
{
   return x >= xs && x <= xe && y >= ys && y <= ye;
}


