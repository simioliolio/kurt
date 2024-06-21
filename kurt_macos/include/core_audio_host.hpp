#include <AudioToolbox/AudioToolbox.h>
#include <memory>

#include "kurt.hpp"

namespace kurt {

class CoreAudioHost {
public:
  CoreAudioHost(std::unique_ptr<Kurt> kurt);
  ~CoreAudioHost();

  bool start();
  bool stop();

private:
  std::unique_ptr<Kurt> _kurt;
  AudioUnit _output_audio_unit;
  bool _audio_started = false;
  void checkError(OSStatus error, const char *operation);

  OSStatus callback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags,
                    const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber,
                    UInt32 inNumberFrames, AudioBufferList *ioData);

  static OSStatus Callback(void *inRefCon,
                           AudioUnitRenderActionFlags *ioActionFlags,
                           const AudioTimeStamp *inTimeStamp,
                           UInt32 inBusNumber, UInt32 inNumberFrames,
                           AudioBufferList *ioData);
};

} // namespace kurt
