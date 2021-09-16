server:server.cpp
	g++ -std=c++11 $^ -o $@ -lpthread
mysql:mysql.c
	gcc mysql.c -o mysql -L/usr/lib64/mysql -lmysqlclient
json:json.cpp
	g++ json.cpp -o json -std=c++11 -ljsoncpp 
