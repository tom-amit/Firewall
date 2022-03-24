//
// Created by karkt on 3/19/2022.
//

#ifndef RULESDAST_RULE_H
#define RULESDAST_RULE_H


#define MAX_PORT 65535

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <optional>
#include <map>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "BadParse.h"
#include "IPv4Layer.h"

using std::string;
using std::to_string;



class Rule {
public:
    static std::vector<string> split_ip(string s);
    //TODO move to be a global variable
    const std::map<string, string> GENERAL_IP {{"any", "*.*.*.*"}};
    const std::vector<string> DIR_DEF{"in", "out", "any"};
    const std::vector<string> ACTION_DEF{"allow", "deny", "any"};
    const std::vector<string> ACK_DEF{"yes", "no", "any"};
    const std::map<string, long unsigned int> PROTOCOL_DEF{{"ANY", 0x111},{"TCP", 0x04}, {"UDP", 0x08}};


    Rule(const string& name,const string& direction, const string& src_ip, const string& src_port, const string& dest_ip, const string& dest_port,const string& protocol, const string& ack, const string& action);

    string getName() const;

    void setName(const string &p_name);

    string getAction() const;

    void setAction(const string &p_action);

    string getDirection() const;

    void setDirection(const string &p_direction);

    std::pair<pcpp::IPv4Address, string> getSrcIp() const;

    void setSrcIp(const string &p_src_ip);

    std::pair<pcpp::IPv4Address, string> getDestIp() const;

    void setDestIp(const string &p_dest_ip);

    std::pair<uint32_t, string> getSrcPort() const;

    void setSrcPort(string p_src_port);

    std::pair<uint32_t, string> getDestPort() const;

    void setDestPort(string p_dest_port);

    string getProtocol() const;

    void setProtocol(string p_protocol);

    std::pair<uint16_t , string> getAck() const;

    void setAck(const string& p_ack);

private:
    string name, action, direction, ack;
    std::pair<pcpp::IPv4Address, string> src_ip, dest_ip;
    uint32_t src_port, dest_port;
    pcpp::ProtocolType protocol;

    string ParseDirection(string p_dir);
    string ParseAction(string p_action);
    std::pair<pcpp::IPv4Address, string> ParseIP(string p_ip_addr);
    uint32_t ParsePort(string p_port_num);
    string ParseAck(string p_ack);
    pcpp::ProtocolType ParseProtocol(string p_protocol);
};


#endif //RULESDAST_RULE_H
