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
#include "LApp.h"

#define etherStop 51
int g_menuSel;		//declare an uninitialized, non-const, global variable

XGpioPs Gpio;
int Status;
XGpioPs_Config *GPIOConfigPtr;
XScuGic InterruptController;
static XScuGic_Config *GicConfig;

int g_iTestCounter;	//global test counter variable

int main()
{
	// Local Variables for main()
	int i = 0;				// index
	int	menusel = 99999;	// Menu Select
	//int mode = 9;			// Mode of Operation
	int enable_state = 0; 	// 0: disabled, 1: enabled
	int thres = 0;			// Trigger Threshold
	//char updateint = 'N';	// switch to change integral values
	u32 databuff = 0;		// size of the data buffer

	g_iTestCounter = 0;		// initialize global test counter
	g_menuSel = 99999;		// initialize global holder variable

	// SD Card Variables
	int doMount = 0;
	FRESULT i_SDReturn = FR_OK;
	FATFS fatfs;

	for (i=0; i<32; i++ ) { RecvBuffer[i] = '_'; }		// Clear RecvBuffer Variable
	for (i=0; i<32; i++ ) { SendBuffer[i] = '_'; }		// Clear SendBuffer Variable

	//*******************Setup the UART **********************//
	XUartPs_Config *Config = XUartPs_LookupConfig(UART_DEVICEID);
	if (NULL == Config) { return 1;}
	Status = XUartPs_CfgInitialize(&Uart_PS, Config, Config->BaseAddress);
	if (Status != 0){ return 1;	}

	/* Conduct a Selftest for the UART */
	Status = XUartPs_SelfTest(&Uart_PS);
	if (Status != 0) { return 1; }

	/* Set to normal mode. */
	XUartPs_SetOperMode(&Uart_PS, XUARTPS_OPER_MODE_NORMAL);
	//*******************Setup the UART **********************//

	//*******************Receive and Process Packets **********************//
	Xil_Out32 (XPAR_AXI_GPIO_0_BASEADDR, 11);
	Xil_Out32 (XPAR_AXI_GPIO_1_BASEADDR, 71);
	Xil_Out32 (XPAR_AXI_GPIO_2_BASEADDR, 167);
	Xil_Out32 (XPAR_AXI_GPIO_3_BASEADDR, 2015);
	Xil_Out32 (XPAR_AXI_GPIO_4_BASEADDR, 12);
	Xil_Out32 (XPAR_AXI_GPIO_5_BASEADDR, 75);
	Xil_Out32 (XPAR_AXI_GPIO_6_BASEADDR, 75);
	Xil_Out32 (XPAR_AXI_GPIO_7_BASEADDR, 50);
	Xil_Out32 (XPAR_AXI_GPIO_8_BASEADDR, 25);
	//*******************Receive and Process Packets **********************//

	//******************Setup Detector and Module Objects*****************//
	//LDetector *Detector = LDetector();
	//Detector->SetMode(1); // Processed Data Mode
	//******************Setup Detector and Module Objects*****************//

	//******************Setup Ethernet Connection*****************//
#if defined (__arm__) || defined(__aarch64__)
#if XPAR_GIGE_PCS_PMA_SGMII_CORE_PRESENT == 1 || XPAR_GIGE_PCS_PMA_1000BASEX_CORE_PRESENT == 1
int ProgramSi5324(void);
int ProgramSfpPhy(void);
#endif
#endif

#if __aarch64__
	Xil_DCacheDisable();
#endif

struct ip_addr ipaddr, netmask, gw;

/* the mac address of the board. this should be unique per board */
unsigned char mac_ethernet_address[] =
{ 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

echo_netif = &server_netif;

#if defined (__arm__) || defined(__aarch64__)
#if XPAR_GIGE_PCS_PMA_SGMII_CORE_PRESENT == 1 || XPAR_GIGE_PCS_PMA_1000BASEX_CORE_PRESENT == 1
	ProgramSi5324();
	ProgramSfpPhy();
#endif
#endif

	init_platform();
	ps7_post_config();
	Xil_DCacheDisable();	//
	InitializeAXIDma();		// Initialize the AXI DMA Transfer Interface
	Xil_Out32 (XPAR_AXI_GPIO_16_BASEADDR, 16384);
	Xil_Out32 (XPAR_AXI_GPIO_17_BASEADDR , 1);
	InitializeInterruptSystem(XPAR_PS7_SCUGIC_0_DEVICE_ID);

#if LWIP_DHCP==1
    ipaddr.addr = 0;
	gw.addr = 0;
	netmask.addr = 0;
#else
	/* initliaze IP addresses to be used */
	IP4_ADDR(&ipaddr,  172, 30,   0, 10);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	//IP4_ADDR(&gw,      192, 168,   1,  1);
#endif

	lwip_init();

  	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(echo_netif, &ipaddr, &netmask,
						&gw, mac_ethernet_address,
						PLATFORM_EMAC_BASEADDR)) {
		xil_printf("Error adding N/W interface\n\r");
		return -1;
	}
	netif_set_default(echo_netif);

	/* now enable interrupts */
	platform_enable_interrupts();

	/* specify that the network if is up */
	netif_set_up(echo_netif);

#if (LWIP_DHCP==1)
	/* Create a new DHCP client for this interface.
	 * Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
	 * the predefined regular intervals after starting the client.
	 */
	dhcp_start(echo_netif);
	dhcp_timoutcntr = 24;

	while(((echo_netif->ip_addr.addr) == 0) && (dhcp_timoutcntr > 0))
		xemacif_input(echo_netif);

	if (dhcp_timoutcntr <= 0) {
		if ((echo_netif->ip_addr.addr) == 0) {
			xil_printf("DHCP Timeout\r\n");
			xil_printf("Configuring default IP of 192.168.1.10\r\n");
			IP4_ADDR(&(echo_netif->ip_addr),  192, 168,   1, 10);
			IP4_ADDR(&(echo_netif->netmask), 255, 255, 255,  0);
			IP4_ADDR(&(echo_netif->gw),      192, 168,   1,  1);
		}
	}

	ipaddr.addr = echo_netif->ip_addr.addr;
	gw.addr = echo_netif->gw.addr;
	netmask.addr = echo_netif->netmask.addr;
#endif

	/* start the application (web server, rxtest, txtest, etc..) */
	start_application();

	//******************Setup Ethernet Connection*****************//

	//****************** Mount SD Card *****************//
	if( doMount == 0 )	//make sure we only mount once
	{
		//i_SDReturn = f_mount(0, "", 0);			// unmount anything that was in logical drive 0:/
		//xil_printf("mount: %d\r\n",i_SDReturn);
		i_SDReturn = f_mount(&fatfs, "", 0);	// Mount the SD card in the default logical drive // This will mount upon first access to the volume
		xil_printf("mount: %d\r\n",i_SDReturn);
		doMount = 1;
	}
	//****************** Mount SD Card *****************//

	//****************** Write to SD Card *****************//
/*	int iterator = 0;
	unsigned int numBytesWritten = 0;
	unsigned int numBytesRead = 0;
	unsigned char myData256[256] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	unsigned char myData512[512] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	unsigned char myBuff2[512] = "";

	FRESULT Res = FR_OK;
	FIL fil;
	FIL fil2;
	FILINFO finfo;

	Res = f_open(&fil, "ft.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

	Res = f_write(&fil, myData256, 256, &numBytesRead);

	Res = f_close(&fil);

	while(1)
	{
		Res = f_open(&fil2, "Test123.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);		//open only if the file exists
		if (Res) {break;}

		Res = f_write(&fil2, (const void *)myData512, sizeof(myData512), &numBytesWritten);	//Write some ints to the file in binary
		if (Res) {break;}

		Res = f_close(&fil2);															//close the file
		if (Res) {break;}

		Res = f_open(&fil2, "Test123.txt", FA_READ);									//open the file read only
		if (Res) {break;}

		Res = f_read(&fil2, myBuff2, 512, &numBytesRead);								//read back from it
		if (Res) {break;}

		Res = f_close(&fil2);															//close the file
		if (Res) {break;}

		if(numBytesWritten == numBytesRead)
			break;

		++iterator;
		sleep(1);
	} */
	//****************** Write to SD Card *****************//

//	xil_printf("Res: %d\r\n",Res);

	//Testing for dynamic allocation of arrays
//	int * testarray = NULL;							//Update the linker script in Echo3/src to adjust the size of the heap; if it's too small, then we can't allocate this
//	testarray = (int *)calloc(10000, sizeof(int));	//10 000 * sizeof(int) is too large to allocate dynamically
//	if(testarray == NULL)
//		xil_printf("Could not allocate array\r\n");
//
//	testarray[10] = 11;
//	testarray[55] = 56;
//	testarray[567] = 88;
//
//	xil_printf("%d_%d_%d_%d_%d\r\n",testarray[1],testarray[10],testarray[55],testarray[99],testarray[567]);
//
//	free(testarray);
//	testarray = NULL;

	// *********** Setup the Hardware Reset GPIO ****************//
	GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);
	if (Status != XST_SUCCESS) { return XST_FAILURE; }
	XGpioPs_SetDirectionPin(&Gpio, SW_BREAK_GPIO, 1);
	// *********** Setup the Hardware Reset MIO ****************//

	// ******************* POLLING LOOP *******************//
	//xil_printf("\n\r Turn on Local Echo: under Terminal-Setup in Tera Term \n\r");
	Xil_Out32(XPAR_AXI_GPIO_18_BASEADDR, 1);
	while(1){
		sw = 0;   //  stop switch reset to 0
		XUartPs_SetOptions(&Uart_PS,XUARTPS_OPTION_RESET_RX);	// Clear UART Read Buffer
		for (i=0; i<32; i++ ) { RecvBuffer[i] = '_'; }			// Clear RecvBuffer Variable

		sleep(0.5);  // Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 0.5 s
		xil_printf("\n\r v2.1 \n\r");
		xil_printf("\n\r MAIN MENU \n\r");
		xil_printf("******************************\n\r");
		xil_printf(" 0) Set Mode of Operation\n\r");
		xil_printf(" 1) Enable or disable the system\n\r");
		xil_printf(" 2) Continuously Read of Processed Data\n\r");
		xil_printf("\n\r **Setup Parameters ** \n\r");
		xil_printf(" 3) Set Trigger Threshold\n\r");
		xil_printf(" 4) Set Integration Times (number of clock cycles * 4ns) \n\r");
		xil_printf("\n\r ** Additional Commands ** \n\r");
		xil_printf(" 5) Perform a DMA transfer of Waveform Data\n\r");
		xil_printf(" 6) Perform a DMA transfer of Processed Data\n\r");
		xil_printf(" 7) Check the Size of the Data Buffered (Max = 4095) \n\r");
		xil_printf(" 8) Clear the Processed Data Buffers\n\r");
		xil_printf(" 9) Execute Print of Data on DRAM \n\r");
		xil_printf("******************************\n\n\r");
		while (XUartPs_IsSending(&Uart_PS)) {i++;}  // Wait until Write Buffer is Sent

		readEtherPoll();
		menusel = g_menuSel;
		if ( menusel < 0 || menusel > 9 )
		{
			xil_printf(" Invalid Command: Enter 0-9 \n\r");
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
		}

		switch (menusel) { // Switch-Case Menu Select

		case 0: //Set Mode of Operation
			g_mode = 99; //Detector->GetMode();
			xil_printf("\n\r Waveform Data: \t Enter 0 <return>\n\r");
			xil_printf(" LPF Waveform Data: \t Enter 1 <return>\n\r");
			xil_printf(" DFF Waveform Data: \t Enter 2 <return>\n\r");
			xil_printf(" TRG Waveform Data: \t Enter 3 <return>\n\r");
			xil_printf(" Processed Data: \t Enter 4 <return>\n\r");

			readEtherPoll();
			g_mode = g_menuSel;

			if (g_mode < 0 || g_mode > 4 ) { xil_printf("Invalid Command\n\r"); break; }
			// mode = 0, AA waveform
			// mode = 1, LPF waveform
			// mode = 2, DFF waveform
			// mode = 3, TRG waveform
			// mode = 4, Processed Data
			//Detector->SetMode(mode);  // Set Mode for Detector
			Xil_Out32 (XPAR_AXI_GPIO_14_BASEADDR, ((u32)g_mode));
			// Register 14
			if ( g_mode == 0 ) { xil_printf("Transfer AA Waveforms\n\r"); }
			if ( g_mode == 1 ) { xil_printf("Transfer LPF Waveforms\n\r"); }
			if ( g_mode == 2 ) { xil_printf("Transfer DFF Waveforms\n\r"); }
			if ( g_mode == 3 ) { xil_printf("Transfer TRG Waveforms\n\r"); }
			if ( g_mode == 4 ) { xil_printf("Transfer Processed Data\n\r"); }
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 1: //Enable or disable the system
			xil_printf("\n\r Disable: Enter 0 <return>\n\r");
			xil_printf(" Enable: Enter 1 <return>\n\r");
			//readEtherPoll();
			//enable_state = g_menuSel;
			enable_state = 1;
			if (enable_state != 0 && enable_state != 1) { xil_printf("Invalid Command\n\r"); break; }
			Xil_Out32(XPAR_AXI_GPIO_18_BASEADDR, ((u32)enable_state));
			// Register 18 Out enabled, In Disabled
			if ( enable_state == 1 ) { xil_printf("DAQ Enabled\n\r"); }
			if ( enable_state == 0 ) { xil_printf("DAQ Disabled\n\r"); }
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 2: //Continuously Read of Processed Data
			xil_printf("\n\r ********Data Acquisition:\n\r");
			xil_printf(" Press 'q' to Stop or Press Hardware USR reset button  \n\r");
			DAQ();
			sw = 0;   // broke out of the read loop, stop swith reset to 0
			break;

		case 3: //Set Threshold
			xil_printf("\n\r Existing Threshold = %d \n\r",Xil_In32(XPAR_AXI_GPIO_10_BASEADDR));
			xil_printf(" Enter Threshold (6144 to 10240) <return> \n\r");

			readEtherPoll();
			thres = g_menuSel;

			Xil_Out32(XPAR_AXI_GPIO_10_BASEADDR, ((u32)thres));
			xil_printf("New Threshold = %d \n\r",Xil_In32(XPAR_AXI_GPIO_10_BASEADDR));
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 4: //Set Integration Times
			xil_printf("\n\r Existing Integration Times \n\r");
			xil_printf(" Time = 0 ns is when the Pulse Crossed Threshold \n\r");
			xil_printf(" Baseline Integral Window \t [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_0_BASEADDR))*4 );
			xil_printf(" Short Integral Window \t [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_1_BASEADDR))*4 );
			xil_printf(" Long Integral Window  \t [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_2_BASEADDR))*4 );
			xil_printf(" Full Integral Window  \t [-200ns,%dns] \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_3_BASEADDR))*4 );

			// Removed this section, if we are hitting this code, we want to change the values
//			xil_printf(" Change: (Y)es (N)o <return>\n\r");
/*			if (updateint == 'N' || updateint == 'n') { break; }
			if (updateint == 'Y' || updateint == 'y') { SetIntegrationTimes(0); } */
			SetIntegrationTimes(0);	// 0 means we want to change the AA waveforms, 1 for LPF, 2 for DFF
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 5: //Perform a DMA transfer
			xil_printf("\n\r Perform DMA Transfer of Waveform Data\n\r");
			xil_printf(" Press 'q' to Exit Transfer  \n\r");
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000);
			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			PrintData();		// Display data to console.
			sw = 0;   // broke out of the read loop, stop swith reset to 0
			break;

		case 6: //Perform a DMA transfer of Processed data
			xil_printf("\n\r ********Perform DMA Transfer of Processed Data \n\r");
			xil_printf(" Press 'q' to Exit Transfer  \n\r");
			xil_printf("This functions does not do anything\r\n");
//			//Xil_Out32 (XPAR_AXI_GPIO_18_BASEADDR, 0);	// Disable : GPIO Reg Capture Module
//			Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 1);	// Enable: GPIO Reg to Readout Data MUX
//			//sleep(1);				// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
//			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000); // Transfer from BRAM to DRAM, start address 0xa000000, 16-bit length
//			Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);
//			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
//			Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 0); 	// Disable: GPIO Reg turn off Readout Data MUX
//			ClearBuffers();
//			PrintData();								// Display data to console.
//			//Xil_Out32 (XPAR_AXI_GPIO_18_BASEADDR, 1);	// Enable : GPIO Reg Capture Module
//			sw = 0;   // broke out of the read loop, stop swith reset to 0
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 7: //Check the Size of the Data Buffers
			databuff = Xil_In32 (XPAR_AXI_GPIO_11_BASEADDR);
			xil_printf("\n\r BRAM Data Buffer Size = %d \n\r",databuff);
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

		case 8: //Clear the processed data buffers
			xil_printf("\n\r Clear the Data Buffers\n\r");
			xil_printf("This functions does not do anything\r\n");
//			ClearBuffers();
			sleep(1); 			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 1 s
			break;

//		case 9: //Print DMA Data
//			xil_printf("\n\r Print Data\n\r");
//			PrintData();
//			break;
		case 9: //Print DMA Data
           ether();
			break;
		default :
			break;
		} // End Switch-Case Menu Select

	}


	/* never reached */
	cleanup_platform();

	return 0;
}

//////////////////////////// InitializeAXIDma////////////////////////////////
// Sets up the AXI DMA
int InitializeAXIDma(void) {
	u32 tmpVal_0 = 0;
	u32 tmpVal_1 = 0;

	tmpVal_0 = Xil_In32(XPAR_AXI_DMA_0_BASEADDR +  0x30);

	tmpVal_0 = tmpVal_0 | 0x1001; //<allow DMA to produce interrupts> 0 0 <run/stop>

	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x30, tmpVal_0);
	tmpVal_1 = Xil_In32(XPAR_AXI_DMA_0_BASEADDR + 0x30);

	return 0;
}
//////////////////////////// InitializeAXIDma////////////////////////////////

//////////////////////////// InitializeInterruptSystem////////////////////////////////
int InitializeInterruptSystem(u16 deviceID) {
	int Status;

	GicConfig = XScuGic_LookupConfig (deviceID);

	if(NULL == GicConfig) {

		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&InterruptController, GicConfig, GicConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;

	}

	Status = SetUpInterruptSystem(&InterruptController);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;

	}

	Status = XScuGic_Connect (&InterruptController,
			XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR,
			(Xil_ExceptionHandler) InterruptHandler, NULL);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;

	}

	XScuGic_Enable(&InterruptController, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR );

	return XST_SUCCESS;

}
//////////////////////////// InitializeInterruptSystem////////////////////////////////


