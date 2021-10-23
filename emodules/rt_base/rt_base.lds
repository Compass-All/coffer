OUTPUT_ARCH( "riscv" )

ENTRY(_start)

SECTIONS
{
  . = RT_BASE_START;
  _start = .;

  _text_start = .;
  .text : {
    *(.text.init)
    *(.text)
  }
  _text_end = .;

  . = ALIGN(0x1000);

  _init_data_start = .;
  .init.data : {
    *(.init.data .init.data.*)
  }
  _init_data_end = .;

  . = ALIGN(0x1000);

  _rodata_start = .;
  .rodata : {
    *(.rdata)
    *(.rodata)
  }
  _rodata_end = .;

  . = ALIGN(0x1000);

  _data_start = .;
  .data : {
    *(.data)
    *(.data.*)
  }
  _data_end = .;

  . = ALIGN(0x1000);

  _bss_start = .;
  .bss : { 
    *(.bss)
    *(.bss.*)
    *(.sbss*)
  }
  _bss_end = .;
  
  . = ALIGN(0x1000);

  _drv_console_start = .;
  .drv_enclave_console :
  {
    *(.drv_enclave_console)
  }
  _drv_console_end = .;

  _end = .;
  . = ALIGN(0x1000);

  _temp_stack_start = .;
  . = . + 0x1000;
  _temp_stack_end = .;

}