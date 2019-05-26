#=========================================
# 选择编译目标
#=========================================
ifndef POWERPCTG	#默认使用本操作系统
	POWERPCTG := 0
endif
export POWERPCTG

#=========================================
# 强制大端
#=========================================
ifndef FORCE_ENDIAN		#本系统模拟FPGA,需要强制大端。1=大端，2=小端，其他=自动选择
	FORCE_ENDIAN := 0
endif
export FORCE_ENDIAN

#=========================================
#是否使用BMU库
#=========================================
ifndef USEBMULIB	#默认不使用BMU库
	USEBMULIB := 0
endif
export USEBMULIB

#=========================================
#是否使用DPDK库
#=========================================
ifndef USEDPDKLIB	#默认使用DPDK库
ifeq ($(shell uname), Linux)	#linux系统才使用
	USEDPDKLIB := 1
else
	USEDPDKLIB := 0
endif
endif
export USEDPDKLIB

#=========================================
#是否使用驱动库
#=========================================
ifndef USEDRIVELIB	#默认使用驱动库
ifeq ($(shell uname), Linux)	#linux系统才使用
	USEDRIVELIB := 1
else
	USEDRIVELIB := 0
endif
endif
export USEDRIVELIB


#########################↑以上设置编译控制开关↑###########################

#=========================================
#设置交叉编译器
#=========================================
ifndef ARCH
	ifeq ($(POWERPCTG), 1)
		ARCH := powerpc
	else
		ARCH := i386
	endif
endif
export ARCH

ifndef CROSS_COMPILE
	ifeq ($(POWERPCTG), 1)
		#CROSS_COMPILE := /home/zhaojzh/p1020/fsl_p1020_p1011-glibc_std/x86-linux2/powerpc-wrs-linux-gnu-ppc_e500v2-glibc_std-
		CROSS_COMPILE := powerpc-wrs-linux-gnu-ppc_e500v2-glibc_std-
	else
		CROSS_COMPILE :=
	endif
endif
export CROSS_COMPILE

#=========================================
# 定义编译器和汇编器
#=========================================
ifndef CC
	CC := $(CROSS_COMPILE)gcc
endif
export CC

ifndef CXX
	CXX := $(CROSS_COMPILE)g++
endif
export CXX

ifndef LD
	LD := $(CROSS_COMPILE)ld
endif
export LD

ifndef AS
	AS := $(CROSS_COMPILE)as
endif
export AS

ifndef AR
	AR := $(CROSS_COMPILE)ar
endif
export AR

ifndef MAKE
	MAKE := make
endif
export MAKE

ifndef RM
	RM := rm
endif
export RM

ifndef MV
	MV	:= mv
endif
export MV

#=========================================
#定义模块路径
#=========================================
#当前makefile路径
CURDIR := $(shell pwd)
#源文件路径
SRC_PATH := ./src
#头文件路径
ifndef INC_PATH
	ifdef TOP_DIR	#总工程的makefile会把所有文件拷贝到最上层的include目录下
		ifeq ($(TOP_DIR)/../include, $(wildcard $(TOP_DIR)/../include))	#总工程的include路径存在
			INC_PATH := $(TOP_DIR)/../include
		else
			INC_PATH := $(TOP_DIR)/include
		endif
		TOP_INC_PATH := $(INC_PATH)
	else
		INC_PATH := $(CURDIR)/include
		ifeq (../../../include, $(wildcard ../../../include))	#总工程的include路径存在
			TOP_INC_PATH := ../../../include
		else
			TOP_INC_PATH := ../../include
		endif
	endif
endif
export INC_PATH TOP_INC_PATH
#DPDK头文件路径
ifndef DPDK_INC_PATH
	ifdef TOP_DIR
		DPDK_INC_PATH := $(TOP_DIR)/../include/thd_inc/dpdk_inc
	else
		DPDK_INC_PATH := $(CURDIR)/../../../include/thd_inc/dpdk_inc
	endif
endif
export DPDK_INC_PATH
#输出文件路径
ifndef TARGET_PATH	#如果没有指定输出文件路径，则输出到本模块的output文件夹下
	ifdef TOP_DIR
		TARGET_PATH := $(TOP_DIR)/output
	else
		TARGET_PATH := $(CURDIR)/output
	endif
endif
export TARGET_PATH
#输出的可执行文件不放到公共库路径中
ifdef TOP_DIR
	OUTPUT_PATH := $(TOP_DIR)/output
else
	OUTPUT_PATH := $(CURDIR)/output
