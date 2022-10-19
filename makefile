Client_dir = Client
Server_dir = Server
Cliente_OBJ = #TODO
Server_OBJ = #TODO
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
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@ -g

clean:
	rm -f $(Client_dir)/$(OBJ_dir)/* $(Client_dir)/binary/* $(Server_dir)/$(OBJ_dir)/* $(Server_dir)/binary/*