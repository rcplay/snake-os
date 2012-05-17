TARGET=mpd

$(TARGET): $(TARGET)-src mpd-src/src/mpd $(TARGET).opk
$(TARGET)-src:
	wget http://sourceforge.net/projects/musicpd/files/mpd/0.16.8/mpd-0.16.8.tar.gz/download -c -O $(TARGET)-src.tar.gz
	sha1sum -c $(TARGET)-src.tar.gz.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src
#	patch -p0 < samba-defaults.diff

$(TARGET)-src/src/mpd:
	cd mpd-src && OGG_CFLAGS="-I${TOPDIR}/libogg/libogg-src/include/" OGG_LIBS="-L${TOPDIR}/libogg/libogg-src/src/.libs/ -logg" FLAC_LIBS="-L${TOPDIR}/libFLAC/libFLAC-src/src/libFLAC/.libs/ -lFLAC" FLAC_CFLAGS="-I${TOPDIR}/libFLAC/libFLAC-src/include/" ID3TAG_LIBS="-L${TOPDIR}/libid3tag/libid3tag-src/.libs/ -lid3tag -lz" ID3TAG_CFLAGS="-I${TOPDIR}/libid3tag/libid3tag-src/" CURL_LIBS="-L${TOPDIR}/libcurl/libcurl-src/lib/.libs/ -lcurl" CURL_CFLAGS="-I${TOPDIR}/libcurl/libcurl-src/include/"\
		ac_cv_func_inotify_init=yes ac_cv_lib_vorbisidec_ov_read=yes ./configure --prefix=/opt --host=arm-linux --with-tremor=${TOPDIR}/tremor/output/ \
		CFLAGS="${CFLAGS_COMMON}" CPPFLAGS="-I${TOPDIR}/mpd/inotify/" CXXFLAGS="${CFLAGS_COMMON}" \
		LDFLAGS="-static" \
		PKG_CONFIG_LIBDIR=${TOPDIR}/libFLAC/libFLAC-src/src/libFLAC/:${TOPDIR}/libogg/libogg-src \
		GLIB_LIBS="-L${TOPDIR}/glib/glib-src/glib/.libs/ -lglib-2.0 \
			-L${TOPDIR}/glib/glib-src/gthread/.libs/ -lgthread-2.0 -lpthread \
			-L${TOPDIR}/libiconv/libiconv-src/lib/.libs/ -liconv \
			-L${TOPDIR}/gettext/gettext-src/gettext-runtime/intl/.libs/ -lintl" \
		GLIB_CFLAGS="-I${TOPDIR}/glib/glib-src/glib/ -I${TOPDIR}/glib/glib-src/" \
		MAD_LIBS=" -L${TOPDIR}/libmad/libmad-src/.libs/ -lmad" \
		MAD_CFLAGS="-I${TOPDIR}/libmad/libmad-src/" \
		SQLITE_LIBS="-L${TOPDIR}/libsqlite3/libsqlite3-src/.libs/ -lsqlite3" \
		SQLITE_CFLAGS="-I${TOPDIR}/libsqlite3/libsqlite3-src/"
	make -C mpd-src/

$(TARGET).opk:
	cp $(TARGET)-src/src/mpd data/bin/
	chmod -R 755 data/bin/*

	chmod -R 755 data/etc/init.d/mpd

	chown -R root:root data
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r $(TARGET).opk data.tar.gz control.tar.gz debian-binary