endif
#库文件搜索路径
ifndef LIB_PATH
	LIB_PATH := 
	LIB_PATH += $(TARGET_PATH)
	ifeq ($(USEBMULIB), 1)	#使用BMU库
		ifndef BMU_LIB_PATH
			ERR += $(error BMU_LIB_PATH is not defined)
		else
			LIB_PATH += $(BMU_LIB_PATH)
		endif
	endif
	ifneq ($(shell uname), Linux)	#window操作系统生成dll的时候需要加载关联的库
		ifdef TOP_DIR	#总工程的makefile会把所有文件拷贝到最上层的include目录下
			LIB_PATH += $(TOP_DIR)/output
		else
			LIB_PATH += $(CURDIR)/output
			TOP_LIB_PATH := ../../output
			ifeq ($(TOP_LIB_PATH), $(wildcard $(TOP_LIB_PATH)))	#存在../../output文件夹,这种总工程的输出文件路径
				LIB_PATH += $(TOP_LIB_PATH)
			endif
		endif
	endif
endif
export LIB_PATH
#运行时库文件搜索路径
ifndef LIB_RUN_PATH	#默认库放在同一路径下
	LIB_RUN_PATH := ./
	LIB_RUN_PATH += $(LIB_PATH)
endif
export LIB_RUN_PATH
#中间文件路径
OBJ_PATH := $(CURDIR)/obj

