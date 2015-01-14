
all: apache.opk

httpd-src:
	wget http://mirror.softaculous.com/apache/httpd/httpd-2.4.10.tar.gz -O httpd-src.tar.gz
	sha1sum -c httpd-src.tar.gz.sha1
	wget http://mirror.softaculous.com/apache/httpd/httpd-2.4.10-deps.tar.gz -O httpd-deps-src.tar.gz
	sha1sum -c httpd-deps-src.tar.gz.sha1
	mkdir httpd-src
	tar -xf httpd-src.tar.gz --strip-components=1 -C httpd-src
	tar -xf httpd-deps-src.tar.gz --strip-components=1 -C httpd-src
	patch -p1 -d httpd-src < httpd-cross.patch

httpd-src/dest/bin/httpd: httpd-src
	cd httpd-src ; ./configure --prefix=/opt/apache --host=arm-linux --with-included-apr --with-pcre=${TOPDIR}/pcre/inst/ CFLAGS="${CFLAGS_COMMON}" LDFLAGS="-lpthread" ac_cv_file__dev_zero=yes ac_cv_func_setpgrp_void=yes apr_cv_tcp_nodelay_with_cork=yes ac_cv_sizeof_struct_iovec=8 ap_cv_void_ptr_lt_long=no ac_cv_define_PTHREAD_PROCESS_SHARED=no apr_cv_mutex_robust_shared=no ac_cv_sizeof_off_t=4 ac_cv_sizeof_ino_t=4 ac_cv_type_ssize_t=4 ac_cv_type_size_t=4 ac_cv_sizeof_pid_t=4 --with-mpm=prefork
	make -C httpd-src
	make -C httpd-src install DESTDIR=${TOPDIR}/apache/dest/

apache.opk: httpd-src/dest/bin/httpd
	mkdir data/apache
	cp -r dest/opt/apache/bin data/apache/
	cp -r dest/opt/apache/cgi-bin data/apache/
	cp -r dest/opt/apache/conf data/apache/
	cp -r dest/opt/apache/error data/apache/
	cp -r dest/opt/apache/htdocs data/apache/
	cp -r dest/opt/apache/logs data/apache/
	cp -r dest/opt/apache/modules data/apache/

	cp data/apache/conf/httpd.conf data/apache/conf/httpd.conf-default

	cp httpd.conf-snake data/apache/conf/httpd.conf

	mkdir data/apache/lib
	cp -L dest/opt/apache/lib/libapr-1.so.0 data/apache/lib/
	cp -L dest/opt/apache/lib/libaprutil-1.so.0 data/apache/lib/
	cp -L dest/opt/apache/lib/libexpat.so.0 data/apache/lib/

	cp -L ${TOPDIR}/pcre/inst/lib/libpcre.so.1 data/apache/lib/

	chmod -R 755 data/etc/init.d/apache


	chown -R root:root data

	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r apache.opk data.tar.gz control.tar.gz debian-binary
