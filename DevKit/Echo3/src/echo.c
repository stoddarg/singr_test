/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "sleep.h"
#include <xil_io.h>
#include <xil_cache.h>

#include "xsdps.h"	/* SD device driver */
#include "ff.h"		/* SD Function Header */

#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

struct tcp_pcb *globalTCP_pcb;	//global struct
int g_menuSel;			//global menu variable
int g_buffer_addr;

int g_iBytesRead;	//global test counter variable

//SD Card Variables
static FIL fil2;
static u32 testdata[2048] = {};// __attribute__ ((aligned(32)));

int transfer_data(struct tcp_pcb *tpcb) {

	//keepalive
	return 0;
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP TCP echo server ------\n\r");
	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	/* set variables */
	char * buffer = NULL;
	char * pEnd = NULL;
	char cDone[8] = "141414\n";
	FRESULT ffres;
	uint NumBytesRead = 0;

	if (!p)
	{
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	/* allocate memory for the buffer and initialize it */
	buffer = (char *) malloc(p->len);
	memcpy(buffer, "0", 8);

	/* receive the data */
	memcpy(buffer, p->payload, p->len);

	if(g_menuSel == 9)	//if we want to transfer data, come here
	{
		if(buffer[0] == 'q')	//if the value sent was a 'q', then the user wants to break out of the
		{						//capturing loop and go back to the main menu
			xil_printf("quit\r\n");
			g_txcomplete = 0;
			g_menuSel = 0;
			free(buffer);
			free(pEnd);
			return ERR_OK;
		}
		if(buffer[0] == 'a')
		{
			xil_printf("continue");
			while(tcp_sndbuf(tpcb) < 65535){	//wait until the send buffer is clear and ready
			}

			ffres = f_open(&fil2, "test007.bin", FA_OPEN_EXISTING | FA_READ);
			ffres = f_lseek(&fil2, g_iBytesRead);
			ffres = f_read(&fil2, testdata, 8192, &NumBytesRead);
			g_iBytesRead += NumBytesRead;								//keep track of where we are in the file

			//queue up the data in the tcp_sndbuf
			err = tcp_write(tpcb, (void*)testdata, 8192, 0x01);			//write in the buffer we read from the file
			err = tcp_write(tpcb, (void*)cDone, strlen(cDone), 0x01);	//put in a 'finished' set of bytes

			ffres = f_close(&fil2);
		}
	}
	else	//Otherwise come here for the polling loop
	{
		if((p->len) < 9)	//check that the packet is int size or less
			g_menuSel = strtol(buffer, &pEnd, 10);	//gjs
		else
			g_menuSel = 99999;	//indicate that we didn't read a value from the buffer

		/* echo back the payload */
		/* in this case, we assume that the payload is < TCP_SND_BUF */
		if (tcp_sndbuf(tpcb) > p->len) {
			//err = tcp_write(tpcb, p->payload, p->len, 1);	//don't echo anything back
			//err = tcp_write(tpcb, 0x1, p->len, 1);
		} else
			xil_printf("no space in tcp_sndbuf\n\r");
	}

	/* free the received pbuf */
	pbuf_free(p);
	free (buffer);
	return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}
