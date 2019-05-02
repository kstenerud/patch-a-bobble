#include <windows.h>
#include "bitbuff.h"

int get_bit(char* data, long bit_offset)
{
   return ((data[(int)(bit_offset - (bit_offset%8)) / 8]
            & (1 << (int)(7-(bit_offset%8)))) != 0);
}

void set_bit(char* data, long bit_offset, int value)
{
   if(value)
      data[(int)(bit_offset - (bit_offset%8)) / 8]
         |= (char)((1 << (int)(7-(bit_offset%8))) & 0xff);
   else
      data[(int)(bit_offset - (bit_offset%8)) / 8]
         &= (char)((~(1 << (int)(7-(bit_offset%8)))) & 0xff);
}

BOOL bitcode_value(char* buff, long bit_offset, int val, int granularity)
{
   int i;
   long offset = bit_offset;

   if(val < 0 || val >= (1 << granularity))
      return FALSE;

   for(i=granularity-1;i>=0;i--)
      set_bit(buff, offset++, val & (1 << i));

   return TRUE;
}


BOOL BitBuff::set_granularity(int gran)
{
   if(gran < 1 || gran > 8)
      return FALSE;
   m_granularity = gran;
   return TRUE;
}

BOOL BitBuff::put(long bit_offset, int value, int granularity)
{
   if(bit_offset < 0 || bit_offset >= (int)m_len*8)
      return FALSE;
   if(granularity == 0)
      granularity = m_granularity;
   if(granularity < 1 || granularity > 8)
      return FALSE;

   return bitcode_value(m_data, bit_offset, value, granularity);
}

int BitBuff::get(long bit_offset, int granularity)
{
   long offset = bit_offset;
   int val = 0;

   if(bit_offset < 0 || bit_offset >= (int)m_len*8)
      return -1;
   if(granularity == 0)
      granularity = m_granularity;
   if(granularity < 1 || granularity > 8)
      return -1;

   switch(granularity)
   {
      case 8:  val += (::get_bit(m_data, offset++) ? 0x80 : 0);
      case 7:  val += (::get_bit(m_data, offset++) ? 0x40 : 0);
      case 6:  val += (::get_bit(m_data, offset++) ? 0x20 : 0);
      case 5:  val += (::get_bit(m_data, offset++) ? 0x10 : 0);
      case 4:  val += (::get_bit(m_data, offset++) ? 0x08 : 0);
      case 3:  val += (::get_bit(m_data, offset++) ? 0x04 : 0);
      case 2:  val += (::get_bit(m_data, offset++) ? 0x02 : 0);
      default: val += (::get_bit(m_data, offset++) ? 0x01 : 0);
   }
   return val & 0xff;
}

BOOL BitBuff::load(char* filename)
{
   HANDLE fd;
   DWORD len;

   if(filename != NULL)
   {
      if(m_filename != NULL)
         delete [] m_filename;
      m_filename = new char[strlen(filename)+1];
      strcpy(m_filename, filename);
      if( (fd=CreateFile(m_filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
      {
         if( (m_len=GetFileSize(fd, NULL)) != 0xffffffff)
         {
            if(m_data != NULL)
               delete [] m_data;
            m_data = new char[m_len];
            if(ReadFile(fd, m_data, m_len, &len, NULL))
            {
               CloseHandle(fd);
               return TRUE;
            }
            delete [] m_data;
            m_data = NULL;
         }
      }
      delete [] m_filename;
      m_filename = NULL;
   }
   CloseHandle(fd);
   return FALSE;
}

BOOL BitBuff::save(char* filename)
{
   HANDLE fd;
   DWORD len;

   if(filename != NULL)
   {
      if(m_filename != NULL)
         delete [] m_filename;
      m_filename = new char[strlen(filename)+1];
      strcpy(m_filename, filename);
   }

   if(m_filename != NULL && m_data != NULL)
   {
      if( (fd=CreateFile(m_filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
      {
         if(WriteFile(fd, m_data, m_len, &len, NULL))
         {
            CloseHandle(fd);
            return TRUE;
         }
      }
   }
   CloseHandle(fd);
   return FALSE;
}

