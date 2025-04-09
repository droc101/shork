import os
import sys
import shutil
import util

# Converters
import Converter.OBJConverter

if (len(sys.argv) != 3):
	print("Usage: python genassets.py <input_path> <output_path>")
	sys.exit(1)

util.input_path = sys.argv[1]
util.output_path = sys.argv[2]

obj = Converter.OBJConverter.ConvertOBJ(util.input_path)
# Write the data to the output path
with open(util.output_path, 'wb') as f:
	f.write(bytearray(obj))

