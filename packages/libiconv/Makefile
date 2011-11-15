libiconv: libiconv-src libiconv-src/lib/.libs/libiconv.a
libiconv-src:
	wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz -O libiconv-src.tar.gz
	sha1sum -c libiconv-src.tar.gz.sha1
	mkdir libiconv-src
	tar -xf libiconv-src.tar.gz --strip-components=1 -C libiconv-src

libiconv-src/lib/.libs/libiconv.a:
	cd libiconv-src/ && ./configure --disable-shared --enable-static --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libiconv-src

