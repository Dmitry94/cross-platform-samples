#  Makefile for Static library.
# 1. Compile every *.cpp in the folder
# 2. All obj files under obj folder
# 3. static library .a at lib folder

CC := g++
SOURCES	:=	library.cpp
OUT_FILE_NAME := library.a
OUT_DIR	:=	../../build/library
OBJ_DIR	:=	$(OUT_DIR)/object

override CPPFLAGS += -I ../library
override CXXFLAGS += -std=c++11

$(OUT_FILE_NAME): $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
	ar -r -o $(OUT_DIR)/$@ $^

$(OBJ_DIR)/%.o: $(SOURCES)
	mkdir -p $(OBJ_DIR)
	$(CC) -c ${CPPFLAGS} ${CXXFLAGS} -o $@ $<

.PHONY: clean

clean:
	rm -rf $(OUT_DIR)
	rm -f Makefile.bak
