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

	PGresult *res = PQprepare(conn, "print_order",
             "select o_oid, t_tid, start.s_name, dest.s_name, o_type, o_price, o_time, i_dtime "
             "from orders, train, station start, station dest, infor "
             "where o_oid=$1 and o_tid=t_tid and o_dsid=start.s_sid and o_asid=dest.s_sid "
             "and o_tid=i_tid and o_dsid=i_sid", 1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	char *param_val[1] = {input[0].value};
	int param_len[1] = {0};
	int param_format[1] = {0};

	res = PQexecPrepared(conn, "print_order", 1, (const char **)param_val,
                         param_len, param_format, 0);
	ExecStatusType status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	printf("<table border=\"1\">\n");
	printf("<tr><th>订单号</th><th>车次</th><th>出发站</th><th>到达站</th><th>座位类型</th><th>票价</th><th>日期</th><th>发车时间</th></tr>\n");
	char *val = NULL;
	int num_rows = PQntuples(res);
	int r;
	for (r = 0; r < num_rows; r++) {
		printf("<tr>\n");
		val = PQgetvalue(res, r, 0);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 1);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 2);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 3);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 4);
			if (strncmp(val, "yz", 2) == 0) {
				printf("<td>硬座</td>\n");
			} else if (strncmp(val, "rz", 2) == 0) {
				printf("<td>软座</td>\n");
			} else if (strncmp(val, "yws", 3) == 0) {
				printf("<td>硬卧上铺</td>\n");
			} else if (strncmp(val, "ywz", 3) == 0) {
				printf("<td>硬卧中铺</td>\n");
			} else if (strncmp(val, "ywx", 3) == 0) {
				printf("<td>硬卧下铺</td>\n");
			} else if (strncmp(val, "rws", 3) == 0) {
				printf("<td>软卧上铺</td>\n");
			} else if (strncmp(val, "rwx", 3) == 0) {
				printf("<td>软卧下铺</td>\n");
			} else {
				printf("failed exection: seattype match error\n");
				PQfinish(conn);
				return 1;
			}
		val = PQgetvalue(res, r, 5);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 6);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 7);
			printf("<td>%s</td>\n", val);
		printf("</tr>\n");
	}
	printf("</table>\n");
	PQclear(res);

	PQfinish(conn);

	printf("<a href=\"/cgi-bin/delete_order.cgi?orderid=%s\">删除订单</a>\n", input[0].value);
	printf("<a href=\"/index.html\">返回主页</a>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}
