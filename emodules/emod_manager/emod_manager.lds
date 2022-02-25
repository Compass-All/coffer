OUTPUT_ARCH( "riscv" )

ENTRY(_start)

SECTIONS
{
  _start = .;

  _text_start = .;
  .text : {
    *(.text.init)
    *(.text)
		. = ALIGN(8);
  }
  _text_end = .;

  . = ALIGN(0x1000);

  _init_data_start = .;
  .init.data : {
    *(.init.data .init.data.*)
		. = ALIGN(8);
  }
  _init_data_end = .;

  . = ALIGN(0x1000);

  _rodata_start = .;
  .rodata : {
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
		. = ALIGN(8);
  }
  _rodata_end = .;

  . = ALIGN(0x1000);

  _srodata_start = .;
  .srodata : {
    *(.srodata)
    *(.srodata.*)
    . = ALIGN(8);
  }
  _srodata_end = .;

  . = ALIGN(0x1000);

  _data_start = .;
  .data : {
    *(.data)
    *(.data.*)
    *(.sdata)
    . = ALIGN(8);
  }
  _data_end = .;

  . = ALIGN(0x1000);

  _bss_start = .;
  .bss : { 
    *(.bss)
    *(.bss.*)
    *(.sbss*)
		. = ALIGN(8);
  }
  _bss_end = .;
  
  . = ALIGN(0x1000);
  _end = .;
}