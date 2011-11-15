libFLAC: libFLAC-src libFLAC-src/src/libFLAC/.libs/libFLAC.a
libFLAC-src:
	wget http://sourceforge.net/projects/flac/files/flac-src/flac-1.2.1-src/flac-1.2.1.tar.gz/download -O libFLAC-src.tar.gz
	sha1sum -c libFLAC-src.tar.gz.sha1
	mkdir libFLAC-src
	tar -xf libFLAC-src.tar.gz --strip-components=1 -C libFLAC-src

libFLAC-src/src/libFLAC/.libs/libFLAC.a:
	cd libFLAC-src/ && ./configure --disable-shared --prefix=/opt --host=arm-linux --disable-xmms-plugin --disable-cpplibs CFLAGS="${CFLAGS_COMMON}" CPPFLAGS="-I${TOPDIR}/libogg/libogg-src/include/" LDFLAGS=" -L${TOPDIR}/libogg/libogg-src/src/.libs/" 
	make -C libFLAC-src
