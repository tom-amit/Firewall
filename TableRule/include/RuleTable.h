//
// Created by karkt on 3/19/2022.
//
#ifndef RULESDAST_RULETABLE_H
#define RULESDAST_RULETABLE_H

#define TABLE_LENGTH 9
#define DEFAULT_COLOR_COUT "39"
#define INVALID_COLOR_COUT "31"

#include "Rule.h"
#include "Packet.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
#include <memory>
#include <cmath>
#include <bitset>
using std::unique_ptr;
using std::get;
class RuleTable {
public:
    RuleTable();
    const string SUCCESS{"OK"};
    std::optional<std::string> AddRule(const string &name, const string &direction, const string &src_ip,
                                       const string &dest_ip, const string &src_port, const string &dest_port,
                                       const string &protocol, const string &ack,
                                       const string &action);

    void DisplayTable();

    bool ParsePacket(pcpp::Packet &p_packet, const string &dir = "any");

    std::optional<string> AddEmptyRule();
    std::optional<string> SwapRuleTo(uint64_t id1, uint64_t id2);
    std::optional<std::string> RemoveRule(const string &index);
    std::optional<std::string> EditRule(const uint16_t id, const string &name, const string &direction, const string &src_ip,
                                        const string &dest_ip, const string &src_port, const string &dest_port,
                                        const string &protocol, const string &ack,
                                        const string &action);
    std::vector<unique_ptr<Rule>> table;
    static bool compare_ip_addresses(const string &rule, const string &target);
    void clear_table();
private:
    std::optional<std::string> p_AddRule(const Rule &rule);


    uint64_t len;
    std::vector<string> rule_names;
    uint16_t display_padding;

};


#endif //RULESDAST_RULETABLE_H
