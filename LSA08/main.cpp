/*
 * LSA08.cpp
 *
 * Created: 12/24/2017 1:41:30 PM
 * Author : Subash Timilsina
 */ 

#include <avr/io.h>
#include "Linetracker.h"

int main(void)
{
	initUART0();
  Linetracker L1(0);	
  L1.initialise();
  L1.Calibrate();

    while (1) 
    {
		UART0TransmitString("Data\t");
		UART0TransmitData(L1.Get_Sensors_Data());
		UART0TransmitString("\tCount\t");
		UART0TransmitData(L1.Get_Junc_pincount());
		UART0Transmit('\n');
    }
}

