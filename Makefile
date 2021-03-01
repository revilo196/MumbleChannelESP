OBJS	= .pio/build/native/test/test_mumble_data.o .pio/build/native/lib/MumbleBase/mumble_data.o .pio/build/native/lib/MumbleBase/mumble.pb.o .pio/build/native/lib/unity.o  .pio/build/native/lib/pb_common.o
SOURCE	= test/test_mumble_data.cpp lib/MumbleBase/mumble_data.cpp lib/MumbleBase/mumble.pb.c /home/oliverw/.platformio/packages/tool-unity/unity.c .pio/libdeps/nodemcu-32s/Nanopb/pb_common.c
HEADER	= lib/MumbleBase/mumble_base.h lib/MumbleBase/mumble_data.h lib/MumbleBase/mumble.pb.h .pio/libdeps/nodemcu-32s/Nanopb/pb.h /home/oliverw/.platformio/packages/tool-unity/unity.h /home/oliverw/.platformio/packages/tool-unity/unity_internals.h  .pio/libdeps/nodemcu-32s/Nanopb/pb_common.h 
OUT	= .pio/build/native/utest
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

INC=-Ilib/MumbleBase/ -I.pio/libdeps/nodemcu-32s/Nanopb/ -I/home/oliverw/.platformio/packages/tool-unity/

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

.pio/build/native/test/test_mumble_data.o: test/test_mumble_data.cpp
	$(CC) $(FLAGS) $(INC) test/test_mumble_data.cpp -std=c11 -o .pio/build/native/test/test_mumble_data.o

.pio/build/native/lib/MumbleBase/mumble_data.o: lib/MumbleBase/mumble_data.cpp
	$(CC) $(FLAGS) $(INC) lib/MumbleBase/mumble_data.cpp -std=c11 -o .pio/build/native/lib/MumbleBase/mumble_data.o

.pio/build/native/lib/MumbleBase/mumble.pb.o: lib/MumbleBase/mumble.pb.c
	$(CC) $(FLAGS) $(INC) lib/MumbleBase/mumble.pb.c -std=c11 -o .pio/build/native/lib/MumbleBase/mumble.pb.o

.pio/build/native/lib/unity.o: /home/oliverw/.platformio/packages/tool-unity/unity.c
	$(CC) $(FLAGS) $(INC)  /home/oliverw/.platformio/packages/tool-unity/unity.c -std=c11 -o .pio/build/native/lib/unity.o

.pio/build/native/lib/pb_common.o: .pio/libdeps/nodemcu-32s/Nanopb/pb_common.c
	$(CC) $(FLAGS) $(INC) .pio/libdeps/nodemcu-32s/Nanopb/pb_common.c -std=c11 -o .pio/build/native/lib/pb_common.o

clean:
	rm -f $(OBJS) $(OUT)