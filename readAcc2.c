/**********************************************************
 *
 * readAcc.c
 *
 * Example code which reads acceleration in
 * three dimensions and displays the reulting data on
 * the Orbit OLED display.
 *
 *    C. P. Moore
 *    11 Feb 2020
 *
 **********************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/uart.h"
#include "inc/hw_i2c.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "../OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"
#include "acc.h"
#include "i2c_driver.h"

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} vector3_t;

/**********************************************************
 * Constants
 **********************************************************/
// Systick configuration

#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 16

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3
#define SW1 GPIO_PIN_4


//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX


char statusStr[MAX_STR_LEN + 1];
volatile uint8_t slowTick = false;
/*******************************************
 *      Local prototypes
 *******************************************/
void initClock (void);
void initDisplay (void);
void displayUpdate (char *str1, char *str2, int16_t num, uint8_t charLine);
void initAccl (void);

void initialiseUSB_UART (void);
void UARTSend (char *pucBuffer);

vector3_t getAcclData (void);

/***********************************************************
 * Initialisation functions: clock, SysTick, PWM
 ***********************************************************
 * Clock
 ***********************************************************/

void
SysTickIntHandler (void)
{
    static uint8_t tickCount = 0;
    const uint8_t ticksPerSlow = SYSTICK_RATE_HZ / SLOWTICK_RATE_HZ;

    updateButtons ();       // Poll the buttons
    if (++tickCount >= ticksPerSlow)
    {                       // Signal a slow tick
        tickCount = 0;
        slowTick = true;
    }

}

void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);




    SysTickPeriodSet (SysCtlClockGet () / SYSTICK_RATE_HZ);

    SysTickIntRegister (SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable ();
    SysTickEnable ();
}

/*********************************************************
 * initDisplay
 *********************************************************/
void
initDisplay (void)
{
    // Initialise the Orbit OLED display
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
    usnprintf(text_buffer, sizeof(text_buffer), "%s %s %d", str1, str2, (num*100)/256);
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, charLine);
}

/*********************************************************
 * initAccl
 *********************************************************/
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

    // set +-2g, 13 bit resolution, active low interrupts
    toAccl[0] = ACCL_DATA_FORMAT;
    toAccl[1] = (ACCL_RANGE_2G | ACCL_FULL_RES);
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

void initialiseUSB_UART (void)
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

void UARTSend (char *pucBuffer)
{
    // Loop while there are more characters to send.
    while(*pucBuffer)
    {
        // Write the next character to the UART Tx FIFO.
        UARTCharPut(UART_USB_BASE, *pucBuffer);
        pucBuffer++;
    }
}

void initLED() {
    // Enable GPIO Port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Set up the specific port pin as medium strength current & pull-down config.
    // Refer to TivaWare peripheral lib user manual for set up for configuration options
    GPIOPadConfigSet(GPIO_PORTF_BASE, GREEN_LED, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPadConfigSet(GPIO_PORTF_BASE, BLUE_LED, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPadConfigSet(GPIO_PORTF_BASE, RED_LED, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPD);
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    // Set data direction register as output
    GPIODirModeSet(GPIO_PORTF_BASE, GREEN_LED, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, BLUE_LED, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, RED_LED, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, SW1, GPIO_DIR_MODE_IN);

    // Write a zero to the output pin 3 on port F
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED, 0x00);
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


/********************************************************
 * main
 ********************************************************/
int
main (void)
{
    vector3_t acceleration_raw;
    vector3_t max_acceleration;

    initClock ();
    initAccl ();
    initDisplay ();
    initLED ();
    initialiseUSB_UART();
    uint8_t line = 2;

    OLEDStringDraw ("UP", 0, 0);

    while (1)
    {
//        max_acceleration.x = (acceleration_raw.x > max_acceleration.x) ? acceleration_raw.x : max_acceleration.x;
        SysCtlDelay (SysCtlClockGet () / 6);    // Approx 2 Hz
        acceleration_raw = getAcclData();
        displayUpdate ("Accl", "X", acceleration_raw.x, 1);
        displayUpdate ("Accl", "Y", acceleration_raw.y, 2);
        displayUpdate ("Accl", "Z", acceleration_raw.z, 3);


        if (slowTick)
        {
            slowTick = false;
            // Form and send a status message to the console
//          usprintf (statusStr, "UP=%2d DN=%2d | \r\n", upPushes, downPushes); // * usprintf
            usprintf (statusStr, "%d\n", acceleration_raw.z);
            UARTSend (statusStr);
        }

//        OLEDStringDraw ("            ", 0, line+1);
//        OLEDStringDraw ("            ", 0, line);
//        OLEDStringDraw ("            ", 0, line-1);
//        if (abs(acceleration_raw.z) > abs(acceleration_raw.y) && abs(acceleration_raw.z) > abs(acceleration_raw.x)) {
//            if (acceleration_raw.z > 0) {
//                OLEDStringDraw ("o", 6, line);
//            } else {
//                if (!GPIOPinRead(GPIO_PORTF_BASE, SW1)) {
//                    OLEDStringDraw ("x", 6, line);
//                } else {
//                    OLEDStringDraw ("IDIOT", 4, line);
//
//                }
//            }
//        } else if (abs(acceleration_raw.y) > abs(acceleration_raw.z) && abs(acceleration_raw.y) > abs(acceleration_raw.x)) {
//            if (acceleration_raw.y > 0) {
//                OLEDStringDraw ("^", 6, line);
//                OLEDStringDraw ("|", 6, line+1);
//            } else {
//                OLEDStringDraw ("v", 6, line);
//                OLEDStringDraw ("|", 6, line-1);
//            }
//
//        } else {
//            if (acceleration_raw.x > 0) {
//                OLEDStringDraw ("->", 6, line);
//            } else {
//                OLEDStringDraw ("<-", 6, line);
//            }
//
//        }


        if (abs((acceleration_raw.x * 100)/256) < 15 && abs((acceleration_raw.y * 100)/256) < 15) {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, GREEN_LED);
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, 0);
        }
    }
}
