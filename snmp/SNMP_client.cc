//
// Created by a1ef0 on 7/9/23.
//

#include <stdexcept>
#include <vector>

#include "SNMP_client.h"

SNMPClient::SNMPClient(const std::string& peer_name,
                       const std::string& community_name) {
    init_snmp("snmp_driver");

    snmp_sess_init(&session);
    session.peername = strdup(peer_name.c_str());

    session.version = SNMP_VERSION_1;

    session.community = (u_char *) community_name.c_str();
    session.community_len = strlen(
            reinterpret_cast<const char *>(session.community));
}


std::vector<std::string>
SNMPClient::send_request(const std::string& oid_string) {
    oid anOID[MAX_OID_LEN];
    size_t anOID_len;

    netsnmp_variable_list *vars;
    int status;
    int count = 1;

    ss = snmp_open(&session);

    if (!ss) {
        snmp_sess_perror("ack", &session);
        exit(1);
    }

    pdu = snmp_pdu_create(SNMP_MSG_GET);
    anOID_len = MAX_OID_LEN;

    if (!snmp_parse_oid(oid_string.c_str(), anOID, &anOID_len)) {
        snmp_perror(oid_string.c_str());
        throw std::invalid_argument{"Invalid OID string!"};
    }
    snmp_add_null_var(pdu, anOID, anOID_len);

    status = snmp_synch_response(ss, pdu, &response);
    std::vector<std::string> response_strings;
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {

        for(vars = response->variables; vars; vars = vars->next_variable)
            print_variable(vars->name, vars->name_length, vars);

        for(vars = response->variables; vars; vars = vars->next_variable) {
            if (vars->type == ASN_OCTET_STR) {
                char *sp = (char *)malloc(1 + vars->val_len);
                memcpy(sp, vars->val.string, vars->val_len);
                sp[vars->val_len] = '\0';
                response_strings.emplace_back(sp);
                printf("value #%d is a string: %s\n", count++, sp);
                free(sp);
            }
            else
                printf("value #%d is NOT a string! Ack!\n", count++);
        }
    } else {
        if (status == STAT_SUCCESS)
            fprintf(stderr, "Error in packet\nReason: %s\n",
                    snmp_errstring(response->errstat));
        else if (status == STAT_TIMEOUT)
            fprintf(stderr, "Timeout: No response from %s.\n",
                    session.peername);
        else
            snmp_sess_perror("snmp_driver", ss);
    }


    if (response)
        snmp_free_pdu(response);
    snmp_close(ss);

    return response_strings;
}