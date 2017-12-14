#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>

int main()
{
	char *data = NULL;
	char *p = NULL;
	char buffer[100];
	struct {
		char name[30];
		char value[30];
	} input[10];
	int input_num = 0;

	printf("Content-type:text/html\n\n");
	printf("<!DOCTYPE html>\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<meta charset='utf-8'/>\n");
	printf("</head>\n");
	printf("<body>\n");

	data = getenv("QUERY_STRING");
	if (data == NULL) {
		printf("get data failed\n");
		return 1;
	}
	p = buffer;
	while (*data != '\0') {
		switch (*data) {
			case '=':
				*p = '\0';
				p = buffer;
				data++;
				strcpy(input[input_num].name, buffer);
				break;
			case '&':
				*p = '\0';
				p = buffer;
				data++;
				strcpy(input[input_num].value, buffer);
				input_num++;
				break;
			default:
				*p++ = *data++;
				break;
		}
	}
	*p = '\0';
	p = buffer;
	strcpy(input[input_num].value, buffer);
	input_num++;

	PGconn *conn = PQconnectdb("dbname=lab2");
	if (PQstatus(conn) != CONNECTION_OK) {
		printf("Connection to db failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		return 1;
	}

/*
	PGresult *res = PQprepare(conn, "delete_order",
             "delete from orders where o_orderid=$1",
             1, NULL);
*/
	PGresult *res = PQprepare(conn, "delete_order",
             "delete from orders where o_oid=$1",
             1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	char *param_val[1] = {input[0].value};
	int param_len[1] = {0};
	int param_format[1] = {0};

	res = PQexecPrepared(conn, "delete_order", 1, (const char **)param_val,
                         param_len, param_format, 0);
	ExecStatusType status = PQresultStatus(res);
	if (status != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	PQfinish(conn);

	printf("订单删除成功!<br />\n");
	printf("<a href=\"/index.html\">返回主页</a>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}

