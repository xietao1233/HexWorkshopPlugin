//////////////////////////////////////////////////////////////////////////////
// hwapi.h - Hex Workshop API
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

#ifndef _HWAPI_H
#define _HWAPI_H /* [ */

#include "hwapierr.h"

#ifdef HWAPI_EXPORTS
#  define HWAPI   extern "C" __declspec(dllexport)
#  define HWAPIEP
#else
#  define HWAPI   extern "C" __declspec(dllimport)
#  define HWAPIEP extern "C" __declspec(dllexport)
#endif

/**
 * @brief Hex Workshop Document Handle
 * @ingroup common
 *
 * A HWDOCUMENT handle represents an open document within Hex Workshop.  
 * Plug-ins are provided a HWDOCUMENT during Plug-in execution or Plug-ins
 * receive document handles by calling hwOpenDocument or hwNewDocument.
 *
 * Handles should not be stored or re-used across plug-in executions.
 */
typedef HANDLE HWDOCUMENT ;

/**
 * @brief Hex Workshop Plug-in Session Handle
 * @ingroup common
 *
 * HWSESSION handles are provided to the plug-in during execution and 
 * represent a plug-in execution session. 
 *
 * Handles should not be stored or re-used across plug-in executions.
 */
typedef HANDLE HWSESSION ;

/**
 * @brief signed 64 bit integer
 * @ingroup common
 */
typedef __int64 QWORD ;

/**
 * @brief Signed/unsigned indicator
 * @ingroup common
 */
typedef enum 
{
    HWAPI_SIGN_SIGNED   = 0,    /**< @brief signed value */
    HWAPI_SIGN_UNSIGNED = 1     /**< @brief unsigned value */
} HWAPI_SIGN ;


/**
 * @brief Byte Order indicator
 * @ingroup common
 */
typedef enum 
{
    HWAPI_BYTEORDER_BIG_ENDIAN    = 0,  /**< @brief Big Endian (MSB) */
    HWAPI_BYTEORDER_LITTLE_ENDIAN = 1   /**< @brief Little Endian (LSB) */
} HWAPI_BYTEORDER ;


/** @name Open, Close, Save APIs */

/**
 * @brief Open an existing document
 * @ingroup document
 * 
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param lpstrFile [IN] Path to file
 * @param bReadOnly [IN] TRUE to open read-only, FALSE for write access
 *
 * @return NULL if unable to open a file or a non-NULL value on success
 */
HWAPI HWDOCUMENT hwOpenDocument(HWSESSION    hSession,
                                LPCTSTR     lpstrFile,
                                BOOL        bReadOnly) ;

/**
 * @brief Create a new empty document
 * @ingroup document
 *
 * @param hSession [IN] Hex Workshop Plug-in session handle
 *
 * @return NULL on error or a non-NULL value on success
 */
HWAPI HWDOCUMENT hwNewDocument(HWSESSION hSession) ;

/**
 * @brief Save document
 * @ingroup document
 *
 * @note Use hwSaveDocumentAs for document created using hwNewDocument
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwSaveDocument(HWDOCUMENT hDocument) ;

/**
 * @brief Save Document as under the specified file name.
 * @ingroup document
 *
 * @note If the file exists, the user is prompted to overwrite.
 *
 * @param hDocument  [IN] Hex Workshop document handle
 * @param szFileName [IN] Full path to destination file
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwSaveDocumentAs(HWDOCUMENT hDocument, 
                                    LPCTSTR    szFileName) ;

/**
 * @brief Close the specified document 
 * @ingroup document
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwCloseDocument(HWDOCUMENT hDocument) ;

/** @} */
/** @name Property APIs */

/**
 * @brief Get document size
 * @ingroup document
 *
 * @param hDocument   [IN]  Hex Workshop document handle
 * @param pqwFileSize [OUT] Size of the document in bytes
 * 
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwGetDocumentSize(HWDOCUMENT hDocument, 
                                     QWORD*     pqwFileSize) ;


/**
 * @brief Get the document filename
 * @ingroup document
 *
 * @param hDocument     [IN]  Hex Workshop document handle
 * @param lpstrFileName [OUT] Buffer to place filename
 * @param nFileName     [IN]  Size of the buffer in TCHARs
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwGetFileName(HWDOCUMENT hDocument, 
                                 LPTSTR     lpstrFileName, 
                                 size_t     nFileName) ;

/**
 * @brief Get the document read-only status
 * @ingroup document
 *
 * @param hDocument     [IN]  Hex Workshop document handle
 * @param pbReadOnly    [OUT] TRUE if readonly otherwise FALSE
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwGetReadOnly(HWDOCUMENT hDocument, 
                                 BOOL*      pbReadOnly) ;

/** @} */
/** @name Data Manipulation APIs */

