#ifndef _DB_SQLITE_H_
#define _DB_SQLITE_H_
/*----------------------------------------------------------------------------*
 * DbSQLite.h
 *
 * 6 APR 2005
 *
 * This source code is derived from the SQLiteWrapper source produced by
 * rene.nyffenegger@adp-gmbh.ch.  I altered the original source while
 * packaging it for use with Microsoft MFC.
 *
 * The primary objective in this exercise was to make the wrapper
 * suitable for both MCBS and Unicode because Unicdoe is native to the
 * Windows CE OS. There are three key differences between this wrapper
 * and the author's original source.
 *
 * First, all std::string variables were converted to Microsoft's generic
 * string pointers using LPCTSTR.  This should be familiar to developer's
 * accustomed to working with MFC.  This also means that you should rely
 * upon the the Microsoft TEXT or _T macros for hard-coded character strings.
 *
 * Second, I have change the class SQLiteStatement to CSqlStatement and
 * SQLiteWrapper to CDbSQLite.  This was primarily a matter of preference
 * since most MFC developer's recoginize Microsoft's convention for the
 * CFunction nomenclature.
 *
 * Finally, I have added the header SQLite3i.h with typedefs to the various
 * sqlite3.h functions.  These type definitions are "internal" accessors
 * to the sqlite3 functions utilizing either the UTF-8 or UTF-16 variation
 * as appropriate.
 *
 * The remaining comments at the beginning of this file are the author's
 * original copyright message.
 *----------------------------------------------------------------------------*
 * SQLiteWrapper.h
 *
 * Copyright (C) 2004 Ren Nyffenegger
 *
 * This source code is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented; you must not
 *    claim that you wrote the original source code. If you use this source code
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original source code.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Ren Nyffenegger rene.nyffenegger@adp-gmbh.ch
 *----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "sqlite3i.h"
#include "sqlite3.h"
#include <vector>

class CSqlStatement
{
public:
   CSqlStatement();
// CSqlStatement(const CSqlStatement&);
   virtual ~CSqlStatement();
private:
//
// CSqlStatement's ctor only to be called by CDbSQLite
//
   friend class CDbSQLite;
   CSqlStatement(LPCTSTR statement, sqlite3* db);

public:

   typedef enum
   {
      eInteger = SQLITE_INTEGER,
      eFloat   = SQLITE_FLOAT  ,
      eText    = SQLITE_TEXT   ,
      eBlob    = SQLITE_BLOB   ,
      eNull    = SQLITE_NULL   ,
   }
   EDataType;

   EDataType DataType(int pos_zero_indexed);
   int Fields();
   LPCTSTR FieldName(int pos_zero_indexed);
   LPCTSTR FieldType(int pos_zero_indexed);

   int ValueInt(int pos_zero_indexed);
   LPCTSTR ValueString(int pos_zero_indexed);

   bool Bind (int pos_zero_indexed, LPCTSTR value);
   bool Bind (int pos_zero_indexed, double value);
   bool Bind (int pos_zero_indexed, int value);
   bool BindNull (int pos_zero_indexed);

   bool Execute();
   bool NextRow();

// Call Reset if not depending on the NextRow cleaning up.
// For example for select count(*) statements.

   bool Reset();
   bool RestartSelect();

private:
   sqlite3_stmt* m_stmt;
   CString m_szText;
};

class CDbSQLite
{
public:
   CDbSQLite();
   virtual ~CDbSQLite();

   bool Open(LPCTSTR db_file);
   bool IsOpen() {return m_bIsOpend;}

   class ResultRecord
   {
   public:
      std::vector<LPCTSTR> m_fields;
   };

   class ResultTable
   {
     friend class CDbSQLite;
   public:
     ResultTable() : ptr_cur_record_(0) {}
    
     std::vector<ResultRecord> m_records;
    
     ResultRecord* next()
     {
        if (ptr_cur_record_ < m_records.size())
        {
           return &(m_records[ptr_cur_record_++]);
        }

        return 0;
     }
    
   private:
      void reset()
      {
         m_records.clear();
         ptr_cur_record_=0;
      }
   private:
      unsigned int ptr_cur_record_;
   };

   bool SelectStatement(LPCTSTR stmt, ResultTable& res);
   bool DirectStatement(LPCTSTR stmt);
   CSqlStatement* Statement(LPCTSTR stmt);

   LPCTSTR LastError();
   //
   // Transaction related
   //
   bool Begin();
   bool Commit();
   bool Rollback();

   // Pack database
   bool Pack();

private:
   static int SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);
   sqlite3* m_db;
   CString m_szName;
   CString m_szText;
   bool		m_bIsOpend;	// 数据库是否已经打开
};

#endif
