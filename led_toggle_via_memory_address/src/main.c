// Blink LED by determining correct register addresses

// From User Guide
// LED Port: A
// LED Pin: 5


// From Memory Mapping section of Datasheet, looking at expanded Peripherals memory diagram
#define PERIPHERALS_BASE	(0x40000000UL)

// From Block Diagram in Datasheet, GPIO PORT A clock connected to AHB1 bus
// From Memory Mapping section of Datasheet, Table 10, AHB1 starts at 0x4002 0000
#define AHB1_PERIPHERALS_OFFSET	(0x00020000UL)
#define AHB1_PERIPHERALS_BASE (PERIPHERALS_BASE + AHB1_PERIPHERALS_OFFSET)

// From same table, GPIOA also starts at 0x4002 0000
#define GPIOA_OFFSET (0x0000UL)
#define GPIOA_BASE (AHB1_PERIPHERALS_BASE + GPIOA_OFFSET)

// From same table, Resetting Clock Control (RCC) starts at 0x4002 3800
#define RCC_OFFSET	(0x3800UL)
#define RCC_BASE	(AHB1_PERIPHERALS_BASE + RCC_OFFSET)


// From reference manual, determine RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
#define AHB1EN_REG_OFFSET (0x30UL)
#define RCC_AHB1EN_REG (*(volatile unsigned int *)(RCC_BASE + AHB1EN_REG_OFFSET))
//From same part of reference manual, clock for Port A enabled by setting bit 0 to 1
#define GPIOAEN	(1U<<0)

// GPIO have both a direction register (input or output) and a data register
// From reference manual, GPIO Port mode register (MODER) sets direction, address offset is 0x00
#define MODE_REG_OFFSET	(0x00UL)
#define GPIOA_MODE_REG (*(volatile unsigned int *)(GPIOA_BASE + MODE_REG_OFFSET))
// Need to set output mode for Port 5 via MODER5 (bits 10 and 11)
// 01 is general purpose output mode
// Reset value for Port A is 0xA800 0000 (each digit in hexadecimal is 4 bits), therefore bits 10 and 11 already 0

// GPIO output data register is same section of reference manual
#define OUTPUT_DATA_REG_OFFSET	(0x14UL)
#define GPIOA_OUTPUT_DATA_REG	(*(volatile unsigned int *)(GPIOA_BASE + OUTPUT_DATA_REG_OFFSET))
// Write 1 to bit 5 to turn LED on
#define PIN5 (1U<<5)
#define LED_PIN	(PIN5)



int main(void){
	//Enable clock access to GPIOA, using OR-equals operator so only one bit affected
	RCC_AHB1EN_REG |= GPIOAEN;

	//Set PA5 as output pin, by setting bit 10 to 1 and bit 11 to 0 with the AND-equals operator and ~ bit inversion
	GPIOA_MODE_REG |= (1U<<10);
	GPIOA_MODE_REG &=~(1U<<11);

	// Inside infinite loop, set PA5 high
	while(1){
		GPIOA_OUTPUT_DATA_REG ^= LED_PIN;
		for(int i=0; i<1000000; i++){}
	}

	return 0;
}
