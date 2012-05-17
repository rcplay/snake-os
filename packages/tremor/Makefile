tremor/output/lib/libvorbisidec.a:
	cd tremor-src/ && ./configure --disable-shared --disable-oggtest --prefix=${TOPDIR}/tremor/output/ --host=arm-linux CFLAGS="${CFLAGS_COMMON}" CPPFLAGS="-I${TOPDIR}/libogg/libogg-src/include/" LDLAGS=" -L${TOPDIR}/libogg/libogg-src/src/.libs/" 
	make -C tremor-src install
