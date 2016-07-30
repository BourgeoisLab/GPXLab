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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "uxmlpars.h"

#define ENCODE_ANSI                     0
#define ENCODE_UTF8                     1
#define ENCODE_UTF16LE                  2
#define ENCODE_UTF16BE                  3
#define TOWCHARLE(a,b)                  ((b<<8) | (a&0xFF))
#define TOWCHARBE(a,b)                  ((a<<8) | (b&0xFF))

enum e_item
{
    ERROR,
    INVALID,
    OPEN_TAG,
    CLOSE_TAG,
    TAG_CONTENT,
    TAG_CONTENT_CDATA,
    OPEN_TAG_W_ATTRIBUTES
};

/**
 * Read Unicode character from UTF-8 input.
 */
static int UXML_readUTF8(T_uXml* pXML, long a)
{
    if ((a & 0x80) == 0)   // bit 8 is 0
    {
        return a & 0x7F;   // one byte (0..127)
    }
    if (a & 0x40)     // bit 7 is 1 too
    {
        long b = pXML->getChar(pXML);
        if (b == -1)
            return -1;   // EOF
        if ((a & 0x20) == 0) // two bytes ((a <= 0xDF) && (a >= 0xC0))
        {
            return (((a & 0x1F) << 6) | (b & 0x3F));
        }
        else
        {
            long c = pXML->getChar(pXML);
            if (c == -1)
                return -1;   // EOF
            if ((a & 0x10) == 0) // three bytes (a >= 0xE0) &&...
            {
                return (((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F));
            }
            else
            {
                long d = pXML->getChar(pXML);
                if (d == -1)
                    return -1;   // EOF
                if ((a & 0x08) == 0) // four bytes
                {
                    return (((a & 0x07) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (d & 0x3F));
                }
                //else 5 or 6 bytes...
            }
        }
    }
    return -1; // invalid, or not yet supported.
}

/**
 * Get next character from the input (buffer, file, socket...)
 * This function reads the BOM, if the encoding flag is not yet initialized,
 * and handles Unicode stuff (utf-8, utf-16-le, utf-16-be).
 * Returns the next Unicode character from input stream
 */
static int UXML_getc(T_uXml* pXML)
{
    int c;

    if (pXML->eof)
        return -1;

    if (pXML->unget_c != -1)
    {
        c = pXML->unget_c;
        pXML->unget_c = -1;
        return c;
    }

    c = pXML->getChar(pXML);

    // Unicode stuff begins here
    if (pXML->encoding == -1) // not yet initialized -> read BOM?
    {
        pXML->encoding = ENCODE_ANSI;

        if (c == 0xFF || c == 0xFE || c == 0xEF)
        {
            // Read BOM
            int c2 = pXML->getChar(pXML);
            if (c == 0xFF && c2 == 0xFE)
                pXML->encoding = ENCODE_UTF16LE;
            else if (c == 0xFE && c2 == 0xFF)
                pXML->encoding = ENCODE_UTF16BE;
            else if (c == 0xEF && c2 == 0xBB)
            {
                if (pXML->getChar(pXML) == 0xBF)
                    pXML->encoding = ENCODE_UTF8;
            }
            else
                pXML->unget_c = c2;
        }

        if (pXML->encoding != ENCODE_ANSI) // if found BOM, read first byte after the BOM (in user data).
            c = pXML->getChar(pXML);
    }

    if (c != EOF)
    {
        // Read more bytes for Unicode...

        if (pXML->encoding == ENCODE_ANSI)
        {
            // using code page
        }
        else if (pXML->encoding == ENCODE_UTF8)
        {
            c = UXML_readUTF8(pXML, c);
        }
        else if (pXML->encoding == ENCODE_UTF16LE)
        {
            int c2 = pXML->getChar(pXML);
            if (c2 == EOF)
                c = EOF;
            else
                c = TOWCHARLE(c, c2);
        }
        else if (pXML->encoding == ENCODE_UTF16BE)
        {
            int c2 = pXML->getChar(pXML);
            if (c2 == EOF)
                c = EOF;
            else
                c = TOWCHARBE(c, c2);
        }
    }

    if (c == EOF)
        pXML->eof = 1;

    return c;
}


/**
 */
static void UXML_ungetc(T_uXml* pXML, int c)
{
    if (pXML->eof)
        return;
    pXML->unget_c = c;
}

/**
 * Reads one character from peripheral.
 * Returns the number of bytes written in the destination buffer.
 */
static int readEncodedChar(T_uXml* pXML, char* buffer, int max)
{
    int nwrite=0;

    int c = UXML_getc(pXML); // get Unicode char (wchar)

    if (c == -1) // EOF
        return 0;

    if (max > 0)
    {
        if (pXML->encoding == ENCODE_ANSI)
        {
            buffer[nwrite++] = (char)(c&0xFF);
        }
        else // c is Unicode wide char. (BOM)
        {
            // we encode as UTF-8 with a max size of three bytes
            if (c < 0x80)
            {
                // one byte
                buffer[nwrite++] = (char)(c&0xFF);    // only low byte
            }
            else if (c < 0x800)
            {
                // two bytes
                if (max > 1)
                {
                    buffer[nwrite++] = (char) (0xC0 + ((c>>6)&0x03F));
                    buffer[nwrite++] = (char) (0x80 + (c&0x003F));
                }
            }
            else
            {
                // three bytes
                if (max > 2)
                {
                    buffer[nwrite++] = (char) (0xE0 + ((c>>12)&0x000F));
                    buffer[nwrite++] = (char) (0x80 + ((c>>6)&0x003F));
                    buffer[nwrite++] = (char) (0x80 + ((c&0x003F)));
                }
            }
        }
    }
    return nwrite;
}


/**
 */
static enum e_item UXML_findItem(T_uXml* pXML)
{
    int c;

    while (!pXML->eof)
    {
        //skip spaces
        do
        {
            c = UXML_getc(pXML);
        } while (isspace(c) && !pXML->eof);
        if (pXML->eof)
            return ERROR;

        // check for self-closing tag <.../>
        if (c == '/')
        {
            c = UXML_getc(pXML);
            if (c == '>')
            {
                return CLOSE_TAG;
            }
            else
            {
                UXML_ungetc(pXML, c);
                return INVALID;
            }
        }

        // check if a content is following
        if (c != '<')
        {
            UXML_ungetc(pXML, c);
            return TAG_CONTENT; // the content is following
        }

        c = UXML_getc(pXML);

        // check for <? ... ?> TAG
        if (c == '?')
        {
            //skip stuff
            while(c != '>' && !pXML->eof)
                c = UXML_getc(pXML);
            if (pXML->eof)
                return ERROR;
        }

        // check for comment or CDATA
        else if (c == '!')
        {
            const char *ptr;
            const char *pCommentEnd = "-->";
            const char *pCDATA = "[CDATA[";

            // check for <![CDATA[content]]>
            ptr = pCDATA;
            while(*ptr && !pXML->eof)
            {
                c = UXML_getc(pXML);
                if (c != *ptr++)
                    break;
            }
            if (*ptr == '\0')
                return TAG_CONTENT_CDATA; // found <![CDATA[content]]>

            //skip comment
            ptr = pCommentEnd;
            while(*ptr && !pXML->eof)
            {
                c = UXML_getc(pXML);
                if (c != *ptr++)
                    ptr = pCommentEnd;
            }
        }

        // new TAG
        else
        {
            int read = 0;
            int m_Counter = 0;

            // end TAG </...>
            if (c == '/')
            {
                // read TAG
                while(pXML->pTag[m_Counter-read] != '>' && m_Counter < UXML_TAG_SIZE && !pXML->eof)
                {
                    read = readEncodedChar(pXML, &pXML->pTag[m_Counter], UXML_TAG_SIZE-m_Counter);
                    m_Counter += read;
                }
                if (read == 0 && !pXML->eof)
                {
                    pXML->eof = 1;
                    return ERROR;
                }

                // string termination
                if (m_Counter > 0 && m_Counter <= UXML_TAG_SIZE)
                    pXML->pTag[m_Counter-1] = '\0';
                return CLOSE_TAG; // found close tag
            }

            // open TAG <...>
            else
            {
                // read TAG
                pXML->pTag[m_Counter++] = tolower(c);
                while(pXML->pTag[m_Counter - read] != '>' &&
                      pXML->pTag[m_Counter - read] != ' ' &&
                      pXML->pTag[m_Counter - read] != '/' &&
                      m_Counter < UXML_TAG_SIZE && !pXML->eof)
                {
                    read = readEncodedChar(pXML, &pXML->pTag[m_Counter], UXML_TAG_SIZE - m_Counter);
                    m_Counter += read;
                }
                if (read == 0 && !pXML->eof)
                {
                    pXML->eof = 1;
                    return ERROR;
                }

                c = pXML->pTag[m_Counter - read];

                // string termination
                if (m_Counter > 0 && m_Counter <= UXML_TAG_SIZE)
                    pXML->pTag[m_Counter - 1] = 0;

                if (c == ' ')
                {
                    //skip spaces
                    do
                    {
                        c = UXML_getc(pXML);
                    } while (isspace(c) && !pXML->eof);
                    if (pXML->eof)
                        return ERROR;
                }

                if (c == '/')
                {
                    UXML_ungetc(pXML, c);
                }
                if (c != '>')
                {
                    UXML_ungetc(pXML, c);
                    return OPEN_TAG_W_ATTRIBUTES;
                }
                return OPEN_TAG;
            }
        }
    }
    return ERROR;
}

static int UXML_scanContent(T_uXml* pXML, char delim, int ungetDelim, int isCDATA)
{
    int read = 0;
    int m_scanContentIndex = 0;

    if (isCDATA)
    {
        // read until "]]>"
        while(!pXML->eof)
        {
            read = readEncodedChar(pXML, &pXML->pContent[m_scanContentIndex], pXML->maxContent-m_scanContentIndex);
            m_scanContentIndex += read;
            if (pXML->pContent[m_scanContentIndex-read-2] == ']' &&
                pXML->pContent[m_scanContentIndex-read-1] == ']' &&
                pXML->pContent[m_scanContentIndex-read-0] == '>')
                break;
        }
        if (m_scanContentIndex > 0 && m_scanContentIndex <= pXML->maxContent)
            pXML->pContent[m_scanContentIndex - 3] = 0; // cut "]]>"

        // read until 'delim'
        while(!pXML->eof)
        {
            if (UXML_getc(pXML) == delim)
                break;
        }
    }
    else
    {
        // read until 'delim'
        while(!pXML->eof)
        {
            read = readEncodedChar(pXML, &pXML->pContent[m_scanContentIndex], pXML->maxContent-m_scanContentIndex);
            m_scanContentIndex += read;
            if (pXML->pContent[m_scanContentIndex-read] == delim)
                break;
        }
        if (m_scanContentIndex > 0 && m_scanContentIndex <= pXML->maxContent)
            pXML->pContent[m_scanContentIndex - 1] = 0; // cut 'delim'
    }

    // unget 'delim'
    if (ungetDelim)
        UXML_ungetc(pXML, delim);

    return 1;
}

static int UXML_scanAttributes(T_uXml* pXML)
{
    int c;
    int read = 0;
    int m_Counter = 0;

    //skip spaces
    do
    {
        c = UXML_getc(pXML);
    } while (isspace(c) && !pXML->eof);
    if (pXML->eof)
        return -1;

    // check if end of the open tag
    if (c == '>')
    {
        return 0;
    }
    else if (c == '<' || c == '/')
    {
        UXML_ungetc(pXML, c);
        return 0;
    }

    // read attribute name
    pXML->pAttribute[m_Counter++] = tolower(c);
    while (pXML->pAttribute[m_Counter - read] != '>' &&
        pXML->pAttribute[m_Counter - read] != '=' &&
        pXML->pAttribute[m_Counter - read] != ' ' &&
        pXML->pAttribute[m_Counter - read] != '/' &&
        m_Counter < UXML_TAG_SIZE && !pXML->eof)
    {
        read = readEncodedChar(pXML, &pXML->pAttribute[m_Counter], UXML_TAG_SIZE - m_Counter);
        m_Counter += read;
    }
    if (read == 0 && !pXML->eof)
    {
        pXML->eof = 1;
        return -1;
    }

    c = pXML->pAttribute[m_Counter - read];

    // string termination
    if (m_Counter > 0 && m_Counter <= UXML_TAG_SIZE)
        pXML->pAttribute[m_Counter - 1] = '\0';

    if (c == ' ')
    {
        //skip spaces
        do
        {
            c = UXML_getc(pXML);
        } while (isspace(c) && !pXML->eof);
        if (pXML->eof)
            return -1;
    }

    if (c == '>' || c == '/')
    {
        *pXML->pContent = '\0';
        UXML_ungetc(pXML, c);
    }
    else if (c == '=')
    {
        // read until '"'
        while (!pXML->eof)
        {
            c = UXML_getc(pXML);
            if (c == '"')
                break;
        }
        UXML_scanContent(pXML, '"', 0, 0);
    }
    return 1;
}

static void UXML_recursiveXmlParser(T_uXml* pXML)
{
    switch (UXML_findItem(pXML))
    {
    case OPEN_TAG:
        pXML->recursionDepth++;
        if (pXML->openTag)
            pXML->openTag(pXML, pXML->pTag);
        UXML_recursiveXmlParser(pXML);
        break;

    case OPEN_TAG_W_ATTRIBUTES:
        pXML->recursionDepth++;
        if (pXML->openTag)
            pXML->openTag(pXML, pXML->pTag);
        while (UXML_scanAttributes(pXML))
        {
            if (pXML->setAttribute)
                pXML->setAttribute(pXML, pXML->pTag, pXML->pAttribute, pXML->pContent);
        }
        break;

    case CLOSE_TAG:
        if (pXML->closeTag)
            pXML->closeTag(pXML, pXML->pTag);
        pXML->recursionDepth--;
        break;

    case TAG_CONTENT:
        UXML_scanContent(pXML, '<', 1, 0);
        pXML->setContent(pXML, pXML->pTag, pXML->pContent);
        UXML_recursiveXmlParser(pXML);
        break;

    case TAG_CONTENT_CDATA:
        UXML_scanContent(pXML, '<', 1, 1);
        pXML->setContent(pXML, pXML->pTag, pXML->pContent);
        UXML_recursiveXmlParser(pXML);
        break;

    case INVALID:
        break;

    case ERROR:
        return;
    }
}

void UXML_init(T_uXml* pXML)
{
    if (pXML)
    {
        memset(pXML, 0, sizeof(T_uXml));
        pXML->unget_c = -1;
        pXML->encoding = -1;
    }
}

int UXML_parseFile(T_uXml* pXML)
{
    if (!pXML->setContent)
        return -1;

    while(!pXML->eof)
        UXML_recursiveXmlParser(pXML);

    if (pXML->eof)
        return 0;
    else
        return -1;
}
