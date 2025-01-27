/*
 * Dibbler - a portable DHCPv6
 *
 * authors: Tomasz Mrugalski <thomson@klub.com.pl>
 *          Marek Senderski <msend@o2.pl>
 *
 * released under GNU GPL v2 only licence
 *
 */

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "DHCPServer.h"
#include "Portable.h"
#include "Logger.h"
#include "daemon.h"

using namespace std;

TDHCPServer * ptr = 0;


char user_defined_workdir[256];
char user_defined_server_conf[256];
char user_defined_relay_conf[256];
char user_defined_resolv[256];
char user_defined_ntpconf[256];
char user_defined_radvf[256];
char user_defined_server_pid_file[256];
char user_defined_relay_pid_file[256];
char user_defined_aaaspi_file[256];
char user_defined_srv_keymap_file[256];
char user_defined_server_log_file[256];
char user_defined_relay_log_file[256];


/// the default working directory
std::string WORKDIR;

void signal_handler(int n) {
    Log(Crit) << "Signal received. Shutting down." << LogEnd;
    ptr->stop();
}

int status() {
    pid_t pid = getServerPID();
    if (pid==-1) {
	cout << "Dibbler server: NOT RUNNING." << endl;
    } else {
	cout << "Dibbler server: RUNNING, pid=" << pid << endl;
    }
    int result = (pid>0)? 0: -1;
    
    pid = getClientPID();
    if (pid==-1) {
	cout << "Dibbler client: NOT RUNNING." << endl;
    } else {
	cout << "Dibbler client: RUNNING, pid=" << pid << endl;
    }

    pid = getRelayPID();
    if (pid==-1) {
	cout << "Dibbler relay : NOT RUNNING." << endl;
    } else {
	cout << "Dibbler relay : RUNNING, pid=" << pid << endl;
    }

    return result;
}

int run() {
    if (!init(SRVPID_FILE, WORKDIR.c_str())) {
	die(SRVPID_FILE);
	return -1;
    }

    TDHCPServer srv(SRVCONF_FILE);
    
    ptr = &srv;
    if (ptr->isDone()) {
	die(SRVPID_FILE);
	return -1;
    }
    
    // connect signals
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    ptr->run();

    die(SRVPID_FILE);
    return 0;
}


int help() {
    cout << "Usage:" << endl;
    cout << " dibbler-server ACTION SERVER_CONF_FILE WORKDIR  " << endl
	 << " ACTION = status|start|stop|run" << endl
	 << " status    - show status and exit" << endl
	 << " start     - start installed service" << endl
	 << " stop      - stop installed service" << endl
	 << " install   - Not available in Linux/Unix systems." << endl
	 << " uninstall - Not available in Linux/Unix systems." << endl
	 << " run       - run in the console" << endl
	 << " help      - displays usage info." << endl;
    return 0;
}

int main(int argc, char * argv[])
{
    char command[256];
    int result=-1;

    logStart("(SERVER, Linux port)", "Server", SRVLOG_FILE);

    // parse command line parameters
    if (argc>1) {
	int len = strlen(argv[1])+1;
	if (len>255)
	    len = 255;
	strncpy(command,argv[1],len);
    } else {
	memset(command,0,256);
    }

    //PARSE SERVER CONFIGURATION FILE
    if (argc>2) {
	int len = strlen(argv[2])+1;
	if (len>255)
	    len = 255;
	strncpy(user_defined_server_conf,argv[2],len);
    } else {
	memset(user_defined_server_conf,0,256);
    }

    // parse workdir
    if (argc>3) {
	int len = strlen(argv[3])+1;
	if (len>255)
	    len = 255;
	strncpy(user_defined_workdir,argv[3],len);
    } else {
	memset(user_defined_workdir,0,256);
    }

    WORKDIR = user_defined_workdir;
    strncpy(user_defined_server_pid_file, (WORKDIR + "/server.pid").c_str(), strlen((WORKDIR + "/server.pid").c_str()));
    strncpy(user_defined_relay_pid_file, (WORKDIR + "/relay.pid").c_str(), strlen((WORKDIR + "/relay.pid").c_str()));
    strncpy(user_defined_aaaspi_file, (WORKDIR + "/AAA/AAA-SPI").c_str(), strlen((WORKDIR + "/AAA/AAA-SPI").c_str()));
    strncpy(user_defined_srv_keymap_file, (WORKDIR + "/AAA/keys-mapping").c_str(), strlen((WORKDIR + "/AAA/keys-mapping").c_str()));

    strncpy(user_defined_server_log_file, (WORKDIR + "/dibbler-server.log").c_str(), strlen((WORKDIR + "/dibbler-server.log").c_str()));
    strncpy(user_defined_relay_log_file, (WORKDIR + "/dibbler-relay.log").c_str(), strlen((WORKDIR + "/dibbler-relay.log").c_str()));



    if (!strncasecmp(command,"start",5) ) {
	result = start(SRVPID_FILE, WORKDIR.c_str());
    } else
    if (!strncasecmp(command,"run",3) ) {
	result = run();
    } else
    if (!strncasecmp(command,"stop",4)) {
	result = stop(SRVPID_FILE);
    } else
    if (!strncasecmp(command,"status",6)) {
	result = status();
    } else
    if (!strncasecmp(command,"help",4)) {
	result = help();
    } else
    if (!strncasecmp(command,"install",7)) {
	cout << "Function not available in Linux/Unix systems." << endl;
	result = 0;
    } else
    if (!strncasecmp(command,"uninstall",9)) {
	cout << "Function not available in Linux/Unix systems." << endl;
	result = 0;
    } else
    {
	help();
    }

    logEnd();

    return result? EXIT_FAILURE : EXIT_SUCCESS;
}
