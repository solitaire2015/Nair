// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "tair_serv.h"
#include "tair_client_capi.hpp"
#include <concurrency/ThreadManager.h> 
#include <concurrency/PosixThreadFactory.h> 
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <server/TNonblockingServer.h>    

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace::apache::thrift::concurrency;     

using  std::string;

using  std::wstring;

using boost::shared_ptr;

using namespace  ::tair_thrift;

class tair_servHandler : virtual public tair_servIf {
public:
	tair_servHandler() {
		handler = tair_init();

	}

	int32_t Tair_begin(const std::string& master_addr, const std::string& slave_addr, const std::string& group_name) {
		master_address = master_addr.c_str();
		slave_address = slave_addr.c_str();
		group = group_name.c_str();
		tair_begin(handler, master_address, slave_address, group);
	}

	void Tair_set_loglevel(const std::string& level) {
		tair_level = level.c_str();
		tair_set_loglevel(tair_level);
	}

	int32_t Tair_put(const int32_t area, const Tair_data_pair& key, const Tair_data_pair& data, const int32_t expire, const int32_t version) {
		return tair_put(handler, area, key.c_str(), data.c_str(), expire, version);
	}

	int32_t Tair_get(const int32_t area, const Tair_data_pair& key, const Tair_data_pair& data) {
		return tair_get(handler, area, key.c_str(), data.c_str());
	}

	int32_t Tair_remove(const int32_t area, const Tair_data_pair& key) {
		return tair_remove(handler, area, key.c_str());
	}
private:
	tair_handler handler;
	char* master_address;
	char* slave_address;
	char* group;
	char* tair_level;
};

int main(int argc, char **argv) {
	int port = 9090;
	shared_ptr<tair_servHandler> handler(new tair_servHandler());
	shared_ptr<TProcessor> processor(new tair_servProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
	shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory()); 
	shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(THREAD_NUM);
	threadManager->threadFactory(threadFactory);
	threadManager->start();
	TNonblockingServer server(processor, protocolFactory, port, threadManager);
	server.serve();
	return 0;
}
