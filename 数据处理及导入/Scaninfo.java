import java.io.*;
import java.sql.*;
public class Scaninfo {

	/**
	 * @param args
	 * @throws ClassNotFoundException 
	 * @throws SQLException 
	 * @throws IOException 
	 */
	public static void main(String[] args) throws ClassNotFoundException, SQLException, IOException {
		// TODO Auto-generated method stub
		String path="/home/zengkkkai/Documents/database/train-2016-10/0";
		File file=new File(path);
		String[] name=file.list();
		File[] array=file.listFiles();
		Class.forName("org.postgresql.Driver");
		Connection connection = null;
		Statement stmt=null;
		connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/zengkkkai?characterEncoding=utf8","postgres","postgres");
		
		for(int i=0;i<array.length;i++){
			File files=new File(array[i].toString());
			System.out.println(array[i].toString());
			FileInputStream fin=new FileInputStream(files);
			InputStreamReader rd=new InputStreamReader(fin);
			BufferedReader bf=new BufferedReader(rd);
			String info=bf.readLine();
			StringBuffer sb=new StringBuffer();
			stmt=connection.createStatement();
			while((info=bf.readLine())!=null){
				String[] infom=info.split(",");
			//	String[] a=new String[infom.length];String[] b=new String[infom.length];
//				for(int j=0;i<infom.length;j++){
//					//a[j]=infom[j].replace(" ","");
//					b[j]=infom[j].replace("-","0").replace(" ", "");
//					System.out.println(b[j]);
//				}
				infom[7]=infom[7].replace(" ", "");
				infom[8]=infom[8].replace(" ", "");
				infom[9]=infom[9].replace(" ", "");
				if(infom[7].equals("/")) infom[7]="0/0";
				else if(infom[7].equals("//")) infom[7]="0/0/0";
				if(infom[8].equals("/")) infom[8]="0/0";
				else if(infom[8].equals("//")) infom[8]="0/0/0";
				if(infom[9].equals("/")) infom[9]="0/0";
				else if(infom[9].equals("//")) infom[9]="0/0/0";
				String[] arr1=infom[7].replace("-","0").split("/");
				String[] arr2=infom[8].replace("-","0").split("/");
				String[] arr3=infom[9].replace("-","0").split("/");
				String[] temp1=new String[2];
				String[] temp2=new String[3];
				String[] temp3=new String[2];
				if(arr1.length==1){
					temp1[0]=arr1[0];
					temp1[1]=arr1[0];
					if(arr2.length==3){
						temp2[0]=arr2[0];
						temp2[1]=arr2[1];
						temp2[2]=arr2[2];
					}else{
						temp2[0]=arr2[0];
						temp2[1]=arr2[0];
						temp2[2]=arr2[0];
					}
					if(arr3.length==2){
						temp3[0]=arr3[0];
						temp3[1]=arr3[1];
					}else{
						temp3[0]=arr3[0];
						temp3[1]=arr3[0];
					}
				}else if(arr1.length==2){
					temp1[0]=arr1[0];
					temp1[1]=arr1[1];
					if(arr2.length==3){
						temp2[0]=arr2[0];
						temp2[1]=arr2[1];
						temp2[2]=arr2[2];
					}else{
						temp2[0]=arr2[0];
						temp2[1]=arr2[0];
						temp2[2]=arr2[0];
					}
					if(arr3.length==2){
						temp3[0]=arr3[0];
						temp3[1]=arr3[1];
					}else{
						temp3[0]=arr3[0];
						temp3[1]=arr3[0];
					}
				}else if(arr1.length==3){
					if(arr3.length==2){
						temp1[0]=arr3[0];
						temp1[1]=arr3[1];
					}else{
						temp1[0]=arr3[0];
						temp1[1]=arr3[0];
					}
					temp2[0]=arr1[0];
					temp2[1]=arr1[1];
					temp2[2]=arr1[2];
					if(arr2.length==2){
						temp3[0]=arr2[0];
						temp3[1]=arr2[1];
					}else{
						temp3[0]=arr2[0];
						temp3[1]=arr2[0];
					}
				}
				
					sb.setLength(0);
					sb.append("insert into infor (i_tname,i_sorder,i_sname,i_atime,i_dtime,i_stay,i_ttime,i_distance,i_yzprice,i_rzprice,i_ywsprice,i_ywzprice,i_ywxprice,i_rwsprice,i_rwxprice) values (");
					sb.append("'"+name[i].replace(".csv", "")+"','"+infom[0].replace("-","0").replace(" ", "")+"','"+infom[1].replace("-","0").replace(" ", "")+"','"+infom[2].replace("-","0").replace(" ", "")+"','"+infom[3].replace("-","0").replace(" ", "")+"','"+infom[4].replace("-","0").replace(" ", "")+"','"+infom[5].replace("-","0").replace(" ", "")+"','"+infom[6].replace("-","0").replace(" ", "")+"','"+temp1[0]+"','"+temp1[1]+"','"+temp2[0]+"','"+temp2[1]+"','"+temp2[2]+"','"+temp3[0]+"','"+temp3[1]+"')");
					stmt.addBatch(sb.toString());
				
			}
			stmt.executeBatch();
			bf.close();
		}
			connection.close();

		
	}

}
