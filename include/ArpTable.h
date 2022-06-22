//
// Created by amit on 6/2/22.
//

#ifndef FIREWALL_ARPTABLE_H
#define FIREWALL_ARPTABLE_H

#include <ArpLayer.h>
#include <PcapLiveDeviceList.h>
#include "NICS.h"

struct ArpEntry {
    pcpp::IPv4Address ip;
    pcpp::MacAddress mac;
    pcpp::PcapLiveDevice* device;

    ArpEntry(pcpp::IPv4Address _ip, pcpp::MacAddress _mac, pcpp::PcapLiveDevice* _device){ip = _ip; mac = _mac;device = _device;}
};

class ArpTable {
public:
	static std::vector<ArpEntry> table;

	static pcpp::MacAddress* Lookup(pcpp::IPv4Address ip);

    static void AddEntry(ArpEntry entry);

    static void RespondToArp(pcpp::Packet& packet, pcpp::PcapLiveDevice* dev);
	
private:
	static ArpEntry* GetEntry(pcpp::IPv4Address ip);
	
	static void RequestArp(pcpp::IPv4Address ip);
};

#endif //FIREWALL_ARPTABLE_H
