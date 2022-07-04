//
// Created by tk on 30/03/2022.
//
#include <iostream>
#include "include/Controller.h"
#include "include/NICS.h"

using std::string;

int main() {
    NICS::GetData("ens33", "ens36");
    Controller c = Controller();
    c.run();
}