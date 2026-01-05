{
  "targets": [
    {
      "target_name": "smbios",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "cflags_cc": [ "-std=c++17" ],
      "sources": [
        "src/binding.cpp",
        "src/smbios_common.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(node -p \"require('@mapbox/node-pre-gyp').include\")"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "conditions": [
        ["OS=='win'", {
          "sources": [ "src/windows/smbios_windows.cpp" ],
          "libraries": [],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
              "AdditionalOptions": [ "/std:c++17" ]
            }
          }
        }],
        ["OS=='mac'", {
          "sources": [ "src/mac/smbios_macos.cpp" ],
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LANGUAGE_STANDARD": "c++17",
            "CLANG_CXX_LIBRARY": "libc++",
            "MACOSX_DEPLOYMENT_TARGET": "10.13"
          },
          "link_settings": {
            "libraries": [
              "-framework IOKit",
              "-framework CoreFoundation"
            ]
          }
        }],
        ["OS=='linux'", {
          "sources": [ "src/linux/smbios_linux.cpp" ]
        }]
      ]
    }
  ]
}
