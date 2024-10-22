OUTPUT_ARCH( "riscv" )
__stack_size = 4096;
ENTRY( _start )

MEMORY {
  RAM (rwx)  : ORIGIN = 0x80000000,  LENGTH = 30K
}
 
SECTIONS
{

  .text : ALIGN( 16 ) {
    PROVIDE(_text_start = .);
    *(.text.init) *(.text .text.*)
    *(.rodata .rodata.*)
    PROVIDE(_text_end = .);
  }>RAM
  
  .data : ALIGN( 16 ) {
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*) *(.data .data.*)
    PROVIDE(_data_end = .);
  }>RAM
  
 
  .bss : ALIGN( 16 ) {
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) *(.bss .bss.*)
    PROVIDE(_bss_end = .);
  }>RAM

 .stack : ALIGN( 16 ) {
    _estack = .;
    . = . + __stack_size;
    _sstack = .;
    . = ALIGN(16);
    __global_pointer = . + 0x800;
    . = ALIGN(16);
    end = ABSOLUTE(.); 
  }>RAM
  

  PROVIDE( SYSCON = 0x11100001 );
  PROVIDE( TIMERL = 0x1100bff8 );
  PROVIDE( TIMERH = 0x1100bffc );
}
