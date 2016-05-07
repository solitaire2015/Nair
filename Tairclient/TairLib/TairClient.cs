using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Thrift.Collections;
using Thrift.Protocol;
using Thrift.Transport;

namespace TairLib
{
    public class TairClient
    {
        private TSocket socket;
        private TFramedTransport transport;
        private TProtocol protocol;
        private tair_serv.Client client;
        public TairClient()
        {
            socket = new TSocket("192.168.112.128", 9090);
            transport = new TFramedTransport(socket);
            protocol = new TBinaryProtocol(transport);
            client = new tair_serv.Client(protocol);
            transport.Open();
        }
        public string Get(long area,string key)
        {
            return client.Tair_get(area, key);
        }
        public long Put(long area,string key,string value,long expire,long version)
        {
            return client.Tair_put(area, key, value, expire, version);
        }
        public bool Begin(string master_addr,string slave_addr,string groupName)
        {
            return client.Tair_begin(master_addr, slave_addr, groupName);
        }
        public void SetLogLevel(string level)
        {
            client.Tair_set_loglevel(level);
        }
        public long Remove(long area,string key)
        {
            return client.Tair_remove(area, key);
        }
    }
}
