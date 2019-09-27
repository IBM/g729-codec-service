project('g729-codec-service','c', 'cpp', version: '0.0.1', default_options : ['c_std=c17', 'cpp_std=c++17'])
usockets = subproject('usockets')
rapidjson = subproject('rapidjson')
uwebsockets = subproject('uwebsockets')
zlib = subproject('zlib')
bcg729 = subproject('bcg729')
spdlog = subproject('spdlog', default_options: ['library_type=shared'])

uwebsockets_dep = uwebsockets.get_variable('uwebsockets_dep')
librapidjson = rapidjson.get_variable('rapidjson_dep')
usockets_dep = usockets.get_variable('usockets_dep')
zlib_dep = zlib.get_variable('zlib_dep')
bcg729_dep = bcg729.get_variable('bcg729_dep')
spdlog_dep = spdlog.get_variable('spdlog_headeronly_dep')

# bcg729 = dependency('bcg729', method: 'cmake')
executable('g729-codec-service',
'src/AudioFrame.cpp',
'src/AudioFrame.hpp',
'src/Codec.hpp',
'src/G729Decoder.cpp',
'src/G729Decoder.hpp',
'src/G729Encoder.cpp',
'src/G729Encoder.hpp',
'src/G729ToMulaw.cpp',
'src/G729ToMulaw.hpp',
'src/MulawDecoder.cpp',
'src/MulawDecoder.hpp',
'src/MulawEncoder.cpp',
'src/MulawEncoder.hpp',
'src/MulawToG729.cpp',
'src/MulawToG729.cpp',
'src/g711u.c',
'src/g711u.h',
'src/g711_extern.h',
'src/bcg729-extern.h',
'src/G729CodecService.cpp',
'src/resource-messages.h',
'src/NetworkOptions.hpp',
'src/TLSOptions.hpp',
'src/LogUtils.hpp',
dependencies: [bcg729_dep, librapidjson, usockets_dep, uwebsockets_dep, zlib_dep, spdlog_dep],
install: true
)

# inc = include_directories('src')
# executable('tests',
# 'test/lib/catch.hpp',
# 'test/G729DecoderTest.cpp',
# 'src/AudioFrame.cpp',
# 'src/AudioFrame.hpp',
# 'src/Codec.hpp',
# 'src/G729Decoder.cpp',
# 'src/G729Decoder.hpp',
# 'src/G729Encoder.cpp',
# 'src/G729Encoder.hpp',
# 'src/G729ToMulaw.cpp',
# 'src/G729ToMulaw.hpp',
# 'src/MulawDecoder.cpp',
# 'src/MulawDecoder.hpp',
# 'src/MulawEncoder.cpp',
# 'src/MulawEncoder.hpp',
# 'src/MulawToG729.cpp',
# 'src/g711u.c',
# 'src/g711u.h',
# 'src/g711_extern.h',
# 'src/bcg729-extern.h',
# dependencies: [bcg729_dep, librapidjson, usockets_dep, uwebsockets_dep, zlib_dep, spdlog_dep],
# install: true,
# include_directories: inc
# )
