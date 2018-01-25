/* 
* Linetracker.h
*
* Created: 12/24/2017 1:42:17 PM
* Author: Subash Timilsina
*/


#ifndef __LINETRACKER_H__
#define __LINETRACKER_H__

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "headers.h"

#define JUNCTION_PIN		J,0  // rx2 h0 tx2 h1 rx3 J0 tx3 j1
#define UART_ENABLE			J,1

#define ADDRESS_CHAR			'A'
#define CALIBRATE_CHAR			'C'
#define LINEMODE_CHAR			'L'
#define THRESHOLD_CHAR			'T'
#define JUNCTIONWIDTH_CHAR		'J'
#define LCDCONTRAST_CHAR		'S'
#define LCDBACKLIT_CHAR			'B'
#define BAUDRATE_CHAR			'R'
#define UARTDATAOUTMODE_CHAR	'D'	
#define JUNCTIONINFO_CHAR		'X'
#define SENSORINFO_CHAR			'O'


#define BAUDDRATE0		0X00	//9600
#define BAUDDRATE1		0X01	//19200
#define BAUDDRATE2		0X02	//38400
#define BAUDDRATE3		0X03	//57600
#define BAUDDRATE4		0X04	//115200
#define BAUDDRATE5		0X05	//230400

// optional if junction pin used in interrupt

#define	 JUNCTION_INT		INT0

#define	 INT_JUNC_ISC1			ISC01
#define	 INT_JUNC_INTF			INTF0
#define  JUNC_EICR				EICRA
#define  INT_JUNC_VECT			INT0_vect


class Linetracker
{
	private:
	char address;
	char checksum;
	int junction_count;
	int rcvdat;
	bool junction_detect;

	public:
	Linetracker(int addr);
	void initialise();
	void initialise_interrupt();
	void Off_interrupt();
	void send_data(char command, char data);
	void Set_Address(unsigned char add);
	void Calibrate();
	void Set_Line();
	void Set_Line_Threshhold();
	void Set_Junction_Width();
	void Set_LCD_Contrast();
	void Set_LCD_Backlit();
	void Set_Baudrate();
	void clear_junction();
	void Uart_Data_Outputmode();
	int Get_Junc_pincount();
	inline bool Get_Junc_State(){return junction_detect;};
	inline void Inc_junc_count(){junction_count++;};
	inline void Set_Junction_count(int val){junction_count = val;};
	int Get_Sensors_Data();
	int Get_JunctionCount();
};

#endif //__LINETRACKER_H__
