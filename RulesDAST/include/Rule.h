//
// Created by karkt on 3/19/2022.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <optional>

#include "../../PcapPlusPlus/header/IPv4Layer.h"
#ifndef RULESDAST_RULE_H
#define RULESDAST_RULE_H

using std::string;
using std::to_string;

class Rule {
public:
    Rule(const string& name,const string& direction, const string& src_ip, const string& src_port, const string& dest_ip, const string& dest_port,const string& protocol, const string& ack, const string& action);
    string name, action, direction, src_ip, dest_ip, src_port, dest_port, protocol, ack;
    //pcpp::IPAddress src_ip, dest_ip;
    //uint16_t src_port, dest_port;
    //pcpp::ProtocolType protocol;
    //std::byte ack;
};


#endif //RULESDAST_RULE_H
