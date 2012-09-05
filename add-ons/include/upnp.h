/*******************************************************************************
 *
 * Copyright (c) 2000-2003 Intel Corporation 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met: 
 *
 * * Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer. 
 * * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * * Neither name of Intel Corporation nor the names of its contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef UPNP_H
#define UPNP_H

/** @name The API */

/*! @{ */


#if defined MYLIB_LARGEFILE_SENSITIVE && _FILE_OFFSET_BITS+0 != 64
	#if defined __GNUC__
		#warning libupnp requires largefile mode - use AC_SYS_LARGEFILE
	#else
		#error  libupnp requires largefile mode - use AC_SYS_LARGEFILE
	#endif
#endif 


#include <stdio.h>
#ifndef WIN32
	#include <sys/param.h>
#endif
#if (defined(BSD) && BSD >= 199306)
	#include <time.h>
#endif
#include "ixml.h"
/* #include "upnpconfig.h" */


#ifndef UPNP_CONFIG_H
#define UPNP_CONFIG_H


/***************************************************************************
 * Library version
 ***************************************************************************/

/** The library version (string) e.g. "1.3.0" */
#define UPNP_VERSION_STRING "1.6.6"

/** Major version of the library */
#define UPNP_VERSION_MAJOR 1

/** Minor version of the library */
#define UPNP_VERSION_MINOR 6

/** Patch version of the library */
#define UPNP_VERSION_PATCH 6

/** The library version (numeric) e.g. 10300 means version 1.3.0 */
#define UPNP_VERSION    \
  ((UPNP_VERSION_MAJOR*100 + UPNP_VERSION_MINOR)*100 + UPNP_VERSION_PATCH)

#define UPNP_HAVE_CLIENT 1

#endif


#ifdef WIN32
	#ifndef UPNP_STATIC_LIB
		#ifdef LIBUPNP_EXPORTS
			/* set up declspec for dll export to make functions visible to library users */
			#define EXPORT_SPEC __declspec(dllexport)
		#else
			#define EXPORT_SPEC __declspec(dllimport)
		#endif
	#else
		#define EXPORT_SPEC
	#endif
	#ifdef UPNP_USE_MSVCPP
		/* define some things the M$ VC++ doesn't know */
		#define UPNP_INLINE
		typedef __int64 int64_t;
		#define PRId64 "I64d"
		#define PRIzu "lu"
	#endif
	#ifdef UPNP_USE_BCBPP
		/* define some things Borland Builder doesn't know */
		#define UPNP_INLINE inline
		typedef __int64 int64_t;
#warning The Borland C compiler is probably broken on PRId64, please someone provide a proper fix here
		#define PRId64 "I64d"
		#define PRIzu "zu"
	#endif
#else
	#define EXPORT_SPEC
	#define UPNP_INLINE inline
	/* Invented this macro so that we can live a little longer with MSVC lack of C99. */
	#define PRIzu "zu"
#endif

/*
 * Defining this macro here gives some interesting information about unused
 * functions in the code. Of course, this should never go uncommented on a
 * release.
 */
/*#define inline*/


#ifndef WIN32
	#define UpnpCloseSocket close
#else
	#define UpnpCloseSocket closesocket
	#define fseeko fseek
#endif
#ifndef WIN32
	#define SOCKET int
#endif


#ifndef WIN32
	#include <netinet/in.h>
#else
	#include <winsock2.h>
	#include <time.h>
#endif


#include <sys/types.h>


#define NUM_HANDLE 200
#define LINE_SIZE  180
#define NAME_SIZE  256
#define MNFT_NAME_SIZE  64
#define MODL_NAME_SIZE  32
#define SERL_NUMR_SIZE  64
#define MODL_DESC_SIZE  64
#define UPNP_INFINITE -1
#define UPNP_USING_CHUNKED			-3
#define UPNP_UNTIL_CLOSE			-4


/** @name Error codes 
 *  The functions in the SDK API can return a variety of error 
 *  codes to describe problems encountered during execution.  This section 
 *  lists the error codes and provides a brief description of what each error 
 *  code means.  Refer to the documentation for each function for a 
 *  description of what an error code means in that context.
 */
/*! @{ */

/** @name UPNP_E_SUCCESS [0]
 *  {\tt UPNP_E_SUCCESS} signifies that the operation completed successfully.
 *  For asynchronous functions, this only means that the packet generated by 
 *  the operation was successfully transmitted on the network.  The result of 
 *  the entire operation comes as part of the callback for that operation.
 */
/*! @{ */
#define UPNP_E_SUCCESS          0
/*! @} */

/** @name UPNP_E_INVALID_HANDLE [-100]
 *  {\tt UPNP_E_INVALID_HANDLE} signifies that the handle passed to a 
 *  function is not a recognized as a valid handle.
 */
/*! @{ */
#define UPNP_E_INVALID_HANDLE   -100
/*! @} */

/** @name UPNP_E_INVALID_PARAM [-101]
 *  {\tt UPNP_E_INVALID_PARAM} signifies that one or more of the parameters 
 *  passed to the function is not valid.  Refer to the documentation for each 
 *  function for more information on the valid ranges of the parameters.
 */
/*! @{ */
#define UPNP_E_INVALID_PARAM    -101
/*! @} */

/** @name UPNP_E_OUTOF_HANDLE [-102]
 *  {\tt UPNP_E_OUTOF_HANDLE} signifies that the SDK does not have any
 *  more space for additional handles.  The SDK allocates space for only 
 *  a few handles in order to conserve memory.
 */
/*! @{ */
#define UPNP_E_OUTOF_HANDLE     -102
/*! @} */

#define UPNP_E_OUTOF_CONTEXT    -103

/** @name UPNP_E_OUTOF_MEMORY [-104]
 *  {\tt UPNP_E_OUTOF_MEMORY} signifies that not enough resources are 
 *  currently available to complete the operation.  Most operations require 
 *  some free memory in order to complete their work.
 */
/*! @{ */
#define UPNP_E_OUTOF_MEMORY     -104
/*! @} */

/** @name UPNP_E_INIT [-105]
 *  {\tt UPNP_E_INIT} signifies that the SDK has already been 
 *  initialized.  The SDK needs to be initialied only once per process.
 *  Any additional initialization attempts simply return this error with
 *  no other ill effects.
 */
/*! @{ */
#define UPNP_E_INIT             -105
/*! @} */

#define UPNP_E_BUFFER_TOO_SMALL -106

/** @name UPNP_E_INVALID_DESC [-107]
 *  {\tt UPNP_E_INVALID_DESC} signifies that the description document passed
 *  to {\bf UpnpRegisterRootDevice} or {\bf UpnpRegisterRootDevice2} is an 
 *  invalid description document.  
 */
/*! @{ */
#define UPNP_E_INVALID_DESC     -107
/*! @} */

/** @name UPNP_E_INVALID_URL [-108]
 *  {\tt UPNP_E_INVALID_URL} signifies that a URL passed into the function
 *  is invalid.  The actual cause is function specific, but in general, the
 *  URL itself might be malformed (e.g. have invalid characters in it) or
 *  the host might be unreachable.
 */
/*! @{ */
#define UPNP_E_INVALID_URL      -108
/*! @} */

#define UPNP_E_INVALID_SID      -109
#define UPNP_E_INVALID_DEVICE   -110

/** @name UPNP_E_INVALID_SERVICE [-111]
 *  {\tt UPNP_E_INVALID_SERVICE} is returned only by {\bf UpnpNotify}, 
 *  {\bf UpnpNotifyExt}, {\bf UpnpAcceptSubscription}, and 
 *  {\bf UpnpAcceptSubscriptionExt} to signify that the device ID/service
 *  ID pair does not refer to a valid service.
 */
/*! @{ */
#define UPNP_E_INVALID_SERVICE  -111
/*! @} */

/** @name UPNP_E_BAD_RESPONSE [-113]
 *  {\tt UPNP_E_BAD_RESPONSE} signifies that the response received from the 
 *  remote side of a connection is not correct for the protocol.  This applies
 *  to the GENA, SOAP, and HTTP protocols.
 */
/*! @{ */
#define UPNP_E_BAD_RESPONSE     -113
/*! @} */

#define UPNP_E_BAD_REQUEST      -114

/** @name UPNP_E_INVALID_ACTION [-115]
 *  {\tt UPNP_E_INVALID_ACTION} signifies that the SOAP action message is 
 *  invalid.  This can be because the DOM document passed to the function was
 *  malformed or the action message is not correct for the given action.
 */
/*! @{ */
#define UPNP_E_INVALID_ACTION   -115
/*! @} */

/** @name UPNP_E_FINISH [-116]
 *  {\tt UPNP_E_FINISH} signifies that {\bf UpnpInit} has not been called, or
 *  that {\bf UpnpFinish} has already been called.  None of the API functions 
 *  operate until {\bf UpnpInit} successfully completes.
 */
/*! @{ */
#define UPNP_E_FINISH           -116
/*! @} */

/** @name UPNP_E_INIT_FAILED [-117]
 *  {\tt UPNP_E_INIT_FAILED} signifies that {\bf UpnpInit} cannot complete.  
 *  The typical reason is failure to allocate sufficient resources.
 */
/*! @{ */
#define UPNP_E_INIT_FAILED      -117
/*! @} */

/** @name UPNP_E_URL_TOO_BIG [-118]
 *  {\tt UPNP_E_URL_TOO_BIG} signifies that the URL passed into a function 
 *  is too long.  The SDK limits URLs to 180 characters in length.  
 */
#define UPNP_E_URL_TOO_BIG      -118

/** @name UPNP_E_BAD_HTTPMSG [-119]
 *  {\tt UPNP_E_BAD_HTTPMSG} signifies that the HTTP message contains invalid
 *  message headers.  The error always refers to the HTTP message header 
 *  received from the remote host.  The main areas where this occurs are in
 *  SOAP control messages (e.g. {\bf UpnpSendAction}), GENA subscription
 *  message (e.g. {\bf UpnpSubscribe}), GENA event notifications (e.g. {\bf
 *  UpnpNotify}), and HTTP transfers (e.g. {\bf UpnpDownloadXmlDoc}).
 */
/*! @{ */
#define UPNP_E_BAD_HTTPMSG      -119
/*! @} */

/** @name UPNP_E_ALREADY_REGISTERED [-120]
 *  {\tt UPNP_E_ALREADY_REGISTERED} signifies that a client or a device is
 *  already registered.  The SDK currently has a limit of one registered 
 *  client and one registered device per process.
 */
/*! @{ */
#define UPNP_E_ALREADY_REGISTERED -120
/*! @} */

/** @name UPNP_E_NETWORK_ERROR [-200]
 *  {\tt UPNP_E_NETWORK_ERROR} signifies that a network error occurred.  It 
 *  is the generic error code for network problems that are not covered under 
 *  one of the more specific error codes.  The typical meaning is the SDK 
 *  failed to read the local IP address or had problems configuring one of
 *  the sockets.
 */
/*! @{ */
#define UPNP_E_NETWORK_ERROR    -200
/*! @} */

