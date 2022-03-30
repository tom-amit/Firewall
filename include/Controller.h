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
    static RuleTable table;
    //TODO find alternative of the below functions (not using the static keyword)
    static void add_rule(std::vector<string> args);
    static void remove_rule(std::vector<string> args);
    static void show_rules(std::vector<string> args);
    static void start(std::vector<string> args);
    static void stop(std::vector<string> args);
    const std::map<string, std::pair<u_int16_t , void(*)(std::vector<string>)>> cmd_map{
            {"add-rule", {9, &add_rule}},
            {"remove-rule", {1, &remove_rule}},
            {"show-rules", {0, &show_rules}},
            {"start", {0, &start}},
            {"stop", {0, &stop}}
    };
public:
    Controller();
    void run();
};


#endif //FIREWALL_CONTROLLER_H
