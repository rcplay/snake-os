all:
	mkdir -p data/bin
	cp ../snake-os/apps/curl/src/.libs/curl data/bin/
	chown -R root:root data
	cd data; tar --exclude-vcs -zcf ../data.tar.gz *

	chown -R root:root control
	cd control; tar --exclude-vcs -zcf ../control.tar.gz *

	chown -R root:root debian-binary
	ar -r curl.opk data.tar.gz control.tar.gz debian-binary
