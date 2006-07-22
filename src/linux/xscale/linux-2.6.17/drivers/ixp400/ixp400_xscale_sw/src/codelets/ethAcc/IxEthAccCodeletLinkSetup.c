/**
 * @file IxEthAccCodeletLinkSetup.c
 *
 * @date 22 April 2004
 *
 * @brief This file contains the implementation of the Ethernet Access 
 * Codelet that implements the link layer configuration and control
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

/*
 * Put the system defined include files required.
 */
#include "IxOsal.h"

/*
 * Put the component defined include files required.
 */
#include "IxFeatureCtrl.h"
#include "IxEthAcc.h"
#include "IxEthMii.h"
#include "IxEthAccCodelet.h"
#include "IxEthAccCodelet_p.h"

/* The Eth PHY Configuration (100/10 Mbps , Full/Half Duplex, 
 * with/without autonegotiation).
 */
typedef struct
{
    BOOL speed100;	/**< 100 Mbits */
    BOOL fullDuplex;	/**< Full Duplex */
    BOOL autonegotiate;	/**< Autonegotiation */
} IxEthAccCodeletPhyConf;

/* Default PHY Configuration parameters */
PRIVATE IxEthAccCodeletPhyConf ixEthAccCodeletPhyConf = 
{
    TRUE,	/* 100 Mbits */
    TRUE,	/* Full duplex */
    TRUE	/* Autonegotiate */
};

/* Mapping of the Logical Port ID to the MII Port ID */
PRIVATE UINT32 ixEthAccCodeletPhyAddresses[IX_ETHACC_CODELET_MAX_PORT];

/*
 * Function definition: ixEthAccCodeletLinkUpCheck()
 *
 * Check the phys are ready
 */

IX_STATUS ixEthAccCodeletLinkUpCheck(IxEthAccPortId portId)
{
   IX_STATUS returnStatus = IX_SUCCESS;
   BOOL fullDuplex;
   BOOL linkUp;
   BOOL speed;
   BOOL autoneg;

   /* get the status */
   ixEthMiiLinkStatus(ixEthAccCodeletPhyAddresses[portId], 
		      &linkUp, 
		      &speed, 
		      &fullDuplex, 
		      &autoneg);

   if (!linkUp)
   {
          unsigned int retry = 20; /* 20 retries */
          printf("Wait for PHY %u to be ready ...\n", portId);
          while ((!linkUp) && (retry-- > 0))
          {
            ixOsalSleep(100);  /* 100 milliseconds */

	    /* get the status again */
            ixEthMiiLinkStatus(ixEthAccCodeletPhyAddresses[portId], 
			       &linkUp, 
			       &speed, 
			       &fullDuplex, 
			       &autoneg);
          }
          if (!linkUp) 
          {
            returnStatus = IX_FAIL;
          }
   }

   /* return fail if one of the links is not up */
   return returnStatus;
}

/**
 * Function definition: ixEthAccCodeletLinkMonitor()
 *
 * Function checks the link status and sets the duplex mode if there
 * is a change in it
 */

PRIVATE void
ixEthAccCodeletLinkMonitor(IxEthAccPortId portId)
{
    BOOL speed, linkUp, fullDuplex, autoneg;
    /* get the link status */
    ixEthMiiLinkStatus(ixEthAccCodeletPhyAddresses[portId], 
		       &linkUp,
		       &speed,
		       &fullDuplex,
		       &autoneg);
    
    /* Set the MAC duplex mode */
    if(fullDuplex)
    {
	ixEthAccPortDuplexModeSet (portId, IX_ETH_ACC_FULL_DUPLEX);
    }
    else
    {
	ixEthAccPortDuplexModeSet (portId, IX_ETH_ACC_HALF_DUPLEX);
    }
}

/**
 * Function definition: ixEthAccCodeletPhyInit()
 *
 * This function scans for and then initialises any available PHYs on
 * the board. It uses the EthAcc MII library routines to do so.
 * 
 */

