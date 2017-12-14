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
	PGresult *res = PQprepare(conn, "search_traininfo",
             "select sc_sequence, st_stationname, sc_arrive, sc_departure, "
                    "sc_yzprice, r_yzticket, sc_rzprice,  r_rzticket, "
                    "sc_ywsprice, r_ywsticket, sc_ywzprice, r_ywzticket, sc_ywxprice, r_ywxticket, "
                    "sc_rwsprice, r_rwsticket, sc_rwxprice, r_rwxticket "
             "from stations, schedule, remain, trains "
             "where sc_stationno=r_stationno and sc_trainno=r_trainno "
                   "and sc_stationno=st_stationno and sc_trainno=t_trainno "
                   "and t_trainid=$1 and r_date=$2 "
             "order by sc_sequence", 2, NULL);
*/
	PGresult *res = PQprepare(conn, "search_traininfo",
             "select i_sorder, s_name, i_atime, i_dtime, "
                    "i_yzprice, r_yzleft, i_rzprice,  r_rzleft, "
                    "i_ywsprice, r_ywsleft, i_ywzprice, r_ywzleft, i_ywxprice, r_ywxleft, "
                    "i_rwsprice, r_rwsleft, i_rwxprice, r_rwxleft "
             "from station, infor, remaining, train "
             "where i_sid=r_sid and i_tid=r_tid "
                   "and i_sid=s_sid and i_tid=t_tid "
                   "and t_name=$1 and r_time=$2 "
             "order by i_sorder", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		printf("failed execution: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	PQclear(res);

	char *param_val[2] = {input[0].value, input[1].value};
	int param_len[2] = {0, 0};
	int param_format[2] = {0, 0};

	res = PQexecPrepared(conn, "search_traininfo", 2, (const char **)param_val,
                         param_len, param_format, 0);
	ExecStatusType status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed exection: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	printf("<table border=\"1\">\n");
	printf("<caption>%s</caption>\n", input[0].value);
	printf("<tr>\n");
	printf("<th>序号</th><th>站名</th><th>到时</th><th>发时</th>\n");
	printf("<th>硬座/软座票价</th><th>硬座/软座余票</th>\n");
	printf("<th>硬卧（上/中/下）票价</th><th>硬卧（上/中/下）余票</th>\n");
	printf("<th>软卧（上/下）票价</th><th>软卧（上/下）余票</th>\n");
	printf("</tr>\n");
	int yz=0, rz=0, yws=0, ywz=0, ywx=0, rws=0, rwx=0;
	char *val = NULL;
	int num_rows = PQntuples(res);
	int r;
	for (r = 0; r < num_rows; r++) {
		printf("<tr>\n");
		/* 序号|站名|到时|发时 */
		val = PQgetvalue(res, r, 0);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 1);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 2);
			printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 3);
			printf("<td>%s</td>\n", val);
		/* 硬座/软座票价|硬座/软座余票 */
		val = PQgetvalue(res, r, 4);
			printf("<td>%s/", val);
		val = PQgetvalue(res, r, 6);
			printf("%s</td>\n", val);
		val = PQgetvalue(res, r, 5);
			printf("<td>%s/", val);
			yz += atoi(val);
		val = PQgetvalue(res, r, 7);
			printf("%s</td>\n", val);
			rz += atoi(val);
		/* 硬卧（上/中/下）票价|硬卧（上/中/下）余票 */
		val = PQgetvalue(res, r, 8);
			printf("<td>%s/", val);
		val = PQgetvalue(res, r, 10);
			printf("%s/", val);
		val = PQgetvalue(res, r, 12);
			printf("%s</td>\n", val);
		val = PQgetvalue(res, r, 9);
			printf("<td>%s/", val);
			yws += atoi(val);
		val = PQgetvalue(res, r, 11);
			printf("%s/", val);
			ywz += atoi(val);
		val = PQgetvalue(res, r, 13);
			printf("%s</td>\n", val);
			ywx += atoi(val);
		/* 软卧（上/下）票价|软卧（上/下）余票 */
		val = PQgetvalue(res, r, 14);
			printf("<td>%s/", val);
		val = PQgetvalue(res, r, 16);
			printf("%s</td>\n", val);
		val = PQgetvalue(res, r, 15);
			printf("<td>%s/", val);
			rws += atoi(val);
		val = PQgetvalue(res, r, 17);
			printf("%s</td>\n", val);
			rwx += atoi(val);
		printf("</tr>\n");
	}
	printf("</table>\n");
	PQclear(res);

	PQfinish(conn);

	printf("<hr />\n");
	printf("<form action=\"/cgi-bin/book.cgi\" method=\"POST\" accept-charset=\"UTF-8\">\n");
	printf("车次:<br />\n");
	printf("<input type=\"text\" name=\"trainid\" value=\"%s\" readonly><br />\n", input[0].value);
	printf("日期:<br />\n");
	printf("<input type=\"text\" name=\"date\" value=\"%s\" readonly><br />\n", input[1].value);
	printf("出发车站序号:<br />\n");
	printf("<input type=\"text\" name=\"startstation\"><br />\n");
	printf("目的地车站序号:<br />\n");
	printf("<input type=\"text\" name=\"destination\"><br />\n");
	printf("身份证号:<br />\n");
	printf("<input type=\"text\" name=\"userid\"><br />\n");
	printf("座位类型:<br />\n");
	printf("<select name=\"seattype\">\n");
	if (yz > 0) {
		printf("<option value=\"yz\">硬座</option>");
	}
	if (rz > 0) {
		printf("<option value=\"rz\">软座</option>");
	}
	if (yws > 0) {
		printf("<option value=\"yws\">硬卧上</option>");
	}
	if (ywz > 0) {
		printf("<option value=\"ywz\">硬卧中</option>");
	}
	if (ywx > 0) {
		printf("<option value=\"ywx\">硬卧下</option>");
	}
	if (rws > 0) {
		printf("<option value=\"rws\">软卧上</option>");
	}
	if (rwx > 0) {
		printf("<option value=\"rwx\">软卧下</option>");
	}
	printf("</select><br />\n");
	printf("<br />\n");
	printf("<input type=\"submit\" value=\"预定\"><br />\n");
	printf("</form>\n");

	printf("<hr />");
	printf("<a href=\"/index.html\">返回主页</a>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}

