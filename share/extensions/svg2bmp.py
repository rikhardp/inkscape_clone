import os, sys

if __name__ == '__main__':	
	os.system('convert ' + sys.argv[1] + ' tmp.bmp')
	inf = open('tmp.bmp', 'r')
	print inf.read()
	os.remove('tmp.bmp')
