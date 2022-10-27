Client_dir = Client
Server_dir = Server
OBJ_dir = object
HEADERS_dir = include


cliente-lib.o: cliente_stub.c network-cliente.c data.c entry.c
	#gcc -r ld -o cliente-lib

tree-server:
	#TODO

tree-client:
#cd $(Client_dir)
	gcc $(Client_dir)/source/tree_client.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/tree-client.o -g -Wall
	gcc $(Client_dir)/source/client_stub.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/client_stub.o -g -Wall
	gcc $(Client_dir)/source/network_client.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/network_client.o -g -Wall
	gcc $(Client_dir)/source/sdmessage.pb-c.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/sdmessage.pb-c.o -g -Wall
	gcc $(Client_dir)/source/entry.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/entry.o -g -Wall
	gcc $(Client_dir)/source/data.c -c -I $(Client_dir)/$(HEADERS_dir) -o $(Client_dir)/$(OBJ_dir)/data.o -g -Wall

	gcc $(Client_dir)/$(OBJ_dir)/entry.o $(Client_dir)/$(OBJ_dir)/data.o $(Client_dir)/$(OBJ_dir)/tree-client.o $(Client_dir)/$(OBJ_dir)/client_stub.o $(Client_dir)/$(OBJ_dir)/network_client.o $(Client_dir)/$(OBJ_dir)/sdmessage.pb-c.o -lprotobuf-c -o $(Client_dir)/binary/tree-client

tree-server:
	gcc $(Server_dir)/source/tree_server.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/tree-server.o -g -Wall
	gcc $(Server_dir)/source/tree_skel.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/tree_skel.o -g -Wall
	gcc $(Server_dir)/source/network_server.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/network_server.o -g -Wall
	gcc $(Server_dir)/source/sdmessage.pb-c.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/sdmessage.pb-c.o -g -Wall
	gcc $(Server_dir)/source/entry.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/entry.o -g -Wall
	gcc $(Server_dir)/source/data.c -c -I $(Server_dir)/$(HEADERS_dir) -o $(Server_dir)/$(OBJ_dir)/data.o -g -Wall
	cp $(Server_dir)/extra_obj/tree.o.extra $(Server_dir)/$(OBJ_dir)/tree.o

	gcc $(Server_dir)/$(OBJ_dir)/entry.o $(Server_dir)/$(OBJ_dir)/data.o $(Server_dir)/$(OBJ_dir)/tree.o $(Server_dir)/$(OBJ_dir)/tree-server.o $(Server_dir)/$(OBJ_dir)/tree_skel.o $(Server_dir)/$(OBJ_dir)/network_server.o $(Server_dir)/$(OBJ_dir)/sdmessage.pb-c.o -lprotobuf-c -o $(Server_dir)/binary/tree-server

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
