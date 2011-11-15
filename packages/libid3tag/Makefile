libid3tag: libid3tag-src libid3tag-src/.libs/libid3tag.a
libid3tag-src:
	wget ftp://ftp.mars.org/pub/mpeg/libid3tag-0.15.1b.tar.gz -O libid3tag-src.tar.gz
	sha1sum -c libid3tag-src.tar.gz.sha1
	mkdir libid3tag-src
	tar -xf libid3tag-src.tar.gz --strip-components=1 -C libid3tag-src

libid3tag-src/.libs/libid3tag.a:
	cd libid3tag-src/ && ./configure --disable-shared --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libid3tag-src
