CC = g++
CFLAGS = -g
LDFLAGS = -ljpeg
OBJS = main.o utils.o image.o
TARGETS = main

$(TARGETS): ${OBJS}
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)


clean:
	rm $(OBJS) $(TARGETS)