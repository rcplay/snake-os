libcurl: libcurl-src libcurl-src/lib/.libs/libcurl.a
libcurl-src:
	wget http://curl.haxx.se/download/curl-7.27.0.tar.gz -O libcurl-src.tar.gz
	sha1sum -c libcurl-src.tar.gz.sha1
	mkdir libcurl-src
	tar -xf libcurl-src.tar.gz --strip-components=1 -C libcurl-src

libcurl-src/lib/.libs/libcurl.a:
	cd libcurl-src/ && ./configure --disable-shared --enable-static --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}" --disable-manual --without-random --disable-verbose --disable-debug --disable-cookies --disable-dict --disable-sspi --disable-rtsp --disable-gopher --disable-telnet --disable-tftp  --disable-pop3 --disable-imap --disable-smtp HAVECRYPTO="yes" OPENSSL_ENABLED="1" CPPFLAGS="-I${TOPDIR}/openssl/openssl-src/include/ -I${TOPDIR}/openssl/openssl-src/include/openssl/" LDFLAGS="-L${TOPDIR}/openssl/openssl-src/ -lcrypto -lssl -ldl " --with-ssl=${TOPDIR}/openssl/openssl-src/
	make -C libcurl-src

