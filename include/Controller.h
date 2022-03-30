//
// Created by tk on 30/03/2022.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>
#include "../TableRule/include/RuleTable.h"
#ifndef FIREWALL_CONTROLLER_H
#define FIREWALL_CONTROLLER_H


using std::string;

class Controller {
private:
    typedef void (Controller::*MFP)(const std::vector<string> &args);

    RuleTable table;

    void add_rule(const std::vector<string> &args);

    void remove_rule(const std::vector<string> &args);

    void show_rules(const std::vector<string> &args);

    void start(const std::vector<string> &args);

    void stop(const std::vector<string> &args);

    std::map<string, std::pair<u_int16_t, MFP>> cmd_map;
public:
    Controller();

    void run();
};


#endif //FIREWALL_CONTROLLER_H
