#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>

extern emod_manager_t emod_manager;

#define get_pa 		emod_manager.emod_manager_api.get_pa