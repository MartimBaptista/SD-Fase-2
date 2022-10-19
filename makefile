OBJ_dir = obj
OBJ = #TODO
HEADERS_dir = include

tree.o:
	cp extra_obj/tree.o.extra $(OBJ_dir)/tree.o

%.o: src/%.c
	gcc $< -c -I $(HEADERS_dir) -o $(OBJ_dir)/$@ -g

clean:
	rm -f $(OBJ_dir)/* bin/*