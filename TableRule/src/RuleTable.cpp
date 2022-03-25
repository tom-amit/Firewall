//
// Created by karkt on 3/19/2022.
//
#include "../include/RuleTable.h"

RuleTable::RuleTable(){
    //TODO change pointers to smart pointer, i.e unique_ptr, shared_ptr
    table =  std::vector<Rule*>{new Rule(string("default"), string("any"), string("any"),string("any"),
                                         string("any"),string("any"),string("any"),string("any"),
                                         string("deny"))};
    __len = 1;
    display_padding = 13;
}
bool RuleTable::ParsePacket(pcpp::Packet &p_packet , const string& dir) {
    //TODO change string constants used here to a defined variable @Rule.h
    //TODO add check that protocol indeed exists inside PROTOCOL_DEF
    //TODO remove all magic strings and numbers if they exist
    auto ip_layer = *p_packet.getLayerOfType<pcpp::IPv4Layer>();
    auto tcp_hdr = *p_packet.getLayerOfType<pcpp::tcphdr>();
    uint32_t srcPort, destPrt;
    //TODO only if have enough free time or either typedef does not work here, final option is to just create all elements before the if in one big ugly if-else
    typedef pcpp::TcpLayer protocol_type;
    string protocol = "";
    if (ip_layer.getProtocol() == 0x04) { //TCP
        typedef pcpp::TcpLayer protocol_type;
        protocol="TCP";
    }
    if (ip_layer.getProtocol() == 0x08) {//UDP
        typedef pcpp::UdpLayer protocol_type;
        protocol="UDP";
    }
    auto layer = p_packet.getLayerOfType<protocol_type>();
    srcPort = static_cast<uint32_t>(layer->getSrcPort());
    destPrt = static_cast<uint32_t>(layer->getDstPort());
    for (auto& rule: table){
        //TODO add direction checking
        if((rule->getDirection() == dir || dir == "any") &&
        compare_ip_addresses(rule->getSrcIp().second, ip_layer.getSrcIPv4Address().toString())&&
        compare_ip_addresses(rule->getDestIp().second, ip_layer.getDstIPv4Address().toString()) &&
                (rule->getSrcPort().first == srcPort || rule->getSrcPort().second == "any") &&
                (rule->getDestPort().first == destPrt || rule->getDestPort().second == "any") &&
                (rule->getProtocol() == protocol || rule->getProtocol() == "any") &&
                (rule->getAck().first == tcp_hdr.ackFlag || rule->getAck().second == "any")){
            if(rule->getAction() == "allow"){
                return true;
            }
            else{
                return false;
            }
        }
    }
    return false;
}

bool RuleTable::compare_ip_addresses(const string &rule, const string &target) {
    std::vector<string> split_src = Rule::split_ip(rule);
    std::vector<string> split_target = Rule::split_ip(target);
    for(auto [r, t] = std::pair{split_src.begin(), split_target.begin()};
        r != split_src.end() && t!=split_target.end(); ++r, ++t){
        if(*r != "*" && *r != *t) return false; // "*" : magic string
    }
    return true;
}
std::optional<string> RuleTable::AddRule(const string &name, const string &direction, const string &src_ip, const string &src_port,
                   const string &dest_ip, const string &dest_port, const string &protocol, const string &ack,
                   const string &action) {
    //TODO make sure that there are no duplicate names
    try{
        return AddRule(Rule(name, direction, src_ip, src_port,dest_ip, dest_port, protocol, ack, action));;
    }
    catch(BadParse& e){
        for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
            std::cout << ">";
        }
        std::cout << std::endl;
        std::cout << "Encountered parsing error (will not create rule):\n" << e.what() << std::endl;
        for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
            std::cout << "<";
        }
        std::cout << std::endl;
        return {};
    }
}

std::optional<string> RuleTable::AddRule(const Rule& rule)  {
    table.insert(table.end()-1, new Rule(rule));
    __len++;
    return "OK"; // "magic string"
}

void RuleTable::DisplayTable(){
    std::cout.width(display_padding); std::cout << std::left << "Name";
    std::cout.width(display_padding); std::cout << std::left << "Direction";
    std::cout.width(display_padding); std::cout << std::left << "Source IP";
    std::cout.width(display_padding); std::cout << std::left << "Source Port";
    std::cout.width(display_padding); std::cout << std::left << "Dest IP";
    std::cout.width(display_padding); std::cout << std::left << "Dest Port";
    std::cout.width(display_padding); std::cout << std::left << "Protocol";
    std::cout.width(display_padding); std::cout << std::left << "ACK";
    std::cout.width(display_padding); std::cout << std::left << "Action";
    std::cout << std::endl;
    for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
        std::cout << "=";
    }
    std::cout << std::endl;
    for(auto& r : table){
        std::cout << std::endl;
        std::cout.width(display_padding); std::cout << std::left << r->getName();
        std::cout.width(display_padding); std::cout << std::left << r->getDirection();
        std::cout.width(display_padding); std::cout << std::left << r->getSrcIp().second;
        std::cout.width(display_padding); std::cout << std::left << r->getSrcPort().second;
        std::cout.width(display_padding); std::cout << std::left << r->getDestIp().second;
        std::cout.width(display_padding); std::cout << std::left << r->getDestPort().second;
        std::cout.width(display_padding); std::cout << std::left << r->getProtocol();
        std::cout.width(display_padding); std::cout << std::left << r->getAck().second;
        std::cout.width(display_padding); std::cout << std::left << r->getAction();
    }
    std::cout << std::endl;
}