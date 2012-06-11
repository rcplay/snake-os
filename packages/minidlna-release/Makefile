
all: minidlna-release.opk

minidlna-src:
	wget http://sourceforge.net/projects/minidlna/files/minidlna/1.0.24/minidlna_1.0.24_src.tar.gz/download -O minidlna-src.tar.gz
	sha1sum -c minidlna-src.tar.gz.sha1
	mkdir minidlna-src
	tar -xf minidlna-src.tar.gz --strip-components=1 -C minidlna-src
	patch -p0 < minidlna-libcheck.patch

minidlna-src/minidlna: minidlna-src
	cp config.h minidlna-src/
	make -C minidlna-src CC="arm-linux-gcc" \
		CFLAGS=" -Wall -s -Os -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -DSTATIC \
			-DTIVO_SUPPORT -DHAVE_INOTIFY \
			-DDB_PATH=\"/opt/var/cache/minidlna\" -DLOG_PATH=\"/opt/var/log/\" \
			-I${TOPDIR}/libjpeg/libjpeg-src/ \
			-I${TOPDIR}/libexif/libexif-src/ \
			-I${TOPDIR}/libogg/libogg-src/include/ \
			-I${TOPDIR}/libvorbis/libvorbis-src/include/ \
			-I${TOPDIR}/libsqlite3/libsqlite3-src/ \
			-I${TOPDIR}/libFLAC/libFLAC-src/include/ \
			-I${TOPDIR}/libid3tag/libid3tag-src/ \
			-I${TOPDIR}/libiconv/libiconv-src/include/ \
			-I${TOPDIR}/ffmpeg/ffmpeg-src/ -I${TOPDIR}/ffmpeg/ffmpeg-src/libavformat/ -I${TOPDIR}/ffmpeg/ffmpeg-src/libavutil/ -I${TOPDIR}/ffmpeg/ffmpeg-src/libavcodec/" \
		LIBS="-lvorbis -logg -lsqlite3 -lpthread -lexif -ljpeg -lFLAC -lid3tag -lz -lavformat -lavcodec -lavutil -liconv -lm" \
		LDFLAGS=" -static \
			-L${TOPDIR}/libjpeg/libjpeg-src/.libs/ \
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

minidlna-release.opk: minidlna-src/minidlna
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
	ar -r minidlna-release.opk data.tar.gz control.tar.gz debian-binary
