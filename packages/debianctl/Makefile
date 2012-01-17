all:
	chown -R root:root data
	chmod +x data/share/www-service/debianctl
	chmod +x data/etc/init.d/debianctl
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r debianctl.opk data.tar.gz control.tar.gz debian-binary
