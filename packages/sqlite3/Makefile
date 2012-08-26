CC=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-gcc
AR=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-ar
RANLIB=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-ranlib
AS=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-as
LD=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-ld
NM=$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/arm-linux-nm

KERNELDIR=$(TOPDIR)/snake-os/kernels/linux
SQLITE_DIR=$(TOPDIR)/sqlite3

all: sqlite3-src sqlite3-binary sqlite3.opk

sqlite3-src:
	wget http://www.sqlite.org/sqlite-autoconf-3071300.tar.gz -O sqlite3-src.tar.gz
	mkdir -p sqlite3-src
	tar -xf sqlite3-src.tar.gz --strip-components=1 -C sqlite3-src

sqlite3-src/Makefile:
	cd sqlite3-src && \
	./configure --disable-shared --enable-static --host=arm-linux --disable-dynamic-extensions CFLAGS="${CFLAGS_COMMON}"

sqlite3-binary: sqlite3-src/Makefile
	cd sqlite3-src && \
	make ARCH=arm CROSS_COMPILE=arm-linux- LDFLAGS="-all-static"

sqlite3-clean:
	rm -f sqlite3.opk
	rm -rf data/bin
	rm -rf data/lib
	rm -rf data/include
	rm -f control.tar.gz
	rm -f data.tar.gz
	rm -f sqlite3-src.tar.gz
	rm -rf sqlite3-src

sqlite3-opk-clean:
	rm -f sqlite3.opk

sqlite3.opk:
	make install DESTDIR=$(SQLITE_DIR)/data -C $(SQLITE_DIR)/sqlite3-src

	mv data/usr/local/bin data
	rm -rf data/usr

	chown -R root:root data
	cd $(SQLITE_DIR)/data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	cd $(SQLITE_DIR)/control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r sqlite3.opk data.tar.gz control.tar.gz debian-binary
