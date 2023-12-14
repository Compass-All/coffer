#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <util/console.h>

extern emod_manager_t       emod_manager;
#define panic		        emod_manager.emod_manager_api.panic
#define spin_lock_log       emod_manager.emod_manager_api.spin_lock_log   
#define spin_unlock_log     emod_manager.emod_manager_api.spin_unlock_log 
#define map_page	        emod_manager.emod_manager_api.map_page
#define unmap_page	        emod_manager.emod_manager_api.unmap_page
#define get_pa	            emod_manager.emod_manager_api.get_pa
#define emm_spin_trylock_grand  emod_manager.emod_manager_api.spin_trylock_grand
#define emm_spin_unlock_grand   emod_manager.emod_manager_api.spin_unlock_grand

#ifndef IN_EMOD_DEBUG
#include <emodules/emod_debug/emod_debug.h>
extern emod_debug_t 	emod_debug;
#define printd		    emod_debug.emod_debug_api.printd
#define printf		    emod_debug.emod_debug_api.printf
#define hexdump		    emod_debug.emod_debug_api.hexdump
#define start_timer	    emod_debug.emod_debug_api.start_timer
#define call_timer	    emod_debug.emod_debug_api.call_timer
#define end_timer	    emod_debug.emod_debug_api.end_timer
#include <emodules/debug.h>
#endif

#ifndef IN_EMOD_ALLOC
#include <emodules/emod_alloc/emod_alloc.h>
extern emod_alloc_t		emod_alloc;
#define kmalloc         emod_alloc.emod_alloc_api.kmalloc
#define calloc 	        emod_alloc.emod_alloc_api.calloc
#define memalign	    emod_alloc.emod_alloc_api.memalign
#define free	        emod_alloc.emod_alloc_api.free
#endif
