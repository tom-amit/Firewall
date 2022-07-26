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

    /**
     * lookup MAC address in ARP table, if not found sends an ARP request
     * @param ip ip to search MAC for
     * @return the MAC address of the ip, or nullptr if not found
     */
	static pcpp::MacAddress* Lookup(pcpp::IPv4Address ip);

    static void AddEntry(ArpEntry entry);

    /**
     * @brief responds to an ARP request
     * @param packet the ARP request
     * @param dev the device that caught the request
     */
    static void RespondToArp(pcpp::Packet& packet, pcpp::PcapLiveDevice* dev);
	
private:
	static ArpEntry* GetEntry(pcpp::IPv4Address ip);

    /**
     * @brief sends an ARP request
     * @param ip the ip to request MAC from
     */
	static void RequestArp(pcpp::IPv4Address ip);
};

#endif //FIREWALL_ARPTABLE_H
