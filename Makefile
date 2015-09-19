all: main.cpp Login.o Register.o MWin.o Dialog.o ClientService.o ClientSocket.o Socket.o User.o Date.o Meeting.o
	g++ -std=c++11 -o all main.cpp Login.o Register.o MWin.o Dialog.o ClientService.o ClientSocket.o Socket.o User.o Date.o Meeting.o `pkg-config gtkmm-3.0 --cflags --libs`
Login.o:Login.h Login.cpp
	g++ -c Login.h Login.cpp `pkg-config gtkmm-3.0 --cflags --libs`
Register.o:Register.h Register.cpp
	g++ -c Register.h Register.cpp `pkg-config gtkmm-3.0 --cflags --libs`
MWin.o:MWin.h MWin.cpp
	g++ -c MWin.h MWin.cpp `pkg-config gtkmm-3.0 --cflags --libs`
Dialog.o:Dialog.h Dialog.cpp
	g++ -c Dialog.h Dialog.cpp `pkg-config gtkmm-3.0 --cflags --libs`
ClientService.o:ClientService.h ClientService.cpp
	g++ -std=c++11 -c ClientService.h ClientService.cpp
ClientSocket.o:ClientSocket.h ClientSocket.cpp
	g++ -std=c++11 -c ClientSocket.h ClientSocket.cpp
Socket.o:Socket.h Socket.cpp
	g++ -std=c++11 -c Socket.h Socket.cpp
User.o:User.h User.cpp
	g++ -std=c++11 -c User.h User.cpp
Date.o:Date.h Date.cpp
	g++ -std=c++11 -c Date.h Date.cpp
Meeting.o:Meeting.h Meeting.cpp
	g++ -std=c++11 -c Meeting.h Meeting.cpp

server:server.o ServerService.o ServerSocket.o Socket.o AgendaService.o Storage.o Meeting.o Date.o User.o
	g++ -std=c++11 -o server server.o ServerService.o ServerSocket.o Socket.o AgendaService.o Storage.o Meeting.o Date.o User.o
server.o:server.cpp
	g++ -std=c++11 -c server.cpp
ServerService.o:ServerService.h ServerService.cpp
	g++ -std=c++11 -c ServerService.h ServerService.cpp
ServerSocket.o:ServerSocket.h ServerSocket.cpp
	g++ -std=c++11 -c ServerSocket.h ServerSocket.cpp
AgendaService.o:AgendaService.h AgendaService.cpp
	g++ -std=c++11 -c AgendaService.h AgendaService.cpp
Storage.o:Storage.h Storage.cpp
	g++ -std=c++11 -c Storage.h Storage.cpp

clean:
	rm *.o client server *.gch