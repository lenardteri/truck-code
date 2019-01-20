/**\file
 *
 * jbus_send.h
 *
 * This file contains the JBus class, which is responsible for collecting J1939
 * messages from the CAN card. The messages are later translated to their
 * message-specific format using classes in j1939_interpreters.h.
 *
 * @author Abdul Rahman Kreidieh
 * @version 1.0.0
 * @date January 15, 2019
 */

#ifndef SRC_JBUS_JBUS_H_
#define SRC_JBUS_JBUS_H_

#include "j1939_struct.h"
#include <string>


/** repetition interval should be 10 milliseconds for engine, 50 milliseconds
 * for retarder, 40 milliseconds for EBS */
#define JBUS_INTERVAL_MSECS	5


/** Primary class used to communicate with the CAN card.
 *
 * This class is responsible for initializing communication with the CAN card,
 * as well as send and receive messages from the CAN card.
 */
class JBus
{
public:
	/** Initialize the jbus connection.
	 *
	 * Opens a connection with the CAN card. If access to channel and connection
	 * IDs used by the CAN driver are actually required outside of the CAN
	 * driver, and its client calls, we must call operations on the "fd"
	 * returned by can_open (actually a pointer) to retrieve them.
	 *
	 * @param filename location of the CAN data stream
	 * @param flags flag variable for the open() process. If RDONLY, this method
	 * will arm the pulse
	 * @param p_other pointer in case other information is needed for some
	 * driver initialization
	 * @return file descriptor that will be used in all subsequent calls, -1 if
	 * an error was experienced
	 */
	virtual int init(std::string filename, int flags, void *p_other);

	/** Update a PDU object with information from the can card.
	 *
	 * @param fd file descriptor acquired while opening the connection
	 * @param pdu pointer to a generic PDU message that will be updated with
	 * information received from the CAN card
	 * @param extended updated to be 1 if the identifier is in extended (29 bit)
	 * format, 0 if it is in the unextended (11 bit) format
	 * @param slot external slot, internal type
	 * @return J1939_CAN_MESSAGE_ERROR (0) on can_read failure. Otherwise
	 * returns the (positive) number of bytes in the message, no fatal error
	 * conditions are recognized.
	 */
	virtual int receive(int fd, j1939_pdu_typ *pdu, int *extended, int *slot);

	/** Wrapper for the close call; some drivers are not file structured
	 * and may require disconnect functions to be called.
	 *
	 * Sets the input "file descriptor" to NULL, so that attempts to close twice
	 * can be caught. Requires passing address of fd/handle to this routine.
	 *
	 * @param pfd pointer to file descriptor
	 * @return -1 if handle is NULL, otherwise returns the value from the close
	 * of the real fd
	 */
	virtual int close_conn(int *pfd);

	virtual ~JBus();
};


#endif /* SRC_JBUS_JBUS_H_ */