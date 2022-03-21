#include "include/RuleTable.h"

int main ()
{
    RuleTable mainTable;
    string name = "amit", direction="in", srcp = "192.168.1.1", srcpo="132",destp="349.33.11.22", destpo ="1234",protocol ="UDP", ack = "yes", action="allow";
    mainTable.AddRule(name, direction, srcp, srcpo, destp, destpo, protocol, ack, action);
    mainTable.DisplayTable();

}