# <u> Network Firewall By Tom K. and Amit S.</u>

A network firewall implementation in C++17 using PcapPlusPlus.

The project was initially compiled using Make but was converted to CMake for ease and GUI support.

# <u>How To Build Pcap++ for CMake usage:</u>

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

## Getting Ultralight (OPTIONAL):

Download the latest release of Ultralight from https://github.com/ultralight-ux/Ultralight/releases.

You can download an already built version or build from the source yourself, unfortunately the library does not come
with an option to install to root, so you'll have to remember where you downloaded/built it.

When compiling the Firewall make sure you add the CMake flag:
`-DUL_SDK_ROOT=/path/to/your/sdk/`