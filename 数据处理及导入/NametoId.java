import java.io.File;
import java.sql.*;
public class NametoId {

	/**
	 * @param args
	 * @throws ClassNotFoundException 
	 * @throws SQLException 
	 */
	public static void main(String[] args) throws ClassNotFoundException, SQLException {
		// TODO Auto-generated method stub
	
		Class.forName("org.postgresql.Driver");
		Connection connection = null;
		Statement stmt=null;
		connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/zengkkkai?characterEncoding=utf8","postgres","postgres");
		StringBuffer sb=new StringBuffer();
		stmt=connection.createStatement();
		sb.setLength(0);
		sb.append("update infor set i_tid=(select t_tid from train where train.t_name = infor.i_tname)");
		stmt.addBatch(sb.toString());
		stmt.executeBatch();
		stmt.close();
		connection.close();
	}

}
