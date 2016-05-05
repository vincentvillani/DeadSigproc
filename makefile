CC_FLAGS=-m64
LIBS=
DEBUG_FLAGS=-g3 -O0 -std=c++11
RELEASE_FLAGS=-O3 -std=c++11
FILE_LIST=main.cpp

debug:
	g++ $(CC_FLAGS) -o debug.out $(FILE_LIST) $(LIBS) $(DEBUG_FLAGS)

debugMulti:
	g++ $(CC_FLAGS) -o debugMultiStream.out $(FILE_LIST) $(LIBS) $(DEBUG_FLAGS)

release:
	g++ $(CC_FLAGS) -o release.out $(FILE_LIST) $(LIBS) $(RELEASE_FLAGS)


clean:
	rm debug.out release.out