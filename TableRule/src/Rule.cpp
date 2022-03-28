//
// Created by karkt on 3/19/2022.
//

#include "../include/Rule.h"

Rule::Rule(const string &name, const string &direction, const string& src_ip, const string &src_port,
           const string& dest_ip, const string &dest_port, const string &protocol, const string &ack,
           const string &action)
{
    this->name = name;
    this->direction = ParseDirection(direction);
    this->src_ip = ParseIP(src_ip);
    this->src_port = ParsePort(src_port);
    this->dest_ip = ParseIP(dest_ip);
    this->dest_port = ParsePort(dest_port);
    this->ack = ParseAck(ack);
    this->protocol = ParseProtocol(protocol);;
    this->action = ParseAction(action);
}

string Rule::getName() const {
    return name;
}

void Rule::setName(const string &p_name) {
    name = p_name;
}

string Rule::getAction() const {
    return action;
}

void Rule::setAction(const string &p_action) {
    action = ParseAction(p_action);
}

string Rule::getDirection() const {
    return direction;
}

void Rule::setDirection(const string &p_direction) {
    direction = ParseDirection(p_direction);
}

std::pair<pcpp::IPv4Address, string> Rule::getSrcIp() const {
    return src_ip;
}
void Rule::setSrcIp(const string &p_src_ip) {
    src_ip = ParseIP(p_src_ip);
}

std::pair<pcpp::IPv4Address, string> Rule::getDestIp() const {
    return dest_ip;
}

void Rule::setDestIp(const string &p_dest_ip) {
    dest_ip = ParseIP(p_dest_ip);
}

std::pair<uint32_t, string> Rule::getSrcPort() const {
    return {src_port, src_port > MAX_PORT ? "any" : to_string(src_port)};
}

void Rule::setSrcPort(string p_src_port) {
    src_port = ParsePort(p_src_port);
}

std::pair<uint32_t, string> Rule::getDestPort() const {
    return {dest_port, dest_port > MAX_PORT ? "any" : to_string(dest_port)};
}

void Rule::setDestPort(string p_dest_port) {
    dest_port = ParsePort(p_dest_port);
}

string Rule::getProtocol() const {
    for (const auto & it : PROTOCOL_DEF)
        if (it.second == protocol)
            return it.first;
    throw BadParse("or could not find matching protocol", name);
}

void Rule::setProtocol(string p_protocol) {
    Rule::protocol = ParseProtocol(std::move(p_protocol));
}

std::pair<uint16_t, string> Rule::getAck() const {
    return {(ack == "yes") ? 1 : ((ack == "no") ? 0 : -1) , ack};
}

void Rule::setAck(const string& p_ack) {
    Rule::ack = ParseAck(p_ack);
}

void strToFunc(string& str, int (*func)(int)){
    std::for_each(str.begin(), str.end(), [&func](char & c){
        c = func(c);
    });
}
std::vector<string> Rule::split_ip(const string& s) {
    string delimiter = ".";
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    std::vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}
std::string Rule::ParseDirection(string dir){
    strToFunc(dir, ::tolower);
    if(std::find(DIR_DEF.begin(), DIR_DEF.end(), dir) != DIR_DEF.end()){
        return dir;
    }
    std::cout << "PARSE DIR:" << dir << std::endl;
    throw BadParse("Rule Direction", dir);
}
std::string Rule::ParseAction(string p_action){
    strToFunc(p_action, ::tolower);
    if(std::find(ACTION_DEF.begin(), ACTION_DEF.end(), p_action) != ACTION_DEF.end()){
        return p_action;
    }
    throw BadParse("Rule Action", p_action);
}

std::pair<pcpp::IPv4Address,string> Rule::ParseIP(string p_ip_addr){
    std::vector<string> ip_split = split_ip(p_ip_addr);
    pcpp::IPv4Address temp(p_ip_addr);
    if(temp.toString() == p_ip_addr){
        return {temp, temp.toString()};
    }
    else{
        strToFunc(p_ip_addr, ::tolower);
        if(GENERAL_IP.find(p_ip_addr) != GENERAL_IP.end()){
            return {NULL, GENERAL_IP.at(p_ip_addr)};
        }
        string replacement = "";
        for(auto& c: p_ip_addr){
            if(c == '*'){
                replacement+='0';
            }
            else{
                replacement+=c;
            }
        }
        temp = pcpp::IPv4Address(replacement);
        if(temp.toString() == replacement){
            return {NULL, p_ip_addr};
        }
    }
    throw BadParse("Rule IP", p_ip_addr);
}
uint32_t Rule::ParsePort(string p_port_num){
    strToFunc(p_port_num, ::tolower);
    try{
        if(p_port_num == "any") return MAX_PORT+1;
        uint32_t tmp = std::stoi(p_port_num);
        if(tmp >= 0 && tmp <= MAX_PORT) return tmp;
        throw std::exception();
    }
    catch(...){
        throw BadParse("Rule Port", p_port_num);
    }

}
string Rule::ParseAck(string p_ack){
    strToFunc(p_ack, ::tolower);
    if(std::find(ACK_DEF.begin(), ACK_DEF.end(), p_ack) != ACK_DEF.end()){
        return p_ack;
    }
    throw BadParse("Rule ACK", p_ack);

}
pcpp::ProtocolType Rule::ParseProtocol(string p_protocol){
    strToFunc(p_protocol, ::toupper);
    if ( auto it{ PROTOCOL_DEF.find( p_protocol ) }; it != PROTOCOL_DEF.end() )
    {
        return PROTOCOL_DEF.at(p_protocol);
    }
    throw BadParse("Rule Protocol", p_protocol);
}