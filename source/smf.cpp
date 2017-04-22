#include "on.smf.h"

#define SMF_CONSOLE
#define IO_READ_MODE "r"
//#define IO_READ_MODE "rb"

namespace on { namespace smf {

  SmfLoader::SmfLoader(char* filename)
  {
    if (filename == 0) return;
    m_filename = filename;
  }

  SmfLoader::~SmfLoader() { unload(); }

  int SmfLoader::bpos(FILE *frf)
  {
    fpos_t pp;
    fgetpos(frf, &pp);
    return (int)pp;
  }

  int SmfLoader::LoadHeader()
  {
    FILE*	fp;
    int32 isize = sizeof(mthd);

    m_file.ckHead = (mthdp)malloc(isize);

    if (m_filename == 0)
      return false;

    fp = fopen(m_filename, IO_READ_MODE /*"a"*/);

    int32 j = fread(m_file.ckHead, isize, 1, fp);

    fclose(fp);

    //heaDirty=true;
#if defined(SMF_CONSOLE)
    printf("SMF:MIDI-header successfully loaded\n\n");
#endif
    return 0; // j;
  }

  int SmfLoader::LoadTracks()
  {
    FILE*	fp;
    uint32 numberoftracks = m_file.ckHead->ckNTracks.GetValue();
    m_file.ckData = new snd_mtrk[numberoftracks];

    //if (SMF_CONSOLE) printf("DEBUG> ntracks = \"%d\"\n",m_file.ckHead->ckNTracks.GetValue());
    int32 isize = sizeof(mtrk);

    fp = fopen(m_filename, "rb" /*"a"*/);// mtrk_p

    int32 l_pos = 14;// at this point we're only interested in the header

    // SEEK_CUR 1 // SEEK_END 2 // SEEK_SET 0
    for (uint32 i = 0; i < numberoftracks; i++)
    {
      //if (i==1) break;
      printf("getting track number %d\n", i);
      l_pos = GetTrack(fp, i, l_pos);
    }

    fclose(fp);

    //delete [] m_file.ckData;
    //heaDirty=true;
#if defined(SMF_CONSOLE)
    printf("SMF:MIDI-Track->header successfully loaded\n\n");
#endif
    return 0; // j;
  }

  int SmfLoader::GetTrack(FILE *fp, const uint32 trackid, const long position)
  {
    //if (SMF_CONSOLE) printf("\t%d...just checking\n",trackid);
    //if (i==1) break;
    fseek(fp, position, 0); // m_file.ckDatam_tracks
    int32 j = fread(&m_file.ckData[trackid].ckHead, 8, 1, fp);
    int32 newsize = m_file.ckData[trackid].ckHead.ckID.ckSize.GetValue();
    //m_file.ckData[trackid].

#if defined(SMF_CONSOLE)
    printf("\tbuffer-pos> \"%d\"\n", bpos(fp));
    //uint cks = m_file.ckData[trackid].ckHead.ckID.ckSize.uuiv.GetValue();
    printf("\ttkID[%d]> \"%s\"\n", trackid, m_file.ckData[trackid].ckHead.ckID.ckHeadID.charID);
    printf("\ttkID[%d].Size> \"%d\"\n", trackid, newsize);
#endif

    m_file.ckData[trackid].ckData = new uint8[newsize];

    j = fread(m_file.ckData[trackid].ckData, newsize, 1, fp);

    return position + 8 + newsize;
  }

  uint8 *SmfLoader::GetTrackData(const uint32 trackid)
  {
    uint8 *hf = 0;
    return hf;
    //return m_file.ckData[trackid].;
  }

  void SmfLoader::unload()
  {
    free(m_file.ckHead);
    if (m_filename != NULL)m_filename = 0;
  }

  int SmfLoader::get_message(uint32 ntrack, uint32 position)
  {
    return 0;
  }

}; };