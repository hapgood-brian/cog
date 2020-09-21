OS=`uname`
rm cmake_install.cmake 2> /dev/null
rm config.h 2> /dev/null
rm CMakeCache.txt 2> /dev/null
rm Makefile 2> /dev/null
rm -rf build 2> /dev/null
rm -rf CMakeFiles 2> /dev/null
rm -rf usr 2> /dev/null
rm -rf src 2> /dev/null
rm -rf pal 2> /dev/null
rm -rf share 2> /dev/null
rm -rf .output 2> /dev/null
rm -rf ../tmp 2> /dev/null
[ "$OS" == 'Darwin' ] && {
  rm -rf cog.xcodeproj 2> /dev/null
  rm -rf CMakeScripts 2> /dev/null
  rm -rf cog.build 2> /dev/null
} || [ "$OS" == 'Linux' ] && {
  rm cog.workspace 2> /dev/null
  rm cog.project 2> /dev/null
  rm build.ninja 2> /dev/null
  rm -rf .ninja_deps 2> /dev/null
  rm -rf .ninja_log 2> /dev/null
} || {
  rm *.vcxproj
  rm *.vcxproj.filters
  rm *.vcxproj.user
  rm *.sln
  rm -rf Debug
  rm -rf Release
  rm -rf MinSizeRel
  rm -rf RelWithDebInfo
  rm -rf x64
  rm -rf .vs
}