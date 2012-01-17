#!/bin/sh

TEMPDISK=$(grep "^tempdisk=" /opt/etc/config/myfilebrowser | cut -d = -f 2)

/bin/haserl --upload-limit=200000 --upload-dir=/usb/${TEMPDISK} ./myfilebrowser.cgi