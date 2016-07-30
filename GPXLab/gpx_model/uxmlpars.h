/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

/**
 * @file uxmlpars.h
 *
 * @brief XML parser (SAX)
 *
 * Generic XML parser.
 *
 * The user must specify callback functions and a buffer (pContent) which
 * will contain the tag content and the size of the buffer (maxContent).
 *
 * Callback functions:
 *   - \b UXML_GetChar       Called to get the next character from a stream (file or buffer)
 *   - \b UXML_OpenTag       Called when a new tag was found (<...>)
 *   - \b UXML_CloseTAG      Called when a tag was closed (</...>)
 *   - \b UXML_SetContent    Called when a tag content was found
 *   - \b UXML_setAttribute  Called when a tag attribute was found
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 30 Jul 2016
 */

#ifndef _UXMLPARS_H_
#define _UXMLPARS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** Maximal length of a tag */
#define UXML_TAG_SIZE   256

/** Function pointer for UXML_GetChar */
typedef int  (*UXML_GetChar)(void* pXml);

/** Function pointer for UXML_OpenTag */
typedef void (*UXML_OpenTag)(void* pXml, char* pTagName);

/** Function pointer for UXML_CloseTAG */
typedef void (*UXML_CloseTAG)(void* pXml, char* pTagName);

/** Function pointer for UXML_SetContent */
typedef void (*UXML_SetContent)(void* pXml, char* pTagName, char* pTagContent);

/** Function pointer for UXML_setAttribute */
typedef void (*UXML_setAttribute)(void* pXml, char* pTagName, char* pAttribute, char *value);

/** Parsing structure. Set here the callback functions and the tag content buffer */
typedef struct t_uXml
{
    void*               fp;                         /**< Input stream handler (file or buffer) */
    int                 state;                      /**< User defined state for a state machine */
    int                 eof;                        /**< end of file flag (0/1) or negative error code */
    int                 encoding;                   /**< File encoding */
    int                 unget_c;                    /**< "Ungeted" character (intern use) */
    int                 recursionDepth;             /**< Recursion depth */
    int                 maxContent;                 /**< Maximal size of buffer *pContent */
    char                pTag[UXML_TAG_SIZE];        /**< Begin and end tag name in lower case */
    char                pAttribute[UXML_TAG_SIZE];  /**< Attribute name in lower case */
    char*               pContent;                   /**< Tag content buffer */
    void*               pObject;                    /**< User defined object */
    UXML_GetChar        getChar;                    /**< Called to get the next character from a stream (file or buffer) */
    UXML_OpenTag        openTag;                    /**< Called when a new tag was found (<...>) */
    UXML_CloseTAG       closeTag;                   /**< Called when a tag was closed (</...>) */
    UXML_SetContent     setContent;                 /**< Called when a tag content was found */
    UXML_setAttribute   setAttribute;               /**< Called when a tag attribute was found */
} T_uXml;

/**
 * @brief Initializes T_uXml structure
 * @param pXML Pointer to a T_uXml structure
 */
void UXML_init(T_uXml* pXML);

/**
 * @brief Parses XML file
 * @param pXML Pointer to a T_uXml structure
 * @return 0 on success
 */
int UXML_parseFile(T_uXml* pXML);

#ifdef __cplusplus
}
#endif

#endif //_UXMLPARS_H_
