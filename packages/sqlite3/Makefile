CC=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-gcc
AR=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-ar
RANLIB=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-ranlib
AS=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-as
LD=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-ld
NM=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-nm

KERNELDIR=$(TOPDIR)/snake-os/kernels/linux
SQLITE_DIR=$(TOPDIR)/sqlite
SQLITE_SRC_DIR=$(TOPDIR)/sqlite/sqlite-src/src
SQLITE_DRIVERS=$(SQLITE_DIR)/sqlite-src/drivers/2.6.21
SQLITE_UTILS_DIR=$(SQLITE_DIR)/sqlite-src/cmd

all: sqlite-src sqlite-binary sqlite.opk

sqlite-src:
	wget http://www.sqlite.org/sqlite-autoconf-3071300.tar.gz -O sqlite-src.tar.gz
	mkdir -p sqlite-src
	tar -xf sqlite-src.tar.gz --strip-components=1 -C sqlite-src

sqlite-src/Makefile:
	cd sqlite-src && \
	./configure --disable-shared --enable-static --host=arm-linux --disable-dynamic-extensions CFLAGS="${CFLAGS_COMMON}"

sqlite-binary: sqlite-src/Makefile
	cd sqlite-src && \
	make ARCH=arm CROSS_COMPILE=arm-linux- LDFLAGS="-all-static"

sqlite-clean:
	rm -f sqlite.opk
	rm -rf data/bin
	rm -rf data/lib
	rm -rf data/include
	rm -f control.tar.gz
	rm -f data.tar.gz
	rm -f sqlite-src.tar.gz
	rm -rf sqlite-src

sqlite-opk-clean:
	rm -f sqlite.opk

sqlite.opk:
	make install DESTDIR=$(SQLITE_DIR)/data -C $(SQLITE_DIR)/sqlite-src

	mv data/usr/local/bin data
	rm -rf data/usr

	chown -R root:root data
	cd $(SQLITE_DIR)/data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	cd $(SQLITE_DIR)/control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r sqlite.opk data.tar.gz control.tar.gz debian-binary
