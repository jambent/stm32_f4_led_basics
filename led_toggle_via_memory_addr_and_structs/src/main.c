// Blink LED using structs

#include<stdint.h>

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


//From same part of reference manual, clock for Port A enabled by setting bit 0 to 1
#define GPIOAEN	(1U<<0)


// Write 1 to bit 5 to turn LED on
#define PIN5 (1U<<5)
#define LED_PIN	(PIN5)


// structure to contain required registers for GPIOA
typedef struct{
	volatile uint32_t MODE_REG;
	volatile uint32_t SPACER[4];  // required to maintain correct memory spacing between MODE_REG and OUTPUT_DATA_REG
	volatile uint32_t OUTPUT_DATA_REG;
}GPIOA_TypeDef;

// structure to contain required registers for RCC
typedef struct{
	volatile uint32_t SPACER[12];
	volatile uint32_t RCC_AHB1EN_REG;
}RCC_TypeDef;


#define RCC	((RCC_TypeDef*) RCC_BASE)
#define GPIOA ((GPIOA_TypeDef*)GPIOA_BASE)



int main(void){

	RCC->RCC_AHB1EN_REG |= GPIOAEN;

	GPIOA->MODE_REG |= (1U<<10);
	GPIOA->MODE_REG &=~ (1U<<11);

	while(1){
		GPIOA->OUTPUT_DATA_REG ^= LED_PIN;
		for(int i=0; i<1000000; i++){}
	}

	return 0;
}



