/*
 * Dibbler - a portable DHCPv6
 *
 * authors: Tomasz Mrugalski <thomson@klub.com.pl>
 *          Marek Senderski <msend@o2.pl>
 * changes: Michal Kowalczuk <michal@kowalczuk.eu>
 *
 * released under GNU GPL v2 only licence
 */

class TMsg;
#ifndef MSG_H
#define MSG_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "SmartPtr.h"
#include "Container.h"
#include "DHCPConst.h"
#include "IPv6Addr.h"
#include "Opt.h"
#include "Key.h"
//#include "KeyList.h"
#include "ScriptParams.h"

// Hey! It's grampa of all messages
class TMsg
{
  public:
    // Used to create TMsg object (normal way)
    TMsg(int iface, SPtr<TIPv6Addr> addr, int msgType);
    TMsg(int iface, SPtr<TIPv6Addr> addr, int msgType, long transID);

    // used to create TMsg object based on received char[] data
    TMsg(int iface, SPtr<TIPv6Addr> addr, char* &buf, int &bufSize);

    virtual int getSize();

    // transmit (or retransmit)

    virtual unsigned long getTimeout();

    virtual int storeSelf(char * buffer);

    virtual std::string getName() const = 0;

    // returns requested option (or NULL, there is no such option)
    SPtr<TOpt> getOption(int type);
    void firstOption();
    int countOption();
    void addOption(SPtr<TOpt> opt) { Options.push_back(opt); }

    virtual SPtr<TOpt> getOption();

    long getType();
    long getTransID();
    TOptList & getOptLst();
    SPtr<TIPv6Addr> getAddr(); /// @todo: rename to getPeerAddr()
    int getIface();
    virtual ~TMsg();
    bool isDone();
    bool isDone(bool done);

    // useful auth stuff below
    void calculateDigests(char* buffer,  size_t len);
    void setAuthDigestPtr(char* ptr, unsigned len); /// @todo: remove from here (and move to AUTH option)
    bool loadAuthKey();
    void setAuthKey(const TKey& key);
    TKey getAuthKey();

    bool validateAuthInfo(char *buf, int bufSize);
    bool validateAuthInfo(char *buf, int bufSize, const std::vector<DigestTypes>& authLst);
    uint32_t getSPI();
    void setSPI(uint32_t val);
    DigestTypes DigestType_;

    // unknown (junk?) stuff below
#if AUTH_CRAP
    uint32_t getAAASPI();
    void setAAASPI(uint32_t val);
    void setKeyGenNonce(char *value, unsigned len);
    char* getKeyGenNonce();
    unsigned getKeyGenNonceLen();
    SPtr<KeyList> AuthKeys;
#endif

    // notify scripts stuff
    void* getNotifyScriptParams();

  protected:
    int MsgType;
    long TransID;

    bool delOption(int code);

    TOptList Options;
    TOptList::iterator NextOpt; // to be removed together with firstOption() and getOption();
    void setAttribs(int iface, SPtr<TIPv6Addr> addr,
                    int msgType, long transID);
    virtual bool check(bool clntIDmandatory, bool srvIDmandatory);

    bool IsDone; // Is this transaction done?
    int Iface;   // logical interface (for direct messages it equals PhysicalIface
                 // for relayed messages Iface points to relayX, PhysicalInterface to ethX)
    SPtr<TIPv6Addr> PeerAddr; // server/client address from/to which message was received/should be sent

    // useful auth stuff below
    uint32_t SPI_; // Key identifier

    char* AuthDigestPtr_;    // Digest (pointer to Authentication Information field of OPTION AUTH and OPTION AAAAUTH
    unsigned AuthDigestLen_; // Length of the digest

    TKey AuthKey_; // Auth Key

    // char* AuthKeyPtr_;      // Key pointer to key used do calculate Authentication information
    // unsigned AuthKeyLen_;    // length of the key

#if AUTH_CRAP
    // unknown (junk?) auth stuff below
    uint32_t AAASPI; // AAA-SPI sent by client in OPTION_AAAAUTH
    char *KeyGenNonce;
    unsigned KeyGenNonceLen;
#endif

    // a pointer to NotifyScriptParams structure (if defined)
    TNotifyScriptParams* NotifyScripts;
};

typedef std::list< SPtr<TMsg> > TMsgLst;

#endif
