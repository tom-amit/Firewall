//
// Created by ubu1 on 30/03/2022.
//

#include "../include/Firewall.h"

struct Cookie {
    pcpp::PcapLiveDevice* dev;
    RuleTable* table;

    Cookie(pcpp::PcapLiveDevice* _dev, RuleTable* _table){ dev = _dev; table = _table;}
};

static void onPacketArrives1(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    pcpp::Packet parsedPacket(packet);
    Cookie* typedCookie = static_cast<Cookie*>(cookie);
    pcpp::PcapLiveDevice* otherDev = typedCookie->dev;
    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;
    pcpp::EthLayer* ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

    //TODO: delegate this to a different function without magic numbers
    if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>() == NULL || parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address() != pcpp::IPv4Address("172.16.1.2"))
        return;

    if(!typedCookie->table->ParsePacket(parsedPacket, "out"))
        return;

    //std::cout << "allowed packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

    ethernet->setSourceMac(pcpp::MacAddress(otherDev->getMacAddress()));
    ethernet->setDestMac(pcpp::MacAddress("08:00:27:7e:ef:c8"));

    if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive > 1) {
        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
        parsedPacket.computeCalculateFields();
        otherDev->sendPacket(*parsedPacket.getRawPacket());
    }
}
static void onPacketArrives2(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    pcpp::Packet parsedPacket(packet);
    Cookie* typedCookie = static_cast<Cookie*>(cookie);
    pcpp::PcapLiveDevice* otherDev = typedCookie->dev;
    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;
    pcpp::EthLayer* ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

    //TODO: delegate this to a different function without magic numbers
    if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>() == NULL || parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address() != pcpp::IPv4Address("192.168.50.2"))
        return;

    if(!typedCookie->table->ParsePacket(parsedPacket, "in"))
        return;

    ethernet->setSourceMac(pcpp::MacAddress(otherDev->getMacAddress()));
    ethernet->setDestMac(pcpp::MacAddress("08:00:27:f7:c7:e5"));

    if(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive > 1) {
        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
        parsedPacket.computeCalculateFields();
        otherDev->sendPacket(*parsedPacket.getRawPacket());
    }
}

Firewall::Firewall() {
    SetLiveDevices();
    table = new RuleTable();
}

void Firewall::SetLiveDevices() {
    dev1 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("enp0s8");
    dev2 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName("enp0s9");
}
void Firewall::OpenLiveDevices() {
    dev1->open();
    dev2->open();
}
void Firewall::StartLiveDevicesCapture() {
    Cookie* cookie1 = (new Cookie(dev2, table));
    Cookie* cookie2 = (new Cookie(dev1, table));
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