/**
 * @brief Read a block a data at the specified offset
 * @ingroup document
 *
 * @param hDocument [IN]  Hex Workshop document handle
 * @param qwOffset  [IN]  Starting offset location
 * @param vpBuffer  [OUT] Data Buffer
 * @param qwLength  [IN]  Length of data buffer in bytes
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwReadAt(HWDOCUMENT hDocument, 
                            QWORD      qwOffset, 
                            void*      vpBuffer, 
                            QWORD      qwLength) ;

/**
 * @brief Write a block of data starting at the specified offset
 * @ingroup document
 *
 * @note This operation will overwrite data in the document
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param qwOffset  [IN] Starting offset location
 * @param vpBuffer  [IN] Data Buffer
 * @param qwLength  [IN] Length of data buffer in bytes
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwWriteAt(HWDOCUMENT hDocument, 
                             QWORD      qwOffset, 
                             void*      vpBuffer, 
                             QWORD      qwLength) ;



/**
 * @brief Replace data at the specified offset
 * @ingroup document
 *
 * @param hDocument   [IN] Hex Workshop document handle
 * @param qwOffset    [IN] Starting offset location
 * @param vpBuffer    [IN] Data Buffer
 * @param qwSrcLength [IN] Length of the data to replace in bytes (original data)
 * @param qwTrgLength [IN] Length of vpBuffer in bytes (target or new data)
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwReplaceAt(HWDOCUMENT hDocument, 
                               QWORD      qwOffset, 
                               void*      vpBuffer, 
                               QWORD      qwSrcLength, 
                               QWORD      qwTrgLength) ;

/**
 * @brief Insert data at the specified offset
 * @ingroup document
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param qwOffset  [IN] Starting offset location
 * @param vpBuffer  [IN] Data Buffer
 * @param qwLength  [IN] Length of data buffer in bytes
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwInsertAt(HWDOCUMENT hDocument, 
                              QWORD      qwOffset, 
                              void*      vpBuffer, 
                              QWORD      qwLength) ;

/**
 * @brief Delete data at the specified offset
 * @ingroup document
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param qwOffset  [IN] Starting offset location
 * @param qwLength  [IN] Length of data to delete in bytes
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwDeleteAt(HWDOCUMENT hDocument, 
                              QWORD      qwOffset, 
                              QWORD      qwLength) ;
/** @} */
/** @name Undo Control APIs */

/**
 * @brief Enable the Hex Workshop undo features
 * @ingroup document
 *
 * Enables the Hex Workshop undo feature.  By default, undo is enabled, but
 * can be disabled by calling hwUndoDisable.
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwUndoEnable(HWDOCUMENT hDocument) ;

/**
 * @brief Disabled the Hex Workshop undo feature
 * @ingroup document
 *
 * Disables the Hex Workshop undo feature.  Users may want to disable undo 
 * when implementing import functions that include many operations.
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwUndoDisable(HWDOCUMENT hDocument) ;

/**
 * @brief Begin grouping undo operations
 * @ingroup document
 * 
 * Groups multiple document changes into a single undo operation.  Groups
 * cannot be nested -- calling this API twice, without calling hwUndoEndGroup
 * results in an error.
 * 
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwUndoBeginGroup(HWDOCUMENT hDocument) ;

/**
 * @brief Stop grouping undo operations
 * @ingroup document
 *
 * Stop grouping document changes into a single undo operation and commit the
 * undo group.
 * 
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwUndoEndGroup(HWDOCUMENT hDocument) ;

/** @} */

/**
 * @brief Force the editor view to repaint
 * @ingroup editor
 *
 * Forces Hex Workshop to redraw the editor view with any changes.  Hex 
 * Workshop automatically refreshes the editor view at the end of a plug-in
 * execution.  Redraws can be expensive and care should be used when calling.
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwRefreshView(HWDOCUMENT hDocument) ;


/**
 * @brief Bookmark Data Types
 * @ingroup bookmarks
 *
 * @see HWAPI_BOOKMARK
 * @see hwBookmarksAdd
 * @see hwBookmarksGetAt
 */
typedef enum 
{
    HWAPI_DATATYPE_NONE        = 0,  /**< @brief No Data Type */
    HWAPI_DATATYPE_BYTE        = 1,  /**< @brief 8-bit byte */
    HWAPI_DATATYPE_WORD        = 2,  /**< @brief 16-bit word */
    HWAPI_DATATYPE_DWORD       = 3,  /**< @brief 32-bit dword */
    HWAPI_DATATYPE_QWORD       = 4,  /**< @brief 64-bit qword */
    HWAPI_DATATYPE_FLOAT       = 5,  /**< @brief 32-bit float */
    HWAPI_DATATYPE_DOUBLE      = 6,  /**< @brief 64 bit double */
    HWAPI_DATATYPE_STRING      = 7,  /**< @brief ASCII string */
    HWAPI_DATATYPE_UNICODE_STRING = 8, /**< @brief Unicode (wchar_t) string */
    HWAPI_DATATYPE_MSDOSDate   = 9,  /**< @brief 16-bit MSDOS date format */
    HWAPI_DATATYPE_MSDOSTime   = 10, /**< @brief 16 bit MSDOS time format */
    HWAPI_DATATYPE_FILETIME    = 11, /**< @brief 64-bit FILETIME time/date format */
    HWAPI_DATATYPE_UNIXTIME    = 12, /**< @brief 32-bit time_t format */
    HWAPI_DATATYPE_OLEDATETIME = 13, /**< @brief 64-bit OLEDATETIME format */
    HWAPI_DATATYPE_STRUCTURE   = 15, /**< @brief structure definition */
    HWAPI_DATATYPE_BLOB        = 19, /**< @brief 8-bit blob (unexpanded array) */
    HWAPI_DATATYPE_UNIXTIME64  = 20, /**< @brief 64-bit time64_t format */
    HWAPI_DATATYPE_CHAR        = 22, /**< @brief 8-bit char */
    HWAPI_DATATYPE_WCHAR       = 23, /**< @brief 16-bit wchar_t */
    HWAPI_DATATYPE_HALFFLOAT   = 24  /**< @brief 16-bit half precision float (IEEE 754-2008) */
} HWAPI_DATATYPE ;

