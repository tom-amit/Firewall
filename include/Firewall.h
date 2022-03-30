//
// Created by ubu1 on 30/03/2022.
//

#ifndef FIREWALL_FIREWALL_H
#define FIREWALL_FIREWALL_H

#include <EthLayer.h>
#include <PcapLiveDeviceList.h>
#include <IPv4Layer.h>
#include "../TableRule/include/RuleTable.h"

class Firewall {
private:
    pcpp::PcapLiveDevice *dev1, *dev2; //dev1 is 'internal' and dev2 is 'external'

    void SetLiveDevices();
    void OpenLiveDevices();
    void StartLiveDevicesCapture();
    void StopLiveDevicesCapture();
public:
    Firewall();

    RuleTable* table;

    void Run();
    void Stop();
};


#endif //FIREWALL_FIREWALL_H
