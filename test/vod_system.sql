drop database if exists vod_system;

create database if not exists vod_system;

use vod_system;

create table if not exists table_vod(
  id int primary key auto_increment,
  name varchar(255) comment "视频名称",
  vdesc varchar(255) comment "视频描述",
  vsize bigint  comment "视频大小",
  vurl varchar(255) comment "视频文件访问路径",
  purl varchar(255) comment "封面图访问路径",
  ultime datetime  comment "上传时间");

insert table_vod values
(null,"美女与帅哥.mp4","非常好看",10240,"/video/test.mp4","/image/test.png",now()),
(null,"夏天与冬天.mp4","非常好看",20240,"/video/tmp.mp4","/image/tmp.png",now());
 
