include /usr/local/etc/PcapPlusPlus.mk

# All Target
all:
	g++ $(PCAPPP_INCLUDES) -c -o obj/main.o main.cpp
	g++ $(PCAPPP_LIBS_DIR) -o firewall-app obj/main.o $(PCAPPP_LIBS)

# Clean Target
clean:
	rm obj/main.o
	rm firewall-app
