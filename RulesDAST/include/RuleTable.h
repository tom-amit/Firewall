//
// Created by karkt on 3/19/2022.
//
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <optional>

#include "../../PcapPlusPlus/header/IPv4Layer.h"
#include "Rule.h"
#ifndef RULESDAST_RULETABLE_H
#define RULESDAST_RULETABLE_H


class RuleTable {
    RuleTable();
    const std::optional<std::string> AddRule(const std::string& name, const pcpp::IPAddress& src_ip, const uint16_t& src_port, const pcpp::IPAddress& dest_ip, const uint16_t& dest_port, const std::byte& ack, const std::string& action);
private:
    std::vector<Rule> table;
    uint64_t count;
};


#endif //RULESDAST_RULETABLE_H
