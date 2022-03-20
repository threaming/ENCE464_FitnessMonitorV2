//********************************************************
//
// uartDemo.c - Example code for ENCE361
//
// Link with modules:  buttons2, OrbitOLEDInterface
//
// Author:  P.J. Bones	UCECE
// Last modified:	16.4.2018
//

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "buttons4.h"
#include "acc.h"
#include "i2c_driver.h"
#include "circBufV.h"
#include "math.h"


//********************************************************
// Constants
//********************************************************
#define SYSTICK_RATE_HZ 1000
#define SLOWTICK_RATE_HZ 100
#define ACC_DATA_RATE 200
#define MAX_STR_LEN 16
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX
//--Circular Buffer
#define BUF_SIZE 25

//********************************************************
// Prototypes
//********************************************************
void SysTickIntHandler (void);
void initClock (void);
void initSysTick (void);
void initDisplay (void);
void initialiseUSB_UART (void);
void UARTSend (char *pucBuffer);
void initAccl (void);
void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);
vector3_t getAcclData (void);

//********************************************************
// Global variables
//********************************************************
char statusStr[MAX_STR_LEN + 1];
volatile uint8_t slowTick = false;
volatile uint8_t accTick = false;
static circBuf_t g_inBuffer;        // Buffer of size BUF_SIZE integers (sample values)

//*******************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*******************************************************************
void
SysTickIntHandler (void)
{
    static uint8_t tickCount = 0;
    static uint8_t acc_tickCount = 0;
//    const uint8_t ticksPerSlow = SYSTICK_RATE_HZ / SLOWTICK_RATE_HZ;
    const uint8_t ACCticksPerSlow = ACC_DATA_RATE / SLOWTICK_RATE_HZ;
    const uint8_t ticksPerACC = SYSTICK_RATE_HZ / ACC_DATA_RATE;

    updateButtons ();		// Poll the buttons
    if (acc_tickCount >= ACCticksPerSlow)
    {                       // Signal a slow tick
        acc_tickCount = 0;
        slowTick = true;
    }
    if (++tickCount >= ticksPerACC)
    {                       // Signal a slow tick
        tickCount = 0;
        accTick = true;
        acc_tickCount++;
    }
//    if (++tickCount >= ticksPerSlow)
//    {                       // Signal a slow tick
//        tickCount = 0;
//        slowTick = true;
////        acc_tickCount++;
//    }

}

//********************************************************
// Initialisation functions: clock, SysTick, display, UART
//********************************************************
void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}

//*******************************************************************
void
initSysTick (void)
{
    //
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet (SysCtlClockGet () / SYSTICK_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister (SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
}

// *******************************************************
void
initDisplay (void)
{
  // intialise the Orbit OLED display
	OLEDInitialise ();
}


//*****************************************************************************
// Function to display a changing message on the display.
// The display has 4 rows of 16 characters, with 0, 0 at top left.
//*****************************************************************************
void
displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine)
{
    char text_buffer[17];           //Display fits 16 characters wide.

    // "Undraw" the previous contents of the line to be updated.
    OLEDStringDraw ("                ", 0, charLine);
    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    usnprintf(text_buffer, sizeof(text_buffer), "%s %s %3d", str1, str2, num);
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, charLine);
}


//********************************************************
// initialiseUSB_UART - 8 bits, 1 stop bit, no parity
//********************************************************
void
initialiseUSB_UART (void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    //
    SysCtlPeripheralEnable(UART_USB_PERIPH_UART);
    SysCtlPeripheralEnable(UART_USB_PERIPH_GPIO);
    //
    // Select the alternate (UART) function for these pins.
    //
    GPIOPinTypeUART(UART_USB_GPIO_BASE, UART_USB_GPIO_PINS);
    GPIOPinConfigure (GPIO_PA0_U0RX);
    GPIOPinConfigure (GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_USB_BASE, SysCtlClockGet(), BAUD_RATE,
    		UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
			UART_CONFIG_PAR_NONE);
    UARTFIFOEnable(UART_USB_BASE);
    UARTEnable(UART_USB_BASE);
}


//**********************************************************************
// Transmit a string via UART0
//**********************************************************************
void
UARTSend (char *pucBuffer)
{
    // Loop while there are more characters to send.
    while(*pucBuffer)
    {
        // Write the next character to the UART Tx FIFO.
        UARTCharPut(UART_USB_BASE, *pucBuffer);
        pucBuffer++;
    }
}

