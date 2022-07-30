//
// Created by tk on 30/03/2022.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>
#include "../TableRule/include/RuleTable.h"
#include "Firewall.h"

#ifndef FIREWALL_CONTROLLER_H
#define FIREWALL_CONTROLLER_H


using std::string;

class Controller {
private:
    typedef bool (Controller::*MFP)(const std::vector<string> &args);

    Firewall* firewall = nullptr;
	bool firewallStopped, firewallInitialised;

    std::map<string, std::pair<u_int16_t, MFP>> cmd_map;
public:
	/**
	 * @brief Constructor for the controller.
	 */
    Controller();

	/**
	 * @brief add rule in the rule table
	 * @param args vector of strings containing the rule to be added
	 * @return True if succeeded, false otherwise
	 */
    bool add_rule(const std::vector<string> &args);
	/**
	 * @brief remove  rule in the rule table
	 * @param args should contain 1 entry which is the index of the rule to be removed
	 * @return True if succeeded, false otherwise
	 */
    bool remove_rule(const std::vector<string> &args);
	/**
	 * @brief edit rule
	 * @param args vector of strings containing the rule to be edited with the index of the rule at first
	 * @return True if succeeded, false otherwise
	 */
    bool edit_rule(const std::vector<string> &args);
	/**
	 * @brief shows the rules in the terminal
	 * @param args empty vector
	 * @return True if succeeded, false otherwise
	 */
    bool show_rules(const std::vector<string> &args);
	/**
	 * @brief start the firewall
	 * @param args empty vector
	 * @return True if succeeded, false otherwise
	 */
    bool start(const std::vector<string> &args);
	/**
	 * @brief stop the firewall
	 * @param args empty vector
	 * @return True if succeeded, false otherwise
	 */
    bool stop(const std::vector<string> &args);
	/**
	 * @brief changes NICS in the firewall
	 * @param args vector of 2 entries containing the names of the new NICs
	 * @return True if succeeded, false otherwise
	 */
	bool changeNICS(const std::vector<string> &args);
	/**
	 * @brief swap rules in the rule table
	 * @param args vector of 2 entries containing the 2 indices to swap
	 * @return True if succeeded, false otherwise
	 */
	bool swap_rule_to(const std::vector<string> &args);

	/**
	 * @brief runs the Controller through the terminal
	 */
    bool run();

	/**
	 * @brief resets the firewall
	 * @return True
	 */
	bool reset_firewall() const;

	/**
	 * @brief gets the hit count for each rule in the table
	 * @return vector of hit counts
	 */
    [[nodiscard]] std::vector<uint64_t> get_hit_counts() const;

	bool help(const std::vector<string> &args);
};


#endif //FIREWALL_CONTROLLER_H
