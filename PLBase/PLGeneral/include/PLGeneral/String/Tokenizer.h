/*********************************************************\
 *  File: Tokenizer.h                                    *
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


#ifndef __PLGENERAL_TOKENIZER_H__
#define __PLGENERAL_TOKENIZER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGeneral/String/String.h"
#include "PLGeneral/Container/Stack.h"
#include "PLGeneral/Container/Array.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGeneral {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class File;
class BufferedReader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tokenizer class for parsing texts (also called 'scanner' or 'lexer' -> lexical analysis)
*
*  @remarks
*    The tokenizer parses an input stream (e.g. a file or string) and produces
*    a subsequent list of tokens by filtering out whitespace, comments and such.
*    There are many settings that can be manipulated to serve your needs, like choosing
*    the type of comment tags or the characters that shall be considered whitespace.
*    In general, the tokenizer should only be used to produce a list of tokens,
*    after that a real parser is used to check the syntax and semantics of the
*    parsed code. In addition to this basic functionality the class also provides
*    some advanced functions to parse typical syntax like equations (a=b) or vectors
*    (a b c). These functions can be used if no strict syntax is needed, e.g. for
*    config files. But they are not intended to replace a decent parser.
*
*  @verbatim
*    Usage example:
*    Tokenizer cTokenizer;                         // Tokenizer instance
*    String sText = "This is a test";              // Text to be parsed
*    cTokenizer.Start(sText);                      // Start tokenizer
*    String sToken;                                // String for current token
*    sToken = cTokenizer.GetNextToken();           // Will return 'This'
*    sToken = cTokenizer.GetNextToken();           // Will return 'is'
*    sToken = cTokenizer.GetNextToken();           // Will return 'a'
*    sToken = cTokenizer.GetNextToken();           // Will return 'test'
*  @endverbatim
*/
class Tokenizer {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGENERAL_API Tokenizer();

		/**
		*  @brief
		*    Destructor
		*/
		PLGENERAL_API ~Tokenizer();

		/**
		*  @brief
		*    Get delimiters (characters that a treated as whitespace)
		*
		*  @return
		*    Delimiter characters
		*/
		PLGENERAL_API String GetDelimiters() const;

		/**
		*  @brief
		*    Set delimiters (characters that a treated as whitespace)
		*
		*  @param[in] sDelimiters
		*    Delimiter characters
		*
		*  @remarks
		*    Default: " \t\r\n"
		*/
		PLGENERAL_API void SetDelimiters(const String &sDelimiters);

		/**
		*  @brief
		*    Get single characters
		*
		*  @return
		*    Single characters
		*/
		PLGENERAL_API String GetSingleChars() const;

		/**
		*  @brief
		*    Set single characters
		*
		*  @param[in] sSingleChars
		*    Single characters
		*
		*  @remarks
		*    Default: "{}()[]<*>/=,;\""
		*/
		PLGENERAL_API void SetSingleChars(const String &sSingleChars);

		/**
		*  @brief
		*    Get characters that are used for quotes
		*
		*  @return
		*    Quote characters
		*/
		PLGENERAL_API String GetQuotes() const;

		/**
		*  @brief
		*    Set characters that are used for quotes
		*
		*  @param[in] sQuotes
		*    Quote characters
		*
		*  @remarks
		*    Default: "\"\'"
		*/
		PLGENERAL_API void SetQuotes(const String &sQuotes);

		/**
		*  @brief
		*    Get the string that starts a multi-line comment
		*
		*  @return
		*    Comment start tag
		*/
		PLGENERAL_API String GetCommentStartTag() const;

		/**
		*  @brief
		*    Set the string that starts a multi-line comment
		*
		*  @param[in] sCommentStartTag
		*    Comment start tag
		*
		*  @remarks
		*    Default: "/*"
		*/
		PLGENERAL_API void SetCommentStartTag(const String &sCommentStartTag);

		/**
		*  @brief
		*    Get the string that ends a multi-line comment
		*
		*  @return
		*    Comment end tag
		*/
		PLGENERAL_API String GetCommentEndTag() const;

		/**
		*  @brief
		*    Set the string that ends a multi-line comment
		*
		*  @param[in] sCommentEndTag
		*    Comment end tag
		*
		*  @remarks
		*    Default: "*\/"
		*/
		PLGENERAL_API void SetCommentEndTag(const String &sCommentEndTag);

