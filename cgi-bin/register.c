#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>

int main()
{
	int i = 0;
	char c = 0;
	char *p = NULL;
	char buffer[100];
	int content_length = 0;
	struct {
		char name[100];
		char value[100];
	} input[10];
	int input_num = 0;

	if (getenv("CONTENT_LENGTH")) {
		content_length = atoi(getenv("CONTENT_LENGTH"));
	}

	p = buffer;
	for (i = 0; i < content_length; i++) {
		c = getchar();
		switch (c) {
			case '=':
				*p = '\0';
				p = buffer;
				strcpy(input[input_num].name, buffer);
				break;
			case '&':
				*p = '\0';
				p = buffer;
				strcpy(input[input_num].value, buffer);
				input_num++;
				break;
			default:
				*p++ = c;
				break;
		}
	}
	*p = '\0';
	p = buffer;
	strcpy(input[input_num].value, buffer);
	input_num++;

	printf("Content-type:text/html\n\n");
	printf("<!DOCTYPE html>\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<meta charset='utf-8'/>\n");
	printf("</head>\n");
	printf("<body>\n");

	PGconn *conn = PQconnectdb("dbname=lab2");
	if (PQstatus(conn) != CONNECTION_OK) {
		printf("Connection to db failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		return 1;
	}

/*
	PGresult *res = PQprepare(conn, "register",
             "insert into users(u_username, u_userid, u_phone, u_card, u_bank) "
             "values ($1, $2, $3, $4, $5)",
             5, NULL);
*/
	PGresult *res = PQprepare(conn, "register",
             "insert into passenger(p_name, p_idcard, p_tele, p_credit, p_bank) "
             "values ($1, $2, $3, $4, $5)",
             5, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	char *param_val[5] = {input[0].value, input[1].value, input[2].value, input[3].value, input[4].value};
	int param_len[5] = {0, 0, 0, 0, 0};
	int param_format[5] = {0, 0, 0, 0, 0};

	res = PQexecPrepared(conn, "register", 5, (const char **)param_val,
                         param_len, param_format, 0);
	ExecStatusType status = PQresultStatus(res);
	if (status != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	PQfinish(conn);

	printf("注册成功!<br />\n");
	printf("<a href=\"/index.html\">返回主页</a>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}

