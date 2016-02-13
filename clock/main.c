#include "stm32f4xx.h"
#include "clock.h"

int main(void)
{
	// Set clock to 168MHz
	CLOCK_SetClockTo168MHz();

	// Update SystemCoreClock variable (for debug only)
	SystemCoreClockUpdate();

	while (1);
}
