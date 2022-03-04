#include <iostream>
#include <IPv4Layer.h>
#include <Packet.h>
#include <PcapFileDevice.h>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <unistd.h>
#include <EthLayer.h>
#include <TcpLayer.h>
/**
* A struct for collecting packet statistics
*/
struct PacketStats
{
    int ethPacketCount;
    int ipv4PacketCount;
    int ipv6PacketCount;
    int tcpPacketCount;
    int udpPacketCount;
    int dnsPacketCount;
    int httpPacketCount;
    int sslPacketCount;

    /**
    * Clear all stats
    */
    void clear() { ethPacketCount = 0; ipv4PacketCount = 0; ipv6PacketCount = 0; tcpPacketCount = 0; udpPacketCount = 0; tcpPacketCount = 0; dnsPacketCount = 0; httpPacketCount = 0; sslPacketCount = 0; }

    /**
    * C'tor
    */
    PacketStats() { clear(); }

    /**
    * Collect stats from a packet
    */
    void consumePacket(pcpp::Packet& packet)
    {
        if (packet.isPacketOfType(pcpp::Ethernet))
            ethPacketCount++;
        if (packet.isPacketOfType(pcpp::IPv4))
            ipv4PacketCount++;
        if (packet.isPacketOfType(pcpp::IPv6))
            ipv6PacketCount++;
        if (packet.isPacketOfType(pcpp::TCP))
            tcpPacketCount++;
        if (packet.isPacketOfType(pcpp::UDP))
            udpPacketCount++;
        if (packet.isPacketOfType(pcpp::DNS))
            dnsPacketCount++;
        if (packet.isPacketOfType(pcpp::HTTP))
            httpPacketCount++;
        if (packet.isPacketOfType(pcpp::SSL))
            sslPacketCount++;
    }

    /**
    * Print stats to console
    */
    void printToConsole()
    {
        std::cout
                << "Ethernet packet count: " << ethPacketCount << std::endl
                << "IPv4 packet count:     " << ipv4PacketCount << std::endl
                << "IPv6 packet count:     " << ipv6PacketCount << std::endl
                << "TCP packet count:      " << tcpPacketCount << std::endl
                << "UDP packet count:      " << udpPacketCount << std::endl
                << "DNS packet count:      " << dnsPacketCount << std::endl
                << "HTTP packet count:     " << httpPacketCount << std::endl
                << "SSL packet count:      " << sslPacketCount << std::endl;
    }
};

/**
* a callback function for the blocking mode capture which is called each time a packet is captured
*/
static bool onPacketArrivesBlockingMode(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    // extract the stats object form the cookie
    PacketStats* stats = (PacketStats*)cookie;

    // parsed the raw packet
    pcpp::Packet parsedPacket(packet);

    // collect stats from packet
    stats->consumePacket(parsedPacket);

    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

    sleep(3);

    // return false means we don't want to stop capturing after this callback
    return false;
}

/**
* A callback function for the async capture which is called each time a packet is captured
*/
static void onPacketArrives1(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{

    pcpp::Packet parsedPacket(packet);
    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

    pcpp::PcapLiveDevice* otherDev = (pcpp::PcapLiveDevice*)cookie;

    pcpp::EthLayer* ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();
    if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>() != NULL && parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address() == pcpp::IPv4Address("172.16.1.2")) {
        ethernet->setSourceMac(pcpp::MacAddress(otherDev->getMacAddress()));
        ethernet->setDestMac(pcpp::MacAddress("08:00:27:7e:ef:c8"));

        if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive > 1) {
            parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
            parsedPacket.computeCalculateFields();


            otherDev->sendPacket(*parsedPacket.getRawPacket());
        }
    }
}
static void onPacketArrives2(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{

    pcpp::Packet parsedPacket(packet);
    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

    pcpp::PcapLiveDevice* otherDev = (pcpp::PcapLiveDevice*)cookie;

    pcpp::EthLayer* ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

    if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>() != NULL && parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address() == pcpp::IPv4Address("192.168.50.2")) {
        //std::cout << "sending forwarded packet" << std::endl;
        ethernet->setSourceMac(pcpp::MacAddress(otherDev->getMacAddress()));
        ethernet->setDestMac(pcpp::MacAddress("08:00:27:f7:c7:e5"));

        if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive > 1) {
            parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
            parsedPacket.computeCalculateFields();

            if(parsedPacket.getLayerOfType<pcpp::TcpLayer>() == NULL || parsedPacket.getLayerOfType<pcpp::TcpLayer>()->getDstPort() != 7777)
                otherDev->sendPacket(*parsedPacket.getRawPacket());
        }
    }
}



int main(int argc, char* argv[])
{

    pcpp::PcapLiveDevice* dev1 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("enp0s8");
    pcpp::PcapLiveDevice* dev2 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("enp0s9");
    if (dev1 == NULL)
    {
        std::cerr << "Cannot find interface with name 'enp0s8'" << std::endl;
        return 1;
    }
    if (dev2 == NULL)
    {
        std::cerr << "Cannot find interface with name 'enp0s9'" << std::endl;
        return 1;
    }

    // before capturing packets let's print some info about these interfaces
    std::cout
            << "Interface info:" << std::endl
            << "   Interface name:        " << dev1->getName() << std::endl // get interface name
            << "   Interface ip:          " << dev1->getIPv4Address() << std::endl // get interface ip
            << "   Interface description: " << dev1->getDesc() << std::endl // get interface description
            << "   MAC address:           " << dev1->getMacAddress() << std::endl // get interface MAC address
            << "   Default gateway:       " << dev1->getDefaultGateway() << std::endl // get default gateway
            << "   Interface MTU:         " << dev1->getMtu() << std::endl; // get interface MTU

    if (dev1->getDnsServers().size() > 0)
        std::cout << "   DNS server:            " << dev1->getDnsServers().at(0) << std::endl;

    std::cout
            << "Interface info:" << std::endl
            << "   Interface name:        " << dev2->getName() << std::endl // get interface name
            << "   Interface ip:          " << dev2->getIPv4Address() << std::endl // get interface ip
            << "   Interface description: " << dev2->getDesc() << std::endl // get interface description
            << "   MAC address:           " << dev2->getMacAddress() << std::endl // get interface MAC address
            << "   Default gateway:       " << dev2->getDefaultGateway() << std::endl // get default gateway
            << "   Interface MTU:         " << dev2->getMtu() << std::endl; // get interface MTU

    if (dev2->getDnsServers().size() > 0)
        std::cout << "   DNS server:            " << dev2->getDnsServers().at(0) << std::endl;

    // open the device before start capturing/sending packets
    if (!dev1->open())
    {
        std::cerr << "Cannot open device 1" << std::endl;
        return 1;
    }
    if (!dev2->open())
    {
        std::cerr << "Cannot open device 2" << std::endl;
        return 1;
    }

    // create the stats object
    PacketStats stats;

    std::cout << std::endl << "Starting capture in blocking mode..." << std::endl;

    // clear stats
    stats.clear();

    // start capturing in blocking mode. Give a callback function to call to whenever a packet is captured, the stats object as the cookie and a 10 seconds timeout
    dev1->startCapture(onPacketArrives1, dev2);
    dev2->startCapture(onPacketArrives2, dev1);

    // thread is blocked until capture is finished
    //pcpp::multiPlatformSleep(10);
    std::string inp;
    do {
        std::cin >> inp;
    }while(inp != "quit");
    std::cout << "quitting..." << std::endl;

    dev1->stopCapture();
    dev2->stopCapture();
    // capture is finished, print results
    std::cout << "Results:" << std::endl;
    //stats.printToConsole();

    return 0;
}