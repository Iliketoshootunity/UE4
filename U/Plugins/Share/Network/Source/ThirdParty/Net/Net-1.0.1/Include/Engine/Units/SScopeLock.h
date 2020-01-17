#pragma once
#include "Engine/HAL/SPlatform.h"
#include "Engine/HAL/SPlatformMutex.h"

class SScopeLock
{
public:

    /**
    * Constructor that performs a lock on the synchronization object
    *
    * @param InSynchObject The synchronization object to manage
    */
	SScopeLock(SMutex* InSynchObject);
        

    /** Destructor that performs a release on the synchronization object. */
	~SScopeLock();
    

private:

    // Holds the synchronization object to aggregate and scope manage.
    SMutex * SynchObject;
};