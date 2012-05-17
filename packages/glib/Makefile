glib: glib-src glib-src/glib/.libs/libglib-2.0.a
glib-src:
	wget -c ftp://ftp.gtk.org/pub/glib/2.32/glib-2.32.1.tar.xz -O glib-src.tar.xz
	sha1sum -c glib-src.tar.xz.sha1
	mkdir glib-src
	tar -xf glib-src.tar.xz --strip-components=1 -C glib-src

glib-src/glib/.libs/libglib-2.0.a:
	cd glib-src/ && ./configure --disable-shared --enable-static --host=arm-linux CFLAGS="${CFLAGS_COMMON}" \
		glib_cv_stack_grows=no glib_cv_uscore=no ac_cv_func_posix_getpwuid_r=yes ac_cv_func_posix_getgrgid_r=yes \
		LDFLAGS="-L${TOPDIR}/libiconv/libiconv-src/lib/.libs/ -L${TOPDIR}/gettext/gettext-src/gettext-runtime/intl/.libs/" \
		CPPFLAGS="-I${TOPDIR}/libiconv/libiconv-src/include/ -I${TOPDIR}/gettext/gettext-src/gettext-runtime/intl/" \
		PKG_CONFIG_LIBDIR=/ \
		LIBFFI_LIBS="-L${TOPDIR}/libffi/libffi-src/arm-unknown-linux-gnu/.libs -lffi" LIBFFI_CFLAGS="-I${TOPDIR}/libffi/libffi-src/arm-unknown-linux-gnu/include" \
		LIBS="-L${TOPDIR}/libffi/libffi-src/arm-unknown-linux-gnu/.libs/ -lffi"
	-make -C glib-src # tests will fail but libs are done at that point

