//
// Created by karkt on 3/19/2022.
//

#ifndef RULESDAST_RULE_H
#define RULESDAST_RULE_H


#define MAX_PORT 65535
#define IRRELEVANT_CIDR 65535

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
    Rule(); // For invalid rules

    static std::vector<string> split_ip(const string &s);

    static void strToFunc(string &str, int (*func)(int));

    const static inline string ANY{"any"};
    const static inline string IP_ASTERISK{"*"};
    const static inline std::map<string, string> GENERAL_IP{{"any", "*.*.*.*"}};
    const static inline std::vector<string> DIR_DEF{"in", "out", "any"};
    const static inline std::map<string, bool> ACTION_DEF{{"allow", true},
                                                          {"deny",  false}};
    const static inline std::map<string, int16_t> ACK_DEF{{"yes", 1},
                                                          {"no",  0},
                                                          {"any", -1}};
    const static inline std::map<string, long unsigned int> PROTOCOL_DEF{{"ANY", 0x111},
                                                                         {"TCP", 0x04},
                                                                         {"UDP", 0x08}};


    Rule(const string &name, const string &direction, const string &src_ip,
         const string &dest_ip, const string &src_port, const string &dest_port, const string &protocol,
         const string &ack,
         const string &action);

    [[nodiscard]] string getName() const;

    void setName(const string &p_name);

    [[nodiscard]] string getAction() const;

    void setAction(const string &p_action);

    [[nodiscard]] string getDirection() const;

    void setDirection(const string &p_direction);

    [[nodiscard]] std::tuple<pcpp::IPv4Address, uint16_t, string> getSrcIp() const;

    void setSrcIp(const string &p_src_ip);

    [[nodiscard]] std::tuple<pcpp::IPv4Address, uint16_t ,string> getDestIp() const;

    void setDestIp(const string &p_dest_ip);

    [[nodiscard]] std::tuple<uint32_t, uint32_t, string> getSrcPort() const;

    void setSrcPort(string p_src_port);

    [[nodiscard]] std::tuple<uint32_t, uint32_t, string> getDestPort() const;

    void setDestPort(string p_dest_port);

    [[nodiscard]] string getProtocol() const;

    void setProtocol(string p_protocol);

    [[nodiscard]] std::pair<int16_t, string> getAck() const;

    void setAck(const string &p_ack);

    [[nodiscard]] bool isNotValid() const;
    void setNotValid(const bool p_notValid);

private:
    string name, action, direction, ack;

    //IP, CIDR, STR-IP
    std::tuple<pcpp::IPv4Address, uint16_t, string> src_ip, dest_ip;
    std::pair<uint32_t, uint32_t> src_port, dest_port;
    pcpp::ProtocolType protocol;
    bool isInvalid;

    static string ParseDirection(string dir);

    static string ParseAction(string p_action);

    static std::tuple<pcpp::IPv4Address, uint16_t, string> ParseIP(string p_ip_addr);

    static std::pair<uint32_t, uint32_t>  ParsePort(string p_port_num);

    static string ParseAck(string p_ack);

    static pcpp::ProtocolType ParseProtocol(string p_protocol);
};


#endif //RULESDAST_RULE_H
