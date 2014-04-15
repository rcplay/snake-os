TARGET=openssl

$(TARGET): $(TARGET)-src $(TARGET)-src/src/.libs/libevent.a
$(TARGET)-src:
	wget http://www.openssl.org/source/openssl-1.0.1g.tar.gz -O $(TARGET)-src.tar.gz
	sha1sum -c $(TARGET)-src.tar.gz.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src

$(TARGET)-src/src/.libs/libevent.a:
	cd $(TARGET)-src/ && ./Configure linux-armv4 -march=armv4
	make -C $(TARGET)-src CC="arm-linux-gcc" AR="arm-linux-ar r" RANLIB="arm-linux-ranlib"
