/* See LICENSE for license details. */
OUTPUT_ARCH( "riscv" )

SECTIONS
{
  .text : 
  {
    *(.text.init)
    *(.text)
  }

  . = ALIGN(0x1000);
  .rodata :
  {
    *(.rdata)
    *(.rodata)
  }

  .data : 
  {
    *(.data)
    *(.data.*)
  }

  .bss : { 
    *(.bss)
    *(.bss.*)
    *(.sbss*)
  }

}