/** @name UPNP_E_SOCKET_WRITE [-201]
 *  {\tt UPNP_E_SOCKET_WRITE} signifies an error writing to a socket.  This
 *  occurs in any function that makes network connections, such 
 *  as discovery (e.g. {\bf UpnpSearchAsync} or {\bf UpnpSendAdvertisement}), 
 *  control (e.g. {\bf UpnpSendAction}), eventing (e.g. {\bf UpnpNotify}), 
 *  and HTTP functions (e.g. {\bf UpnpDownloadXmlDoc}).
 */
/*! @{ */
#define UPNP_E_SOCKET_WRITE     -201
/*! @} */

/** @name UPNP_E_SOCKET_READ [-202]
 *  {\tt UPNP_E_SOCKET_READ} signifies an error reading from a socket.  This
 *  occurs in any function that makes network connections, such 
 *  as discovery (e.g. {\bf UpnpSearchAsync} or {\bf UpnpSendAdvertisement}), 
 *  control (e.g. {\bf UpnpSendAction}), eventing (e.g. {\bf UpnpNotify}), 
 *  and HTTP functions (e.g. {\bf UpnpDownloadXmlDoc}).
 */
/*! @{ */
#define UPNP_E_SOCKET_READ      -202
/*! @} */

/** @name UPNP_E_SOCKET_BIND [-203]
 *  {\tt UPNP_E_SOCKET_BIND} signifies that the SDK had a problem binding
 *  a socket to a network interface.  This occurs in any function that makes 
 *  network connections, such as discovery (e.g. {\bf UpnpSearchAsync} or 
 *  {\bf UpnpSendAdvertisement}), control (e.g. {\bf UpnpSendAction}), eventing 
 *  (e.g. {\bf UpnpNotify}), and HTTP functions (e.g. 
 *  {\bf UpnpDownloadXmlDoc}).
 */
/*! @{ */
#define UPNP_E_SOCKET_BIND      -203
/*! @} */

/** @name UPNP_E_SOCKET_CONNECT [-204]
 *  {\tt UPNP_E_SOCKET_CONNECT} signifies that the SDK had a problem
 *  connecting to a remote host.  This occurs in any function that makes 
 *  network connections, such as discovery (e.g. {\bf UpnpSearchAsync} or 
 *  {\bf UpnpSendAdvertisement}), control (e.g. {\bf UpnpSendAction}), eventing 
 *  (e.g. {\bf UpnpNotify}), and HTTP functions (e.g. 
 *  {\bf UpnpDownloadXmlDoc}).
 */
/*! @{ */
#define UPNP_E_SOCKET_CONNECT   -204
/*! @} */

/** @name UPNP_E_OUTOF_SOCKET [-205]
 *  {\tt UPNP_E_OUTOF_SOCKET} signifies that the SDK cannot create any
 *  more sockets.  This occurs in any function that makes 
 *  network connections, such as discovery (e.g. {\bf UpnpSearchAsync} or 
 *  {\bf UpnpSendAdvertisement}), control (e.g. {\bf UpnpSendAction}), eventing 
 *  (e.g. {\bf UpnpNotify}), and HTTP functions (e.g. 
 *  {\bf UpnpDownloadXmlDoc}).
 */
/*! @{ */
#define UPNP_E_OUTOF_SOCKET     -205
/*! @} */

/** @name UPNP_E_LISTEN [-206]
 *  {\tt UPNP_E_LISTEN} signifies that the SDK had a problem setting the
 *  socket to listen for incoming connections.  This error only happens during
 *  initialization (i.e. {\bf UpnpInit}).
 */
/*! @{ */
#define UPNP_E_LISTEN           -206
/*! @} */

/** @name UPNP_E_TIMEDOUT [-207]
 *  {\tt UPNP_E_TIMEDOUT} signifies that too much time elapsed before the
 *  required number of bytes were sent or received over a socket.  This error
 *  can be returned by any function that performs network operations.
 */
/*! @{ */
#define UPNP_E_TIMEDOUT         -207
/*! @} */

/** @name UPNP_E_SOCKET_ERROR [-208]
 *  {\tt UPNP_E_SOCKET_ERROR} is the generic socket error code for
 *  conditions not covered by other error codes.  This error can be returned
 *  by any function that performs network operations.
 */
/*! @{ */
#define UPNP_E_SOCKET_ERROR	    -208
/*! @} */

#define UPNP_E_FILE_WRITE_ERROR -209

/** @name UPNP_E_CANCELED [-210]
 *  {\tt UPNP_E_CANCELED} signifies that the operation was canceled. This
 *  error can be returned by any function that allows for external
 *  cancelation.
 */
/*! @{ */
#define UPNP_E_CANCELED         -210
/*! @} */

#define UPNP_E_EVENT_PROTOCOL         -300

/** @name UPNP_E_SUBSCRIBE_UNACCEPTED [-301]
 *  {\tt UPNP_E_SUBSCRIBE_UNACCEPTED} signifies that a subscription
 *  request was rejected from the remote side.  
 */
/*! @{ */
#define UPNP_E_SUBSCRIBE_UNACCEPTED   -301
/*! @} */

/** @name UPNP_E_UNSUBSCRIBE_UNACCEPTED [-302]
 *  {\tt UPNP_E_UNSUBSCRIBE_UNACCEPTED} signifies that an unsubscribe
 *  request was rejected from the remote side.
 */
/*! @{ */
#define UPNP_E_UNSUBSCRIBE_UNACCEPTED -302
/*! @} */

/** @name UPNP_E_NOTIFY_UNACCEPTED [-303]
 *  {\tt UPNP_E_NOTIFY_UNACCEPTED} signifies that the remote host did not
 *  accept the notify sent from the local device.
 */
/*! @{ */
#define UPNP_E_NOTIFY_UNACCEPTED      -303
/*! @} */

/** @name UPNP_E_INVALID_ARGUMENT [-501]
 *  {\tt UPNP_E_INVALID_ARGUMENT} signifies that one or more of the parameters
 *  passed to a function is invalid.  Refer to the individual function
 *  descriptions for the acceptable ranges for parameters.
 */
/*! @{ */
#define UPNP_E_INVALID_ARGUMENT       -501
/*! @} */

/** @name UPNP_E_FILE_NOT_FOUND [-502]
 *  {\tt UPNP_E_FILE_NOT_FOUND} signifies that the filename passed
 *  to one of the device registration functions was not found or was not
 *  accessible.
 */
/*! @{ */
#define UPNP_E_FILE_NOT_FOUND         -502
/*! @} */

/** @name UPNP_E_FILE_READ_ERROR [-503]
 *  {\tt UPNP_E_FILE_READ_ERROR} signifies an error when reading a file.
 */
/*! @{ */
#define UPNP_E_FILE_READ_ERROR        -503
/*! @} */

/** @name UPNP_E_EXT_NOT_XML [-504]
 *  {\tt UPNP_E_EXT_NOT_XML} signifies that the file name of the description
 *  document passed to {\bf UpnpRegisterRootDevice2} does not end in ".xml".
 */
/*! @{ */
#define UPNP_E_EXT_NOT_XML            -504
/*! @} */

#define UPNP_E_NO_WEB_SERVER          -505
#define UPNP_E_OUTOF_BOUNDS	      -506

/** @name UPNP_E_NOT_FOUND [-507]
 *  {\tt UPNP_E_NOT_FOUND} signifies that the response to a SOAP request
 *  did not contain the required XML constructs.  
 */
/*! @{ */
#define UPNP_E_NOT_FOUND	      -507
/*! @} */

/** @name UPNP_E_INTERNAL_ERROR [-911]
 *  {\tt UPNP_E_INTERNAL_ERROR} is the generic error code for internal
 *  conditions not covered by other error codes.
 */
/*! @{ */
#define UPNP_E_INTERNAL_ERROR         -911
/*! @} */

/* SOAP-related error codes */
#define UPNP_SOAP_E_INVALID_ACTION    401
#define UPNP_SOAP_E_INVALID_ARGS      402
#define UPNP_SOAP_E_OUT_OF_SYNC       403
#define UPNP_SOAP_E_INVALID_VAR       404
#define UPNP_SOAP_E_ACTION_FAILED     501

/*! @} */

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

/* #include "upnpdebug.h" */

enum UpnpOpenFileMode{UPNP_READ, UPNP_WRITE};

/*! @name Constants, Structures, and Types */
/*! @{ */

/** Returned when a control point application registers with {\bf
 *  UpnpRegisterClient}.  Client handles can only be used with 
 *  functions that operate with a client handle.  */

typedef int  UpnpClient_Handle;

/** Returned when a device application registers with {\bf
 *  UpnpRegisterRootDevice} or {\bf UpnpRegisterRootDevice2}.  Device handles 
 *  can only be used with functions that operate with a device handle.  */

typedef int  UpnpDevice_Handle;

/** @name UPnP_EventType
    @memo The reason code for an event callback.
    @doc The {\bf Event} parameter will be different depending on the
         reason for the callback.  The descriptions for each event
	 type describe the contents of the {\bf Event} parameter.
  */

enum Upnp_EventType_e {

  /*
   * Control callbacks
   */

  /** Received by a device when a control point issues a control
   *  request.  The {\bf Event} parameter contains a pointer to a {\bf
   *  Upnp_Action_Request} structure containing the action.  The application
   *  stores the results of the action in this structure. */

  UPNP_CONTROL_ACTION_REQUEST,

  /** A {\bf UpnpSendActionAsync} call completed. The {\bf Event}
   *  parameter contains a pointer to a {\bf Upnp_Action_Complete} structure
   *  with the results of the action.  */

  UPNP_CONTROL_ACTION_COMPLETE,

  /** Received by a device when a query for a single service variable
   *  arrives.  The {\bf Event} parameter contains a pointer to a {\bf
   *  Upnp_State_Var_Request} structure containing the name of the variable
   *  and value.  */

  UPNP_CONTROL_GET_VAR_REQUEST,

  /** A {\bf UpnpGetServiceVarStatus} call completed. The {\bf Event}
   *  parameter contains a pointer to a {\bf Upnp_State_Var_Complete} structure
   *  containing the value for the variable.  */

  UPNP_CONTROL_GET_VAR_COMPLETE,

  /*
   * Discovery callbacks
   */

  /** Received by a control point when a new device or service is available.  
   *  The {\bf Event} parameter contains a pointer to a {\bf
   *  Upnp_Discovery} structure with the information about the device
   *  or service.  */

  UPNP_DISCOVERY_ADVERTISEMENT_ALIVE,

  /** Received by a control point when a device or service shuts down. The {\bf
   *  Event} parameter contains a pointer to a {\bf Upnp_Discovery}
   *  structure containing the information about the device or
   *  service.  */

  UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE,

  /** Received by a control point when a matching device or service responds.
   *  The {\bf Event} parameter contains a pointer to a {\bf
   *  Upnp_Discovery} structure containing the information about
   *  the reply to the search request.  */

  UPNP_DISCOVERY_SEARCH_RESULT,

  /** Received by a control point when the search timeout expires.  The
   *  SDK generates no more callbacks for this search after this 
   *  event.  The {\bf Event} parameter is {\tt NULL}.  */

