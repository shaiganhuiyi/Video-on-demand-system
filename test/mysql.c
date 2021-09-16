#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>

int main(int argc,char *argv[])
{
    //1.初始化句柄
    MYSQL *mysql = mysql_init(NULL);
    if (mysql == NULL){
   printf("mysql init error!\n");
     return -1;
}   
    //2.连接服务器
   if( mysql_real_connect(mysql,"127.0.0.1","root",0,"vod_system",0,NULL,0)==NULL) {
    printf("connect mysql server failed! %s\n",mysql_error(mysql));
    return -1;
                    }
  
    //3.设置字符集
    if(mysql_set_character_set(mysql,"utf8")!=0){
  printf("mysql clien set character failed! %s\n",mysql_error(mysql));
  return -1;
  } 
    //4.选择数据库
    if(mysql_select_db(mysql,"vod_system")!=0){
      printf("select database failed! %s\n",mysql_error(mysql));
       return -1;
      }
    //5.执行语句
    // char *sql="insert table_vod values(null,'秋天','落叶满地',1024,'/video/q.mp4',\'/image/q.jpg',now());";
    // char *sql="update table_vod set vdesc='这是一个科普视频，非常精彩'where id=3;";
     // char *sql="delete from table_vod where id=3;";
    char *sql = "select *from table_vod;";
    if(mysql_query(mysql,sql)!=0){
     printf("query sql failed! %s\n",mysql_error(mysql));
     return -1;
     }
     //6.保存结果集到本地
      MYSQL_RES* res = mysql_store_result(mysql);
      if(res==NULL){
       printf("store result failed! %s\n",mysql_error(mysql));
       return -1;
       }
      int num_row = mysql_num_rows(res);
      int num_col = mysql_num_fields(res);
      int i,j;
      for(i=0;i<num_row;i++){
      MYSQL_ROW row = mysql_fetch_row(res);//res中会保存当前的读取位置，每次向后移动
       for(j=0;j<num_col;j++){
        printf("%s\t",row[j]);
       }
      printf("\n");
     }
     mysql_free_result(res);//释放结果集
    //6.查询-保存结果集-获取结果集条数-获取结果集列数-遍历结果集-释放结果集
    //7.释放句柄：
    return 0;
}
