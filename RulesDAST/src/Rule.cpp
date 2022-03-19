//
// Created by karkt on 3/19/2022.
//

#include "../include/Rule.h"
Rule::Rule(const string &name, const string &direction, const string &src_ip, const string &src_port,
           const string &dest_ip, const string &dest_port, const string &protocol, const string &ack,
           const string &action)
{
    this->name = name;
    this->direction = direction;
    this->src_ip = src_ip;
    this->src_port = src_port;
    this->dest_ip = dest_ip;
    this->dest_port = dest_port;
    this->ack = ack;
    this->protocol = protocol;
    this->action = action;
}