  UPNP_DISCOVERY_SEARCH_TIMEOUT,

  /*
   * Eventing callbacks
   */

  /** Received by a device when a subscription arrives.
   *  The {\bf Event} parameter contains a pointer to a {\bf
   *  Upnp_Subscription_Request} structure.  At this point, the
   *  subscription has already been accepted.  {\bf UpnpAcceptSubscription}
   *  needs to be called to confirm the subscription and transmit the
   *  initial state table.  This can be done during this callback.  The SDK
   *  generates no events for a subscription unless the device 
   *  application calls {\bf UpnpAcceptSubscription}.
   */

  UPNP_EVENT_SUBSCRIPTION_REQUEST,

  /** Received by a control point when an event arrives.  The {\bf
   *  Event} parameter contains a {\bf Upnp_Event} structure
   *  with the information about the event.  */

  UPNP_EVENT_RECEIVED,

  /** A {\bf UpnpRenewSubscriptionAsync} call completed. The status of
   *  the renewal is in the {\bf Event} parameter as a {\bf
   *  Upnp_Event_Subscription} structure.  */

  UPNP_EVENT_RENEWAL_COMPLETE,

  /** A {\bf UpnpSubscribeAsync} call completed. The status of the
   * subscription is in the {\bf Event} parameter as a {\bf
   * Upnp_Event_Subscription} structure.  */

  UPNP_EVENT_SUBSCRIBE_COMPLETE,

  /** A {\bf UpnpUnSubscribeAsync} call completed. The status of the
   *  subscription is in the {\bf Event} parameter as a {\bf
   *  Upnp_Event_Subscribe} structure.  */

  UPNP_EVENT_UNSUBSCRIBE_COMPLETE,

  /** The auto-renewal of a client subscription failed.   
   *  The {\bf Event} parameter is a {\bf Upnp_Event_Subscribe} structure 
   *  with the error code set appropriately. The subscription is no longer 
   *  valid. */

  UPNP_EVENT_AUTORENEWAL_FAILED,

  /** A client subscription has expired. This will only occur 
   *  if auto-renewal of subscriptions is disabled.
   *  The {\bf Event} parameter is a {\bf Upnp_Event_Subscribe}
   *  structure. The subscription is no longer valid. */
  
  UPNP_EVENT_SUBSCRIPTION_EXPIRED

};

typedef enum Upnp_EventType_e Upnp_EventType;

/** The {\bf Upnp_SID} holds the subscription identifier for a subscription
    between a client and a device.  The SID is a string representation of
    a globally unique id (GUID) and should not be modified.
  */
    
typedef char Upnp_SID[44];

/** @name Upnp_SType
    @memo Represents the different types of searches that
          can be performed using the SDK for UPnP Devices API.
    @doc  By specifying these different values to 
          {\bf UpnpSearchAsync}, the control point application
	  can control the scope of the search from all devices
	  to specific devices or services.
  */

enum Upnp_SType_e {

  /** Search for all devices and services on the network. */
  UPNP_S_ALL,    

  /** Search for all root devices on the network. */
  UPNP_S_ROOT,   

  /** Search for a particular device type or a particular device
      instance. */
  UPNP_S_DEVICE, 
                       
  /** Search for a particular service type, possibly on a particular
   *  device type or device instance.  */
  UPNP_S_SERVICE 
                       
};

typedef enum Upnp_SType_e Upnp_SType;

/** @name Upnp_DescType
    @memo Specifies the type of description in 
          {\bf UpnpRegisterRootDevice2}.
    @doc  These values control how {\bf UpnpRegisterRootDevice2}
          interprets the {\bf description} parameter.
   */
enum Upnp_DescType_e { 

	/** The description is the URL to the description document. */
	UPNPREG_URL_DESC, 
	
	/** The description is a file name on the local file system 
	    containing the description of the device. */
	UPNPREG_FILENAME_DESC,
    
	/** The description is a pointer to a character array containing 
	    the XML description document. */
	UPNPREG_BUF_DESC 

};

typedef enum Upnp_DescType_e Upnp_DescType;

/** Returned as part of a {\bf UPNP_CONTROL_ACTION_COMPLETE} callback.  */

struct Upnp_Action_Request
{
  /** The result of the operation. */
  int ErrCode;

  /** The socket number of the connection to the requestor. */
  int Socket;

  /** The error string in case of error. */
  char ErrStr[LINE_SIZE];

 /** The Action Name. */
  char ActionName[NAME_SIZE];

  /** The unique device ID. */
  char DevUDN[NAME_SIZE];

  /** The service ID. */
  char ServiceID[NAME_SIZE];

  /** The DOM document describing the action. */
  IXML_Document *ActionRequest;

  /** The DOM document describing the result of the action. */
  IXML_Document *ActionResult;

  /** IP address of the control point requesting this action. */
  struct in_addr CtrlPtIPAddr;

  /** The DOM document containing the information from the
      the SOAP header. */
  IXML_Document *SoapHeader;
};

struct Upnp_Action_Complete
{
  /** The result of the operation. */
  int ErrCode;

  /** The control URL for service. */
  char CtrlUrl[NAME_SIZE];

  /** The DOM document describing the action. */
  IXML_Document *ActionRequest;

  /** The DOM document describing the result of the action. */
  IXML_Document *ActionResult;

};

/** Represents the request for current value of a state variable in a service
 *  state table.  */

struct Upnp_State_Var_Request
{
  /** The result of the operation. */
  int ErrCode;

  /** The socket number of the connection to the requestor. */
  int Socket;

  /** The error string in case of error. */
  char ErrStr[LINE_SIZE];

  /** The unique device ID. */
  char DevUDN[NAME_SIZE];

  /** The  service ID. */
  char ServiceID[NAME_SIZE];

  /** The name of the variable. */
  char StateVarName[NAME_SIZE];

  /** IP address of sender requesting the state variable. */
  struct in_addr CtrlPtIPAddr;

  /** The current value of the variable. This needs to be allocated by 
   *  the caller.  When finished with it, the SDK frees this {\bf DOMString}. */
  DOMString CurrentVal;
};

/** Represents the reply for the current value of a state variable in an
    asynchronous call. */

struct Upnp_State_Var_Complete
{
  /** The result of the operation. */
  int ErrCode;

  /** The control URL for the service. */
  char CtrlUrl[NAME_SIZE];

  /** The name of the variable. */
  char StateVarName[NAME_SIZE];

  /** The current value of the variable or error string in case of error. */
  DOMString CurrentVal;
};

/** Returned along with a {\bf UPNP_EVENT_RECEIVED} callback.  */

struct Upnp_Event
{
  /** The subscription ID for this subscription. */
  Upnp_SID Sid;

  /** The event sequence number. */
  int EventKey;

  /** The DOM tree representing the changes generating the event. */
  IXML_Document *ChangedVariables;

};

/*
 * This typedef is required by Doc++ to parse the last entry of the 
 * Upnp_Discovery structure correctly.
 */


/** Returned in a {\bf UPNP_DISCOVERY_RESULT} callback. */
struct Upnp_Discovery
{
	/** The result code of the {\bf UpnpSearchAsync} call. */
	int  ErrCode;                  
				     
	/** The expiration time of the advertisement. */
	int  Expires;                  
				     
	/** The unique device identifier. */
	char DeviceId[LINE_SIZE];      

	/** The device type. */
	char DeviceType[LINE_SIZE];    

	/** The service type. */
	char ServiceType[LINE_SIZE];

	/** The service version. */
	char ServiceVer[LINE_SIZE];    

	/** The URL to the UPnP description document for the device. */
	char Location[LINE_SIZE];      

	/** The operating system the device is running. */
	char Os[LINE_SIZE];            
				     
	/** Date when the response was generated. */
	char Date[LINE_SIZE];            
				     
	/** Confirmation that the MAN header was understood by the device. */
	char Ext[LINE_SIZE];           
				     
	/** The host address of the device responding to the search. */
	struct sockaddr_in DestAddr; 
};

/** Returned along with a {\bf UPNP_EVENT_SUBSCRIBE_COMPLETE} or {\bf
 * UPNP_EVENT_UNSUBSCRIBE_COMPLETE} callback.  */

struct Upnp_Event_Subscribe {

  /** The SID for this subscription.  For subscriptions, this only
   *  contains a valid SID if the {\bf Upnp_EventSubscribe.result} field
   *  contains a {\tt UPNP_E_SUCCESS} result code.  For unsubscriptions,
   *  this contains the SID from which the subscription is being
   *  unsubscribed.  */

  Upnp_SID Sid;            

  /** The result of the operation. */
  int ErrCode;              

  /** The event URL being subscribed to or removed from. */
  char PublisherUrl[NAME_SIZE]; 

  /** The actual subscription time (for subscriptions only). */
  int TimeOut;              
                              
};
  
/** Returned along with a {\bf UPNP_EVENT_SUBSCRIPTION_REQUEST}
 *  callback.  */

struct Upnp_Subscription_Request
{
  /** The identifier for the service being subscribed to. */
  char *ServiceId; 

  /** Universal device name. */
  char *UDN;       

  /** The assigned subscription ID for this subscription. */
  Upnp_SID Sid;

};


struct File_Info
{
  /** The length of the file. A length less than 0 indicates the size 
   *  is unknown, and data will be sent until 0 bytes are returned from
   *  a read call. */
  off_t file_length;

  /** The time at which the contents of the file was modified;
   *  The time system is always local (not GMT). */
  time_t last_modified;

  /** If the file is a directory, {\bf is_directory} contains
   * a non-zero value. For a regular file, it should be 0. */
  int is_directory;

  /** If the file or directory is readable, this contains 
   * a non-zero value. If unreadable, it should be set to 0. */
  int is_readable;

  /** The content type of the file. This string needs to be allocated 
   *  by the caller using {\bf ixmlCloneDOMString}.  When finished 
   *  with it, the SDK frees the {\bf DOMString}. */
   
  DOMString content_type;

};

/* The type of handle returned by the web server for open requests. */

typedef void *UpnpWebFileHandle;

/** The {\bf UpnpVirtualDirCallbacks} structure contains the pointers to
 *  file-related callback functions a device application can register to
 *  virtualize URLs.  
 */
struct UpnpVirtualDirCallbacks
{
  /** Called by the web server to query information on a file.  The callback
   *  should return 0 on success or -1 on an error. */
  int (*get_info) (
    IN  const char *filename,     /** The name of the file to query. */
    OUT struct File_Info *info    /** Pointer to a structure to store the 
                                      information on the file. */
    );
                                  
  /** Called by the web server to open a file.  The callback should return
   *  a valid handle if the file can be opened.  Otherwise, it should return
   *  {\tt NULL} to signify an error. */
  UpnpWebFileHandle (*open)(
    IN const char *filename,       /** The name of the file to open. */ 
    IN enum UpnpOpenFileMode Mode  /** The mode in which to open the file. 
                                       Valid values are {\tt UPNP_READ} or 
                                       {\tt UPNP_WRITE}. */
    );

