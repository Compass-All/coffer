# Enclave Applications for Coffer

This is not a stand-alone repository.

## Repository Structure

```
coffer_user_mode
├── include
│   ├── enclave_app
│   │   └── enclave_util.h
│   └── host_app
│       └── host_util.h
├── README.md
└── src
    ├── enclave_app
    │   ├── ... (eapps)
    │   └── util
	│		└── enclave_util.c
    └── host_app
        ├── ... (host apps)
        └── util
			└── host_util.c
```