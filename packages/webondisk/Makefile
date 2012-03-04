all:
	chown -R root:root data
	chmod -R 755 data/etc/config/webondisk
	chmod +x data/etc/init.d/*
	chmod +x data/bin/*
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r webondisk.opk data.tar.gz control.tar.gz debian-binary