  /** Called by the web server to perform a sequential read from an open
   *  file.  The callback should copy {\bf buflen} bytes from the file into
   *  the buffer.
   *  @return [int] An integer representing one of the following:
   *    \begin{itemize}
   *      \item {\tt 0}:  The file contains no more data (EOF).
   *      \item {\tt >0}: A successful read of the number of bytes in the 
   *                      return code.
   *      \item {\tt <0}: An error occurred reading the file.
   *    \end{itemzie}
   */
   int (*read) (
     IN UpnpWebFileHandle fileHnd,  /** The handle of the file to read. */
     OUT char *buf,                 /** The buffer in which to place the 
				        data. */
     IN size_t buflen               /** The size of the buffer (i.e. the 
                                        number of bytes to read). */
     );

  /** Called by the web server to perform a sequential write to an open
   *  file.  The callback should write {\bf buflen} bytes into the file from
   *  the buffer.  It should return the actual number of bytes written, 
   *  which might be less than {\bf buflen} in the case of a write error.
   */
   int (*write) (
     IN UpnpWebFileHandle fileHnd, /** The handle of the file to write. */
     IN char *buf,                 /** The buffer with the bytes to write. */
     IN size_t buflen              /** The number of bytes to write. */
     );

  /** Called by the web server to move the file pointer, or offset, into
   *  an open file.  The {\bf origin} parameter determines where to start
   *  moving the file pointer.  A value of {\tt SEEK_CUR} moves the
   *  file pointer relative to where it is.  The {\bf offset} parameter can
   *  be either positive (move forward) or negative (move backward).  
   *  {\tt SEEK_END} moves relative to the end of the file.  A positive 
   *  {\bf offset} extends the file.  A negative {\bf offset} moves backward 
   *  in the file.  Finally, {\tt SEEK_SET} moves to an absolute position in 
   *  the file. In this case, {\bf offset} must be positive.  The callback 
   *  should return 0 on a successful seek or a non-zero value on an error.
   */
   int (*seek) (
     IN UpnpWebFileHandle fileHnd,  /** The handle of the file to move the 
                                        file pointer. */
     IN off_t offset,                /** The number of bytes to move in the 
                                        file.  Positive values move foward and 
                                        negative values move backward.  Note 
                                        that this must be positive if the 
                                        {\bf origin} is {\tt SEEK_SET}. */
     IN int origin                  /** The position to move relative to.  It 
                                        can be {\tt SEEK_CUR} to move relative 
                                        to the current position, 
					{\tt SEEK_END} to move relative to 
					the end of the file, or {\tt 
					SEEK_SET} to specify an absolute 
					offset. */
     );

   /** Called by the web server to close a file opened via the {\bf open}
    *  callback.  It should return 0 on success, or a non-zero value on an 
    *  error.
    */
   int (*close) (
     IN UpnpWebFileHandle fileHnd   /** The handle of the file to close. */
     );

};

typedef struct virtual_Dir_List
{
    struct virtual_Dir_List *next;
    char dirName[NAME_SIZE];
} virtualDirList;

/** All callback functions share the same prototype, documented below.
 *  Note that any memory passed to the callback function
 *  is valid only during the callback and should be copied if it
 *  needs to persist.  This callback function needs to be thread
 *  safe.  The context of the callback is always on a valid thread 
 *  context and standard synchronization methods can be used.  Note, 
 *  however, because of this the callback cannot call SDK functions
 *  unless explicitly noted.
 *
 *  \begin{verbatim}
      int CallbackFxn( Upnp_EventType EventType, void* Event, void* Cookie );
    \end{verbatim} 
 *
 *  where {\bf EventType} is the event that triggered the callback, 
 *  {\bf Event} is a structure that denotes event-specific information for that
 *  event, and {\bf Cookie} is the user data passed when the callback was
 *  registered.
 *
 *  See {\bf Upnp_EventType} for more information on the callback values and
 *  the associated {\bf Event} parameter.  
 *
 *  The return value of the callback is currently ignored.  It may be used
 *  in the future to communicate results back to the SDK.
 */

typedef int  (*Upnp_FunPtr) (
    IN Upnp_EventType EventType, 
    IN void *Event, 
    IN void *Cookie
    );

/*! @} */ /* Constants, Structures, and Types */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! @name Initialization and Registration */
/*! @{ */
/** Initializes the Linux SDK for UPnP Devices. This function must be called
 *  before any other API function can be called.  It should be called
 *  only once.  Subsequent calls to this API return a {\tt UPNP_E_INIT}
 *  error code.
 *
 *  Optionally, the application can specify a host IP address (in the
 *  case of a multi-homed configuration) and a port number to use for
 *  all UPnP operations.  Since a port number can be used only by one
 *  process, multiple processes using the SDK must specify
 *  different port numbers.
 *
 *  If unspecified, the SDK will use the first adapter's IP address 
 *  and an arbitrary port.
 *
 *  This call is synchronous.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist 
 *              to initialize the SDK.
 *      \item {\tt UPNP_E_INIT}: The SDK is already initialized. 
 *      \item {\tt UPNP_E_INIT_FAILED}: The SDK initialization 
 *              failed for an unknown reason.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_LISTEN}: An error occurred listening to a socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error ocurred creating a socket.
 *      \item {\tt UPNP_E_INTERNAL_ERROR}: An internal error ocurred.
 *    \end{itemize} */

EXPORT_SPEC int UpnpInit(
    IN const char *HostIP,      /** The host IP address to use, in 
                                    string format, for example "192.168.0.1", 
                                    or {\tt NULL} to use the first adapter's 
                                    IP address. */
    IN unsigned short DestPort  /** The destination port number to use.  0 
                                    will pick an arbitrary free port. */
    );

/** Terminates the Linux SDK for UPnP Devices. This function must be the last 
 *  API function called. It should be called only once. Subsequent calls to 
 *  this API return a {\tt UPNP_E_FINISH} error code.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_FINISH}: The SDK is already terminated or 
 *                                 it is not initialized. 
 *    \end{itemize} */

EXPORT_SPEC int UpnpFinish(void);

/** If '0' is used as the port number in {\bf UpnpInit}, then this
 *  function can be used to retrieve the actual port allocated to
 *  the SDK. If {\bf UpnpInit} has not succeeded then 0 is 
 *  returned.
 *
 *  @return [unsigned short] The port on which an internal server is 
 *                           listening for UPnP related requests. 
 */
EXPORT_SPEC unsigned short UpnpGetServerPort(void);

/** If {\tt NULL} is used as the IP address in {\bf UpnpInit}, then this
 *  function can be used to retrieve the actual interface address
 *  on which device is running. If {\bf UpnpInit} has not succeeded 
 *  then {\tt NULL} is returned.
 *
 *  @return [char*] The IP address on which an internal server is listening 
 *                  for UPnP related requests. 
 */
EXPORT_SPEC char * UpnpGetServerIpAddress(void);

/** {\bf UpnpRegisterClient} registers a control point application with the
 *  SDK.  A control point application cannot make any other API calls
 *  until it registers using this function.
 *
 *  {\bf UpnpRegisterClient} is a synchronous call and generates no callbacks.
 *  Callbacks can occur as soon as this function returns.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_FINISH}: The SDK is already terminated or 
 *                                 is not initialized. 
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf Callback} or {\bf Hnd} 
 *              is not a valid pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              register this control point.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpRegisterClient(
    IN Upnp_FunPtr Callback,   /** Pointer to a function for receiving 
                                   asynchronous events. */
    IN const void *Cookie,     /** Pointer to user data returned with the 
                                   callback function when invoked. */
    OUT UpnpClient_Handle *Hnd /** Pointer to a variable to store the 
                                   new control point handle. */
    );  

/** {\bf UpnpRegisterRootDevice} registers a device application with
 *  the SDK.  A device application cannot make any other API
 *  calls until it registers using this function.  Device applications
 *  can also register as control points (see {\bf UpnpRegisterClient}
 *  to get a control point handle to perform control point
 *  functionality).
 *
 *  {\bf UpnpRegisterRootDevice} is synchronous and does not generate
 *  any callbacks.  Callbacks can occur as soon as this function returns.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_FINISH}: The SDK is already terminated or 
 *                                 is not initialized. 
 *      \item {\tt UPNP_E_INVALID_DESC}: The description document was not 
 *              a valid device description.
 *      \item {\tt UPNP_E_INVALID_URL}: The URL for the description document 
 *              is not valid.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf Callback} or {\bf Hnd} 
 *              is not a valid pointer or {\bf DescURL} is {\tt NULL}.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting the 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: There are insufficient resources to 
 *              register this root device.
 *    \end{itemize} */

EXPORT_SPEC int UpnpRegisterRootDevice(
    IN const char *DescUrl,    /** Pointer to a string containing the 
                                   description URL for this root device 
                                   instance. */
    IN Upnp_FunPtr Callback,   /** Pointer to the callback function for 
                                   receiving asynchronous events. */
    IN const void *Cookie,     /** Pointer to user data returned with the 
                                   callback function when invoked. */
    OUT UpnpDevice_Handle *Hnd /** Pointer to a variable to store the 
                                   new device handle. */
    );

/** {\bf UpnpRegisterRootDevice2} is similar to {\bf UpnpRegisterRootDevice},
 *  except that it also allows the description document to be specified as a 
 *  file or a memory buffer. The description can also be configured to have the
 *  correct IP and port address.
 *
 *  NOTE: For the configuration to be functional, the internal web server
 *  MUST be present. In addition, the web server MUST be activated
 *  (using {\bf UpnpSetWebServerRootDir}) before calling this function.
 *  The only condition where the web server can be absent is if the 
 *  description document is specified as a URL and no configuration is 
 *  required (i.e. {\tt config_baseURL = 0}.)
 *
 *  {\bf UpnpRegisterRootDevice2} is synchronous and does not generate
 *  any callbacks.  Callbacks can occur as soon as this function returns.
 *
 *  Examples of using different types of description documents:
 *  \begin{verbatim}
    1) Description specified as a URL:
          descriptionType == UPNPREG_URL_DESC
          description is the URL
          bufferLen = 0 (ignored)
    2) Description specified as a file:
          descriptionType == UPNPREG_FILENAME_DESC
          description is a filename
          bufferLen = 0 (ignored)
    3) Description specified as a memory buffer:
          descriptionType == UPNPREG_BUF_DESC
          description is pointer to a memory buffer
          bufferLen == length of memory buffer
    \end{verbatim}
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_FINISH}: The SDK is already terminated or 
 *                                 is not initialized.
 *      \item {\tt UPNP_E_INVALID_DESC}: The description document is not 
 *              a valid device description.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf Callback} or {\bf Hnd} 
 *              is not a valid pointer or {\bf DescURL} is {\tt NULL}.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting the 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: There are insufficient resources to 
 *              register this root device.
 *      \item {\tt UPNP_E_URL_TOO_BIG}: Length of the URL is bigger than the 
 *              internal buffer.
 *      \item {\tt UPNP_E_FILE_NOT_FOUND}: The description file could not 
 *              be found.
 *      \item {\tt UPNP_E_FILE_READ_ERROR}: An error occurred reading the 
 *              description file.
 *      \item {\tt UPNP_E_INVALID_URL}: The URL to the description document 
 *              is invalid.
 *      \item {\tt UPNP_E_EXT_NOT_XML}: The URL to the description document 
 *              or file should have a {\tt .xml} extension.
 *      \item {\tt UPNP_E_NO_WEB_SERVER}: The internal web server has been 
 *              compiled out; the SDK cannot configure itself from the 
 *              description document.
 *    \end{itemize} */
 
