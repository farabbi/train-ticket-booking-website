insert into passenger(p_name, p_idcard, p_tele, p_credit, p_bank) 
	values ($1, $2, $3, $4, $5);

select i_sorder, s_name, i_atime, i_dtime, i_yzprice, r_yzleft, i_rzprice, 
	r_rzleft, i_ywsprice, r_ywsleft, i_ywzprice, r_ywzleft, i_ywxprice, 
	r_ywxleft, i_rwsprice, r_rwsleft, i_rwxprice, r_rwxleft 
from station, infor, remaining, train 
where i_sid=r_sid and i_tid=r_tid and i_sid=s_sid and i_tid=t_tid and 
	t_name=$1 and r_time=$2 
order by i_sorder;


select t_tid from train where t_name=$1;

select i_sid, i_yzprice, i_rzprice, i_ywsprice, i_ywzprice, i_ywxprice, 
	i_rwsprice, i_rwxprice 
from infor 
where i_tid=$1 and i_sorder=$2;

select count(distinct o_oid) from orders;

select p_pid from passenger where p_idcard=$1;

insert into orders(o_time, o_pid, o_tid, o_dsid, o_asid, o_oid, o_type, o_price) 
	values($1, $2, $3, $4, $5, $6, $7, $8);


select o_oid, t_tid, start.s_name, dest.s_name, o_type, o_price, o_time, i_dtime 
from orders, train, station start, station dest, infor 
where o_oid=$1 and o_tid=t_tid and o_dsid=start.s_sid and o_asid=dest.s_sid and 
	o_tid=i_tid and o_dsid=i_sid;

select o_tiid, o_oid, o_time, i_dtime, start.s_name, dest.s_name, o_price 
from orders, station start, station dest, infor 
where o_time between $1 and $2 and o_tid=i_tid and o_dsid=i_sid and 
	o_dsid=start.s_sid and o_asid=dest.s_sid 
order by o_oid, o_tiid;

delete from orders where o_oid=$1;

select count(distinct o_oid) from orders;

select sum(o_price) from orders;

select count(*) from orders;

select t_name, count(*) as number 
from train, orders 
where t_tid=o_tid 
group by t_name 
order by number desc;


select td.t_name,ta.t_name,s_name 
from 
	(select i3.t1 as i_dtid,i3.t2 as i_atid,i3.i_sid as i_zzsid 
	 from 
	 	(select i1.i_tid as t1,i2.i_tid as t2,i2.i_sid 
	 	 from 
	 	 	(select * 
	 	 	from infor 
	 	 	where i_tid in 
	 	 		(select i_tid 
	 	 		from infor 
	 	 		where i_sid=$1)) i1,
	 		(select * 
	 		from infor 
	 		where i_tid in 
	 			(select i_tid 
	 			from infor 
	 			where i_sid=$2)) i2 
	 	where i1.i_sid=i2.i_sid) i3 
	 where 
	 	(select i_sorder 
	 	from infor 
	 	where i_tid=i3.t1 and i_sid=i3.i_sid)>(select i_sorder 
	 											from infor 
	 											where i_tid=i3.t1 
	 												and i_sid=$1) 
	 		  and (select i_sorder 
	 		  		from infor 
	 		  		where i_tid=i3.t2 and i_sid=i3.i_sid)<(select i_sorder 
	 		  												from infor 
	 		  												where i_tid=i3.t2 
	 		  													and i_sid=$2) 
	 		  and i3.t1!=i3.t2 and i3.i_sid!=$1 and i3.i_sid!=$2 
	 	group by i3.t1,i3.t2,i3.i_sid) i4,train as td,train as ta,station 
where td.t_tid=i4.i_dtid 
		and ta.t_tid=i4.i_atid 
		and s_sid=i4.i_zzsid;



select t_name,s1.s_name,s2.s_name,i3.i_dtime,i3.i_atime,i3.i_dttime,
	i3.i_attime,i3.i_dprice,i3.i_aprice  
from 
	(select i1.i_tid as i_tid,i1.i_sid as i_dsid,i2.i_sid as i_asid,
		i1.i_dtime as i_dtime,i2.i_atime as i_atime, i1.i_yzprice as i_dprice,
		i1.i_ttime as i_dttime,i2.i_ttime as i_attime,i2.i_yzprice as i_aprice 
	from infor as i1,infor as i2 
	where i1.i_tid=i2.i_tid 
		and i1.i_tid in 
			((select i_tid 
			from infor 
			where i_sid =$2) 
			intersect 
			(select i_tid 
			from infor 
			where i_sid =$1)) 
		and i1.i_sid =$2 
		and i2.i_sid =$1 
		and i1.i_sorder<i2.i_sorder) i3,train,station as s1,station as s2 
where i3.i_tid=train.t_tid 
	and i3.i_dsid=s1.s_sid 
	and i3.i_asid=s2.s_sid 
order by i3.i_dtime;

