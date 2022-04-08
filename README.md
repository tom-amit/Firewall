# <u> Network Firewall By Tom K. and Amit S.</u>

A network firewall implementation in C++17 using PcapPlusPlus.

The project was initially compiled using Make but was converted to CMake for ease and GUI support.

# <u>How To Build Pcap++ for CMake usage:</u>

## Dependencies for Pcap++:

- libpcap
- CMake > 3.12

### Install them:

```shell
sudo apt-get install libpcap-dev
```

Check your CMake version by running:

```shell
cmake --version
```

If your version is higher than 3.12 you can skip to [Building Pcap++](#building-pcap).

### Update CMake:

Go to https://cmake.org/download/ and download a version that is higher than 3.12 and perform these steps:

```shell
cd $CMAKE_DOWNLOAD_PATH #The path of your download.....
./configure
make
sudo make install
```

## Building-Pcap++:

```shell
git clone https://github.com/clementperon/PcapPlusPlus.git
cd PcapPlusPlus
git checkout cmake-ng
mkdir build && cd build
make
sudo make install
```

Now we've built Pcap++ for CMake usage.