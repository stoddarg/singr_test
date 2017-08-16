/*
 * readEtherPoll.c
 *
 *  Created on: Feb 24, 2017
 *      Author: GStoddard
 */

#include "readEtherPoll.h"

extern int g_menuSel;

int readEtherPoll()
{
	int returnval = 0;
	//reset global variable
	g_menuSel = 99999;

	/* receive and process packets */
	while (1) {
		if (TcpFastTmrFlag) {
			tcp_fasttmr();
			TcpFastTmrFlag = 0;
		}
		if (TcpSlowTmrFlag) {
			tcp_slowtmr();
			TcpSlowTmrFlag = 0;
		}
		xemacif_input(echo_netif);
		returnval = transfer_data();

		switch(returnval)
		{
		case 0:
			xil_printf("y\r\n");
			break;
		case 1:
			xil_printf("badfileopen\r\n");
			break;
		case 2:
			xil_printf("numbytesmismatch\r\n");
			break;
		case 3:
			xil_printf("indexmismatch\r\n");
			break;
		default:
			xil_printf("???\r\n");
			break;
		}
		if(g_menuSel < 10000 && g_menuSel >= -200)	//if we don't have the correct size menu select variable, keep waiting for input? //increased range on both sides of 0
		{											//essentially, if the global is set, we have received something
			break;
		}
	}

	return 0;
}
