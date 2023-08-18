FROM psptoolchain-libk-baremetal-cmake:latest

COPY . /src

RUN apk add cmake ninja gmp-dev mpfr-dev mpc1-dev python3 py3-ecdsa py3-pycryptodome
RUN cd /src && mkdir build_iplsdk && cd build_iplsdk && cmake -GNinja -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_TOOLCHAIN_FILE=/usr/local/pspdev/share/bootstrap-psp-toolchain.cmake ..
RUN cd /src/build_iplsdk && ninja && ninja install
RUN cp /src/make_ipl.py $PSPDEV/bin && ln -s $PSPDEV/bin/make_ipl.py $PSPDEV/bin/make_ipl
RUN cp /src/make_signed_ipl.py $PSPDEV/bin && ln -s $PSPDEV/bin/make_signed_ipl.py $PSPDEV/bin/make_signed_ipl