#=========================================
#定义需编译的C文件
#=========================================
SRC_FILE := $(wildcard $(SRC_PATH)/*.c)

#=========================================
#定义动态库及可执行文件后缀
#=========================================
ifeq ($(shell uname), Linux)	#windows系统下的动态库后缀是dll，cygwin无法找到so后缀结尾的动态库
	DLL_SUFFIX := so
	EXE_SUFFIX := out
else
	DLL_SUFFIX := dll
	EXE_SUFFIX := exe
endif

#=========================================
#最终生成目标
#=========================================
TARGET_NAME_PREFIX := fpgasim

TARGET_NAME = $(TARGET_NAME_PREFIX).$(EXE_SUFFIX)

TARGET := $(OUTPUT_PATH)/$(TARGET_NAME)

#=========================================
#定义需加载的库文件
#=========================================
LIB_FILE_PREFIX := 
LIB_FILE_PREFIX += libtestsim libfpgasim
LIB_FILE_PREFIX += libtestcommon liblicom
ifneq ($(shell uname), Linux)	#window操作系统生成dll的时候需要加载关联的库
	LIB_FILE_PREFIX += liblicom
endif
ifeq ($(USEDPDKLIB), 1)
	LIB_FILE_PREFIX += libtestdpdk libdpdk libpthread libdl
else
	LIB_FILE_PREFIX += librt
endif
ifeq ($(USEBMULIB), 1)
	LIB_FILE_PREFIX += libbmu
	LIB_FILE_PREFIX += libdl librt libutil
endif

LIB_FILE := $(patsubst lib%, -l%, $(notdir $(LIB_FILE_PREFIX)))

#=========================================
# 定义基本的编译参数和创建参数
#=========================================
ifndef CFLAGS
	#CFLAGS += -O2 -Wall -pipe -fPIC
	CFLAGS := -g -O0 -Wall -pipe
else
	ifneq ($(findstring -g,$(CFLAGS)), -g)
		CFLAGS += -g
	endif
	ifneq ($(findstring -O0,$(CFLAGS)), -O0)
		CFLAGS += -O0
	endif
	ifneq ($(findstring -Wall,$(CFLAGS)), -Wall)
		CFLAGS += -Wall
	endif
	ifneq ($(findstring -pipe,$(CFLAGS)), -pipe)
		CFLAGS += -pipe
	endif
endif
export CFLAGS

ifndef CXXFLAGS
	#CXXFLAGS += -O2 -Wall -pipe -fPIC
	CXXFLAGS := -g -O0 -Wall -pipe
else
	ifneq ($(findstring -g,$(CXXFLAGS)), -g)
		CXXFLAGS += -g
	endif
	ifneq ($(findstring -O0,$(CXXFLAGS)), -O0)
		CXXFLAGS += -O0
	endif
	ifneq ($(findstring -Wall,$(CXXFLAGS)), -Wall)
		CXXFLAGS += -Wall
	endif
	ifneq ($(findstring -pipe,$(CXXFLAGS)), -pipe)
		CXXFLAGS += -pipe
	endif
endif
export CXXFLAGS

#=========================================
#额外宏定义
#=========================================
ifndef DEFINE
	DEFINE :=
	#平台选型
	ifeq ($(FORCE_ENDIAN), 1)
		DEFINE += -D LI_BIG_ENDIAN
	else ifeq ($(FORCE_ENDIAN), 2)
		DEFINE += -D LI_LITTLE_ENDIAN
	else
	endif
	#使用BMU库
	ifeq ($(USEBMULIB), 1)
	DEFINE += -D LI_USE_BMU_LIB
	endif
	#使用DPDK库
	ifeq ($(USEDPDKLIB), 1)
	DEFINE += -D USE_DPDK_LIB
	endif
	#使用驱动库
	ifeq ($(USEDRIVELIB), 1)
	DEFINE += -D USE_DRIVE_LIB
	endif
endif
export DEFINE
	
#=========================================
#定义头文件搜索路径及宏定义
#=========================================
ifndef INCLUDE
	INCLUDE := -I $(INC_PATH)
	ifdef TOP_INC_PATH
		ifeq ($(TOP_INC_PATH), $(wildcard $(TOP_INC_PATH)))	#存在../../include文件夹,这种总工程的公共头文件路径
		INCLUDE += -I $(TOP_INC_PATH)
		endif
	endif
	ifeq ($(USEDPDKLIB), 1)	#使用DPDK库
		ifndef DPDK_INC_PATH
			ERR += $(error DPDK_INC_PATH is not defined)
		else
			ifeq ($(DPDK_INC_PATH), $(wildcard $(DPDK_INC_PATH)))
				INCLUDE += -I $(DPDK_INC_PATH)
			else
				ERR += $(error $(DPDK_INC_PATH) is not exist)
			endif
		endif
	endif
	ifeq ($(USEBMULIB), 1)	#使用BMU库
		ifndef BMU_INC_PATH
			ERR += $(error BMU_INC_PATH is not defined)
		else
			INCLUDE += -I $(BMU_INC_PATH)
		endif
	endif
endif

#=========================================
#位置无关
#=========================================
ifeq ($(shell uname), Linux)	#window操作系统生成dll的时候默认位置无关
	FPIC := -fPIC
endif

#=========================================
#动态库
#=========================================
SHARED := 

#=========================================
#操作系统位数
#=========================================
ifndef SYSTEM_BITS	#上层makefile没有传入则使用编译器所在系统的位数
SYSTEM_BITS := 
endif
export SYSTEM_BITS

#=========================================
#定义链接的参数
#=========================================
LINKFLAGS := -rdynamic
RUN_LIB_REPFIX := -Wl,-rpath=
ifneq ($(LIB_FILE),)	#需要加载库
	LINKFLAGS += $(addprefix $(RUN_LIB_REPFIX),$(LIB_RUN_PATH))
endif

#=========================================
#链接的库
#=========================================
LIB :=
ifneq ($(LIB_FILE),)	#需要加载库
	ifneq ($(LIB_PATH),)	#LIB_PATH非空
		LIB += $(addprefix -L ,$(LIB_PATH))
	endif
		LIB += $(LIB_FILE)
endif

#=========================================
#中间目标
#=========================================
OBJ_NAME := $(patsubst %.c, %.o, $(notdir $(SRC_FILE)))
OBJ := $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

#=========================================
# 定义生成的目标
#=========================================
Q := @
exes := $(TARGET)

#=========================================
# 导出变量给子makefile用
#=========================================
#export top_dir CFLAGS CXXFLAGS LIB_PATH LIB_RUN_PATH MAKE RM TARGET_PATH TARGET_NAME TARGET OBJ_NAME OBJ CROSS_COMPILE CC CXX AS AR Q

#=========================================
# 定义执行顺序目标
#=========================================
poly_exes := starttip
poly_exes += err
poly_exes += mkoutdir
poly_exes += $(exes)
#poly_exes += mvtip
poly_exes += endtip

.PHONY : all
all : $(poly_exes)

starttip :
	$(Q)echo "++++++++++++++++++++++++++++ start of making +++++++++++++++++++++++++++"
	$(Q)echo "***********************************************************"
	$(Q)echo "*                  Building "${TARGET_NAME}"                 *"
	$(Q)echo "***********************************************************"

err :
	$(ERR)

mkoutdir :
	$(Q)if [ ! -d "`dirname $(TARGET)`" ]; then\
		mkdir `dirname $(TARGET)`;\
	fi
	$(Q)if [ ! -d "$(OBJ_PATH)" ]; then\
		mkdir $(OBJ_PATH);\
	fi

mvtip :
	

endtip :
	$(Q)echo "++++++++++++++++++++++++++++ end of making  ++++++++++++++++++++++++++++"

$(TARGET) :  $(OBJ)
	$(CXX) $(CXXFLAGS) $(DEFINE) $(INCLUDE) $(SHARED) $(SYSTEM_BITS) $(LINKFLAGS) $^ -o $@ $(LIB)

$(OBJ) : $(OBJ_PATH)/%.o:$(SRC_PATH)/%.c
	$(CXX) $(CXXFLAGS) $(DEFINE) $(INCLUDE) $(FPIC) $(SYSTEM_BITS) -o $@ -c $<

#=========================================
#设置 make clean
#=========================================
clean_path := $(OBJ_PATH) $(TARGET_PATH)

.PHONY : clean
clean:
	$(RM) -rf $(OBJ_PATH)/*.o
	$(RM) -rf $(TARGET)
	$(RM) -rf $(OBJ_PATH)
