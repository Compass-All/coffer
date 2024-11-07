# coffer
A software-based modular TEE architecture on RISC-V

## Quick Start
``` shell
git clone https://github.com/GartonChan/coffer.git
git submodule update --init --recursive

# to use docker, please use sudo or the user in docker group
docker build -t coffer .  
docker create --name coffer_test -it -w /root -v $(pwd)/coffer:/root/coffer --privileged coffer /bin/bash
docker start coffer_test
docker exec -it coffer_test /bin/bash

# inside the Container (coffer_test)
cd coffer
make clean_all && make
./scripts/mkroot.sh && ./scripts/run-qemu.sh | tee coffer.log
# check coffer.log for the complete output
```

