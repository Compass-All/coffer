# Emodule

## Basic

An emodule consists of the following:
- an emodule descriptor
- a set of API
- an initialization function

## Details

### Emodule descriptor

Emodule descriptors are defined as the following:

```C
typedef struct {
	u32 	emod_id;
	char	name[32];
	u[?]	signature;
} emod_desc_t;
```

`emod_id` is the id of the emodule, representing what kind of functioning the emodule provides. If two emodules provide exactly the same API, they are considered as equivalent and share the same `emod_id`. Only one of them can be loaded into a specific enclave.

To see the full list of available `emod_id`s, check `include/emodules/emodule_id.h`.

`name[32]` is the name of the emodule. Equivalent emodules may have different names to be distinguished from each other.

`signature` is used for emodule attestation. **TO BE DONE**.

### Emodule API

Each emodule has a set of API functions, which can be accessed via its API structure. For example, given an emodule named `emod_example`, its API structure should be defined in `include/emodules/emod_example/emod_example.h` and take the following form:

```C
typedef struct {
	int (*example_func1)(int arg1, int arg2);
	void (*example_func2)(int arg1);
	// ...
} emod_example_api_t;
```

### Emodule and Getter

An example emodule may be defined by the following structure:

```C
typedef struct {
	emod_desc_t emod_example_desc;
	emod_example_api_t emod_example_api;
} emod_example_t;
```

And inside each emodule, a getter function must be provided, in a way similar to:

```C
// this structure is to be initialized in the initialization function
static emod_example_t emod_example;

static emod_example_t get_emod_example()
{
	return emod_example;
}
```

This getter function is how other emodules talks to the emodule.

### Emodule Initialization

All emodules are initialized by `emod_manager`, which itself is a special emodule. Each emodule should provide an initialization function, which will be invoked by `emod_manager` after being loaded into memory.

The initialization function satisfies the following conditions:
- it takes `emod_manager`'s getter function as the parameter and records it for future use
- it returns itself's getter function
- it initializes its API structure and emodule structure

### Emodule Dependency

Emodules may have dependencies over other emodules. Emodules that are relied on are lasily loaded in Coffer. That is, emodules only gets loaded when it's API functions get invoked.

Each emodule should have a pair of `dependency.c` and `dependency.h` files for managing dependent emodules. The file should at least contains `emod_manager`.

**TO BE DONE:** How function invocations trigger emodule loading?

When an emodule wants to load another emodule, it invokes `acquire_emodule()` function residing in `emod_manager`'s API. It returns the getter function of the newly loaded emodule so that the emodule can get invoked.