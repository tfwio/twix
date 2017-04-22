// after #include "defs.h"
// snd.h


//#pragma warning THIS FILE IS OBSOLETE!

#ifndef __snd_def__
#define __snd_def__
#include <stdlib.h>
#include <string.h>
//BOF////////////////////////////////////////////////////////////////
#include "on.tfwio.h"
//BOF////////////////////////////////////////////////////////////////
namespace on {
  namespace snd {

    using namespace tfwio;

#pragma region
#pragma pack(1)
    // unsigned short endian utility type
    // 16-Bit automating endian conversion utility
    typedef union t_uust{
      ui16  usvalue;
      ui8   ubvalue[2];
      ui16  GetValue(){
        return endo_ui16(ubvalue);
      }
    } uushort;

    // 32-Bit automating endian conversion utility
    typedef union t_uunt{
      ui32  uivalue;
      ui8   ubvalue[4];
      //uint  GetValue(){ return (ubvalue[0]<<24|ubvalue[1]<<16|ubvalue[2]<<8|ubvalue[3]); }
      ui32    GetValue(){
        return (int)endo_ui32(ubvalue);
      }
    } uuint;
    // another redef of header identifiers ;)
    typedef union t_snd_id{
      ui8   charID[4];
      uuint uuiv;
      char  *get_charID(){
        char *returned = new char[5];
        strncpy(returned, (char*)charID, 4);
        returned[4] = (char)0;
        return returned;
      }
    } snd_cktag;
#pragma pack(8)
    // common long ;)
    typedef struct t_snd_chunk_tag{
      snd_cktag ckHeadID;
      uuint   ckSize;
      char    *get_strval(){
        return ckHeadID.get_charID();
      }
    } snd_ckltag;
#pragma pack()
#pragma endregion " automated helper types "
  };
}; //h_out
#endif // __snd_def__
//EOF////////////////////////////////////////////////////////////////
#ifndef __iffdefs__
#define __iffdefs__
namespace on {
  namespace snd {
#pragma region
    // data is of a string type with the respective identity...
    // Where we are most likely going to have ascii or ansi char strings.
    enum wnfo {
      snd_w_ArchivalLocation = 0x4941524C, // "IARL"
      snd_w_Artist = 0x49415254, // "IART"
      snd_w_Commissioned = 0x49434D53, // "ICMS"
      snd_w_Comment = 0x49434D54, // "ICMT"
      snd_w_Copyright = 0x49434F50, // "ICOP"
      snd_w_DateCreated = 0x49435244, // "ICRD"
      snd_w_Cropped = 0x49435250, // "ICRP"
      snd_w_Dimensions = 0x4944494D, // "IDIM"
      snd_w_DotsPerInch = 0x49445049, // "IDPI"
      snd_w_Engineer = 0x49454E47, // "IENG"
      snd_w_Genre = 0x49474E52, // "IGNR"
      snd_w_Keywords = 0x494B4559, // "IKEY"
      snd_w_Lightness = 0x494C4754, // "ILGT"
      snd_w_Medium = 0x494D4544, // "IMED"
      snd_w_Title = 0x494E414D, // "INAM"
      snd_w_NumColors = 0x49504C54, // "IPLT"
      snd_w_Product = 0x49505244, // "IPRD"
      snd_w_Subject = 0x4953424A, // "ISBJ"
      snd_w_Software = 0x49534654, // "ISFT"
      snd_w_Sharpness = 0x49534850, // "ISHP"
      snd_w_Source = 0x49535243, // "ISRC"
      snd_w_SourceForm = 0x49535246, // "ISRF"
      snd_w_Technician = 0x49544348  // "ITCH"
      // there are 23
    };
    // really, not any of this belongs here.
    // the only thing that I can think of that does belong here would be a
    // base class for reading riff headers or the riff_io class (which is
    // defined elsewhere)
    static const std::string itt_hd[23] = {
      "IARL", "IART", "ICMS", "ICMT", "ICOP",
      "ICRD", "ICRP", "IDIM", "IDPI", "IENG",
      "IGNR", "IKEY", "ILGT", "IMED", "INAM",
      "IPLT", "IPRD", "ISBJ", "ISFT", "ISHP",
      "ISRC", "ISRF", "ITCH"
    };
#pragma endregion enum wnfo, const std::string itt_hd[23]
  };
};
#endif // __iffdefs__
//EOF////////////////////////////////////////////////////////////////

