//
// Created by ubu1 on 30/03/2022.
//

#include "../include/Firewall.h"

struct Cookie {
    std::vector<ARPAwaitingPacket>* arpAwaitingPackets;
    RuleTable* table;

    Cookie(std::vector<ARPAwaitingPacket>* _arpAwaitingPackets, RuleTable* _table){ arpAwaitingPackets = _arpAwaitingPackets; table = _table; }
};

struct CookieWithDir {
    Cookie* cookie;
    std::string dir;

    CookieWithDir(Cookie* _cookie, std::string _dir){ cookie = _cookie; dir = _dir; }
};

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookieWithDir)
{
    auto *typedCookieWithDir = static_cast<CookieWithDir *>(cookieWithDir);
    auto *typedCookie = typedCookieWithDir->cookie;
    auto dir = typedCookieWithDir->dir;

    pcpp::Packet parsedPacket(packet);

    std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;
    if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() != nullptr)
        std::cout << "TTL of packet: "<< (int)(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive) << std::endl << std::endl;
    //check if the packet is an arp packet
    if (parsedPacket.getLayerOfType<pcpp::ArpLayer>() != nullptr) {
        auto arpLayer = parsedPacket.getLayerOfType<pcpp::ArpLayer>();
        if (arpLayer->getTargetMacAddress() != pcpp::MacAddress::Zero){
            ArpTable::AddEntry(*(new ArpEntry(arpLayer->getSenderIpAddr(), arpLayer->getSenderMacAddress(), dev)));
            std::cout << "added arp entry with the values:" << std::endl << arpLayer->getSenderIpAddr().toString() << std::endl << arpLayer->getSenderMacAddress().toString() << std::endl << std::endl;
            auto *arpAwaitingPackets = typedCookie->arpAwaitingPackets;
            auto it = arpAwaitingPackets->begin();
            while (it != arpAwaitingPackets->end())
            {
                if (it->ip == arpLayer->getSenderIpAddr()) {
                    it->parsedPacket.getLayerOfType<pcpp::EthLayer>()->setDestMac(arpLayer->getSenderMacAddress());
                    it->parsedPacket.computeCalculateFields();
                    std::cout << " ---------------- resending packet" << std::endl << it->parsedPacket.toString(true) << std::endl << std::endl;
                    dev->sendPacket(*it->parsedPacket.getRawPacket());
                    it = arpAwaitingPackets->erase(it);
                }
                else {
                    ++it;
                }
            }
            /*for (auto &awaitingPacket : *arpAwaitingPackets) {
                if (awaitingPacket.ip == arpLayer->getSenderIpAddr()) {
                    awaitingPacket.parsedPacket.getLayerOfType<pcpp::EthLayer>()->setDestMac(arpLayer->getSenderMacAddress());
                    awaitingPacket.parsedPacket.computeCalculateFields();
                    std::cout << " ---------------- resending packet" << std::endl << awaitingPacket.parsedPacket.toString(true) << std::endl << std::endl;
                    dev->sendPacket(*awaitingPacket.parsedPacket.getRawPacket());
                    //remove the packet from the awaiting packets
                    arpAwaitingPackets->erase(awaitingPacket);
                }
            }*/
        }
        else{
            ArpTable::RespondToArp(parsedPacket, dev);
        }
    }
    else {
        if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() != nullptr &&
            parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive < 2) {
            Firewall::SendTTLExpiredPacket(parsedPacket, dev);
            return;
        }

        auto *ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

        if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() == nullptr)
            return;

        auto targetDev = NICS::GetDeviceForIP(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address());
        if (targetDev == nullptr)
            return;

        if (!typedCookie->table->ParsePacket(parsedPacket, dir))
            return;

        //std::cout << "allowed packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

        parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive -= 1;
        ethernet->setSourceMac(pcpp::MacAddress(targetDev->getMacAddress()));
        pcpp::MacAddress *macAddress = ArpTable::Lookup(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address());
        if (macAddress == nullptr) {
            typedCookie->arpAwaitingPackets->insert(typedCookie->arpAwaitingPackets->end(),
                                                    *(new ARPAwaitingPacket(parsedPacket,
                                                                            parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address())));
        } else {
            ethernet->setDestMac(*macAddress);
            parsedPacket.computeCalculateFields();
            targetDev->sendPacket(*parsedPacket.getRawPacket());
            std::cout << "temp packet:" << std::endl << parsedPacket.toString(true);
        }
    }
}

Firewall::Firewall() {
    SetLiveDevices();
    table = new RuleTable();
    arpAwaitingPackets = *(new std::vector<ARPAwaitingPacket>());
}

void Firewall::SetLiveDevices() {
    auto devices = NICS::getDeviceList();
    dev1 = devices[0];
    dev2 = devices[1];
}

