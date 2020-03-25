#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"


#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

volatile static int started = 0;

// start() jumps here in supervisor mode on all CPUs.
void
main()
{
  if(cpuid() == 0){
    consoleinit();
    printfinit();
    printf("\n");
    printf("xv6 kernel is booting\n");
    printf("\n");

    printf(ANSI_COLOR_RED"       _                                         \n");
    printf(ANSI_COLOR_YELLOW"      (_)                                        \n");
    printf(ANSI_COLOR_CYAN"  __ _  _ __   __  ___  _ __    ___   _ __    ___ \n");
    printf(ANSI_COLOR_GREEN" / _` || |\\ \\ / / / _ \\| '_ \\  / _ \\ | '_ \\  / _ \\ \n");
    printf(ANSI_COLOR_BLUE"| (_| || | \\ V / |  __/| | | || (_) || | | ||  __/ \n");
    printf(ANSI_COLOR_MAGENTA" \\__, ||_|  \\_/   \\___||_| |_| \\___/ |_| |_| \\___|\n");
    printf(ANSI_COLOR_YELLOW"  __/ |    \n");
    printf(ANSI_COLOR_RED" |___/   "ANSI_COLOR_RESET "\n");

    printf(ANSI_COLOR_GREEN "givenone" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_RED"2017-19428" ANSI_COLOR_CYAN" 서준원"ANSI_COLOR_RESET"\n");


    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    procinit();      // process table
    trapinit();      // trap vectors
    trapinithart();  // install kernel trap vector
    plicinit();      // set up interrupt controller
    plicinithart();  // ask PLIC for device interrupts
    binit();         // buffer cache
    iinit();         // inode cache
    fileinit();      // file table
    virtio_disk_init(); // emulated hard disk
    userinit();      // first user process
    __sync_synchronize();
    started = 1;



  } else {
    
    while(started == 0)
      ;
    __sync_synchronize();
    printf("hart %d starting\n", cpuid());
    kvminithart();    // turn on paging
    trapinithart();   // install kernel trap vector
    plicinithart();   // ask PLIC for device interrupts
  }

  scheduler();        
}
