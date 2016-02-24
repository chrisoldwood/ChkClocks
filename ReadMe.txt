Check Clocks v1.2
=================

Introduction
------------

This is a utility that scans all the computers on the network and checks the
time against the machine that the utility is running on. It uses the API
NetRemoteTOD() so may not return the time for 9x or non-windows based machines.

Releases
--------

Stable releases are formally packaged and made available from my Win32 tools page:
http://www.chrisoldwood.com/win32.htm

The latest code is available from my GitHub repo:
https://github.com/chrisoldwood/ChkClocks

Installation
------------

Run Setup.exe or copy the file ChkClocks.exe to a folder.

Uninstallation
--------------

Delete the files ChkClocks.*

Documentation
-------------

To run a check use the "File | Check" command. You can abort a check during
the actual checking process (not the enumeration of domains) but it may take
a few seconds as it waits for all the threads to finish.

If there are machines which are not automatically discovered (such as in a DMZ)
but can be reached you can add them to the "include" list through the
"Options | Scanning" dialog. Alternatively, if there are machines or domains
which should never be checked add them to the "exclude" list. This can also
be done by right-clicking on an item in the report view. Machine names in
these lists must start with a "\\". The "Auto-Exclude Empty Domains" option
will automatically add any domains which appear to contain no machines to
the exclude list to avoid enumerating them in future.

The "Options | Reporting" dialog allows to set which machines you want to
display and what format to use for the time difference. The tolerance
setting treats all machines within this value as being "correct" and so wont be
displayed if the "Hide If Correct" option is ticked.

The "Report | ..." menu items allow you to send a copy of the report to the
clipboard, save to a file or send it to a printer.

Development
-----------

See DevNotes.txt

Contact Details
---------------

Email: gort@cix.co.uk
Web:   http://www.chrisoldwood.com

Chris Oldwood 
22nd November 2004
