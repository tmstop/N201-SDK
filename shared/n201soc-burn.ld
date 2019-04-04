OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  flash (rxai!w) : ORIGIN = 0x20000000, LENGTH = 4M
  ilm   (rxai!w) : ORIGIN = 0x80000000, LENGTH = 64K
  load  (rxai!w) : ORIGIN = 0x8000FC00, LENGTH = 1K
  ram   (wxa!ri) : ORIGIN = 0x90000000, LENGTH = 64K
}


SECTIONS
{
  __stack_size = DEFINED(__stack_size) ? __stack_size : 2K;

  .bootloader           :
  {
    PROVIDE( __flash_start = . );
    KEEP (*(SORT_NONE(.bootloader)))
  } >flash AT>flash 





  .loader_lma :
  {
    . = ALIGN(4);
    PROVIDE( __loader_lmastart = . );
  } >flash AT>flash 

  .loader_vma :
  {
    . = ALIGN(4);
    PROVIDE( __loader_start = . );
  } >load AT>flash

  .loader :
  {
    KEEP (*(SORT_NONE(.loader)))
    . = ALIGN(4);
    PROVIDE( __loader_lmaend = . );
  } >load AT>flash





  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( __text_lmastart = . );
  } >flash AT>flash

  .ialign         :
  {
    . = ALIGN(4);
    PROVIDE( __text_start = . );
  } >ilm AT>flash 

  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  } >ilm AT>flash 


  .text           :
  {
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
  } >ilm AT>flash 

  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >ilm AT>flash 


  .endt         :
  {
    . = ALIGN(4);
    PROVIDE( __text_lmaend = . );
  } >flash AT>flash



  .preinit_array  :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >flash AT>flash 

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >flash AT>flash 

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >flash AT>flash 

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >flash AT>flash 

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >flash AT>flash 


  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( __data_lmastart = . );
  } >flash AT>flash 

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( __data_start = . );
  } >ram AT>flash 

  .data          :
  {
    *(.rdata)
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    *(.data .data.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800 );
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
  } >ram AT>flash 

  . = ALIGN(4);
  PROVIDE( _edata = . );
  PROVIDE( edata = . );
  
  .endd         :
  {
    . = ALIGN(4);
    PROVIDE( __data_lmaend = . );
  } >flash AT>flash

  PROVIDE( _fbss = . );
  PROVIDE( __bss_start = . );
  .bss            :
  {
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(4);
  } >ram AT>ram 

  . = ALIGN(8);
  PROVIDE( _end = . );
  PROVIDE( end = . );

  .stack ORIGIN(ram) + LENGTH(ram) - __stack_size :
  {
    PROVIDE( _heap_end = . );
    . = __stack_size;
    PROVIDE( _stack = . );
  } >ram AT>ram 
}
