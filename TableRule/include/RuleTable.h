//
// Created by karkt on 3/19/2022.
//
#ifndef RULESDAST_RULETABLE_H
#define RULESDAST_RULETABLE_H

#define TABLE_LENGTH 9
#define DEFAULT_COLOR_COUT "39"
#define INVALID_COLOR_COUT "31"

#include "Rule.h"
#include "Packet.h"
#include "TcpLayer.h"
#include "UdpLayer.h"
#include <memory>
#include <cmath>
#include <bitset>
using std::unique_ptr;
using std::get;
class RuleTable {
public:
	const string SUCCESS{"OK"};
	std::vector<unique_ptr<Rule>> table;

	/**
	 * @brief Construct a new Rule Table object
	 */
    RuleTable();

	/**
	 * @brief add a new rule to the table
	 * @param name the name of the rule
	 * @param direction the direction of the rule
	 * @param src_ip the source ip of the rule
	 * @param dest_ip the destination ip of the rule
	 * @param src_port the source port of the rule
	 * @param dest_port the destination port of the rule
	 * @param protocol the protocol of the rule
	 * @param ack the ack of the rule
	 * @param action the action of the rule
	 * @return an optional string
	 */
	std::optional<std::string> AddRule(const string &name, const string &direction, const string &src_ip,
                                       const string &dest_ip, const string &src_port, const string &dest_port,
                                       const string &protocol, const string &ack,
                                       const string &action);

	/**
	 * @brief prints the table
	 */
    void DisplayTable();

	/**
	 * @brief compares a packet to each rule in the table until if finds a match
	 * @param p_packet the packet to compare
	 * @param dir the direction of the packet
	 * @return True if the packet should be allowed, false otherwise.
	 */
    bool ParsePacket(pcpp::Packet &p_packet, const string &dir = "any");
	/**
	 * @brief adds an empty rule to the table (automatically invalid)
	 * @return optional string
	 */
    std::optional<string> AddEmptyRule();
	/**
	 * @brief swaps between 2 rules
	 * @param id1 index of the first rule
	 * @param id2 index of the second rule
	 * @return optional string
	 */
	std::optional<string> SwapRuleTo(uint64_t id1, uint64_t id2);
	/**
	 * @brief deletes a rule from the table
	 * @param index the index of the rule to delete
	 * @return optional string
	 */
    std::optional<std::string> RemoveRule(const string &index);
	/**
	 * @brief edit a rule in the table
	 * @param id the index of the rule to edit
	 * @param name the name of the rule
	 * @param direction the direction of the rule
	 * @param src_ip the source ip of the rule
	 * @param dest_ip the destination ip of the rule
	 * @param src_port the source port of the rule
	 * @param dest_port the destination port of the rule
	 * @param protocol the protocol of the rule
	 * @param ack the ack of the rule
	 * @param action the action of the rule
	 * @return optional string
	 */
    std::optional<std::string> EditRule(uint16_t id, const string &name, const string &direction, const string &src_ip,
                                        const string &dest_ip, const string &src_port, const string &dest_port,
                                        const string &protocol, const string &ack,
                                        const string &action);

	/**
	 * @brief compares ip addresses (more precisely rule and target) and returns whether they are matching or not (supports CIDR)
	 * @param rule ip of rule to compare to
	 * @param target ip of target that is compared
	 * @return
	 */
    static bool compare_ip_addresses(const string &rule, const string &target);

	/**
	 * @brief resets the table
	 */
    void clear_table();

	/**
	 * @brief retrieves the hit count for each rule
	 * @return vector of hit counts
	 */
    std::vector<uint64_t> get_hit_counts();
private:
	/**
	 * @brief private function to add rule
	 * @param rule the rule to add
	 * @return optional string
	 */
    std::optional<std::string> p_AddRule(const Rule &rule);


    uint64_t len;
    std::vector<string> rule_names;
    uint16_t display_padding;

};


#endif //RULESDAST_RULETABLE_H
