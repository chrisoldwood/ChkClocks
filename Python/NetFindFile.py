import sys
import win32net
import win32api
import win32file
import win32netcon
import time
import threading

#
# Find all machines in the given domain.
#

def FindMachines(strDomain):
    lstMachines = []

    nRetCode = 1
    nDetail  = 100
    eType    = win32netcon.SV_TYPE_ALL
    nLength  = win32netcon.MAX_PREFERRED_LENGTH
    
    try:
        # Enumerate domain.
        while nRetCode:
            (lstTemp,nTotal,nResult)=win32net.NetServerEnum('', nDetail, eType, strDomain, nRetCode, nLength)

            lstMachines.extend(lstTemp)
            nRetCode = nResult
            
    except win32net.error:
        print strDomain + ": " + sys.exc_value[2]

    lstResult = []

    # Build machine-name list
    for i in lstMachines:
        lstResult.append(str(i['name']))

    return lstResult

#
# Get the last modified time for a file.
#

def GetModifiedTime(strFileName):
    strTime = ""
    
    try:
        tAttribs = win32file.GetFileAttributesEx(strFileName)
        strTime  = tAttribs[3].Format("%d/%m/%Y %H:%M:%S")

    except win32file.error:
        pass

    return strTime    


#
# Get the product version number for a file.
#

def GetVersionInfo(strFileName):
    strVersion = ""

    try:
        strExt = strFileName[-4:]

        if strExt in (".exe", ".dll", ".ocx"):
            strVersion = win32api.GetFileVersionInfo(strFileName, "\\StringFileInfo\\040904b0\\ProductVersion")
            strVersion = strVersion.replace(", ", ".")
        
    except win32api.error:
        pass

    return strVersion    

#
# Recusively find the file from the base path.
#

def DoFindFile(strBasePath, strFileName, nDepth, bFirstCall):

    lstFiles = []

    try:
        # Look in this folder first.
        lstThis = win32api.FindFiles(strBasePath + strFileName)

        for i in lstThis:
            lstFiles.append(strBasePath + i[8])

        # Recurse into sub-folders?
        if nDepth > 0:
            lstDirs = win32api.FindFiles(strBasePath + "*.*")

            # For all sub-folders...
            for i in lstDirs:
                if (i[0] & win32file.FILE_ATTRIBUTE_DIRECTORY) and (i[8] != ".") and (i[8] != ".."):
                    lstAll = DoFindFile(strBasePath + i[8] + "\\", strFileName, nDepth-1, False)
                    lstFiles.extend(lstAll)
                    
    except win32net.error:
        if bFirstCall:
            print strBasePath + ": " + sys.exc_value[2]

    return lstFiles

#
# Find the given file on the given machine.
# The search is limited to 'nDepth' folders deep.
#

def NetFindFile(strMachine, strFileName, nDepth):

    lstPaths = []
    nDepth   = int (nDepth, 10)

    # Must be 0..n
    if nDepth < 0:
        return lstPaths

    strSearchPath = "\\\\" + strMachine + "\\c$\\"

    # Search the machine
    lstFiles = DoFindFile(strSearchPath, strFileName, nDepth, True)

    for f in lstFiles:
        strTime    = GetModifiedTime(f)
        strVersion = GetVersionInfo(f)

        if not strVersion:
            print f + "\t" + GetModifiedTime(f)
        else:
            print f + "\t" + GetModifiedTime(f) + "\t" + strVersion


#
# Multi-threading control class.
#

class NetFindThread(threading.Thread):

    def __init__(self, strMachine, strFileName, nDepth):
        threading.Thread.__init__(self)

        self.strMachine  = strMachine
        self.strFileName = strFileName
        self.nDepth      = nDepth

        self.setName(strMachine)

    def run(self):
        NetFindFile(self.strMachine, self.strFileName, self.nDepth)


#
# Display usage.
#

def Usage():
    print 'Usage: NetFindFile [-c computer|-d domain] [-f filter] [-r search-depth]'
    print ' "filter" can be a filename (e.g. cmd.exe) or a filter (e.g. cmd*.*)'
    print ' "search-depth" is the number of sub-folders to recurse into'


#
# Main function.
#

def main():

    # Expecting at least 6 arguments.
    if len(sys.argv) < 7:
        Usage()
        sys.exit(2)

    # Get computer or domain name.
    if (sys.argv[1] == "-d"):
        strType   = 'd'
        strTarget = sys.argv[2]
    elif (sys.argv[1] == "-c"):
        strType   = 'c'
        strTarget = sys.argv[2]
    else:
        Usage()
        sys.exit(2)

    # Get filter.    
    if (sys.argv[3] == "-f"):
        strFilter = sys.argv[4]
    else:
        Usage()
        sys.exit(2)

    # Get search-depth.
    if (sys.argv[5] == "-r"):
        strDepth = sys.argv[6]
    else:
        Usage()
        sys.exit(2)

    # Build list of target machines.
    lstMachines = []

    if (strType == 'd'):
        lstMachines = FindMachines(strTarget)
    else:
        lstMachines.append(strTarget)

    lstThreads = []

    # Create and start threads for each machine
    for m in lstMachines:
        thrdFinder = NetFindThread(m, strFilter, strDepth)
        thrdFinder.start()

        lstThreads.append(thrdFinder)

    # Wait for all threads to finish
#    while threading.activeCount() > 1:
#        sleep(1)

    for t in lstThreads:
        t.join()
        
#
# Entry point.
#

if __name__ == '__main__':
    main()
