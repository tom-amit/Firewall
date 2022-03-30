//
// Created by karkt on 3/19/2022.
//
#ifndef RULESDAST_RULETABLE_H
#define RULESDAST_RULETABLE_H

#define TABLE_LENGTH 9


#include "Rule.h"
#include "Packet.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
class RuleTable {
public:
    RuleTable();

    std::optional<std::string> AddRule(const string &name, const string &direction, const string &src_ip, const string &src_port,
                                       const string &dest_ip, const string &dest_port, const string &protocol, const string &ack,
                                       const string &action);
    void DisplayTable();
    bool ParsePacket(pcpp::Packet& p_packet, const string& dir="any");

private:
    std::optional<std::string> p_AddRule(const Rule& rule);

    std::vector<Rule*> table;
    uint64_t len;
    std::vector<string> rule_names;
    uint16_t display_padding;
    bool compare_ip_addresses(const string& rule, const string& target);
};


#endif //RULESDAST_RULETABLE_H
