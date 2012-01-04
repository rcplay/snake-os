
VERSION=cvs
BUILDVER=1

all: minidlna-src/minidlna minidlna.opk

minidlna-src/minidlna:
	cd minidlna-src && ./configure --host=arm-linux --with-staticbin --disable-nls CFLAGS="${CFLAGS_COMMON} -static-libgcc -DHAVE_INOTIFY" \
		--with-logpath=/opt/var/log/ --with-dbpath=/opt/var/cache/minidlna --with-osname="SnakeOS"  --with-osver="1.3.2" --with-osurl="http://code.google.com/p/snake-os/" --with-tivo \
		CPPFLAGS=" -I${TOPDIR}/libjpeg/libjpeg-src/ \
			-I${TOPDIR}/libexif/libexif-src/ \
			-I${TOPDIR}/libogg/libogg-src/include/ \
			-I${TOPDIR}/libvorbis/libvorbis-src/include/ \
			-I${TOPDIR}/libsqlite3/libsqlite3-src/ \
			-I${TOPDIR}/libFLAC/libFLAC-src/include/ \
			-I${TOPDIR}/libid3tag/libid3tag-src/ \
			-I${TOPDIR}/libiconv/libiconv-src/include/ \
			-I${TOPDIR}/ffmpeg/ffmpeg-src/" \
		LIBS="-lz -lm -lpthread -lavcodec -lavutil -liconv -logg" \
		LDFLAGS=" -L${TOPDIR}/libjpeg/libjpeg-src/.libs/ \
			-L${TOPDIR}/libexif/libexif-src/libexif/.libs/ \
			-L${TOPDIR}/libogg/libogg-src/src/.libs/ \
			-L${TOPDIR}/libvorbis/libvorbis-src/lib/.libs/ \
			-L${TOPDIR}/libsqlite3/libsqlite3-src/.libs/ \
			-L${TOPDIR}/libFLAC/libFLAC-src/src/libFLAC/.libs/ \
			-L${TOPDIR}/libid3tag/libid3tag-src/.libs/ \
			-L${TOPDIR}/libiconv/libiconv-src/lib/.libs/ \
			-L${TOPDIR}/ffmpeg/ffmpeg-src/libavformat/ \
			-L${TOPDIR}/ffmpeg/ffmpeg-src/libavutil/ \
			-L${TOPDIR}/ffmpeg/ffmpeg-src/libavcodec/"
	make -C minidlna-src

minidlna.opk:
	cp minidlna-src/minidlna data/bin/
	chmod -R 755 data/bin/minidlna
	cp minidlna-src/minidlna.conf data/etc/
	chmod -R 644 data/etc/minidlna.conf
	chmod -R 755 data/etc/init.d/minidlna

	chown -R root:root data
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r minidlna.opk data.tar.gz control.tar.gz debian-binary
