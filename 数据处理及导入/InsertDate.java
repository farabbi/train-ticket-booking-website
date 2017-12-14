import java.sql.*;
public class InsertDate {

	/**
	 * @param args
	 * @throws ClassNotFoundException 
	 * @throws SQLException 
	 */
	public static void main(String[] args) throws ClassNotFoundException, SQLException {
		// TODO Auto-generated method stub
		Class.forName("org.postgresql.Driver");
		Connection conn=null;
		conn=DriverManager.getConnection("jdbc:postgresql://localhost:5432/zengkkkai?characterEncoding=utf8","postgres","postgres");
		Statement stmt=null;
		stmt=conn.createStatement();
		StringBuffer a=new StringBuffer();
		a.setLength(0);
//		a.append("insert into remaining (r_time,r_tid,r_sid,r_yzleft,r_rzleft,r_ywsleft,r_ywzleft,r_ywxleft,r_rwsleft,r_rwxleft) values ");
//		a.append("('2016-11-24','(select i_tid from infor)','(select i_sid from infor)','5','5','5','5','5','5','5')");
		//a.append("insert into remaining (r_tid,r_sid) select i_tid,i_sid from infor");
		//a.append("update remaining set r_time='2016-11-30',r_yzleft=5,r_rzleft=5,r_ywsleft=5,r_ywzleft=5,r_ywxleft=5,r_rwsleft=5,r_rwxleft=5 where r_time is null");
		//a.append("update remaining set r_yzleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_yzprice=0");
		
		//a.append("update remaining set r_rzleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_rzprice=0");
		
		//a.append("update remaining set r_ywsleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_ywsprice=0");
		
		//a.append("update remaining set r_ywzleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_ywzprice=0");
		
		//a.append("update remaining set r_ywxleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_ywxprice=0");
		
		//a.append("update remaining set r_rwsleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_rwsprice=0");
		
		a.append("update remaining set r_rwxleft=0 from infor where r_tid=i_tid and r_sid=i_sid and i_rwxprice=0");

		stmt.addBatch(a.toString());
		stmt.executeBatch();
		stmt.close();
		conn.close();
	}

}
