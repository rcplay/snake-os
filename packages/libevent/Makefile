TARGET=libevent

$(TARGET): $(TARGET)-src $(TARGET)-src/src/.libs/libevent.a
$(TARGET)-src:
	wget https://github.com/downloads/libevent/libevent/libevent-2.0.21-stable.tar.gz -O $(TARGET)-src.tar.gz
	sha1sum -c $(TARGET)-src.tar.gz.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src

$(TARGET)-src/src/.libs/libevent.a:
	cd $(TARGET)-src/ && ./configure --disable-shared --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C $(TARGET)-src
