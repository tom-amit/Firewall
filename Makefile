include /usr/local/etc/PcapPlusPlus.mk

SUBDIRS = TableRule

all: $(SUBDIRS)
	g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/main.o main.cpp
	g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/Controller.o src/Controller.cpp
	g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/Firewall.o src/Firewall.cpp
	g++ -std=c++17 $(PCAPPP_LIBS_DIR) -static-libstdc++ -o Firewall obj/main.o TableRule/obj/Rule.o TableRule/obj/BadParse.o TableRule/obj/RuleTable.o obj/Controller.o obj/Firewall.o $(PCAPPP_LIBS)

$(SUBDIRS):
	$(MAKE) -C $@
.PHONY: $(SUBDIRS)