IX_STATUS ixEthAccCodeletPhyInit(void)
{
   BOOL phyPresent[IXP400_ETH_ACC_MII_MAX_ADDR];
   UINT32 phyNo;
   UINT32 phyNoAddr;
   UINT32 ixEthAccCodeletMaxPhyNo;
   unsigned int portId;

   /* Scan for available Ethernet PHYs on the board */
   if(ixEthMiiPhyScan(phyPresent,IX_ETHACC_CODELET_MAX_PORT) == IX_FAIL)
   {	
       return (IX_FAIL);
   }
   else
   {
       /* 
	* Mapping from portId to Phy Address is done here. 
	* This is board specific and depends on PHY address
	* setup (generally done by EEPROM or Jumpers)
	*
	* This codelet assumes the PHY with the lowest
	* address is connected to the NPE B and the PHY with the
	* highest address is connected to NPE C.
	*/
       ixEthAccCodeletMaxPhyNo = 0;
       for(phyNoAddr=0, phyNo=0;
	   phyNoAddr<IXP400_ETH_ACC_MII_MAX_ADDR; 
	   phyNoAddr++)
       {
	   if(phyPresent[phyNoAddr])
	   {
	       ixEthAccCodeletPhyAddresses[phyNo++] = phyNoAddr;
               ixEthAccCodeletMaxPhyNo = phyNo;

	       if(ixEthAccCodeletMaxPhyNo == IX_ETHACC_CODELET_MAX_PORT)
	       {
		   break;
	       }
	   }
       }
   }
    
   for(phyNo=0; 
       phyNo<ixEthAccCodeletMaxPhyNo; 
       phyNo++)
   {
       portId = phyNo;
       /* Reset each phy */

       if (ixFeatureCtrlDeviceRead() == IX_FEATURE_CTRL_DEVICE_TYPE_IXP42X)
       {
           if ((ixFeatureCtrlProductIdRead() & IX_FEATURE_CTRL_SILICON_STEPPING_MASK) == 
             IX_FEATURE_CTRL_SILICON_TYPE_B0)
           {
	       /*Only when Ethernet is available, then add dynamic entries */
               if (((ixFeatureCtrlComponentCheck(IX_FEATURECTRL_ETH0) == 
                     IX_FEATURE_CTRL_COMPONENT_ENABLED) && (0 == portId)) ||
                   ((ixFeatureCtrlComponentCheck(IX_FEATURECTRL_ETH1) == 
                     IX_FEATURE_CTRL_COMPONENT_ENABLED) && (1 == portId)))
	       {
                   ixEthMiiPhyReset(ixEthAccCodeletPhyAddresses[portId]);

		   /* Set each phy properties */
		   ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
				  ixEthAccCodeletPhyConf.speed100, 
				  ixEthAccCodeletPhyConf.fullDuplex, 
				  ixEthAccCodeletPhyConf.autonegotiate);
	           /* wait until the link is up before setting the MAC duplex
	  	    * mode, the PHY duplex mode may change after autonegotiation 
		    */
	           (void)ixEthAccCodeletLinkUpCheck(portId);
	           (void)ixEthAccCodeletLinkMonitor(portId);
	       
	           printf("\nPHY %d configuration:\n", portId);
	           ixEthMiiPhyShow(ixEthAccCodeletPhyAddresses[portId]);
               } 
           }
           else if ((ixFeatureCtrlProductIdRead() & IX_FEATURE_CTRL_SILICON_STEPPING_MASK) == 
                  IX_FEATURE_CTRL_SILICON_TYPE_A0) 
           {
               ixEthMiiPhyReset(ixEthAccCodeletPhyAddresses[portId]);

	       /* Set each phy properties */
	       ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
			         ixEthAccCodeletPhyConf.speed100, 
			         ixEthAccCodeletPhyConf.fullDuplex, 
			         ixEthAccCodeletPhyConf.autonegotiate);
	       /* wait until the link is up before setting the MAC duplex
	        * mode, the PHY duplex mode may change after autonegotiation 
		*/
	       (void)ixEthAccCodeletLinkUpCheck(portId);
	       (void)ixEthAccCodeletLinkMonitor(portId);
	       
	       printf("\nPHY %d configuration:\n", portId);
	       ixEthMiiPhyShow(ixEthAccCodeletPhyAddresses[portId]);
           }
           else
           {
               printf("LinkSetup: Error. Operation for other silicon stepping is undefined!.\n");
               return (IX_FAIL);
           }
       }
       else if (ixFeatureCtrlDeviceRead() == IX_FEATURE_CTRL_DEVICE_TYPE_IXP46X)
       {
           ixEthMiiPhyReset(ixEthAccCodeletPhyAddresses[portId]);

           /* Set each phy properties */
           ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
                             ixEthAccCodeletPhyConf.speed100,
                             ixEthAccCodeletPhyConf.fullDuplex,
                             ixEthAccCodeletPhyConf.autonegotiate);
	   /* wait until the link is up before setting the MAC duplex
	    * mode, the PHY duplex mode may change after autonegotiation 
	    */
	   (void)ixEthAccCodeletLinkUpCheck(portId);
	   (void)ixEthAccCodeletLinkMonitor(portId);
	   
	   printf("\nPHY %d configuration:\n", portId);
	   ixEthMiiPhyShow(ixEthAccCodeletPhyAddresses[portId]);
       }
   }
   return (IX_SUCCESS);
}


