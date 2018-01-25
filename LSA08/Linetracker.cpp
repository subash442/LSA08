/* 
* Linetracker.cpp
*
* Created: 12/24/2017 1:42:17 PM
* Author: Subash Timilsina
*/



#include "Linetracker.h"

Linetracker::Linetracker(int addr) // 0 to 255
{
	address = addr;
	junction_detect = false;
	junction_count = 0;
}

void Linetracker::initialise()
{
	sei();
	initUART2();	//initialise communication
	
	INPUT(JUNCTION_PIN);	//To detect junction pin
	CLEAR(JUNCTION_PIN);	//Internal Pull down junction pin

	OUTPUT(UART_ENABLE);		//to receive data uart enable pin as output
	CLEAR(UART_ENABLE);		//active low pin //always ready to send data so cleared
}

/****************************************optional if junction pulse use interrupt************************************/

void Linetracker::initialise_interrupt() //call this function in initialise
{
	sei();
	EIMSK &= ~(1<<JUNCTION_INT);
	JUNC_EICR |= (1<<INT_JUNC_ISC1);	//falling edge
	EIMSK |= (1<<JUNCTION_INT);		//setting INT pin
	EIFR |= (1<<INT_JUNC_INTF);	    //clear int flag

	/******************************************ISR FOR JUNCTION COUNT*************************************/
	/* put it in main
	*	ISR(INT_JUNC_VECT)
	*	{
	*		objectname.Inc_junc_count();
	*   }
	*/

}

void Linetracker::Off_interrupt()
{
	EIMSK &= ~(1<<JUNCTION_INT);		//clearing the interrupt pin      
}

/***********************************************************************************************************************/

void Linetracker::send_data(char command, char data)
{
	checksum = address + command + data;
	UART2Transmit(address);
	UART2Transmit(command);
	UART2Transmit(data);
	UART2Transmit(checksum);
}

/************************************ Setting of LSA08 *********************************/
void Linetracker::Set_Address(unsigned char add)	//calibrate
{
	send_data(ADDRESS_CHAR,add);
}

void Linetracker::Calibrate()	//calibrate
{
	send_data(CALIBRATE_CHAR,0X00);
}

void Linetracker::Set_Line() //dark line or light line
{
	send_data(LINEMODE_CHAR,0X00); //light on 0X00 and dark on 0X01
}

void Linetracker::Set_Line_Threshhold()	//Minimum value to be read
{
	send_data(THRESHOLD_CHAR,0X04);
}

void Linetracker::Set_Junction_Width()	//from 0 to 8
{
	send_data(JUNCTIONWIDTH_CHAR,0X08);		//width 8 Gamefield junction length 600 mm
}

void Linetracker::Set_LCD_Contrast() //set LCD contrast from 0 to 255
{
	send_data(LCDCONTRAST_CHAR,0X5A);	//contrast 90
}

void Linetracker::Set_LCD_Backlit() //set lcd backlit level
{
	send_data(LCDBACKLIT_CHAR,0x05); //light level 5
}

void Linetracker::Set_Baudrate()	//Set baudrate for communication
{
	send_data(BAUDRATE_CHAR,BAUDDRATE2);	//38400
}

void Linetracker::Uart_Data_Outputmode()	//calibrate
{
	send_data(UARTDATAOUTMODE_CHAR,0X02);		//sensor data from 0X00 left to 0X70 right;
}

void Linetracker::clear_junction()	//clear junction count of LSA08
{
	send_data(JUNCTIONINFO_CHAR,0X00);
}

/*****************************************************************************************************************************/

int Linetracker::Get_Junc_pincount()		//count from avr using junction pulse  //using polling
{
	if(junction_detect && !(READ(JUNCTION_PIN)))
	{
		junction_count++;
		junction_detect = false;
	}
	if(READ(JUNCTION_PIN))
	{
		junction_detect = true;
	}
	return junction_count;	
}

int Linetracker::Get_Sensors_Data()
{
	rcvdat = UART2Receive();
	return rcvdat;
}

int Linetracker::Get_JunctionCount()	//junction count from line tracker
{
	SET(UART_ENABLE);
	send_data('X',0X01);
	while(!((rcvdat=UART2Receive()) & 0X01));
	CLEAR(UART_ENABLE);
	return rcvdat;
}

