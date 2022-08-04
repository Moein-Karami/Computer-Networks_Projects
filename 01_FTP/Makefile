inc_dir = ./include
src_dir = ./src
build_dir = ./build
cfalgs = -std=c++11
cc = g++

All: Server Client

Server: ${build_dir}/CommandHandler.o ${build_dir}/server.o ${build_dir}/ServerClient.o ${build_dir}/socket_tool.o ${build_dir}/User.o
	${cc} ${cfalgs} ${build_dir}/CommandHandler.o ${build_dir}/server.o ${build_dir}/ServerClient.o ${build_dir}/socket_tool.o ${build_dir}/User.o -o Server.out

${build_dir}/server.o: ${inc_dir}/defines_and_libs.hpp ${inc_dir}/User.hpp ${inc_dir}/ServerClient.hpp ${inc_dir}/server.hpp ${inc_dir}/socket_tool.hpp ${inc_dir}/json.hpp ${inc_dir}/CommandHandler.hpp ${src_dir}/server.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/server.cpp -o ${build_dir}/server.o

${build_dir}/CommandHandler.o: ${inc_dir}/defines_and_libs.hpp ${inc_dir}/User.hpp ${inc_dir}/ServerClient.hpp ${inc_dir}/CommandHandler.hpp ${src_dir}/CommandHandler.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/CommandHandler.cpp -o ${build_dir}/CommandHandler.o

${build_dir}/ServerClient.o: ${inc_dir}/defines_and_libs.hpp ${inc_dir}/User.hpp ${inc_dir}/ServerClient.hpp ${src_dir}/ServerClient.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/ServerClient.cpp -o ${build_dir}/ServerClient.o

${build_dir}/socket_tool.o: ${inc_dir}/defines_and_libs.hpp ${inc_dir}/socket_tool.hpp ${src_dir}/socket_tool.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/socket_tool.cpp -o ${build_dir}/socket_tool.o

${build_dir}/User.o: ${inc_dir}/defines_and_libs.hpp ${inc_dir}/User.hpp ${src_dir}/User.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/User.cpp -o ${build_dir}/User.o

Client: ${build_dir}/client.o ${build_dir}/socket_tool.o
	${cc} ${cfalgs} ${build_dir}/client.o ${build_dir}/socket_tool.o -o Client.out

${build_dir}/client.o: ${inc_dir}/defines_and_libs.hpp ${inc_dir}/socket_tool.hpp ${inc_dir}/json.hpp ${src_dir}/client.cpp
	${cc} ${cfalgs} -c -I ${inc_dir} ${src_dir}/client.cpp -o ${build_dir}/client.o

clear:
	rm Client.out Server.out ${build_dir}/*