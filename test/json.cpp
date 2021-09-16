#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>

int main(int argc,char *argv[] )
{
    //1.多个数据对象组织成为json格式的字符串--序列化
   Json::Value root;
   root["姓名"]="张三";
   root["年龄"]=18; 
   root["成绩"].append(33);
   root["成绩"].append(44);
   root["成绩"].append(55);
   Json::StyledWriter writer; 
   std::cout<< writer.write(root)<< std::endl;
   //2. json格式的字符串解析成为多个数据对象-反序列化
   std::string buf = R"( {"名称": "后羿", "类型": "射手", "三围": [285, 86, 73]})";
   Json::Value val;
   Json::Reader reader;
   bool ret = reader.parse(buf,val);
  if(ret == false){
     printf("parse error\n");
     return -1;
     }
 std::cout<<"人物: "<<val["名称"].asString()<<std::endl;
 std::cout<<"角色: "<<val["类型"].asString()<<std::endl;
 int num = val["三围"].size();
 for (int i=0;i<num;i++){
     std::cout<<"属性: "<<val["三围"][i].asInt()<<std::endl;
      }

    return 0;
}



