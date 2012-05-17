libmad: libmad-src libmad-src/src/.libs/libmad.a
libmad-src:
	wget http://sourceforge.net/projects/mad/files/libmad/0.15.1b/libmad-0.15.1b.tar.gz/download  -O libmad-src.tar.gz
	sha1sum -c libmad-src.tar.gz.sha1
	mkdir libmad-src
	tar -xf libmad-src.tar.gz --strip-components=1 -C libmad-src

libmad-src/src/.libs/libmad.a:
	cd libmad-src/ && ./configure --disable-shared --enable-static --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libmad-src
