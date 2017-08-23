/*
 * xilffs_polled_example.c
 *
 *  Created on: Aug 8, 2017
 *      Author: GStoddard
 */


/******************************************************************************
*
* Copyright (C) 2013 - 2015 Xilinx, Inc.  All rights reserved.
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
/*****************************************************************************/
/**
*
* @file xilffs_polled_example.c
*
*
* @note This example uses file system with SD to write to and read from
* an SD card using ADMA2 in polled mode.
* To test this example File System should not be in Read Only mode.
*
* This example was tested using SD2.0 card and eMMC (using eMMC to SD adaptor).
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who Date     Changes
* ----- --- -------- -----------------------------------------------
* 1.00a hk  10/17/13 First release
* 2.2   hk  07/28/14 Make changes to enable use of data cache.
* 2.5   sk  07/15/15 Used File size as 8KB to test on emulation platform.
*
*</pre>
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "stdlib.h"
#include "xparameters.h"	/* SDK generated parameters */
#include "xsdps.h"		/* SD device driver */
#include "xil_printf.h"
#include "ff.h"
#include "xil_cache.h"
#include "xplatform_info.h"
#include "sleep.h"

#define SIZEOF_DATA_ARRAY	12288

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FfsSdPolledExample(void);

/************************** Variable Definitions *****************************/
static FIL fil;		/* File object */
static FIL fil2;
static FATFS fatfs;
static char FileName[32] = "test008.bin";
static char *SD_File;
u32 Platform;
static u32 testdata[2048] = {};// __attribute__ ((aligned(32)));
static u8 testpieces[8192] = {};//__attribute__ ((aligned(32)));

#ifdef __ICCARM__
#pragma data_alignment = 32
unsigned int DestinationAddress[10*1024];
unsigned int SourceAddress[10*1024];
#pragma data_alignment = 4
#else
u8 DestinationAddress[10*1024] __attribute__ ((aligned(32)));
u8 SourceAddress[10*1024] __attribute__ ((aligned(32)));
#endif

#define TEST 7

/*****************************************************************************/
/**
*
* Main function to call the SD example.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int main(void)
{
	int Status;

	xil_printf("SD Polled File System Example Test \r\n");

	Status = FfsSdPolledExample();
	if (Status != XST_SUCCESS) {
		xil_printf("SD Polled File System Example Test failed \r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran SD Polled File System Example Test \r\n");

	return XST_SUCCESS;

}

/*****************************************************************************/
/**
*
* File system example using SD driver to write to and read from an SD card
* in polled mode. This example creates a new file on an
* SD card (which is previously formatted with FATFS), write data to the file
* and reads the same data back to verify.
*
* @param	None
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None
*
******************************************************************************/
int FfsSdPolledExample(void)
{
	FRESULT Res;
	UINT NumBytesRead;
	UINT NumBytesWritten;
	u32 BuffCnt;
	u32 FileSize = (8*1024);
	TCHAR *Path = "0:/";
//	int whatishere = 0;
//	int whatishere2 = 0;

	Platform = XGetPlatform_Info();
	if (Platform == XPLAT_ZYNQ_ULTRA_MP) {
		/*
		 * Since 8MB in Emulation Platform taking long time, reduced
		 * file size to 8KB.
		 */
		FileSize = 8*1024;	//need the file size to be the equal to the number of ints we have
	}

	//Test out bitwise things

	/*u8 b1 = 0;
	u8 b2 = 0;
	u8 b3 = 0;
	u8 b4 = 0;
//	u8 bitmaskzero = 0;
//	u8 bitmaskones = 255;
	u8 myintholder = 0;
	//u32 myint = 123;
	int ii = 0;

	u32 numtest = 4279234730U;	//the "U" specifies to the compiler that this is an unsigned 32 bit constant
	for(ii = 0; ii < 4; ii++)
	{
		myintholder = numtest >> (ii * 8);	//this worked!!
		xil_printf("u:%u ",myintholder);
	}
	//alternatively
	b4 = numtest >> (8*0);	//saves the least 8 least significant bits
	b3 = numtest >> (8*1);
	b2 = numtest >> (8*2);
	b1 = numtest >> (8*3);
	xil_printf("\r\nu:%u_%u_%u_%u\r\n",b1,b2,b3,b4);*/

	//end bitwise test
	int flipper = 0;
	BuffCnt = 0;
	while(BuffCnt < 2048)	//test data set = 111111, 121212, 131313, ...
	{
		switch(flipper)
		{
		case 0:								//"	3 1 178	7 "
			testdata[BuffCnt] = 50442759;	//"00000011 00000001 10110010 00000111"
			flipper++;
			break;
		case 1:								//" 3 1 217 124 "
			testdata[BuffCnt] = 50452860;	//"00000011 00000001 11011001 01111100"
			flipper++;
			break;
		case 2:								//" 3 2 0 241 "
			testdata[BuffCnt] = 50462961;	//"00000011 00000010 00000000 11110001"
			flipper = 0;
			break;
		default:
			xil_printf("WTF\r\n");
			break;
		}
		BuffCnt++;
	}

	for(BuffCnt = 0; BuffCnt < 2048; BuffCnt++)	//test data set broken into 4 pieces per datum
	{
		testpieces[BuffCnt * 4 + 0] = testdata[BuffCnt] >> (8 * 3);
		testpieces[BuffCnt * 4 + 1] = testdata[BuffCnt] >> (8 * 2);
		testpieces[BuffCnt * 4 + 2] = testdata[BuffCnt] >> (8 * 1);
		testpieces[BuffCnt * 4 + 3] = testdata[BuffCnt] >> (8 * 0);
	}

	for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){	//example data set
		SourceAddress[BuffCnt] = TEST + BuffCnt;
	}

	/*
	 * Register volume work area, initialize device
	 */
	Res = f_mount(&fatfs, Path, 0);	//can also use "0" for the second variable input

	if (Res != FR_OK) {
		xil_printf("1 %d\r\n",Res);
		return XST_FAILURE;
	}

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
	SD_File = (char *)FileName;

