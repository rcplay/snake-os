pcre: dest/opt/lib/libpcre.a

pcre-src:
	wget ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.35.tar.gz -O pcre-src.tar.gz
	sha1sum -c pcre-src.tar.gz.sha1
	mkdir pcre-src
	tar -xf pcre-src.tar.gz --strip-components=1 -C pcre-src

dest/opt/lib/libpcre.a: pcre-src
	cd pcre-src/ && ./configure --prefix=${TOPDIR}/pcre/inst --host=arm-linux --disable-cpp CFLAGS="${CFLAGS_COMMON}"
	make -C pcre-src
	make -C pcre-src install
