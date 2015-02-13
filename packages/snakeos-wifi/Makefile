
all: snakeos-wifi.opk

linux-src:
	cp -r ${TOPDIR}/snake-os/kernels/linux/ linux-tmp
	make -C linux-tmp clean
	cp kernel.conf linux-tmp/.config
	mv linux-tmp linux-src

linux-src/arch/arm/boot/zImage: linux-src
	make -C linux-src zImage ARCH=arm CROSS_COMPILE=arm-linux-

rt5370-src:
	-rm -r rt5370-tmp
	cp -r rt5370-orig rt5370-tmp
	cd rt5370-tmp; patch -p1 < ../rt5370-snake.patch
	mv rt5370-tmp rt5370-src

rt5370-src/os/linux/rt5370sta.ko: linux-src/arch/arm/boot/zImage rt5370-src
	make -C rt5370-src LINUX_SRC=$(CURDIR)/linux-src/ CHIPSET=5370


mt7601u-src:
	-rm -r mt7601u-tmp
	cp -r mt7601u-orig mt7601u-tmp
	cd mt7601u-tmp; patch -p1 < ../mt7601u-snake.patch
	mv mt7601u-tmp mt7601u-src

mt7601u-src/os/linux/mt7601Usta.ko: linux-src/arch/arm/boot/zImage mt7601u-src
	make -C mt7601u-src LINUX_SRC=$(CURDIR)/linux-src/



rtl8192cu-src:
	-rm -r rtl8192cu-tmp
	cp -r rtl8192cu-orig rtl8192cu-tmp
	cd rtl8192cu-tmp; patch -p1 < ../rtl8192cu-snake.patch
	mv rtl8192cu-tmp rtl8192cu-src

rtl8192cu-src/8192cu.ko: linux-src/arch/arm/boot/zImage rtl8192cu-src
	make -C rtl8192cu-src KSRC=$(CURDIR)/linux-src/


wpa_supplicant-src:
	wget http://hostap.epitest.fi/releases/wpa_supplicant-2.3.tar.gz -O $@.tar.gz
	sha1sum -c $@.tar.gz.sha1
	mkdir $@
	tar -xf $@.tar.gz --strip-components=1 -C $@

wpa_supplicant-src/wpa_supplicant/.config: wpa_supplicant-src
	cp wpa_supplicant.conf $@

wpa_supplicant-src/wpa_supplicant/wpa_supplicant: wpa_supplicant-src/wpa_supplicant/.config
	CC=arm-linux-gcc CFLAGS="${CFLAGS_COMMON} -MMD" make -C wpa_supplicant-src/wpa_supplicant

snakeos-wifi.opk: rt5370-src/os/linux/rt5370sta.ko mt7601u-src/os/linux/mt7601Usta.ko rtl8192cu-src/8192cu.ko wpa_supplicant-src/wpa_supplicant/wpa_supplicant
	cp wpa_supplicant-src/wpa_supplicant/wpa_supplicant data/bin/

	cp rt5370-src/RT2870STA.dat data/share/wifi/
	cp rt5370-src/os/linux/rt5370sta.ko data/share/wifi/

	cp mt7601u-src/os/linux/mt7601Usta.ko data/share/wifi/

	cp rtl8192cu-src/8192cu.ko data/share/wifi/

	chown -R root:root data
	chmod +x data/bin/*
	chmod +x data/share/www-service/*
	chmod +x data/etc/init.d/*


	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r snakeos-wifi.opk data.tar.gz control.tar.gz debian-binary
