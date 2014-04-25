TARGET=iperf

IPERF_DIR=$(TOPDIR)/$(TARGET)

$(TARGET): $(TARGET).opk

$(TARGET)-src:
	wget http://iperf.fr/download/iperf_2.0.5/iperf-2.0.5-source.tar.gz -O $(TARGET)-src.tar.gz
	mkdir -p $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src
	patch -p0 < fix-man.patch

$(TARGET)/data/bin/iperf: $(TARGET)-src
	cd $(TARGET)-src ; \
	./autogen.sh && \
	./configure \
		--host=arm-linux \
		CC=arm-linux-gcc \
		CXX=arm-linux-g++ \
		--prefix=/ \
		CFLAGS="${CFLAGS_COMMON} -static" \
		CXXFLAGS="${CFLAGS_COMMON}" \
		--host=i686-linux-gnu \
		--target=arm-linux-gnu \
		--enable-static=yes \
		--enable-shared=no \
		--cache-file=$(TOPDIR)/$(TARGET)/arm-linux.cache ; \
	make

$(TARGET).opk: $(TARGET)/data/bin/iperf
	make install DESTDIR=$(IPERF_DIR)/data -C $(IPERF_DIR)/$(TARGET)-src

	rm -rf $(IPERF_DIR)/data/share/man

	chown -R root:root debian-binary data control

	cd $(IPERF_DIR)/data; tar --exclude-vcs -zcf ../data.tar.gz *
	cd $(IPERF_DIR)/control; tar --exclude-vcs -zcf ../control.tar.gz *

	ar -r $(TARGET).opk data.tar.gz control.tar.gz debian-binary

clean:
	rm -f $(TARGET).opk
	rm -rf data/bin
	rm -f control.tar.gz
	rm -f data.tar.gz
	rm -f $(TARGET)-src.tar.gz
	rm -rf $(TARGET)-src
