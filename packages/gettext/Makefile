gettext: gettext-src gettext-src/gettext-runtime/intl/.libs/libintl.a
gettext-src:
	wget -c http://ftp.gnu.org/gnu/gettext/gettext-0.18.1.1.tar.gz -O gettext-src.tar.gz
	sha1sum -c gettext-src.tar.gz.sha1
	mkdir gettext-src
	tar -xf gettext-src.tar.gz --strip-components=1 -C gettext-src

gettext-src/gettext-runtime/intl/.libs/libintl.a:
	cd gettext-src/gettext-runtime/ && ./configure --disable-shared --enable-static --prefix=/opt --disable-java --disable-libasprintf --host=arm-linux CFLAGS="${CFLAGS_COMMON}"
	make -C gettext-src/gettext-runtime

