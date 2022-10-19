OBJ_dir = obj
OBJ = data.o entry.o serialization.o tree-private.o tree.o
HEADERS_dir = include

idkname: $(OBJ)
	gcc $(addprefix $(OBJ_dir)/,$(OBJ)) -o bin/idkname -lrt -g

tree.o:
	cp extra_obj/tree.o.extra $(OBJ_dir)/tree.o

%.o: src/%.c
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@ -g

clean:
	rm -f $(OBJ_dir)/* bin/*