#ifndef __sndiff__
#define __sndiff__
namespace on {
  namespace snd {
#pragma region
#pragma pack(1)
    // Simple Chunk Header Tag
    typedef struct t_iff_zstring_ansi {
      snd_ckltag  header;
      // clean up your result with delete returnvalue;
      ubit *get_str_type(){
        uuint ttype = header.ckHeadID.uuiv;
        ubit *mo = new ubit[5];
        strncpy((char *)mo, (char *)header.ckHeadID.charID, 4);
        mo[4] = (char)0;
        return mo;
      }
      char * get_char_type(){
        uuint ttype = header.ckHeadID.uuiv;
        char *mo = new char[5];
        strncpy((char *)mo, (char *)header.ckHeadID.charID, 4);
        mo[4] = (char)0;
        return mo;
      }
    } iff_zstring;
    // Simple Chunk Header Tag
    typedef struct t_simple_header {
      uuint ttype;
      uint  length;
      uuint ttag;
    } iff_simple_chunk;
    // iff: zstring type
    // I've not correctly named this yet...
    // Neither has it been implemented.
    typedef struct t_iff_zstr {
      ui8   length;
      char  *value;
      t_iff_zstr(void);
      t_iff_zstr(int size){
        value = (char*)malloc(size);
      }
    } iff_zstr, zstr;
    // iff version tag
    typedef struct t_ck_iver {
      ui16    Major;
      ui16    Minor;
    } iver;
    // iff info tag
    typedef struct t_ck_nfo {
      snd_ckltag  infoHead;
      ui32    Length;
      iver    ifil;
      zstr    inam;
      zstr    isng;
      //  public Dictionary<long,INFOsub> nfosub;
      //  public INFO(int ckSize, BinaryReader bir, FileStream fis)
      //  {
      //    ifil = new iver(); inam = new ZSTR(); isng = new ZSTR();
      //
      //    long origin = fis.Seek(8,SeekOrigin.Current);
      //    infoHead = efx.STR.getStr(bir.ReadChars(4));
      //    Length = bir.ReadInt32();
      //    long pos = fis.Position;
      //    nfosub = new Dictionary<long,INFOsub>();
      //    while (fis.Position < pos+ckSize-4)
      //    {
      //      long hand = fis.Position;
      //      INFOsub inx = new INFOsub(bir,fis,this);
      //      fis.Seek(hand+inx.Length+8,SeekOrigin.Begin);
      //    }
      //  }
    } iff_ck_nfo;
    // iff chunk
    // a default chunk type
    typedef struct t_iff_chunk {
      ui8   ckID[4];  //  usually RIFF unless it's a sub-tag
      uint  ckLength; //  the length of the file-eight?
      ui8   ckTag[4]; //  the tagname
      char  *get_tag(){
        char *returned = new char[5];
        strncpy(returned, (char*)ckTag, 4);
        returned[4] = (char)0;
        return returned;
      }
      char  *get_hid(){
        char *returned = new char[5];
        strncpy(returned, (char*)t_iff_chunk::ckID, 4);
        returned[4] = (char)0;
        return returned;
      }
    } CHUNK, *lpCHUNK;
    // 
    typedef struct t_fmt_chunk {
      char  chunkID[4];
      ui32  chunkSize;
      ui16  wFormatTag;
      ui16  wChannels;
      //  is this of the right size?
      ui32  dwSamplesPerSec;
      ui32  dwAvgBytesPerSec;
      ui16  wBlockAlign;
      ui16  wBitsPerSample;

      char  *get_ckID(){
        char *returned = new char[5];
        strncpy(returned, chunkID, 4);
        returned[4] = (char)0;
        return returned;
      }
      // Note: there may be additional fields here, depending upon wFormatTag.
    } FormatChunk, *lpFMT;
    //
    typedef struct t_iff_taginfo {
      char  ckID[4];
      ui32  ckSize;
      char  *get_chunkID(){
        char *returned = new char[5];
        strncpy(returned, ckID, 4);
        returned[4] = (char)0;
        return returned;
      }
    } *lpTAGINFO;
    // 
    typedef struct t_ck_data {
      //use the freaking automated type, will you?
      char    chunkID[4];
      ui32  chunkSize;
      //should be a union unique to several audio data types & channels
      //but thats for the reader to worry about
      ui8   *waveformData;
      //delete 'return_value'
      char  *get_chunkID(){
        char *returned = new char[5];
        strncpy(returned, chunkID, 4);
        returned[4] = (char)0;
        return returned;
      }
    } DataChunk, *lpDATA;
    // 
#define IFF_HEADERSIZE (size_t)0x0C
    // 
#pragma pack()
#pragma endregion  " type defs "
#pragma region
    // R-IFF/Wave Format reader
    class iff_riff_wave {
    protected:
      FILE       *fp;
      lpCHUNK     fmt_chunk;
    public:
      char       *file_path;
      long int    file_length;

