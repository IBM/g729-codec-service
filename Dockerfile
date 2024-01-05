FROM registry.access.redhat.com/ubi9/ubi-minimal

RUN microdnf --nodocs update --disableplugin=subscription-manager -y && \
    microdnf --nodocs install --disableplugin=subscription-manager -y git python3 python3-pip python3-setuptools python3-wheel && \
    microdnf --nodocs install --disableplugin=subscription-manager libasan libubsan gdb make automake autoconf procps libuv pkgconfig sudo file gcc gcc-c++ openssl-devel -y && \
    microdnf clean all && \
    rm -rf /var/cache/microdnf

# Set password length and expiry for compliance with vulnerability advisor
RUN sed -i 's/^PASS_MAX_DAYS.*/PASS_MAX_DAYS   90/' /etc/login.defs && \
    sed -i 's/^PASS_MIN_DAYS.*/PASS_MIN_DAYS    1/' /etc/login.defs && \
    echo 'minlen = 9' >> /etc/security/pwquality.conf

RUN pip3 install ninja

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

# Install tini
ENV TINI_VERSION v0.18.0
ADD https://github.com/krallin/tini/releases/download/${TINI_VERSION}/tini /tini
RUN chmod +x /tini

RUN useradd -u 1001 -r -g 0 -s /sbin/nologin default \
    && mkdir -p /home/default \
    && chown -R 1001:0 /home/default \
    && chmod -R g+rw /home/default
RUN mkdir /licenses
COPY LICENSE /licenses
USER 1001

ENTRYPOINT ["/tini", "--"]

CMD [ "/usr/local/bin/g729-codec-service" ]