		/**
		*  @brief
		*    Get the string that starts a single-line comment
		*
		*  @return
		*    Comment start tag
		*/
		PLGENERAL_API String GetSingleLineComment() const;

		/**
		*  @brief
		*    Set the string that starts a single-line comment
		*
		*  @param[in] sSingleLineComment
		*    Comment start tag
		*
		*  @remarks
		*    Default: "//"
		*/
		PLGENERAL_API void SetSingleLineComment(const String &sSingleLineComment);

		/**
		*  @brief
		*    Get case sensitivity flag
		*
		*  @return
		*    'true' if the text is parsed case sensitive
		*/
		PLGENERAL_API bool IsCaseSensitive() const;

		/**
		*  @brief
		*    Set case sensitivity flag
		*
		*  @param[in] bCaseSensitive
		*    'true' if the text is parsed case sensitive (default is false)
		*
		*  @remarks
		*    Default: false
		*/
		PLGENERAL_API void SetCaseSensitive(bool bCaseSensitive);

		/**
		*  @brief
		*    Starts the tokenizer on a string
		*
		*  @param[in] sBuffer
		*    String buffer
		*/
		PLGENERAL_API void Start(const String &sBuffer);

		/**
		*  @brief
		*    Starts the tokenizer on a file
		*
		*  @param[in] cFile
		*    File to read, released automatically if Stop() is called
		*/
		PLGENERAL_API void Start(File &cFile);

		/**
		*  @brief
		*    Stops the tokenizer
		*/
		PLGENERAL_API void Stop();

		/**
		*  @brief
		*    Reads all tokens until the end of the stream
		*
		*  @return
		*    Array of all tokens of the stream
		*/
		PLGENERAL_API Array<String> GetTokens();

		/**
		*  @brief
		*    Reads the next token from the stream
		*
		*  @return
		*    Next token
		*
		*  @note
		*    - After the token has been read this function goes to the next token in the stream
		*    - To get the read token again, use GetToken()
		*
		*  @see
		*    - GetToken()
		*/
		PLGENERAL_API String GetNextToken();

		/**
		*  @brief
		*    Expects the next token to be equal to a given string
		*
		*  @return
		*    'true' if the next token is equal to the string
		*
		*  @note
		*    - If the expected token has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*    - The comparison of strings is done according to the settings given in SetCaseSensitive()
		*
		*  @see
		*    - IsCaseSensitive()
		*    - SetCaseSensitive()
		*/
		PLGENERAL_API bool ExpectToken(const String &sExpected);

		/**
		*  @brief
		*    Finds a given token in the stream
		*
		*  @return
		*    'true' if the token has been found
		*
		*  @note
		*    - Reads the next token until the expected token has been found or the end of the stream has been reached
		*    - If the function has succeed, the next call of GetNextToken() will return the desired token
		*    - The comparison of strings is done according to the settings given in SetCaseSensitive()
		*
		*  @see
		*    - IsCaseSensitive()
		*    - SetCaseSensitive()
		*/
		PLGENERAL_API bool FindToken(const String &sExpected);

		/**
		*  @brief
		*    Returns the current token
		*
		*  @return
		*    Current token
		*
		*  @note
		*    - Does not go to the next token in the stream, so multiple calls of this function will
		*      always return the same token.
		*/
		PLGENERAL_API String GetToken() const;

		/**
		*  @brief
		*    Compares the current token with a given string
		*
		*  @return
		*    'true' if the token is equal to the string
		*
		*  @note
		*    - Does not go to the next token in the stream, so multiple calls of this function will
		*      always return the same token
		*    - The comparison of strings is done according to the settings given in SetCaseSensitive()
		*
		*  @see
		*    - IsCaseSensitive()
		*    - SetCaseSensitive()
		*/
		PLGENERAL_API bool CompareToken(const String &sExpected);

		/**
		*  @brief
		*    Returns the current position in the stream
		*
		*  @return
		*    Position in the stream
		*/
		PLGENERAL_API uint32 GetPosition() const;

		/**
		*  @brief
		*    Returns the current line (counted by '\n' occurences)
		*
		*  @return
		*    Line in the file
		*/
		PLGENERAL_API uint32 GetLine() const;

		/**
		*  @brief
		*    Saves the current state of the tokenizer on a state stack
		*/
		PLGENERAL_API void PushState();

