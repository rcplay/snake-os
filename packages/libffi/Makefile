libffi: libffi-src libffi-src/arm-unknown-linux-gnu/.libs/libffi.a
libffi-src:
	wget ftp://sourceware.org/pub/libffi/libffi-3.0.11.tar.gz -O libffi-src.tar.gz
	sha1sum -c libffi-src.tar.gz.sha1
	mkdir libffi-src
	tar -xf libffi-src.tar.gz --strip-components=1 -C libffi-src

libffi-src/arm-unknown-linux-gnu/.libs/libffi.a:
	cd libffi-src/ && ./configure --disable-shared --enable-static --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libffi-src

