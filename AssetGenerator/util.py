import gzip
import struct

input_path = ""
output_path = ""

# Convert an integer to bytes little-endian, 4 bytes
def IntToBytes(i):
	return struct.pack("<I", i)

# Create a C-style string with a maximum length
def CString(s, maxlen):
	if (len(s) > maxlen - 1):
		raise ValueError("String is too long")
	return s.ljust(maxlen, '\0')