HDR_DIR = ./hdrs
SRC_DIR = ./srcs
OBJ_DIR = ./objs


IGNORE := $(SRC_DIR)/MotorSaw.cpp 

#$(info $$IGNORE is [${IGNORE}])

CC_FILES := $(filter-out $(IGNORE), $(wildcard $(SRC_DIR)/*.cpp))
#$(info $$CC_FILES is [${CC_FILES}])

OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CC_FILES))
#$(info $$OBJ_FILES is [${OBJ_FILES}])

TARGETS = MotorSaw 
DEFAULT_TGT = MotorSaw

CC = g++
#OPT = -flto -O3
DEBUG = -g
CFLAGS = -std=c++11 -fPIC -Wall # -pg
CVINC = -I/usr/local/include/opencv -I/usr/local/include
CVLIBS = -L/usr/local/lib -lopencv_highgui -lopencv_videoio -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_objdetect -lopencv_calib3d
INC = -I$(HDR_DIR) $(CVINC)
LDFLAGS = $(CVLIBS) -lrt -lwiringPi -lpthread -lcrypt -lsweep -lfreenect -lrealsense2
INCLUDE = -I/usr/local/include/libfreenecta -I/usr/include/libusb-1.0

all: $(DEFAULT_TGT)

everything: $(TARGETS)

MotorSaw: $(OBJ_FILES)
	$(CC) $(OPT) $(DEBUG) $(INCLUDE) $(CFLAGS) $(INC) $(SRC_DIR)/$@.cpp -o $@ $^ $(LDFLAGS)

$(OBJ_FILES): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(OPT) $(DEBUG) $(CFLAGS) $(INC) -c $< -o $@

noexec:
	rm -f $(TARGETS)
clean:
	rm -f $(TARGETS) ./$(OBJ_DIR)/*.o
