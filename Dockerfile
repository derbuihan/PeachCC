FROM ubuntu:latest
RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -y build-essential sudo file
RUN adduser --disabled-password --gecos '' user
RUN echo 'user ALL=(root) NOPASSWD:ALL' > /etc/sudoers.d/user
USER user
WORKDIR /home/user