/**
 * Function definition: ixEthAccCodeletLinkLoopbackEnable()
 *
 * This function sets the PHY in test loopback mode
 * 
 */

IX_STATUS ixEthAccCodeletLinkLoopbackEnable(IxEthAccPortId portId)
{
    /* force the PHY setup to 100 Mb full Duplex */
    ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
		      TRUE, 
		      TRUE, 
		      FALSE);

    /* Enable PHY Loopback */
    ixEthMiiPhyLoopbackEnable(ixEthAccCodeletPhyAddresses[portId]);

    /* Get the link status. This is only used to display the current
     * state of the link on the console
     */
    (void)ixEthAccCodeletLinkUpCheck(portId);
    (void)ixEthAccCodeletLinkMonitor(portId);

    return (IX_SUCCESS);
}

/**
 * Function definition: ixEthAccCodeletLinkLoopbackDisable()
 *
 * This function turns off the PHY loopback mode
 * 
 */

IX_STATUS ixEthAccCodeletLinkLoopbackDisable(IxEthAccPortId portId)
{
    /* Disable PHY Loopback */
    ixEthMiiPhyLoopbackDisable(ixEthAccCodeletPhyAddresses[portId]);

    /* reset the PHY */
    ixEthMiiPhyReset(ixEthAccCodeletPhyAddresses[portId]);

    /* Set the phy properties */
    ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
		      ixEthAccCodeletPhyConf.speed100, 
		      ixEthAccCodeletPhyConf.fullDuplex, 
		      ixEthAccCodeletPhyConf.autonegotiate);

    /* get the link status */
    (void)ixEthAccCodeletLinkUpCheck(portId);
    (void)ixEthAccCodeletLinkMonitor(portId);

    printf("\nPHY %d configuration:\n", portId);
    ixEthMiiPhyShow(ixEthAccCodeletPhyAddresses[portId]);

    return (IX_SUCCESS);
}

/**
 * Function definition: ixEthAccCodeletPortSlowSpeedSet()
 *
 * This function sets the port to 10 Mbit with no negotiaton
 *
 */

IX_STATUS ixEthAccCodeletLinkSlowSpeedSet(IxEthAccPortId portId)
{
   BOOL speed, linkUp, fullDuplex, autoneg;
    /* get current duplex mode  */
   ixEthMiiLinkStatus(ixEthAccCodeletPhyAddresses[portId], 
		       &linkUp, 
		       &speed, 
		       &fullDuplex, 
		       &autoneg);
    /* set 10 Mbit, current duplex mode, no negotiation */
    ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
		      FALSE, 
		      fullDuplex, 
		      FALSE);
    ixEthMiiPhyReset(ixEthAccCodeletPhyAddresses[portId]);

    /* get the link status */
    (void)ixEthAccCodeletLinkUpCheck(portId);
    (void)ixEthAccCodeletLinkMonitor(portId);

    printf("\nPHY %d configuration:\n", portId);
    ixEthMiiPhyShow(ixEthAccCodeletPhyAddresses[portId]);

    return (IX_SUCCESS);
}

/**
 * Function definition: ixEthAccCodeletPortDefaultSpeedSet()
 *
 * This function restores the port default rate and negotiation mode.
 * 
 */

IX_STATUS ixEthAccCodeletLinkDefaultSpeedSet(IxEthAccPortId portId)
{
    /* set default values */
    ixEthMiiPhyConfig(ixEthAccCodeletPhyAddresses[portId],
		      ixEthAccCodeletPhyConf.speed100, 
		      ixEthAccCodeletPhyConf.fullDuplex, 
		      ixEthAccCodeletPhyConf.autonegotiate);

    ixEthMiiPhyReset(ixEthAccCodeletPhyAddresses[portId]);

    /* get the link status */
    (void)ixEthAccCodeletLinkUpCheck(portId);
    (void)ixEthAccCodeletLinkMonitor(portId);

    return (IX_SUCCESS);
}
