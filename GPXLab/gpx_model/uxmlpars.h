#ifndef UXMLPARS_H
#define UXMLPARS_H

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
 */

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

#endif //UXMLPARS_H
