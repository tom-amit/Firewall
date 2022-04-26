//
// Created by karkt on 3/19/2022.
//

//TODO make sure there are no memory leaks since we moved to unique_ptr!
#include "../include/RuleTable.h"

RuleTable::RuleTable() : len(1), display_padding(20) {
    table = std::vector<unique_ptr<Rule>>{};
    table.emplace_back(new Rule(string("default"), string("any"), string("any"), string("any"),
                                string("any"), string("any"), string("any"), string("any"),
                                string("deny")));
}

bool str_equals(const string &a, const string &b) {
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    return true;
}

bool RuleTable::ParsePacket(pcpp::Packet &p_packet, const string &dir) {
    auto ip_layer = p_packet.getLayerOfType<pcpp::IPv4Layer>();
    pcpp::tcphdr* tcp_hdr;
    uint32_t srcPort = MAX_PORT + 1, destPrt = MAX_PORT + 1;
    string protocol = "";
    if (p_packet.getLayerOfType<pcpp::TcpLayer>() != nullptr) {//TCP
        auto layer = p_packet.getLayerOfType<pcpp::TcpLayer>();
        srcPort = static_cast<uint32_t>(layer->getSrcPort());
        destPrt = static_cast<uint32_t>(layer->getDstPort());
        tcp_hdr = layer->getTcpHeader();
        protocol = "TCP";
    } else if (p_packet.getLayerOfType<pcpp::UdpLayer>() != nullptr) {//UDP
        auto layer = p_packet.getLayerOfType<pcpp::UdpLayer>();
        srcPort = static_cast<uint32_t>(layer->getSrcPort());
        destPrt = static_cast<uint32_t>(layer->getDstPort());
        protocol = "UDP";
    }

    for (auto &rule: table) {
        string r_dir = rule->getDirection(), r_src_ip = rule->getSrcIp().second, r_dest_ip = rule->getDestIp().second,
                r_protocol = rule->getProtocol(), r_action = rule->getAction();
        Rule::strToFunc(r_action, ::tolower);
        if ((str_equals(r_dir, dir) || str_equals(r_dir, Rule::ANY)) &&
            compare_ip_addresses(r_src_ip, ip_layer->getSrcIPv4Address().toString()) &&
            compare_ip_addresses(r_dest_ip, ip_layer->getDstIPv4Address().toString()) &&
            (rule->getSrcPort().first == srcPort || str_equals(rule->getSrcPort().second, Rule::ANY)) &&
            (rule->getDestPort().first == destPrt || str_equals(rule->getDestPort().second, Rule::ANY)) &&
            (str_equals(r_protocol, protocol) || str_equals(r_protocol, Rule::ANY)) &&
            (!str_equals(protocol, "TCP") || rule->getAck().first == tcp_hdr->ackFlag || str_equals(rule->getAck().second, Rule::ANY))) {
            if (auto it{Rule::ACTION_DEF.find(r_action)}; it != Rule::ACTION_DEF.end()) {
                return Rule::ACTION_DEF.at(r_action);
            }
        }
    }
    return false;
}

bool RuleTable::compare_ip_addresses(const string &rule, const string &target) {
    // Assuming both IPs are valid, no need to case check the strings
    std::vector<string> split_src = Rule::split_ip(rule);
    std::vector<string> split_target = Rule::split_ip(target);
    for (auto[r, t] = std::pair{split_src.begin(), split_target.begin()};
         r != split_src.end() && t != split_target.end(); ++r, ++t) {
        if (*r != Rule::IP_ASTERISK && *r != *t) return false;
    }
    return true;
}

std::optional<string> RuleTable::AddRule(const string &name, const string &direction, const string &src_ip,
                                         const string &dest_ip, const string &src_port, const string &dest_port,
                                         const string &protocol, const string &ack,
                                         const string &action) {
    for (auto &it: table) {
        if (str_equals(it->getName(), name)) {
            std::cerr << "No duplicate names are allowed for the rules, RULE: (" << name << ")" << std::endl;
            return {};
        }
    }
    try {
        return p_AddRule(Rule(name, direction, src_ip, dest_ip, src_port, dest_port, protocol, ack, action));
    }
    catch(BadParse& e){
        for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
            std::cerr << ">";
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

std::optional<string> RuleTable::p_AddRule(const Rule &rule) {
    table.emplace(table.end() - 1, new Rule(rule));
    len++;
    return SUCCESS;
}

std::optional<std::string> RuleTable::RemoveRule(const string &name) {
    for (auto[i, it] = std::pair{0, table.begin()}; it != table.end() - 1; ++i, it++) {
        if ((*it)->getName() == name) {
            table.erase(it);
            len--;
            return SUCCESS;
        }
    }
    return {};
}

void RuleTable::DisplayTable() {
    std::cout.width(display_padding);
    std::cout << std::left << "Name";
    std::cout.width(display_padding);
    std::cout << std::left << "Direction";
    std::cout.width(display_padding);
    std::cout << std::left << "Source IP";
    std::cout.width(display_padding);
    std::cout << std::left << "Dest IP";
    std::cout.width(display_padding);
    std::cout << std::left << "Source Port";
    std::cout.width(display_padding);
    std::cout << std::left << "Dest Port";
    std::cout.width(display_padding);
    std::cout << std::left << "Protocol";
    std::cout.width(display_padding);
    std::cout << std::left << "ACK";
    std::cout.width(display_padding);
    std::cout << std::left << "Action";
    std::cout << std::endl;
    for (int i = 0; i < TABLE_LENGTH * display_padding; ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
    for(auto& r : table) {
        std::cout << std::endl;
        std::cout.width(display_padding);
        std::cout << std::left << r->getName();
        std::cout.width(display_padding);
        std::cout << std::left << r->getDirection();
        std::cout.width(display_padding);
        std::cout << std::left << r->getSrcIp().second;
        std::cout.width(display_padding);
        std::cout << std::left << r->getDestIp().second;
        std::cout.width(display_padding);
        std::cout << std::left << r->getSrcPort().second;
        std::cout.width(display_padding);
        std::cout << std::left << r->getDestPort().second;
        std::cout.width(display_padding);
        std::cout << std::left << r->getProtocol();
        std::cout.width(display_padding);
        std::cout << std::left << r->getAck().second;
        std::cout.width(display_padding);
        std::cout << std::left << r->getAction();
    }
    std::cout << std::endl;
}
