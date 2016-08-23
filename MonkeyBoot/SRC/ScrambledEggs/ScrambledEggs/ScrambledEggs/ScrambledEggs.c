// ScrambledEggs.cpp : Defines the entry point for the console application.
//

#include "string.h"
#include "stdio.h"
#include "ihex.h"


char Command[256];
char InputHexFile[256];
char OutputHexFile[256];
char HEX_LineBufferIn[IHEX_LINE_BUFFER_SIZE];
char HEX_LineBufferOut[IHEX_LINE_BUFFER_SIZE];
uint8_t TmpArray[IHEX_LINE_BUFFER_SIZE];

uint32_t MLS = 0;
uint32_t Ketchup = 0;

uint32_t NextMLS(uint32_t MLS)
{
	uint8_t A, B, C;

	A = (MLS & (1 << 28))>>28;
	B = (MLS & (1 << 26))>>26;

	C = (~(A ^ B)) & 0x01;

	MLS = MLS << 1;
	MLS |= C;

	return MLS;
}


int main(int argc, char * argv[])
{
	FILE * InputFile;
	FILE * OutputFile;
	char *Tmp;
	iHEX_LineInfo LI;
	uint32_t i;

	if (argc != 5)
	{
		printf("Usage:\r\n");
		printf("ScrambledEggs (1.0) needs 4 arguments.  \r\n");
		printf("Arg1 :   The Command:   S for Scramble, U for Unscramble or P for pass through\r\n");
		printf("Arg2 :   input hex file\r\n");
		printf("Arg3 :   output hex file\r\n");
		printf("Arg4:    The amount of Ketchup on your eggs (postive integer)\r\n");
	}
	else
	{
		strncpy(Command, argv[1], sizeof(Command));

		if (Command[0] != 'S' && Command[0] != 'U' && Command[0] != 'P')
		{
			printf("I don't understand the %s command\r\n", Command);
			return -1;
		}

		sscanf(argv[4], "%i", &MLS);
		Ketchup = MLS;

		strncpy(InputHexFile, argv[2], sizeof(InputHexFile));
		strncpy(OutputHexFile, argv[3], sizeof(OutputHexFile));

		if (strcmp(InputHexFile, OutputHexFile) == 0)
		{
			printf("Input and Output files cannot be the same!\r\n");
			return -1;
		}

		OutputFile = fopen(OutputHexFile, "w+");
		
		if (OutputFile == NULL)
		{
			printf("Could not open %s for writing.\r\n", OutputHexFile);
			return -1;
		}
		
		InputFile = fopen(InputHexFile, "r");

		if (InputFile == NULL)
		{
			printf("Could not open %s for reading\r\n", InputHexFile);
			return -1;
		}


		printf("Scrambling the eggies.... %s to %s\r\n", InputHexFile, OutputHexFile);

		LI.LineNumber = 0;

		while (!feof(InputFile))
		{
			Tmp = fgets(HEX_LineBufferIn, IHEX_LINE_BUFFER_SIZE, InputFile);

			if (Tmp == NULL)
				goto Exit;
		
			StripRightWhiteSpace(HEX_LineBufferIn);

			LI.LineNumber++;


			if (strlen(HEX_LineBufferIn) == 0)
				continue;

			if (ReadIHEX_32_Line(HEX_LineBufferIn, &LI) != IHEX_LINE_GOOD)
			{
				printf("Line %d is invalid...   Exiting!\r\n", LI.LineNumber);
				goto Exit;
				break;
			}

			switch (Command[0])
			{
				case 'S':

					
				
					if (LI.RecordType == IHEX_RECORD_TYPE_00)
					{
						for (i = 0; i < LI.ByteCount; i++)
						{
							LI.LineData[i] ^= (uint8_t)(MLS & 0xFF);
							MLS = NextMLS(MLS);
						}

						for (i = 0; i < LI.ByteCount; i++)
						{
							TmpArray[i] = LI.LineData[LI.ByteCount - i - 1];
						}

						
						
						if ((LI.ByteCount & 0x01) == 0 && (LI.ByteCount>2))
						{
							if ((LI.DataStartAddress & (1 << 4) || (LI.DataStartAddress & (1 << 7))))
							{

								for (i = 0; i < LI.ByteCount >> 1; i++)
								{
									TmpArray[0] = LI.LineData[i*2];
									TmpArray[1] = LI.LineData[(i * 2) + 1];

									LI.LineData[i * 2] = TmpArray[1];
									LI.LineData[(i*2) + 1] = TmpArray[0];
								}
							}
						}
					}

					

					//In the pass through case,  we just dump the output for debugging
					MakeIHEX_32_Line(HEX_LineBufferOut, &LI);
					fprintf(OutputFile, "%s\n", HEX_LineBufferOut);

					break;

				case 'U':
					
					if (LI.RecordType == IHEX_RECORD_TYPE_00)
					{
						if ((LI.ByteCount & 0x01) == 0 && (LI.ByteCount>2))
						{
							if ((LI.DataStartAddress & (1 << 4) || (LI.DataStartAddress & (1 << 7))))
							{

								for (i = 0; i < LI.ByteCount >> 1; i++)
								{
									TmpArray[0] = LI.LineData[i * 2];
									TmpArray[1] = LI.LineData[(i * 2) + 1];

									LI.LineData[i * 2] = TmpArray[1];
									LI.LineData[(i*2) + 1] = TmpArray[0];
								}
							}
						}

						for (i = 0; i < LI.ByteCount; i++)
						{
							TmpArray[i] = LI.LineData[LI.ByteCount - i - 1];
						}

						for (i = 0; i < LI.ByteCount; i++)
						{
							LI.LineData[LI.ByteCount - i - 1] = TmpArray[i];
						}

						for (i = 0; i < LI.ByteCount; i++)
						{
							LI.LineData[i] ^= (uint8_t)(MLS & 0xFF);
							MLS = NextMLS(MLS);
						}
					}

					//In the pass through case,  we just dump the output for debugging
					MakeIHEX_32_Line(HEX_LineBufferOut, &LI);
					fprintf(OutputFile, "%s\n", HEX_LineBufferOut);

					break;
				case 'P':

					//In the pass through case,  we just dump the output for debugging
					MakeIHEX_32_Line(HEX_LineBufferOut, &LI);
					printf("Line %d\r\n", LI.LineNumber);
					printf(" Input -->%s\r\n", HEX_LineBufferIn);
					printf(" Output-->%s\r\n", HEX_LineBufferOut);

					if (strcmp(HEX_LineBufferIn, HEX_LineBufferOut) != 0)
					{
						printf("These lines do not match!\r\n");
						goto Exit;
					}
					
					fprintf(OutputFile,"%s\n", HEX_LineBufferOut);
					
					break;
			}

	

		}

		printf("The eggs have been scrambled with plenty of ketchup on top!\r\n");
		Exit:
		fclose(InputFile);
		fclose(OutputFile);
	}
	return 0;
}

