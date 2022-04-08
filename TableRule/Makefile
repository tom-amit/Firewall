include /usr/local/etc/PcapPlusPlus.mk
# All Target
all:
	# g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/main.o main.cpp
	g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/Rule.o src/Rule.cpp
	g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/BadParse.o src/BadParse.cpp
	g++ -std=c++17 $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o obj/RuleTable.o src/RuleTable.cpp
	# g++ -std=c++17 $(PCAPPP_LIBS_DIR) -static-libstdc++ -o table obj/main.o obj/Rule.o obj/BadParse.o obj/RuleTable.o $(PCAPPP_LIBS)

# Clean Target
clean:
	rm obj/main.o
	rm obj/BadParse.o
	rm obj/Rule.o
	rm obj/RuleTable.o
	rm table