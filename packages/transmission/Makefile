
TARGET=transmission

$TARGET: $(TARGET)-src transmission-src/daemon/transmission-daemon $(TARGET).opk

$(TARGET)-src:
	wget http://download.transmissionbt.com/files/transmission-2.60.tar.bz2 -c -O $(TARGET)-src.tar.bz2
	sha1sum -c $(TARGET)-src.tar.bz2.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.bz2 --strip-components=1 -C $(TARGET)-src

transmission-src/daemon/transmission-daemon:
	cd transmission-src ; \
	./configure \
		--host=arm-linux CC=arm-linux-gcc CXX=arm-linux-g++ --prefix=/opt/ CFLAGS="${CFLAGS_COMMON}" CXXFLAGS="${CFLAGS_COMMON}" \
        LIBCURL_LIBS="-L${TOPDIR}/libcurl-ssl/libcurl-src/lib/.libs -lcurl"  \
        LIBCURL_CFLAGS=-I${TOPDIR}/libcurl-ssl/libcurl-src/include \
        OPENSSL_LIBS="-L${TOPDIR}/openssl/openssl-src/ -lssl -lcrypto" \
        OPENSSL_CFLAGS=-I${TOPDIR}/openssl/openssl-src/include \
        LIBEVENT_LIBS="-L${TOPDIR}/libevent/libevent-src/.libs -levent" \
        LIBEVENT_CFLAGS=-I${TOPDIR}/libevent/libevent-src/include \
		--host=i686-linux-gnu --target=arm-linux-gnu --enable-static=yes --enable-shared=no \
		--disable-dependency-tracking  \
		--disable-libtool-lock  \
		--without-gtk        \
		--disable-libnotify  \
		--disable-nls        \
		--disable-cli        \
		--disable-mac        \
		--enable-daemon      \
		--enable-lightweight   
	make PREFIX=/opt/ CONFIG_PREFIX=/opt/ -j3 -C transmission-src


$(TARGET).opk:
	cp transmission-src/daemon/transmission-daemon data/bin/
	cp transmission-src/daemon/transmission-remote data/bin/
	cp -a transmission-src/web data/share/transmission/	

	chown -R root:root data/*
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control/*
	chmod +x control/prerm control/postinst

	cd control; tar --exclude-vcs -zcf ../control.tar.gz *
	ar -r transmission.opk data.tar.gz control.tar.gz debian-binary

clean:	
	make -C transmission-src clean
