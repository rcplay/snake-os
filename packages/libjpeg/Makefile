all: libjpeg-src libjpeg-src/.libs/libjpeg.a

libjpeg-src:
	wget http://www.ijg.org/files/jpegsrc.v8d.tar.gz -O libjpeg-src.tar.gz
	sha1sum -c libjpeg-src.tar.gz.sha1
	mkdir libjpeg-src
	tar -xf libjpeg-src.tar.gz --strip-components=1 -C libjpeg-src

libjpeg-src/.libs/libjpeg.a:
	cd libjpeg-src/ && ./configure --disable-shared --prefix=/opt --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C libjpeg-src