/**
 * @brief Bookmark definition
 * @ingroup bookmarks
 *
 * @note the cbSize must be set when getting and setting bookmarks
 * 
 * @see hwBookmarksAdd
 * @see hwBookmarksGetAt
 * @see HWAPI_DATATYPE
 * @see HWAPI_SIGN
 * @see HWAPI_BYTEORDER
 *
 */
typedef struct
{
    WORD            cbSize ;        /**< @brief Size of this structure in bytes */
    QWORD           qwAddress ;     /**< @brief Bookmark Address */
    DWORD           dwArrayCount ;  /**< @brief Number of elements */
    HWAPI_DATATYPE  eType ;         /**< @brief Bookmark Type */
    HWAPI_SIGN      eSign ;         /**< @brief Bookmark Sign */
    HWAPI_BYTEORDER eByteOrder ;    /**< @brief Bookmark Byte Order */
    TCHAR           cDescription[80] ;  /**< @brief Bookmark Description */
    TCHAR           cStructureName[128] ; /**< @brief Bookmark Structure name */
} HWAPI_BOOKMARK ;

/**
 * @brief Bookmark Properties Definition
 * @ingroup bookmarks
 *
 * @note The cbSize must be set when getting and setting properties
 * @note If a file name is specified for cStructureLib instead of a full path,
 *       Hex Workshop will attempt to load the structure library from the 
 *       default structure library directory.
 *
 * @see hwBookmarkCollectionGetProps
 * @see hwBookmarkCollectionSetProps
 */
typedef struct
{
    WORD  cbSize ;              /**< @brief Size of this structure in bytes */
    TCHAR cDescription[80] ;    /**< @brief Collection Description */
    TCHAR cAuthor[80] ;         /**< @brief Collection Author */
    TCHAR cStructureLib[256] ;  /**< @brief Collection's assoicated structure library */
} HWAPI_BOOKMARK_COLLECTION_PROPS ;

