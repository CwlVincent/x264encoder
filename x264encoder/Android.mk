
LOCAL_PATH := $(call my-dir)



include $(CLEAR_VARS) 
LOCAL_MODULE    := x264encoder
LOCAL_SRC_FILES := example_sszpf_x264_JNI.cpp \
	PUX264Encoder.cpp \
	x264encoder.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_LDFLAGS += -fuse-ld=bfd -g
LOCAL_LDLIBS := -llog -lz
LOCAL_SHARED_LIBRARIES :=  h264enc
include $(BUILD_SHARED_LIBRARY)