		/**
		*  @brief
		*    Restores the last saved state from the stack
		*/
		PLGENERAL_API void PopState();

		/**
		*  @brief
		*    Deletes the last saved state from the stack
		*/
		PLGENERAL_API void DropState();

		/**
		*  @brief
		*    Expects the next token to be a number and returns it as an integer value
		*
		*  @param[out] nNumber
		*    Receives the number
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @note
		*    - If a number has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseNumber(int &nNumber);

		/**
		*  @brief
		*    Expects the next token to be a floating point number and returns it as a float value
		*
		*  @param[out] fNumber
		*    Receives the number
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @note
		*    - If a number has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseNumber(float &fNumber);

		/**
		*  @brief
		*    Expects the next token to be a floating point number and returns it as a double value
		*
		*  @param[out] dNumber
		*    Receives the number
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @note
		*    - If a number has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseNumber(double &dNumber);

		/**
		*  @brief
		*    Expects the next tokens to be a vector and returns it as an array of strings
		*
		*  @param[out] cVector
		*    Receives the vector elements
		*  @param[in]  sStart
		*    Open bracket (e.g. "[")
		*  @param[in]  sEnd
		*    Closed bracket (e.g. "]")
		*  @param[in]  sSeperator
		*    Seperator between the elements (e.g. ","). Can also be ""
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: [one, two, three]
		*
		*  @note
		*    - If a vector has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseVector(Array<String> &cVector, const String &sStart = "[", const String &sEnd = "]", const String &sSeperator = ",");

		/**
		*  @brief
		*    Expects the next tokens to be a vector and returns it as an array of ints
		*
		*  @param[out] cVector
		*    Receives the vector elements
		*  @param[in]  sStart
		*    Open bracket (e.g. "[")
		*  @param[in]  sEnd
		*    Closed bracket (e.g. "]")
		*  @param[in]  sSeperator
		*    Seperator between the elements (e.g. ","). Can also be ""
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: [1, 2, 3]
		*
		*  @note
		*    - If a vector has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseVector(Array<int> &cVector, const String &sStart = "[", const String &sEnd = "]", const String &sSeperator = ",");

		/**
		*  @brief
		*    Expects the next tokens to be a vector and returns it as an array of floats
		*
		*  @param[out] cVector
		*    Receives the vector elements
		*  @param[in]  sStart
		*    Open bracket (e.g. "[")
		*  @param[in]  sEnd
		*    Closed bracket (e.g. "]")
		*  @param[in]  sSeperator
		*    Seperator between the elements (e.g. ","). Can also be ""
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: [1.0, 2.1, 3.2]
		*
		*  @note
		*    - If a vector has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseVector(Array<float> &cVector, const String &sStart = "[", const String &sEnd = "]", const String &sSeperator = ",");

		/**
		*  @brief
		*    Expects the next tokens to be a vector and returns it as an array of doubles
		*
		*  @param[out] cVector
		*    Receives the vector elements
		*  @param[in]  sStart
		*    Open bracket (e.g. "[")
		*  @param[in]  sEnd
		*    Closed bracket (e.g. "]")
		*  @param[in]  sSeperator
		*    Seperator between the elements (e.g. ","). Can also be ""
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: [1.0, 2.1, 3.2]
		*
		*  @note
		*    - If a vector has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseVector(Array<double> &cVector, const String &sStart = "[", const String &sEnd = "]", const String &sSeperator = ",");

		/**
		*  @brief
		*    Expects the next tokens to be an equation and returns it
		*
		*  @param[out] sName
		*    Name of the element
		*  @param[out] sValue
		*    Value as a string
		*  @param[in]  sEquation
		*    Euqation sign (e.g. "=")
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: Console = On
		*
		*  @note
		*    - If an equation has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseEquation(String &sName, String &sValue, const String &sEquation = "");

		/**
		*  @brief
		*    Expects the next tokens to be an equation and returns it
		*
		*  @param[out] sName
		*    Name of the element
		*  @param[out] nValue
		*    Value as an int
		*  @param[in]  sEquation
		*    Euqation sign (e.g. "=")
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: Health = 100
		*
		*  @note
		*    - If an equation has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseEquation(String &sName, int &nValue, const String &sEquation = "");

		/**
		*  @brief
		*    Expects the next tokens to be an equation and returns it
		*
		*  @param[out] sName
		*    Name of the element
		*  @param[out] fValue
		*    Value as a float
		*  @param[in]  sEquation
		*    Euqation sign (e.g. "=")
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: Gamma = 2.1
		*
		*  @note
		*    - If an equation has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseEquation(String &sName, float &fValue, const String &sEquation = "");

		/**
		*  @brief
		*    Expects the next tokens to be an equation and returns it
		*
		*  @param[out] sName
		*    Name of the element
		*  @param[out] dValue
		*    Value as a double
		*  @param[in]  sEquation
		*    Euqation sign (e.g. "=")
		*
		*  @return
		*    'true' on success, else 'false'
		*
		*  @remarks
		*    Example: Speed = 3.25
		*
		*  @note
		*    - If an equation has been found, the tokenizer goes to the next token in the stream,
		*      otherwise it stayes at the current token
		*/
		PLGENERAL_API bool ParseEquation(String &sName, double &dValue, const String &sEquation = "");


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Tokenizer(const Tokenizer &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Tokenizer &operator =(const Tokenizer &cSource);

		/**
		*  @brief
		*    Checks if the next character in the stream is one of the given characters
		*
		*  @param[in] sCharacters
		*    List of characters
		*
		*  @return
		*    'true' if the next character in the stream is one of the given characters, else 'false'
		*
		*  @note
		*    - 'm_pStream' must be valid!
		*/
		bool StreamIsChar(const String &sCharacters);

		/**
		*  @brief
		*    Checks if the next string in the stream equals the given one
		*
		*  @param[in] sString
		*    String to compare with
		*
		*  @return
		*    'true' if the next string in the stream equals the given one, else 'false'
		*
		*  @note
		*    - 'm_pStream' must be valid!
		*/
		bool StreamIsString(const String &sString);

		/**
		*  @brief
		*    Reads the next bytes of the stream
		*
		*  @param[in] nSize
		*    Number of bytes to read from the stream
		*
		*  @note
		*    - 'm_pStream' must be valid!
		*/
		void StreamRead(uint32 nSize);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const String EndOfLine;	/**< End of line ("\n") */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal parse mode of the state machine
		*/
		enum EParseMode {
			eEatGarbage,							/**< Eat garbage until the first character of a word has been found */
			eReadWord,								/**< Read a word until a delimiter has been found */
			eSingleChar,							/**< Read a single character word */
			eReadQuote,								/**< read the beginning of a quote */
			eReadQuoteInside,						/**< Read until the end quote has been found */
			eReadQuoteEnd,							/**< Read the end of a quote */
			eSkipComment							/**< Skip a comment until the comment end-tag has been found */
		};

		/**
		*  @brief
		*    Data structure to store the state of the tokenizer
		*/
		struct STokenizerState {
			String		m_sToken;					/**< The current token */
			uint32		m_nPosition;				/**< The current position */
			uint32		m_nLine;					/**< The current line */
			EParseMode	m_nParseMode;				/**< Current parse mode */
			String		m_sEndTag;					/**< End tag of the current mode, used for quotes and comments */
		};

		// Tokenizer options
		String m_sDelimiters;						/**< List of delimiters (white space) */
		String m_sSingleChars;						/**< List of characters that count as words (e.g. : or ,) */
		String m_sQuotes;							/**< List of characters that start quotes (e.g. " or ') */
		String m_sCommentStartTag;					/**< Start tag for multiline comments (e.g. "(*") */
		String m_sCommentEndTag;					/**< End tag for multiline comments (e.g. "*)") */
		String m_sSingleLineComment;				/**< Tag for singleline comments (e.g. "//") */
		bool   m_bCaseSensitive;					/**< Are comparisons case sensitive? */

		// Tokenizer status
		BufferedReader			*m_pStream;			/**< Character stream to read from, can be a null pointer */
		String					 m_sToken;			/**< The current token */
		uint32					 m_nPosition;		/**< The current position */
		uint32					 m_nLine;			/**< The current line */
		EParseMode				 m_nParseMode;		/**< Current parse mode */
		String					 m_sEndTag;			/**< End tag of the current mode, used for quotes and comments */
		Stack<STokenizerState>	 m_cStateStack;		/**< Stack of tokenizer states */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGeneral


#endif // __PLGENERAL_TOKENIZER_H__
