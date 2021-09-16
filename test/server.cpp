#include"httplib.h"

using namespace httplib;

void HelloWorld(const Request &req, Response &rsp)
{
   std::cout << req.body << std:: endl;//打印请求正文 
   std::cout << req.method << std::endl;//打印请求方法
   std::cout << req.path << std::endl;//打印请求的资源路径
   for(auto it = req.headers.begin();it != req.headers.end(); ++it){
    std::cout << it ->first << "=" << it->second <<std::endl;//遍历请求头部信息
   }
   std::string rsp_body= "<html><body><h1>Hello 56班</h1></body></html>";
   rsp.set_content(rsp_body,"text/heml");//给与正文信息
   rsp.status=200;//设置状态响应码
   return ;   
}
int main(int argc, char *argb[])
{


   Server server;
   server.Get("/", HelloWorld);//注册请求路由--告诉httplib什么请求使用哪个函数进行处理。
   server.listen("0.0.0.0",9000);//搭建服务器，当收到请求，就会使用对应的函数处理 
   return 0;
}
