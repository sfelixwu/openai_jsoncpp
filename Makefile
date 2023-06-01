
# Makefile for s2023, ecs36b
# https://macappstore.org/libjson-rpc-cpp/
# https://github.com/cinemast/libjson-rpc-cpp
# for ecs36b openai, using JSON-CPP

CC = g++ -std=c++14
CFLAGS = -g -D_ECS36B_JSONCPP_
# CFLAGS = -g -D_ECS36B_JSONCPP_ -D_ECS36B_VERBOSE_

LDFLAGS = 	-L/opt/homebrew/lib/ -ljsoncpp			\
		-lmicrohttpd -ljsonrpccpp-common		\
		-ljsonrpccpp-server -ljsonrpccpp-client -lcurl

INC	=	ecs36b_Common.h ecs36b_Exception.h ecs36b_openai.hpp

INC_CL	=	JvTime.h
OBJ	=	JvTime.o ecs36b_JSON.o

# rules.
all: 	ecs36b_openai_test_00 ecs36b_openaiclient ecs36b_openaiserver ecs36b_contrary

#
#
# <target> : [... tab...] <dependency>
# [... tab ....] <action>

ecs36b_openaiclient.h:		ecs36b_openai_jsonrpc.json
	jsonrpcstub ecs36b_openai_jsonrpc.json --cpp-server=ecs36b_openAIServer --cpp-client=ecs36b_openAIClient

ecs36b_openaiserver.h:		ecs36b_openai_jsonrpc.json
	jsonrpcstub ecs36b_openai_jsonrpc.json --cpp-server=ecs36b_openAIServer --cpp-client=ecs36b_openAIClient

ecs36b_openaiclient.o:		ecs36b_openaiclient.cpp ecs36b_openaiclient.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) ecs36b_openaiclient.cpp

ecs36b_contrary.o:		ecs36b_contrary.cpp ecs36b_openaiclient.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) ecs36b_contrary.cpp

ecs36b_openaiserver.o:		ecs36b_openaiserver.cpp ecs36b_openaiserver.h $(INC_CL) $(INC)
	$(CC) -c $(CFLAGS) ecs36b_openaiserver.cpp

ecs36b_openaiclient:	ecs36b_openaiclient.o $(OBJ)
	$(CC) -o ecs36b_openaiclient ecs36b_openaiclient.o $(OBJ) $(LDFLAGS)

ecs36b_contrary:	ecs36b_contrary.o $(OBJ)
	$(CC) -o ecs36b_contrary ecs36b_contrary.o $(OBJ) $(LDFLAGS)

ecs36b_openaiserver:	ecs36b_openaiserver.o $(OBJ)
	$(CC) -o ecs36b_openaiserver ecs36b_openaiserver.o $(OBJ) $(LDFLAGS)

ecs36b_JSON.o:		ecs36b_JSON.cpp $(INC)
	$(CC) -c $(CFLAGS) ecs36b_JSON.cpp

JvTime.o:	JvTime.cpp JvTime.h $(INC)
	$(CC) -c $(CFLAGS) JvTime.cpp

ecs36b_openai_test_00.o:	ecs36b_openai_test_00.cpp $(INC)
	$(CC) -c $(CFLAGS) ecs36b_openai_test_00.cpp

ecs36b_openai_test_00:	ecs36b_openai_test_00.o $(OBJ)
	$(CC) -o ecs36b_openai_test_00 ecs36b_openai_test_00.o $(OBJ) $(LDFLAGS)

clean:
	rm -f *.o *~ core ecs36b_openai_test_00 ecs36b_openaiclient ecs36b_openaiserver ecs36b_openaiclient.h ecs36b_openaiserver.h ecs36b_contrary



