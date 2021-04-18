/*
 * gpio.c
 *
 *  Created on: Dec 3, 2019
 *      Author: TOSHIBA.SXO11
 */
#include "gpio.h"
/*===========================PORT Control===============================*/
/**
 * Description: set port direction (which is DDR register)
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 * @param direction: set the port direction and takes the following values
 * 				- 0x00 -> Input
 * 				- 0xff -> Output
 */
void gpioPortDirection(uint8 port, uint8 direction){
	switch(port){
		case GPIOA:
			PORTA_DIR = direction;
			break;
		case GPIOB:
			PORTB_DIR = direction;
			break;
		case GPIOC:
			PORTC_DIR = direction;
			break;
		case GPIOD:
			PORTD_DIR = direction;
			break;
	}
}

/**
 * Description: set the port value (which is PORT register)
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param value: set the port value and takes the following values
 * 				- 0x00 -> Low
 * 				- 0xff -> High
 */
void gpioPortWrite(uint8 port, uint8 value){
	switch(port){
		case GPIOA:
			PORTA_DATA = value;
			break;
		case GPIOB:
			PORTB_DATA = value;
			break;
		case GPIOC:
			PORTC_DATA = value;
			break;
		case GPIOD:
			PORTD_DATA = value;
			break;
	}
}

/**
 * Description: toggle the port value (which is PORT register)
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 */
void gpioPortToggle(uint8 port){
	switch(port){
		case GPIOA:
			PORTA_DATA = ~PORTA_DATA;
			break;
		case GPIOB:
			PORTB_DATA = ~PORTB_DATA;
			break;
		case GPIOC:
			PORTC_DATA = ~PORTC_DATA;
			break;
		case GPIOD:
			PORTD_DATA = ~PORTD_DATA;
			break;
	}
}

/**
 * Description: read the current port value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @return
 */
uint8 gpioPortRead(uint8 port){
	uint8 pinValue; /*to return the value inside the pin register*/
	switch(port){
		case GPIOA:
			pinValue = PORTA_PIN;
			break;
		case GPIOB:
			pinValue = PORTB_PIN;
			break;
		case GPIOC:
			pinValue = PORTC_PIN;
			break;
		case GPIOD:
			pinValue = PORTD_PIN;
			break;
	}
	return pinValue;
}

/*===========================PIN Control===============================*/
/**
 * Description: set selected pins (more than one pin [ORed]) direction
 *
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param pins: the selected pins the user need to set its direction and takes
 * 				any member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 *
 * @param direction: set the pins direction and takes the following values
 * 				- 0x00 -> Input
 * 				- 0xff -> Output
 */
void gpioPinDirection(uint8 port, uint8 pins, uint8 direction){
	if(direction == OUTPUT){
		switch(port){
			case GPIOA:
				PORTA_DIR |= (pins);
				break;
			case GPIOB:
				PORTB_DIR |= (pins);
				break;
			case GPIOC:
				PORTC_DIR |= (pins);
				break;
			case GPIOD:
				PORTD_DIR |= (pins);
				break;
		}
	} else if(direction == INPUT){
		switch(port){
			case GPIOA:
				PORTA_DIR &= ~(pins);
				break;
			case GPIOB:
				PORTB_DIR &= ~(pins);
				break;
			case GPIOC:
				PORTC_DIR &= ~(pins);
				break;
			case GPIOD:
				PORTD_DIR &= ~(pins);
				break;
		}
	}
}

/**
 * Description: set selected pins (more than one pin [ORed]) values
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param pins: the selected pins the user need to write its values and takes
 * 				any member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 * @param value
 */
void gpioPinWrite(uint8 port, uint8 pins, uint8 value){
	if(value == HIGH){
		switch(port){
			case GPIOA:
				PORTA_DATA |= (pins);
				break;
			case GPIOB:
				PORTB_DATA |= (pins);
				break;
			case GPIOC:
				PORTC_DATA |= (pins);
				break;
			case GPIOD:
				PORTD_DATA |= (pins);
				break;
		}
	}else if(value==LOW){
		switch(port){
			case GPIOA:
				PORTA_DATA &= ~(pins);
				break;
			case GPIOB:
				PORTB_DATA &= ~(pins);
				break;
			case GPIOC:
				PORTC_DATA &= ~(pins);
				break;
			case GPIOD:
				PORTD_DATA &= ~(pins);
				break;
		}
	}
}

/**
 * Description: toggle selected pin (only one pin) direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param pins: the selected pin the user need to toggle it's value and takes
 * 				only one member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 */
void gpioPinToggle(uint8 port, uint8 pins){
	switch(port){
		case GPIOA:
			PORTA_DATA ^= pins;
			break;
		case GPIOB:
			PORTB_DATA ^= pins;
			break;
		case GPIOC:
			PORTC_DATA ^= pins;
			break;
		case GPIOD:
			PORTD_DATA ^= pins;
			break;
	}
}

/**
 * Description: read selected pin (only one pin) direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 * @param pin: the selected pin the user need to read it's value and takes
 * 				only one member from the enum EN_bits:
 * 				- BIT0
 * 				- BIT1
 * 				- BIT2
 * 				- BIT3
 * 				- BIT4
 * 				- BIT5
 * 				- BIT6
 * 				- BIT7
 * @return
 */
uint8 gpioPinRead(uint8 port, uint8 pin){
	uint8 pinValue; /*to return the value inside the pin register*/
	switch(port){
		case GPIOA:
			pinValue = PORTA_PIN & pin;
			break;
		case GPIOB:
			pinValue = PORTB_PIN & pin;
			break;
		case GPIOC:
			pinValue = PORTC_PIN & pin;
			break;
		case GPIOD:
			pinValue = PORTD_PIN & pin;
			break;
	}
	return pinValue;
}

