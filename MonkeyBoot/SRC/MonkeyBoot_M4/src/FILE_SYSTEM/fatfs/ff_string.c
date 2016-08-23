/*
 * ff_string.c
 *
 *  Created on: May 21, 2014
 *      Author: ehughes
 */


char *FRESULT_String[] = { 	"OK",				/* (0) Succeeded */
                            "DISK_ERR",			/* (1) A hard error occurred in the low level disk I/O layer */
                            "INT_ERR",				/* (2) Assertion failed */
                            "NOT_READY",			/* (3) The physical drive cannot work */
                            "NO_FILE",				/* (4) Could not find the file */
                            "NO_PATH",				/* (5) Could not find the path */
                            "INVALID_NAME",		/* (6) The path name format is invalid */
                            "DENIED",				/* (7) Access denied due to prohibited access or directory full */
                            "EXIST",				/* (8) Access denied due to prohibited access */
                            "INVALID_OBJECT",		/* (9) The file/directory object is invalid */
                            "WRITE_PROTECTED",		/* (10) The physical drive is write protected */
                            "INVALID_DRIVE",		/* (11) The logical drive number is invalid */
                            "NOT_ENABLED",			/* (12) The volume has no work area */
                            "NO_FILESYSTEM",		/* (13) There is no valid FAT volume */
                            "MKFS_ABORTED",			/* (14) The f_mkfs() aborted due to any parameter error */
                            "TIMEOUT",				/* (15) Could not get a grant to access the volume within defined period */
                            "LOCKED",				/* (16) The operation is rejected according to the file sharing policy */
                            "NOT_ENOUGH_CORE",		/* (17) LFN working buffer could not be allocated */
                            "TOO_MANY_OPEN_FILES",	/* (18) Number of open files > _FS_SHARE */
                            "INVALID_PARAMETER"		/* (19) Given parameter is invalid */

                         };
