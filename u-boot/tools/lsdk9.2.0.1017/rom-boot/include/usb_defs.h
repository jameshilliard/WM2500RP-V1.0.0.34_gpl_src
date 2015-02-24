#ifndef USB_DEFS_H
#define USB_DEFS_H

/*
 * These registers/address define are referred to 
 * 1. CI13420_CI13520_CI13620_CI13720_IPCS_PM_HighSpeedControllerCore_20A.pdf
 * 2. Virian_Spec.pdf
 */

#define BASE_ADDRESS	     		    0xA0000000
#define RESET_BASE_ADDRESS	        	(BASE_ADDRESS+0x18060000)
#define RESET_REGISTER_ADDR_OFFSERT		0x0000001C
#define RESET_REGISTER					(RESET_BASE_ADDRESS +  RESET_REGISTER_ADDR_OFFSERT)


// Varian RESET register bits defines
#define VARIAN_RESET_USB_HOST               (1 << 5)
#define VARIAN_RESET_USB_PHY                (1 << 4)
#define VARIAN_RESET_OHCI_DLL_RESET         (1 << 3)

// CI 13320A registers addr defines
#define CI13320A_USB_BASE				(BASE_ADDRESS+0x1b000000)
#define CI13320A_USB_USBCMD				(CI13320A_USB_BASE + 0x140)
#define CI13320A_USB_USBSTS				(CI13320A_USB_BASE + 0x144)
#define CI13320A_USB_USBINTR			(CI13320A_USB_BASE + 0x148)
#define CI13320A_USB_FRINDEX			(CI13320A_USB_BASE + 0x14C)
#define CI13320A_USB_PERIODICLISTBASE	(CI13320A_USB_BASE + 0x154)     //Host Mode
#define CI13320A_USB_DEVICEADDR			(CI13320A_USB_BASE + 0x154)     //Device Mode
#define CI13320A_USB_ASYNCLISTADDR		(CI13320A_USB_BASE + 0x158)     //Host Mode
#define CI13320A_USB_ENDPOINTLISTADDR	(CI13320A_USB_BASE + 0x158)     //Device Mode
#define CI13320A_USB_TTCTRL				(CI13320A_USB_BASE + 0x15C)     
#define CI13320A_USB_BURSTSIZE			(CI13320A_USB_BASE + 0x160)     
#define CI13320A_USB_TXFILLTUNING		(CI13320A_USB_BASE + 0x164)     
#define CI13320A_USB_TXTTFILLTUNING		(CI13320A_USB_BASE + 0x168)     
#define CI13320A_USB_IC_USB				(CI13320A_USB_BASE + 0x16C)     
#define CI13320A_USB_ULPI_VIEWPORT		(CI13320A_USB_BASE + 0x170)     
#define CI13320A_USB_ENDPTNAK			(CI13320A_USB_BASE + 0x178)     
#define CI13320A_USB_ENDPTNAKEN	    	(CI13320A_USB_BASE + 0x17C)     
#define CI13320A_USB_CONFIGFLAG	    	(CI13320A_USB_BASE + 0x180)     
#define CI13320A_USB_PORTSC0		   	(CI13320A_USB_BASE + 0x184)     
#define CI13320A_USB_OTGSC		    	(CI13320A_USB_BASE + 0x1A4)     
#define CI13320A_USB_USBMODE		   	(CI13320A_USB_BASE + 0x1A8)     
#define CI13320A_USB_ENDPTSETUPSTAT    	(CI13320A_USB_BASE + 0x1AC)     
#define CI13320A_USB_ENDPTPRIME       	(CI13320A_USB_BASE + 0x1B0)     
#define CI13320A_USB_ENDPTFLUSH       	(CI13320A_USB_BASE + 0x1B4)     
#define CI13320A_USB_ENDPTSTAT         	(CI13320A_USB_BASE + 0x1B8)     
#define CI13320A_USB_ENDPTCOMPLETE     	(CI13320A_USB_BASE + 0x1BC)     
#define CI13320A_USB_ENDPTCTRL0       	(CI13320A_USB_BASE + 0x1C0)     
#define CI13320A_USB_ENDPTCTRL1       	(CI13320A_USB_BASE + 0x1C4)     
#define CI13320A_USB_ENDPTCTRL2       	(CI13320A_USB_BASE + 0x1C8)     
#define CI13320A_USB_ENDPTCTRL3       	(CI13320A_USB_BASE + 0x1CC)     
#define CI13320A_USB_ENDPTCTRL4       	(CI13320A_USB_BASE + 0x1D0)     
#define CI13320A_USB_ENDPTCTRL5       	(CI13320A_USB_BASE + 0x1D4)     
#define CI13320A_USB_ENDPTCTRL6       	(CI13320A_USB_BASE + 0x1D8)     
#define CI13320A_USB_ENDPTCTRL7       	(CI13320A_USB_BASE + 0x1DC)     
#define CI13320A_USB_ENDPTCTRL8       	(CI13320A_USB_BASE + 0x1E0)     

