//
// Created by karkt on 3/19/2022.
//

#include "../include/Rule.h"

#include <utility>
Rule::Rule(){
    //TODO strings and values here are only for initial invalid rules, make sure a regular user cannot set them in a way
    // that they will be accepted
    this->hit_count = 0;
    this->name = "No Name";
    this->direction = "none";
    this->src_ip = std::make_tuple(pcpp::IPv4Address("0.0.0.0"), 0, "none");
    this->dest_ip = std::make_tuple(pcpp::IPv4Address("0.0.0.0"), 0, "none");
    this->src_port = std::make_pair(MAX_PORT+1, MAX_PORT+1);
    this->dest_port = std::make_pair(MAX_PORT+1, MAX_PORT+1);
    this->ack="none";
    this->protocol = 0;
    this->action = "none";
    this->isInvalid = true;
}
Rule::Rule(const string &name, const string &direction, const string &src_ip,
           const string &dest_ip, const string &src_port, const string &dest_port, const string &protocol,
           const string &ack,
           const string &action) {
    this->hit_count = 0;
    this->name = name;
    this->direction = ParseDirection(direction);
    this->src_ip = ParseIP(src_ip);
    this->dest_ip = ParseIP(dest_ip);
    this->src_port = ParsePort(src_port);
    this->dest_port = ParsePort(dest_port);
    this->ack = ParseAck(ack);
    this->protocol = ParseProtocol(protocol);
    this->action = ParseAction(action);
    this->isInvalid = false;
}
void Rule::IncrementHitCount(){
    this->hit_count++;
}
uint64_t Rule::getHitCount() const{
    return hit_count;
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

std::tuple<pcpp::IPv4Address, uint16_t, string> Rule::getSrcIp() const {
    return src_ip;
}
void Rule::setSrcIp(const string &p_src_ip) {
    src_ip = ParseIP(p_src_ip);
}

std::tuple<pcpp::IPv4Address, uint16_t, string> Rule::getDestIp() const {
    return dest_ip;
}

void Rule::setDestIp(const string &p_dest_ip) {
    dest_ip = ParseIP(p_dest_ip);
}

std::tuple<uint32_t, uint32_t, string> Rule::getSrcPort() const {
    string repr;
    // if it's any, return the max port, if it's a range, return the range, else return the one of the ports
    if (src_port.first == MAX_PORT+1 && src_port.second == MAX_PORT+1) {
        repr = "any";
    } else if (src_port.first == src_port.second) {
        repr = to_string(src_port.first);
    } else {
        repr = to_string(src_port.first) + "-" + to_string(src_port.second);
    }
    return {src_port.first, src_port.second, repr};
}

void Rule::setSrcPort(string p_src_port) {
    src_port = ParsePort(std::move(p_src_port));
}

std::tuple<uint32_t, uint32_t, string> Rule::getDestPort() const {
    string repr;
    // if it's any, return the max port, if it's a range, return the range, else return the one of the ports
    if (dest_port.first == MAX_PORT+1 && dest_port.second == MAX_PORT+1) {
        repr = "any";
    } else if (dest_port.first == dest_port.second) {
        repr = to_string(dest_port.first);
    } else {
        repr = to_string(dest_port.first) + "-" + to_string(dest_port.second);
    }
    return {dest_port.first, dest_port.second, repr};
}

void Rule::setDestPort(string p_dest_port) {
    dest_port = ParsePort(std::move(p_dest_port));
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

std::pair<int16_t, string> Rule::getAck() const {
    return {ACK_DEF.at(ack), ack};
}

void Rule::setAck(const string& p_ack) {
    Rule::ack = ParseAck(p_ack);
}


bool Rule::isNotValid() const{
    return isInvalid;
}

void Rule::strToFunc(string &str, int (*func)(int)) {
    std::for_each(str.begin(), str.end(), [&func](char &c) {
        c = func(c);
    });
}

std::vector<string> Rule::split_ip(const string &s) {
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
    //std::cout << "PARSE DIR:" << dir << std::endl;
    throw BadParse("Rule Direction", dir);
}
std::string Rule::ParseAction(string p_action) {
    strToFunc(p_action, ::tolower);
    if (auto it{ACTION_DEF.find(p_action)}; it != ACTION_DEF.end()) {
        return p_action;
    }
    throw BadParse("Rule Action", p_action);
}

std::tuple<pcpp::IPv4Address, uint16_t, string> Rule::ParseIP(string p_ip_addr){
    uint16_t  final_cidr = IRRELEVANT_CIDR;
    size_t pos = p_ip_addr.find("/");
    if(pos != string::npos){
        string cidr = p_ip_addr.substr(pos+1);
        try{
            final_cidr = std::stoi(cidr);
            if (final_cidr > 32 || final_cidr < 0){
                throw std::invalid_argument("");
            }
        }catch(std::invalid_argument &e){
            throw BadParse("Rule CIDR in IP Rule", cidr);
        }
        p_ip_addr = p_ip_addr.substr(0,pos);
    }
    std::vector<string> ip_split = split_ip(p_ip_addr);

    pcpp::IPv4Address temp(p_ip_addr);
    if(temp.toString() == p_ip_addr){
        if(final_cidr != IRRELEVANT_CIDR){
            p_ip_addr += "/" + to_string(final_cidr);
        }
        return {temp, final_cidr, p_ip_addr};
    }
    else{
        //TODO currently CIDR is ignored in general IPs or partly general, but need to reject such IPs, CIDR and ASTERISK
        // cannot happen simultaneously
        strToFunc(p_ip_addr, ::tolower);
        if(GENERAL_IP.find(p_ip_addr) != GENERAL_IP.end()){
            return {NULL, IRRELEVANT_CIDR, GENERAL_IP.at(p_ip_addr)};
        }
        //TODO check if removing assignment will still work when adding characters below because Clang-Tidy complains.
        string replacement = "";
		//TODO remove support for asterrisks
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
            return {NULL, IRRELEVANT_CIDR, p_ip_addr};
        }
    }
    throw BadParse("Rule IP", p_ip_addr);
}
std::pair<uint32_t, uint32_t>  Rule::ParsePort(string p_port_num){
    //check if port is a range
    size_t pos = p_port_num.find('-');
    if(pos != string::npos){
        string port_start = p_port_num.substr(0, pos);
        string port_end = p_port_num.substr(pos+1);
        try{
            uint32_t port_start_num = std::stoi(port_start);
            uint32_t port_end_num = std::stoi(port_end);
            if(port_start_num > port_end_num
                || port_start_num < 0 || port_end_num > 65535){
                throw std::invalid_argument("");
            }
            return {port_start_num, port_end_num};
        }catch(std::invalid_argument &e){
            throw BadParse("Rule Port Range", p_port_num);
        }
    }
    strToFunc(p_port_num, ::tolower);
    try{
        if (p_port_num == ANY) return {0, MAX_PORT+1};
        uint32_t tmp = std::stoi(p_port_num);
        if(tmp >= 0 && tmp <= MAX_PORT) return {tmp, tmp};
        throw std::exception();
    }
    catch(...){
        throw BadParse("Rule Port", p_port_num);
    }

}
string Rule::ParseAck(string p_ack){
    strToFunc(p_ack, ::tolower);
    if (ACK_DEF.find(p_ack) != ACK_DEF.end()) {
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

void Rule::setNotValid(const bool p_notValid) {
    isInvalid = p_notValid;
}

