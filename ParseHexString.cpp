//////////////////////////////////////////////////////////////////////////////
// SelectionExample.cpp - Sample Hex Workshop Plugin
//
// This source code and information is provided "as is" without any warranty
// of any kind, either expressed or implied, including but not limited to 
// the implied waranties of merchantability and/or fitness for a particular 
// purpose.
//
// Copyright (c) 2010 BreakPoint Software, Inc.  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

// Includes
#include "stdafx.h"

#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "hwapi.h"

// Plug-in Command constants
#define FILL_RANDOM_COMMAND    _T("Selection Example\\Fill Random Bytes")
#define CLIP_SELECTION_COMMAND _T("Selection Example\\Clip to Selection")
#define COPY_NEW_FILE_COMMAND  _T("Selection Example\\Copy to New Document")

// Block size used during copy operation (64K chunks)
#define COPY_BLOCK_SIZE  0x10000 

// Forward declarations (helper functions that perform tasks)
BOOL doFillRandom(HWSESSION hSession, HWDOCUMENT hDoc) ;
BOOL doClipSelection(HWSESSION hSession, HWDOCUMENT hDoc) ;
BOOL doCopyToNewFile(HWSESSION hSession, HWDOCUMENT hDoc) ;

// DllMain
BOOL APIENTRY DllMain(HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved)
{
    return TRUE;
}

// Plugin Entrypoint: Identify available Plugin-Commands
HWAPIEP BOOL HWPLUGIN_Identify(LPTSTR lpstrPluginCommand,
                               size_t nMaxPluginCommand)
{
    _sntprintf(lpstrPluginCommand, nMaxPluginCommand,
            _T("%s;%s;%s"), 
            FILL_RANDOM_COMMAND,
            CLIP_SELECTION_COMMAND,
            COPY_NEW_FILE_COMMAND) ;

    return TRUE ;
}


// Plugin Entrypoint: Determine capabilities/pre-conditions for a command
HWAPIEP DWORD HWPLUGIN_RequestCapabilities(LPCTSTR lpstrPluginCommand)
{
    // Return unique capabilities for each Plug-in command

    if (_tcsicmp(lpstrPluginCommand, FILL_RANDOM_COMMAND) == 0)
    {
        // Fill Random requires both a file and a selection range
        return HWPLUGIN_CAP_FILE_REQUIRE | HWPLUGIN_CAP_SELECTION_REQUIRE ;
    }
    else if (_tcsicmp(lpstrPluginCommand, CLIP_SELECTION_COMMAND) == 0)
    {
        // Clip Selection requires both a file and a selection range
        return HWPLUGIN_CAP_FILE_REQUIRE | HWPLUGIN_CAP_SELECTION_REQUIRE ;
    }
    else if (_tcsicmp(lpstrPluginCommand, COPY_NEW_FILE_COMMAND) == 0)
    {
        // Copy to new file require a file, but selection is optional
        return HWPLUGIN_CAP_FILE_REQUIRE ;
    }

    return 0 ;
}

// Plugin Entrypoint: Execute a plugin command
HWAPIEP BOOL HWPLUGIN_Execute( LPCTSTR        lpstrPluginCommand,
                               HWSESSION    hSession,
                               HWDOCUMENT    hDocument )
{
    // Delegate plug-in command to helper functioms
    if (_tcsicmp(lpstrPluginCommand, FILL_RANDOM_COMMAND) == 0)
    {
        // Fill w/ Random Bytes
        return doFillRandom(hSession, hDocument) ;
    }
    else if (_tcsicmp(lpstrPluginCommand, CLIP_SELECTION_COMMAND) == 0)
    {
        // Clip the document to the selection
        return doClipSelection(hSession, hDocument) ;
    }
    else if (_tcsicmp(lpstrPluginCommand, COPY_NEW_FILE_COMMAND) == 0)        
    {
        // Copy selection/entire file to a new document
        return doCopyToNewFile(hSession, hDocument) ;
    }
    else
    {
        // Unknown Command
        return FALSE ;
    }
}

