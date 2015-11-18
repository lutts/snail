#!/bin/sh

OURROOT=$PWD

cd /

# replace "fto::" with "", replace "fto_" with ""
find $OURROOT \
  -name build -prune -o \
  -name bin -prune -o \
  -name "moc_*" -prune -o \
  -name thirdparty -prune -o \
  -name test -prune -o \
  -type f \( -name '*.c' -o -name '*.cc' -o -name '*.cxx' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
  -print | while read file; do
  filename=${file##*/}
  sed -i -e "/$filename/!s/fto_//" -e 's/fto:://' $file
done

cd $OURROOT

# revert include/config.h
git checkout include/config.h

sed -i -e '/DISABLE_TEST_CODE/s/^#//' -e 's/add_subdirectory(test)/# add_subdirectory(test)/' CMakeLists.txt

make

