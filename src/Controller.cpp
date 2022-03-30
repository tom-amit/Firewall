//
// Created by tk on 30/03/2022.
//

#include "../include/Controller.h"
#include <cstdio>

Controller::Controller() {
    table = RuleTable();
    cmd_map = {
            {"add-rule",    {9, &Controller::add_rule}},
            {"remove-rule", {1, &Controller::remove_rule}},
            {"show-rules",  {0, &Controller::show_rules}},
            {"start",       {0, &Controller::start}},
            {"stop",        {0, &Controller::stop}}
    };
}

void Controller::add_rule(const std::vector<string> &args) {
    table.AddRule(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
}

void Controller::remove_rule(const std::vector<string> &args) {
    table.RemoveRule(args[0]);
}

void Controller::show_rules(const std::vector<string> &args) {
    table.DisplayTable();
}

void Controller::start(const std::vector<string> &args) {

}

void Controller::stop(const std::vector<string> &args) {

}

void Controller::run() {
    table.AddRule("amit12345", "in", "234.222.*.3", "123", "192.169.33.1", "655399", "UDP", "yes", "allow");
    table.AddRule("itay", "in", "234.222.11.3", "123", "192.169.33.1", "65535", "UDP", "yes", "allow");
    std::vector<std::string> args;
    do {
        args.clear();
        std::cout << "Enter command: " << std::endl;
        std::string rawInput;
        while (true) {
            std::cin >> rawInput;
            args.push_back(rawInput);
            if (std::cin.peek() == '\n')
            {
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
        std::string selection = args[0], cmd;
    }while(true);
}