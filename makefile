Client_dir = Client
Server_dir = Server
OBJ_dir = object
HEADERS_dir = include


cliente-lib.o: cliente_stub.c network-cliente.c data.c entry.c
	#gcc -r ld -o cliente-lib

tree-server:
	#TODO

tree-client:
	#TODO


tree.o:
	cp extra_object/tree.o.extra $(OBJ_dir)/tree.o

%.o: source/%.c
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@ -g -Wall

clean:
	rm -f $(Client_dir)/$(OBJ_dir)/* $(Client_dir)/binary/* $(Server_dir)/$(OBJ_dir)/* $(Server_dir)/binary/*

#temp
test_client:
	gcc testing_stuff/test_client.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/test_client.o -g -Wall
	gcc $(Client_dir)/source/network_client.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/network_client.o -g -Wall
	gcc -o testing_stuff/test_client $(Client_dir)/$(OBJ_dir)/test_client.o $(Client_dir)/$(OBJ_dir)/network_client.o

test_server:
	gcc testing_stuff/test_server.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/test_server.o -g -Wall
	gcc $(Server_dir)/source/network_server.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/network_server.o -g -Wall
	gcc -o testing_stuff/test_server $(Server_dir)/$(OBJ_dir)/test_server.o $(Server_dir)/$(OBJ_dir)/network_server.o
