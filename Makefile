
hello  : server.o client.o lb.o
		g++ -o lb     lb.o -lpthread
		g++ -o server server.o -lpthread

		g++ -o client client.o -lpthread

lb.o: lb.h	
		g++ -c lb.cpp -lpthread

server.o: lb.h lbconfig.h client_config.h server_config.h
		g++ -c server.cpp
client.o: lb.h lbconfig.h client_config.h server_config.h
		g++ -c  client.cpp
read_config.o:
		g++ -c read_config.cpp
server_config.o:
		g++ -c server_config.cpp
clean :
		rm server.o client.o lb.o lb server client
