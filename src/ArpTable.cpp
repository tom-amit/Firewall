//
// Created by tom on 6/2/22.
//

#include <EthLayer.h>
#include "../include/ArpTable.h"

pcpp::MacAddress* ArpTable::Lookup(pcpp::IPv4Address ip){
    ArpEntry* entry = GetEntry(ip);
    if(entry != nullptr){
        return &(entry->mac);
    }
    else{
        RequestArp(ip);
        return nullptr;
    }
}

void ArpTable::AddEntry(ArpEntry entry){
    for(ArpEntry ent : table){
        if(ent.ip == entry.ip){
            ent.mac = entry.mac;
            return;
        }
    }

    table.push_back(entry);
}

ArpEntry* ArpTable::GetEntry(pcpp::IPv4Address ip){
    //loop over all entries in the table with indices
    for(int i = 0; i < table.size(); i++){
        if(table[i].ip == ip){
            return &table[i];
        }
    }

    return nullptr;
}

void ArpTable::RequestArp(pcpp::IPv4Address ip){
    auto arpRequestPacket = pcpp::Packet();
    auto dev = NICS::GetDeviceForIP(ip);
    std::cout << " --------------- Requested ARP from dev ip " << dev->getIPv4Address().toString() << std::endl;

    auto ethernetLayer = new pcpp::EthLayer(dev->getMacAddress(), pcpp::MacAddress("FF:FF:FF:FF:FF:FF"));
    arpRequestPacket.addLayer(ethernetLayer);

    auto arpLayer = new pcpp::ArpLayer(pcpp::ARP_REQUEST, dev->getMacAddress(), pcpp::MacAddress("FF:FF:FF:FF:FF:FF"), dev->getIPv4Address(), ip);
    arpRequestPacket.addLayer(arpLayer);

    arpRequestPacket.computeCalculateFields();

    dev->sendPacket(*arpRequestPacket.getRawPacket());
    std::cout << " ------------- SENT ARP PACKET:" << std::endl << arpRequestPacket.toString(true) << std::endl << std::endl;

}

void ArpTable::RespondToArp(pcpp::Packet &packet, pcpp::PcapLiveDevice *dev) {
    auto arpRequestPacket = pcpp::Packet();
    auto packetLayer = packet.getLayerOfType<pcpp::ArpLayer>();

    auto ethernetLayer = new pcpp::EthLayer(dev->getMacAddress(), packetLayer->getSenderMacAddress());
    arpRequestPacket.addLayer(ethernetLayer);

    auto arpLayer = new pcpp::ArpLayer(pcpp::ARP_REPLY, dev->getMacAddress(), packetLayer->getSenderMacAddress(), dev->getIPv4Address(), packetLayer->getSenderIpAddr());
    arpRequestPacket.addLayer(arpLayer);

    arpRequestPacket.computeCalculateFields();

    dev->sendPacket(*arpRequestPacket.getRawPacket());
}

std::vector<ArpEntry> ArpTable::table = *(new std::vector<ArpEntry>());