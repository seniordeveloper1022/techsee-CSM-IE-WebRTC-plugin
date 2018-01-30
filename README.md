# CSM-IE-WebRTC-plugin
CoSMo Open Source Webrtc Plugin for IE 11

This code requires a 32 bits build of libwebrtc branch head 65. Corresponding installer will be provided soon.

For those familiar with the GN build system,and already knwo how to build a specific branch head, here are the arguments we pass to the "gn gen" command.

target_cpu = "x86"
treat_warnings_as_errors=false
disable_libfuzzer=true
is_win_fastlink=true
msvc_use_absolute_paths=true
symbol_level=2
use_rtti=true

For the vast majority of others, in the mean time, one can find a precompiled Debug version of the plugin for testing purpposes [here](https://drive.google.com/drive/folders/1Gkcg_94VnM0h84ZCJH9XbFyKRSm65wqb?usp=sharing)

You need to register the plugin as follows:
regsvr32 /i WebRTCPlugin.dll
