//
// Created by karkt on 3/19/2022.
//
#include "../include/RuleTable.h"

RuleTable::RuleTable(): len(1), display_padding(13){
    //TODO change pointers to smart pointer, i.e unique_ptr, shared_ptr
    table =  std::vector<Rule*>{new Rule(string("default"), string("any"), string("any"),string("any"),
                                         string("any"),string("any"),string("any"),string("any"),
                                         string("deny"))};
}
bool RuleTable::ParsePacket(pcpp::Packet &p_packet , const string& dir) {
    //TODO maybe add support for internal/external for IPs?
    //TODO change string constants used here to a defined variable @Rule.h
    //TODO add check that protocol indeed exists inside PROTOCOL_DEF
    //TODO remove all magic strings and numbers if they exist
    auto ip_layer = *p_packet.getLayerOfType<pcpp::IPv4Layer>();
    auto tcp_hdr = *p_packet.getLayerOfType<pcpp::tcphdr>();
    uint32_t srcPort = MAX_PORT+1, destPrt = MAX_PORT+1;
    string protocol = "";
    if (ip_layer.getProtocol() == 0x04) { //TCP
        auto layer = p_packet.getLayerOfType<pcpp::TcpLayer>();
        srcPort = static_cast<uint32_t>(layer->getSrcPort());
        destPrt = static_cast<uint32_t>(layer->getDstPort());
        protocol="TCP";
    }
    if (ip_layer.getProtocol() == 0x08) {//UDP
        auto layer = p_packet.getLayerOfType<pcpp::UdpLayer>();
        srcPort = static_cast<uint32_t>(layer->getSrcPort());
        destPrt = static_cast<uint32_t>(layer->getDstPort());
        protocol="UDP";
    }
    if(srcPort > MAX_PORT || destPrt > MAX_PORT)
        throw std::invalid_argument("Invalid src_port and dest_port");
    for (auto& rule: table){
        string r_dir = rule->getDirection(), r_src_ip = rule->getSrcIp().second, r_dest_ip = rule->getDestIp().second,
            r_protocol = rule->getProtocol();
        if((rule->getDirection() == dir || dir == "any") &&
                compare_ip_addresses(r_src_ip, ip_layer.getSrcIPv4Address().toString())&&
                compare_ip_addresses(r_dest_ip, ip_layer.getDstIPv4Address().toString()) &&
                (rule->getSrcPort().first == srcPort || rule->getSrcPort().second == "any") &&
                (rule->getDestPort().first == destPrt || rule->getDestPort().second == "any") &&
                (r_protocol == protocol || r_protocol == "any") &&
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
    //TODO strings comparison is case insensitive, fix that later...
    for(auto &it : table){
        if(it->getName() == name){
            std::cerr << "No duplicate names are allowed for the rules, RULE: (" << name << ")" <<  std::endl;
            return {};
        }
    }
    try{
        return AddRule(Rule(name, direction, src_ip, src_port,dest_ip, dest_port, protocol, ack, action));;
    }
    catch(BadParse& e){
        for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
            std::cout << ">";
        }
        std::cerr << std::endl;
        std::cerr << "Encountered parsing error (will not create rule):\n" << e.what() << std::endl;
        for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
            std::cerr << "<";
        }
        std::cerr << std::endl;
        return {};
    }
}

std::optional<string> RuleTable::AddRule(const Rule& rule)  {
    table.insert(table.end()-1, new Rule(rule));
    len++;
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
