//
// Created by karkt on 3/19/2022.
//

#ifndef RULESDAST_RULE_H
#define RULESDAST_RULE_H


#define MAX_PORT 65535
#define IRRELEVANT_CIDR 33

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <optional>
#include <map>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "BadParse.h"
#include "IPv4Layer.h"

using std::string;
using std::to_string;

class Rule {
public:
    Rule(); // For invalid rules

    static std::vector<string> split_ip(const string &s);

    static void strToFunc(string &str, int (*func)(int));

    const static inline string ANY{"any"};
    const static inline std::map<string, string> GENERAL_IP{{"any", "0.0.0.0"}};
    const static inline std::map<string, bool> ACTION_DEF{{"allow", true},
                                                          {"drop",  false},
                                                          {"none",  false}};
    const static inline std::map<string, int16_t> ACK_DEF{{"yes", 1},
                                                          {"no",  0},
                                                          {"any", -1},
                                                          {"none", -100}};
    const static inline std::map<string, long unsigned int> PROTOCOL_DEF{{"none", 0},
                                                                         {"ANY", 0x111},
                                                                         {"TCP", 0x04},
                                                                         {"UDP", 0x08}};

	/**
	 * @brief Construct a new Rule object
	 * @param name Name of the rule
	 * @param src_ip Source IP of the rule
	 * @param dest_ip Destination IP of the rule
	 * @param src_port Source port of the rule
	 * @param dest_port Destination port of the rule
	 * @param protocol Protocol of the rule
	 * @param ack Whether the rule is for ACK packets
	 * @param action Action of the rule, either allow or drop
	 */
    Rule(const string &name, const string &src_ip,
         const string &dest_ip, const string &src_port, const string &dest_port, const string &protocol,
         const string &ack,
         const string &action);

	/**
	 * @return return the name of the rule
	 */
    [[nodiscard]] string getName() const;

	/**
	 * @param p_name sets the name of the rule
	 */
    void setName(const string &p_name);

	/**
	 * @return returns the action of the rule
	 */
    [[nodiscard]] string getAction() const;

	/**
	 * @param p_action set the action of the rule
	 */
    void setAction(const string &p_action);
	/**
	 * @return returns the source IP of the rule
	 */
    [[nodiscard]] std::tuple<pcpp::IPv4Address, uint16_t, string> getSrcIp() const;
	/**
	 * @param p_src_ip set the source IP of the rule
	 */
    void setSrcIp(const string &p_src_ip);
	/**
	 * @return returns the destination IP of the rule
	 */
    [[nodiscard]] std::tuple<pcpp::IPv4Address, uint16_t ,string> getDestIp() const;
	/**
	 * @param p_dest_ip set the destination IP of the rule
	 */
    void setDestIp(const string &p_dest_ip);
	/**
	 * @return returns the destination port of the rule
	 */
    [[nodiscard]] std::tuple<uint32_t, uint32_t, string> getSrcPort() const;
	/**
	 * @param p_src_port set the source port  of the rule
	 */
    void setSrcPort(string p_src_port);
	/**
	 * @return returns the destination port of the rule
	 */
    [[nodiscard]] std::tuple<uint32_t, uint32_t, string> getDestPort() const;
	/**
	 * @param p_dest_port set the destination port of the rule
	 */
    void setDestPort(string p_dest_port);

	/**
	 * @return the protocol type of the rule
	 */
    [[nodiscard]] string getProtocol() const;

	/**
	 * @param p_protocol set the protocol type of the rule
	 */
    void setProtocol(string p_protocol);

	/**
	 * @return get the ack flag of the rule
	 */
    [[nodiscard]] std::pair<int16_t, string> getAck() const;

	/**
	 * @param p_ack set the ack flag of the rule
	 */
    void setAck(const string &p_ack);

	/**
	 * @return True if the rule is invalid, else false.
	 */
    [[nodiscard]] bool isNotValid() const;

	/**
	 * @param p_notValid sets whether the rule is invalid or not
	 */
    void setNotValid(bool p_notValid);

	/**
	 * @return the hit count of the rule
	 */
    [[nodiscard]] uint64_t getHitCount() const;

	/**
	 * @brief Increment the hit count of the rule
	 */
    void IncrementHitCount();
private:
    string name, action, ack;

    uint64_t hit_count;
    //IP, CIDR, STR-IP
    std::tuple<pcpp::IPv4Address, uint16_t, string> src_ip, dest_ip;
    std::pair<uint32_t, uint32_t> src_port, dest_port;
    pcpp::ProtocolType protocol;

    bool isInvalid;

	/**
	 * @brief Parse the action and make sure it is valid
	 * @param p_action the action to parse
	 * @return the action if it is valid, else throw BadParse
	 */
    static string ParseAction(string p_action);
	/**
	 * @brief Parse the ip address and  make sure it is valid
	 * @param p_ip_addr the ip to parse
	 * @return a tuple of three ip indicators (IP in pcap++ format, cidr, string of ip) if valid, else throw BadParse
	 */
    static std::tuple<pcpp::IPv4Address, uint16_t, string> ParseIP(string p_ip_addr);

	/**
	 * @brief Parse the port (or port range)  and make sure it is valid
	 * @param p_port_num the port (or port range) to parse
	 * @return a pair of numbers which indicate the range of the valid ports if valid, else throw BadParse
	 */
    static std::pair<uint32_t, uint32_t>  ParsePort(string p_port_num);

	/**
	 * @brief parse the ack bit and make sure it is valid
	 * @param p_ack the ack bit to parse
	 * @return the ack bit if it is valid, else throw BadParse
	 */
    static string ParseAck(string p_ack);

	/**
	 * @brief parse the protocol and make sure it is valid
	 * @param p_protocol the protocol to parse
	 * @return the protocol type if it is valid, else throw BadParse
	 */
    static pcpp::ProtocolType ParseProtocol(string p_protocol);
};


#endif //RULESDAST_RULE_H
