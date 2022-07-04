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

#define IsHexChar(a) ((a >= '0' && a <= '9') || (a >= 'A' && a <= 'F') || (a >= 'a' && a <= 'f'))
#define IsSkipChar(a) ((a == ' ') || (a == 0xd) || (a == 0xa) || (a == '\t'))
#define IsNumber(a) ((a >= '0' && a <= '9'))
#define IsUpper(a) ((a >= 'a' && a <= 'f'))
#define IsLower(a) ((a >= 'A' && a <= 'F'))

// Plug-in Command constants
#define PARSE_HEX_STRING  _T("Parse Hex String\\Parse")

// Forward declarations (helper functions that perform tasks)
BOOL doParseHexString(HWSESSION hSession, HWDOCUMENT hDoc);

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
		_T("%s"),
        PARSE_HEX_STRING);

    return TRUE ;
}


// Plugin Entrypoint: Determine capabilities/pre-conditions for a command
HWAPIEP DWORD HWPLUGIN_RequestCapabilities(LPCTSTR lpstrPluginCommand)
{
    // Return unique capabilities for each Plug-in command

    if (_tcsicmp(lpstrPluginCommand, PARSE_HEX_STRING) == 0)
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
    if (_tcsicmp(lpstrPluginCommand, PARSE_HEX_STRING) == 0)
    {
        // parse hex string
        return doParseHexString(hSession, hDocument) ;
    }
    else
    {
        // Unknown Command
        return FALSE ;
    }
}

BOOL doParseHexString(HWSESSION hSession, HWDOCUMENT hDoc)
{
    BOOL bReturn = FALSE;
	QWORD qwStartPosition;
	QWORD qwLength;
    HWND hMain = hwGetWindowHandle(hSession);

	// Check readonly document status
	BOOL bReadOnly = TRUE;
	hwGetReadOnly(hDoc, &bReadOnly);
	if (bReadOnly)
	{
		MessageBox(hMain,
			_T("Document is read-only; cannot perform operation."),
			_T("Error"),
			MB_ICONSTOP | MB_APPLMODAL);
        return bReturn;
	}

	// Obtain starting position and length
    if ((hwGetCaretPosition(hDoc, &qwStartPosition) == HWAPI_RESULT_SUCCESS) &&
        (hwGetSelection(hDoc, &qwLength) == HWAPI_RESULT_SUCCESS))
    {
        HANDLE hClip = NULL; 
        LPSTR pData = NULL;
        LPSTR pStr = NULL;

        __try 
        {
			// Group all changes into a single undo operation
			hwUndoBeginGroup(hDoc);

            if (!IsClipboardFormatAvailable(CF_TEXT))
                __leave;
            if (!OpenClipboard(hMain))
            {    
                MessageBox(hMain, _T("·¢¿ª¼ôÇÐ°åÊ§°Ü!"), _T("´íÎó"), MB_OK);
                __leave;
            }

            hClip = GetClipboardData(CF_TEXT);
            if (!hClip)
                __leave;
            pData = (LPSTR)GlobalLock(hClip);

            size_t uDataLen = strlen(pData);
            size_t u1 = 0, u2 = 0, l1 = uDataLen, l2 = 0;
			if (uDataLen) 
            {
                l2 = ((uDataLen / 2) | 15) + 1;
                pStr = new char[l2];
                LPSTR p1 = pData, p2 = pStr;
                char szBuff[4] = { 0 };

                while ((p1 + 1) < (pData + uDataLen))
                {
                    if (IsHexChar(*p1) && IsHexChar(*(p1 + 1)))
                    {
                        szBuff[0] = *p1;
                        szBuff[1] = *(p1+1);
                        szBuff[2] = 0;
                        *(p2 + u2) = (char)strtol(szBuff, 0, 16);
                        u2 += 1;
                        p1 += 2;
                    }
                    else if (IsSkipChar(*p1))
                    {
                        p1 += 1;
                    }
                    else
                    {
                        // error
						u2 = 0;
						break;
                    }
                }

				hwInsertAt(hDoc, qwStartPosition, pStr, u2);

				if (p1 != (pData + uDataLen))
				{
					MessageBox(hMain, _T("½âÎöÈ±Ê§²¿·ÖÄ©Î²Êý¾Ý!"), _T("¾¯¸æ"), MB_OK);
				}
			}
        }
        __finally
        {
			if (pStr)
				delete[] pStr;
            if (pData)
                GlobalUnlock(pData);
            CloseClipboard();
		    // Commit the undo group
		    hwUndoEndGroup(hDoc);
        }
    }

    return bReturn;
}
