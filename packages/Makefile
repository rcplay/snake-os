
all: transmission curl myfilebrowser minidlna

export TOPDIR=$(PWD)
export CFLAGS_COMMON=-Os -s -pipe -march=armv4

PATH+=:$(TOPDIR)/snake-os/tools/arm-uclibc-3.4.6/bin/

snake-os: snake-os/output/rootfs.jffs2
snake-os/output/rootfs.jffs2:
	cd snake-os; sh mk_snakeos_image.sh


transmission: output/transmission.opk
output/transmission.opk: snake-os
	cp ./snake-os/output/transmission.opk ./output/


curl: output/curl.opk
output/curl.opk: snake-os
	make -C curl
	cp curl/curl.opk output/curl.opk

myfilebrowser: output/myfilebrowser.opk
output/myfilebrowser.opk:
	make -C myfilebrowser
	cp myfilebrowser/myfilebrowser.opk output/myfilebrowser.opk


MINIDLNA_DEPS=libjpeg libexif libogg libvorbis libsqlite3 libFLAC libid3tag libiconv ffmpeg

.PHONY: ${MINIDLNA_DEPS}
${MINIDLNA_DEPS}:
	make -C $@

minidlna: output/minidlna.opk
output/minidlna.opk: ${MINIDLNA_DEPS}
	make -C minidlna
	cp minidlna/minidlna.opk output/minidlna.opk

minidlna-release: output/minidlna-release.opk
output/minidlna-release.opk: ${MINIDLNA_DEPS}
	make -C minidlna-release
	cp minidlna-release/minidlna-release.opk output/minidlna-release.opk

debianctl: output/debianctl.opk
output/debianctl.opk:
	make -C debianctl
	cp debianctl/debianctl.opk output/debianctl.opk

samba3: output/samba3.opk
output/samba3.opk:
	make -C samba3
	cp samba3/samba3.opk output/samba3.opk

webondisk: output/webondisk.opk
output/webondisk.opk:
	make -C webondisk
	cp webondisk/webondisk.opk output/webondisk.opk

.PHONY: libcurl gettext libffi libmad tremor glib
libcurl gettext libffi libmad tremor:
	make -C $@

glib: gettext libffi libiconv
	make -C $@

mpd: libcurl libid3tag libmad libiconv libogg tremor libvorbis libFLAC libsqlite3 glib output/mpd.opk
output/mpd.opk:
	make -C mpd
	cp mpd/mpd.opk output/mpd.opk


extramodules: output/extramodules.opk
output/extramodules.opk:
	make -C extramodules
	cp extramodules/extramodules.opk output/extramodules.opk

usbip: output/usbip.opk
output/usbip.opk: glib
	make -C usbip
	cp usbip/usbip.opk output/usbip.opk

