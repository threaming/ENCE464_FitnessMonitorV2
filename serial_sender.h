/*
 * serial_sender.h
 *
 *  Created on: 23/03/2022
 *      Author: mattr
 */

#ifndef SERIAL_SENDER_H_
#define SERIAL_SENDER_H_


#define MAX_SERIAL_STR_LEN 16

void SerialInit (void);
void SerialSend (char *pucBuffer);
void SerialPlot(int16_t value1, int16_t value2, int16_t value3, int16_t value4);

#endif /* SERIAL_SENDER_H_ */