      iff_riff_wave() {}
      ~iff_riff_wave() {}

      // Get File Size/Length
      // this only works if the file is in an 'open' state
      // with (at least) read access.
      // Returns file size or (bool) false.
      uint iff_riff_wave::get_buffer_position(){
        if (file_path == 0) return false;
        fpos_t pp;
        fgetpos(fp, &pp);
        return (uint)pp;
      }
      // 
      uint iff_riff_wave::get_file_size(){
        if (file_path == 0) return false;
        uint uipos, uiend;
        uipos = get_buffer_position();
        fseek(fp, 0, 2); // 2==SEEK_END
        uiend = get_buffer_position();
        fseek(fp, uipos, 0); // ordinal position==0==SEEK_SET
        return uiend;
      }
      // 
      int iff_riff_wave::get_file_sizeu(){
        if (file_path == 0) return false;
        int uipos, uiend;
        uipos = get_buffer_position();
        fseek(fp, 0, 2); // 2==SEEK_END
        uiend = get_buffer_position();
        fseek(fp, uipos, 0); // ordinal position==0==SEEK_SET
        return uiend;
      }
      // Peek Ahead
      // Gets the next tag (IDENTITY) and returns the buffer to
      // it's original position.
      // delete return_value
      char* iff_riff_wave::peek_ahead(){
        int spoint = get_buffer_position();
        char *data = new char[5];
        fread(data, 4, 1, fp);
        data[4] = 0;
        fseek(fp, spoint, 0);
        return data;
      }
      // 
      int iff_riff_wave::get_int_position(long position, bool return_bpos = false){
        int opos;
        if (return_bpos) opos = get_buffer_position();
        fseek(fp, position, 0);
        int data;
        fread(&data, 4, 1, fp);
        if (return_bpos) fseek(fp, opos, 0);
        return data;
      }
      // 
      int iff_riff_wave::get_next_chunk(){
        int cpos = get_buffer_position();
        if (cpos >= file_length) return false;
        char *pchar = peek_ahead();
        int value = get_int_position(cpos + 4);
        fseek(fp, cpos, 0);
        return cpos + 8 + value;
      }
      // 
      lpTAGINFO iff_riff_wave::get_next_generic(){
        int cpos = get_buffer_position();
        if (cpos >= file_length) return false;

        // or do we do size of pointer?
        // I'll go with the size of the structure.
        int gsize = sizeof(t_iff_taginfo);

        lpTAGINFO gCK;
        gCK = (lpTAGINFO)malloc(gsize);

        fread(gCK, gsize, 1, fp);

        if (use_console) printf("'%s'> '%d'\n", gCK->get_chunkID(), gCK->ckSize);

        fseek(fp, cpos, 0);
        printf("==========================\n");
        return gCK;
      }
      // Load Header
      // sets the header data structure
      int iff_riff_wave::load_header(){
        if (file_path == 0) return false;
        fopen_s(&fp, file_path, "rb" /*"a"*/);

        file_length = get_file_size();
        fseek(fp, 0, 0);
        fmt_chunk = (lpCHUNK)malloc(IFF_HEADERSIZE);

        int j = fread(fmt_chunk, IFF_HEADERSIZE, 1, fp);
        fseek(fp, 12, 0);
        printf("==========================\n");
        for (int i = 0; i<file_length; i++)
        {
          get_next_generic();
          i = get_next_chunk();
          if (i < file_length) fseek(fp, i, 0);
        }
        fclose(fp);
        if (use_console) printf((char*)("IFF-header successfully loaded\n\n"));
        return j;
      }
      // 
      int iff_riff_wave::set_file(char *new_path){
        if (new_path == 0) return false;
        file_path = new_path;
        return true;
      }

    };
#pragma endregion " reader class "
  };
}; // end of namespace
#endif // __sndiff__
//EOF////////////////////////////////////////////////////////////////
