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
    /**
     * @brief gets data about NICS by their names
     * @param dev1 first device name
     * @param dev2 second device name
     */
    static void GetData(const std::string& dev1, const std::string& dev2);

    /**
     * @return a list of the device's data
     */
    static std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>>& getDeviceList();

    /**
     * @param ip ip address
     * @return the device on the network of the ip
     */
    static pcpp::PcapLiveDevice* GetDeviceForIP(pcpp::IPv4Address ip);
private:
    static std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>> deviceList;

};


#endif //FIREWALL_NICS_H
