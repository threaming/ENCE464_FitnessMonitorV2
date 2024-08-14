/**********************************************************
 *
 * i2c_hal_tiva.c
 *
 * Helper functions used to communicate with peripherals
 * on the Orbit BoosterPack via I2C.
 *
 * Functions excerpted from Digilent's Orbit Booster full
 * demo, available from
 * https://reference.digilentinc.com
 * /orbit_boosterpack/orbit_boosterpack
 *
 * C. P. Moore
 * 11 Feb 2020
 * 
 * Adapted to hal architecture by a. Ming
 * 13 Aug 2024
 *
 **********************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "i2c_hal.h"
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"

#define I2CPort         GPIO_PORTB_BASE
#define I2CSDAPort      GPIO_PORTB_BASE
#define I2CSCLPort      GPIO_PORTB_BASE
#define I2CSDA_PIN      GPIO_PIN_3
#define I2CSCL_PIN      GPIO_PIN_2

#define I2CSCL          GPIO_PB2_I2C0SCL
#define I2CSDA          GPIO_PB3_I2C0SDA


/* Helper Functions */
void Delay_us_(void)
{
    int16_t i=0;
    for (i=0;i<1000;i++)
        ;
}

uint32_t idSelector(I2c_Id_t I2C_ID) {
    switch (I2C_ID) {
        case I2C_ID_1:
            return I2C0_BASE;
            break;
        default:
            // Wrong ID selected
            return 0x00;
    }
}

/* ------------------------------------------------------------ */
/***    i2c_hal_register
**
**  Parameters:
**      I2C_ID  -   ID of registered 
**
**  Return Value:
**      true    -   Initialisation Successful
**      false   -   Initialisation Failed
**
**  Errors:
**      none
**
**  Description:
**      Initialises the I2C module on a tiva board.
**
*/
bool i2c_hal_register(I2c_Id_t I2C_ID) {
    uint32_t i2cBase = idSelector(I2C_ID);

    switch (i2cBase) {
        case I2C0_BASE:
            /* Enable I2C Peripheral */
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
            SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
            /* Set I2C GPIO pins */
            GPIOPinTypeI2C(I2CSDAPort, I2CSDA_PIN);
            GPIOPinTypeI2CSCL(I2CSCLPort, I2CSCL_PIN);
            GPIOPinConfigure(I2CSCL);
            GPIOPinConfigure(I2CSDA);
            /* Setup I2C */
            I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
            break;
        default:
            // Wrong ID selected
            return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/***    i2c_hal_transmit
**
**  Parameters:
**      I2C_ID  -   ID of registered 
**      pbData  -   Pointer to transmit buffer (read or write)
**      size    -   Number of byte transactions to take place
**      fRW     -   Flag for read/write
**      addrd   -   I2C address of the device
**
**  Return Value:
**      none
**
**  Errors:
**      none
**
**  Description:
**      Transmits data to a device via the I2C bus. Differs from
**      I2C EEPROM Transmit in that the registers in the device it
**      is addressing are addressed with a single byte. Lame, but..
**      it works.
**
*/
void i2c_hal_transmit(I2c_Id_t I2C_ID, char* pbData, uint32_t size, bool fRW, char addr){
    int32_t  i;
    char *  pbTemp;
    pbTemp = pbData;
    uint32_t i2cBase = idSelector(I2C_ID);

    /* ID selection assertion*/
    if(!i2cBase) return;

    /* Initiate Transmission */
    I2CMasterSlaveAddrSet(i2cBase, addr, I2C_WRITE);
    I2CMasterDataPut(i2cBase, *pbTemp);
    I2CMasterControl(i2cBase, I2C_MASTER_CMD_BURST_SEND_START);

    Delay_us_();
    while(!I2CMasterBusBusy(i2cBase));
    pbTemp++;

    /* Read Data */
    if(fRW == I2C_READ) {

        /* Restart I2C on reading */
        I2CMasterSlaveAddrSet(i2cBase, addr, I2C_READ);
        while(I2CMasterBusy(i2cBase));

        /* Configure I2C Bus */
        for(i = 0; i < size; i++) {
            if(size == i + 1 && size == 1) {
                I2CMasterControl(i2cBase, I2C_MASTER_CMD_SINGLE_RECEIVE);
                Delay_us_();
                while(I2CMasterBusy(i2cBase));
            }
            else if(size == i + 1 && size > 1) {
                I2CMasterControl(i2cBase, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
                Delay_us_();
                while(I2CMasterBusy(i2cBase));
            }
            else if(i == 0) {
                I2CMasterControl(i2cBase, I2C_MASTER_CMD_BURST_RECEIVE_START);
                Delay_us_();
                while(I2CMasterBusy(i2cBase));
                while(!I2CMasterBusBusy(i2cBase));
            }
            else {
                I2CMasterControl(i2cBase, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
                Delay_us_();
                while(I2CMasterBusy(i2cBase));
                while(!I2CMasterBusBusy(i2cBase));
            }

            while(I2CMasterBusy(i2cBase));
            /* Read Data */
            *pbTemp = (char)I2CMasterDataGet(i2cBase);
            pbTemp++;

        }

    }

    /* Write Data*/
    else if(fRW == I2C_WRITE) {
        /* Send Data */
        for(i = 0; i < size; i++) {
            I2CMasterDataPut(i2cBase, *pbTemp);
            while(I2CMasterBusy(i2cBase));
            if(i == size - 1) {
                I2CMasterControl(i2cBase, I2C_MASTER_CMD_BURST_SEND_FINISH);
                Delay_us_();
                while(I2CMasterBusy(i2cBase));
            }
            else {
                I2CMasterControl(i2cBase, I2C_MASTER_CMD_BURST_SEND_CONT);
                Delay_us_();
                while(I2CMasterBusy(i2cBase));
                while(!I2CMasterBusBusy(i2cBase));
            }

            pbTemp++;
        }

    }
}