EXPORT_SPEC int UpnpRegisterRootDevice2(
    IN Upnp_DescType descriptionType,/** The type of the description 
                                         document. */
    IN const char* description,      /** Treated as a URL, file name or 
                                         memory buffer depending on 
                                         description type. */
    IN size_t bufferLen,             /** The length of memory buffer if 
                                         passing a description in a buffer, 
                                         otherwise it is ignored. */
    IN int config_baseURL,           /** If nonzero, {\tt URLBase} of 
                                         description document is 
                                         configured and the description 
                                         is served using the internal 
                                         web server. */
    IN Upnp_FunPtr Fun,              /** Pointer to the callback function 
                                         for receiving asynchronous 
                                         events. */
    IN const void* Cookie,           /** Pointer to user data returned 
                                         with the callback function when 
                                         invoked. */
    OUT UpnpDevice_Handle* Hnd       /** Pointer to a variable to store 
                                         the new device handle. */
    );

/** {\bf UpnpUnRegisterClient} unregisters a control point application, 
 *  unsubscribing all active subscriptions. After this call, the 
 *  {\bf UpnpClient_Handle} is no longer valid.
 *
 *  {\bf UpnpUnRegisterClient} is a synchronous call and generates no
 *  callbacks.  The SDK generates no more callbacks after this 
 *  function returns.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *                   point handle.
 *    \end{itemize} */

EXPORT_SPEC int UpnpUnRegisterClient(
    IN UpnpClient_Handle Hnd  /** The handle of the control point instance 
                                  to unregister. */
    );

/** Unregisters a root device registered with {\bf UpnpRegisterRootDevice} or
 *  {\bf UpnpRegisterRootDevice2}. After this call, the 
 *  {\bf UpnpDevice_Handle} is no longer valid. For all advertisements that 
 *  have not yet expired, the SDK sends a device unavailable message 
 *  automatically.
 *
 *  {\bf UpnpUnRegisterRootDevice} is a synchronous call and generates no
 *  callbacks.  Once this call returns, the SDK will no longer 
 *  generate callbacks to the application.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid  
 *              device handle.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpUnRegisterRootDevice(
   IN UpnpDevice_Handle /** The handle of the root device instance to 
                            unregister. */
   );


/** OBSOLETE METHOD : use {\bf UpnpSetMaxContentLength} instead.
 * Warning: the Handle argument provided here is not used, so the effect
 * of this function is global to the SDK (= same as 
 * {\bf UpnpSetMaxContentLength} ).
 */
EXPORT_SPEC int UpnpSetContentLength(
    IN UpnpClient_Handle Hnd,  
    IN int contentLength       
    );


/** Sets the maximum content-length that the SDK will process on an incoming 
 *  SOAP requests or responses. This API allows devices that have memory 
 *  constraints to exhibit consistent behaviour if the size of the 
 *  incoming SOAP message exceeds the memory that device can allocate. 
 *  The default maximum content-length is {\tt DEFAULT_SOAP_CONTENT_LENGTH} 
 *  = 16K bytes.
 *   
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *    \end{itemize}
 */
EXPORT_SPEC int UpnpSetMaxContentLength(
    IN size_t contentLength    /** The maximum permissible content length 
			           for incoming SOAP actions, in bytes. */
    );

/*! @} */ /* Initialization and Registration */

/******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                        D I S C O V E R Y                                   *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************/

/*! @name Discovery */
/*! @{ */

/** {\bf UpnpSearchAsync} searches for devices matching the given
 *  search target.  The function returns immediately and the SDK 
 *  calls the default callback function, registered during the 
 *  {\bf UpnpRegisterClient} call, for each matching root device,
 *  device, or service.  The application specifies the search type by the 
 *  {\bf Target} parameter.  
 *
 *  Note that there is no way for the SDK to distinguish which client
 *  instance issued a particular search.  Therefore, the client can get
 *  search callbacks that do not match the original criteria of the search.
 *  Also, the application will receive multiple callbacks for each search.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf Target} is {\tt NULL}.
 *    \end{itemize} */

EXPORT_SPEC int UpnpSearchAsync(
    IN UpnpClient_Handle Hnd, /** The handle of the client performing 
                                  the search. */
    IN int Mx,                /** The time, in seconds, to wait for 
                                  responses. If the time is greater 
                                  than {\tt MAX_SEARCH_TIME} then the time is 
                                  set to {\tt MAX_SEARCH_TIME}. If the time is 
                                  less than {\tt MIN_SEARCH_TIME} then the 
                                  time is set to {\tt MIN_SEARCH_TIME}. */ 
    IN const char *Target,    /** The search target as defined in the UPnP 
                                  Device Architecture v1.0 specification. */
    IN const void *Cookie     /** The user data to pass when the callback 
                                  function is invoked. */
    ); 

/** {\bf UpnpSendAdvertisement} sends out the discovery announcements for
 *  all devices and services for a device.  Each announcement is made with
 *  the same expiration time.
 *
 *  {\bf UpnpSendAdvertisement} is a synchronous call.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid 
 *              device handle.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: There are insufficient resources to 
 *              send future advertisements.
 *    \end{itemize}
 */
EXPORT_SPEC int UpnpSendAdvertisement(
    IN UpnpDevice_Handle Hnd, /** The device handle for which to send out the 
                                  announcements. */
    IN int Exp                /** The expiration age, in seconds, of 
                                  the announcements. */
    );

/*! @} */ /* Discovery */

/******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                            C O N T R O L                                   *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************/

/*! @name Control */
/*! @{ */

/** {\bf UpnpGetServiceVarStatus} queries the state of a state 
 *  variable of a service on another device.  This is a synchronous call.
 *  A positive return value indicates a SOAP error code, whereas a negative
 *  return code indicates an SDK error code. {\bf Note that the use of this 
 *  function is deprecated by the UPnP Forum}.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: {\bf ActionUrl} is not a valid URL.
 *      \item {\tt UPNP_E_INVALID_DESC}: The XML document was not 
 *              found or it does not contain a valid XML description.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf StVarVal} is not a valid 
 *              pointer or {\bf VarName} or {\bf ActionUrl} is {\tt NULL}. 
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *      \item {\tt UPNP_SOAP_E_INVALID_VAR}: The given variable is invalid 
 *              according to the device.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpGetServiceVarStatus(
    IN UpnpClient_Handle Hnd,     /** The handle of the control point. */
    IN const char *ActionURL,     /** The URL of the service. */
    IN const char *VarName,       /** The name of the variable to query. */
    OUT DOMString *StVarVal       /** The pointer to store the value 
                                      for {\bf VarName}. The SDK 
                                      allocates this string and the caller 
                                      needs to free it using 
				      {\bf ixmlFreeDOMString}. */
    );

/** {\bf UpnpGetServiceVarStatusAsync} queries the state of a variable of a 
 *  service, generating a callback when the operation is complete. {\bf Note 
 *  that the use of this function is deprecated by the UPnP Forum}.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf ActionUrl} is not a valid URL.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf VarName}, {\bf Fun} or 
 *              {\bf ActionUrl} is not a valid pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpGetServiceVarStatusAsync(
    IN UpnpClient_Handle Hnd, /** The handle of the control point. */
    IN const char *ActionURL, /** The URL of the service. */
    IN const char *VarName,   /** The name of the variable to query. */
    IN Upnp_FunPtr Fun,       /** Pointer to a callback function to 
                                  be invoked when the operation is complete. */
    IN const void *Cookie     /** Pointer to user data to pass to the 
                                  callback function when invoked. */
    );

/** {\bf UpnpSendAction} sends a message to change a state variable
 *  in a service.  This is a synchronous call that does not return until the 
 *  action is complete.
 * 
 *  Note that a positive return value indicates a SOAP-protocol error code.
 *  In this case,  the error description can be retrieved from {\bf RespNode}.
 *  A negative return value indicates an SDK error.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: {\bf ActionUrl} is not a valid URL.
 *      \item {\tt UPNP_E_INVALID_ACTION}: This action is not valid.
 *      \item {\tt UPNP_E_INVALID_DEVICE}: {\bf DevUDN} is not a 
 *              valid device.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf ServiceType}, {\bf Action}, 
 *              {\bf ActionUrl}, or 
 *              {\bf RespNode} is not a valid pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSendAction(
    IN UpnpClient_Handle Hnd,     /** The handle of the control point 
                                      sending the action. */
    IN const char *ActionURL,     /** The action URL of the service. */
    IN const char *ServiceType,   /** The type of the service. */
    IN const char *DevUDN,        /** This parameter is ignored and must be
				      {\tt NULL}. */
    IN IXML_Document *Action,     /** The DOM document for the action. */
    OUT IXML_Document **RespNode  /** The DOM document for the response 
                                    to the action.  The SDK allocates 
                                    this document and the caller needs to free 
                                    it.  */
   );

/** {\bf UpnpSendActionEx} sends a message to change a state variable
 *  in a service.  This is a synchronous call that does not return until the 
 *  action is complete.
 *
 *  Note that a positive return value indicates a SOAP-protocol error code.
 *  In this case,  the error description can be retrieved from {\bf RespNode}.
 *  A negative return value indicates an SDK error.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: {\bf ActionUrl} is not a valid URL.
 *      \item {\tt UPNP_E_INVALID_ACTION}: This action is not valid.
 *      \item {\tt UPNP_E_INVALID_DEVICE}: {\bf DevUDN} is not a 
 *              valid device.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf ServiceType}, {\bf Action}, 
 *              {\bf ActionUrl}, or 
 *              {\bf RespNode} is not a valid pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSendActionEx(
    IN UpnpClient_Handle Hnd,    /** The handle of the control point 
                                     sending the action. */
    IN const char *ActionURL,    /** The action URL of the service. */
    IN const char *ServiceType,  /** The type of the service. */
    IN const char *DevUDN,       /** This parameter is ignored and must be
				     {\tt NULL}. */
    IN IXML_Document *Header,    /** The DOM document for the SOAP header. 
                                     This may be {\tt NULL} if the header is 
				     not required. */
    IN IXML_Document *Action,    /** The DOM document for the action. */
    OUT IXML_Document **RespNode /** The DOM document for the response 
                                     to the action.  The SDK allocates 
                                     this document and the caller needs to free 
                                     it.  */
   );

