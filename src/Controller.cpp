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
            {"show-rules",  {0, &Controller::show_rules}},
            {"start",       {0, &Controller::start}},
            {"stop",        {0, &Controller::stop}}
    };
}

void Controller::add_rule(const std::vector<string> &args) {
    firewall.table->AddRule(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
}

void Controller::remove_rule(const std::vector<string> &args) {
    firewall.table->RemoveRule(args[0]);
}

void Controller::show_rules(const std::vector<string> &args) {
    firewall.table->DisplayTable();
}

void Controller::start(const std::vector<string> &args) {
    firewall.Run();
}

void Controller::stop(const std::vector<string> &args) {
    firewall.Stop();
}

void Controller::run() {
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
}