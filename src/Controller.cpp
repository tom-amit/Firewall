//
// Created by tk on 30/03/2022.
//

#include "../include/Controller.h"
#include <cstdio>

Controller::Controller() {
    firewall = Firewall();
    cmd_map = {
            {"add-rule",    {9, &Controller::add_rule}},
            {"remove-rule", {1, &Controller::remove_rule}},
            {"edit-rule",   {10, &Controller::edit_rule}},
            {"show-rules",  {0, &Controller::show_rules}},
            {"start",       {0, &Controller::start}},
            {"stop",        {0, &Controller::stop}}
    };
}

bool Controller::add_rule(const std::vector<string> &args) {
    std::optional<string> ret = firewall.table->AddRule(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
    show_rules({});
    return ret.has_value();
}

bool Controller::remove_rule(const std::vector<string> &args) {
    firewall.table->RemoveRule(args[0]);
    show_rules({});
    return false;
}

bool Controller::edit_rule(const std::vector<std::string> &args) {
    uint16_t id = std::stoi(args[0]);
    std::optional<string> ret = firewall.table->EditRule(id, args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
    show_rules({});
    return ret.has_value();

}

bool Controller::show_rules(const std::vector<string> &args) {
    firewall.table->DisplayTable();
    return true;
}

bool Controller::start(const std::vector<string> &args) {
    firewall.Run();
    return true;
}

bool Controller::stop(const std::vector<string> &args) {
    firewall.Stop();
    return true;
}

bool Controller::run() {
    std::cout << firewall.table->compare_ip_addresses("192.168.0.0/15", "192.179.3.3") << std::endl;
    return true;
    firewall.table->AddRule("amit12345", "any", "any", "any", "any", "any", "any", "any", "allow");
    //firewall.table->AddRule("itay", "in", "234.222.11.3", "2.*.*.1", "192", "65535", "UDP", "yes", "allow");
    std::vector<std::string> args;
    do {
        args.clear();
        std::cout << "Enter command: " << std::endl;
        std::string rawInput;
        while (true) {
            std::cin >> rawInput;
            args.push_back(rawInput);
            if (std::cin.peek() == '\n') {
                break;
            }
        }
        if(args[0] == "quit"){
            break;
        }
        if ( auto it{ cmd_map.find( args[0] ) }; it != cmd_map.end())
        {
            if(args.size() - 1 == cmd_map.at(args[0]).first){
                (this->*(cmd_map.at(args[0]).second))(std::vector<string>(args.begin() + 1, args.end()));
            }
        }
    }while(true);
    return true;
}