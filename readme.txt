tair 源码修改记录：
1、修改 tair_client_api_impl.cpp 第2015行 
   计数时去掉 expire_time < 0 的判断，避免后序添加计数覆盖超时时间
