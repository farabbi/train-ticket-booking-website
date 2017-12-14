#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>

int main()
{
	char *val = NULL;
	int destination_price = 0;
	int startstation_price = 0;
	int num_rows = 0;
	int r = 0;

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

	/* 总订单数 */
//	PGresult *res = PQexec(conn, "select count(distinct o_orderid) from orders");
	PGresult *res = PQexec(conn, "select count(distinct o_oid) from orders");
	ExecStatusType status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed exection: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
	printf("总订单数: %s<br />\n", val);
	PQclear(res);

	/* 总票价 */
	/* destination price */
/*
	res = PQexec(conn, "select o_ticketno, o_seattype, sc_yzprice, sc_rzprice, "
                       "sc_ywsprice, sc_ywzprice, sc_ywxprice, sc_rwsprice, sc_rwxprice "
                       "from schedule, orders "
                       "where sc_trainno=o_trainno and sc_stationno=o_destination");
*/
	res = PQexec(conn, "select o_tiid, o_type, i_yzprice, i_rzprice, "
                       "i_ywsprice, i_ywzprice, i_ywxprice, i_rwsprice, i_rwxprice "
                       "from infor, orders "
                       "where i_tid=o_tid and i_sid=o_asid");
	num_rows = PQntuples(res);
	for (r = 0; r < num_rows; r++) {
		val = PQgetvalue(res, r, 1);
		if (strncmp(val, "yz", 2) == 0) {
			val = PQgetvalue(res, r, 2);
		} else if (strncmp(val, "rz", 2) == 0) {
			val = PQgetvalue(res, r, 3);
		} else if (strncmp(val, "yws", 3) == 0) {
			val = PQgetvalue(res, r, 4);
		} else if (strncmp(val, "ywz", 3) == 0) {
			val = PQgetvalue(res, r, 5);
		} else if (strncmp(val, "ywx", 3) == 0) {
			val = PQgetvalue(res, r, 6);
		} else if (strncmp(val, "rws", 3) == 0) {
			val = PQgetvalue(res, r, 7);
		} else if (strncmp(val, "rwx", 3) == 0) {
			val = PQgetvalue(res, r, 8);
		} else {
			printf("failed exection: seattype match error\n");
			PQfinish(conn);
			return 1;
		}
		destination_price += atoi(val);
	}
	PQclear(res);
	/* startstation price */
/*
	res = PQexec(conn, "select o_ticketno, o_seattype, sc_yzprice, sc_rzprice, "
                       "sc_ywsprice, sc_ywzprice, sc_ywxprice, sc_rwsprice, sc_rwxprice "
                       "from schedule, orders "
                       "where sc_trainno=o_trainno and sc_stationno=o_startstation");
*/
	res = PQexec(conn, "select o_tiid, o_type, i_yzprice, i_rzprice, "
                       "i_ywsprice, i_ywzprice, i_ywxprice, i_rwsprice, i_rwxprice "
                       "from infor, orders "
                       "where i_tid=o_tid and i_sid=o_dsid");
	num_rows = PQntuples(res);
	for (r = 0; r < num_rows; r++) {
		val = PQgetvalue(res, r, 1);
		if (strncmp(val, "yz", 2) == 0) {
			val = PQgetvalue(res, r, 2);
		} else if (strncmp(val, "rz", 2) == 0) {
			val = PQgetvalue(res, r, 3);
		} else if (strncmp(val, "yws", 3) == 0) {
			val = PQgetvalue(res, r, 4);
		} else if (strncmp(val, "ywz", 3) == 0) {
			val = PQgetvalue(res, r, 5);
		} else if (strncmp(val, "ywx", 3) == 0) {
			val = PQgetvalue(res, r, 6);
		} else if (strncmp(val, "rws", 3) == 0) {
			val = PQgetvalue(res, r, 7);
		} else if (strncmp(val, "rwx", 3) == 0) {
			val = PQgetvalue(res, r, 8);
		} else {
			printf("failed exection: seattype match error\n");
			PQfinish(conn);
			return 1;
		}
		startstation_price += atoi(val);
	}
	PQclear(res);
	printf("总票价: %d元<br />\n", destination_price - startstation_price);

	/* 总订票费 */
	res = PQexec(conn, "select count(*) from orders");
	status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed exection: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	val = PQgetvalue(res, 0, 0);
	printf("总订票费: %d元<br />", 5 * atoi(val));
	PQclear(res);

	/* 热点车次 */
/*
	res = PQexec(conn, "select t_trainid, count(*) as number "
                       "from trains, orders "
                       "where t_trainno=o_trainno "
                       "group by t_trainid "
                       "order by number desc");
*/
	res = PQexec(conn, "select t_name, count(*) as number "
                       "from train, orders "
                       "where t_tid=o_tid "
                       "group by t_name "
                       "order by number desc");
	status = PQresultStatus(res);
	if ((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)) {
		printf("failed exection: %s\n", PQresultErrorMessage(res));
		PQfinish(conn);
		return 1;
	}
	printf("<table border=\"1\">\n");
	printf("<caption>热点车次</caption>\n");
	printf("<tr><th>排名</th><th>车次</th><th>售出票数</th></tr>\n");
	for (r = 0; r < 10; r++) {
		printf("<tr>\n");
		printf("<td>%d</td>\n", r+1);
		val = PQgetvalue(res, r, 0);
		printf("<td>%s</td>\n", val);
		val = PQgetvalue(res, r, 1);
		printf("<td>%s</td>\n", val);
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


