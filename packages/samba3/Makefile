TARGET=samba3

$(TARGET): $(TARGET)-src samba3-src/source3/bin/smbd $(TARGET).opk
$(TARGET)-src:
	wget http://ftp.samba.org/pub/samba/samba-3.6.3.tar.gz -c -O $(TARGET)-src.tar.gz
	sha1sum -c $(TARGET)-src.tar.gz.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src
	patch -p0 < samba-defaults.diff

samba3-src/source3/bin/smbd:
	cd samba3-src/source3 ; \
	ac_cv_sizeof_int=4 ac_cv_sizeof_long=4 ac_cv_sizeof_short=2 \
	samba_cv_FTRUNCATE_NEEDS_ROOT=no samba_cv_HAVE_BROKEN_FCNTL64_LOCKS=no \
	samba_cv_HAVE_BROKEN_GETGROUPS=no samba_cv_HAVE_BROKEN_READDIR=no \
	samba_cv_HAVE_FCNTL_LOCK=yes samba_cv_HAVE_FNMATCH=yes \
	samba_cv_HAVE_FTRUNCATE_EXTEND=no samba_cv_HAVE_IFACE_AIX=no \
	samba_cv_HAVE_IFACE_IFCONF=yes samba_cv_HAVE_IFACE_IFREQ=yes \
	samba_cv_HAVE_INO64_T=yes samba_cv_HAVE_IRIX_SPECIFIC_CAPABILITIES=no \
	samba_cv_HAVE_OFF64_T=yes samba_cv_HAVE_ROOT=yes \
	samba_cv_HAVE_SECURE_MKSTEMP=yes samba_cv_HAVE_SHARED_MMAP=yes \
	samba_cv_HAVE_STRUCT_FLOCK64=yes samba_cv_HAVE_SYSV_IPC=no \
	samba_cv_HAVE_TRUNCATED_SALT=no samba_cv_HAVE_UNION_SEMUN=no \
	samba_cv_HAVE_UNSIGNED_CHAR=yes samba_cv_NEED_SGI_SEMUN_HACK=no \
	samba_cv_REPLACE_INET_NTOA=no samba_cv_SIZEOF_INO_T=4 \
	samba_cv_SIZEOF_OFF_T=4 samba_cv_SYSCONF_SC_NGROUPS_MAX=yes \
	samba_cv_USE_SETRESUID=no samba_cv_USE_SETREUID=yes \
	samba_cv_USE_SETEUID=yes samba_cv_USE_SETUIDX=no \
	samba_cv_have_longlong=yes samba_cv_have_setresgid=no \
	samba_cv_have_setresuid=no samba_cv_HAVE_GETTIMEOFDAY_TZ=yes \
	samba_cv_CC_NEGATIVE_ENUM_VALUES=yes libreplace_cv_HAVE_GETADDRINFO=no \
	ac_cv_file__proc_sys_kernel_core_pattern=yes \
	CFLAGS=" -pipe -Os -s -march=armv4 " \
	CPPFLAGS="-DNDEBUG -Dfcntl=fcntl64 -DHAVE_IFACE_IFCONF=1" \
	CC=arm-linux-gcc LD=arm-linux-ld AR=arm-linux-ar RANLIB=arm-linux-ranlib ./configure --host=arm-linux \
	--prefix=/bin --bindir=/bin --sbindir=/bin --sharedstatedir=/var --localstatedir=/var --sysconfdir=/etc --with-privatedir=/etc \
	--with-lockdir=/var --with-piddir=/var --libexecdir=/bin --with-logfilebase=/var --libdir=/etc --with-configdir=/etc \
	--without-syslog --disable-cups --with-sendfile-support --without-winbind --disable-external-libtalloc --disable-pie --disable-shared


	make -j3 -C samba3-src/source3 

$(TARGET).opk:
	cp $(TARGET)-src/source3/bin/smbd data/bin/smbd3
	cp $(TARGET)-src/source3/bin/nmbd data/bin/nmbd3
	chmod -R 755 data/bin/*

	chmod -R 755 data/etc/init.d/samba3

	chown -R root:root data
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r $(TARGET).opk data.tar.gz control.tar.gz debian-binary