void Firewall::OpenLiveDevices() {
    std::cout << "Devices:";
    for (auto &device : NICS::getDeviceList()){
        std::cout << " " << std::get<0>(device)->getName();
    }
    std::cout << std::endl;
    std::get<0>(dev1)->open();
    std::get<0>(dev2)->open();

    if(dev1filter != nullptr) {
        std::get<0>(dev1)->clearFilter();
        delete dev1filter;
    }
    if(dev2filter != nullptr) {
        std::get<0>(dev2)->clearFilter();
        delete dev2filter;
    }
    dev1filter = (new pcpp::MacAddressFilter(std::get<0>(dev1)->getMacAddress(), pcpp::DST));
    dev2filter = (new pcpp::MacAddressFilter(std::get<0>(dev2)->getMacAddress(), pcpp::DST));

    std::get<0>(dev1)->setFilter(*dev1filter);
    std::get<0>(dev2)->setFilter(*dev2filter);;
}

void Firewall::StartLiveDevicesCapture() {
    auto *cookie = (new Cookie(&arpAwaitingPackets, table));
    auto *cookieWithDir1 = (new CookieWithDir(cookie, "out"));
    auto *cookieWithDir2 = (new CookieWithDir(cookie, "in"));
    std::get<0>(dev1)->startCapture(onPacketArrives, cookieWithDir1);
    std::get<0>(dev2)->startCapture(onPacketArrives, cookieWithDir2);
}
void Firewall::CloseLiveDevices() {
	std::get<0>(dev1)->close();
	std::get<0>(dev2)->close();
}

void Firewall::StopLiveDevicesCapture() {
    std::get<0>(dev1)->stopCapture();
    std::get<0>(dev2)->stopCapture();
}

void Firewall::Run() {
    OpenLiveDevices();
    StartLiveDevicesCapture();
}

void Firewall::Stop() {
    StopLiveDevicesCapture();
	CloseLiveDevices();
}

void Firewall::SendTTLExpiredPacket(const pcpp::Packet &expiredPacket, pcpp::PcapLiveDevice *dev) {
    std::cout << "src expired: " << expiredPacket.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPv4Address().toString() << std::endl;
    std::cout << "dst expired: " << expiredPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address().toString() << std::endl;

    std::cout << "current IP: " << dev->getIPv4Address().toString() << std::endl;
    auto TTLExpired = pcpp::Packet();

    // Create the Ethernet layer
    auto expiredEthLayer = expiredPacket.getLayerOfType<pcpp::EthLayer>();
    auto ethLayer = new pcpp::EthLayer(expiredEthLayer->getDestMac(), expiredEthLayer->getSourceMac(), pcpp::IPv4);
    TTLExpired.addLayer(ethLayer);

    // Create the IPv4 layer
    auto expiredIPv4Layer = new pcpp::IPv4Layer(expiredPacket.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPv4Address(),
                                                expiredPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address());
    auto ipv4Layer = new pcpp::IPv4Layer(dev->getIPv4Address(), expiredIPv4Layer->getSrcIPv4Address());

	expiredIPv4Layer->getIPv4Header()->timeToLive = 0;
	ipv4Layer->getIPv4Header()->timeToLive = 128;

	//print both IP addresses
	std::cout << "src ip: " << ipv4Layer->getSrcIPv4Address().toString() << std::endl;
	std::cout << "dst ip: " << ipv4Layer->getDstIPv4Address().toString() << std::endl;
    TTLExpired.addLayer(ipv4Layer);

    // Create the ICMP layer
    pcpp::Layer *expiredL4Layer = nullptr;
    if (expiredPacket.getLayerOfType<pcpp::UdpLayer>() != nullptr)
        expiredL4Layer = new pcpp::UdpLayer(expiredPacket.getLayerOfType<pcpp::UdpLayer>()->getSrcPort(),
                                            expiredPacket.getLayerOfType<pcpp::UdpLayer>()->getDstPort());
    else if (expiredPacket.getLayerOfType<pcpp::TcpLayer>() != nullptr)
        expiredL4Layer = new pcpp::TcpLayer(expiredPacket.getLayerOfType<pcpp::TcpLayer>()->getSrcPort(),
                                            expiredPacket.getLayerOfType<pcpp::TcpLayer>()->getDstPort());

    auto icmpLayer = new pcpp::IcmpLayer();
    TTLExpired.addLayer(icmpLayer);
    icmpLayer->setTimeExceededData(0, expiredIPv4Layer, expiredL4Layer);

    TTLExpired.computeCalculateFields();

    std::cout << "sending ttl expired packet:" << std::endl << TTLExpired.toString(true);
    std::cout << "TTL of TTLExpired " << (int)(TTLExpired.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive) << std::endl << std::endl;
    dev->sendPacket(*TTLExpired.getRawPacket());
}


