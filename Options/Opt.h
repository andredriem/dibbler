/*                                                                           
 * Dibbler - a portable DHCPv6                                               
 *                                                                           
 * authors: Tomasz Mrugalski <thomson@klub.com.pl>                           
 *          Marek Senderski <msend@o2.pl>                                    
 *                                                                           
 * released under GNU GPL v2 only licence                                
 *                                                                           
 * $Id: Opt.h,v 1.12 2009-03-24 23:17:18 thomson Exp $
 *                                                                           
 */

#ifndef OPT_H
#define OPT_H

#include <list>
#include "SmartPtr.h"
#include "Container.h"
#include "DUID.h"

class TMsg;

class TOpt
{
  public:

    typedef enum {
	Layout_OptAddr,
	Layout_OptAddrLst,
	Layout_OptString,
	Layout_OptStringLst,
	Layout_OptDuid,
	Layout_OptGeneric
    } EOptionLayout;

    TOpt(int optType, TMsg* parent);
    virtual ~TOpt();

    /**
     * Return the size of the option, including :
     *  - Option number,
     *  - Option size
     *  - data
     * 
     * @return the size, I've already said that !
     */
    virtual int getSize() = 0;
    
    /**
     * This method transform the instance of the option class into bytecode
     * ready to be sent to the client or server, conform to the RFC
     * 
     * @param buf The address where to store the option
     * @return The address where the option ends
     */
    virtual char * storeSelf(char* buf) = 0;
    virtual bool doDuties() = 0;
    
    /**
     * Validate the option
     * 
     * @return true if the option is valid.
     */
    virtual bool isValid();

    virtual std::string getPlain();
    
    int getOptType();
    int getSubOptSize();
    
    char* storeSubOpt(char* buf);
    SPtr<TOpt> getOption(int optType);

    // suboptions management
    void firstOption();
    SPtr<TOpt> getOption();
    void addOption(SPtr<TOpt> opt);
    void delOption();
    int countOption();
    void delAllOptions();
    void setParent(TMsg* Parent);

    SPtr<TDUID> getDUID();
    void setDUID(SPtr<TDUID> duid);
    EOptionLayout getLayout() { return Layout; }

 protected:
    TContainer< SPtr<TOpt> > SubOptions;
    EOptionLayout Layout;
    int OptType;
    TMsg* Parent;
    SPtr<TDUID> DUID;
    bool Valid;
};

typedef std::list< SPtr<TOpt> > TOptList;

#endif
