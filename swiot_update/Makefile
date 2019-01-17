#
## Makefile for MQTTLinux
#
#
#CC=aarch64-linux-gnu-gcc
#AR=aarch64-linux-gnu-ar
#CC=aarch64-linux-gnu-gcc
#AR=aarch64-linux-gnu-ar
CC=aarch64-linux-gnu-gcc
AR=aarch64-linux-gnu-ar
#LIB_DIR = -L $(PWD)
LIB_DIR += -L $(PWD)/lib
    

#INCLUDE_DIR = -I $(PWD)
INCLUDE_DIR += -I $(PWD)/include

APP=$(PWD)/swiot_update
CFLAGS = -Wall -Wextra -fPIC -shared -fpermissive
DEPS = -lm  -lswiot_sdk -lpthread -lrecovery -lproperty -landroid_cutils
SRC = $(wildcard $(PWD)/src/*.c)

#OBJS = $(patsubst %.c,%.o,$(SRC) )

# rules
all : $(APP)
	
$(APP) : $(SRC)
	$(CC)  -o $@ $^ $(INCLUDE_DIR) $(LIB_DIR)  -Wl,--whole-archive $(DEPS) -Wl,--no-whole-archive 
	
#%.o: %.c
#	$(CC) -g -c $(CFLAGS) $(INCLUDE_DIR) $^ -o $@ 

.PHONY: clean

clean:
	rm -f $(APP)
