#  Makefile for application using library.

CC := g++
SOURCES := main.cpp
OUT_FILE_NAME := application.app
OUT_DIR	:=	../../build/application
OBJ_DIR	:=	$(OUT_DIR)/object

LIBPATHS = ../../build/library/library.a

override CXXFLAGS += -std=c++11
override LDFLAGS += -lm
override CPPFLAGS += -I ../library

$(OUT_FILE_NAME): $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES)) $(LIBPATHS)
	$(CC) ${LDFLAGS} -o $(OUT_DIR)/$@ $^ $(LINKFLAGS)

$(OBJ_DIR)/%.o: $(SOURCES)
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(INCLUDE) ${CPPFLAGS} ${CXXFLAGS} -o $@ $<

.PHONY: clean

clean:
	rm -rf $(OUT_DIR)
	rm -f Makefile.bak
