PROJECT = xombie


EXE = bin/${PROJECT}

CC = g++ -g -pg
CFLAGS = -std=c++0x `sdl-config --cflags` -Wall -W -Isrc -Isrc/item
LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL_gfx

OBJDIRS = obj obj/item
OBJS = obj/audio.o\
       obj/char.o\
       obj/common.o\
       obj/conf.o\
       obj/engine.o\
       obj/font.o\
       obj/fstack.o\
       obj/imgbase.o\
       obj/inventory.o\
       obj/invview.o\
       obj/item.o\
       obj/main.o\
       obj/mob.o\
       obj/random.o\
       obj/range.o\
       obj/shot.o\
       obj/sprite.o\
       obj/str.o\
       obj/view.o\
       obj/weapon.o\
       obj/world.o\
       obj/worldview.o\
       obj/xml.o\
       \
       obj/item/pistol.o\
       obj/item/shotgun.o\
       obj/item/firstaidkit.o



all: build

re: clean build

build: ${EXE}

run: build
	(cd bin; ./${PROJECT})

debug: build
	(cd bin; gdb ${PROJECT})

profile: build
	(cd bin; ./${PROJECT}; gprof ${PROJECT} > profile.txt; firefox profile.txt&)

tags:
	(cd src; ctags *.cpp *.h)

${EXE}: $(OBJDIRS) ${OBJS}
	$(CC) ${OBJS} ${LIBS} -o ${EXE} 

obj:
	mkdir -p obj

obj/item:
	mkdir -p obj/item

obj/%.o: src/%.c
	$(CC) ${CFLAGS} -c -o $@ $<

obj/%.o: src/%.cpp
	$(CC) ${CFLAGS} -c -o $@ $<

clean:
	rm -rf ${OBJS} ${EXE} src/tags $(OBJDIRS)

