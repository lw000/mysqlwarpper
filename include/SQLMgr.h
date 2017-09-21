#ifndef __SQLMGR_H__
#define __SQLMGR_H__

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"
#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/metadata.h"
#include "cppconn/prepared_statement.h"

#include <functional>

class SQLQueryResult;
class SQLMgr;

class SQLMgr {
public:
	SQLMgr();
	~SQLMgr();

public:
	bool connect(const std::string& hostName, const std::string& userName,
			const std::string& password);
	bool connect(const std::string& hostName, const std::string& userName,
			const std::string& password, const std::string& schema);
	void useSchema(const std::string& schema);

public:
	sql::Connection* getConnection();

public:
	void print();

private:
	sql::Driver* _driver;
	sql::Connection* _con;
};

class SQLQuery {
public:
	SQLQuery();
	~SQLQuery();

private:

};

class SQLQueryResult {
public:
	SQLQueryResult(SQLMgr* mgr);
	SQLQueryResult(sql::Connection* conn);

public:
	~SQLQueryResult();

public:
	void reset();

public:
	sql::ResultSet* getResultSet() const;
	sql::Statement* getStatement() const;
	sql::PreparedStatement* getPreparedStatement() const;

public:
	bool createStatement();
	bool executeQuery(const std::string& sqlString,
			std::function<void(sql::ResultSet*)> onResultFunc,
			typename std::function<void(const std::string&)> onErrorFunc);

public:
	sql::PreparedStatement* prepareStatement(const std::string& sql);
	bool executeQuery(std::function<void(sql::ResultSet*)> onResultFunc,
			std::function<void(const std::string&)> onErrorFunc);

public:
	virtual void onReset() = 0;
	virtual void onResult(sql::ResultSet* res) = 0;
	virtual void onError(const std::string& error) = 0;

private:
	SQLMgr* _mgr;
	sql::ResultSet* _res;
	sql::Statement * _stmt;
	sql::PreparedStatement *_pstmt;
	sql::Connection* _conn;

	typename std::function<void(sql::ResultSet*)> _onResultFunc;
	typename std::function<void(const std::string&)> _onErrorFunc;
};

#endif	//__SQLMGR_H__
