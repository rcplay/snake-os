libexif: libexif-src libexif-src/libexif/.libs/libexif.a
libexif-src:
	wget http://sourceforge.net/projects/libexif/files/libexif/0.6.20/libexif-0.6.20.tar.gz/download -O libexif-src.tar.gz
	sha1sum -c libexif-src.tar.gz.sha1
	mkdir libexif-src
	tar -xf libexif-src.tar.gz --strip-components=1 -C libexif-src

libexif-src/libexif/.libs/libexif.a:
	cd libexif-src/ && ./configure --disable-shared --disable-nls --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libexif-src