ffmpeg: ffmpeg-src ffmpeg-src/libavformat/libavformat.a ffmpeg-src/libavutil/libavutil.a ffmpeg-src/libavcodec/libavcodec.a
ffmpeg-src:
	wget http://ffmpeg.org/releases/ffmpeg-0.8.12.tar.gz -O ffmpeg-src.tar.gz
	sha1sum -c ffmpeg-src.tar.gz.sha1
	mkdir ffmpeg-src
	tar -xf ffmpeg-src.tar.gz --strip-components=1 -C ffmpeg-src
	patch -p0 < ffmpeg-nobx.patch

ffmpeg-src/libavformat/libavformat.a ffmpeg-src/libavutil/libavutil.a ffmpeg-src/libavcodec/libavcodec.a:
	cd ffmpeg-src/ && ./configure --disable-shared --prefix=/opt --arch=arm --enable-cross-compile --cross-prefix=arm-linux- --target-os=linux --disable-armv5te --cpu=armv4 --disable-ffplay --disable-ffmpeg --disable-ffserver --disable-avdevice --disable-swscale --disable-postproc --disable-avfilter --disable-pthreads
	make -C ffmpeg-src

