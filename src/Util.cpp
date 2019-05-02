#include "util.h"

char* rom_names[15][2] =
{
	{"a78-10.13", "gfx1a.dat"},
	{"a78-16.31", "gfx1b.dat"},
	{"a78-11.14", "gfx2a.dat"},
	{"a78-17.32", "gfx2b.dat"},
	{"a78-12.15", "gfx3a.dat"},
	{"a78-18.33", "gfx3b.dat"},
	{"a78-09.12", "gfxba.dat"},
	{"a78-15.30", "gfxbb.dat"},
	{"a78-13.16", "gfxsa.dat"},
	{"a78-19.34", "gfxsb.dat"},
	{"a78-08.37", "screen.dat"},
	{"a78-05.52", "level.dat"}, // Bubble Bobble
	{"a78-24.52", "level.dat"}, // Bubble Bobble (US with mode select)
	{"a78-21.52", "level.dat"}, // Bubble Bobble (US)
	{"bb5",       "level.dat"}, // Bobble Bobble / Super Bobble Bobble
};

BOOL load_file(char* filename, char* buff, int len)
{
   HANDLE fd;
   DWORD length = (DWORD) len;
   DWORD rlen;
   BOOL result;

   if(filename == NULL || buff == NULL || len <= 0)
      return FALSE;

   if( (fd=CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
      return FALSE;
   result = ReadFile(fd, buff, length, &rlen, NULL);
   CloseHandle(fd);
   if(result == FALSE || rlen != length)
      return FALSE;
   return TRUE;
}


void set_track_and_text(HWND track, HWND text, int val, int off_val)
{
   char buff[10];
   SendMessage(track, TBM_SETPOS, 1, val);
   if(off_val >= 0 && val == off_val)
      sprintf(buff, "Off");
   else
      sprintf(buff, "%d", val);
   SetWindowText(text, buff);
}

BOOL load_filerequestor(char* filepath, char* filename, char* title, char* ext, char* desc)
{
   OPENFILENAME ofn;
   char filter_buff[100] = {0};
   char ext_buff[10] = {0};
   char* alltext = "All Files (*.*)" "\0" "*.*" "\0" "\0";
   char* ptr = filter_buff;

   if(ext != NULL && desc != NULL)
   {
      sprintf(filter_buff, "%s (*.%s)%c*.%s", desc, ext, 0, ext);
      strcpy(ext_buff, ext);
      ptr += strlen(ptr)+1;
      ptr += strlen(ptr)+1;
   }

   memcpy(ptr, alltext, 21);
   filepath[0] = 0;

   ofn.lStructSize = sizeof(OPENFILENAME);
   ofn.hwndOwner = NULL;
   ofn.hInstance = 0;
   ofn.lpstrFilter = filter_buff; //"Bubble Patch (*.bub)" "\0" "*.bub" "\0" "All Files (*.*)" "\0" "*.*" "\0" "\0";
   ofn.lpstrCustomFilter = NULL;
   ofn.nMaxCustFilter = 0;
   ofn.nFilterIndex = 1;
   ofn.lpstrFile = filepath;
   ofn.nMaxFile = 1000;
   ofn.lpstrFileTitle = filename;
   ofn.nMaxFileTitle = 1000;
   ofn.lpstrInitialDir = NULL;
   ofn.lpstrTitle = title;
   ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_EXPLORER;
   ofn.nFileOffset = 0;
   ofn.nFileExtension = 0;
   ofn.lpstrDefExt = ext != NULL ? ext : NULL;
   ofn.lCustData = 0;
   ofn.lpfnHook = NULL;
   ofn.lpTemplateName = NULL;

   return GetOpenFileName(&ofn);
}

BOOL save_filerequestor(char* filepath, char* filename, char* title, char* ext, char* desc)
{
   OPENFILENAME ofn;
   char filter_buff[100] = {0};
   char ext_buff[10] = {0};
   char* alltext = "All Files (*.*)" "\0" "*.*" "\0" "\0";
   char* ptr = filter_buff;

   if(ext != NULL && desc != NULL)
   {
      sprintf(filter_buff, "%s (*.%s)%c*.%s", desc, ext, 0, ext);
      strcpy(ext_buff, ext);
      ptr += strlen(ptr)+1;
      ptr += strlen(ptr)+1;
   }

   memcpy(ptr, alltext, 21);
   filepath[0] = 0;

   ofn.lStructSize = sizeof(OPENFILENAME);
   ofn.hwndOwner = NULL;
   ofn.hInstance = 0;
   ofn.lpstrFilter = filter_buff; //"Bubble Patch (*.bub)" "\0" "*.bub" "\0" "All Files (*.*)" "\0" "*.*" "\0" "\0";
   ofn.lpstrCustomFilter = NULL;
   ofn.nMaxCustFilter = 0;
   ofn.nFilterIndex = 1;
   ofn.lpstrFile = filepath;
   ofn.nMaxFile = 1000;
   ofn.lpstrFileTitle = filename;
   ofn.nMaxFileTitle = 1000;
   ofn.lpstrInitialDir = NULL;
   ofn.lpstrTitle = title;
   ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_EXPLORER;
   ofn.nFileOffset = 0;
   ofn.nFileExtension = 0;
   ofn.lpstrDefExt = ext != NULL ? ext : NULL;
   ofn.lCustData = 0;
   ofn.lpfnHook = NULL;
   ofn.lpTemplateName = NULL;

   return GetSaveFileName(&ofn);
}

void msgbox(char* msg, ...)
{
   char buff[2000];
   va_list ap;
   va_start(ap, msg);
   vsprintf(buff, msg, ap);
   va_end(ap);
   MessageBox(NULL, buff, "Message", MB_OK);
}

void errorbox(char* msg, ...)
{
   char buff[2000];
   va_list ap;
   va_start(ap, msg);
   vsprintf(buff, msg, ap);
   va_end(ap);
   MessageBox(NULL, buff, "Error", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
}


BOOL get_load_files(char* screen, char* level1, char* level2, char* level3, char* level4)
{
   if(!load_filerequestor(level1, screen, "Load roms from where?", "*", "All Files"))
      return FALSE;

   strip_filename(level1);
   strcpy(screen, level1);
   strcpy(level2, level1);
   strcpy(level3, level1);
   strcpy(level4, level1);
   strcat(level1, rom_names[ROMS_LEVEL_BUBLBOBL][0]);
   strcat(level2, rom_names[ROMS_LEVEL_BUBLBOBL_UMS][0]);
   strcat(level3, rom_names[ROMS_LEVEL_BUBLBOBL_U][0]);
   strcat(level4, rom_names[ROMS_LEVEL_BOBLBOBL][0]);
   strcat(screen, rom_names[ROMS_SCREEN][0]);

   return TRUE;
}

BOOL get_save_files(char* screen, char* level1, char* level2, char* level3, char* level4)
{
   if(!save_filerequestor(level1, screen, "Save roms to where?", "*", "All Files"))
      return FALSE;

   strip_filename(level1);
   strcpy(screen, level1);
   strcpy(level2, level1);
   strcpy(level3, level1);
   strcpy(level4, level1);
   strcat(level1, rom_names[ROMS_LEVEL_BUBLBOBL][0]);
   strcat(level2, rom_names[ROMS_LEVEL_BUBLBOBL_UMS][0]);
   strcat(level3, rom_names[ROMS_LEVEL_BUBLBOBL_U][0]);
   strcat(level4, rom_names[ROMS_LEVEL_BOBLBOBL][0]);
   strcat(screen, rom_names[ROMS_SCREEN][0]);

   return TRUE;
}


BOOL make_data_files(char* dir)
{
   char msgbuf[300];
   char filepath[500] = {0};
   char filename[500] = {0};
   char src_file[500] = {0};
   char dst_file[500] = {0};
   char* psrc;
   char* pdst;
   int i;

   sprintf(msgbuf, "Please select a directory containing a fresh set of Bubble Bobble roms.\nThe roms must be named as follows:\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\nAnd then one of the following:\n%s\n%s\n%s\n%s",
           rom_names[ROMS_GFX1A][0], rom_names[ROMS_GFX1B][0],
           rom_names[ROMS_GFX2A][0], rom_names[ROMS_GFX2B][0],
           rom_names[ROMS_GFX3A][0], rom_names[ROMS_GFX3B][0],
           rom_names[ROMS_GFXBA][0], rom_names[ROMS_GFXBB][0],
           rom_names[ROMS_GFXSA][0], rom_names[ROMS_GFXSB][0],
           rom_names[ROMS_SCREEN][0],
           rom_names[ROMS_LEVEL_BUBLBOBL][0], rom_names[ROMS_LEVEL_BUBLBOBL_UMS][0], rom_names[ROMS_LEVEL_BUBLBOBL_U][0],
           rom_names[ROMS_LEVEL_BOBLBOBL][0]);
   MessageBox(NULL, msgbuf, "Create Data Files", MB_OK | MB_APPLMODAL);

   sprintf(msgbuf, "Locate the file: %s", rom_names[ROMS_SCREEN][0]);

   if(!load_filerequestor(filepath, filename, msgbuf, "*", "All Files"))
      return FALSE;

   strip_filename(filepath);
   strcpy(src_file, filepath);
   strcpy(dst_file, dir);
   psrc = src_file + strlen(src_file);
   pdst = dst_file + strlen(dst_file);

   for(i=0;i<=10;i++)
   {
      strcpy(psrc, rom_names[i][0]);
      strcpy(pdst, rom_names[i][1]);
      if(!CopyFile(src_file, dst_file, FALSE))
      {
         MessageBox(NULL, "Error copying files!", "File Error", MB_OK | MB_APPLMODAL);
         return FALSE;
      }
   }

   strcpy(psrc, rom_names[ROMS_LEVEL_BUBLBOBL][0]);
   strcpy(pdst, rom_names[ROMS_LEVEL_BUBLBOBL][1]);
   if(CopyFile(src_file, dst_file, FALSE))
      return TRUE;
   strcpy(psrc, rom_names[ROMS_LEVEL_BUBLBOBL_UMS][0]);
   strcpy(pdst, rom_names[ROMS_LEVEL_BUBLBOBL_UMS][1]);
   if(CopyFile(src_file, dst_file, FALSE))
      return TRUE;
   strcpy(psrc, rom_names[ROMS_LEVEL_BUBLBOBL_U][0]);
   strcpy(pdst, rom_names[ROMS_LEVEL_BUBLBOBL_U][1]);
   if(CopyFile(src_file, dst_file, FALSE))
      return TRUE;
   strcpy(psrc, rom_names[ROMS_LEVEL_BOBLBOBL][0]);
   strcpy(pdst, rom_names[ROMS_LEVEL_BOBLBOBL][1]);
   if(CopyFile(src_file, dst_file, FALSE))
      return TRUE;

   MessageBox(NULL, "Error copying files!", "File Error", MB_OK | MB_APPLMODAL);
   return FALSE;
}

void strip_filename(char* str)
{
   char* ptr = str;
   char* lastptr = str;

   if(ptr == NULL)
      return;

   while(*ptr != 0)
   {
      if(*ptr == '\\' || *ptr == ':' || *ptr == '/')
         lastptr = ptr;
      ptr++;
   }
   *(lastptr+1) = 0;
}

BOOL load_roms(LvlSet* lset)
{
   char screen_name[500];
   char level1_name[500];
   char level2_name[500];
   char level3_name[500];
   char level4_name[500];

   if(!get_load_files(screen_name, level1_name, level2_name, level3_name, level4_name))
      return FALSE;
   if(!lset->load_roms(screen_name, level1_name, level2_name, level3_name, level4_name))
   {
      errorbox("Error reading %s and %s: files are corrupt.", level1_name, screen_name);
      return FALSE;
   }
   return TRUE;
}

BOOL save_roms(LvlSet* lset)
{
   char screen_name[500];
   char level1_name[500];
   char level2_name[500];
   char level3_name[500];
   char level4_name[500];

   if(!get_save_files(screen_name, level1_name, level2_name, level3_name, level4_name))
      return FALSE;
   if(!lset->save_roms(screen_name, level1_name, level2_name, level3_name, level4_name))
   {
      errorbox("Error reading %s and %s: files are corrupt.", level1_name, screen_name);
      return FALSE;
   }
   return TRUE;
}

void set_tooltip(HINSTANCE hinst, HWND hwnd, HWND htooltip, HWND htool, TOOLINFO* tinfo, char* text)
{
   tinfo->cbSize = sizeof(TOOLINFO);
   tinfo->uFlags = TTF_IDISHWND;
   tinfo->hwnd   = hwnd;
   tinfo->uId    = (UINT)htool;
   tinfo->rect.top = tinfo->rect.bottom = tinfo->rect.left = tinfo->rect.right = 0;
   tinfo->hinst  = hinst;
   tinfo->lpszText = text;

   SendMessage(htooltip, TTM_ADDTOOL, 0, (LPARAM)(&tinfo));
}

