FROM psptoolchain-libk-baremetal-cmake:latest

COPY . /src

RUN apk add cmake make gmp-dev mpfr-dev mpc1-dev
RUN cd /src && mkdir build_iplsdk && cd build_iplsdk && cmake -DCMAKE_TOOLCHAIN_FILE=/usr/local/pspdev/share/bootstrap-psp-toolchain.cmake ..
RUN cd /src/build_iplsdk && make && make install