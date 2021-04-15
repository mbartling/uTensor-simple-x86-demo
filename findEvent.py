import numpy as np
import glob
import re
import sys

from collections import defaultdict
from pprint import pprint


def mHash_fnv1a(mStr, toUint16=True):
  np.seterr(over='ignore')
  val_32_const = np.uint32(0x811c9dc5)
  prime_32_const = np.uint32(0x1000193)
  value = val_32_const
  for c in mStr:
      value = (value ^ np.uint32(ord(c))) * prime_32_const
  if toUint16:
    value = np.uint16(value)
  return value

def get_target_files():
  x = glob.glob('**/*.[ch]??', recursive=True)
  x = filter(lambda y: "CMake" not in y, x)
  x = filter(lambda y: "TESTS" not in y, x)
  x = filter(lambda y: "extern" not in y, x)
  return x

def get_event_map():
  tgts = get_target_files()
  event_names = []
  event_map = defaultdict(list)
  for f in tgts:
    with open(f) as fp:
      for line in fp:
        m = re.match("\s*DECLARE_\w+\((\w+)\)", line)
        if m:
          #print(m)
          event_names.append(m.group(1))

  for evt in event_names:
    x = mHash_fnv1a(evt)
    event_map[x].append(evt)
  pprint(event_map)
  return event_map

if __name__ == "__main__":
  x = get_event_map()
  evtId = int(sys.argv[1])
  print(x[evtId])
