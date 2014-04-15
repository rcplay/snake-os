TARGET=usbip

USBIP_DIR=$(TOPDIR)/$(TARGET)
USBIP_DRIVERS=$(USBIP_DIR)/$(TARGET)-src/drivers/2.6.21
USBIP_UTILITIES=$(TARGET)-src/src/cmd

UTILITIES=$(USBIP_UTILITIES)/bind_driver $(USBIP_UTILITIES)/usbip $(USBIP_UTILITIES)/usbipd
MODULES=$(USBIP_DRIVERS)/usbip.ko $(USBIP_DRIVERS)/usbip_common_mod.ko $(USBIP_DRIVERS)/vhci-hcd.ko

$(TARGET): $(TARGET).opk

usb.ids:
	wget http://www.linux-usb.org/usb.ids

$(TARGET)-src:
	wget http://ufpr.dl.sourceforge.net/project/usbip/usbip/0.1.7/usbip-0.1.7.tar.gz -O $(TARGET)-src.tar.gz
	mkdir -p $(TARGET)-src
	tar -xf $(TARGET)-src.tar.gz --strip-components=1 -C $(TARGET)-src
	patch -p0 < nslu-arm.patch

$(MODULES):
	make \
		KSOURCE=$(TOPDIR)/snake-os/kernels/linux \
		ARCH=arm \
		CROSS_COMPILE=arm-linux- \
		-C $(USBIP_DRIVERS)

$(UTILITIES): $(TARGET)-src $(MODULES)
	cd $(TARGET)-src/src ; \
	./autogen.sh && \
	./configure \
		--disable-shared \
		--enable-static \
		--host=arm-linux \
		--prefix=/ CFLAGS="${CFLAGS_COMMON}" \
		--with-usbids-dir=/opt/share/usbip/ ; \
	make \
		ARCH=arm \
		CROSS_COMPILE=arm-linux- \
		LDFLAGS="-all-static -L${TOPDIR}/glib/glib-src/glib/.libs/"

$(TARGET).opk: $(TARGET)-src $(MODULES) $(UTILITIES) usb.ids
	make install DESTDIR=$(USBIP_DIR)/data -C $(USBIP_DIR)/$(TARGET)-src/src
	mv usb.ids data/share/usbip

	mkdir -p data/lib/modules/2.6.16-gazineu/$(TARGET)
	cp $(USBIP_DRIVERS)/*.ko data/lib/modules/2.6.16-gazineu/$(TARGET)

	rm -rf data/include
	rm -rf data/lib/libusbip.*a

	chown -R root:root debian-binary data control

	cd $(USBIP_DIR)/data; tar --exclude-vcs -zcf ../data.tar.gz *
	cd $(USBIP_DIR)/control; tar --exclude-vcs -zcf ../control.tar.gz *

	ar -r $(TARGET).opk data.tar.gz control.tar.gz debian-binary

clean:
	rm -f $(TARGET).opk
	rm -rf data/bin
	rm -rf data/lib
	rm -rf data/include
	rm -f control.tar.gz
	rm -f data.tar.gz
	rm -f $(TARGET)-src.tar.gz
	rm -rf $(TARGET)-src
