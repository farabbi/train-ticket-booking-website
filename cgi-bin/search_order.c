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

	PGresult *res = PQprepare(conn, "search_order",
             "select o_tiid, o_oid, o_time, i_dtime, start.s_name, dest.s_name, o_price "
             "from orders, station start, station dest, infor "
             "where o_time between $1 and $2 and o_tid=i_tid and o_dsid=i_sid "
             "and o_dsid=start.s_sid and o_asid=dest.s_sid "
             "order by o_oid, o_tiid", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	char *param_val[2] = {input[0].value, input[1].value};
	int param_len[2] = {0, 0};
	int param_format[2] = {0, 0};

	res = PQexecPrepared(conn, "search_order", 2, (const char **)param_val,
                         param_len, param_format, 0);
	ExecStatusType status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}

	printf("<table border=\"1\">\n");
	printf("<tr><th>订单号</th><th>日期</th><th>发车时间</th><th>出发站</th><th>到达站</th><th>总费用</th></tr>\n");
	char *val = NULL;
	char *val_temp1 = NULL;
	int price = 0;
	int count = 0;
	int num_rows = PQntuples(res);
	int r;
	for (r = 0; r < num_rows; r++) {
		printf("<tr>\n");
		val = PQgetvalue(res, r, 1);
		val_temp1 = val;
			printf("<td><a href=\"/cgi-bin/print_order.cgi?orderid=%s\">%s</a></td>\n", val, val);
		val = PQgetvalue(res, r, 2);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 3);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 4);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 6);
		price += atoi(val);
		count++;
		if (r < num_rows-1) {		
			val = PQgetvalue(res, r+1, 1);
		}
		while (r < num_rows-1 && strcmp(val_temp1, val) == 0) {
			r++;
			val = PQgetvalue(res, r, 6);
			price = atoi(val);
			count++;
			if (r < num_rows-1) {		
				val = PQgetvalue(res, r+1, 1);
			}
		}
		val = PQgetvalue(res, r, 5);
			printf("<td>%s</td>\n", val);
		printf("<td>%d</td>\n", price + 5 * count);
		price = 0;
		count = 0;
		printf("</tr>\n");
	}
	printf("</table>\n");
	PQclear(res);

	PQfinish(conn);

	printf("<a href=\"/index.html\">返回</a>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}

