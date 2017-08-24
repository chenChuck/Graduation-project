ALL:	
#arm-none-linux-gnueabi-gcc $(wildcard *.c) -g -I./include -lpthread -ljpeg -L./include  -static -o run
	gcc $(wildcard *.c) -g -I./include -lpthread -ljpeg -L./include  -static -o run
#cc $(wildcard *.c) -g -I./include -lpthread -L/home/cody/jpeg-8b/lib/ -ljpeg -static -o run
clean:
	rm -rf $(patsubst %.c, %, $(wildcard *.c)) run
#arm-fsl-linux-gnueabi-gcc $(wildcard *.c) -g -I./include -lpthread -L./include -ljpeg -o run
#cc $(wildcard *.c) -g -I./include -lpthread -o run

