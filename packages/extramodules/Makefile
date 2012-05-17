
all: extramodules.opk

linux-src:
	cp -r ${TOPDIR}/snake-os/kernels/linux/ linux-tmp
	make -C linux-tmp clean
	cp kernel.conf linux-tmp/.config
	mv linux-tmp linux-src

data/lib/extramodules/modules.dep: linux-src
	make -C linux-src zImage
	make -C linux-src modules
	make -C linux-src modules_install INSTALL_MOD_PATH=${TOPDIR}/extramodules/data/


extramodules.opk: data/lib/extramodules/modules.dep
	chown -R root:root data
	chmod +x data/bin/*
	chmod +x data/share/www-service/*
	chmod +x data/etc/init.d/*

	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r extramodules.opk data.tar.gz control.tar.gz debian-binary
