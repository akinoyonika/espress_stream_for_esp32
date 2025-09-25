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
