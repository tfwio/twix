#ifndef SND_MIDI
#define SND_MIDI smf
#pragma once

#if !defined(__tfw_ioh__)
// this really shouldn't be necessary
#include "on.tfwio.h"
#endif

#include <stdio.h>
#include <string>
#include <vector>

namespace on { namespace smf {

    // Bit packing forward (v >> b).
    // v is value.
    // b is shift amount.
#define mm_ishf(v,b) v >> b
    // Bit packing backward (v << b).
    // v is value.
    // b is shift amount.
#define mm_ishr(v,b) v << b

  typedef std::vector<std::string> strvec; // a simple string vector type

  // returns std::string vector of note names.
  static strvec nnx() {
    strvec nnxx;
    for (int i = 0; i<127;) {
      i += 3;
      nnxx.push_back("C ");
      nnxx.push_back("C#");
      nnxx.push_back("D ");
      i += 2;
      nnxx.push_back("D#");
      nnxx.push_back("E ");
      if (i >= 128) return nnxx;
      //printf("%d: %s\n",i-1,nnxx[i-1].c_str());
      i += 1;
      nnxx.push_back("F ");
      i += 4;
      nnxx.push_back("F#");
      nnxx.push_back("G ");
      nnxx.push_back("G#");
      nnxx.push_back("A ");
      i += 2;
      nnxx.push_back("A#");
      nnxx.push_back("B ");
    }
    return nnxx;
  }

  namespace meta {

    typedef enum t_smf_metaevent {
      meta_seqnum = 0xFF00,	//(o) seq#
      meta_text = 0xFF01,	//( ) text event
      meta_copyright = 0xFF02,	//( ) copyright
      meta_sequence = 0xFF03,	//( ) sequence
      meta_instrument = 0xFF04,	//( ) instrument name
      meta_lyrics = 0xFF05,	//( ) lyrics
      meta_marker = 0xFF06,	//( )
      meta_cuepoint = 0xFF07,	//( )
      meta_prefix = 0xFF20,	//(o)
      meta_tempo = 0xFF51,	//(o) 0x5103tttttt
      meta_smpte = 0xFF54,	//(o) FF 54 05 hr mn se fr ff SMPTE Offset
      meta_timesig = 0xFF58,	//(o) FF 58 04 nn dd cc bb    Time Signature
      meta_keysig = 0xFF59,	//(o) FF 59 02 sf mi  Key Signature
      //		sf = -7:  7 flats
      //		sf = -1:  1 flat
      //		sf = 0:  key of C
      //		sf = 1:  1 sharp
      //		sf = 7: 7 sharps
      //		mi = 0:  major key
      //		mi = 1:  minor key
      meta_endtrack = 0xFF2F,	//(o) SYSEX (not end)
      meta_sysex = 0xFF7F,	//(o) SYSEX (not end)
    } meta_event, smf_metaevent;

  }; // ns meta

  typedef enum t_smf_sys_events {
    sys_ = 0x70,		/** 	*/
    sys_excl = 0x7FFF,	/** 	*/
    //sys_		= 0x80,		/** 	*/
    //sys_		= 0x8FFF,	/** 	*/
    //sys_		= 0x90,		/** 	*/
    //sys_		= 0x9FFF,	/** 	*/
    //sys_		= 0xA0,		/** 	*/
    //sys_		= 0xAFFF,	/** 	*/
    //sys_		= 0xB0,		/** 	*/
    //sys_		= 0xBFFF,	/** 	*/
    //sys_		= 0xC0,		/** 	*/
    //sys_		= 0xCFFF,	/** 	*/
    //sys_	= 0xD0,		/** 	*/
    //sys_	= 0xE0,		/** 	*/
    //sys_	= 0xEFFF,	/** 	*/
    sys_ex = 0xF0,		/** 	*/
    //sys_	= 0xFFFF	/** 	*/
  } snd_SSX, smf_sys_events;

  typedef struct { union { int8 mm_x8[8]; }; } mm_msg; // not ever used!

  using snd_ckltag = on::snd::snd_ckltag;
  using uushort = on::snd::uushort;

#pragma pack(1)

  /////////////////////////////////////////////////////////////////////////////
  // SMF: Standard MIDI Track
  /////////////////////////////////////////////////////////////////////////////
  typedef struct t_mtrk {
    snd_ckltag	ckID;		/** {'M','T','r','k'}
                        int			ckSize;			...Size of the track
                        not incl: ID & Length */
    // ... the data chunk follows
    // we should result in a size of 8+the_data
  } mtrk, *mtrk_p;


  //////////////on::snd:://////////////////////////////////////////////////////
  // SMF: Standard Midi Format Header
  /////////////////////////////////////////////////////////////////////////////

  // Standard Midi Format Header
  typedef struct t_mthd {
    on::snd::snd_ckltag	ckMThd;		/**<	{'M','T','h','d'}
                                  int ckSize;		...the following 3 shorts are a length
                                  of 6, hence the size of 'ck_size'. */
    uushort		ckFormat;	/**<	( 0 | 1 | 2 ) */
    uushort		ckNTracks;	/**<	Number of MTrk(s) */
    uushort		ckDivision;	/**<	See MIDI FMT Spec or usage... */
    // ... the data chunk follows
    // we should result in a size of 14+the_data
  } mthd, *mthdp;

#pragma pack()

  // Class 'MTrk' structure for IO.  See snd_mtrk.
  class smf_mtrk : public mtrk {};

  // Class 'MThd' structure for IO.  See snd_mthd.
  class smf_mthd : public mthd {};

  typedef on::io::typ_TPL<mtrk, uint8>      snd_mtrk;
  typedef on::io::typ_TPL<mthdp, snd_mtrk>  snd_mthd;

  ///////////////////////////////
  //       //          //      //
  //  ///////  //  //  //  //////
  //       //  //  //  //      //
  //////// //  //  //  //  //////
  //       //  //////  //  //////
  ///////////////////////////////
  
  ///////////////////////////////
  //          //  //     ////  //
  //  //  //  //  // ///// //  //
  //  //  //  //  // ///// //  //
  //  //  //  //  // ///// //  //
  //  //////  //  //     ////  //
  ///////////////////////////////

  // midi/smf i/o class
  //  TODO: inherit from our file_io_base class (somewhere in tfw_inc)
  class SmfLoader
  {
  public:

    // doesn't load the file, just sets the file-name.
    SmfLoader(char* filename = 0);
    ~SmfLoader();

    // Reads the MThd structure
    int32 LoadHeader();

    // Reads the MTrk structure(s)...
    // 	this should, in the future be broken into 2 or more
    // 	functions.
    // Reads track data to memory.
    int32 LoadTracks();

    int32 GetTrack(FILE *fp, const uint32 trackid, const long position);

    // NOT IMPLEMENTED
    uint8* GetTrackData(const uint32 trackid);

    // NOT IMPLEMENTED
    int32 get_message(uint32 ntrack, uint32 position);

  private:

    // Called by the Destructor
    void unload();

    char		 *m_filename;
    snd_mthd	m_file;
    int bpos(FILE *frf);
  };

}; }; // ns smf // ns snd

#endif // SND_MIDI

