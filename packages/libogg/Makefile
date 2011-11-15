libogg: libogg-src libogg-src/src/.libs/libogg.a
libogg-src:
	wget http://downloads.xiph.org/releases/ogg/libogg-1.3.0.tar.gz -O libogg-src.tar.gz
	sha1sum -c libogg-src.tar.gz.sha1
	mkdir libogg-src
	tar -xf libogg-src.tar.gz --strip-components=1 -C libogg-src

libogg-src/src/.libs/libogg.a:
	cd libogg-src/ && ./configure --disable-shared --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libogg-src
