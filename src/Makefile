PROJECT = xombie

EXE = ../bin/$(PROJECT)

CXX := $(CXX) -g -DDEBUG
CFLAGS = -Wall -W -I. -Iitem
LIBS = -lSDL_mixer -lSDL_ttf -lSDL_image

ifneq ($(OS),Windows_NT)
	CFLAGS := $(CFLAGS) `sdl-config --cflags`
	LIBS := $(LIBS) `sdl-config --libs`
else
	CFLAGS := $(CFLAGS) -I"C:/Dev-Cpp/include/SDL"
	LIBS := -lSDLmain $(LIBS) -lSDL -lfreetype -mwindows
endif



OBJDIRS = ../obj/item ../obj
OBJS = ../obj/audio.o\
       ../obj/char.o\
       ../obj/common.o\
       ../obj/conf.o\
       ../obj/e.o\
       ../obj/engine.o\
       ../obj/font.o\
       ../obj/fstack.o\
       ../obj/imgbase.o\
       ../obj/inventory.o\
       ../obj/invview.o\
       ../obj/item.o\
       ../obj/main.o\
       ../obj/mob.o\
       ../obj/random.o\
       ../obj/range.o\
       ../obj/SDL_rotozoom.o\
       ../obj/shot.o\
       ../obj/sprite.o\
       ../obj/str.o\
       ../obj/view.o\
       ../obj/weapon.o\
       ../obj/world.o\
       ../obj/worldview.o\
       ../obj/xml.o\
       \
       ../obj/item/pistol.o\
       ../obj/item/shotgun.o\
       ../obj/item/firstaidkit.o



all: build

re: clean build

build: $(EXE)

run: build
	@(cd ../bin; echo \ \ ./$(PROJECT); ./$(PROJECT))

debug: build
	(cd ../bin; gdb $(PROJECT))

profile: build
	(cd ../bin; ./$(PROJECT); gprof $(PROJECT) > profile.txt; firefox profile.txt&)

tags:
	ctags *.cpp *.h

$(EXE): $(OBJS)
	@echo \ \ CXX $(EXE)
	@$(CXX) $(OBJS) $(LIBS) -o $(EXE)

../obj:
	mkdir -p $<

../obj/item:
	mkdir -p $<

../obj/%.o: %.c
	@echo \ \ CXX $<
	@$(CXX) $(CFLAGS) -c -o $@ $<

../obj/%.o: %.cpp
	@echo \ \ CXX $<
	@$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) tags $(EXE)

