// aaccodec.cc
// 2/13/2012
// See: liveMedia/ADTSAudioStreamSource.cc

#include "aaccodec.h"
#include "module/datastream/inputstream.h"
#include <QtCore>

#define DEBUG "aaccodec"
#include "module/debug/debug.h"

namespace { // anonymous
  unsigned const samplingFrequencyTable[16] = {
    96000, 88200, 64000, 48000,
    44100, 32000, 24000, 22050,
    16000, 12000, 11025, 8000,
    7350, 0, 0, 0
  };
} // anonymous namespace

AACInfo
AACCodec::parseAACInfo(InputStream *in)
{
  Q_ASSERT(in);
  DOUT("enter");
  in->reset();

  // Now, having opened the input file, read the fixed header of the first frame,
  // to get the audio stream's parameters:
  unsigned char fixedHeader[4]; // it's actually 3.5 bytes long
  if (in->read((char*)fixedHeader, sizeof(fixedHeader))
      != sizeof(fixedHeader)) {
    DOUT("exit: insufficient input size");
    return AACInfo();
  }

  // Check the 'syncword':
  if (!(fixedHeader[0] == 0xFF && (fixedHeader[1]&0xF0) == 0xF0)) {
    DOUT("exit: bad 'syncword' at start of ADTS file");
    return AACInfo();
  }

  // Get and check the 'profile':
  quint8 profile = (fixedHeader[2]&0xC0)>>6; // 2 bits
  if (profile == 3) {
    DOUT("exit: bad (reserved) 'profile': 3 in first frame of ADTS file");
    return AACInfo();
  }

  // Get and check the 'sampling_frequency_index':
  quint8 samplingFrequencyIndex = (fixedHeader[2]&0x3C)>>2; // 4 bits
  if (samplingFrequencyTable[samplingFrequencyIndex] == 0) {
    DOUT("exit: bad 'sampling_frequency_index' in first frame of ADTS file");
    return AACInfo();
  }

  // Get and check the 'channel_configuration':
  quint8 channelConfiguration
    = ((fixedHeader[2]&0x01)<<2)|((fixedHeader[3]&0xC0)>>6); // 3 bits

  // If we get here, the frame header was OK.
  // Reset the fid to the beginning of the file:
  in->reset();

  uint fSamplingFrequency = samplingFrequencyTable[samplingFrequencyIndex];
  uint fNumChannels = channelConfiguration == 0 ? 2 : channelConfiguration;
  //uint fuSecsPerFrame
  //  = (1024/*samples-per-frame*/*1000000) / fSamplingFrequency/*samples-per-second*/;

  AACInfo ret;

  // Construct the 'AudioSpecificConfig', and from it, the corresponding ASCII string:
  //unsigned char audioSpecificConfig[2];
  quint8 const audioObjectType = profile + 1;
  ret.config[0] = (audioObjectType<<3) | (samplingFrequencyIndex>>1);
  ret.config[1] = (samplingFrequencyIndex<<7) | (channelConfiguration<<3);
  //sprintf(fConfigStr, "%02X%02x", audioSpecificConfig[0], audioSpecificConfig[1]);

  ret.frequency = fSamplingFrequency;
  ret.channels = fNumChannels;
  ret.profile = profile;
  DOUT("exit");
  return ret;
}

