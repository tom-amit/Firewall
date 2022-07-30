#include "../include/Firewall.h"

#include <utility>

/**
 * @brief contains needed information for the packet callback function
 */
struct Cookie {
    std::vector<ARPAwaitingPacket>* arpAwaitingPackets;
    shared_ptr<RuleTable> table;

	Cookie(std::vector<ARPAwaitingPacket>* _arpAwaitingPackets, shared_ptr<RuleTable> _table) : arpAwaitingPackets(_arpAwaitingPackets), table(std::move(_table)) {}
};

/**
 * @brief contains needed information for the packet callback function + what direction the device is in
 */
struct CookieWithDir {
    unique_ptr<Cookie> cookie;
    std::string dir;

    CookieWithDir(Cookie* _cookie, std::string _dir){ cookie.reset(_cookie); dir = std::move(_dir); }
};

/**
 * @brief Callback function for when a packet arrives, does the packet routing
 * @param packet The packet that arrived
 * @param dev The PcapLiveDevice that caught the packet
 * @param cookie a parameter that is transferred to all calls of this function, contains needed shared information
 */
static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    auto typedCookie = static_cast<Cookie*>(cookie);

    pcpp::Packet parsedPacket(packet);

    //std::cout << "packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

    if (parsedPacket.getLayerOfType<pcpp::ArpLayer>() != nullptr) {
        auto arpLayer = parsedPacket.getLayerOfType<pcpp::ArpLayer>();
        if (arpLayer->getTargetMacAddress() != pcpp::MacAddress::Zero){
            // for ARP responses, add entry to ARP table, and release related arp awaiting packets
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
        }
        else{
            // for ARP Requests, respond appropriately
            ArpTable::RespondToArp(parsedPacket, dev);
        }
    }
    else {
        /// check for the existence of an ipv4 layer
        if (parsedPacket.getLayerOfType<pcpp::IPv4Layer>() == nullptr)
            return;

        auto ipv4Layer = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();

        /// check if the packet is destined for the firewall, if so then drop it
        if (ipv4Layer->getDstIPv4Address() == std::get<1>(NICS::getDeviceList()[0]) || ipv4Layer->getDstIPv4Address() == std::get<1>(NICS::getDeviceList()[1]))
            return;

        /// check for expired TTL
        if (ipv4Layer->getIPv4Header()->timeToLive < 2) {
            Firewall::SendTTLExpiredPacket(parsedPacket, dev);
            return;
        }

        auto *ethernet = parsedPacket.getLayerOfType<pcpp::EthLayer>();

        /// get the device for the destination ip
        auto targetDev = NICS::GetDeviceForIP(ipv4Layer->getDstIPv4Address());
        if (targetDev == nullptr)
            return;

        /// check that the packet passes the RuleTable
        if (!typedCookie->table->ParsePacket(parsedPacket))
            return;

        //std::cout << "allowed packet:" << std::endl << parsedPacket.toString(true) << std::endl << std::endl;

        /// update TTL
        ipv4Layer->getIPv4Header()->timeToLive -= 1;

        ethernet->setSourceMac(pcpp::MacAddress(targetDev->getMacAddress()));
        pcpp::MacAddress *macAddress = ArpTable::Lookup(parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address());
        if (macAddress == nullptr) {
            /// if MAC is not in ARP table, add to awaiting packets and sends ARP request
            typedCookie->arpAwaitingPackets->insert(typedCookie->arpAwaitingPackets->end(),
                                                    *(new ARPAwaitingPacket(parsedPacket,
                                                                            parsedPacket.getLayerOfType<pcpp::IPv4Layer>()->getDstIPv4Address())));
			//print packet to see what is being sent
			return;
        } else {
            /// send packet
            ethernet->setDestMac(*macAddress);
            parsedPacket.computeCalculateFields();
            targetDev->sendPacket(*parsedPacket.getRawPacket());
            //std::cout << "temp packet:" << std::endl << parsedPacket.toString(true);
        }
    }
}

Firewall::Firewall() {
    SetLiveDevices();
    table.reset(new RuleTable());
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
    std::get<0>(dev1)->startCapture(onPacketArrives, cookie);
    std::get<0>(dev2)->startCapture(onPacketArrives, cookie);
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

    //std::cout << "sending ttl expired packet:" << std::endl << TTLExpired.toString(true);
    //std::cout << "TTL of TTLExpired " << (int)(TTLExpired.getLayerOfType<pcpp::IPv4Layer>()->getIPv4Header()->timeToLive) << std::endl << std::endl;
    dev->sendPacket(*TTLExpired.getRawPacket());
}