// Fill w/ random byte implementation
BOOL doFillRandom(HWSESSION hSession, HWDOCUMENT hDoc) 
{
    BOOL  bRC = FALSE ;
    QWORD qwStartPosition ;
    QWORD qwLength ;

    // Check readonly document status
    BOOL bReadOnly = TRUE ;
    hwGetReadOnly(hDoc, &bReadOnly) ;
    if (bReadOnly)
    {
        MessageBox(hwGetWindowHandle(hSession),
                _T("Document is read-only; cannot perform operation."),
                _T("Error"),
                MB_ICONSTOP | MB_APPLMODAL) ;
    }
    else
    {
        // Seed random generator
        srand( (unsigned)time( NULL ) );

        // Obtain starting position and length
        if ((hwGetCaretPosition(hDoc, &qwStartPosition) == HWAPI_RESULT_SUCCESS) &&
            (hwGetSelection(hDoc, &qwLength) == HWAPI_RESULT_SUCCESS))
        {
            bRC = TRUE ;

            // Group all changes into a single undo operation
            hwUndoBeginGroup(hDoc) ;

            // Replace each byte in the range with random data.  
            //
            // NOTE: This method is only recommended for small selections and 
            // is fairly slow.   For larger selections, one should work with 
            // large chunks of data (e.g. 64K blocks) instead of individual 
            // bytes.
            for (QWORD i=0; i<qwLength; i++)
            {
                char cByte = rand() % 256 ;
                if (hwReplaceAt(hDoc, qwStartPosition + i, &cByte, 1, 1) != 
                        HWAPI_RESULT_SUCCESS)
                {
                    // Report error to log
                    hwOutputLog(hSession, HWLOG_ERR, 
                            _T("Failed to replace byte at offset %08I64X"), qwStartPosition+i) ;
                    bRC = FALSE ;
                    break ;
                }

                // Update progress every 1K
                if ((i % 1024) == 0)
                {
                    TCHAR cStatus[256] ;

                    _sntprintf(cStatus, sizeof(cStatus) / sizeof(TCHAR), 
                            _T("Random Fill: %I64d of %I64d"), i, qwLength) ;

                    QWORD percentComplete = (i * 100) / qwLength ;
                    if (hwUpdateProgress(hSession, (int) percentComplete, cStatus) != 
                            HWAPI_RESULT_SUCCESS)
                    {
                        // Error indicates user cancel
                        hwOutputLog(hSession, HWLOG_INFO, 
                                _T("User aborted operation")) ;
                        bRC = FALSE ;
                        break ;
                    }
                }
            }
            
            if (bRC)
            {
                // Report success via the output log
                hwOutputLog(hSession, HWLOG_INFO, 
                    _T("Replaced byte range [%08I64X .. %08I64X] with random bytes"), 
                    qwStartPosition, qwStartPosition+qwLength) ;
            }

            // Commit the undo group
            hwUndoEndGroup(hDoc) ;        
        }
    }

    return bRC ;
}

// Clip document to selection implementation
BOOL doClipSelection(HWSESSION hSession, HWDOCUMENT hDoc) 
{
    BOOL  bRC = FALSE ;
    QWORD qwStartPosition ;
    QWORD qwLength ;
    QWORD qwDocSize ;

    // Check readonly document status
    BOOL bReadOnly = TRUE ;
    hwGetReadOnly(hDoc, &bReadOnly) ;
    if (bReadOnly)
    {
        MessageBox(hwGetWindowHandle(hSession),
                _T("Document is read-only; cannot perform operation."),
                _T("Error"),
                MB_ICONSTOP) ;
    }
    else
    {
        // Obtain starting position, length, and document size
        if ((hwGetCaretPosition(hDoc, &qwStartPosition) == HWAPI_RESULT_SUCCESS) &&
            (hwGetSelection(hDoc, &qwLength) == HWAPI_RESULT_SUCCESS) &&
            (hwGetDocumentSize(hDoc, &qwDocSize) == HWAPI_RESULT_SUCCESS))
        {
            bRC = TRUE ;

            // Group all changes into a single undo operation
            hwUndoBeginGroup(hDoc) ;

            // Remove data after selection
            QWORD qwCutLength = qwDocSize - (qwStartPosition + qwLength) ;
            if (qwCutLength > 0)
            {
                if (hwDeleteAt(hDoc, qwStartPosition + qwLength, qwCutLength) 
                        != HWAPI_RESULT_SUCCESS)
                {
                    // Error errors to output log
                    hwOutputLog(hSession, HWLOG_ERR, 
                            _T("Failed to delete range [%08I64X .. %08I64X]"), 
                            qwStartPosition + qwLength, 
                            qwStartPosition + qwLength + qwCutLength) ;
                    bRC = FALSE ;
                }
            }

            // Remove data prior to selection
            if (qwStartPosition > 0)
            {
                if (hwDeleteAt(hDoc, 0, qwStartPosition) 
                        != HWAPI_RESULT_SUCCESS)
                {
                    // Error errors to output log
                    hwOutputLog(hSession, HWLOG_ERR, 
                            _T("Failed to delete range [%08I64X .. %08I64X]"), 
                            0, 
                            qwStartPosition) ;
                    bRC = FALSE ;
                }
            }

            // Commit the undo group
            hwUndoEndGroup(hDoc) ;

            // Reset caret position and selection
            hwSetCaretPosition(hDoc, 0) ;
            hwSetSelection(hDoc, qwLength) ;
            bRC = TRUE ;
        }
    }

    return bRC ;
}