//	Res = f_open(&fil2, "fil2test.txt", FA_OPEN_ALWAYS | FA_WRITE);
//	Res = f_lseek(&fil2, file_size(&fil2));
//	Res = f_write(&fil2, myData256, 256, &NumBytesWritten);//myData256
//	xil_printf("bw: %d",NumBytesWritten);
//	Res = f_close(&fil2);

	Res = f_open(&fil, SD_File, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		xil_printf("2 %d\r\n",Res);
		return XST_FAILURE;
	}
	/*
	 * Check the Time Stamp
	 */
//	Res = f_stat(SD_File, &fno);
//	xil_printf("Timestamp: %u/%02u/%02u, %02u:%02u\r\n", (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31, fno.ftime >> 11, fno.ftime >> 5 & 63);
//	/*
//	 * Change the time stamp on the file.
//	 */
//	//f_utime (const TCHAR* path, const FILINFO* fno);
//	int year = 2017;
//	int month = 2;
//	int mday = 2;
//	int hour = 2;
//	int min = 2;
//	int sec = 2;
//
//	fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
//	fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);
//
//	Res = f_utime(SD_File, &fno);
//	if (Res)
//	{
//		xil_printf("3\r\n");
//		return XST_FAILURE;
//	}
//
//	xil_printf("Timestamp: %u/%02u/%02u, %02u:%02u\r\n", (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31, fno.ftime >> 11, fno.ftime >> 5 & 63);

	/*
	 * Pointer to beginning of file .
	 */
	Res = f_lseek(&fil, file_size(&fil));	//file size should be 0 if it is new; can replace the macro with 0
	if (Res) {
		xil_printf("4 %d\r\n",Res);
		return XST_FAILURE;
	}

	/*
	 * Write data to file.
	 */
	Res = f_write(&fil, (const void*)testpieces, FileSize, &NumBytesWritten);
	if (Res) {
		xil_printf("5 %d\r\n",Res);
		return XST_FAILURE;
	}
	/*
	 * Close file.
	 */
//	Res = f_close(&fil);
//	if (Res) {return XST_FAILURE;}
//
//	/*
//	 * Open the file again
//	 */
//	Res = f_open(&fil, SD_File, FA_READ);
//	if (Res) {
//		xil_printf("22\r\n");
//		return XST_FAILURE;
//	}
	/*
	 * Pointer to beginning of file .
	 */
	Res = f_lseek(&fil, 0);
	if (Res) {
		xil_printf("6 %d\r\n",Res);
		return XST_FAILURE;
	}

	/*
	 * Read data from file.
	 */
	Res = f_read(&fil, (void*)DestinationAddress, FileSize,
			&NumBytesRead);
	if (Res) {
		xil_printf("7 %d\r\n",Res);
		return XST_FAILURE;
	}

	/*
	 * Data verification
	 */
	if(NumBytesRead != NumBytesWritten)
	{
		xil_printf("8 %d\r\n",Res);
		return XST_FAILURE;
	}
//	for(BuffCnt = FileSize; BuffCnt < FileSize; BuffCnt++){	//just look at the final 10k values
//		if(SourceAddress[BuffCnt] != DestinationAddress[BuffCnt])
//		{
//			whatishere = SourceAddress[BuffCnt];
//			whatishere2 = DestinationAddress[BuffCnt];
//			xil_printf("buffcnt: %d\r\n",BuffCnt);
//			xil_printf("9 %d\r\n",Res);
//			return XST_FAILURE;
//		}
//	}
	/*
	 * Close file.
	 */
	Res = f_close(&fil);
	if (Res) {return XST_FAILURE;}


	Res = f_open(&fil2, "fil22.bin", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	Res = f_lseek(&fil2, file_size(&fil2));
	//Res = f_write(&fil2, testdata, 2048, &NumBytesWritten);	//this will only write as much data as we tell it to; an array of 2048 u32's will be 8192 bytes!
	Res = f_write(&fil2, testdata, 8192, &NumBytesWritten);
	xil_printf("\r\nbw: %d\r\n",NumBytesWritten);
	Res = f_close(&fil2);

	return XST_SUCCESS;
}