void
initAccl (void)
{
    char    toAccl[] = {0, 0};  // parameter, value

    /*
     * Enable I2C Peripheral
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    /*
     * Set I2C GPIO pins
     */
    GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
    GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
    GPIOPinConfigure(I2CSCL);
    GPIOPinConfigure(I2CSDA);

    /*
     * Setup I2C
     */
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);

    GPIOPinTypeGPIOInput(ACCL_INT2Port, ACCL_INT2);

    //Initialize ADXL345 Acceleromter

    // set +-16g, 13 bit resolution, active low interrupts
    toAccl[0] = ACCL_DATA_FORMAT;
    toAccl[1] = (ACCL_RANGE_16G | ACCL_FULL_RES);
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_PWR_CTL;
    toAccl[1] = ACCL_MEASURE;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);


    toAccl[0] = ACCL_BW_RATE;
    toAccl[1] = ACCL_RATE_100HZ;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_INT;
    toAccl[1] = 0x00;       // Disable interrupts from accelerometer.
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_X;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Y;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);

    toAccl[0] = ACCL_OFFSET_Z;
    toAccl[1] = 0x00;
    I2CGenTransmit(toAccl, 1, WRITE, ACCL_ADDR);
}

vector3_t addVector3 (vector3_t vector_1, vector3_t vector_2) {
    vector3_t result;
    result.x = vector_1.x + vector_2.x;
    result.y = vector_1.y + vector_2.y;
    result.z = vector_1.z + vector_2.z;
    return result;
}

vector3_t meanVector3 (vector3_t vector) {
    vector3_t result;
    result.x = vector.x / BUF_SIZE;
    result.y = vector.y / BUF_SIZE;
    result.z = vector.z / BUF_SIZE;
    return result;
}

/********************************************************
 * Function to read accelerometer
 ********************************************************/
vector3_t
getAcclData (void)
{
    char    fromAccl[] = {0, 0, 0, 0, 0, 0, 0}; // starting address, placeholders for data to be read.
    vector3_t acceleration;
    uint8_t bytesToRead = 6;

    fromAccl[0] = ACCL_DATA_X0;
    I2CGenTransmit(fromAccl, bytesToRead, READ, ACCL_ADDR);

    acceleration.x = (fromAccl[2] << 8) | fromAccl[1]; // Return 16-bit acceleration readings.
    acceleration.y = (fromAccl[4] << 8) | fromAccl[3];
    acceleration.z = (fromAccl[6] << 8) | fromAccl[5];

    return acceleration;
}


int
main(void)
{
    vector3_t acceleration_raw;
    vector3_t sum;
    uint16_t i;
    vector3_t mean;
    uint8_t stepping = false;
    uint32_t steps = 0;
    uint32_t combined;
	
	initClock ();
	initDisplay ();
	initButtons ();
	initialiseUSB_UART ();
	initSysTick ();
    initAccl ();
    initCircBuf (&g_inBuffer, BUF_SIZE);

    OLEDStringDraw ("UART Acc Test", 0, 0);

	while(1)
	{
//	    SysCtlDelay (SysCtlClockGet () / 6);    // Approx 2 Hz

	    if(accTick) {
	        accTick = 0;

            acceleration_raw = getAcclData();
        //	    char string[MAX_STR_LEN + 1];
        //	    usprintf (string, "%d", acceleration_raw.z);
            writeCircBuf (&g_inBuffer, acceleration_raw);



            sum.x = 0;
            sum.y = 0;
            sum.z = 0;
            for (i = 0; i < BUF_SIZE; i++)
                sum = addVector3 (sum, readCircBuf(&g_inBuffer));

            mean = meanVector3(sum);

            combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);

            if (combined >= 270 && stepping == false) {
                stepping = true;
                steps++;
            } else if (combined <= 235) {
                stepping = false;
            }

//        usprintf(statusStr, "%d", mean.x);
//        OLEDStringDraw (statusStr, 0, 1);
	    }
		// Is it time to send a message?
        if (slowTick)
        {
        	slowTick = false;
        	// Form and send a status message to the console
        	usprintf (statusStr, "%d, %d, %d, %d\n", steps, mean.x, mean.y, mean.z); // * usprintf
//        	usprintf (statusStr, "%d, %d\n", steps, combined);
            UARTSend (statusStr);

        	// Send message to booster display
            char stepsStr[MAX_STR_LEN + 1];
        	usprintf (stepsStr, "# of Steps: %d", steps);
            OLEDStringDraw (stepsStr, 0, 2);
            OLEDStringDraw (statusStr, 0, 1);
        	// Just adding this so something changes
        }
	}

}

