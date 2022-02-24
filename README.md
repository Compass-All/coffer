# Coffer

A software-based modular TEE architecture on RISC-V

## How to build

``` Bash
git clone git@github.com:Compass-All/coffer.git
git submodule update --init
docker build -t coffer_dev .
docker run -it -v $(pwd):/coffer --rm --device /dev/kvm coffer_dev /bin/bash

# Inside docker
cd /coffer
make
```