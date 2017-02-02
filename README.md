# z80BASICextractor
The BASIC Extractor analyzes and unpacks .z80 snapshots created by Spectrum emulators and creates listings of BASIC programs inside of them. It is also capable of extracting variables from the VARS section of memory and writing them in a separate file.

##Usage:
Use one of the Batch files to process all of .z80 files in a folder. "run.bat" will generate both listings and variables.

	usage: z80extractor [input file name or path] [-o output_file] [-stdout] [-list] [-vars]
	
	default: program output files have the same name as input with extension changed to '.txt' for listings and '-vars.txt' for variables.
	-o      output program to the specified file (it will be overwritten if exists!), variables will use the name before extension and add '-vars.txt' to it
	-stdout output to screen instead to a file
	-list   output listings only
	-vars   output variables only
