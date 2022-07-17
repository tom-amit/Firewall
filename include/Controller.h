//
// Created by tk on 30/03/2022.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>
#include "../TableRule/include/RuleTable.h"
#include "Firewall.h"

#ifndef FIREWALL_CONTROLLER_H
#define FIREWALL_CONTROLLER_H


using std::string;

class Controller {
private:
    typedef bool (Controller::*MFP)(const std::vector<string> &args);

    Firewall firewall;
	bool firewallStopped;

    std::map<string, std::pair<u_int16_t, MFP>> cmd_map;
public:
    Controller();

    bool add_rule(const std::vector<string> &args);

    bool remove_rule(const std::vector<string> &args);

    bool edit_rule(const std::vector<string> &args);

    bool show_rules(const std::vector<string> &args);

    bool start(const std::vector<string> &args);

    bool stop(const std::vector<string> &args);

    bool run();

    bool swap_rule_to(const std::vector<string> &args);

    bool reset_firewall() const;

    std::vector<uint64_t> get_hit_counts() const;
};


#endif //FIREWALL_CONTROLLER_H
