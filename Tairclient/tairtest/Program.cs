using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TairLib;

namespace tairtest
{
    class Program
    {
        static void Main(string[] args)
        {
            TairClient client = new TairClient();
            client.Begin("192.168.112.128:5198", "192.168.112.128:5198", "group_1");
            long re = client.Put(0, "lichao", "love", 0, 0);
            string look = client.Get(0, "lichao");
            client.Remove(0, "lichao");
            string look1 = client.Get(0, "lichao");
            int i = 1;
        }
    }
}