/** {\bf UpnpSendActionAsync} sends a message to change a state variable
 *  in a service, generating a callback when the operation is complete.
 *  See {\bf UpnpSendAction} for comments on positive return values. These 
 *  positive return values are sent in the event struct associated with the
 *  {\tt UPNP_CONTROL_ACTION_COMPLETE} event.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: {\bf ActionUrl} is an invalid URL.
 *      \item {\tt UPNP_E_INVALID_DEVICE}: {\bf DevUDN} is an invalid device.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf Fun} is not a valid 
 *              callback function or {\bf ServiceType}, {\bf Act}, or 
 *              {\bf ActionUrl} is {\tt NULL}.
 *      \item {\tt UPNP_E_INVALID_ACTION}: This action is not valid.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSendActionAsync(
    IN UpnpClient_Handle Hnd,   /** The handle of the control point 
                                    sending the action. */
    IN const char *ActionURL,   /** The action URL of the service. */
    IN const char *ServiceType, /** The type of the service. */
    IN const char *DevUDN,      /** This parameter is ignored and must be
				    {\tt NULL}. */
    IN IXML_Document *Action,   /** The DOM document for the action to 
                                    perform on this device. */
    IN Upnp_FunPtr Fun,         /** Pointer to a callback function to 
                                    be invoked when the operation 
		                    completes. */
    IN const void *Cookie       /** Pointer to user data that to be 
                                    passed to the callback when invoked. */
    );

/** {\bf UpnpSendActionExAsync} sends a message to change a state variable
 *  in a service, generating a callback when the operation is complete.
 *  See {\bf UpnpSendAction} for comments on positive return values. These 
 *  positive return values are sent in the event struct associated with the
 *  {\tt UPNP_CONTROL_ACTION_COMPLETE} event.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: {\bf ActionUrl} is an invalid URL.
 *      \item {\tt UPNP_E_INVALID_DEVICE}: {\bf DevUDN} is an invalid device.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf Fun} is not a valid 
 *              callback function or {\bf ServiceType}, {\bf Act}, or 
 *              {\bf ActionUrl} is {\tt NULL}.
 *      \item {\tt UPNP_E_INVALID_ACTION}: This action is not valid.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSendActionExAsync(
    IN UpnpClient_Handle Hnd,   /** The handle of the control point 
                                    sending the action. */
    IN const char *ActionURL,   /** The action URL of the service. */
    IN const char *ServiceType, /** The type of the service. */
    IN const char *DevUDN,      /** This parameter is ignored and must be
				    {\tt NULL}. */
    IN IXML_Document *Header,   /** The DOM document for the SOAP header. 
                                    This may be {\tt NULL} if the header is 
				    not required. */
    IN IXML_Document *Action,   /** The DOM document for the action to 
                                    perform on this device. */
    IN Upnp_FunPtr Fun,         /** Pointer to a callback function to 
                                    be invoked when the operation 
				    completes. */
    IN const void *Cookie       /** Pointer to user data that to be 
                                    passed to the callback when invoked. */
    );

/*! @} */ /* Control */

/******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                        E V E N T I N G                                     *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************/

/*! @name Eventing */
/*! @{ */

/** {\bf UpnpAcceptSubscription} accepts a subscription request and sends
 *  out the current state of the eventable variables for a service.  
 *  The device application should call this function when it receives a 
 *  {\tt UPNP_EVENT_SUBSCRIPTION_REQUEST} callback. This function is 
 *  synchronous and generates no callbacks.
 *
 *  {\bf UpnpAcceptSubscription} can be called during the execution of 
 *  a callback function.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid device 
 *              handle.
 *      \item {\tt UPNP_E_INVALID_SERVICE}: The {\bf DevId}/{\bf ServId} 
 *              pair refers to an invalid service. 
 *      \item {\tt UPNP_E_INVALID_SID}: The specified subscription ID is not 
 *              valid.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf VarName}, 
 *              {\bf NewVal}, {\bf DevID}, or {\bf ServID} is not a valid 
 *              pointer or {\bf cVariables} is less than zero.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpAcceptSubscription(
    IN UpnpDevice_Handle Hnd, /** The handle of the device. */
    IN const char *DevID,     /** The device ID of the subdevice of the 
                                  service generating the event. */
    IN const char *ServID,    /** The unique service identifier of the service 
                                  generating the event. */
    IN const char **VarName,  /** Pointer to an array of event variables. */
    IN const char **NewVal,   /** Pointer to an array of values for 
                                  the event variables. */
    IN int cVariables,        /** The number of event variables in 
                                  {\bf VarName}. */
    IN Upnp_SID SubsId        /** The subscription ID of the newly 
                                  registered control point. */
    );

/** {\bf UpnpAcceptSubscriptionExt} is similar to {\bf UpnpAcceptSubscription}
 *  except that it takes a DOM document for the variables to event rather
 *  than an array of strings. This function is sychronous
 *  and generates no callbacks.
 *
 *  {\bf UpnpAcceptSubscriptionExt} can be called during the execution of 
 *  a callback function.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid device 
 *              handle.
 *      \item {\tt UPNP_E_INVALID_SERVICE}: The {\bf DevId}/{\bf ServId} 
 *              pair refers to an invalid service. 
 *      \item {\tt UPNP_E_INVALID_SID}: The specified subscription ID is not 
 *              valid.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf VarName},  
 *              {\bf NewVal}, {\bf DevID}, {\bf ServID}, or {\bf PropSet} 
 *              is not a valid pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpAcceptSubscriptionExt(
    IN UpnpDevice_Handle Hnd,  /** The handle of the device. */
    IN const char *DevID,      /** The device ID of the subdevice of the 
                                   service generating the event. */
    IN const char *ServID,     /** The unique service identifier of the service 
                                   generating the event. */
    IN IXML_Document *PropSet, /** The DOM document for the property set. 
                                   Property set documents must conform to
                                   the XML schema defined in section 4.3 of the
                                   Universal Plug and Play Device Architecture
                                   specification. */
    IN Upnp_SID SubsId         /** The subscription ID of the newly 
                                   registered control point. */
    );

/** {\bf UpnpNotify} sends out an event change notification to all
 *  control points subscribed to a particular service.  This function is
 *  synchronous and generates no callbacks.
 *
 *  {\bf UpnpNotify} may be called during a callback function to send out
 *  a notification.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid device 
 *              handle.
 *      \item {\tt UPNP_E_INVALID_SERVICE}: The {\bf DevId}/{\bf ServId} 
 *              pair refers to an invalid service.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf VarName}, {\bf NewVal}, 
 *               {\bf DevID}, or {\bf ServID} is not a valid pointer or 
 *               {\bf cVariables} is less than zero.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpNotify(
    IN UpnpDevice_Handle,   /** The handle to the device sending the event. */
    IN const char *DevID,   /** The device ID of the subdevice of the service 
                                generating the event. */
    IN const char *ServID,  /** The unique identifier of the service 
                                generating the event. */
    IN const char **VarName,/** Pointer to an array of variables that 
                                have changed. */
    IN const char **NewVal, /** Pointer to an array of new values for 
                                those variables. */
    IN int cVariables       /** The count of variables included in this 
                                notification. */
    );

/** {\bf UpnpNotifyExt} is similar to {\bf UpnpNotify} except that it takes
 *  a DOM document for the event rather than an array of strings. This 
 *  function is synchronous and generates no callbacks.
 *
 *  {\bf UpnpNotifyExt} may be called during a callback function to send out
 *  a notification.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid device 
 *              handle.
 *      \item {\tt UPNP_E_INVALID_SERVICE}: The {\bf DevId}/{\bf ServId} 
 *              pair refers to an invalid service.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf VarName}, {\bf NewVal}, 
 *               {\bf DevID}, {\bf ServID}, or {\bf PropSet} 
 *               is not a valid pointer or {\bf cVariables} is less than zero.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpNotifyExt(
    IN UpnpDevice_Handle,       /** The handle to the device sending the 
                                    event. */
    IN const char *DevID,       /** The device ID of the subdevice of the 
                                    service generating the event. */
    IN const char *ServID,      /** The unique identifier of the service 
                                    generating the event. */
    IN IXML_Document *PropSet   /** The DOM document for the property set. 
                                    Property set documents must conform to 
                                    the XML schema defined in section 4.3 of 
                                    the Universal Plug and Play Device 
                                    Architecture specification. */
    );

/** {\bf UpnpRenewSubscription} renews a subscription that is about to 
 *  expire.  This function is synchronous.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf Timeout} is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_SID}: The SID being passed to this function 
 *              is not a valid subscription ID.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occured. 
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.  
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting to 
 *              {\bf PublisherUrl}.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error occurred creating a socket.
 *      \item {\tt UPNP_E_BAD_RESPONSE}: An error occurred in response from 
 *              the publisher.
 *      \item {\tt UPNP_E_SUBSCRIBE_UNACCEPTED}: The publisher refused 
 *              the subscription renew.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpRenewSubscription(
    IN UpnpClient_Handle Hnd, /** The handle of the control point that 
                                  is renewing the subscription. */
    INOUT int *TimeOut,       /** Pointer to a variable containing the 
                                  requested subscription time.  Upon return, 
                                  it contains the actual renewal time. */
    IN Upnp_SID SubsId        /** The ID for the subscription to renew. */
    );

/** {\bf UpnpRenewSubscriptionAsync} renews a subscription that is about
 *  to expire, generating a callback when the operation is complete.
 *
 *  Note that many of the error codes for this function are returned in
 *  the {\bf Upnp_Event_Subscribe} structure.  In those cases, the function
 *  returns {\tt UPNP_E_SUCCESS} and the appropriate error code will
 *  be in the {\bf Upnp_Event_Subscribe.ErrCode} field in the {\bf Event}
 *  structure passed to the callback.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_SID}: The {\bf SubsId} is not a valid 
 *              subscription ID.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf Fun} is not a valid 
 *              callback function pointer or {\bf Timeout} is less than zero 
 *              but is not {\tt UPNP_INFINITE}.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occured (returned in 
 *              the {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket (returned in the {\bf Upnp_Event_Subscribe.ErrCode} 
 *              field as part of the callback).
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading  
 *              from a socket (returned in the 
 *              {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding the socket 
 *              (returned in the {\bf Upnp_Event_Subscribe.ErrCode} field as 
 *              part of the callback).
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting to 
 *              {\bf PublisherUrl} (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error occurred creating socket (
 *              returned in the {\bf Upnp_Event_Subscribe.ErrCode} field as 
 *              part of the callback).
 *      \item {\tt UPNP_E_BAD_RESPONSE}: An error occurred in response from 
 *              the publisher (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_SUBSCRIBE_UNACCEPTED}: The publisher refused 
 *              the subscription request (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpRenewSubscriptionAsync(
    IN UpnpClient_Handle Hnd, /** The handle of the control point that 
                                  is renewing the subscription. */
    IN int TimeOut,           /** The requested subscription time.  The 
                                  actual timeout value is returned when 
                                  the callback function is called. */
    IN Upnp_SID SubsId,       /** The ID for the subscription to renew. */
    IN Upnp_FunPtr Fun,       /** Pointer to a callback function to be 
                                  invoked when the renewal is complete. */
    IN const void *Cookie     /** Pointer to user data passed 
                                  to the callback function when invoked. */
    );

/** {\bf UpnpSetMaxSubscriptions} sets the maximum number of subscriptions 
 *  accepted per service. The default value accepts as many as system 
 *  resources allow. If the number of current subscriptions for a service is 
 *  greater than the requested value, the SDK accepts no new 
 *  subscriptions or renewals, however, the SDK does not remove
 *  any current subscriptions.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid device 
 *              handle.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSetMaxSubscriptions(  
    IN UpnpDevice_Handle Hnd, /** The handle of the device for which 
				  the maximum number of subscriptions is 
				  being set. */
    IN int MaxSubscriptions   /** The maximum number of subscriptions to be 
				  allowed per service. */
    );

