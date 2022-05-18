/*
 * serial_sender.h
 *
 * Sends comma-separated values over serial to a plotter
 *
 *  Created on: 23/03/2022
 *      Author: Matt Suter
 *
 *  FitnessThur9-1
 */

#ifndef SERIAL_SENDER_H_
#define SERIAL_SENDER_H_


#define MAX_SERIAL_STR_LEN 16

void SerialInit (void); // Init this library
void SerialSend (char *pucBuffer); // Send an arbitrary string over serial
void SerialPlot(int16_t value1, int16_t value2, int16_t value3, int16_t value4); // Send four values over serial, to be used by a plotter programme

#endif /* SERIAL_SENDER_H_ */
