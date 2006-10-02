################################################################################
#
# A script to recursively list the last modified time, product and file version
# numbers of files under a given path.
#
# -p <path>   The path to search under. The default is the CWD.
# -f <filter> The filename filter. The default is "*.*".
# -r <depth>  The number of sub-folders to recurse into. The default is 0.
#
################################################################################

import sys
import os
import getopt
import win32net
import win32api
import win32file
import win32netcon
import time

#
# Get the product version number for a file.
# NB: This gets the product version 'string' not the numerical value.
#

def GetProductVersionNumber(strFileName):
    strVersion = ""

    try:
        # Get the version number.
        strVersion = win32api.GetFileVersionInfo(strFileName, "\\StringFileInfo\\040904b0\\ProductVersion")

        # Trap (null) values.
        if (strVersion == None):
            strVersion = ""

        strVersion = strVersion.replace(", ", ".")
        
    except win32api.error:
        pass

    return strVersion    


#
# Get the file version number for a file.
# NB: This gets the file version 'string' not the numerical value.
#

def GetFileVersionNumber(strFileName):
    strVersion = ""

    try:
        # Get the version number.
        strVersion = win32api.GetFileVersionInfo(strFileName, "\\StringFileInfo\\040904b0\\FileVersion")

        # Trap (null) values.
        if (strVersion == None):
            strVersion = ""
        
        strVersion = strVersion.replace(", ", ".")
        
    except win32api.error:
        pass

    return strVersion    


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
# Recusively find the file from the base path.
# NB: FindFiles() returns a tuple of which value '8' is the filename.
#

def DoFindFiles(strBasePath, strFilter, nDepth, bFirstCall):

    lstFiles = []

    try:
        # Look in this folder first.
        lstThis = win32api.FindFiles(strBasePath + strFilter)

        # Append fully-qualified paths to the list.
        for i in lstThis:
            strFileName = i[8]

            # Ignore 'parent' and 'this' folders.
            if (strFileName <> ".") and (strFileName <> ".."):
                lstFiles.append(strBasePath + strFileName)

        # Recurse into sub-folders?
        if nDepth > 0:
            lstDirs = win32api.FindFiles(strBasePath + "*.*")

            # For all sub-folders...
            for i in lstDirs:
                if (i[0] & win32file.FILE_ATTRIBUTE_DIRECTORY) and (i[8] != ".") and (i[8] != ".."):
                    lstAll = DoFindFiles(strBasePath + i[8] + "\\", strFilter, nDepth-1, False)
                    lstFiles.extend(lstAll)
                    
    except win32net.error:
        if bFirstCall:
            print strBasePath + ": " + sys.exc_value[2]

    return lstFiles


#
# Find all files that match the file mask (or *.*).
# The search is limited to 'nDepth' folders deep.
#

def FindFiles(strBasePath, strFilter, strDepth):

    lstPaths = []
    nDepth   = int (strDepth, 10)

    # Must be 0..n
    if nDepth < 0:
        return lstPaths

    # Search the path
    lstFiles = DoFindFiles(strBasePath, strFilter, nDepth, True)

    # List file details
    for f in lstFiles:
        strFile    = str(f)
        strTime    = GetModifiedTime(f)
        strProdVer = GetProductVersionNumber(f)
        strFileVer = GetFileVersionNumber(f)

        # Make path relative
        strFile = strFile.replace(strBasePath, "")

        print strFile + "\t" + strTime + "\t" + strProdVer + "\t" + strFileVer


#
# Display usage.
#

def Usage():
    print 'Usage: ListFileVersion [-p path] [-f filter] [-r search-depth]'
    print ''
    print ' <path>         is the base path to search, e.g. "C:\Program Files"'
    print ' <filter>       is any FindFirstFile() compatible filter, e.g. "msvc?*.d??"'
    print ' <search-depth> is the number of sub-folders to recurse into'
    print ''
    print ' Defaults: -p ".\" -f "*.*" -r 0'


#
# Main function.
#

def main():

    # Set program defaults
    strBasePath = "."
    strFilter   = "*.*"
    strDepth    = "0"

    # Parse command line
    try:
        astrOpts, astrArgs = getopt.getopt(sys.argv[1:], "p:f:r:", ["filter=", "recurse="])
    except getopt.GetoptError:
        Usage()
        sys.exit(2)

    # Process command line
    for strOpt, strArg in astrOpts:
        # Is a request for help?
        if strOpt in ("-?", "/?", "-h", "/h"):
            Usage()
            sys.exit(2)

        # Is the path to search?
        if strOpt in ("-p", "/p"):
            strBasePath = strArg

        # Is the file filter?
        if strOpt in ("-f", "/f"):
            strFilter = strArg

        # Is the search-depth?
        if strOpt in ("-r", "/r"):
            strDepth = strArg

    # Check path exists, to allow for improper escaping of spaces
    if (not os.path.isdir(strBasePath)):
        print 'Invalid base path: ' + strBasePath
        sys.exit(2)

    # Ensure the path is terminated
    if (not strBasePath.endswith("\\")):
        strBasePath = strBasePath + "\\"

    # List files...
    FindFiles(strBasePath, strFilter, strDepth)


#
# Entry point.
#

if __name__ == '__main__':
    main()