// Copy selection/entire to new document implementation
BOOL doCopyToNewFile(HWSESSION hSession, HWDOCUMENT hDoc) 
{
    BOOL  bRC = FALSE ;
    QWORD qwStartPosition ;
    QWORD qwLength ;
    QWORD qwDocSize ;    

    // Obtain starting position, length, and document size
    if ((hwGetCaretPosition(hDoc, &qwStartPosition) == HWAPI_RESULT_SUCCESS) &&
        (hwGetSelection(hDoc, &qwLength) == HWAPI_RESULT_SUCCESS) &&
        (hwGetDocumentSize(hDoc, &qwDocSize) == HWAPI_RESULT_SUCCESS))
    {
        // If no selection, use entire document
        if (qwLength == 0)
        {
            qwStartPosition = 0 ; 
            qwLength = qwDocSize ;
        }

        // Create a new document
        HWDOCUMENT hNewDoc = hwNewDocument(hSession) ;
        if (hNewDoc != NULL)
        {
            bRC = TRUE ;

            // No need to allow undo for a new document
            hwUndoDisable(hNewDoc) ;

            QWORD qwBytesLeft = qwLength ; 
            QWORD qwBytesCopied = 0 ;
            char cBuffer[COPY_BLOCK_SIZE] ;

            // Init tick count for progress indicator
            DWORD dwNextProgress = GetTickCount() + 1000 ;
            QWORD qwLastProgressCount = 0 ;

            // Copy Data
            while (qwBytesLeft > 0)
            {
                QWORD qwBytesToCopy = __min(COPY_BLOCK_SIZE, qwLength - qwBytesCopied) ;
                if (hwReadAt(hDoc, qwStartPosition + qwBytesCopied, cBuffer, qwBytesToCopy) 
                        == HWAPI_RESULT_SUCCESS)
                {
                    if (hwInsertAt(hNewDoc, qwBytesCopied, cBuffer, qwBytesToCopy) == HWAPI_RESULT_SUCCESS)
                    {
                        qwBytesLeft -= qwBytesToCopy ;
                        qwBytesCopied += qwBytesToCopy ;
                    }
                    else
                    {
                        // Report errors to output conosole
                        hwOutputLog(hSession, HWLOG_ERR, 
                                _T("Failed to insert at offset %08I64X"), 
                                qwBytesCopied) ;
                        bRC = FALSE ;
                        break ;
                    }
                }
                else
                {
                    // Report errors to output conosole
                    hwOutputLog(hSession, HWLOG_ERR, 
                            _T("Failed to read at offset %08I64X"), 
                            qwStartPosition + qwBytesCopied) ;
                    bRC = FALSE ;
                    break ;
                }

                // Update progress every 1 second (1000 ms)
                if (GetTickCount() > dwNextProgress)
                {
                    dwNextProgress = GetTickCount() + 1000 ;
                    QWORD qwCopied = qwBytesCopied - qwLastProgressCount ;
                    qwLastProgressCount = qwBytesCopied ;

                    TCHAR cStatus[256] ;

                    _sntprintf(cStatus, sizeof(cStatus) / sizeof(TCHAR), 
                            _T("Copying: %I64d of %I64d (%I64d KB/sec)"), 
                            qwBytesCopied, qwLength, qwCopied / 1024) ;
    
                    QWORD percentComplete = (qwBytesCopied * 100) / qwLength ;
                    if (hwUpdateProgress(hSession, (int) percentComplete, cStatus) != 
                            HWAPI_RESULT_SUCCESS)
                    {
                        // Error indicates user cancel
                        hwOutputLog(hSession, HWLOG_INFO,
                                _T("User aborted operation")) ;
                        bRC = FALSE ;
                        break ;
                    }
                }
            }

            // Enable Undo for future operations
            hwUndoEnable(hNewDoc) ;
        }        
    }

    return bRC ;    
}
