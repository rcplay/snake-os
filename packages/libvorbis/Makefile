# libvorbis
libvorbis: libvorbis-src libvorbis-src/lib/.libs/libvorbis.a
libvorbis-src:
	wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.2.tar.gz -O libvorbis-src.tar.gz
	sha1sum -c libvorbis-src.tar.gz.sha1
	mkdir libvorbis-src
	tar -xf libvorbis-src.tar.gz --strip-components=1 -C libvorbis-src

libvorbis-src/lib/.libs/libvorbis.a:
	cd libvorbis-src/ && ./configure --disable-shared --disable-oggtest --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}" CPPFLAGS="-I${TOPDIR}/libogg/libogg-src/include/" LDLAGS=" -L${TOPDIR}/libogg/libogg-src/src/.libs/" 
	make -C libvorbis-src