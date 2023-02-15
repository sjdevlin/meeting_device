/*
   minimal_gpio.c
   2019-07-03
   Public Domain
*/

/*
   gcc -o minimal_gpio minimal_gpio.c
   sudo ./minimal_gpio
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

static volatile uint32_t piPeriphBase = 0x20000000;

static volatile int pi_is_2711 = 0;

#define SYST_BASE  (piPeriphBase + 0x003000)
#define GPIO_BASE  (piPeriphBase + 0x200000)
#define BSCS_BASE  (piPeriphBase + 0x214000)

#define DMA_LEN   0x1000 /* allow access to all channels */
#define CLK_LEN   0xA8
#define GPIO_LEN  0xF4
#define SYST_LEN  0x1C
#define BSCS_LEN  0x40

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD     37
#define GPPUDCLK0 38
#define GPPUDCLK1 39

/* BCM2711 has different pulls */

#define GPPUPPDN0 57
#define GPPUPPDN1 58
#define GPPUPPDN2 59
#define GPPUPPDN3 60

#define SYST_CS  0
#define SYST_CLO 1
#define SYST_CHI 2

static volatile uint32_t  *gpioReg = MAP_FAILED;
static volatile uint32_t  *systReg = MAP_FAILED;
static volatile uint32_t  *bscsReg = MAP_FAILED;

#define PI_BANK (gpio>>5)
#define PI_BIT  (1<<(gpio&0x1F))

/* gpio modes. */

#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2

#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2

void gpioSetMode(unsigned, unsigned);
void gpioSetPullUpDown(unsigned, unsigned);
int gpioRead(unsigned gpio);
void gpioWrite(unsigned gpio, unsigned level);
int gpioInitialise(void);

