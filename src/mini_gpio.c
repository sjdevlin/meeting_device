/*
   minimal_gpio.c
   2019-07-03
   Public Domain
*/

/*
   gcc -o minimal_gpio minimal_gpio.c
   sudo ./minimal_gpio
*/

#include "mini_gpio.h"

void gpioSetMode(unsigned gpio, unsigned mode)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
}


void gpioSetPullUpDown(unsigned gpio, unsigned pud)
{
   int shift = (gpio & 0xf) << 1;
   uint32_t bits;
   uint32_t pull;

   if (pi_is_2711)
   {
      switch (pud)
      {
         case PI_PUD_OFF:  pull = 0; break;
         case PI_PUD_UP:   pull = 1; break;
         case PI_PUD_DOWN: pull = 2; break;
      }

      bits = *(gpioReg + GPPUPPDN0 + (gpio>>4));
      bits &= ~(3 << shift);
      bits |= (pull << shift);
      *(gpioReg + GPPUPPDN0 + (gpio>>4)) = bits;
   }
   else
   {
      *(gpioReg + GPPUD) = pud;

      usleep(20);

      *(gpioReg + GPPUDCLK0 + PI_BANK) = PI_BIT;

      usleep(20);
  
      *(gpioReg + GPPUD) = 0;

      *(gpioReg + GPPUDCLK0 + PI_BANK) = 0;
   }
}

int gpioRead(unsigned gpio)
{
   if ((*(gpioReg + GPLEV0 + PI_BANK) & PI_BIT) != 0) return 1;
   else                                         return 0;
}

void gpioWrite(unsigned gpio, unsigned level)
{
   if (level == 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
}



static uint32_t * initMapMem(int fd, uint32_t addr, uint32_t len)
{
    return (uint32_t *) mmap(0, len,
       PROT_READ|PROT_WRITE|PROT_EXEC,
       MAP_SHARED|MAP_LOCKED,
       fd, addr);
}

int gpioInitialise(void)
{
   int fd;

   gpioHardwareRevision(); /* sets rev and peripherals base address */

   fd = open("/dev/mem", O_RDWR | O_SYNC) ;

   if (fd < 0)
   {
      fprintf(stderr,
         "This program needs root privileges.  Try using sudo\n");
      return -1;
   }

   gpioReg  = initMapMem(fd, GPIO_BASE,  GPIO_LEN);
   systReg  = initMapMem(fd, SYST_BASE,  SYST_LEN);
   bscsReg  = initMapMem(fd, BSCS_BASE,  BSCS_LEN);

   close(fd);

   if ((gpioReg == MAP_FAILED) ||
       (systReg == MAP_FAILED) ||
       (bscsReg == MAP_FAILED))
   {
      fprintf(stderr,
         "Bad, mmap failed\n");
      return -1;
   }
   return 0;
}

