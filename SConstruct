env = Environment()
env.Append(LINKFLAGS = '-lpthread', CPPPATH='src')
env.Program('radix_join', ['main.cpp', Glob('src/*.cpp')])
