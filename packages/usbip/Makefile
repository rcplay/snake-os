KERNELDIR=$(TOPDIR)/snake-os/kernels/linux
USBIP_DIR=$(TOPDIR)/usbip
USBIP_SRC_DIR=$(USBIP_DIR)/usbip-src/src
USBIP_DRIVERS=$(USBIP_DIR)/usbip-src/drivers/2.6.21
USBIP_UTILS_DIR=$(USBIP_DIR)/usbip-src/cmd

USBIP_UTILS=usbip usbipd bind_driver

all: usbip-src usbip-modules usbip-utilities usbip-opk-clean usbip.opk

usbip-src:
	wget http://ufpr.dl.sourceforge.net/project/usbip/usbip/0.1.7/usbip-0.1.7.tar.gz -O usbip-src.tar.gz
	mkdir -p usbip-src
	tar -xf usbip-src.tar.gz --strip-components=1 -C usbip-src
	patch -p0 < nslu-arm.patch

usbip-modules:
	make KSOURCE=$(KERNELDIR) ARCH=arm CROSS_COMPILE=arm-linux- -C $(USBIP_DRIVERS)

usbip-src/src/Makefile:
	cd usbip-src/src && \
	./autogen.sh && \
	./configure --disable-shared --enable-static --host=arm-linux \
		--prefix=/ CFLAGS="${CFLAGS_COMMON}" \
		--with-usbids-dir=/opt/share/usbip/

usbip-utilities: usbip-src/src/Makefile
	cd usbip-src/src && \
	make ARCH=arm CROSS_COMPILE=arm-linux- LDFLAGS="-all-static -L${TOPDIR}/glib/glib-src/glib/.libs/"

usbip-clean:
	rm -f usbip.opk
	rm -rf data/bin
	rm -rf data/lib
	rm -rf data/include
	rm -f control.tar.gz
	rm -f data.tar.gz
	rm -f usbip-src.tar.gz
	rm -rf usbip-src

usbip-opk-clean:
	rm -f usbip.opk

usbip.opk:
	make install DESTDIR=$(USBIP_DIR)/data -C $(USBIP_DIR)/usbip-src/src

	mkdir -p data/lib/modules/2.6.16-gazineu/usbip
	cp $(USBIP_DRIVERS)/*.ko data/lib/modules/2.6.16-gazineu/usbip

	rm -rf data/include
	rm -rf data/lib/libusbip.*a

	chown -R root:root data
	cd $(USBIP_DIR)/data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	cd $(USBIP_DIR)/control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r usbip.opk data.tar.gz control.tar.gz debian-binary
