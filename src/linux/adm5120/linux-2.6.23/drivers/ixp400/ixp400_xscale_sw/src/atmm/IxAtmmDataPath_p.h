/**
 * @file    IxAtmmDataPath_p.h
 *
 * @author Intel Corporation
 * @date    01-MAR-2002
 *
 * @brief   This file contains the internal functions and data types for AtmmDataPath
 *
 * 
 * @par
 * IXP400 SW Release Crypto version 2.3
 * 
 * -- Copyright Notice --
 * 
 * @par
 * Copyright (c) 2001-2005, Intel Corporation.
 * All rights reserved.
 * 
 * @par
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * 
 * @par
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * 
 * @par
 * -- End of Copyright Notice --
 */

#ifndef IXATMMDATAPATH_P_H
#define IXATMMDATAPATH_P_H
/*
 * User defined header files
 */
#include "IxOsalTypes.h"
#include "IxAtmm.h"

/*
 * #defines and macros used in this file.
 */

/*
 * Typedefs
 */

/**
 * @typedef Prototype of the function to query a ports details
 * 
 * @brief Callback prototype for querying a ports details
 *
 * @param IxLogicalPort port (in)  port to query
 * @param unsigned vpi (in) vpi to query on the port
 * @param unsigned vci (in) vci to query on the port
 * @param IxAtmmVcDirection direction (in) direction of the VC
 * @param IxAtmSchedulerVcId *vcId (out) vcId of the port found
 * @param IxAtmmVc *vcDesc (out) vcDesc of the port found
 */
typedef IX_STATUS (*IxAtmmVcQueryCallback) (IxAtmLogicalPort port, 
					    unsigned vpi, 
					    unsigned vci, 
					    IxAtmmVcDirection direction, 
					    IxAtmSchedulerVcId *vcId, 
					    IxAtmmVc *vcDesc);

/* 
 * Function prototypes 
 */

/**
 * @fn IX_STATUS ixAtmmDataPathSetup (IxAtmLogicalPort port)
 *
 * @brief This function configures receive and transmit control for
 *        a port.
 *
 * @param IxAtmLogicalPort port (in) - The port to setup
 *
 * @return IX_STATUS
 *
 */
IX_STATUS
ixAtmmDataPathSetup (IxAtmLogicalPort port);


/**
 * @fn IX_STATUS ixAtmmDataPathUninit (IxAtmLogicalPort port)
 *
 * @brief This function uninitialises the port specified.
 *
 * @param IxAtmLogicalPort port (in) - The port to uninitialise
 *
 * @return IX_STATUS
 *
 */
IX_STATUS
ixAtmmDataPathUninit (IxAtmLogicalPort port);


/**
 * @fn void ixAtmmVcQueryCallbackRegister ( IxAtmmVcQueryCallback callback)
 *
 * @brief This function registers a callback function that is used to query a VC
 *
 * @param IxAtmmVcQueryCallback callback (in) callback function to register
 *
 * @return void
 *
 */
void
ixAtmmVcQueryCallbackRegister ( IxAtmmVcQueryCallback callback);



/**
 * @fn void ixAtmmVcQueryCallbackUnregister ( void)
 *
 * @brief This function unregister the callback function
 *
 * @return void
 *
 */
void
ixAtmmVcQueryCallbackUnregister(void);



#endif /* IXATMMDATAPATH_P_H */