//////////////////////////// Interrupt Handler////////////////////////////////
void InterruptHandler (void ) {

	u32 tmpValue;
	tmpValue = Xil_In32(XPAR_AXI_DMA_0_BASEADDR + 0x34);
	tmpValue = tmpValue | 0x1000;
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x34, tmpValue);

	global_frame_counter++;
}
//////////////////////////// Interrupt Handler////////////////////////////////

//////////////////////////// SetUp Interrupt System////////////////////////////////
int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr) {
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, XScuGicInstancePtr);
	Xil_ExceptionEnable();
	return XST_SUCCESS;

}
//////////////////////////// SetUp Interrupt System////////////////////////////////

//////////////////////////// ReadCommandPoll////////////////////////////////
// Function used to clear the read buffer
// Read In new command, expecting a <return>
// Returns buffer size read
int ReadCommandPoll() {
	u32 rbuff = 0;			// read buffer size returned
	int i = 0; 				// index
	XUartPs_SetOptions(&Uart_PS,XUARTPS_OPTION_RESET_RX);	// Clear UART Read Buffer
	for (i=0; i<32; i++ ) { RecvBuffer[i] = '_'; }			// Clear RecvBuffer Variable
	while (!(RecvBuffer[rbuff-1] == '\n' || RecvBuffer[rbuff-1] == '\r' || RecvBuffer[rbuff-1] == 'd')) {
		rbuff += XUartPs_Recv(&Uart_PS, &RecvBuffer[rbuff],(32 - rbuff));
		sleep(0.1);			// Built in Latency ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 0.1 s
	}
	return rbuff;

}
//////////////////////////// ReadCommandPoll////////////////////////////////


