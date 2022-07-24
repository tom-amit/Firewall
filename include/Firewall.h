//
// Created by ubu1 on 30/03/2022.
//

#ifndef FIREWALL_FIREWALL_H
#define FIREWALL_FIREWALL_H

#include <EthLayer.h>
#include <PcapLiveDeviceList.h>
#include <IPv4Layer.h>
#include "../TableRule/include/RuleTable.h"
#include <IcmpLayer.h>
#include "NICS.h"
#include "ArpTable.h"

struct ARPAwaitingPacket {
    pcpp::Packet parsedPacket;
    pcpp::IPv4Address ip;

    ARPAwaitingPacket(pcpp::Packet _parsedPacket, pcpp::IPv4Address _ip) {
        parsedPacket = _parsedPacket;
        ip = _ip;
    }

    ARPAwaitingPacket() {}
};

class Firewall {
private:
    std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address> dev1, dev2; //dev1 is 'internal' and dev2 is 'external'
    pcpp::MacAddressFilter *dev1filter = nullptr, *dev2filter = nullptr;
    std::vector<ARPAwaitingPacket> arpAwaitingPackets;

    void SetLiveDevices();

	void CloseLiveDevices();

    void OpenLiveDevices();

    void StartLiveDevicesCapture();

    void StopLiveDevicesCapture();



public:
    Firewall();

    //TODO scan the entire project, and make sure no regular pointers are used, memory and efficiency is important here, so smart pointers are a must!
    RuleTable *table;

    void Run();
    void Stop();

    static void SendTTLExpiredPacket(const pcpp::Packet &expiredPacket, pcpp::PcapLiveDevice *dev);
};


#endif //FIREWALL_FIREWALL_H
