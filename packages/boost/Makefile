TARGET=boost
URL=http://sourceforge.net/projects/boost/files/boost/1.52.0/boost_1_52_0.tar.gz

$(TARGET): $(TARGET)-src output/lib/libboost_system.a output/lib/libboost_thread.a
$(TARGET)-src:
	wget -c $(URL) -O $(TARGET)-src.tar.gz
	sha1sum -c $(TARGET)-src.tar.gz.sha1
	mkdir $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src

output/lib/libboost_system.a output/lib/libboost_thread.a:
	if [ -f $(TARGET)-src/tools/build/v2/user-config.jam ]; then \
		grep -q "^using gcc : arm : arm-linux-g++ ;" $(TARGET)-src/tools/build/v2/user-config.jam || echo "using gcc : arm : arm-linux-g++ ;" >> $(TARGET)-src/tools/build/v2/user-config.jam; \
	fi

	cd $(TARGET)-src/ && ./bootstrap.sh --with-libraries=system,thread && ./b2 --prefix=${TOPDIR}/boost/output/ install

