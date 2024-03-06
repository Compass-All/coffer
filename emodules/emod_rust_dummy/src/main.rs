#![no_std]
#![no_main]
#![feature(c_size_t)]

#[macro_use]
extern crate lazy_static;

use core::arch::asm;
use core::ffi::{c_char, c_size_t, c_uchar};
use core::fmt::{self, Write};
use core::panic::PanicInfo;
use core::cell::RefCell;

// #define EMODULE_ID_RUST_DUMMY	8
const EMODULE_ID_RUST_DUMMY: u32 = 8; // todo: move to some public crate?

// static ref MOD_NAME: &'static str = "EModule Rust Dummy";
const MOD_NAME: &'static [c_uchar; 18] = b"EModule Rust Dummy";

macro_rules! sbi_ecall {
    ($ecall_id:expr, $arg1:expr, $arg2:expr) => {{
        let ecall_id: usize = $ecall_id;
        let arg1: usize = $arg1;
        let arg2: usize = $arg2;
        let ret: usize;
        unsafe {
            asm!(
                "ecall",
                in("a7") ecall_id,
                in("a0") arg1,
                in("a1") arg2,
                lateout("a0") ret,
                options(nostack)
            );
        }
        ret
    }};
}

fn putchar(c: u8) {
    sbi_ecall!(1, c as usize, 0);
}

struct EModuleConsole();
impl EModuleConsole {}
impl fmt::Write for EModuleConsole {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        s.chars().for_each(|c| putchar(c as u8));
        Ok(())
    }
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
struct EModDesc {
    emod_id: u32,
    name: [c_uchar; 32], // actually pointer
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
struct EModRustDummyApi {
    handler: extern "C" fn(c_size_t, c_size_t) -> c_size_t,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
struct EModRustDummy {
    emod_rust_dummy_desc: EModDesc,
    emod_rust_dummy_api: EModRustDummyApi,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
struct EModManagerApi {
    test: extern "C" fn(),
    acquire_emodule: extern "C" fn(u32) -> usize,
    map_page: extern "C" fn(usize, usize, u8, u8),
    unmap_page: extern "C" fn(usize, u8),
    panic: extern "C" fn(*const c_uchar) -> !,
    spin_trylock_grand: extern "C" fn() -> i32,
    spin_unlock_grand: extern "C" fn(),
    spin_lock_log: extern "C" fn(),
    spin_unlock_log: extern "C" fn(),
    get_pa: extern "C" fn(usize) -> usize,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
struct EModManager {
    emod_manager_desc: EModDesc,
    emod_manager_api: EModManagerApi,
}

struct EModManagerWrapper(RefCell<Option<EModManager>>);

unsafe impl Sync for EModManagerWrapper {}

lazy_static! {
    static ref EMOD_RUST_DUMMY: EModRustDummy = {
        let mut name: [c_uchar; 32] = [0; 32];
        name[..18].clone_from_slice(MOD_NAME);
        let emod_rust_dummy_desc = EModDesc {
            emod_id: EMODULE_ID_RUST_DUMMY,
            name,
        };
        let emod_rust_dummy_api = EModRustDummyApi {
            handler: handler,
        };
        EModRustDummy {
            emod_rust_dummy_desc,
            emod_rust_dummy_api
        }
    };
    static ref EMOD_MANAGER: EModManagerWrapper = EModManagerWrapper(RefCell::new(None));
}

// unsafe entry
#[no_mangle]
#[link_section = ".text.init"]
unsafe extern "C" fn _start(
    emod_manager_getter: extern "C" fn() -> EModManager
) -> extern "C" fn() -> EModRustDummy {
    let emod_manager = emod_manager_getter();

    EMOD_MANAGER.0.replace(Some(emod_manager));
    (EMOD_MANAGER.0.borrow().as_ref().unwrap().emod_manager_api.test)();

    get_emod_rust_dummy
}

#[no_mangle]
extern "C" fn get_emod_rust_dummy() -> EModRustDummy {
    *EMOD_RUST_DUMMY
}

extern "C" fn handler(_arg0: usize, _arg1: usize) -> usize {
    return 0;
}

#[panic_handler]
fn on_panic(info: &PanicInfo) -> ! {
    let mut mc = EModuleConsole();
    writeln!(mc, "[Panic] {}", info).unwrap();
    let panic_msg = b"Rust panic\n\0" as *const u8;
    (EMOD_MANAGER.0.borrow().as_ref().unwrap().emod_manager_api.panic)(panic_msg)
}
