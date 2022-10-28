OBJ = entry.o data.o client_stub.o network_client.o sdmessage.pb-c.o

tree-client: cliente-lib.o tree_client.o
	gcc object/cliente-lib.o object/tree_client.o -lprotobuf-c -o binary/tree-client

cliente-lib.o: $(OBJ)
	ld -r $(addprefix object/,$(OBJ)) -o object/cliente-lib.o

%.o: source/%.c
	gcc $< -c -I include -o object/$@ -g -Wall

clean:
	rm -f object/* binary/*
