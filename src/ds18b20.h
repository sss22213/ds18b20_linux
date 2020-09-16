#ifndef __goio__h__
#define __goio__h__
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h> 
#include <stdint.h>

// pi2,pi3
#ifdef BCM2708
    #define PERI_BASE 0x3F000000
// pi4
#else
    #define PERI_BASE 0xFE000000
#endif

// GPIO base address
#define GPIO_BASE (PERI_BASE + 0x200000)

int ds_pin = 26;
#endif
