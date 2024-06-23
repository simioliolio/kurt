#include "core_audio_host.hpp"

#include <iostream>

namespace kurt {

CoreAudioHost::CoreAudioHost(std::unique_ptr<Kurt> kurt)
    : _kurt(std::move(kurt)) {};

CoreAudioHost::~CoreAudioHost() { stop(); }

bool CoreAudioHost::start() {

  assert(_audio_started == false);

  std::cout << "Finding output component..." << std::endl;

  // generate description that will match audio HAL
  AudioComponentDescription output_desc = {0};
  output_desc.componentType = kAudioUnitType_Output;
  output_desc.componentSubType = kAudioUnitSubType_DefaultOutput;
  output_desc.componentManufacturer = kAudioUnitManufacturer_Apple;

  AudioComponent output_component = AudioComponentFindNext(NULL, &output_desc);
  if (output_component == NULL) {
    std::cout << "Error: Could not find audio component" << std::endl;
    return false;
  }

  std::cout << "Found audio component" << std::endl;

  checkError(AudioComponentInstanceNew(output_component, &_output_audio_unit),
             "Instantiating the output audio unit");

  // Get sample rate for _output_audio_unit
  Float64 sample_rate;
  UInt32 sample_rate_size = sizeof(sample_rate);
  checkError(AudioUnitGetProperty(
                 _output_audio_unit, kAudioUnitProperty_SampleRate,
                 kAudioUnitScope_Output, 0, &sample_rate, &sample_rate_size),
             "Getting the sample rate from the output audio unit");

  if (sample_rate_size != 8) {
    std::cout << "Warning: Unexpected sample rate byte size: "
              << sample_rate_size << std::endl;
  }
  std::cout << "Sample rate: " << sample_rate << std::endl;

  // Set the stream format
  AudioStreamBasicDescription asbd;
  memset(&asbd, 0, sizeof(asbd));
  asbd.mSampleRate = sample_rate;
  asbd.mFormatID = kAudioFormatLinearPCM; // Linear PCM format
  asbd.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
  asbd.mFramesPerPacket = 1;
  asbd.mChannelsPerFrame = 2; // Stereo
  asbd.mBitsPerChannel = 32;
  asbd.mBytesPerPacket = asbd.mBytesPerFrame =
      (asbd.mBitsPerChannel / 8) * asbd.mChannelsPerFrame;
  asbd.mFramesPerPacket = 1;

  checkError(
      AudioUnitSetProperty(_output_audio_unit, kAudioUnitProperty_StreamFormat,
                           kAudioUnitScope_Input, 0, &asbd, sizeof(asbd)),
      "Setting the Output Audio Unit stream format");

  AURenderCallbackStruct input_callback;
  input_callback.inputProc = Callback;
  input_callback.inputProcRefCon = this;

  checkError(AudioUnitSetProperty(_output_audio_unit,
                                  kAudioUnitProperty_SetRenderCallback,
                                  kAudioUnitScope_Input, 0, &input_callback,
                                  sizeof(input_callback)),
             "Setting the Output Audio Unit property for rendering callback");

  checkError(AudioUnitInitialize(_output_audio_unit), "Audio Unit Initialize");

  checkError(AudioOutputUnitStart(_output_audio_unit),
             "Audio Output Unit Start");

  _audio_started = true;

  _kurt->play();

  return true;
}

bool CoreAudioHost::stop() {
  _kurt->stop();
  if (_audio_started) {
    checkError(AudioOutputUnitStop(_output_audio_unit),
               "Audio Output Unit Stop");
    checkError(AudioUnitUninitialize(_output_audio_unit),
               "Audio Unit Uninitialize");
    checkError(AudioComponentInstanceDispose(_output_audio_unit),
               "Audio Component Instance Dispose");
    _audio_started = false;
  }

  return true;
}

void CoreAudioHost::checkError(OSStatus error, const char *operation) {
  if (error == noErr)
    return;

  char str[20];
  // see if it appears to be a 4-char-code
  *(UInt32 *)(str + 1) = CFSwapInt32HostToBig(error);
  if (isprint(str[1]) && isprint(str[2]) && isprint(str[3]) &&
      isprint(str[4])) {
    str[0] = str[5] = '\'';
    str[6] = '\0';
  } else
    // no, format it as an integer
    printf(str, "%d", (int)error);

  fprintf(stderr, "Error: %s (%s)\n", operation, str);

  exit(1);
}

OSStatus CoreAudioHost::callback(void *inRefCon,
                                 AudioUnitRenderActionFlags *ioActionFlags,
                                 const AudioTimeStamp *inTimeStamp,
                                 UInt32 inBusNumber, UInt32 inNumberFrames,
                                 AudioBufferList *ioData) {
  Float32 *output = (Float32 *)ioData->mBuffers[0].mData;
  UInt16 number_of_channels = ioData->mBuffers[0].mNumberChannels;
  for (UInt32 frame = 0; frame < inNumberFrames; frame++) {
    const auto audio_frame = _kurt->next_frame();
    for (UInt16 channel = 0; channel < number_of_channels; channel++) {
      if (channel < audio_frame.size()) {
        (output)[(frame * number_of_channels) + channel] = audio_frame[channel];
      } else if (audio_frame.size() == 1) {
        // Mono
        (output)[(frame * number_of_channels) + channel] = audio_frame[0];
      } else {
        std::cout << "Warning: Unexpected audio_frame.size() == "
                  << audio_frame.size() << std::endl;
        (output)[(frame * number_of_channels) + channel] = 0.0f;
      }
    }
  }
  return noErr;
}

// static
OSStatus CoreAudioHost::Callback(void *inRefCon,
                                 AudioUnitRenderActionFlags *ioActionFlags,
                                 const AudioTimeStamp *inTimeStamp,
                                 UInt32 inBusNumber, UInt32 inNumberFrames,
                                 AudioBufferList *ioData) {
  CoreAudioHost *host = static_cast<CoreAudioHost *>(inRefCon);
  return host->callback(inRefCon, ioActionFlags, inTimeStamp, inBusNumber,
                        inNumberFrames, ioData);
}

} // namespace kurt