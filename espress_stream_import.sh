#!/bin/bash

if [ ${#} -lt 1 ]
then
	orgdir="../../../espress_stream"
else
	orgdir=${1}
fi

impdirs="include/espress_stream"
impdirs="${impdirs} src"
impdirs="${impdirs} api/include/espress_stream"
impdirs="${impdirs} api/src"

depends="espress_stream/config.h"
depends="${depends} espress_stream/depend.h"
depends="${depends} espress_stream/dgram.h"
depends="${depends} espress_stream/tick.h"
depends="${depends} espress_stream/rand.h"
depends="${depends} espress_stream_dgram.c"
depends="${depends} espress_stream_tick.c"
depends="${depends} espress_stream_rand.c"
depends="${depends} udp.h"
depends="${depends} tick.h"
depends="${depends} rand.h"
depends="${depends} udp.cpp"
depends="${depends} tick.cpp"
depends="${depends} rand.cpp"
depends="${depends} espress_stream/api_depend.h"

for f in ${depends}
do
	mv src/${f} src/${f}.bak
done

for d in ${impdirs}
do
	find ${orgdir}/${d} -name '*.h' | xargs -ri basename {} | xargs -ri rm -f src/espress_stream/{}
	find ${orgdir}/${d} -name '*.c' | xargs -ri basename {} | xargs -ri rm -f src/{}
done

for d in ${impdirs}
do
	find ${orgdir}/${d} -name '*.h' | xargs -ri cp {} src/espress_stream/
	find ${orgdir}/${d} -name '*.c' | xargs -ri cp {} src/
done

for f in ${depends}
do
	mv -f src/${f}.bak src/${f}
done

MAJV=`cat ${orgdir}/arch/linux/Makefile | grep -E '^(TARGET_MAJOR_VER)[^a-z,A-Z,0-9,_]+[0-9]+$' | sed -E 's/[a-z,A-Z,0-9,_]+[^a-z,A-Z,0-9,_]+([0-9]+)$/\1/'`
MINV=`cat ${orgdir}/arch/linux/Makefile | grep -E '^(TARGET_MINOR_VER)[^a-z,A-Z,0-9,_]+[0-9]+$' | sed -E 's/[a-z,A-Z,0-9,_]+[^a-z,A-Z,0-9,_]+([0-9]+)$/\1/'`
PROV=`cat ${orgdir}/arch/linux/Makefile | grep -E '^(TARGET_PROTOCOL_VER)[^a-z,A-Z,0-9,_]+[0-9]+$' | sed -E 's/[a-z,A-Z,0-9,_]+[^a-z,A-Z,0-9,_]+([0-9]+)$/\1/'`

cat src/espress_stream/config.h | \
	sed -E "s/^(#define[^a-z,A-Z,0-9,_]*)(ESPRESS_STREAM_MAJOR_VERSION)[^a-z,A-Z,0-9,_]+[0-9]+$/\1\2 ${MAJV}/" | \
	sed -E "s/^(#define[^a-z,A-Z,0-9,_]*)(ESPRESS_STREAM_MINOR_VERSION)[^a-z,A-Z,0-9,_]+[0-9]+$/\1\2 ${MINV}/" | \
	sed -E "s/^(#define[^a-z,A-Z,0-9,_]*)(ESPRESS_STREAM_PROTOCOL_VERSION)[^a-z,A-Z,0-9,_]+[0-9]+$/\1\2 ${PROV}/" | \
	cat > src/espress_stream/config.new

mv -f src/espress_stream/config.new src/espress_stream/config.h

