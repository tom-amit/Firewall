//
// Created by karkt on 3/19/2022.
//

#include "Rule.h"

#ifndef RULESDAST_RULETABLE_H
#define RULESDAST_RULETABLE_H
#define TABLE_LENGTH 9

class RuleTable {
    std::optional<std::string> AddRule(const Rule& rule);

public:
    RuleTable();

    std::optional<std::string> AddRule(const string &name, const string &direction, const string &src_ip, const string &src_port,
                                       const string &dest_ip, const string &dest_port, const string &protocol, const string &ack,
                                       const string &action);
    void DisplayTable();

private:
    std::vector<Rule*> table;
    uint64_t __len;
    uint16_t display_padding;
};


#endif //RULESDAST_RULETABLE_H
