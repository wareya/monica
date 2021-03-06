#ifndef NALL_HTTP_ROLE_HPP
#define NALL_HTTP_ROLE_HPP

//Role: base class for Client and Server
//provides shared functionality


#include <nall/filemap.hpp>
#include <nall/thread.hpp>
#include <nall/http/request.hpp>
#include <nall/http/response.hpp>

namespace nall { namespace HTTP {

struct Role {
  struct Settings {
    signed connectionLimit =   1024;  //server
    signed headSizeLimit   =  16384;  //client, server
    signed bodySizeLimit   =  65536;  //client, server
    signed chunkSize       =  32768;  //client, server
    signed threadStackSize = 131072;  //server
    signed timeoutReceive  =  15000;  //server
    signed timeoutSend     =  15000;  //server
  } settings;

  inline auto configure(const string& parameters) -> bool;
  inline auto download(signed fd, Message& message) -> bool;
  inline auto upload(signed fd, const Message& message) -> bool;
};

auto Role::configure(const string& parameters) -> bool {
  auto document = BML::unserialize(parameters);
  for(auto parameter : document) {
    auto name = parameter.name();
    auto value = parameter.integer();

    if(0);
    else if(name == "connectionLimit") settings.connectionLimit = value;
    else if(name == "headSizeLimit") settings.headSizeLimit = value;
    else if(name == "bodySizeLimit") settings.bodySizeLimit = value;
    else if(name == "chunkSize") settings.chunkSize = value;
    else if(name == "threadStackSize") settings.threadStackSize = value;
    else if(name == "timeoutReceive") settings.timeoutReceive = value;
    else if(name == "timeoutSend") settings.timeoutSend = value;
  }
  return true;
}

auto Role::download(signed fd, Message& message) -> bool {
  auto& head = message._head;
  auto& body = message._body;
  string chunk;
  uint8_t packet[settings.chunkSize], *p = nullptr;

  head.reset(), head.reserve(4095);
  body.reset(), body.reserve(4095);

  bool headReceived = false;
  bool chunked = false;
  bool chunkReceived = false;
  bool chunkFooterReceived = true;
  signed length = 0;
  signed chunkLength = 0;
  signed contentLength = 0;

  while(true) {
    if(auto limit = settings.headSizeLimit) if(head.size() >= limit) return false;
    if(auto limit = settings.bodySizeLimit) if(body.size() >= limit) return false;

    if(headReceived && !chunked && body.size() >= contentLength) {
      body.resize(contentLength);
      break;
    }

    if(length == 0) {
      length = recv(fd, packet, settings.chunkSize, MSG_NOSIGNAL);
      if(length <= 0) return false;
      p = packet;
    }

    if(!headReceived) {
      head.append((char)*p++);
      --length;

      if(head.endsWith("\r\n\r\n") || head.endsWith("\n\n")) {
        headReceived = true;
        if(!message.setHead()) return false;
        chunked = message.header("Transfer-Encoding").iequals("chunked");
        contentLength = decimal(message.header("Content-Length"));
      }

      continue;
    }

    if(chunked && !chunkReceived) {
      char n = *p++;
      --length;

      if(!chunkFooterReceived) {
        if(n == '\n') chunkFooterReceived = true;
        continue;
      }

      chunk.append(n);

      if(chunk.endsWith("\r\n") || chunk.endsWith("\n")) {
        chunkReceived = true;
        chunkLength = hex(chunk);
        if(chunkLength == 0) break;
        chunk.reset();
      }

      continue;
    }

    if(!chunked) {
      body.resize(body.size() + length);
      memory::copy(body.get() + body.size() - length, p, length);

      p += length;
      length = 0;
    } else {
      signed transferLength = min(length, chunkLength);
      body.resize(body.size() + transferLength);
      memory::copy(body.get() + body.size() - transferLength, p, transferLength);

      p += transferLength;
      length -= transferLength;
      chunkLength -= transferLength;

      if(chunkLength == 0) {
        chunkReceived = false;
        chunkFooterReceived = false;
      }
    }
  }

  if(!message.setBody()) return false;
  return true;
}

auto Role::upload(signed fd, const Message& message) -> bool {
  auto transfer = [&](const uint8_t* data, unsigned size) -> bool {
    while(size) {
      signed length = send(fd, data, min(size, settings.chunkSize), MSG_NOSIGNAL);
      if(length < 0) return false;
      data += length;
      size -= length;
    }
    return true;
  };

  if(message.head([&](const uint8_t* data, unsigned size) -> bool { return transfer(data, size); })) {
    if(message.body([&](const uint8_t* data, unsigned size) -> bool { return transfer(data, size); })) {
      return true;
    }
  }

  return false;
}

}}

#endif
