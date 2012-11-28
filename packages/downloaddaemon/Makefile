
TARGET=downloaddaemon
URL="http://sourceforge.net/p/downloaddaemon/code/1318/tree/tags/downloaddaemon-nightly-1316.tar.gz?format=raw"

$(TARGET): $(TARGET)-src $(TARGET).opk
$(TARGET)-src:
	wget -c $(URL) -O $(TARGET)-src.tar.gz
	sha1sum -c $(TARGET)-src.tar.gz.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src
	cat dd-cfg-open.patch | patch -p0

$(TARGET)-src/src/daemon/DownloadDaemon:
	cd downloaddaemon-src && CC=arm-linux-gcc CXX=arm-linux-g++ cmake . -DBOOST_ROOT=${TOPDIR}/boost/output -DCURL_INCLUDE_DIRs=${TOPDIR}/libcurl-ssl/libcurl-src/include/ -DCURL_LIBRARIES="" -DCMAKE_INSTALL_PREFIX=/ -DCMAKE_MODULE_LINKER_FLAGS="-L${TOPDIR}/libcurl-ssl/libcurl-src/lib/.libs/" -DCMAKE_EXE_LINKER_FLAGS="-lpthread -lz -L${TOPDIR}/libcurl-ssl/libcurl-src/lib/.libs/ -lcurl"
	cd downloaddaemon-src && DESTDIR=${TOPDIR}/$(TARGET)/data make install


$(TARGET).opk: $(TARGET)-src/src/daemon/DownloadDaemon
	chmod -R 755 data/bin/DownloadDaemon

	mkdir -p data/lib/ddlibs
	cp ${TOPDIR}/libcurl-ssl/libcurl-src/lib/.libs/libcurl.so.4 data/lib/ddlibs/
	cp ${TOPDIR}/boost/output/lib/libboost_system.so.1.52.0 data/lib/ddlibs/
	cp ${TOPDIR}/boost/output/lib/libboost_thread.so.1.52.0 data/lib/ddlibs/
	cp ${TOPDIR}/snake-os/tools/arm-uclibc-3.4.6/lib/libstdc++.so.6 data/lib/ddlibs/

	cp dd-init data/etc/init.d/downloadd
	chmod -R 755 data/etc/init.d/downloadd

	cp downloaddaemon.conf data/etc/downloaddaemon/

	chown -R root:root data
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r $(TARGET).opk data.tar.gz control.tar.gz debian-binary
