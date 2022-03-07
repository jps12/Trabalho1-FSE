/*
 * crc16.h
 *
 *  Created on: 18/03/2014
 *      Author: Renato Coral Sampaio
 */

#ifndef CRC16_H_
#define CRC16_H_

void CRC_configura(unsigned char *buffer, int size);
bool CRC_verifica(std::string buffer, int size);

#endif /* CRC16_H_ */