/**
 * @brief Add a Bookmark
 * @ingroup bookmarks
 *
 * Adds a bookmark to a Hex Workshop Document.  
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param pBookmark [IN] Pointer to a HWAPI_BOOKMARK containing the bookmark
 *        definition
 *
 * @note The cbSize method must be initialized in the HWAPI_BOOKMARK 
 *       structure.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarksAdd(HWDOCUMENT      hDocument,
                                  HWAPI_BOOKMARK* pBookmark) ;

/**
 * @brief Get number of bookmarks
 * @ingroup bookmarks
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param pdwCount  [OUT] Buffer to place bookmark count
 *
 * @note Bookmark arrays are expanded into multiple bookmarks entries.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarksGetCount(HWDOCUMENT hDocument,
                                       DWORD*     pdwCount) ;

/**
 * @brief Get bookmark at specified index
 * @ingroup bookmarks
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param dwIndex   [IN] Bookmark index
 * @param pBookmark [OUT] Pointer to a HWAPI_BOOKMARK to be filled with
 *        bookmark data.
 *
 * @note The cbSize method must be initialized in the HWAPI_BOOKMARK 
 *       structure.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarksGetAt(HWDOCUMENT      hDocument,
                                    DWORD           dwIndex,
                                    HWAPI_BOOKMARK* pBookmark) ;

/**
 * @brief Remove bookmark at specified index
 * @ingroup bookmarks
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param dwIndex   [IN] Bookmark index to remove
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarksRemoveAt(HWDOCUMENT hDocument,
                                       DWORD      dwIndex) ;

/**
 * @brief Clear all bookmark entries
 * @ingroup bookmarks
 *
 * Clear all bookmark entries for the specified document.  Bookmark Properties
 * are not modified.
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarksClear(HWDOCUMENT hDocument) ;

/**
 * @brief Load a bookmark collection
 * @ingroup bookmarks
 *
 * Loads a pre-existing bookmark collection from the file system.  If a file 
 * name is provided (not a full path), Hex Workshop will attempt to load the
 * bookmarks from the default Bookmark directory.
 *
 * @param hDocument      [IN] Hex Workshop document handle
 * @param szBookmarkFile [IN] Bookmark file to load
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarkCollectionLoad(HWDOCUMENT hDocument,
                                            TCHAR*     szBookmarkFile) ;

/**
 * @brief Save a bookmark collection
 * @ingroup bookmarks
 *
 * Save the bookmarks to disk.  This operation will fail if the bookmark 
 * collection has never been saved.  For new bookmarks, please use 
 * hwBookmarkCollectionSaveAs.
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarkCollectionSave(HWDOCUMENT hDocument) ;

/**
 * @brief Save bookmark collection as specified file
 * @ingroup bookmarks
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param szBookmarkFile [IN] Bookmark file to save collection as.  If a file
 * name is provided (not a full path), Hex Workshop will attempt to save the
 * bookmarks in the default Bookmark directory.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarkCollectionSaveAs(HWDOCUMENT hDocument,
                                              TCHAR*     szBookmarkFile) ;

/**
 * @brief Get bookmark collection properties
 * @ingroup bookmarks
 *
 * Get the Bookmark collection properties (description, author, and associated
 * structure library).
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param pProps    [IN] Bookmark collection properties
 *
 * @note The cbSize value must be initialized in the 
 *       HWAPI_BOOKMARK_COLLECTION_PROPS
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarkCollectionGetProps(HWDOCUMENT hDocument,
                                                HWAPI_BOOKMARK_COLLECTION_PROPS* pProps) ;

/**
 * @brief Set bookmark collection properties
 * @ingroup bookmarks
 *
 * Set the Bookmark collection properties (description, author, and associated
 * structure library).
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param pProps    [OUT] Bookmark collection properties
 *
 * @note The cbSize value must be initialized in the 
 *       HWAPI_BOOKMARK_COLLECTION_PROPS
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwBookmarkCollectionSetProps(HWDOCUMENT hDocument,
                                                HWAPI_BOOKMARK_COLLECTION_PROPS* pProps) ;
                                              

/**
 * @brief Load (or reload) the designated structure library.
 * @ingroup bookmarks
 *
 * Load the designated structure library.  If the structure library is already
 * loaded, it will be reloaded (removing any structures added from this 
 * library).  The library is automatically set active.
 *
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param lpstrFileName [IN] Filename or path to structure library.  If only 
 *        a file name is provided (not a full path), Hex Workshop will 
 *        expand the path to match the default Structure directory.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwStructureLibraryLoad(HWSESSION hSession,
                                          LPCTSTR   lpstrFileName) ;

/**
 * @brief Close/Unload the specified structure library.
 * @ingroup structures
 * 
 * Close/Unload the specified library and remove all structures added from
 * this library.
 *
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param lpstrFileName [IN] Filename or path to structure library.  If only 
 *        a file name is provided (not a full path), Hex Workshop will 
 *        expand the path to match the default Structure directory.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureLibraryClose(HWSESSION hSession,
                                           LPCTSTR   lpstrFileName) ;

/**
 * @brief Set the active structure library
 * @ingroup structures
 *
 * Sets the active structure library.  The hwStructureAddFloating,
 * hwStructureAddLocked, and hwStructureExecuteFunction APIs use the active
 * structure library (what is displayed in the Structure Viewer's dropdown
 * box) to lookup structure and function names.  Other libraries are not 
 * search.
 *
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param lpstrFileName [IN] Filename or path to structure library.  If only 
 *        a file name is provided (not a full path), Hex Workshop will 
 *        expand the path to match the default Structure directory. 
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureLibrarySetActive(HWSESSION hSession,
                                               LPCTSTR   lpstrFileName) ;

/**
 * @brief Gets the active structure library.
 * @ingroup structures
 *
 * Gets the active structure library and returns the full file path in 
 * lpstrFileName.
 *
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param lpstrFileName [OUT] Buffer to place the active structure library. 
 *        The full path to the structure library is always returned.
 * @param nFileName [IN] The size of the lpstrFileName buffer in TCHARs.  IF
 *        the supplied buffer is too small, the API will return
 *        HWAPI_RESULT_BUFFER_TOO_SMALL.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureLibraryGetActive(HWSESSION hSession,
                                               LPTSTR    lpstrFileName,
                                               size_t    nFileName) ;

/**
 * @brief Removes all structures displayed in the Structure Viewer.
 * @ingroup structures
 *
 * Removes locked and floating structures for all documents.
 *
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureRemoveAll(HWSESSION hSession) ;

/**
 * @brief Removes all structures locked to the designated document.
 * @ingroup structures
 *
 * Removes all structures locked to the designated document.  Float structures
 * and structures locked to other documents are not removed.
 *
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureRemoveAllDocument(HWDOCUMENT hDocument) ; 

/**
 * @brief Add a floating structure
 * @ingroup structures
 *
 * Adds a float structure to the structure viewer control within Hex Workshop.
 * Floating structures are not bound to a document or offset and are 
 * re-evaluated on all document focus and caret position changes.
 *
 * @param hSession [IN] Hex Workshop Plug-in session handle
 * @param lpstrStructureName [IN] The name of the structure that should be 
 *        added.
 * @param byteOrder [IN] The desired byte order that should be used during 
 *        structure parsing/rendering.  If the structure library declares
 *        a byte order, that byte order is used and this parameter is 
 *        ignored.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureAddFloating(HWSESSION       hSession,
                                          LPCTSTR         lpstrStructureName,
                                          HWAPI_BYTEORDER byteOrder) ;


/**
 * @brief Adds a locked structure
 * @ingroup structures
 *
 * Adds a structure to a document at a specific offset.  The structure is 
 * displayed in the structure viewer control within Hex Workshop.
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param lpstrStructureName [IN] The name of the structure that should be 
 *        added.
 * @param qwOffset [IN] The offset to place the locked structure.
 * @param byteOrder [IN] The desired byte order that should be used during 
 *        structure parsing/rendering.  If the structure library declares
 *        a byte order, that byte order is used and this parameter is 
 *        ignored.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureAddLocked(HWDOCUMENT      hDocument,
                                        LPCTSTR         lpstrStructureName,
                                        QWORD           qwOffset,
                                        HWAPI_BYTEORDER byteOrder) ;

/**
 * @brief Execute a function within a structure library.
 * @ingroup structures
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param lpstrFunctionName [IN] The name of the function that should be 
 *        executed.
 * @param byteOrder [IN] The desired byte order that should be used during 
 *        function parsing/evaluation.  If the structure library declares
 *        a byte order, that byte order is used and this parameter is 
 *        ignored.
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes 
 */
HWAPI HWAPI_RESULT hwStructureExecuteFunction(HWDOCUMENT      hDocument,
                                              LPCTSTR         lpstrFunctionName,                                             
                                              HWAPI_BYTEORDER byteOrder) ;

