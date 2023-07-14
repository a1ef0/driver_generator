//
// Created by a1ef0 on 7/9/23.
//

#ifndef DRIVER_GENERATOR_SNMP_CLIENT_H
#define DRIVER_GENERATOR_SNMP_CLIENT_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string>

/**
 * Initializes remote SNMP client handling for further request sending.
 */
class SNMPClient {
public:
    // TODO: add enum for msg_type
    SNMPClient(const std::string& peer_name, const std::string& community_name);

    /**
     * Send request to a remote SNMP client and retrieve an array of response
     * strings
     * @param oid for the SNMP request
     * @return array of response strings
     * @throws std::invalid_argument if the oid is invalid
     */
    std::vector<std::string>
    send_request(const std::string& oid);
private:
    netsnmp_session session;
    netsnmp_session *ss;
    netsnmp_pdu *pdu;
    netsnmp_pdu *response;
};


#endif //DRIVER_GENERATOR_SNMP_CLIENT_H
