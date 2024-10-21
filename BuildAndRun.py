import os

os.system('mkdir build')
os.chdir('./build')
os.system('cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. && make -j4 && ./BookDiff')