/** 
 * @brief Hex Workshop Checksum Algorithm types
 * @ingroup checksums
 *
 * @note These algorithms are defined as bitmasks; however, plug-ins may only
 *       define a single algorithm at a time.
 */
typedef enum 
{
    /** 1 byte: Simple count where all the bytes are added in an 8 bit accumulator.  Initial value is 0. */
    HWCSA_CHECKSUM8        = 0x00000001,   
    /** 2 bytes: Simple count where all the bytes are added in an 16 bit accumulator.  Initial value is 0. */
    HWCSA_CHECKSUM16    = 0x00000002,
    /** 2 bytes: 16 bit Cyclic Redundancy Check (CRC) with a polynomial of 0x8005 and an initial value of 0x0000. */
    HWCSA_CRC16            = 0x00000004,
    /** 2 bytes: 16 bit Cyclic Redundancy Check (CRC) with a polynomial of 0x1021 and an initial value of 0xFFFF. */
    HWCSA_CRC16CCITT    = 0x00000008,
    /** 4 bytes: 32 bit Cyclic Redundancy Check (CRC) with a polynomial of 0x04C11DB7 and an initial value of 0xFFFFFFFF. */
    HWCSA_CRC32            = 0x00000010,
    /** 16 bytes: MD2 Message-Digest Algorithm (RSA Data Security, Inc.) */
    HWCSA_MD2            = 0x00000020,
    /** 16 bytes: MD4 Message-Digest Algorithm (RSA Data Security, Inc.) */
    HWCSA_MD4            = 0x00000040,
    /** 16 bytes: MD5 Message-Digest Algorithm (RSA Data Security, Inc.) */
    HWCSA_MD5            = 0x00000080,
    /** 20 bytes: 160 bit Secure Hash Algorithm (NIST) */
    HWCSA_SHA1            = 0x00000100,
    /** 4 bytes: Simple count where all the bytes are added in an 32 bit accumulator.  Initial value is 0. */
    HWCSA_CHECKSUM32    = 0x00000200,
    /** 8 bytes: Simple count where all the bytes are added in an 64 bit accumulator.  Initial value is 0. */
    HWCSA_CHECKSUM64    = 0x00000400,
    /** 2 bytes: 16 bit Cyclic Redundancy Check (CRC) with a user supplied polynomial and initial value. */
    HWCSA_CUSTOM_CRC16  = 0x00000800,
    /** 28 bytes: 224 bit Secure Hash Algorithm (NIST) */
    HWCSA_SHA224        = 0x00001000,
    /** 32 bytes: 256 bit Secure Hash Algorithm (NIST) */
    HWCSA_SHA256        = 0x00002000,
    /** 48 bytes: 384 bit Secure Hash Algorithm (NIST) */
    HWCSA_SHA384        = 0x00004000,
    /** 64 bytes: 512 bit Secure Hash Algorithm (NIST) */
    HWCSA_SHA512        = 0x00008000,
    /** 16 bytes: 128 bit RACE Integrity Primitives Evaluation Message Digest (RIPEMD) */
    HWCSA_RIPEMD128     = 0x00010000,
    /** 20 bytes: 160 bit RACE Integrity Primitives Evaluation Message Digest (RIPEMD) */
    HWCSA_RIPEMD160     = 0x00020000,
    /** 32 bytes: 256 bit RACE Integrity Primitives Evaluation Message Digest (RIPEMD) */
    HWCSA_RIPEMD256     = 0x00040000,
    /** 40 bytes: 320 bit RACE Integrity Primitives Evaluation Message Digest (RIPEMD) */
    HWCSA_RIPEMD320     = 0x00080000,
    /** 24 bytes: 192 bit Tiger Digest Algorithm */
    HWCSA_TIGER         = 0x00100000,
    /** 64 bytes: 512 bit Whirlpool Hash Algorithm */
    HWCSA_WHIRLPOOL     = 0x00200000,
    /** 4 bytes: 32 bit Cyclic Redundancy Check (CRC) with a user supplied polynomial and initial value. */
    HWCSA_CUSTOM_CRC32  = 0x00400000,
} HW_CHECKSUM_ALGORITHM ;

/** @brief Custom CRC-16 parameters
 * @ingroup checksums
 *
 * @note Make sure to set the cbSize when supplying this structure
 */
typedef struct
{
    DWORD cbSize ;         /**< @brief size of structure in bytes */
    WORD  wPolynomial ;    /**< @brief Polynomial value */
    WORD  wInitialValue ;  /**< @brief Starting value */
    BOOL  bReflectIn ;     /**< @brief Enable reflection of input bytes */
    BOOL  bReflectOut ;    /**< @brief Enable reflect of output */
    WORD  wXOROut ;        /**< @brief XOR with results */
} HW_CUSTOMCRC16INFO ;

/** @brief Custom CRC-32 parameters
 * @ingroup checksums
 *
 * @note Make sure to set the cbSize when supplying this structure
 */
typedef struct
{
    DWORD cbSize ;          /**< @brief size of structure in bytes */
    DWORD dwPolynomial ;    /**< @brief Polynomial value */
    DWORD dwInitialValue ;  /**< @brief Starting value */
    BOOL  bReflectIn ;      /**< @brief Enable reflection of input bytes */
    BOOL  bReflectOut ;     /**< @brief Enable reflect of output */
    DWORD dwXOROut ;        /**< @brief XOR with results */
} HW_CUSTOMCRC32INFO ;


