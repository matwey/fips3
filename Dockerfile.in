FROM matwey/opensuse-ci:BASE_IMAGE_TAG

ENV CTEST_OUTPUT_ON_FAILURE 1
ENV CXXFLAGS -Wall -Wextra -pedantic
ENV FIPS_BUILD_DIR /tmp/build

WORKDIR ${FIPS_BUILD_DIR}

COPY . /usr/src/fips3
RUN cmake /usr/src/fips3 -DCMAKE_BUILD_TYPE=Release && make VERBOSE=1 && make VERBOSE=1 install

CMD ["make", "test"]
