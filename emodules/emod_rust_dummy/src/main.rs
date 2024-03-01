#![no_std]
#![no_main]

use core::arch::asm;
use core::fmt::{self, Write};
use core::panic::PanicInfo;

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

// unsafe entry
#[no_mangle]
#[link_section = ".text.init"]
unsafe extern "C" fn _start() {
    putchar(b'h');
    putchar(b'i');
    putchar(b'\n');

    entry();
}

// safe entry
extern "C" fn entry() {
    let mut mc = EModuleConsole();
    writeln!(mc, "Hello, world!").unwrap();

    panic!("Stop here");
}

#[panic_handler]
fn on_panic(info: &PanicInfo) -> ! {
    let mut mc = EModuleConsole();
    writeln!(mc, "[Panic] {}", info).unwrap();
    loop {}
}