/** @brief Query result buffer size required for a check algorithm
 * @ingroup checksums
 *
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param algorithm [IN] Algorithm type as defined by HW_CHECKSUM_ALGORITHM
 * @param pnLength  [OUT] Checksum result size in bytes
 *
 * @see HW_CHECKSUM_ALGORITHM, hwChecksumDocument, hwChecksumBuffer
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwChecksumLength(HWSESSION             hSession,
                                    HW_CHECKSUM_ALGORITHM algorithm,
                                    size_t*               pnLength) ;

/** @brief Calculate a checksum for a document
 * @ingroup checksums
 *
 * @param hDocument [IN]  Hex Workshop document handle
 * @param algorithm [IN]  Algorithm type as defined by HW_CHECKSUM_ALGORITHM
 * @param vpAlgInfo [IN]  Optional pointer to a HW_CUSTOMCRC16INFO or 
 *                        HW_CUSTOMCRC32INFO structuer for HWCSA_CUSTOM_CRC16 
 *                        and HWCSA_CUSTOM_CRC32 algorithms.  If NULL, Hex
 *                        Workshop will use the user defaults.  Must be NULL
 *                        for other algorithms.
 * @param qwOffset  [IN]  Starting offset location
 * @param qwLength  [IN]  Length (in bytes) to checksum, starting from qwOffset
 * @param vpResults [OUT] Buffer to place results
 * @param nResults  [IN]  Size of results buffer
 *
 * @see HW_CHECKSUM_ALGORITHM, hwChecksumLength
 * @see HW_CUSTOMCRC16INFO, HW_CUSTOMCRC32INFO
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwChecksumDocument(HWDOCUMENT            hDocument, 
                                      HW_CHECKSUM_ALGORITHM algorithm,
                                      const void*           vpAlgInfo,
                                      QWORD                 qwOffset,
                                      QWORD                 qwLength,
                                      void*                 vpResults,
                                      size_t                nResults) ;

/** @brief Calculate a checksum for a buffer
 * @ingroup checksums
 *
 * @param hSession  [IN]  Hex Workshop Plug-in session handle
 * @param algorithm [IN]  Algorithm type as defined by HW_CHECKSUM_ALGORITHM
 * @param vpAlgInfo [IN]  Optional pointer to a HW_CUSTOMCRC16INFO or 
 *                        HW_CUSTOMCRC32INFO structuer for HWCSA_CUSTOM_CRC16 
 *                        and HWCSA_CUSTOM_CRC32 algorithms.  If NULL, Hex
 *                        Workshop will use the user defaults.  Must be NULL
 *                        for other algorithms.
 * @param vBuffer   [IN]  Buffer to calculate checksum on
 * @param nBuffer   [IN]  Size of buffer to calculate checksum on
 * @param vpResults [OUT] Buffer to place results
 * @param nResults  [IN]  Size of results buffer
 *
 * @see HW_CHECKSUM_ALGORITHM, hwChecksumLength
 * @see HW_CUSTOMCRC16INFO, HW_CUSTOMCRC32INFO
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwChecksumBuffer(HWSESSION             hSession,
                                    HW_CHECKSUM_ALGORITHM algorithm,
                                    const void*           vpAlgInfo,
                                    const void*           vBuffer,
                                    QWORD                 nBuffer,
                                    void*                 vpResults,
                                    size_t                nResults) ;
/**
 * @brief Get the editor caret position
 * @ingroup editor
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param pqwOffset  [OUT] Offset address of the editing cursor
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwGetCaretPosition(HWDOCUMENT hDocument, 
                                      QWORD*     pqwOffset) ;

/**
 * @brief Set the editor caret position
 * @ingroup editor
 *
 * @note Selection is cleared on caret position change
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param qwOffset  [IN] Offset address to place editing cursor
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwSetCaretPosition(HWDOCUMENT hDocument, 
                                      QWORD      qwOffset) ;

/**
 * @brief Get selection length of the hex editor window
 * @ingroup editor
 *
 * The selection starts at the curret caret position
 * @see hwGetCaretPosition to receive the caret position
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param pqwLength  [OUT] Length of selection in bytes
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwGetSelection(HWDOCUMENT hDocument, 
                                  QWORD*     pqwLength) ;

/**
 * @brief Select data within the hex editor window
 * @ingroup editor
 *
 * The selection starts at the curret caret position
 * @see hwSetCaretPosition to adjust the starting offset
 *
 * @param hDocument [IN] Hex Workshop document handle
 * @param qwLength  [IN] Length of selection in bytes
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwSetSelection(HWDOCUMENT hDocument, 
                                  QWORD      qwLength) ;


/**
 * @brief Get the window handle of the Hex Workshop frame window
 * @ingroup editor
 *
 * @param hSession [IN] Hex Workshop Plug-in session handle
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWND hwGetWindowHandle(HWSESSION hSession) ;


/**
 * @brief Log level definitions
 * @ingroup logging
 *
 * Log Levels definitions used by hwOutputLog.
 *
 * @see hwOutputLog
 */
typedef enum 
{
    HWLOG_DEBUG = 0,  /**< Debug Log Level; only useful to plug-in author */
    HWLOG_INFO  = 1,  /**< Info Log Level; information useful but not critical for end users */
    HWLOG_WARN  = 2,  /**< Warning Log Level; warning for end user */
    HWLOG_ERR   = 3,  /**< Error Log Level; An error condition has occurred */
} HWAPI_LOG_LEVEL ;


