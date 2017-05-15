import json
import inspect, os
from pprint import pprint

#class to find the length of list
class slist(list):
    @property
    def length(self):
        return len(self)

dir=os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))#to get the directory of this file
with open(dir+"/release/symbols.json") as data_file:    
    data = json.load(data_file)
    


l=slist(data)
print(data[l.length-1]['tag'])#data is list of dictionary, accessing the tag of the last element i.e most recent version
