//
// Created by karkt on 3/19/2022.
//

//TODO make sure there are no memory leaks since we moved to unique_ptr!
#include "../include/RuleTable.h"
//TODO Add swap order of rules function (pretty simple for two rules)
template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
    if (oldIndex > newIndex)
        std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
    else
        std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}


RuleTable::RuleTable() : len(1), display_padding(22) {
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

string number_to_bin(const string num){
    return std::bitset<8>(stoi(num)).to_string();
}

string ip_to_bin(const string &ip) {
    std::vector<string> octets = Rule::split_ip(ip);
    string bin_ip = "";
    //iterate through octets and convert to binary with dots in between each octet
    for (auto octet : octets) {
        bin_ip += number_to_bin(octet);
    }
    return bin_ip;
}
string mask_ip(const string &ip, const string &mask) {
    //given an IP and a mask, return the masked IP
    string masked = "";
    for (auto [i, j] = std::pair{ip.begin(), mask.begin()}; i != ip.end() && j != mask.end(); ++i, ++j) {
        if (*j == '.') {
            masked += '.';
            continue;
        }
        if (*j == '1') masked += *i;
        else masked += '0';
    }
    return masked;
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
    string cidr = rule;
    //check if target is in cidr range
    string ip = cidr.substr(0, cidr.find('/'));
    int mask = stoi(cidr.substr(cidr.find('/') + 1));
    if (mask < 0 || mask > 32) return false;
    if (mask == 0) return true;
    if (mask == 32) {
        if (ip == target) return true;
        return false;
    }
    //check if target is in cidr range
    string mask_str = "";
    for (int i = 0; i < mask; ++i) mask_str += "1";
    for (int i = mask; i < 32; ++i) mask_str += "0";
    string target_bin = ip_to_bin(target);
    string ip_bin = ip_to_bin(ip);
    string target_masked = mask_ip(target_bin, mask_str);
    string ip_masked = mask_ip(ip_bin, mask_str);
    if (target_masked == ip_masked) return true;
    return false;
}
void RuleTable::DisplayTable() {
    std::cout.width(display_padding);
    std::cout << std::left << "Index";
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
    for (int i = 0; i < round(TABLE_LENGTH * display_padding*1.1); ++i) {
        std::cout << "=";
    }
    std::cout << std::endl;
    uint64_t i = 0;
    for(auto& r : table) {
        if (r->isNotValid()){
            ++i;
            continue;
        }
        std::cout.width(display_padding);
        std::cout << std::left << i;
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
        std::cout << std::endl;
        ++i;
    }
}

std::optional<string> RuleTable::AddRule(const string &name, const string &direction, const string &src_ip,
                                         const string &dest_ip, const string &src_port, const string &dest_port,
                                         const string &protocol, const string &ack,
                                         const string &action) {
    for (auto &it: table) {
        if (str_equals(it->getName(), name)) {
            std::cerr << "No duplicate names are allowed for the rules, RULE: (" << name << ")" << std::endl;
            p_AddRule(Rule());
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

std::optional<string> RuleTable::AddEmptyRule(){
    try{
        p_AddRule(Rule());
        return SUCCESS;
    }
    catch(const std::exception& e){
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

std::optional<std::string> RuleTable::RemoveRule(const string &index) {
    try {
        int i = std::stoi(index);
        if (i < 0 || i >= len-1) {
            std::cerr << "Index out of bounds" << std::endl;
            return {};
        }
        table.erase(table.begin() + i);
        len--;
        return SUCCESS;
    }
    catch (std::invalid_argument &e) {
        std::cerr << "Invalid index or not an index" << std::endl;
        return {};
    }
}

std::optional<std::string>
RuleTable::EditRule(const uint16_t id, const string &name, const string &direction, const string &src_ip,
                    const string &dest_ip, const string &src_port, const string &dest_port, const string &protocol,
                    const string &ack, const string &action) {
    uint64_t i = 0;
    for (auto it = table.begin(); it != table.end(); it++, ++i) {
        if (i != id && (*it)->getName() == name) {
            std::cerr << "No duplicate names are allowed for the rules, RULE: (" << name << ")" << std::endl;
            uint64_t j = 0;
            for (auto it = table.begin(); it != table.end(); it++, ++j) {
                if(j == id) {
                    (*it)->setNotValid(true);
                    break;
                }
            }
            return {};
        }
    }
    i = 0;
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
                (*it)->setNotValid(false);
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

std::optional<string> RuleTable::SwapRuleTo(uint64_t id1, uint64_t id2) {
    // given 2 ids, swap the rules at those ids, table is built of unique pointers
    if (id1 >= len-1 || id2 >= len-1) {
        std::cerr << "Invalid index" << std::endl;
        return {};
    }
    move(table, id1, id2);
    return SUCCESS;
}

void RuleTable::clear_table() {
    //clear the table of all rules except the last one
    table.erase(table.begin(), table.end() - 1);
}