// USBCMD bits define
#define CI13320A_USBCMD_RESET                    (1<<1)
#define CI13320A_USBCMD_RUN                      (1<<0)

// USBSTS bits define
#define CI13320A_USBSTS_GPTIMER_INTR1            (1<<25)
#define CI13320A_USBSTS_GPTIMER_INTR0            (1<<24)
#define CI13320A_USBSTS_HOST_PERIODIC_INTR       (1<<19)
#define CI13320A_USBSTS_HOST_ASYNC_INTR          (1<<18)
#define CI13320A_USBSTS_NAT_INTR                 (1<<16)
#define CI13320A_USBSTS_ASYNC_SCHEDULE_STAT      (1<<15)
#define CI13320A_USBSTS_PERIODIC_SCHEDULE_STAT   (1<<14)
#define CI13320A_USBSTS_RECLAMATION              (1<<13)
#define CI13320A_USBSTS_HCHALTED                 (1<<12)
#define CI13320A_USBSTS_ULPIINTR                 (1<<10)
#define CI13320A_USBSTS_DCSUSPEND                (1<<8)
#define CI13320A_USBSTS_SOFRECV                  (1<<7)
#define CI13320A_USBSTS_RESETRECV                (1<<6)
#define CI13320A_USBSTS_INTR_ASYNCADV            (1<<5)
#define CI13320A_USBSTS_SYS_ERR                  (1<<4)
#define CI13320A_USBSTS_FRAMELIST_ROLLOVER       (1<<3)
#define CI13320A_USBSTS_PORT_CHANGE              (1<<2)
#define CI13320A_USBSTS_ERROR_INTERRUPT          (1<<1)
#define CI13320A_USBSTS_INTR                     (1<<0)

// USBINTR bits define
#define CI13320A_USBINTR_GPTIMER_INTR1            (1<<25)
#define CI13320A_USBINTR_GPTIMER_INTR0            (1<<24)
#define CI13320A_USBINTR_HOST_PERIODIC_INTR       (1<<19)
#define CI13320A_USBINTR_HOST_ASYNC_INTR          (1<<18)
#define CI13320A_USBINTR_NAT_INTR                 (1<<16)
#define CI13320A_USBINTR_ULPIINTR                 (1<<10)
#define CI13320A_USBINTR_DCSUSPEND                (1<<8)
#define CI13320A_USBINTR_SOFRECV                  (1<<7)
#define CI13320A_USBINTR_RESETRECV                (1<<6)
#define CI13320A_USBINTR_INTR_ASYNCADV            (1<<5)
#define CI13320A_USBINTR_SYS_ERR                  (1<<4)
#define CI13320A_USBINTR_FRAMELIST_ROLLOVER       (1<<3)
#define CI13320A_USBINTR_PORT_CHANGE_INTR         (1<<2)
#define CI13320A_USBINTR_ERROR_INTR               (1<<1)
#define CI13320A_USBINTR_INTR                     (1<<0)


// USBMODE bits define
#define CI13320A_USBMODE_SRT			     	  (1<<15)
#define CI13320A_USBMODE_VBPS			     	  (1<<5)
#define CI13320A_USBMODE_SDIS			     	  (1<<4)
#define CI13320A_USBMODE_SLOM			     	  (1<<3)
#define CI13320A_USBMODE_ENDIAN					  (1<<2)
#define CI13320A_USBMODE_DEVICE					  (0x2) 
#define CI13320A_USBMODE_HOST					  (0x3) 
#endif