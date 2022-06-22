//
// Created by ubu on 6/13/22.
//

#ifndef FIREWALL_NICS_H
#define FIREWALL_NICS_H

#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <pcap/pcap.h>

using std::string;

class NICS {
public:
    static void GetData(const std::string& dev1, const std::string& dev2);

    static std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>>& getDeviceList();

    static pcpp::PcapLiveDevice* GetDeviceForIP(pcpp::IPv4Address ip);
private:
    static std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>> deviceList;

};


#endif //FIREWALL_NICS_H
