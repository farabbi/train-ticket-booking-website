#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <stdlib.h>
#include <string.h>

void exitNicely(PGconn* conn)
{ PQfinish(conn);exit(1);}

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
   // char *startname="太原";
  //  char *endname="兰州";
//printf("%s:%s<br />%s:%s<br />", input[0].name, input[0].value, input[1].name, input[1].value);
 //   printf("strcmp:%d", strcmp(input[0].value, "太原"));
    
    char *getvalue[12];
    int flags[12];
	int count;
    for(count=0;count<12;count++){
    	flags[count]=0;
    }
    for(count=3;count<14;count++){
    	getvalue[count-3]=input[count].value;
    }
    for(count=0;count<12;count++){
    	if(!strcmp(getvalue[count],"yz"))
    		flags[0]=1;
    	if(!strcmp(getvalue[count],"rz"))
    		flags[1]=1;
    	if(!strcmp(getvalue[count],"yws"))
    		flags[2]=1;
    	if(!strcmp(getvalue[count],"ywz"))
    		flags[3]=1;
    	if(!strcmp(getvalue[count],"ywx"))
    		flags[4]=1;
    	if(!strcmp(getvalue[count],"rws"))
    		flags[5]=1;
    	if(!strcmp(getvalue[count],"rwx"))
    		flags[6]=1;
    	if(!strcmp(getvalue[count],"price"))
    		flags[7]=1;
    	if(!strcmp(getvalue[count],"traveltime"))
    		flags[8]=1;
    	if(!strcmp(getvalue[count],"starttime"))
    		flags[9]=1;
    	if(!strcmp(getvalue[count],"oneway"))
    		flags[10]=1;
    	if(!strcmp(getvalue[count],"return"))
    		flags[11]=1;
    }	
//printf("flags[10]:%d<br />flags[11]:%d<br />", flags[10], flags[11]);
    char *param_val[3];
    
    param_val[0]=input[0].value;
    param_val[1]=input[1].value;
    param_val[2]=input[2].value;
    int param_len[3];
    param_len[0]=0;
    param_len[1]=0;
    param_len[2]=0;
    //param_len[2]=0;
    int param_format[3];
    param_format[0]=0;
    param_format[1]=0;
    param_format[2]=0;
 
	PGconn* conn=PQconnectdb("dbname=lab2");
	if(PQstatus(conn)!=CONNECTION_OK){
		fprintf(stderr,"connection to db failed: %s",PQerrorMessage(conn));
		exitNicely(conn);
	}


PGresult *res1;
PGresult *res2;

