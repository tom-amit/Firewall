//
// Created by tk on 30/03/2022.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "TableRule/include/RuleTable.h"
using std::string;
void add_rule(std::vector<string> args){
    std::cout << "Test!!!!!!!!!!!!! " << args.size() << std::endl;
}
void remove_rule(std::vector<string> args){

}
void show_rules(std::vector<string> args){

}
void start(std::vector<string> args){

}
void stop(std::vector<string> args){

}
int main(){
    std::map<string, std::pair<u_int16_t , void(*)(std::vector<string>)>> cmd_map = {
            {"add-rule", {9, &add_rule}},
            {"remove-rule", {1, &remove_rule}},
            {"show-rules", {0, &show_rules}},
            {"start", {0, &start}},
            {"stop", {0, &start}},
    };
    std::vector<std::string> args;
    do
    {
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
                cmd_map.at(args[0]).second(std::vector<string>(args.begin()+1, args.end()));
            }
        }
        std::string selection = args[0], cmd;
    }while(true);
}