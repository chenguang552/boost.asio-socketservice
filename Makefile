TARGET = WebService

CC := g++

# 头文件包含
INCLUDE := -I./Resources 
# 链接库文件
LIBS := -lpthread -L./libBoost -lboost_system 
# -L/usr/local/lib -lboost 
# 提取Resources文件夹下所有cpp文件目标
SRCS := $(wildcard ./Resources/*.cpp)
# 将所有目标后缀改为.o
OBJS := $(patsubst %cpp, %o, $(SRCS))

.PHONY : all  clean install  objs

all  : $(TARGET)

#objs : $(OBJS)

FLAGS := -g -Wall -std=c++11 -O3 $(INCLUDE)

SHARE := -fPIC -shared

# $@表示所有目标文件 $^表示所有依赖文件
$(TARGET) : $(OBJS)
	$(CC) $(FLAGS) -o $@ $^ $(LIBS)

%o:%c
	$(CC) -g -std=c++11 -c $ -c $^
install:
	cp ./libBoost/libboost_system.so.1.66.0 /lib
	cp ./libBoost/libboost_system.so.1.66.0 /lib64
test:
	g++ -g -Wall -std=c++11 -O3 -I./Resources -o test test.cpp  -lpthread -L./libBoost -lboost_system

clean:
	rm -f $(OBJS) $(TARGET) test
