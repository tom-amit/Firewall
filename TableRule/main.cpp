#include "include/RuleTable.h"
int main ()
{
    RuleTable mainTable;
    string name = "amit", srcp = "192.168.1.1", srcpo = "1", destp = "255.33.11.22", destpo = "1234", protocol = "UDP", ack = "yes", action = "allow";
    mainTable.AddRule(name, srcp, destp, srcpo, destpo, protocol, ack, action);
    mainTable.DisplayTable();
}