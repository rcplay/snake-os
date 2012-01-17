all:
	chown -R root:root data
	chmod -R 700 data/etc/myfilebrowser
	chmod -R 700 data/etc/config/myfilebrowser
	chmod +x data/share/myfilebrowser/cgi-bin/*.cgi
	chmod +x data/share/www-service/*
	chmod +x data/etc/init.d/*
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	chmod +x control/postinst control/prerm
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r myfilebrowser.opk data.tar.gz control.tar.gz debian-binary
