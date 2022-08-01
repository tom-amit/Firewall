#ifndef FIREWALL_FIREWALL_H
#define FIREWALL_FIREWALL_H

#include <EthLayer.h>
#include <PcapLiveDeviceList.h>
#include <IPv4Layer.h>
#include "../TableRule/include/RuleTable.h"
#include <IcmpLayer.h>
#include "NICS.h"
#include "ArpTable.h"
/**
 * @brief a packet and an ip of arp entry missing
 */
struct ARPAwaitingPacket {
    pcpp::Packet parsedPacket;
    pcpp::IPv4Address ip;

    ARPAwaitingPacket(const pcpp::Packet& _parsedPacket, pcpp::IPv4Address _ip) {
        parsedPacket = _parsedPacket;
        ip = _ip;
    }

    ARPAwaitingPacket() = default;
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

    shared_ptr<RuleTable> table;

    void Run();
    void Stop();

    /**
     * @brief Sends the appropriate TTL Eexpired packet for a packet
     * @param expiredPacket the expired packet
     * @param dev the device which caught the packet
     */
    static void SendTTLExpiredPacket(const pcpp::Packet &expiredPacket, pcpp::PcapLiveDevice *dev);
};


#endif //FIREWALL_FIREWALL_H
