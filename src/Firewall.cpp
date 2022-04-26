//
// Created by ubu1 on 30/03/2022.
//

#include "../include/Firewall.h"

struct Cookie {
    pcpp::PcapLiveDevice* dev;
    RuleTable* table;

    Cookie(pcpp::PcapLiveDevice* _dev, RuleTable* _table){ dev = _dev; table = _table;}
};


//TODO: remove magic MAC-IP addresses
static void onPacketArrives1(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    pcpp::Packet parsedPacket(packet);

    if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() != nullptr &&
        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive < 2) {
        Firewall::SendTTLExpiredPacket(parsedPacket, dev);
        return;
    }

    auto *typedCookie = static_cast<Cookie *>(cookie);
    pcpp::PcapLiveDevice *otherDev = typedCookie->dev;
    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;
    auto *ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

    //TODO: delegate this to a different function without magic numbers
    if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() == nullptr ||
        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address() != pcpp::IPv4Address("172.16.1.2"))
        return;

    if (!typedCookie->table->ParsePacket(parsedPacket, "out"))
        return;

    //std::cout << "allowed packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

    ethernet->setSourceMac(pcpp::MacAddress(otherDev->getMacAddress()));
    ethernet->setDestMac(pcpp::MacAddress("08:00:27:7e:ef:c8"));

    parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
    parsedPacket.computeCalculateFields();
    otherDev->sendPacket(*parsedPacket.getRawPacket());
}
//TODO: remove magic MAC-IP addresses
static void onPacketArrives2(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie) {
    pcpp::Packet parsedPacket(packet);

    if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() != nullptr &&
        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive < 2) {
        Firewall::SendTTLExpiredPacket(parsedPacket, dev);
        return;
    }

    auto *typedCookie = static_cast<Cookie *>(cookie);
    pcpp::PcapLiveDevice *otherDev = typedCookie->dev;
    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;
    auto *ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

    //TODO: delegate this to a different function without magic numbers
    if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() == nullptr ||
        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address() != pcpp::IPv4Address("192.168.50.2"))
        return;

    if (!typedCookie->table->ParsePacket(parsedPacket, "in"))
        return;

    ethernet->setSourceMac(pcpp::MacAddress(otherDev->getMacAddress()));
    ethernet->setDestMac(pcpp::MacAddress("08:00:27:f7:c7:e5"));

    parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
    parsedPacket.computeCalculateFields();
    otherDev->sendPacket(*parsedPacket.getRawPacket());
}

Firewall::Firewall() {
    SetLiveDevices();
    table = new RuleTable();
}

void Firewall::SetLiveDevices() {
    dev1 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("enp0s8");
    dev2 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("enp0s9");
}

void Firewall::OpenLiveDevices() const {
    dev1->open();
    dev2->open();
}

void Firewall::StartLiveDevicesCapture() {
    auto *cookie1 = (new Cookie(dev2, table));
    auto *cookie2 = (new Cookie(dev1, table));
    dev1->startCapture(onPacketArrives1, cookie1);
    dev2->startCapture(onPacketArrives2, cookie2);
}
void Firewall::StopLiveDevicesCapture() {
    dev1->stopCapture();
    dev2->stopCapture();
}

void Firewall::Run() {
    OpenLiveDevices();
    StartLiveDevicesCapture();
}

void Firewall::Stop() {
    StopLiveDevicesCapture();
}

//TODO: Use ARP Table to determine dest MAC
pcpp::Packet Firewall::SendTTLExpiredPacket(const pcpp::Packet &expiredPacket, pcpp::PcapLiveDevice *dev) {
    auto TTLExpired = pcpp::Packet();

    // Create the Ethernet layer
    auto expiredEthLayer = expiredPacket.getLayerOfType<pcpp::EthLayer>();
    auto ethLayer = new pcpp::EthLayer(expiredEthLayer->getSourceMac(), expiredEthLayer->getDestMac(), pcpp::IPv4);
    TTLExpired.addLayer(ethLayer);

    // Create the IPv4 layer
    auto expiredIPv4Layer = new pcpp::IPv4Layer(expiredPacket.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPv4Address(),
                                                expiredPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address());
    auto ipv4Layer = new pcpp::IPv4Layer(dev->getIPv4Address(), expiredIPv4Layer->getSrcIPv4Address());
    TTLExpired.addLayer(ipv4Layer);

    // Create the ICMP layer
    pcpp::Layer *expiredL4Layer = nullptr;
    if (expiredPacket.getLayerOfType<pcpp::UdpLayer>() != nullptr)
        expiredL4Layer = new pcpp::UdpLayer(expiredPacket.getLayerOfType<pcpp::UdpLayer>()->getSrcPort(),
                                            expiredPacket.getLayerOfType<pcpp::UdpLayer>()->getDstPort());
    else if (expiredPacket.getLayerOfType<pcpp::TcpLayer>() != nullptr)
        expiredL4Layer = new pcpp::TcpLayer(expiredPacket.getLayerOfType<pcpp::TcpLayer>()->getSrcPort(),
                                            expiredPacket.getLayerOfType<pcpp::TcpLayer>()->getDstPort());

    auto icmpLayer = new pcpp::IcmpLayer();
    TTLExpired.addLayer(icmpLayer);
    icmpLayer->setTimeExceededData(0, expiredIPv4Layer, expiredL4Layer);

    TTLExpired.computeCalculateFields();

    dev->sendPacket(*TTLExpired.getRawPacket());
}


