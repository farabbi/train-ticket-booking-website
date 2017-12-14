import java.io.File;
import java.sql.*;
public class ScanCSVtoDB {

	/**
	 * @param args
	 * @throws ClassNotFoundException 
	 * @throws SQLException 
	 */
	public static void main(String[] args) throws ClassNotFoundException, SQLException {
		// TODO Auto-generated method stub
		String path="/home/zengkkkai/Documents/database/train-2016-10/z";
		File file=new File(path);
		String[] array=file.list();
		for(int i=0;i<array.length;i++){
			
			array[i]=array[i].substring(0, array[i].length()-4);
			
			System.out.println(array[i]);
		}
		Class.forName("org.postgresql.Driver");
		Connection connection = null;
		Statement stmt=null;
		connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/zengkkkai?characterEncoding=utf8","postgres","postgres");
		StringBuffer sb=new StringBuffer();
		stmt=connection.createStatement();
		for(int i=0;i<array.length;i++){
			sb.setLength(0);
			sb.append("insert into train (t_trainid) values (");
			sb.append("'"+array[i]+"')");
			stmt.addBatch(sb.toString());
		}
		stmt.executeBatch();
		connection.close();
	}

}
