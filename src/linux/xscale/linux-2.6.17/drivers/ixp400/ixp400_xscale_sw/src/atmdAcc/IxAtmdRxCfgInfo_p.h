/**
 * @file IxAtmdRxCfgInfo_p.h
 *
 * @author Intel Corporation
 * @date 17 March 2002
 *
 * @brief IxAtmdAcc Rx Configuration
 *
 * 
 * @par
 * IXP400 SW Release version  2.1
 * 
 * -- Intel Copyright Notice --
 * 
 * @par
 * Copyright (c) 2002-2005 Intel Corporation All Rights Reserved.
 * 
 * @par
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel Corporation
 * or its suppliers or licensors.  Title to the Material remains with
 * Intel Corporation or its suppliers and licensors.
 * 
 * @par
 * The Material is protected by worldwide copyright and trade secret laws
 * and treaty provisions. No part of the Material may be used, copied,
 * reproduced, modified, published, uploaded, posted, transmitted,
 * distributed, or disclosed in any way except in accordance with the
 * applicable license agreement .
 * 
 * @par
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you by
 * disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel, except in accordance with the
 * applicable license agreement.
 * 
 * @par
 * Unless otherwise agreed by Intel in writing, you may not remove or
 * alter this notice or any other notice embedded in Materials by Intel
 * or Intel's suppliers or licensors in any way.
 * 
 * @par
 * For further details, please see the file README.TXT distributed with
 * this software.
 * 
 * @par
 * -- End Intel Copyright Notice --
 */

#ifndef IX_ATMDRXCFGINFO_P_H
#define IX_ATMDRXCFGINFO_P_H

#include "IxQMgr.h"

/**
* @brief Rx data initialisation
*/
IX_STATUS ixAtmdAccRxCfgInfoInit (void);


/**
* @brief Rx data uninitialisation
*/
IX_STATUS ixAtmdAccRxCfgInfoUninit (void);


/**
* @brief Rx config display
*/
void ixAtmdAccRxCfgInfoChannelShow (IxAtmLogicalPort port);

/**
* @brief Rx stats display
*/
void ixAtmdAccRxCfgInfoStatsShow (void);

/**
* @brief Rx stats reset
*/
void ixAtmdAccRxCfgInfoStatsReset (void);

/**
* @brief Check is a vc is already configured
*/
BOOL ixAtmdAccRxCfgRxVcExists (IxAtmLogicalPort port,
                               unsigned int vpi,
                               unsigned int vci);

/**
* @brief  Get an available rx free queue
*/
IX_STATUS ixAtmdAccRxCfgFreeQueueGet (IxAtmdAccAalType aalServiceType,
                                      unsigned int minimumQueueSize,
                                      unsigned int npeVcId);

/**
* @brief  Get an available rx channel
*/
IX_STATUS ixAtmdAccRxCfgChannelGet (IxAtmLogicalPort port,
                                    unsigned int vpi,
                                    unsigned int vci,
                                    IxAtmdAccAalType aalServiceType,
                                    unsigned int* npeVcIdPtr,
                                    IxAtmConnId* connIdPtr);

/**
* @brief  Initialise a rx channel
*/
IX_STATUS
ixAtmdAccRxCfgChannelSet (IxAtmConnId connId,
                          unsigned int npeVcId,
                          IxAtmLogicalPort port,
                          unsigned int vpi,
                          unsigned int vci,
                          IxAtmdAccAalType aalServiceType,
                          IxAtmRxQueueId rxQueueId,
                          IxAtmdAccUserId userId,
                          IxAtmdAccRxVcRxCallback rxCallback);

/**
* @brief  Unconfigure a rx channel
*/
IX_STATUS ixAtmdAccRxCfgChannelReset (unsigned int npeVcId);

/**
* @brief Get the npeVcId from the connectionId if it is a valid one
*/
IX_STATUS ixAtmdAccRxCfgNpeVcIdGet (IxAtmConnId connId,
                                    unsigned int* npeVcId);

/**
* @brief Check all resources are available for this channel
*/
IX_STATUS ixAtmdAccRxCfgFreeResourcesCheck (unsigned int npeVcId);

/**
* @brief Invalidate the connId to prevent further access to the channel
*/
IX_STATUS ixAtmdAccRxCfgConnIdInvalidate (unsigned int npeVcId);

/**
* @brief check that receive traffic is enabled
*/
BOOL ixAtmdAccRxCfgVcIsEnabled (unsigned int npeVcId);

/**
* @brief check that receive traffic is disabled
*/
BOOL ixAtmdAccRxCfgVcIsDisabled (unsigned int npeVcId);

/**
* @brief Check if the channel is disconnecting
*/
BOOL ixAtmdAccRxCfgVcIsDisconnecting (unsigned int npeVcId);

/**
* @brief Start receive traffic
*/
void ixAtmdAccRxCfgVcEnable (unsigned int npeVcId);

/**
* @brief Rollback Rx Vc Enable function
* @sa ixAtmdAccRxCfgVcEnable
*/
void ixAtmdAccRxCfgVcEnableRollback (unsigned int npeVcId);

/**
* @brief Stop receive traffic
*/
void ixAtmdAccRxCfgVcDisable (unsigned int npeVcId);

/**
* @brief Set the threshold and enable callback for rxfree queue
*/
IX_STATUS
ixAtmdAccRxCfgRxFreeCallbackSet (unsigned int npeVcId,
                                 unsigned int thresholdLevel,
                                 IxAtmdAccRxVcFreeLowCallback callback);

/**
* @brief disable the rxfree threshold events
*/
IX_STATUS ixAtmdAccRxCfgRxFreeCallbackDisable (unsigned int npeVcId);

/**
* @brief Set the threshold and enable callback for rx queue
*/
IX_STATUS
ixAtmdAccRxCfgRxCallbackSet (IxAtmRxQueueId rxAtmdQueueId,
                             IxQMgrQId rxQmgrQueueId,
                             IxAtmdAccRxDispatcher callback);


/**
* @brief Disable the callback for the rx queue
*/
IX_STATUS
ixAtmdAccRxCfgRxCallbackReset (IxAtmRxQueueId atmRxQueueId,
                               IxQMgrQId qMgrQId);


/**
* @brief Check if any channel is already set in the system
*/
BOOL ixAtmdAccRxCfgRxVcsExist (void);

/**
* @brief Release resources when a channel is down (during a disconnect)
*/
IX_STATUS ixAtmdAccRxCfgResourcesRelease (unsigned int npeVcId);

/**
* @brief send a message to NPE to update the vc lookup table and
*        enable receive
*/
IX_STATUS ixAtmdAccRxCfgNpeVcLookupTableUpdate(unsigned int npeVcId);

#endif /* IX_ATMDRXCFGINFO_P_H */


