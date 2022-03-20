//
// Created by karkt on 3/19/2022.
//

#include "../include/Rule.h"
Rule::Rule(const string &name, const string &direction, const string src_ip, const string &src_port,
           const string dest_ip, const string &dest_port, const string &protocol, const string &ack,
           const string &action)
{
    //TODO: add parsing for stuff like IP and ports, we don't want to represent them using strings obviously.
    this->name = name;
    this->direction = ParseDirection(direction);
    this->src_ip = Rule::ParseIP(src_ip);
    this->src_port = Rule::ParsePort(src_port);
    this->dest_ip = Rule::ParseIP(dest_ip);
    this->dest_port = Rule::ParsePort(dest_port);
    this->ack = Rule::ParseAck(ack);
    this->protocol = Rule::ParseProtocol(protocol);;
    this->action = Rule::ParseAction(action);
}

string Rule::getName() const {
    return name;
}

void Rule::setName(const string &name) {
    Rule::name = name;
}

string Rule::getAction() const {
    return action;
}

void Rule::setAction(const string &action) {
    Rule::action = action;
}

string Rule::getDirection() const {
    return direction;
}

void Rule::setDirection(const string &direction) {
    Rule::direction = direction;
}

pcpp::IPv4Address Rule::getSrcIp() const {
    return src_ip;
}
void Rule::setSrcIp(const pcpp::IPv4Address &srcIp) {
    src_ip = srcIp;
}

pcpp::IPv4Address Rule::getDestIp() const {
    return dest_ip;
}

void Rule::setDestIp(const pcpp::IPv4Address &destIp) {
    dest_ip = destIp;
}

uint16_t Rule::getSrcPort() const {
    return src_port;
}

void Rule::setSrcPort(uint16_t srcPort) {
    src_port = srcPort;
}

uint16_t Rule::getDestPort() const {
    return dest_port;
}

void Rule::setDestPort(uint16_t destPort) {
    dest_port = destPort;
}

pcpp::ProtocolType Rule::getProtocol() const {
    return protocol;
}

void Rule::setProtocol(pcpp::ProtocolType protocol) {
    Rule::protocol = protocol;
}

string Rule::getAck() const {
    return ack;
}

void Rule::setAck(const string& ack) {
    Rule::ack = ack;
}

//TODO no critical, but a lot of these use the same pattern of string transform and find, maybe reduce to one implementation?

std::string Rule::ParseDirection(string dir){
    std::for_each(dir.begin(), dir.end(), [](char & c){
        c = ::tolower(c);
    });
    if(std::find(DIR_DEF.begin(), DIR_DEF.end(), dir) != DIR_DEF.end()){
        return dir;
    }
    return ""; //throw BadParse
}
std::string Rule::ParseAction(string action){
    std::for_each(action.begin(), action.end(), [](char & c){
        c = ::tolower(c);
    });
    if(std::find(ACTION_DEF.begin(), ACTION_DEF.end(), action) != ACTION_DEF.end()){
        return action;
    }
    return ""; //throw BadParse
}
pcpp::IPv4Address Rule::ParseIP(string ip_addr){
    pcpp::IPv4Address temp(ip_addr);
    if(temp.toString() == ip_addr){
        return temp;
    }
    return pcpp::IPv4Address("0.0.0.0"); //throw BadParse
}
uint16_t Rule::ParsePort(const string& port_num){
    try{
        uint16_t tmp = std::stoi(port_num);
        if(tmp >= 0 && tmp <= (2e16)-1){
            return tmp;
        }
        throw std::exception();
    }
    catch(...){
        // throw BadParse
    }
    return -1;

}
string Rule::ParseAck(string ack){
    std::for_each(ack.begin(), ack.end(), [](char & c){
        c = ::tolower(c);
    });
    if(std::find(ACTION_DEF.begin(), ACTION_DEF.end(), ack) != ACTION_DEF.end()){
        return ack;
    } //throw BadParse
    return "";
}
pcpp::ProtocolType Rule::ParseProtocol(string protocol){
    std::for_each(protocol.begin(), protocol.end(), [](char & c){
        c = ::toupper(c);
    });
    if ( auto it{ PROTOCOL_DEF.find( protocol ) }; it != PROTOCOL_DEF.end() )
    {
        return PROTOCOL_DEF[protocol];
    }
    //throw BadParse
    return 0;
}