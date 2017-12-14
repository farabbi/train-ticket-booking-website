#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>

void reverse(char s[])
{
	int c, i, j;

	for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

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

	char *val = NULL;
	char time[105];
		strcpy(time, input[1].value);
	char pid[105];
		strcpy(pid, input[4].value);
	char tid[105];
	char dsid[105];
	char asid[105];
	int oid = 0;
	char oid_s[100];
	char type[105];
		strcpy(type, input[5].value);
	int price_dest = 0;
	int price_start = 0;
	char price[100];

//printf("<br />time:%s<br />pid:%s<br />tid:%s<br />dsid:%s<br />asid:%s<br />oid_s:%s<br />type:%s<br />price:%s<br />", time, pid, tid, dsid, asid, oid_s, type, price);

	/* tid */
	PGresult *res = PQprepare(conn, "book_gettid", "select t_tid from train where t_name=$1", 1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
printf("1");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);
	char *param_val1[1] = {input[0].value};
	int param_len1[1] = {0};
	int param_format1[1] = {0};
	res = PQexecPrepared(conn, "book_gettid", 1, (const char **)param_val1, param_len1, param_format1, 0);
	ExecStatusType status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
printf("2");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
		strcpy(tid, val);
	PQclear(res);
//printf("<br />time:%s<br />pid:%s<br />tid:%s<br />dsid:%s<br />asid:%s<br />oid_s:%s<br />type:%s<br />price:%s<br />", time, pid, tid, dsid, asid, oid_s, type, price);
	/* dsid and asid and price */
	res = PQprepare(conn, "book_getsid", "select i_sid, i_yzprice, i_rzprice, i_ywsprice, i_ywzprice, i_ywxprice, i_rwsprice, i_rwxprice from infor where i_tid=$1 and i_sorder=$2", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
printf("3");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);
	char *param_val2[2] = {tid, input[2].value};
	int param_len2[2] = {0, 0};
	int param_format2[2] = {0, 0};
	res = PQexecPrepared(conn, "book_getsid", 2, (const char **)param_val2, param_len2, param_format2, 0);
	status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
printf("4");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
		strcpy(dsid, val);
	if (strncmp(type, "yz", 2) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 1));
	} else if (strncmp(type, "rz", 2) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 2));
	} else if (strncmp(type, "yws", 3) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 3));
	} else if (strncmp(type, "ywz", 3) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 4));
	} else if (strncmp(type, "ywx", 3) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 5));
	} else if (strncmp(type, "rws", 3) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 6));
	} else if (strncmp(type, "rwx", 3) == 0) {
		price_start = atoi(PQgetvalue(res, 0, 7));
	} else {
		printf("failed exection: seattype match error\n");
		PQfinish(conn);
		return 1;
	}
	PQclear(res);
	char *param_val3[2] = {tid, input[3].value};
	int param_len3[2] = {0, 0};
	int param_format3[2] = {0, 0};
	res = PQexecPrepared(conn, "book_getsid", 2, (const char **)param_val3, param_len3, param_format3, 0);
	status = PQresultStatus(res);
//printf("<br />time:%s<br />pid:%s<br />tid:%s<br />dsid:%s<br />asid:%s<br />oid_s:%s<br />type:%s<br />price:%s<br />", time, pid, tid, dsid, asid, oid_s, type, price);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
printf("5");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
		strcpy(asid, val);
	if (strncmp(type, "yz", 2) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 1));
	} else if (strncmp(type, "rz", 2) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 2));
	} else if (strncmp(type, "yws", 3) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 3));
	} else if (strncmp(type, "ywz", 3) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 4));
	} else if (strncmp(type, "ywx", 3) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 5));
	} else if (strncmp(type, "rws", 3) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 6));
	} else if (strncmp(type, "rwx", 3) == 0) {
		price_dest = atoi(PQgetvalue(res, 0, 7));
	} else {
		printf("failed exection: seattype match error\n");
		PQfinish(conn);
		return 1;
	}
	itoa(price_dest-price_start, price);
	PQclear(res);

	/* oid */
	res = PQexec(conn, "select count(distinct o_oid) from orders");
	status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
printf("6");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
		strcpy(oid_s, val);
	oid = atoi(oid_s) + 1;
	itoa(oid, oid_s);
	PQclear(res);

	/* get_pid */
	res = PQprepare(conn, "book_pid", "select p_pid from passenger where p_idcard=$1", 1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);
	char *param_val6[1] = {pid};
	int param_len6[1] = {0};
	int param_format6[1] = {0};
	res = PQexecPrepared(conn, "book_pid", 1, (const char **)param_val6, param_len6, param_format6, 0);
	status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
	strcpy(pid, val);
	PQclear(res);

	/* final */
	res = PQprepare(conn, "book", "insert into orders(o_time, o_pid, o_tid, o_dsid, o_asid, o_oid, o_type, o_price) "
                                  "values($1, $2, $3, $4, $5, $6, $7, $8)", 8, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
printf("7");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	char *param_val4[8] = {time, pid, tid, dsid, asid, oid_s, type, price};
	int param_len4[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int param_format4[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//printf("<br />time:%s<br />pid:%s<br />tid:%s<br />dsid:%s<br />asid:%s<br />oid_s:%s<br />type:%s<br />price:%s<br />", time, pid, tid, dsid, asid, oid_s, type, price);
	res = PQexecPrepared(conn, "book", 8, (const char **)param_val4, param_len4, param_format4, 0);
	status = PQresultStatus(res);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
printf("8");
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}

	/* print */
	res = PQprepare(conn, "print_order",
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
	char *param_val5[1] = {oid_s};
	int param_len5[1] = {0};
	int param_format5[1] = {0};
	res = PQexecPrepared(conn, "print_order", 1, (const char **)param_val5, param_len5, param_format5, 0);
	status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	printf("<table border=\"1\">\n");
	printf("<tr><th>订单号</th><th>车次</th><th>出发站</th><th>到达站</th><th>座位类型</th><th>票价</th><th>日期</th><th>发车时间</th></tr>\n");
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

	printf("<a href=\"/index.html\">返回主页</a>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}