/** {\bf UpnpSetMaxSubscriptionTimeOut} sets the maximum time-out accepted
 *  for a subscription request or renewal. The default value accepts the 
 *  time-out set by the control point. If a control point requests a 
 *  subscription time-out less than or equal to the maximum, the SDK
 *  grants the value requested by the control point.  If the time-out 
 *  is greater, the SDK returns the maximum value.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid device 
 *              handle.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSetMaxSubscriptionTimeOut(  
    IN UpnpDevice_Handle Hnd,       /** The handle of the device for which 
				        the maximum subscription time-out is 
                                        being set. */
    IN int MaxSubscriptionTimeOut   /** The maximum subscription time-out to 
                                        be accepted. */
    );

/** {\bf UpnpSubscribe} registers a control point to receive event
 *  notifications from another device.  This operation is synchronous.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: {\bf PublisherUrl} is not a valid URL.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf Timeout} is not a valid pointer 
 *              or {\bf SubsId} or {\bf PublisherUrl} is {\tt NULL}.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occured. 
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting to 
 *              {\bf PublisherUrl}.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error occurred creating a socket.
 *      \item {\tt UPNP_E_BAD_RESPONSE}: An error occurred in response from 
 *              the publisher.
 *      \item {\tt UPNP_E_SUBSCRIBE_UNACCEPTED}: The publisher refused 
 *              the subscription request.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSubscribe(
    IN UpnpClient_Handle Hnd,    /** The handle of the control point. */
    IN const char *PublisherUrl, /** The URL of the service to subscribe to. */
    INOUT int *TimeOut,          /** Pointer to a variable containing 
                                     the requested subscription time.  Upon 
                                     return, it contains the actual 
                                     subscription time returned from the 
                                     service. */
    OUT Upnp_SID SubsId          /** Pointer to a variable to receive the 
                                     subscription ID (SID). */
    );

/** {\bf UpnpSubscribeAsync} performs the same operation as
 *  {\bf UpnpSubscribe}, but returns immediately and calls the registered
 *  callback function when the operation is complete.
 *
 *  Note that many of the error codes for this function are returned in
 *  the {\bf Upnp_Event_Subscribe} structure.  In those cases, the function
 *  returns {\tt UPNP_E_SUCCESS} and the appropriate error code will
 *  be in the {\bf Upnp_Event_Subscribe.ErrCode} field in the {\bf Event}
 *  structure passed to the callback.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf PublisherUrl} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf TimeOut} or {\bf Fun} or 
 *              {\bf PublisherUrl} is not a valid pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occured (returned in 
 *              the {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket (returned in the 
 *              {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket (returned in the 
 *              {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding the socket 
 *              (returned in the {\bf Upnp_Event_Subscribe.ErrCode} field as 
 *              part of the callback).
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting to 
 *              {\bf PublisherUrl} (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error occurred creating  the 
 *              socket (returned in the {\bf Upnp_Event_Subscribe.ErrCode} 
 *              field as part of the callback).
 *      \item {\tt UPNP_E_BAD_RESPONSE}: An error occurred in response from 
 *              the publisher (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_SUBSCRIBE_UNACCEPTED}: The publisher refused 
 *              the subscription request (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSubscribeAsync(
    IN UpnpClient_Handle Hnd,      /** The handle of the control point that 
                                       is subscribing. */
    IN const char *PublisherUrl,   /** The URL of the service to subscribe 
                                       to. */
    IN int TimeOut,                /** The requested subscription time.  Upon 
                                       return, it contains the actual 
                                       subscription time returned from the 
                                       service. */
    IN Upnp_FunPtr Fun,            /** Pointer to the callback function for 
                                       this subscribe request. */
    IN const void *Cookie          /** A user data value passed to the 
                                       callback function when invoked. */
    );

/** {\bf UpnpUnSubscribe} removes the subscription of  a control point from a 
 *  service previously subscribed to using {\bf UpnpSubscribe} or 
 *  {\bf UpnpSubscribeAsync}.  This is a synchronous call.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_SID}: The {\bf SubsId} is not a valid 
 *              subscription ID.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occured. 
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting to 
 *              {\bf PublisherUrl}.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error ocurred creating a socket.
 *      \item {\tt UPNP_E_BAD_RESPONSE}: An error occurred in response from 
 *              the publisher.
 *      \item {\tt UPNP_E_UNSUBSCRIBE_UNACCEPTED}: The publisher refused 
 *              the unsubscribe request (the client is still unsubscribed and 
 *              no longer receives events).
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpUnSubscribe(
    IN UpnpClient_Handle Hnd, /** The handle of the subscribed control 
                                  point. */
    IN Upnp_SID SubsId        /** The ID returned when the control point 
                                  subscribed to the service. */
    );

/** {\bf UpnpUnSubscribeAsync} removes a subscription of a control
 *  point from a service previously subscribed to using {\bf
 *  UpnpSubscribe} or {\bf UpnpSubscribeAsync}, generating a callback
 *  when the operation is complete.
 *
 *  Note that many of the error codes for this function are returned in
 *  the {\bf Upnp_Event_Subscribe} structure.  In those cases, the function
 *  returns {\tt UPNP_E_SUCCESS} and the appropriate error code will
 *  be in the {\bf Upnp_Event_Subscribe.ErrCode} field in the {\bf Event}
 *  structure passed to the callback.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_HANDLE}: The handle is not a valid control 
 *              point handle.
 *      \item {\tt UPNP_E_INVALID_SID}: The {\bf SubsId} is not a valid SID.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf Fun} is not a valid callback 
 *              function pointer.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              complete this operation.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occured (returned in 
 *              the {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket (returned in the 
 *              {\bf Upnp_Event_Subscribe.ErrCode} field as part of the 
 *              callback).
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding the socket 
 *              (returned in the {\bf Upnp_Event_Subscribe.ErrCode} field as 
 *              part of the callback).
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting to 
 *              {\bf PublisherUrl} (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: An error occurred creating a socket (
 *              returned in the {\bf Upnp_Event_Subscribe.ErrCode} field as 
 *              part of the callback).
 *      \item {\tt UPNP_E_BAD_RESPONSE}: An error occurred in response from 
 *              the publisher (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *      \item {\tt UPNP_E_UNSUBSCRIBE_UNACCEPTED}: The publisher refused 
 *              the subscription request (returned in the {\bf 
 *              Upnp_Event_Subscribe.ErrCode} field as part of the callback).
 *    \end{itemize} */

EXPORT_SPEC int UpnpUnSubscribeAsync(
    IN UpnpClient_Handle Hnd, /** The handle of the subscribed control 
                                  point. */
    IN Upnp_SID SubsId,       /** The ID returned when the 
                                  control point subscribed to the service. */
    IN Upnp_FunPtr Fun,       /** Pointer to a callback function to be 
                                  called when the operation is complete. */
    IN const void *Cookie     /** Pointer to user data to pass to the 
                                  callback function when invoked. */
    );

/*! @} */ /* Eventing */


/******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                        C L I E N T - A P I                                 *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************/

/*! @name Control Point HTTP API */
/*! @{ */

/** {\bf UpnpDownloadUrlItem} downloads a file specified in a URL.
 *  The SDK allocates the memory for {\bf outBuf} and the 
 *  application is responsible for freeing this memory.  Note that
 *  the item is passed as a single buffer.  Large items should not
 *  be transferred using this function.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf url}, {\bf outBuf} 
 *              or {\bf contentType} is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf url} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              download this file.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting a 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpDownloadUrlItem(
    IN const char *url,          /** URL of an item to download. */
    OUT char **outBuf,           /** Buffer to store the downloaded item. */
    OUT char *contentType        /** HTTP header value content type if 
                                     present. It should be at least 
                                     {\tt LINE_SIZE} bytes in size. */
    );

/** {\bf UpnpOpenHttpGet} gets a file specified in a URL.
 *  The SDK allocates the memory for {\bf handle} and 
 *  {\bf contentType}, the application is responsible for freeing this memory.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf url}, {\bf handle},  
 *              {\bf contentType}, {\bf contentLength} or {\bf httpStatus} 
 *		is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf url} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              download this file.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting a 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *	\item {\tt UPNP_E_BAD_RESPONSE}: A bad response was received from the 
 *	        remote server.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpOpenHttpGet(
	IN const char *url,	    /** The URL of an item to get. */
	IN OUT void **handle,       /** A pointer to store the handle for 
				        this connection. */
	IN OUT char **contentType,  /** A buffer to store the media type of 
				        the item. */
	IN OUT int *contentLength,  /** A pointer to store the length of the 
				        item. */
	IN OUT int *httpStatus,	    /** The status returned on receiving a 
				        response message. */
	IN int timeout		    /** The time out value sent with the 
				        request during which a response is 
					expected from the server, failing 
					which, an error is reported back to 
					the user. */		 
	  );

/** {\bf UpnpOpenHttpGetProxy} gets a file specified in a URL through the
 * specified proxy.
 *  The SDK allocates the memory for {\bf handle} and 
 *  {\bf contentType}, the application is responsible for freeing this memory.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf url}, {\bf handle},  
 *              {\bf contentType}, {\bf contentLength} or {\bf httpStatus} 
 *		is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf url} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              download this file.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting a 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *	\item {\tt UPNP_E_BAD_RESPONSE}: A bad response was received from the 
 *	        remote server.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpOpenHttpGetProxy(
	IN const char *url,	    /** The URL of an item to get. */
    IN const char *proxy_str,    /** The URL of the proxy. */
	IN OUT void **handle,       /** A pointer to store the handle for 
				        this connection. */
	IN OUT char **contentType,  /** A buffer to store the media type of 
				        the item. */
	IN OUT int *contentLength,  /** A pointer to store the length of the 
				        item. */
	IN OUT int *httpStatus,	    /** The status returned on receiving a 
				        response message. */
	IN int timeout		    /** The time out value sent with the 
				        request during which a response is 
					expected from the server, failing 
					which, an error is reported back to 
					the user. */		 
	  );

/** {\bf UpnpOpenHttpGetEx} gets specified number of bytes from a file 
 *  specified in the URL. The number of bytes is specified through a low 
 *  count and a high count which are passed as a range of bytes for the 
 *  request.  The SDK allocates the memory for {\bf handle} and 
 *  {\bf contentType}, the application is responsible for freeing this memory.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf url}, {\bf handle},  
 *              {\bf contentType}, {\bf contentLength} or {\bf httpStatus} 
 *		is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf url} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              download this file.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting a 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *	\item {\tt UPNP_E_BAD_RESPONSE}: A bad response was received from the 
 *	        remote server.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpOpenHttpGetEx(
	IN const char *url,         /** The URL of the item to get. */
	IN OUT void **handle,       /** A pointer to store the handle for 
				        this connection. */
	IN OUT char **contentType,  /** A buffer to store the media type of the 
	                                item. */
	IN OUT int *contentLength,  /** A buffer to store the length of the 
				        item. */
	IN OUT int *httpStatus,	    /** The status returned on receiving a 
				        response message from the remote 
					server. */
	IN int lowRange,            /** An integer value representing the low 
				        end of a range to retrieve. */
	IN int highRange,           /** An integer value representing the high 
				        end of a range to retrieve. */
	IN int timeout	            /** A time out value sent with the request 
				      	during which a response is expected 
					from the server, failing which, an 
					error is reported back to the user. */	
	  );

