FROM registry.access.redhat.com/ubi8/ubi

RUN yum update --disableplugin=subscription-manager -y && \
    yum install --disableplugin=subscription-manager -y git python3 python3-pip python3-setuptools python3-wheel zip && \
    yum install --disableplugin=subscription-manager curl make automake autoconf libuv pkgconfig sudo file gcc gcc-c++ openssl-devel -y && \
    yum clean all && \
    rm -rf /var/cache/yum

# Set password length and expiry for compliance with vulnerability advisor
RUN sed -i 's/^PASS_MAX_DAYS.*/PASS_MAX_DAYS   90/' /etc/login.defs && \
    sed -i 's/^PASS_MIN_DAYS.*/PASS_MIN_DAYS    1/' /etc/login.defs && \
    echo 'minlen = 9' >> /etc/security/pwquality.conf

ADD https://github.com/ninja-build/ninja/releases/download/v1.9.0/ninja-linux.zip .

RUN unzip ninja-linux && cp ninja /usr/local/bin

RUN pip3 install --user meson

ENV PATH $PATH:~/.local/bin

WORKDIR /opt/g729-codec-service/

COPY src src
COPY subprojects subprojects
COPY meson.build .

RUN ~/.local/bin/meson --buildtype=release build && cd build \
	&& ninja \
	&& ninja install

ENV LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:/usr/local/lib:/usr/local/lib64

RUN useradd -u 1001 -r -g 0 -s /sbin/nologin default \
    && mkdir -p /home/default \
    && chown -R 1001:0 /home/default \
    && chmod -R g+rw /home/default
RUN mkdir /licenses
COPY LICENSE /licenses
USER 1001
ENTRYPOINT [ "/usr/local/bin/g729-codec-service" ]


