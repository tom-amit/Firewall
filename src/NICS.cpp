//
// Created by ubu on 6/13/22.
//

#include "../include/NICS.h"


std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>>& NICS::getDeviceList() {
    return deviceList;
}

void NICS::GetData(const std::string& dev1, const std::string& dev2) {
    deviceList = std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>>();

    auto ddev1 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(dev1);
    auto ddev2 = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(dev2);

    auto add = (ddev1->getAddresses()[1]);
    char buffer1[INET_ADDRSTRLEN] = {0, };
    inet_ntop(
            2,
            &((struct sockaddr_in*)(add.netmask))->sin_addr,
            buffer1,
            INET_ADDRSTRLEN
    );

    char buffer12[INET_ADDRSTRLEN] = {0, };
    inet_ntop(
            2,
            &((struct sockaddr_in*)(add.addr))->sin_addr,
            buffer12,
            INET_ADDRSTRLEN
    );
    deviceList.emplace_back(ddev1, std::string(buffer12), std::string(buffer1));

    add = (ddev2->getAddresses()[1]);
    char buffer2[INET_ADDRSTRLEN] = {0, };
    inet_ntop(
            2,
            &((struct sockaddr_in*)(add.netmask))->sin_addr,
            buffer2,
            INET_ADDRSTRLEN
    );

    char buffer22[INET_ADDRSTRLEN] = {0, };
    inet_ntop(
            2,
            &((struct sockaddr_in*)(add.addr))->sin_addr,
            buffer22,
            INET_ADDRSTRLEN
    );
    deviceList.emplace_back(ddev2, std::string(buffer22), std::string(buffer2));
}

pcpp::PcapLiveDevice *NICS::GetDeviceForIP(pcpp::IPv4Address ip) {
    for(auto devTuple : deviceList) {
        if(ip.matchSubnet(std::get<1>(devTuple), std::get<2>(devTuple))) {
            return std::get<0>(devTuple);
        }
    }
    return nullptr;
}

std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>> NICS::deviceList = *(new std::vector<std::tuple<pcpp::PcapLiveDevice*, pcpp::IPv4Address, pcpp::IPv4Address>>());
