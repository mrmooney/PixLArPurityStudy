# pub_dbi package include
import datetime, json, time
import pdb
import subprocess
import copy
import samweb_cli
import sys

def is_intstring(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def _chunk(iterable, chunksize):
   """ Helper to divide an iterable into chunks of a given size """
   iterator = iter(iterable)
   from itertools import islice
   while True:
       l = list(islice(iterator, chunksize))
       if l: yield l
       else: return

try:
   samweb = samweb_cli.SAMWebClient(experiment="lariat")
except:
   raise Exception('Not able to open up the SAMWeb Connection')

query = "run_number = -1"

if is_intstring(sys.argv[1]):
    query = "run_number = %d" % (int(sys.argv[1]))

try:
   list_o_files=samweb.listFiles(query)
except:
   raise Exception('Unable to get list of files for this query:' + query)

for filenames in _chunk(list_o_files, 10):
   mdlist = samweb.getMultipleMetadata(filenames)
   for md in mdlist:
      in_file = md['file_name']
      try:
           locations=samweb.locateFile(in_file)
           location=[ loc['location'] for loc in locations if loc['full_path'].startswith('enstore') ][0]
      except:
           raise Exception('No location available for file: '+in_file)

      file_full_path_tmp=((location.split(":"))[1].split("("))[0]+"/"+in_file
      print file_full_path_tmp