/** {\bf UpnpReadHttpGet} gets specified number of bytes from a file 
 *  specified in a URL. 
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf handle}, {\bf buf} 
 *              or {\bf size} is not a valid pointer.
 *	    \item {\tt UPNP_E_BAD_RESPONSE}: A bad response was received from the 
 *	            remote server.
 *      \item {\tt UPNP_E_BAD_HTTPMSG}: Either the request or response was in 
 *              the incorrect format.
 *      \item {\tt UPNP_E_CANCELED}: another thread called UpnpCancelHttpGet.
 *    \end{itemize}
 *
 *  Note: In case of return values, the status code parameter of the passed 
 *        in handle value may provide additional information on the return 
 *        value.
 */

EXPORT_SPEC int UpnpReadHttpGet(
	IN void *handle,           /** The token created by the call to 
				       {\bf UpnpOpenHttpGet}. */
	IN OUT char *buf,          /** The buffer to store the read item. */
	IN OUT unsigned int *size, /** The size of the buffer to be read. */
	IN int timeout             /** The time out value sent with the 
				       request during which a response is 
				       expected from the server, failing 
				       which, an error is reported back to 
				       the user. */
	);

/** {\bf UpnpHttpGetProgress} rettrieve progress information of a http-get 
 *  transfer. 
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf handle}, {\bf length} 
 *              or {\bf total} is not a valid pointer.
 *    \end{itemize}
 *
 */
EXPORT_SPEC int UpnpHttpGetProgress(
    IN void *handle,           /** The token created by the call to
				       {\bf UpnpOpenHttpGet}. */
	OUT unsigned int *length, /** The number of bytes received. */
	OUT unsigned int *total   /** The content length. */
    );


/** {\bf UpnpCancelHttpGet} set the cancel flag of the  {\bf handle}
 * parameter. 
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf handle} is not a valid pointer.
 *    \end{itemize}
 */  

EXPORT_SPEC int UpnpCancelHttpGet(IN void *handle);

/** {\bf UpnpCloseHttpGet} closes the connection and frees memory that was 
 *	allocated for the {\bf handle} parameter.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: {\bf handle} is not a valid pointer.
 *    \end{itemize}
 */  

EXPORT_SPEC int UpnpCloseHttpGet(IN void *handle);


/** {\bf UpnpOpenHttpPost} makes an HTTP POST request message, opens a 
 *  connection to the server and sends the POST request to the server if 
 *  the connection to the server succeeds.
 *  The SDK allocates the memory for {\bf handle} and 
 *  {\bf contentType}, the application is responsible for freeing this memory.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf url}, {\bf handle} 
 *              or {\bf contentType} is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf url} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: Insufficient resources exist to 
 *              download this file.
 *      \item {\tt UPNP_E_SOCKET_ERROR}: Error occured allocating a socket and 
 *		resources or an error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting a 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpOpenHttpPost(
	IN const char *url,         /** The URL in which to send the POST 
				        request. */
	IN OUT void **handle,	    /** A pointer in which to store the 
	                                handle for this connection.  This 
					handle is required for futher 
					operations over this connection. */
	IN const char *contentType, /** A buffer to store the media type of 
				        content being sent.  */
	IN int contentLength,       /** The length of the content, in bytes, 
				        being posted. */
	IN int timeout              /** The time out value sent with the 
				        request during which a response is 
					expected from the receiver, failing 
					which, an error is reported. */		 
	);


/** {\bf UpnpWriteHttpPost} sends a request to a server to copy the contents of 
 *	a buffer to the URI specified in the {\bf UpnpOpenHttpPost} call.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf handle}, {\bf buf} 
 *              or {\bf size} is not a valid pointer.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpWriteHttpPost(
	IN void *handle,          /** The handle of the connection created 
				      by the call to {\bf UpnpOpenHttpPost}. */
	IN char *buf,             /** The buffer to be posted. */
	IN unsigned int *size,    /** The size, in bytes of {\bf buf}. */
	IN int timeout            /** A timeout value sent with the request 
				      during which a response is expected 
				      from the server, failing which, an 
				      error is reported. */		 
	);

/** {\bf UpnpCloseHttpPost} sends and receives any pending data, closes the 
 *	connection with the server, and frees memory allocated during the 
 *	{\bfUpnpOpenHttpPost} call.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf handle}, or 
 *				{\bf httpStatus} is not a valid pointer.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *    \end{itemize}
 */
  
EXPORT_SPEC int UpnpCloseHttpPost(
	IN void *handle,          /** The handle of the connection to close, 
				      created by the call to 
				      {\bf UpnpOpenHttpPost}. */
	IN OUT int *httpStatus,   /** A pointer to a buffer to store the 
	                              final status of the connection. */
	IN int timeout            /** A time out value sent with the request 
				      during which a response is expected from 
				      the server, failing which, an error is 
				      reported. */		 
	);
  

/** {\bf UpnpDownloadXmlDoc} downloads an XML document specified in a URL.
 *  The SDK parses the document and returns it in the form of a 
 *  DOM document. The application is responsible for freeing the DOM document.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *      \item {\tt UPNP_E_SUCCESS}: The operation completed successfully.
 *      \item {\tt UPNP_E_INVALID_PARAM}: Either {\bf url} or {\bf xmlDoc} 
 *              is not a valid pointer.
 *      \item {\tt UPNP_E_INVALID_DESC}: The XML document was not 
 *              found or it does not contain a valid XML description.
 *      \item {\tt UPNP_E_INVALID_URL}: The {\bf url} is not a valid 
 *              URL.
 *      \item {\tt UPNP_E_OUTOF_MEMORY}: There are insufficient resources to 
 *              download the XML document.
 *      \item {\tt UPNP_E_NETWORK_ERROR}: A network error occurred.
 *      \item {\tt UPNP_E_SOCKET_WRITE}: An error or timeout occurred writing 
 *              to a socket.
 *      \item {\tt UPNP_E_SOCKET_READ}: An error or timeout occurred reading 
 *              from a socket.
 *      \item {\tt UPNP_E_SOCKET_BIND}: An error occurred binding a socket.
 *      \item {\tt UPNP_E_SOCKET_CONNECT}: An error occurred connecting the 
 *              socket.
 *      \item {\tt UPNP_E_OUTOF_SOCKET}: Too many sockets are currently 
 *              allocated.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpDownloadXmlDoc(
    IN const char *url,          /** URL of the XML document. */
    OUT IXML_Document **xmlDoc   /** A pointer in which to store the 
				     XML document. */
    );

/*! @} */ /* Control Point HTTP API */

/******************************************************************************
 ******************************************************************************
 *                                                                            *
 *                    W E B  S E R V E R  A P I                               *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************/

/*! @name Web Server API */
/*! @{ */

/** {\bf UpnpSetWebServerRootDir} sets the document root directory for
 *  the internal web server. This directory is considered the
 *  root directory (i.e. "/") of the web server.
 *
 *  This function also activates or deactivates the web server.
 *  To disable the web server, pass {\tt NULL} for {\bf rootDir}; to 
 *  activate, pass a valid directory string.
 *  
 *  Note that this function is not available when the web server is not
 *  compiled into the SDK.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *       \item {\tt UPPN_E_SUCCESS}: The operation completed successfully.
 *       \item {\tt UPNP_E_INVALID_ARGUMENT}: {\bf rootDir} is an invalid 
 *               directory.
 *    \end{itemize}
 */
 
EXPORT_SPEC int UpnpSetWebServerRootDir( 
    IN const char* rootDir  /** Path of the root directory of the web 
                                server. */
    );

/** {\bf UpnpSetVirtualDirCallbacks} sets the callback function to be used to 
 *  access a virtual directory.  Refer to the description of
 *  {\bf UpnpVirtualDirCallbacks} for a description of the functions.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *       \item {\tt UPPN_E_SUCCESS}: The operation completed successfully.
 *       \item {\tt UPNP_E_INVALID_ARGUMENT}: {\bf callbacks} is not a valid 
 *               pointer.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpSetVirtualDirCallbacks(
    IN struct UpnpVirtualDirCallbacks *callbacks /** Pointer to a structure 
                                                     containing points to the 
                                                     virtual interface 
                                                     functions. */
    );

/** {\bf UpnpEnableWebServer} enables or disables the webserver.  A value of
 *  {\tt TRUE} enables the webserver, {\tt FALSE} disables it.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *       \item {\tt UPPN_E_SUCCESS}: The operation completed successfully.
 *       \item {\tt UPNP_E_INVALID_ARGUMENT}: {\bf enable} is not valid.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpEnableWebserver(
    IN int enable /** {\tt TRUE} to enable, {\tt FALSE} to disable. */
    );

/** {\bf UpnpIsWebServerEnabled} returns {\tt TRUE} if the webserver is
 *  enabled, or {\tt FALSE} if it is not.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *       \item {\tt TRUE}: The webserver is enabled.
 *       \item {\tt FALSE}: The webserver is not enabled
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpIsWebserverEnabled(void);

/** {\bf UpnpAddVirtualDir} adds a virtual directory mapping.
 *
 *  All webserver requests containing the given directory are read using
 *  functions contained in a {\bf UpnpVirtualDirCallbacks} structure registered
 *  via {\bf UpnpSetVirtualDirCallbacks}.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *       \item {\tt UPPN_E_SUCCESS}: The operation completed successfully.
 *       \item {\tt UPNP_E_INVALID_ARGUMENT}: {\bf dirName} is not valid.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpAddVirtualDir(
    IN const char *dirName /** The name of the new directory mapping to add. 
							*/
    );

/** {\bf UpnpRemoveVirtualDir} removes a virtual directory mapping made with
 *  {\bf UpnpAddVirtualDir}.
 *
 *  @return [int] An integer representing one of the following:
 *    \begin{itemize}
 *       \item {\tt UPPN_E_SUCCESS}: The operation completed successfully.
 *       \item {\tt UPNP_E_INVALID_ARGUMENT}: {\bf dirName} is not valid.
 *    \end{itemize}
 */

EXPORT_SPEC int UpnpRemoveVirtualDir(
    IN const char *dirName /** The name of the virtual directory mapping to 
                               remove. */
    );

/** {\bf UpnpRemoveAllVirtualDirs} removes all virtual directory mappings.
 *
 *  @return [void] This function does not return a value.
 *
 */

EXPORT_SPEC void UpnpRemoveAllVirtualDirs(void);

EXPORT_SPEC void UpnpFree(
    IN void *item /* The item to free. */
    );

/*! @} */ /* Web Server API */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*! @} */ /* The API */

#endif

