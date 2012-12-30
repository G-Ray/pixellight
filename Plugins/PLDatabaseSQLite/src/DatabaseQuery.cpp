/*********************************************************\
 *  File: DatabaseQuery.cpp                              *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the �Software�), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <sqlite3.h>
#include <PLCore/Log/Log.h>
#include "PLDatabaseSQLite/Database.h"
#include "PLDatabaseSQLite/DatabaseQueryResult.h"
#include "PLDatabaseSQLite/DatabaseQuery.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLDatabaseSQLite {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DatabaseQuery::~DatabaseQuery()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DatabaseQuery::DatabaseQuery(PLDatabase::Database &cParentDd) : PLDatabase::DatabaseQuery(cParentDd)
{
}

/**
*  @brief
*    Copy constructor
*/
DatabaseQuery::DatabaseQuery(const DatabaseQuery &cSource) : PLDatabase::DatabaseQuery(cSource.GetDatabase())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
DatabaseQuery &DatabaseQuery::operator =(const DatabaseQuery &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual PLDatabase::DatabaseQuery functions    ]
//[-------------------------------------------------------]
PLDatabase::DatabaseQueryResult *DatabaseQuery::Execute(const String &sSQL)
{
	sqlite3 *pSQLite = static_cast<Database&>(GetDatabase()).GetSQLite();
	DatabaseQueryResult *pQueryResult = new DatabaseQueryResult(*this);
	int nError = sqlite3_get_table(pSQLite,
								   sSQL,
								   &pQueryResult->m_ppszResult,
								   &pQueryResult->m_nRow,
								   &pQueryResult->m_nColumn,
								   nullptr);
	if (nError == SQLITE_OK && pQueryResult->m_ppszResult) {
		// 'SELECT'-SQL?
		if (IsSelect(sSQL))
			return pQueryResult; // Return the query result
	} else {
		PL_LOG(Error, String("SQLite can't execute the given SQL statement! Error: ") + sqlite3_errmsg(pSQLite))
	}

	// Destroy the query result
	delete pQueryResult;

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseSQLite
