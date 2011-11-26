/*********************************************************\
 *  File: Socket.h                                       *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __PLCORE_NETWORK_SOCKET_H__
#define __PLCORE_NETWORK_SOCKET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Network/SocketAddress.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Socket class
*
*  @remarks
*    A socket is an object used to send and receive data.
*/
class Socket {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Construction/Destruction                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Socket();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSocket
		*    Socket to copy from
		*/
		PLCORE_API Socket(const Socket &cSocket);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nSocket
		*    System socket handle
		*
		*  @note
		*    - Should be used mainly to create an invalid socket ('INVALID_SOCKET')
		*/
		PLCORE_API Socket(handle nSocket);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~Socket();

		//[-------------------------------------------------------]
		//[ Connect                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether the socket is currently valid
		*
		*  @return
		*    'true' if the socket is currently valid, else 'false'
		*/
		PLCORE_API bool IsValid() const;

		/**
		*  @brief
		*    Closes the socket
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe it's already closed?)
		*/
		PLCORE_API bool Close();

		/**
		*  @brief
		*    Returns the socket address object
		*
		*  @return
		*    The socket address object
		*/
		PLCORE_API const SocketAddress &GetSocketAddress() const;

		/**
		*  @brief
		*    Establishes a connection to a host by using an address and port
		*
		*  @param[in] sAddress
		*    Address of the host to connect to
		*  @param[in] nPort
		*    Number of the port to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Connect(const String &sAddress, uint32 nPort);

		/**
		*  @brief
		*    Establishes a connection to a host by using a given socked address
		*
		*  @param[in] cSocketAddress
		*    Socket address of the host to connect to
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Connect(const SocketAddress &cSocketAddress);

		//[-------------------------------------------------------]
		//[ Bind address                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Associate local address with socket
		*
		*  @param[in] nPort
		*    Number of the port to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Bind(uint32 nPort);

		/**
		*  @brief
		*    Associate local address with socket
		*
		*  @param[in] sAddress
		*    Address of the host to bind to
		*  @param[in] nPort
		*    Number of the port to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Bind(const String &sAddress, uint32 nPort);

		/**
		*  @brief
		*    Associate local address with socket
		*
		*  @param[in] cSocketAddress
		*    Socket address of the host to bind to
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Bind(const SocketAddress &cSocketAddress);

		//[-------------------------------------------------------]
		//[ Listen for new connections                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Mark a socket as accepting connections
		*
		*  @param[in] nMaxQueue
		*    Maximum length of the queue of pending connections
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Listen(int nMaxQueue = 0) const;

		/**
		*  @brief
		*    Accept a connection on a socket (blocking)
		*
		*  @return
		*    Socket with the accepted connection or an invalid socket on error
		*/
		PLCORE_API Socket Accept() const;

		//[-------------------------------------------------------]
		//[ Data transfer                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sends data
		*
		*  @param[in] pBuffer
		*    Data to send, if a null pointer, nothing is send
		*  @param[in] nSize
		*    Size in bytes of the buffer to send, MUST be valid!
		*
		*  @return
		*    Total number of bytes sent which can be less than the number requested
		*    to be sent, negative value on error
		*/
		PLCORE_API int Send(const char *pBuffer, uint32 nSize) const;

		/**
		*  @brief
		*    Returns whether or not data is waiting to be received (non-blocking request)
		*
		*  @return
		*    'true' if data is waiting to be received, else 'false'
		*/
		PLCORE_API bool IsDataWaiting() const;

		/**
		*  @brief
		*    Receives data (blocking request)
		*
		*  @param[out] pBuffer
		*    Buffer that receives the data, if a null pointer, nothing can be received
		*  @param[in]  nSize
		*    Size in bytes of the buffer that receives the data, MUST be valid!
		*
		*  @return
		*    Total number of received bytes, negative value on error
		*
		*  @note
		*    - If there is currently not enough data available, this function will read as much
		*      as possible, meaning that less data can be read than requested.
		*/
		PLCORE_API int Receive(char *pBuffer, uint32 nSize) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SocketAddress m_cSocketAddress;	/**< Socket address */
		handle		  m_nSocket;		/**< Socket handle */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_NETWORK_SOCKET_H__
