{
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "addon/hello_world.cc", "src/codec/*.cpp", "src/player/*.cpp" ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")", "src/codec", "src/player", "3rdparty/ffmpeg/include"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "libraries": [
        "/Users/linbinghe/Github/x-media/3rdparty/ffmpeg/lib/libavformat.dylib"
      ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      'cflags': ['-fexceptions'],
      'cflags_cc': ['-fexceptions'],
      "xcode_settings": {
      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
      "CLANG_CXX_LIBRARY": "libc++",
      "MACOSX_DEPLOYMENT_TARGET": "10.15"
    },
    }
  ]
}