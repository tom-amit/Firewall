//
// Created by amit on 6/2/22.
//

#ifndef FIREWALL_ARPTABLE_H
#define FIREWALL_ARPTABLE_H

#include <ArpLayer.h>

struct ArpEntry {
	pcpp::IPv4Address ip;
	pcpp::MacAddress mac;
	
	ArpEntry(pcpp::IPv4Address _ip, pcpp::MacAddress _mac){ip = _ip; mac = _mac;}
};

class ArpTable {
public:
	std::vector<ArpEntry> table;
	
	ArpTable();
	
	pcpp::MacAddress Lookup(pcpp::IPv4Address ip);
	
private:
	void AddEntry(ArpEntry entry);
	ArpEntry GetEntry(pcpp::IPv4Address ip);
	
	void RequestArp(pcpp::IPv4Address ip);
};

ArpTable::ArpTable(){
	
}

pcpp::MacAddress ArpTable::Lookup(pcpp::IPv4Address ip){
	ArpEntry entry = GetEntry(ip);
	if(entry != null){
		return entry.mac;
	}
	else{
		RequestArp(ip);
		return null;
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

ArpEntry ArpTable::GetEntry(pcpp::IPv4Address ip){
	for(ArpEntry ent : table){
		if(ent.ip == entry.ip){
			return ent;
		}
	}
	
	return null;
}

void ArpTable::RequestArp(pcpp::IPv4Address ip, pcpp::PcapLiveDevice *dev){
	auto arpRequestPacket = pcpp::Packet();
	
	auto arpLayer = new pcpp::ArpLayer(pcpp::ARP_REQUEST, dev->getMacAddress, pcpp::MacAddress("FF:FF:FF:FF:FF:FF"), dev->getIPv4Address(), ip);
	arpRequestPacket.addLayer(arpLayer);
	
	arpRequestPacket.computeCalculateFields();
	
	dev->sendPacket(*arpRequestPacket.getRawPacket())
}

#endif //FIREWALL_ARPTABLE_H
