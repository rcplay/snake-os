
all: hd-idle.opk

hd-idle-src:
	wget http://sourceforge.net/projects/hd-idle/files/hd-idle-1.04.tgz/download -O hd-idle-src.tar.gz
	sha1sum -c hd-idle-src.tar.gz.sha1
	mkdir hd-idle-src
	tar -xf hd-idle-src.tar.gz --strip-components=1 -C hd-idle-src

hd-idle-src/hd-idle: hd-idle-src
	make -C hd-idle-src CC=arm-linux-gcc CFLAGS="${CFLAGS_COMMON}"

hd-idle.opk: hd-idle-src/hd-idle
	cp hd-idle-src/hd-idle data/bin/
	chmod -R 755 data/bin/hd-idle
	chmod -R 755 data/etc/init.d/hd-idle

	chown -R root:root data
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r hd-idle.opk data.tar.gz control.tar.gz debian-binary
