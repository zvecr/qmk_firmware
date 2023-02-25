FROM qmkfm/qmk_cli

RUN apt-get update && apt-get install --no-install-recommends -y \
    curl \
    less \
    locales \
    nano \
    procps \
    sudo \
    udev \
    vim \
    && rm -rf /var/lib/apt/lists/*

RUN update-locale LANG=C.UTF-8

RUN groupadd qmk && \
    useradd -m -g qmk -s /bin/bash -p $(openssl passwd -1 'qmk') qmk

# as udev cannot run within containers...
#   shim a bunch of stuff to automatically use sudo
COPY . /