/**
 * @brief Adds a log message to the Hex Workshop Output Window
 * @ingroup logging
 *
 * @param hSession [IN] Hex Workshop Plug-in session handle
 * @param level    [IN] Log Level as defined by HWAPI_LOG_LEVEL
 * @param message  [IN] Output text (CrLf not required)
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwOutputLog(HWSESSION       hSession,
                               HWAPI_LOG_LEVEL level,
                               LPCTSTR         message,
                               ...) ;

/**
 * @brief Updates the progress indicator in the Plug-in execute status dialog.
 * @ingroup editor
 *
 * The plug-in may call this API to update the status of the plug-in 
 * execution.  HWAPI_RESULT_USER_ABORT is returned if the user has cancelled
 * the operations.
 *
 * @note Call hwUpdateProgess(hSession, -1, NULL) to query for user cancel
 *       status without updating the progress indicator.
 *
 * @param hSession [IN] Hex Workshop Plug-in session handle
 * @param percentComplete [IN] Percent complete (from 0 to 100)
 * @param status [IN] Short string to be placed in status dialog
 *
 * @return HWAPI_RESULT_SUCCESS on success, otherwise see hwapierr.h for error 
 *         codes
 */
HWAPI HWAPI_RESULT hwUpdateProgress(HWSESSION hSession,
                                    int       percentComplete,
                                    LPCTSTR   status) ;

/** @brief The plug-in requires an active file to operate
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * is only enabled when file is opened and active within the editor.
 *
 * @see HWPLUGIN_RequestCapabilities
 */ 
#define HWPLUGIN_CAP_FILE_REQUIRE           0x00000000

/** @brief The plug-in can operate with or without an active file
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * is always enabled.
 *
 * @see HWPLUGIN_RequestCapabilities
 */ 
#define HWPLUGIN_CAP_FILE_OPTIONAL          0x00000001

/** @brief The plug-in operates without an active file
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * is disabled if a file is opened and active within the editor.
 *
 * @see HWPLUGIN_RequestCapabilities
 */ 
#define HWPLUGIN_CAP_FILE_DISALLOW          0x00000002

/** @brief The plug-in should be automatically executed based on file extension
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * Hex Workshop calls the HWPLUGIN_RequestFileAutoexecExtensions entrypoint and
 * will automatically run the plug-in when a file with that extension is 
 * opened.
 *
 * @see HWPLUGIN_RequestCapabilities
 * @see HWPLUGIN_RequestFileAutoexecExtensions
 */ 
#define HWPLUGIN_CAP_FILE_AUTOEXEC          0x00000008

/** @brief The plug-in requires an active selection within the editor
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * Hex Workshop disables the plug-in unless data is selected within the 
 * editor.
 *
 * @see HWPLUGIN_RequestCapabilities
 */ 
#define HWPLUGIN_CAP_SELECTION_REQUIRE      0x00000010

/** @brief The plug-in can operate with or without an active selection
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * is enabled regardless of editor selection state.
 *
 * @see HWPLUGIN_RequestCapabilities
 */ 
#define HWPLUGIN_CAP_SELECTION_OPTIONAL     0x00000000

/** @brief The plug-in cannot operate with an active selection
 * @ingroup entrypoints
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the plug-in command
 * is disabled if data is selected within the editor.
 *
 * @see HWPLUGIN_RequestCapabilities
 */ 
#define HWPLUGIN_CAP_SELECTION_DISALLOW     0x00000020

/** @brief The plug-in does not support cancel
 * @ingroup entrypoints
 *
 * @note Plug-ins are assumed (and should) support cancel.  hwUpdateProgress
 *       should be used to update progress and determine if the user requested
 *       a cancel.
 *
 * When specified as part of HWPLUGIN_RequestCapabilities, the cancel button is
 * disabled in the plug-in status dialog.
 *
 * @see HWPLUGIN_RequestCapabilities
 */
#define HWPLUGIN_CAP_NO_CANCEL              0x10000000

/** @name Required Entrypoints */


/**
 * @brief Called by Hex Workshop to identify a plug-in
 * @ingroup entrypoints
 *
 * Plug-in authors are required to implement the HWPLUGIN_Identify 
 * entrypoint in their plug-in DLL and must return the plug-ins command(s)
 * as strings.  The command strings are displayed under Hex Workshop's
 * Plug-ins menu and provided to the HWPLUGIN_RequestCapabilities and
 * HWPLUGIN_Execute entrypoints.
 * 
 * Authors can define multiple plug-in commands or operations within a single
 * DLL.  To specify multiple commands, delimit each command with a 
 * semicolon(;).
 *
 * Authors may also include a backslash (\) to create a sub-menu item for 
 * plug-ins within the Hex Workshop's plug-in menu.  Only a single backslash
 * is supported.  The full command name (including the slash) is used when
 * calling the other plug-in entrypoints.
 * 
 * For example:
 * <pre>
 * HWAPIEP BOOL HWPLUGIN_Identify(LPTSTR lpstrPluginCommands, 
 *                                size_t nMaxPluginCommand) ;
 *
 *     _tcsncpy(lpstrPluginCommands, 
 *             _T("Plugin by Example, Inc."), 
 *             nMaxPluginCommand) ;
 * or
 *     _tcsncpy(lpstrPluginCommands, 
 *             _T("Operation 1 by Example Inc.;Operation 2 by Example Inc.", 
 *             nMaxPluginCommand) ;
 * or
 *     _tcsncpy(lpstrPluginCommands, 
 *             _T("Example Inc.\Operation 1;Example Inc.\Operation 2", 
 *             nMaxPluginCommand) ;
 * </pre>
 *
 * @note Each command string is limited to 127 characters and will be 
 *       truncated to 127 characters.
 *
 * @see HWPLUGIN_RequestCapabilities
 * @see HWPLUGIN_Execute
 *
 * @param lpstrPluginCommands [OUT] A buffer to place the plug-ins command
 *        list.
 * @param nMaxPluginCommand   [IN] The buffer length of lpstrPluginCommand in
 *        TCHARs.  Please use _tcsncpy as the buffer size may vary.
 *
 * @return TRUE on success or FALSE to disable the plugin
 */