int r,l;
char *val=NULL;
char *valtemp=NULL;
if(flags[10]==1){
	if(flags[0]==1){
//printf("9");
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[1]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_rzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[2]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_ywsprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[3]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_ywzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[4]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_ywxprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[5]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_rwsprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[6]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_rwxprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
}
if(flags[11]==1){
	if(flags[0]==1){
//printf("0");
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[1]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_rzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[2]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_ywsprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[3]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_ywzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[4]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_ywxprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[5]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_rwsprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    if(flags[6]==1){
		res1=PQprepare(conn,"once","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_rwxprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$1) intersect (select i_tid from infor where i_sid =$2)) and i1.i_sid =$1 and i2.i_sid =$2 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
	   res1=PQexecPrepared(conn,"once",2,(const char**)param_val,param_len,param_format,0);
		

		ExecStatusType status=PQresultStatus(res1);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
		    fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res1));
		    exitNicely(conn);
		}
    }
    	if(flags[0]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    if(flags[1]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    if(flags[2]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    if(flags[3]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    if(flags[4]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    if(flags[5]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    if(flags[6]==1){
		    res2=PQprepare(conn,"once2","select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,i3.i_attime,i3.i_dprice,i3.i_aprice  from (select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,i1.i_dtime as i_dtime,i2.i_atime as i_atime,i1.i_yzprice as i_dprice,i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice from infor as i1,infor as i2 where i1.i_tid=i2.i_tid and i1.i_tid in ((select i_tid from infor where i_sid =$2) intersect (select i_tid from infor where i_sid =$1)) and i1.i_sid =$2 and i2.i_sid =$1 and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 where i3.i_tid=train.t_tid and i3.i_dsid=s1.s_sid and i3.i_asid=s2.s_sid order by i3.i_dtime",2,NULL);
   res2=PQexecPrepared(conn,"once2",2,(const char**)param_val,param_len,param_format,0);
    

    ExecStatusType status=PQresultStatus(res2);
    if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_SINGLE_TUPLE)){
        fprintf(stderr,"failed execution: %s\n",PQresultErrorMessage(res2));
        exitNicely(conn);
    }
    }
    

}	
int num_rows1=PQntuples(res1);
int num_cols1=PQnfields(res1);
int num_rows2=PQntuples(res2);
int num_cols2=PQnfields(res2);
	printf("<table border=\"1\">\n");
	printf("<tr><th>车次号</th><th>出发站</th><th>到达站</th><th>出发时间</th><th>行程时间</th><th>票价</th>\n");
//printf("1");
	for(r=0;r<num_rows1;r++){
//printf("2");
		printf("<tr>\n");
		val=PQgetvalue(res1,r,0);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res1,r,1);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res1,r,2);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res1,r,3);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res1,r,5);
		valtemp=PQgetvalue(res1,r,6);
		int a=atoi(val);
		int b=atoi(valtemp);
		int c=b-a;
		printf("<td>%d</td>\n",c);
		val=PQgetvalue(res1,r,7);
		valtemp=PQgetvalue(res1,r,8);
		a=atoi(val);
		b=atoi(valtemp);
		c=b-a;
		printf("<td>%d</td>\n",c);
		printf("</tr>\n");
	}
	if(flags[11]==1){
		for(r=0;r<num_rows2;r++){
		printf("<tr>\n");
		val=PQgetvalue(res2,r,0);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res2,r,1);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res2,r,2);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res2,r,3);
		printf("<td>%s</td>\n",val);
		val=PQgetvalue(res2,r,5);
		valtemp=PQgetvalue(res2,r,6);
		int a=atoi(val);
		int b=atoi(valtemp);
		int c=b-a;
		printf("<td>%d</td>\n",c);
		val=PQgetvalue(res2,r,7);
		valtemp=PQgetvalue(res2,r,8);
		a=atoi(val);
		b=atoi(valtemp);
		c=b-a;
		printf("<td>%d</td>\n",c);
		printf("</tr>\n");
		}
	}
	printf("</table>\n");
	PQclear(res1);
	PQclear(res2);
	
 /*   printf("<html>\n");
    printf("<head><meta charset='utf-8' /></head>\n");
    printf("<body>\n");
	PQprintOpt po={0};
    po.header=1;po.align=1;po.fieldSep="|";
    po.html3=1;po.caption="train";po.tableOpt="border=1";
    PQprint(stdout,res,&po);
    PQclear(res);
    printf("</body></html>\n");*/
//	printf("connection is successful!\n");
	printf("<hr />\n");
	printf("请查询车次具体信息进行预定:<br />\n");
	printf("<form action=\"/cgi-bin/search_traininfo.cgi\" method=\"POST\">\n");
	printf("车次:<br />\n");
	printf("<input type=\"text\" name=\"trainid\"><br />\n");
	printf("日期(yyyy-mm-dd):<br />\n");
	printf("<input type=\"text\" name=\"date\"><br />\n");
	printf("<br />\n");
	printf("<input type=\"submit\" value=\"查询\"><br />\n");
	printf("</form>\n");
	printf("<hr />\n");
	printf("<a href=\"/index.html\">返回主页</a>\n");




	PQfinish(conn);
	return 0;
}
