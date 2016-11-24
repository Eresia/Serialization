SOURCES= $(wildcard *$(srcExt))

OBJECTS= $(SOURCES:$(srcExt)=.o)

OBJECTS_PATH=$(foreach obj, $(OBJECTS), $(objectDir)/$(obj))

all: $(OBJECTS_PATH)

$(objectDir)/%.o: %$(srcExt) $(headers)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm $(OBJECTS_PATH);
