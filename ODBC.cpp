#include "stdafx.h"
#include "ODBC.h"


void CODBC::AllocateHandles()
{
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);


	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

		// ODBC 드라이버 버전 명시

		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);


		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			// 연결 핸들러 할당

			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);


			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

				printf("Allocate Success\n");

			}

			else {

				SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

				printf("%s : %ld : %ld : %s\n", state, rec, native, message);

			}

		}

		else {

			SQLGetDiagRec(SQL_HANDLE_ENV, henv, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

			printf("%s : %ld : %ld : %s\n", state, rec, native, message);

		}

	}

	else {

		SQLGetDiagRec(SQL_HANDLE_ENV, henv, rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

		printf("%s : %ld : %ld : %s\n", state, rec, native, message);

	}
}

void CODBC::ConnectDataSource()
{
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"asdfusers", SQL_NTS, (SQLWCHAR*)L"hm", SQL_NTS, (SQLWCHAR*)L"test", SQL_NTS);

}


void CODBC::ExecuteStatementDirect(SQLWCHAR * sql)
{
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		printf("Connect Success\n");

	}

	else {

		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

		printf("%s : %ld : %ld : %s\n", state, rec, native, message);

	}

	SQLExecDirect(hstmt, (SQLWCHAR*)L"use test;", SQL_NTS);
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)sql, SQL_NTS);
	//retcode = SQLExecDirect(hstmt, (SQLWCHAR*)L"select * from dbo.Test", SQL_NTS);

	if (retcode == SQL_SUCCESS) {

		printf("Query Seuccess\n");

	}

	else {

		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

		printf("%s : %ld : %ld : %s\n", state, rec, native, message);

	}
}

void CODBC::PrepareStatement(SQLWCHAR * sql)
{
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		printf("Connect Success\n");

	}

	else {

		SQLGetDiagRec(SQL_HANDLE_DBC, hdbc, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

		printf("%s : %ld : %ld : %s\n", state, rec, native, message);

	}


	retcode = SQLPrepare(hstmt, sql, SQL_NTS);


	if (retcode == SQL_SUCCESS) {

		printf("\nQuery Prepare Success\n");

	}

	else {

		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

		printf("\n%s : %ld : %ld : %s\n", state, rec, native, message);

	}
}

void CODBC::ExecuteStatement()
{
	retcode = SQLExecute(hstmt);


	if (retcode == SQL_SUCCESS) {

		printf("Query Execute Success\n");

	}

	else {

		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, ++rec, (SQLWCHAR*)state, &native, (SQLWCHAR*)message, sizeof(message), &length);

		printf("%s : %ld : %ld : %s\n", state, rec, native, message);

	}
}

void CODBC::RetrieveResult()
{
	int number;

	char name[10];

	char phone[20];

	SQLLEN cid, cna, cal;


	SQLBindCol(hstmt, 1, SQL_C_SHORT, &number, sizeof(number), &cid);

	SQLBindCol(hstmt, 2, SQL_C_CHAR, &name, sizeof(name), &cna);

	SQLBindCol(hstmt, 3, SQL_C_FLOAT, &phone, sizeof(phone), &cal);


	printf("id\tname\t\tcal\t\tstars");


	do {

		retcode = SQLFetch(hstmt);

		printf("\n %d \t %s \t %s", number, name, phone);

	} while (retcode != SQL_NO_DATA);


	SQLFreeStmt(hstmt, SQL_UNBIND);
}

void CODBC::DisconnectDataSource()
{
	if (hstmt) {

		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

		hstmt = NULL;

	}



	SQLDisconnect(hdbc);



	if (hdbc) {

		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);

		hdbc = NULL;

	}

	if (henv) {

		SQLFreeHandle(SQL_HANDLE_ENV, henv);

		henv = NULL;

	}
}
