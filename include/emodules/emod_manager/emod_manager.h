/**
 * @brief EModule Manager.
 * 
 * emod_manager is aware of every emodule type.
 * However, it does not care about implementation
 * details. In other words, all emodules providing
 * the same sets of API are regarded as equivalent.
 * 
 */

#pragma once

#include <emodules/emodule_id.h>
#include <types.h>

#define MESSAGE_LOAD_MODULE	0x717DB77A

void emod_manager_test();