libsqlite3: libsqlite3-src libsqlite3-src/.libs/libsqlite3.a
libsqlite3-src:
	wget http://www.sqlite.org/sqlite-autoconf-3070900.tar.gz -O libsqlite3-src.tar.gz
	sha1sum -c libsqlite3-src.tar.gz.sha1
	mkdir libsqlite3-src
	tar -xf libsqlite3-src.tar.gz --strip-components=1 -C libsqlite3-src

libsqlite3-src/.libs/libsqlite3.a:
	cd libsqlite3-src/ && ./configure --disable-shared --prefix=/opt --host=arm-linux  --disable-dynamic-extensions CFLAGS="${CFLAGS_COMMON}"
	make -C libsqlite3-src