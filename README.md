# Coffer
A compatible and scalable RISC-V TEE (only with PMP, the generic hardware primitive in RISC-V)

## Quick Start
``` shell
git clone https://github.com/Compass-All/coffer.git
cd coffer
git checkout v2

# initialize submodules coffer-opensbi and coffer-emodules
git submodule update --init --recursive

# to use docker, please use sudo or the user in docker group
docker build -t coffer .  
docker create --name coffer_test -it -w /root -v $(pwd)/coffer:/root/coffer --privileged coffer /bin/bash
docker start coffer_test
docker exec -it coffer_test /bin/bash

# inside the Container (coffer_test)
cd coffer
./setup.sh  # Get u-boot and busybox
make clean_all && make
./scripts/mkroot.sh && ./scripts/run-qemu.sh | tee coffer.log
# check coffer.log for the complete output
# less coffer.log
```

