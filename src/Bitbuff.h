#ifndef BITBUFF__HEADER
#define BITBUFF__HEADER

#include <windows.h>

void nibble_encode(char* buff, int offset, int val);
int nibble_decode(char* buff, int offset);
int get_bit(char* data, long bit_offset);
void set_bit(char* data, long bit_offset, int value);
BOOL bitcode_value(char* buff, long bit_offset, int val, int granularity);

class BitBuff
{
public:
         BitBuff(int granularity)
         : m_filename(NULL)
         , m_data(NULL)
         , m_len(0)
         , m_granularity(8)
         {set_granularity(granularity);}
         ~BitBuff() {if(m_filename != NULL) delete [] m_filename;
                     if(m_data != NULL) delete [] m_data;}
   BOOL  load           (char* filename);
   BOOL  save           (char* filename);
   int   get            (long bit_offset, int granularity = 0);
   BOOL  put            (long bit_offset, int value, int granularity = 0);
   BOOL  set_granularity(int gran);
   int   get_granularity() {return m_granularity;}
   long  get_len        () {return m_len*8;}
   char* get_filename   () {return m_filename;}
private:

   char* m_filename;
   char* m_data;
   DWORD m_len;
   int   m_granularity;
};

#endif
