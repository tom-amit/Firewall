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
        if (rule->isNotValid())
            continue;
        string r_dir = rule->getDirection(), r_src_ip = get<2>(rule->getSrcIp()), r_dest_ip = get<2>(rule->getDestIp()) ,
                r_protocol = rule->getProtocol(), r_action = rule->getAction();
        Rule::strToFunc(r_action, ::tolower);
        if ((str_equals(r_dir, dir) || str_equals(r_dir, Rule::ANY)) &&
            compare_ip_addresses(r_src_ip, ip_layer->getSrcIPv4Address().toString()) &&
            compare_ip_addresses(r_dest_ip, ip_layer->getDstIPv4Address().toString()) &&
            (srcPort >= get<0>(rule->getSrcPort()) && srcPort <= get<1>(rule->getSrcPort())) &&
            (destPrt >= get<0>(rule->getDestPort()) && destPrt <= get<1>(rule->getDestPort())) &&
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
    //given a rule (contains IP and CIDR) and a target (IP), return true if the target is in the rule's CIDR

    if (rule.find('/') == string::npos) {
        // Assuming both IPs are valid, no need to case check the strings
        std::vector<string> split_src = Rule::split_ip(rule);
        std::vector<string> split_target = Rule::split_ip(target);
        for (auto[r, t] = std::pair{split_src.begin(), split_target.begin()};
             r != split_src.end() && t != split_target.end(); ++r, ++t) {
            if (*r != Rule::IP_ASTERISK && *r != *t) return false;
        }
        return true;
    }
    string ip = rule.substr(0, rule.find('/'));
    string cidr = rule.substr(rule.find('/') + 1);
    if (cidr.size() > 3)
        return false;
    if (cidr.size() == 3) {
        if (cidr[0] == '0')
            cidr = cidr.substr(1);
        else
            return false;

    }
    if (cidr.size() == 2) {
        if (cidr[0] == '0')
            cidr = cidr.substr(1);
        else
            return false;
    }
    if (cidr.size() == 1) {
        if (cidr[0] == '0')
            cidr = cidr.substr(1);
        else
            return false;
    }
    if (cidr.size() == 0)
        return false;
    uint32_t mask = 0;
    for (int i = 0; i < cidr.size(); i++) {
        mask += (cidr[i] - '0') * pow(2, (cidr.size() - i - 1));
    }
    uint32_t ip_int = 0;
    for (int i = 0; i < ip.size(); i++) {
        ip_int += (ip[i] - '0') * pow(2, (ip.size() - i - 1));
    }
    uint32_t target_int = 0;
    for (int i = 0; i < target.size(); i++) {
        target_int += (target[i] - '0') * pow(2, (target.size() - i - 1));
    }
    return (ip_int & mask) == (target_int & mask);

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
        if (r->isNotValid()){
            continue;
        }
        std::cout << std::endl;
        std::cout.width(display_padding);
        std::cout << std::left << r->getName();
        std::cout.width(display_padding);
        std::cout << std::left << r->getDirection();
        std::cout.width(display_padding);
        std::cout << std::left << get<2>(r->getSrcIp());
        std::cout.width(display_padding);
        std::cout << std::left << get<2>(r->getDestIp());
        std::cout.width(display_padding);
        std::cout << std::left << get<2>(r->getSrcPort());
        std::cout.width(display_padding);
        std::cout << std::left << get<2>(r->getDestPort());
        std::cout.width(display_padding);
        std::cout << std::left << r->getProtocol();
        std::cout.width(display_padding);
        std::cout << std::left << r->getAck().second;
        std::cout.width(display_padding);
        std::cout << std::left << r->getAction();
    }
    std::cout << std::endl;
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
        p_AddRule(Rule());
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

std::optional<std::string>
RuleTable::EditRule(const uint16_t id, const string &name, const string &direction, const string &src_ip,
                    const string &dest_ip, const string &src_port, const string &dest_port, const string &protocol,
                    const string &ack, const string &action) {
    uint64_t i = 0;
    for (auto it = table.begin(); it != table.end() - 1; ++i, it++) {
        if (i == id) {
            try{
                if (!name.empty()) {
                    (*it)->setName(name);
                }
                if (!direction.empty()) {
                    (*it)->setDirection(direction);
                }
                if (!src_ip.empty()) {
                    (*it)->setSrcIp(src_ip);
                }
                if (!dest_ip.empty()) {
                    (*it)->setDestIp(dest_ip);
                }
                if (!src_port.empty()) {
                    (*it)->setSrcPort(src_port);
                }
                if (!dest_port.empty()) {
                    (*it)->setDestPort(dest_port);
                }
                if (!protocol.empty()) {
                    (*it)->setProtocol(protocol);
                }
                if (!ack.empty()) {
                    (*it)->setAck(ack);
                }
                if (!action.empty()) {
                    (*it)->setAction(action);
                }
                if ((*it)->isNotValid()) {
                    (*it)->setNotValid(false);
                }
                return SUCCESS;
            }
            catch (const std::exception &e) {
                for(int j = 0;j<TABLE_LENGTH*display_padding;++j){
                    std::cerr << ">";
                }
                std::cerr << std::endl;
                std::cerr << "Encountered parsing error (will not modify rule):\n" << e.what() << std::endl;
                for(int j = 0;j<TABLE_LENGTH*display_padding;++j){
                    std::cerr << "<";
                }
                std::cerr << std::endl;
                table[i]->setNotValid(true);
                return {};
            }
        }
    }
}
