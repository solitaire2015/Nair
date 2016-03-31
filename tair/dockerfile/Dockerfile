FROM docker.neg/centos-sshd:1.0.1

MAINTAINER rick rick.b.liu@newegg.com

RUN mkdir -p /opt/nair/

COPY nair_server /opt/nair/nair_server

COPY nairserver.conf /opt/nair/nairserver.conf

COPY run.sh /opt/nair/run.sh

ENV NAIR DEFAULT

WORKDIR /opt/nair/

CMD ["./run.sh"]

EXPOSE 8887
