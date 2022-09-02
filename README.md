# <u> Network Firewall By Tom K. and Amit S.</u>

A network firewall implementation in C++17 using PcapPlusPlus.

The project was initially compiled using Make but was converted to CMake for ease and GUI support.

# <u>Building Dependencies for CMake usage:</u>

## Dependencies for Pcap++:

- libpcap
- CMake > 3.12
- Optional:
  - Ultralight (for GUI)

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
cmake ..
make
sudo make install
```

Now we've built Pcap++ for CMake usage.

## Getting Ultralight (RECOMMENDED):

The project downloads Ultralight on its own if specified, just add the flag:`-DGUI_ON=1`

Ultralight is the utility that gives you a visual interface.
# <u>Building the firewall</u>

```shell
git clone https://github.com/tom-amit/Firewall.git --depth 1
cd $FIREWALL_DOWNLOAD_PATH
mkdir build && cd build
cmake .. # OPTIONAL: -DGUI_ON=1
make
```

The executable is located inside the bin/ folder.

# <u> Running the firewall</u>

- The firewall was built using Linux, so you need to run it on Linux.
- In order to run the firewall you need to have 2 network cards that are connected to different networks 
on the routing computer (the computer hosting the firewall)
- To actually test the firewall you obviously need 2 different computers connected to the 2 networks respectively.