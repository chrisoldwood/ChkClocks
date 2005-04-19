import sys
import win32net
import win32netcon

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
        while nRetCode:
            (lstTemp,nTotal,nResult)=win32net.NetServerEnum('', nDetail, eType, strDomain, nRetCode, nLength)

            lstMachines.extend(lstTemp)
            nRetCode = nResult
            
    except win32net.error:
        print strDomain + ": " + sys.exc_value[2]

    lstResult = []

    for i in lstMachines:
        lstResult.append(str(i['name']))

    return lstResult

#
# Display usage.
#

def Usage():
    print 'Usage: ListDomain domain'

#
# Main function.
#

def main():

    if not sys.argv[1:]:
        Usage()
        sys.exit(2)

    lstMachines = FindMachines(sys.argv[1])

    for i in lstMachines:
        print i
   
#
# Entry point.
#

if __name__ == '__main__':
    main()
