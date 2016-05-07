bin_PROGRAMS = server
server_SOURCES = tair_serv.cpp tair_serv_server.skeleton.cpp tair_service_constants.cpp tair_service_types.cpp  
INCLUDES =-I/usr/local/include/thrift -I/root/tairconfigserver/include -I/lib/tbsys/include/tbnet -I/lib/tbsys/include/tbsys
LDFLAGS=-L/usr/local/lib -L/root/tairconfigserver/lib -L/lib/tbsys/lib
LDADD = -lthriftnb -levent -lthrift -lrt -ltairclientapi -ltbsys -ltbnet