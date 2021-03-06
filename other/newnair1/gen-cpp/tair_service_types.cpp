/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "tair_service_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace tair_thrift {


Tair_data_pair::~Tair_data_pair() throw() {
}


void Tair_data_pair::__set_len(const int64_t val) {
  this->len = val;
}

void Tair_data_pair::__set_data(const std::string& val) {
  this->data = val;
}

uint32_t Tair_data_pair::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->len);
          this->__isset.len = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->data);
          this->__isset.data = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Tair_data_pair::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("Tair_data_pair");

  xfer += oprot->writeFieldBegin("len", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->len);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("data", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->data);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Tair_data_pair &a, Tair_data_pair &b) {
  using ::std::swap;
  swap(a.len, b.len);
  swap(a.data, b.data);
  swap(a.__isset, b.__isset);
}

Tair_data_pair::Tair_data_pair(const Tair_data_pair& other0) {
  len = other0.len;
  data = other0.data;
  __isset = other0.__isset;
}
Tair_data_pair& Tair_data_pair::operator=(const Tair_data_pair& other1) {
  len = other1.len;
  data = other1.data;
  __isset = other1.__isset;
  return *this;
}
void Tair_data_pair::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "Tair_data_pair(";
  out << "len=" << to_string(len);
  out << ", " << "data=" << to_string(data);
  out << ")";
}

} // namespace
