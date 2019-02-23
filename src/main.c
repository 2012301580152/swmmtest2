//-----------------------------------------------------------------------------
//   main.c
//
//   Project:  EPA SWMM5
//   Version:  5.1
//   Date:     05/10/2018
//   Author:   L. Rossman

//   Main stub for the command line version of EPA SWMM 5.1
//   to be run with swmm5.dll.


#include <stdio.h>
#include <time.h>
//#include "swmm5.h"
#include "swmm5_iface.h"
#include <stdlib.h>

int test(char *argv)
{
	int i, r;
	float x, y, z;
	char rptfile[] = "tmp.rpt";
	char outfile[] = "tmp.out";

	// 检查是否提供了SWMMH输入文件名
	//if (argc < 2)
	//{
	//	printf("\n没有提供文件名。\n");
	//	return 0;
	//}

	// 进行SWMMH分析
	//r = RunSwmmDll(argv, rptfile, outfile);
	/*if (r > 0)
	{
		printf("\nSWMMH执行未成功；错误代码 = %d\n", r);
	}*/
	//else
	{
		// 打开输出文件作为SWMMH输出文件
		r = OpenSwmmOutFile(outfile);
		if (r == 1)
		{
			printf("\nSWMMH出数文件中不合理结果。\n");
		}
		else if (r == 2)
		{
			printf("\n文件不是SWMMH输出文件。\n");
		}
		else
		{
			printf("\n时段    总降雨量    总径流   总出流");
			printf("\n====================================");
			for (i = 1; i <= SWMM_Nperiods; i++)
			{
				GetSwmmResult(3, 0, 1, i, &x);
				GetSwmmResult(3, 0, 4, i, &y);
				GetSwmmResult(3, 0, 11, i, &z);
				printf("\n%6d  %8.2f  %8.2f  %8.2f", i, x, y, z);
			}
			CloseSwmmOutFile();
		}
	}
	/*remove(rptfile);
	remove(outfile);*/
	return 0;
}

int  main(int argc, char *argv[])
//
//  Input:   argc = number of command line arguments
//           argv = array of command line arguments
//  Output:  returns error status
//  Purpose: runs the command line version of EPA SWMM 5.1.
//
//  Command line is: swmm5 f1  f2  f3
//  where f1 = name of input file, f2 = name of report file, and
//  f3 = name of binary output file if saved (or blank if not saved).
//
{
    char *inputFile;
    char *reportFile;
    char *binaryFile;
    char *arg1;
    char blank[] = "";
    int  version, vMajor, vMinor, vRelease;
    char errMsg[128];
    int  msgLen = 127;
    time_t start;
    double runTime;
    
    version = swmm_getVersion();
    vMajor = version / 10000;
    vMinor = (version - 10000 * vMajor) / 1000;
    vRelease = (version - 10000 * vMajor - 1000 * vMinor);
    start = time(0);

	argc = 4;	

    // --- check for proper number of command line arguments
    if (argc == 1)
    {
        printf("\nNot Enough Arguments (See Help --help)\n\n");
    }
    else if (argc == 2)
    {
        // --- extract first argument
        arg1 = argv[1];

        if (strcmp(arg1, "--help") == 0 || strcmp(arg1, "-h") == 0)
        {
            // Help
            printf("\n\nSTORMWATER MANAGEMENT MODEL (SWMM5) HELP\n\n");
            printf("COMMANDS:\n");
            printf("\t--help (-h)       Help Docs\n");
            printf("\t--version (-v)    Build Version\n");
            printf("\nRUNNING A SIMULATION:\n");
            printf("\t swmm5 <input file> <report file> <output file>\n\n");
        }
        else if (strcmp(arg1, "--version") == 0 || strcmp(arg1, "-v") == 0)
        {
            // Output version number
            printf("\n%d.%d.%0d\n\n", vMajor, vMinor, vRelease);
        }
        else
        {
            printf("\nUnknown Argument (See Help --help)\n\n");
        }
    }
    else
    {
        // --- extract file names from command line arguments
		/*argv[1] = "input.inp";
		argv[2] = "tmp.rpt";
		argv[3] = "tmp.out";*/
        inputFile = "lxswmm.inp";;
        reportFile = "tmp.rpt";
        if (argc > 3) binaryFile = "tmp.out";
        else          binaryFile = blank;
        printf("\n... EPA-SWMM %d.%d (Build %d.%d.%0d)\n", vMajor, vMinor,
            vMajor, vMinor, vRelease);

        // --- run SWMM
        swmm_run(inputFile, reportFile, binaryFile);

        // Display closing status on console
        runTime = difftime(time(0), start);
        printf("\n\n... EPA-SWMM completed in %.2f seconds.", runTime);
        if      ( swmm_getError(errMsg, msgLen) > 0 ) printf(" There are errors.\n");
        else if ( swmm_getWarnings() > 0 ) printf(" There are warnings.\n");
        else printf("\n");

		test(inputFile);


    }

// --- Use the code below if you need to keep the console window visible
/* 
    printf("    Press Enter to continue...");
    getchar();
*/
	printf("\n\n");
	system("pause");
    return 0;
}