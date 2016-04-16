Usage:

This tool is INI file based:
Next to the .exe an INI file is expected with the following section:

[Emulators]
;; root directionary to look for emulators
RootDirectory="<root search dir>"

If no INI is set or the entry is missing the .exe folder is used as root path (not tested, probably won't work)

The tool then scans the RootDirectory and the folders inside that folder (recursively) for .exe files (of supposed emulators)
Next to the Emulator .exe file an INI file is expected with the following entries:

[EMULATOR]
;; optional if not set the file name of the .exe is used
DisplayName=<Name in UI> 
;; Command line arguments of the emulator <rom> is replaced by the full path of the selected ROM file
CommandLineArgs=<rom>
;; folder of the Roms (relative to .exe) (note: folders above the ROM folder are not tested and may not work)
ROMFolder=ROMS
;; file extension of a ROM
ROMExtension=zip

with this information the ROMs for the emulator are searched and added to the UI. 

Controller Input (XBox Controller):
A,B -> Move Input to ROM list (if currently in Emulator list) or start Emulator with ROM (if in ROM list)
X,Y -> Move Input to Emulator list

L2+R2 -> closes a running Emulator
L2+R2+A, L2+R2+B -> close the tool (if no emulator is running)
