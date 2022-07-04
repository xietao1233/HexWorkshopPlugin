//////////////////////////////////////////////////////////////////////////////
// hwapierr.h - Hex Workshop API Error Definitions
//
// This source code and information is provided "as is" without any warranty
// of any kind, either expressed or implied, including but not limited to 
// the implied waranties of merchantability and/or fitness for a particular 
// purpose.
//
// Copyright (c) 2010 BreakPoint Software, Inc.  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////
//
////

#ifndef _HWAPI_RESULT_H
#define _HWAPI_RESULT_H /* [ */

/** 
 * @brief Hex Workshop Plug-in API Error Definitions
 *
 * hwapierr.h defines the Hex Workshop Plug-in error return codes for 
 * functions defined in hwapi.h
 **/

/**
 * @brief Hex Workshop Plug-in API return codes
 * @ingroup errors
 * 
 * HWAPI_RESULT_SUCCESS (0) is return for a successful operations, otherwise the 
 * operation failed and the return code indicates the failure cause.
 */
typedef enum HWAPI_RESULT
{
    HWAPI_RESULT_SUCCESS            = 0x00000000,   /**< Success; no errors */
    HWAPI_RESULT_OUTOFRANGE         = 0x80000001,   /**< An offset, index or length is out of bounds */
    HWAPI_RESULT_CLOSED_DOCUMENT    = 0x80000002,   /**< Cannot operate on a closed document */
    HWAPI_RESULT_INVALID_HWDOCUMENT = 0x80000003,   /**< The HWDOCUMENT handle is not valid */
    HWAPI_RESULT_INVALID_HWSESSION  = 0x80000004,   /**< The HWSESSION handle is not valid */
    HWAPI_RESULT_INVALID_PARAMETER  = 0x80000005,   /**< An invalid parameter was passed to this API */
    HWAPI_RESULT_NOT_IMPLEMENTED    = 0x80000006,   /**< The api method has not yet been implemented */
    HWAPI_RESULT_BUFFER_TOO_SMALL   = 0x80000007,   /**< The supplied buffer it too small for the operation */
    HWAPI_RESULT_USER_ABORT         = 0x80000008,   /**< The user aborted the operation */
    HWAPI_RESULT_FAILED             = 0x80000009,   /**< The operation failed */
    HWAPI_RESULT_NOT_FOUND          = 0x8000000A,   /**< Unable to find requested resource */
    HWAPI_RESULT_INTERNAL_ERROR     = 0xFFFFFFFF,   /**< An internal unexpected error occured */
} HWAPI_RESULT ;

#endif /* ] */

