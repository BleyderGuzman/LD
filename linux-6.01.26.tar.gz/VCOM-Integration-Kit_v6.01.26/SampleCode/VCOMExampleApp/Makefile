all :
	$(MAKE) -C ../../VCOMExample
	$(MAKE) bin/VCOMExampleApp

CXXSOURCES	=							\
				src/VCOMExampleApp.cpp

obj/%.o : src/%.cpp
	@echo "###############################################################################"
	@echo -n "### COMPILE : "
	g++ -I./include -I../../VCOMExample/API -I../../VCOMExample/include -c $< -o $@

bin/VCOMExampleApp : $(CXXSOURCES:src/%.cpp=obj/%.o) ../../VCOMExample/include/*.h ../../VCOMExample/API/*.h
	@echo "###############################################################################"
	@echo -n "### LINK    : "
	g++ -o bin/VCOMExampleApp $(CXXSOURCES:src/%.cpp=obj/%.o) ../../VCOMExample/lib/libVCOMExample.a -lusb-1.0 -ldl

clean :
	rm obj/* bin/*
