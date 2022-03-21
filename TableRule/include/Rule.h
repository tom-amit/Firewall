//
// Created by karkt on 3/19/2022.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <optional>
#include <map>
#include <iostream>
#include <cstdlib>
#include "IPv4Layer.h"
#ifndef RULESDAST_RULE_H
#define RULESDAST_RULE_H


using std::string;
using std::to_string;



class Rule {
public:
    Rule(const string& name,const string& direction, const string& src_ip, const string& src_port, const string& dest_ip, const string& dest_port,const string& protocol, const string& ack, const string& action);

    string getName() const;

    void setName(const string &name);

    string getAction() const;

    void setAction(const string &action);

    string getDirection() const;

    void setDirection(const string &direction);

    pcpp::IPv4Address getSrcIp() const;

    void setSrcIp(const pcpp::IPv4Address &srcIp);

    pcpp::IPv4Address getDestIp() const;

    void setDestIp(const pcpp::IPv4Address &destIp);

    uint16_t getSrcPort() const;

    void setSrcPort(uint16_t srcPort);

    uint16_t getDestPort() const;

    void setDestPort(uint16_t destPort);

    string getProtocol() const;

    void setProtocol(pcpp::ProtocolType protocol);

    string getAck() const;

    void setAck(const string& ack);

private:
    string name, action, direction, ack;
    pcpp::IPv4Address src_ip, dest_ip;
    uint16_t src_port, dest_port;
    pcpp::ProtocolType protocol;

    //TODO move to be a global variable
    std::vector<string> DIR_DEF{"in", "out", "any"};
    std::vector<string> ACTION_DEF{"allow", "deny", "any"};
    std::vector<string> ACK_DEF{"yes", "no", "any"};
    std::map<string, long unsigned int> PROTOCOL_DEF{{"TCP", 0x04}, {"UDP", 0x08}};
    //std::map<long unsigned int, string> PROTOCOL_HEX_DEF{{0x04, "TCP"}, {0x08, "UDP"}};

    std::string ParseDirection(string dir);
    std::string ParseAction(string action);
    pcpp::IPv4Address ParseIP(string ip_addr);
    uint16_t ParsePort(const string& port_num);
    string ParseAck(string ack);
    pcpp::ProtocolType ParseProtocol(string protocol);

};


#endif //RULESDAST_RULE_H
