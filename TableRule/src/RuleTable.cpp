//
// Created by karkt on 3/19/2022.
//
#include "../include/RuleTable.h"

RuleTable::RuleTable(){
    //TODO change pointers to smart pointer, i.e unique_ptr, shared_ptr
    table =  std::vector<Rule*>{new Rule(string("default"), string("any"), string("any"),string("any"),
                                         string("any"),string("any"),string("any"),string("any"),
                                         string("deny"))};
    __len = 1;
    display_padding = 13;
}

std::optional<std::string> RuleTable::AddRule(const string &name, const string &direction, const string &src_ip, const string &src_port,
                   const string &dest_ip, const string &dest_port, const string &protocol, const string &ack,
                   const string &action) {
    return AddRule(Rule(name, direction, src_ip, src_port,dest_ip, dest_port, protocol, ack, action));;
}

std::optional<std::string> RuleTable::AddRule(const Rule& rule)  {
    table.insert(table.end()-1, new Rule(rule));
    __len++;
    return {};
}

void RuleTable::DisplayTable(){
    std::cout.width(display_padding); std::cout << std::left << "Name";
    std::cout.width(display_padding); std::cout << std::left << "Direction";
    std::cout.width(display_padding); std::cout << std::left << "Source IP";
    std::cout.width(display_padding); std::cout << std::left << "Source Port";
    std::cout.width(display_padding); std::cout << std::left << "Dest IP";
    std::cout.width(display_padding); std::cout << std::left << "Dest Port";
    std::cout.width(display_padding); std::cout << std::left << "Protocol";
    std::cout.width(display_padding); std::cout << std::left << "Source Port";
    std::cout.width(display_padding); std::cout << std::left << "Action";
    std::cout << std::endl;
    for(int i = 0;i<TABLE_LENGTH*display_padding;++i){
        std::cout << "=";
    }
    std::cout << std::endl;
    for(auto& r : table){
        std::cout << std::endl;
        std::cout.width(display_padding); std::cout << std::left << r->getName();
        std::cout.width(display_padding); std::cout << std::left << r->getDirection();
        std::cout.width(display_padding); std::cout << std::left << r->getSrcIp().toString();
        std::cout.width(display_padding); std::cout << std::left << r->getSrcPort();
        std::cout.width(display_padding); std::cout << std::left << r->getDestIp().toString();
        std::cout.width(display_padding); std::cout << std::left << r->getDestPort();
        std::cout.width(display_padding); std::cout << std::left << r->getProtocol();
        std::cout.width(display_padding); std::cout << std::left << r->getAck();
        std::cout.width(display_padding); std::cout << std::left << r->getProtocol();
    }
    std::cout << std::endl;
}