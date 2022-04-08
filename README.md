# <u> Network Firewall By Tom K. and Amit S.</u>

A network firewall implementation in C++17 using PcapPlusPlus.

The project was initially compiled using Make but was converted to CMake for ease and GUI support.

# <u>How To Build Pcap++ for CMake usage:</u>

## Dependencies for Pcap++:

```shell
sudo apt-get install libpcap-dev
```

## Building Pcap++:

```shell
git clone https://github.com/clementperon/PcapPlusPlus.git
cd PcapPlusPlus
git checkout cmake-ng
mkdir build && cd build
make
sudo make install
```

Now we've built Pcap++ for CMake usage.