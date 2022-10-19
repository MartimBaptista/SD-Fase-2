OBJ_dir = object
OBJ = data.o entry.o serialization.o tree-private.o tree.o
HEADERS_dir = include

%.o: source/%.c
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@ 

#tree.o:
#	cp tree.o $(OBJ_dir)/$@

serialization:
	gcc source/serialization.c -c -I $(HEADERS_dir) -o object/serialization.o 

testData: data.o
	gcc source/test_data.c -c -I $(HEADERS_dir) -o object/test_data.o 
	gcc object/data.o object/test_data.o -o binary/test_data 

testEntry: data.o entry.o
	gcc source/test_entry.c -c -I $(HEADERS_dir) -o object/test_entry.o 
	gcc object/data.o object/entry.o object/test_entry.o -o binary/test_entry 

testTree: data.o entry.o tree.o
	gcc source/test_tree.c -c -I $(HEADERS_dir) -o object/test_tree.o 
	gcc object/data.o object/entry.o object/tree.o object/test_tree.o -o binary/test_tree 
clean:
	rm -f $(OBJ_dir)/* binary/*
