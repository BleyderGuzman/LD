CXXSOURCES	=										\
				src/Device.cpp						\
				src/DumpLog.cpp						\
				src/ICmd.cpp						\
				src/SerialComPOSIX.cpp				\
				src/STranRS232.cpp					\
				src/TransportLibUSB.cpp				\
				src/TransportRS232.cpp				\
				src/TransportRS232_NX.cpp			\
				src/TransportSEPOSIX.cpp			\
				src/V100Cmd.cpp						\
				src/V100CommandHandler.cpp			\
				src/V100DeviceHandler.cpp			\
				src/V100IDCmd.cpp					\
				src/V100_shared_types.cpp			\
				src/VCOMCore.cpp					\
				src/XModem.cpp

all: lib/libVCOMExample.a

obj/%.o : src/%.cpp
	@echo "###############################################################################"
	@echo -n "### COMPILE : "
	g++ -fPIC -IAPI -Iinclude -I/usr/include/libusb-1.0 -c $< -o $@

clean:
	rm ./obj/*.o ./lib/*

lib/libVCOMExample.a : $(CXXSOURCES:src/%.cpp=obj/%.o) API/*.h include/*.h
	ar rv $@ $(CXXSOURCES:src/%.cpp=obj/%.o)
	g++ -shared -Wl,--version-script=version ./obj/*.o -o ./lib/libVCOMExample.6.01.26.so
	ln -sf libVCOMExample.6.01.26.so ./lib/libVCOMExample.so
