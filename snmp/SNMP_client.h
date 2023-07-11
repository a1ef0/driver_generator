//
// Created by a1ef0 on 7/9/23.
//

#ifndef DRIVER_GENERATOR_SNMP_CLIENT_H
#define DRIVER_GENERATOR_SNMP_CLIENT_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string>

class SNMPClient {
public:
    // TODO: add enum for msg_type
    SNMPClient(const std::string& peer_name, const std::string& community_name);

    std::vector<std::string>
    send_request(const std::string& oid = ".1.3.6.1.2.1.1.1.0");
private:
    netsnmp_session session;
    netsnmp_session *ss;
    netsnmp_pdu *pdu;
    netsnmp_pdu *response;
};


#endif //DRIVER_GENERATOR_SNMP_CLIENT_H
