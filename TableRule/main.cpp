#include "include/RuleTable.h"

int main ()
{
    RuleTable mainTable;
    string name = "amit", direction="in", srcp = "192.168.1.1", srcpo="132",destp="255.33.11.22", destpo ="1234",protocol ="UDP", ack = "yes", action="allow";
    mainTable.AddRule(name, direction, srcp, srcpo, destp, destpo, protocol, ack, action);
    mainTable.AddRule("itay", "in", "234.222.11.3", "123", "192.169.33.1", "6553", "UDP", "yes", "allow");
    mainTable.AddRule("itay", "in", "234.222.11.3", "123", "192.169.33.1", "65535", "UDP", "yes", "allow");
    mainTable.DisplayTable();

}