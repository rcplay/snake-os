
all: transmission curl myfilebrowser


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
