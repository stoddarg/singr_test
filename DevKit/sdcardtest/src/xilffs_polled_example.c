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

#include "xtime_l.h"

#define SIZEOF_DATA_ARRAY	12288

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int FfsSdPolledExample(void);

/************************** Variable Definitions *****************************/
static FIL fil;		/* File object */
static FIL fil5;
static FIL fil6;
static FATFS fatfs;
static FILINFO fno;
static char FileName[32] = "test22.bin";
//static char FileName2[32] = "ft.txt";
static char *SD_File;
u32 Platform;

int myData512[512] = {};
unsigned char myData256[256] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//unsigned char myData512[512] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
unsigned int myArray2[SIZEOF_DATA_ARRAY] __attribute__ ((aligned(32)));

#ifdef __ICCARM__
#pragma data_alignment = 32
unsigned int DestinationAddress[10*1024];
unsigned int SourceAddress[10*1024];
#pragma data_alignment = 4
#else
u8 DestinationAddress[10*1024*1024] __attribute__ ((aligned(32)));
u8 SourceAddress[10*1024*1024] __attribute__ ((aligned(32)));
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
	XTime tStart, tEnd;

	XTime_GetTime(&tStart);
	xil_printf("SD Polled File System Example Test \r\n");

	Status = FfsSdPolledExample();
	if (Status != XST_SUCCESS) {
		xil_printf("SD Polled File System Example Test failed \r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran SD Polled File System Example Test \r\n");
	XTime_GetTime(&tEnd);

	xil_printf("Output took %llu clock cycles\r\n", 2*(tEnd - tStart));
	xil_printf("To get time, in us, divide by %llu / 1000000", COUNTS_PER_SECOND);

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
	int it = 0;
	FRESULT Res;
	UINT NumBytesRead;
	UINT NumBytesWritten;
	u32 BuffCnt;
	u32 FileSize = (8*1024);
	//TCHAR *Path = "0:/";

	Platform = XGetPlatform_Info();
	if (Platform == XPLAT_ZYNQ_ULTRA_MP) {
		/*
		 * Since 8MB in Emulation Platform taking long time, reduced
		 * file size to 8KB.
		 */
		FileSize = 8*1024;
	}

	for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
		SourceAddress[BuffCnt] = TEST + BuffCnt;
		if(BuffCnt < 512)
			myData512[BuffCnt] = TEST + BuffCnt;
	}
	for(it = 0; it < 12288; it++)
	{
		myArray2[it] = TEST + it;
	}
/*	for(it = 0; it < 512; it++)	//this works to set the values for ints to increasing values
	{
		if(it < 256)
		{
			myData256[it] = it;
			myData512[it] = it;
		}
		else
			myData512[it] = it;
	} */
	/*
	 * Register volume work area, initialize device
	 */
	Res = f_mount(&fatfs, "0", 0);

	if (Res != FR_OK) {
		return XST_FAILURE;
	}

	/*
	 * Open file with required permissions.
	 * Here - Creating new file with read/write permissions. .
	 * To open file with write permissions, file system should not
	 * be in Read Only mode.
	 */
	SD_File = (char *)FileName;

	Res = f_open(&fil, SD_File, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	if (Res) {
		return XST_FAILURE;
	}
	/*
	 * Check the Time Stamp
	 */
	Res = f_stat(SD_File, &fno);
	xil_printf("Timestamp: %u/%02u/%02u, %02u:%02u\r\n", (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31, fno.ftime >> 11, fno.ftime >> 5 & 63);
	/*
	 * Change the time stamp on the file.
	 */
	//f_utime (const TCHAR* path, const FILINFO* fno);
	int year = 2017;
	int month = 2;
	int mday = 2;
	int hour = 2;
	int min = 2;
	int sec = 2;

	fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
	fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

	Res = f_utime(SD_File, &fno);
	if (Res)
		return XST_FAILURE;

	xil_printf("Timestamp: %u/%02u/%02u, %02u:%02u\r\n", (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31, fno.ftime >> 11, fno.ftime >> 5 & 63);

	/*
	 * Pointer to beginning of file .
	 */
	Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}

	/*
	 * Write data to file.
	 */
	Res = f_write(&fil, (const void*)SourceAddress, FileSize,
			&NumBytesWritten);
	if (Res) {
		return XST_FAILURE;
	}

	/*
	 * Pointer to beginning of file .
	 */
	Res = f_lseek(&fil, 0);
	if (Res) {
		return XST_FAILURE;
	}

	/*
	 * Read data from file.
	 */
	Res = f_read(&fil, (void*)DestinationAddress, FileSize,
			&NumBytesRead);
	if (Res) {
		return XST_FAILURE;
	}

	/*
	 * Data verification
	 */
	if(NumBytesRead != NumBytesWritten)
		return XST_FAILURE;

	for(BuffCnt = 0; BuffCnt < FileSize; BuffCnt++){
		if(SourceAddress[BuffCnt] != DestinationAddress[BuffCnt]){
			return XST_FAILURE;
		}
	}

	fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
	fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

	Res = f_utime(SD_File, &fno);
	if (Res)
		return XST_FAILURE;

	xil_printf("Timestamp: %u/%02u/%02u, %02u:%02u\r\n", (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31, fno.ftime >> 11, fno.ftime >> 5 & 63);
	/*
	 * Close file.
	 */
	Res = f_close(&fil);
	if (Res) {return XST_FAILURE;}

	//Write progressively larger buffers until one of them writes properly
	//256 bytes
	Res = f_open(&fil5, "test512.bin", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	if (Res) {return XST_FAILURE;}

	Res = f_write(&fil5, (const void *)myData512, 512, &NumBytesWritten);
	if (Res) {return XST_FAILURE;}

	Res = f_close(&fil5);
	if (Res) {return XST_FAILURE;}

	int index = 0;
/*	unsigned int * myArray;
	myArray = (unsigned int *)calloc(SIZEOF_DATA_ARRAY, sizeof(unsigned int));

	for(index = 0; index < SIZEOF_DATA_ARRAY; index++)
	{
		if((index % 2) == 0)
			myArray[index] = 3;
		else
			myArray[index] = 4;
	} */

	Res = f_open(&fil6, "lrgtest2.bin", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);	//removed the (const void *)
	Res = f_lseek(&fil6, SIZEOF_DATA_ARRAY);
	Res = f_lseek(&fil6, 0);

//	for(index = 0; index < 24; index++)	//write 24 512 byte blocks
//	{
		Res = f_write(&fil6, (const void *)myArray2, SIZEOF_DATA_ARRAY, &NumBytesWritten);
//		Res = f_sync(&fil6);
//	}
	Res = f_close(&fil6);

	//free(myArray);
	//myArray = NULL;

	return XST_SUCCESS;
}
