/*
 * Dibbler - a portable DHCPv6                                               
 *                                                                           
 * authors: Tomasz Mrugalski <thomson@klub.com.pl>                           
 *          Marek Senderski <msend@o2.pl>                                    
 *                                                                           
 * released under GNU GPL v2 or later licence                                
 *                                                                           
 * $Id: SrvParsIfaceOpt.h,v 1.3 2004-06-28 22:37:59 thomson Exp $
 *
 * $Log: not supported by cvs2svn $
 */

#ifndef TSRCPARSIFACEOPT_H_
#define TSRCPARSIFACEOPT_H_
#include "SrvParsClassOpt.h"

class TSrvParsIfaceOpt : public TSrvParsClassOpt
{
public:
    TSrvParsIfaceOpt(void);
    ~TSrvParsIfaceOpt(void);

    bool uniAddress();
    void setUniAddress(bool isUni);

    void setAddress(SmartPtr<TIPv6Addr> addr);
    SmartPtr<TIPv6Addr> getAddress();

    //DNS - oriented routines    
    void addDNSSrv(SmartPtr<TIPv6Addr> addr);
    void firstDNSSrv();
    SmartPtr<TIPv6Addr> getDNSSrv();
    void setDNSSrv(TContainer<SmartPtr<TIPv6Addr> > *dnsSrv);
    void setDomain(string domain);
    string getDomain();

    //NTP - oriented routines
    void addNTPSrv(SmartPtr<TIPv6Addr> addr);
    void firstNTPSrv();
    SmartPtr<TIPv6Addr> getNTPSrv();
    void setNTPSrv(TContainer<SmartPtr<TIPv6Addr> > *NTPSrv);
    void setTimeZone(string timeZone);
    string getTimeZone();

    void setIfaceMaxLease(long maxLease);
    long getIfaceMaxLease();

private:
    bool UniAddress;
    long IfaceMaxLease;
    SmartPtr<TIPv6Addr> Address;
    TContainer<SmartPtr<TIPv6Addr> > DNSSrv;		
    string Domain;			
    TContainer<SmartPtr<TIPv6Addr> > NTPSrv;		
    string TimeZone;		
};

#endif
