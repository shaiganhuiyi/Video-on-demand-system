#include"db.hpp"
#include"httplib.h"

#define WWW_ROOT "./WWW"
// vurl = /video/test.mp4; 资源的相对根目录就是WWW_ROOT
// 如果获取到了vurl视频的访问路径，则加上相对根目录就是实际存储路径
using namespace httplib;

vod_system:: TableVod *table_vod;

bool WriteFile(const std::string &filename, const std::string filebody)
{
   std:: ofstream ofs(filename, std::ios::binary);
   if (ofs.is_open() == false){
     printf("open file : %s failed!\n",filename.c_str());
     return false;
     }
     ofs.write(filebody.c_str(), filebody.size());
     if (ofs.good() == false){
       printf("write file : %s body faile!\n", filename.c_str());
       return false;
       }
       ofs.close();
       return true;
}
void GetAllVideo(const Request &req,Response &rsp)
{
    //1.从数据库中获取所有的视频元信息
     Json::Value videos;
     bool ret =table_vod->GetAll(&videos);
     if (ret == false){
         printf("get all video info failed!\n");
	 rsp.status = 500;//响应状态码500-服务器内部错误
	 return ;
	 }
    //2.将Json::Value元信息序列化为json字符串
    Json::FastWriter writer;
    std::string body = writer.write(videos);
    //3.将json字符串填充到rsp的正文中
    rsp.set_content(body,"application/json");
    //4.设置rsp的响应状态码
    rsp.status = 200; 
   return ;
}
void GetOneVideo(const Request &req,Response &rsp)
{
     //0.获取指定的视频信息ID /video/(\d+) --括号表示捕捉匹配到的数据
     //req.matches -- 放的都是正则表达式匹配结果
     //  /video/44  /video/(\d+) -> req.matches[0]=/video/44; req.matches[1]=44
     int video_id = std::stoi(req.matches[1]);
     //  //1.从数据库中获取所有的视频元信息
     Json::Value videos;
     bool ret =table_vod->GetOne(video_id,&videos);
     if (ret == false){
         printf("get one video info failed!\n");
	 rsp.status = 500;//响应状态码500-服务器内部错误
	 return ;
	 }
    //2.将Json::Value元信息序列化为json字符串
    Json::FastWriter writer;
    std::string body = writer.write(videos);
    //3.将json字符串填充到rsp的正文中
    rsp.set_content(body,"application/json");
    //4.设置rsp的响应状态码
    rsp.status = 200; 
   return ;
}
void UpdateVideo(const Request &req,Response &rsp)
{
    //1.获取要修改视频元信息的视频id
    int video_id = std::stoi(req.matches[1]);
    //1.修改数据库中的指定id的数据
    //修改后的数据通过正文传递，正文是一个json字符串
    Json::Reader reader;
    Json::Value video;
    bool ret = reader.parse(req.body, video); //json字符串的反序列化 
    if (ret == false){
        printf("update video info format erro\n");
	rsp.status = 400;//客户端错误-坏的请求
        return ;
	}
        ret = table_vod->Update(video_id, video);
	if (ret == false){
	   printf("update video info from database failed!\n");
	   rsp.status = 500;//响应状态码500-服务器内部错误
	   return ;
	   }
        //2.设置状态码 --可以不用设置，rsp默认的状态就是200
        rsp.status = 200;
	return ;
	
}
void DeleteVideo(const Request &req,Response &rsp)
{
   //删除不仅仅是删除元信息，还要删除源文件
   int video_id = std::stoi(req.matches[1]);
   Json::Value video;
   bool ret = table_vod->GetOne(video_id, &video);
   if (ret == false){
      printf("delete-get video info from database failed!\n");
      rsp.status = 500;//响应状态码500-服务器内部错误
      return ;
      }
    std::string video_path = WWW_ROOT + video["vurl"].asString();
    std::string image_path = WWW_ROOT + video["purl"].asString();
    unlink(video_path.c_str());//删除文件
    unlink(image_path.c_str());//删除文件
     ret = table_vod->Delete(video_id);
    if (ret == false){
       printf("delete video info from database failed!\n");
       rsp.status = 500;//响应状态码500-服务器内部错误
       return ;
       }
       rsp.status = 200;
       return;
}
void UploadVideo(const Request &req,Response &rsp)
{ 
  std::string video_name;
  std::string video_desc;
  std::string image_body;
  std::string image_real_name;
  std::string video_body;
  std::string video_real_name;
  if (req.has_file("videoname")){
       //视频名称的输入框数据
       const auto& file = req.get_file_value("videoname");//获取指定的元信息
       //file.filename; file.content;
       video_name = file.content;
       }

   if (req.has_file("descinfo")){
       //视频的描述信息输入框数据
      const auto& file = req.get_file_value("videoname");//获取指定的元信息
      video_desc = file.content;
      }
  
   if (req.has_file("imagefile")){
       //视频的封面图片文件数据
      const auto& file = req.get_file_value("imagefile");//获取指定的元信息
      image_body = file.content;
      image_real_name =file.filename;
      }
   if (req.has_file("videofile")){
       //视频的文件数据
      const auto& file = req.get_file_value("videofile");//获取指定的元信息
      video_body = file.content;
      video_real_name = file.filename;
      }
       //组织视频元信息
      std::string video_url = "/video/" + video_real_name;
      std::string image_url = "/img/" + image_real_name;
      int64_t video_size = video_body.size();
      WriteFile(WWW_ROOT + video_url, video_body);
      WriteFile(WWW_ROOT + image_url, image_body);

      Json::Value video;
      video["name"] = video_name;
      video["vdesc"] = video_desc;
      video["vsize"] =(Json::Value::Int64)video_size;
      video["vurl"] = video_url;
      video["purl"] = image_url;
      bool ret = table_vod->Insert(video);
      if (ret == false){
       printf("insert video info into database failed!\n");
       rsp.status = 500;//响应状态码500-服务器内部错误
       return ;
       }
      return ;
}
 
 
int main(int argc,char*argv[])
{
    MYSQL *mysql = vod_system::MysqlInit();
    table_vod = new vod_system::TableVod(mysql);
    
    Server srv;
    //设置静态资源根目录的目的是，当请求静态文件数据时，httplib会根据访问路径
    //前缀上设置的根目录，找到静态资源文件，读取文件数据返回给客户端
    srv.set_base_dir(WWW_ROOT);//设置静态文件资源的根目录
    //注册请求路由
    
    //所有视频元信息获取
    srv.Get("/video",GetAllVideo);
    //单个视频元信息获取
    //正则表达式；以某种特殊字符表示某种数据规则的方式--以特殊字符匹配符合某种规则的文本
    // \d--匹配数字字符  +-匹配前面的子表式一次或多次 \d+--匹配一个数字
    //(\d+)--使用括号括起来，表示在进行匹配的时候还要进行数据捕捉-单独提取出来
   // R“()"---c++11中的一种用法，表示括号中的字符都是原始字符，去除特殊字符的特殊含义
     srv.Get(R"(/video/(\d+))",GetOneVideo);
    //单个视频元信息修改
    srv.Put(R"(/video/(\d+))",UpdateVideo);
    //单个视频删除
    srv.Delete(R"(/video/(\d+))",DeleteVideo);
    //单个视频上传
    srv.Post("/video",UploadVideo);
    srv.listen("0.0.0.0",9000); 
    vod_system::MysqlRelease(mysql); 
     return 0;

}