/*===========================Upper Nibble Control===============================*/
/**
 * Description: set the upper nibble direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param direction
 */
void gpioUpperNibbleDirection(uint8 port, uint8 direction){
	if(direction == OUTPUT){
		switch(port){
			case GPIOA:
				PORTA_DIR |= (0XF0);
				break;
			case GPIOB:
				PORTB_DIR |= (0XF0);
				break;
			case GPIOC:
				PORTC_DIR |= (0XF0);
				break;
			case GPIOD:
				PORTD_DIR |= (0XF0);
				break;
		}
	}
	else if(direction == LOW){
		switch(port){
			case GPIOA:
				PORTA_DIR &= ~(0XF0);
				break;
			case GPIOB:
				PORTB_DIR &= ~(0XF0);
				break;
			case GPIOC:
				PORTC_DIR &= ~(0XF0);
				break;
			case GPIOD:
				PORTD_DIR &= ~(0XF0);
				break;
		}
	}
}

/**
 * Description: write the upper nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param value
 */
void gpioUpperNibbleWrite(uint8 port, uint8 value){
	if(value == HIGH){
		switch(port){
			case GPIOA:
				PORTA_DATA |= (0XF0);
				break;
			case GPIOB:
				PORTB_DATA |= (0XF0);
				break;
			case GPIOC:
				PORTC_DATA |= (0XF0);
				break;
			case GPIOD:
				PORTD_DATA |= (0XF0);
				break;
		}
	}else if(value == LOW){
		switch(port){
			case GPIOA:
				PORTA_DATA &= ~(0XF0);
				break;
			case GPIOB:
				PORTB_DATA &= ~(0XF0);
				break;
			case GPIOC:
				PORTC_DATA &= ~(0XF0);
				break;
			case GPIOD:
				PORTD_DATA &= ~(0XF0);
				break;
		}
	}
}

/**
 * Description: toggle the upper nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 */
void gpioUpperNibbleToggle(uint8 port){
	switch(port){
		case GPIOA:
			PORTA_DATA ^= 0XF0;
			break;
		case GPIOB:
			PORTB_DATA ^= 0XF0;
			break;
		case GPIOC:
			PORTC_DATA ^= 0XF0;
			break;
		case GPIOD:
			PORTD_DATA ^= 0XF0;
			break;
	}
}

/**
 * Description: read the upper nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @return
 */
uint8 gpioUpperNibbleRead(uint8 port){
	uint8 pinValue;
	switch(port){
		case GPIOA:
			pinValue = PORTA_PIN & 0XF0;
			break;
		case GPIOB:
			pinValue = PORTB_PIN & 0XF0;
			break;
		case GPIOC:
			pinValue = PORTC_PIN & 0XF0;
			break;
		case GPIOD:
			pinValue = PORTD_PIN & 0XF0;
			break;
	}
	return pinValue;
}

/*===========================Lower Nibble Control===============================*/
/**
 * Description: set the lower nibble direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 * @param direction
 */
void gpioLowerNibbleDirection(uint8 port, uint8 direction){
	if(direction == OUTPUT){
		switch(port){
			case GPIOA:
				PORTA_DIR |= (0X0F);
				break;
			case GPIOB:
				PORTB_DIR |= (0X0F);
				break;
			case GPIOC:
				PORTC_DIR |= (0X0F);
				break;
			case GPIOD:
				PORTD_DIR |= (0X0F);
				break;
		}
	} else if(direction == INPUT){
		switch(port){
			case GPIOA:
				PORTA_DIR &= ~(0X0F);
				break;
			case GPIOB:
				PORTB_DIR &= ~(0X0F);
				break;
			case GPIOC:
				PORTC_DIR &= ~(0X0F);
				break;
			case GPIOD:
				PORTD_DIR &= ~(0X0F);
				break;
		}
	}
}

/**
 * Description: write the lower nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param value
 */
void gpioLowerNibbleWrite(uint8 port, uint8 value){
	if(value == HIGH){
		switch(port){
			case GPIOA:
				PORTA_DATA |= (0X0F);
				break;
			case GPIOB:
				PORTB_DATA |= (0X0F);
				break;
			case GPIOC:
				PORTC_DATA |= (0X0F);
				break;
			case GPIOD:
				PORTD_DATA |= (0X0F);
				break;
		}
	} else if(value == LOW){
		switch(port){
			case GPIOA:
				PORTA_DATA &= ~(0X0F);
				break;
			case GPIOB:
				PORTB_DATA &= ~(0X0F);
				break;
			case GPIOC:
				PORTC_DATA &= ~(0X0F);
				break;
			case GPIOD:
				PORTD_DATA &= ~(0X0F);
				break;
		}
	}
}

/**
 * Description: toggle the lower nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 */
void gpioLowerNibbleToggle(uint8 port){
	switch(port){
		case GPIOA:
			PORTA_DATA ^= 0X0F;
			break;
		case GPIOB:
			PORTB_DATA ^= 0X0F;
			break;
		case GPIOC:
			PORTC_DATA ^= 0X0F;
			break;
		case GPIOD:
			PORTD_DATA ^= 0X0F;
			break;
	}
}

/**
 * Description: read the lower nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @return
 */
uint8 gpioLowerNibbleRead(uint8 port){
	uint8 pinValue;
		switch(port){
			case GPIOA:
				pinValue = PORTA_PIN & 0X0F;
				break;
			case GPIOB:
				pinValue = PORTB_PIN & 0X0F;
				break;
			case GPIOC:
				pinValue = PORTC_PIN & 0X0F;
				break;
			case GPIOD:
				pinValue = PORTD_PIN & 0X0F;
				break;
		}
		return pinValue;
}