HWAPIEP BOOL HWPLUGIN_Identify(LPTSTR lpstrPluginCommands, 
                                size_t nMaxPluginCommand) ;


/**
 * @brief Called by Hex Workshop to determine capabilities for a plugin
 * @ingroup entrypoints
 *
 * Plug-in authors are required to implement the HWPLUGIN_RequestCapabilities
 * entrypoint in their plug-in DLL and must return one or more of the
 * HWPLUGIN_CAPABILITY_ flags.
 *
 * HWPLUGIN_RequestCapabilities is invoked for each of the command strings
 * specified by HWPLUGIN_Identify.
 *
 * The flags are used to determine when a Plug-in command should be enabled.
 *
 * For example:
 * <pre>
 *     return HWPLUGIN_CAP_FILE_OPTIONAL | 
 *             HWPLUGIN_CAP_SELECTION_OPTIONAL ;
 * </pre>
 *
 * @param lpstrPluginCommand [IN] The command string provided by
 *        HWPLUGIN_Identify
 *
 * @see HWPLUGIN_RequestCapabilities
 * @see HWPLUGIN_Execute
 * @see HWPLUGIN_RequestFileAutoexecExtensions
 * @see HWPLUGIN_CAP_FILE_REQUIRE
 * @see HWPLUGIN_CAP_FILE_OPTIONAL
 * @see HWPLUGIN_CAP_FILE_DISALLOW
 * @see HWPLUGIN_CAP_FILE_AUTOEXEC
 * @see HWPLUGIN_CAP_SELECTION_REQUIRE
 * @see HWPLUGIN_CAP_SELECTION_OPTIONAL
 * @see HWPLUGIN_CAP_SELECTION_DISALLOW
 * @see HWPLUGIN_CAP_NO_CANCEL
 *
 * @return One or more of the HWPLUGIN_CAP_ flags.
 */
HWAPIEP DWORD HWPLUGIN_RequestCapabilities(LPCTSTR lpstrPluginCommand) ;

/**
 * @brief Called by Hex Workshop to execute a plug-in command
 * @ingroup entrypoints
 *
 * @note The hDocument handle may be NULL if HWPLUGIN_CAPABILITY_NO_DOCUMENT
 *       is specified by HWPLUGIN_RequestCapabilities
 *
 * @param lpstrPluginCommand [IN] The command string supplied as part of
 *        HWPLUGIN_Identify
 * @param hSession  [IN] Hex Workshop Plug-in session handle
 * @param hDocument [IN] Hex Workshop document handle
 *
 * @see HWPLUGIN_Identify
 * @see HWPLUGIN_RequestCapabilities
 * @see HWPLUGIN_RequestFileAutoexecExtensions
 *
 * @return TRUE if the operation was successful, otherwise false.
 */
HWAPIEP BOOL  HWPLUGIN_Execute( LPCTSTR    lpstrPluginCommand,
                                HWSESSION  hSession,
                                HWDOCUMENT hDocument ) ;

/** @} */
/** @name Optional Entrypoints */

/**
 * @brief Called by Hex Workshop to execute a plug-in command
 * @ingroup entrypoints
 *
 * Extensions must be delimited by a ; and are not case sensitive.  Wildcards
 * are not supported.
 *
 * For example:
 * <pre>
 * HWAPIEP BOOL  HWPLUGIN_RequestFileAutoexecExtensions(LPCTSTR    lpstrPluginCommand,
 *                                                      LPTSTR     lpstrExtensions,
 *                                                      size_t     nExtensions) 
 * {
 *     _tcsncpy(lpstrExtensions, _T("mpeg;mpg;mp3"), nExtensions) ;
 * }
 * </pre>
 * 
 * @note Only called if HWPLUGIN_CAP_FILE_AUTOEXEC is return by 
 *       HWPLUGIN_RequestCapabilities
 *
 * @param lpstrPluginCommand [IN] The command string supplied as part of
 *        HWPLUGIN_Identify
 * @param lpstrExtensions    [OUT] List of file extensions separated by ;s
 * @param nExtensions        [OUT] Size of lpstrExtension buffer in TCHARs
 *
 * @see HWPLUGIN_Identify
 * @see HWPLUGIN_RequestCapabilities
 * @see HWPLUGIN_Execute
 *
 * @return TRUE if the operation was successful, otherwise false.
 */
HWAPIEP BOOL  HWPLUGIN_RequestFileAutoexecExtensions(LPCTSTR    lpstrPluginCommand,
                                                     LPTSTR     lpstrExtensions,
                                                     size_t     nExtensions) ;
/** @} */

#endif /* ] */