//////////////////////////// Set Integration Times ////////////////////////////////
// wfid = 	0 for AA
//			1 for DFF
//			2 for LPF
// At the moment, the software is expecting a 5 char buffer.  This needs to be fixed.
void SetIntegrationTimes(u8 wfid){
	int integrals[8];
	u32 setsamples[8];
	int i = 0;

	for (i = 0; i<8; i++) {integrals[i] = 0; setsamples[i] = 0;}

	switch (wfid) { // Switch-Case for performing set integrals

			case 0: //AA Integrals
				xil_printf("  Enter Baseline Stop Time in ns: -52 to 0 <return> \t");
				readEtherPoll();
				integrals[0] = g_menuSel;
				xil_printf("\n\r");

				xil_printf("  Enter Short Integral Stop Time in ns: -52 to 8000 <return> \t");
				readEtherPoll();
				integrals[1] = g_menuSel;
				xil_printf("\n\r");

				xil_printf("  Enter Long Integral Stop Time in ns: -52 to 8000 <return> \t");
				readEtherPoll();
				integrals[2] = g_menuSel;
				xil_printf("\n\r");

				xil_printf("  Enter Full Integral Stop Time in ns: -52 to 8000 <return> \t");
				readEtherPoll();
				integrals[3] = g_menuSel;
				xil_printf("\n\r");

				setsamples[0] = ((u32)((integrals[0]+52)/4));
				setsamples[1] = ((u32)((integrals[1]+52)/4));
				setsamples[2] = ((u32)((integrals[2]+52)/4));
				setsamples[3] = ((u32)((integrals[3]+52)/4));

				Xil_Out32 (XPAR_AXI_GPIO_0_BASEADDR, setsamples[0]);
				Xil_Out32 (XPAR_AXI_GPIO_1_BASEADDR, setsamples[1]);
				Xil_Out32 (XPAR_AXI_GPIO_2_BASEADDR, setsamples[2]);
				Xil_Out32 (XPAR_AXI_GPIO_3_BASEADDR, setsamples[3]);

				xil_printf("\n\r  Inputs Rounded to the Nearest 4 ns : Number of Samples\n\r");
				xil_printf("  Baseline Integral Window  [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_0_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_0_BASEADDR) );
				xil_printf("  Short Integral Window 	  [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_1_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_1_BASEADDR));
				xil_printf("  Long Integral Window      [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_2_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_2_BASEADDR));
				xil_printf("  Full Integral Window      [-200ns,%dns]: %d \n\r",-52 + ((int)Xil_In32(XPAR_AXI_GPIO_3_BASEADDR))*4, 38+Xil_In32(XPAR_AXI_GPIO_3_BASEADDR));

				break;

			case 1://DFF Integrals
				xil_printf("Enter Baseline Stop Time in ns: -32 to 0 <return> \t");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%d",&integrals[0]);
				xil_printf("\n\r");

				xil_printf("Enter Integral Stop Time in ns: 0 to 500 <return> \t");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%d",&integrals[1]);
				xil_printf("\n\r");

				setsamples[0] = ((u32)((integrals[0]+32)/4));
				setsamples[1] = ((u32)((integrals[1])/4));

				Xil_Out32 (XPAR_AXI_GPIO_7_BASEADDR, setsamples[0]);
				Xil_Out32 (XPAR_AXI_GPIO_8_BASEADDR, setsamples[1]);

				xil_printf("Inputs Rounded to the Nearest 4 ns : Number of Samples");
				xil_printf("Baseline Integral Window  [-32ns,%dns]: %d \n\r",-32 + ((int)Xil_In32(XPAR_AXI_GPIO_7_BASEADDR))*4, 9+Xil_In32(XPAR_AXI_GPIO_7_BASEADDR) );
				xil_printf("Integral Window 	      [0ns,%dns]: %d \n\r",((int)Xil_In32(XPAR_AXI_GPIO_8_BASEADDR))*4, 1+Xil_In32(XPAR_AXI_GPIO_8_BASEADDR));

				break;

			case 2://LPF Integrals
				xil_printf("Enter Baseline Stop Time in ns: -60 to 0 <return> \t");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%d",&integrals[0]);
				xil_printf("\n\r");

				xil_printf("Enter Tail Integral Stop Time in ns: 200 to 8000 <return> \t");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%d",&integrals[1]);
				xil_printf("\n\r");

				xil_printf("Enter Full Integral Stop Time in ns: 0 to 8000 <return> \t");
				ReadCommandPoll();
				sscanf(RecvBuffer,"%d",&integrals[2]);
				xil_printf("\n\r");

				setsamples[0] = ((u32)((integrals[0]+60)/4));
				setsamples[1] = ((u32)((integrals[1])/4));
				setsamples[2] = ((u32)((integrals[2])/4));

				Xil_Out32 (XPAR_AXI_GPIO_4_BASEADDR, setsamples[0]);
				Xil_Out32 (XPAR_AXI_GPIO_5_BASEADDR, setsamples[1]);
				Xil_Out32 (XPAR_AXI_GPIO_6_BASEADDR, setsamples[2]);

				xil_printf("Inputs Rounded to the Nearest 4 ns : Number of Samples");
				xil_printf("Baseline Integral Window  [-32ns,%dns]: %d \n\r",-32 + ((int)Xil_In32(XPAR_AXI_GPIO_4_BASEADDR))*4, 16+Xil_In32(XPAR_AXI_GPIO_4_BASEADDR) );
				xil_printf("Tail Integral Window 	  [200ns,%dns]: %d \n\r",((int)Xil_In32(XPAR_AXI_GPIO_5_BASEADDR))*4, 1+Xil_In32(XPAR_AXI_GPIO_5_BASEADDR));
				xil_printf("Full Integral Window 	  [0ns,%dns]: %d \n\r",((int)Xil_In32(XPAR_AXI_GPIO_6_BASEADDR))*4, 1+Xil_In32(XPAR_AXI_GPIO_6_BASEADDR));

				break;

			default:
				break;
	}

	return;
}
//////////////////////////// Set Integration Times ////////////////////////////////

//////////////////////////// PrintData ////////////////////////////////
int PrintData( ){
	int index = 0;
	int dram_addr = 0;
	int dram_base = 0xa000000;		// 167772160
	int dram_ceiling = 0xa00c000;	// 167821312 - 167772160 = 49152
	unsigned int * data_array = NULL;
	data_array = calloc(SIZEOF_DATA_ARRAY, sizeof(unsigned int));	//dynamically allocate buffer to save on stack space
	if(data_array == NULL)											//check we have allocated the buffer
		return 1;
	FRESULT ffres;
	uint numBytesWritten = 0;
	FIL datafile;

	Xil_DCacheInvalidateRange(0x00000000, 65536);	//make sure the PS doesn't corrupt the memory  by accessing while doing DMA transfer

	for (dram_addr = dram_base; dram_addr <= dram_ceiling; dram_addr+=4, index++){	//read over the addresses and save into an array to save to SD
		data_array[index] = Xil_In32(dram_addr);
	}

	//Save to SD card here
	ffres = f_open(&datafile, "datatest.txt", FA_OPEN_ALWAYS | FA_WRITE);
	ffres = f_lseek(&datafile, file_size(&datafile));
	ffres = f_write(&datafile, data_array, SIZEOF_DATA_ARRAY, &numBytesWritten);
	ffres = f_close(&datafile);

	free(data_array);
	data_array = NULL;
	return sw;
}
//////////////////////////// PrintData ////////////////////////////////


//////////////////////////// Clear Processed Data Buffers ////////////////////////////////
//void ClearBuffers() {
//	Xil_Out32(XPAR_AXI_GPIO_9_BASEADDR,1);
//	usleep(1);						// Built in Latency - 1 us // 1 clock cycle-ish
//	Xil_Out32(XPAR_AXI_GPIO_9_BASEADDR,0);
//}
//////////////////////////// Clear Processed Data Buffers ////////////////////////////////

//////////////////////////// DAQ ////////////////////////////////
int DAQ(){

	xil_printf("\r\nDAQ is looping\r\n");

	Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 1);				//enable read out
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x48, 0xa000000);	//tell the fpga what address to start at
	Xil_Out32 (XPAR_AXI_DMA_0_BASEADDR + 0x58 , 65536);		//and how much to transfer
	usleep(54); 			// Built in Latency - 54 us
	Xil_Out32 (XPAR_AXI_GPIO_15_BASEADDR, 0);				//disable read out

	Xil_Out32(XPAR_AXI_GPIO_9_BASEADDR,1);	//Clear the buffers //Reset the read address
	usleep(1);								// Built in Latency - 1 us // 1 clock cycle-ish
	Xil_Out32(XPAR_AXI_GPIO_9_BASEADDR,0);

	PrintData();

	return sw;
}
//////////////////////////// DAQ ////////////////////////////////

int ether(){
	int timer = 0;
	GPIOConfigPtr = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	Status = XGpioPs_CfgInitialize(&Gpio, GPIOConfigPtr, GPIOConfigPtr ->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XGpioPs_SetDirectionPin(&Gpio, etherStop, 1);
	int sw;

	//	a = pbuf_free_ooseq_queued;
	g_txcomplete = 1;
	while (g_txcomplete == 1) {
		sw = XGpioPs_ReadPin(&Gpio, etherStop); //read pin

		if (sw == 1) { //sw=1 when switch is pushed
			g_txcomplete = 0; //invert value stored in toggle
			//continue; //break out and stop looping

		}
		if (TcpFastTmrFlag) {
			tcp_fasttmr();
			TcpFastTmrFlag = 0;
		}
		if (TcpSlowTmrFlag) {
			tcp_slowtmr();
			TcpSlowTmrFlag = 0;
		}
		xemacif_input(echo_netif);
		//transfer_data();	//Put printData here so that we send data each time
		//usleep(500000);
		DAQ();
		if(timer++ >= 100)
			break;
	}
	xil_printf("Broke out after 100 loops\r\n");

	/* Never reached */
	return 0;
}

