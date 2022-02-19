#include <iostream>
#include <IPv4Layer.h>
#include <Packet.h>
#include <PcapFileDevice.h>
#include "PcapLiveDeviceList.h"
#include "stdlib.h"
#include "EthLayer.h"
#include "VlanLayer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "UdpLayer.h"
#include "DnsLayer.h"
using namespace std;
int main(int argc, char* argv[])
{	/*
	// open a pcap file for reading
	pcpp::PcapFileReaderDevice reader("1_packet.pcap");
	if (!reader.open())
	{
		printf("Error opening the pcap file\n");
		return 1;
	}

	// read the first (and only) packet from the file
	pcpp::RawPacket rawPacket;
	if (!reader.getNextPacket(rawPacket))
	{
		printf("Couldn't read the first packet in the file\n");
		return 1;
	}

	// parse the raw packet into a parsed packet
	pcpp::Packet parsedPacket(&rawPacket);

	// verify the packet is IPv4
	if (parsedPacket.isPacketOfType(pcpp::IPv4))
	{
		// extract source and dest IPs
		pcpp::IPv4Address srcIP = parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPv4Address();
		pcpp::IPv4Address destIP = parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address();

		// print source and dest IPs
		printf("Source IP is '%s'; Dest IP is '%s'\n", srcIP.toString().c_str(), destIP.toString().c_str());
	}

	// close the file
	reader.close();
	*/
	string interfaceIPAddr = "192.168.1.22";

	// find the interface by IP address
	pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp(interfaceIPAddr);
	if (dev == NULL)
	{
		cerr << "Cannot find interface with IPv4 address of '" << interfaceIPAddr << "'" << endl;
		return 1;
	}
	// open the device before start capturing/sending packets
	if (!dev->open())
	{
		std::cerr << "Cannot open device" << std::endl;
		return 1;
	}

	pcpp::EthLayer newEthernetLayer(pcpp::MacAddress("00:50:43:11:22:33"), pcpp::MacAddress("aa:bb:cc:dd:ee"));
	pcpp::IPv4Layer newIPLayer(pcpp::IPv4Address(std::string("192.168.1.22")), pcpp::IPv4Address(std::string("10.0.0.1")));
	newIPLayer.getIPv4Header()->ipId = htons(2000);
	newIPLayer.getIPv4Header()->timeToLive = 64;
	pcpp::UdpLayer newUdpLayer(12345, 53);

	pcpp::DnsLayer newDnsLayer;
	newDnsLayer.addQuery("www.ebay.com", pcpp::DNS_TYPE_A, pcpp::DNS_CLASS_IN);
	pcpp::Packet newPacket(100);
	newPacket.addLayer(&newEthernetLayer);
	newPacket.addLayer(&newIPLayer);
	newPacket.addLayer(&newUdpLayer);
	newPacket.addLayer(&newDnsLayer);
	newPacket.computeCalculateFields();

	pcpp::PcapFileWriterDevice writer2("1_new_packet.pcap");
	writer2.open();
	writer2.writePacket(*(newPacket.getRawPacket()));
	writer2.close();

	if (!dev->sendPacket(&newPacket))
	{
		std::cerr << "Couldn't send packet" << std::endl;
		return 1;
	}
	return 0;
}
