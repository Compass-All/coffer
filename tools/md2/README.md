# Automatically update enclave module hashes

## Workflow for adding an e-module

1. put the module directory under `emodules` 
2. run `make` under `tools/md2`
3. update Makefile under `coffer-opensbi`: add a `GENFLAGS` line