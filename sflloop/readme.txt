this program batch-edits sfl files which contain only one range

info.csv is a plain text file which should contain line entries formated like so:
filename.sfl,100,150
this will set the range in filename.sfl to start at sample 100 and have length of 150-100=50

you can run the program in the extract mode like so:
.\sflloop.exe extract
this will read all the sfl files in the current directory, and output range info into out.csv