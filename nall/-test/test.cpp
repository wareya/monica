#include <nall/platform.hpp>
#include <nall/any.hpp>
#include <nall/bit.hpp>
#include <nall/bitvector.hpp>
#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/dl.hpp>
#include <nall/dsp.hpp>
#include <nall/file.hpp>
#include <nall/file-system-object.hpp>
#include <nall/filemap.hpp>
#include <nall/function.hpp>
#include <nall/hashset.hpp>
#include <nall/hid.hpp>
#include <nall/image.hpp>
#include <nall/intrinsics.hpp>
#include <nall/invoke.hpp>
#include <nall/map.hpp>
#include <nall/matrix.hpp>
#include <nall/maybe.hpp>
#include <nall/memory.hpp>
#include <nall/mosaic.hpp>
#include <nall/priority-queue.hpp>
#include <nall/property.hpp>
#include <nall/random.hpp>
#include <nall/range.hpp>
#include <nall/serializer.hpp>
#include <nall/service.hpp>
#include <nall/set.hpp>
#include <nall/shared-memory.hpp>
#include <nall/shared-pointer.hpp>
#include <nall/stdint.hpp>
#include <nall/stream.hpp>
#include <nall/string.hpp>
#include <nall/thread.hpp>
#include <nall/traits.hpp>
#include <nall/utility.hpp>
#include <nall/varint.hpp>
#include <nall/vector.hpp>
#include <nall/beat/delta.hpp>
#include <nall/beat/linear.hpp>
#include <nall/beat/metadata.hpp>
#include <nall/beat/patch.hpp>
#include <nall/beat/multi.hpp>
#include <nall/beat/archive.hpp>
#ifdef _WIN32
  #include <nall/database/odbc.hpp>
#else
  #include <nall/database/sqlite3.hpp>
#endif
#include <nall/decode/base64.hpp>
#include <nall/decode/bmp.hpp>
#include <nall/decode/gzip.hpp>
#include <nall/decode/inflate.hpp>
#include <nall/decode/png.hpp>
#include <nall/decode/zip.hpp>
#include <nall/encode/base64.hpp>
#include <nall/encode/bmp.hpp>
#include <nall/encode/zip.hpp>
#include <nall/hash/crc16.hpp>
#include <nall/hash/crc32.hpp>
#include <nall/hash/sha256.hpp>
#include <nall/http/client.hpp>
#include <nall/http/server.hpp>
#include <nall/stream/stream.hpp>
#include <nall/stream/memory.hpp>
#include <nall/stream/vector.hpp>
#ifdef _WIN32
  #include <nall/windows/guid.hpp>
  #include <nall/windows/registry.hpp>
  #include <conio.h>
#endif
using namespace nall;

#include <nall/main.hpp>

/*
auto nall::main(lstring) -> void {
  auto fp = popen(string{"icarus -m \"/home/byuu/Emulation/Famicom/Mega Man 3 (USA).fc/\""}, "r");
  if(fp) {
    string output;
    char byte;
    while((byte = fgetc(fp)) != EOF) output.append(byte);
    pclose(fp);
    print(output, "\n");
  } else {
    print("fail\n");
  }
}
*/

/*
auto nall::main(lstring) -> void {
  file::remove("database.sqlite");
  Database::SQLite3 db("database.sqlite");

  db.execute("drop table if exists users");
  db.execute("create table users(id integer primary key, role text, name text, age integer)");

  db.execute("begin");
  for(unsigned n = 23; n < 72; n++) {
    db.execute("insert into users(role, name, age) values(?, ?, ?)", "Administrator").bind(1, string{"byuu ", n}).bind(2, n);
  }
  db.execute("commit");

print("---\n");

  for(auto row : db.execute("select * from users")) {
    print("[", row.columns(), "] ", row.integer(0), ",", row.text(1), ",", row.text(2), ",", row.integer(3), "\n");
  }

print("---\n");

  if(auto row = db.execute("select * from users where id = ?", 3)) {
    print("[", row.columns(), "] ", row.integer(0), ",", row.text(1), ",", row.text(2), ",", row.integer(3), "\n");
  }

print("---\n");

  auto q = db.execute("select * from users limit 10,5");
  while(q.step()) print(q.integer(0), "\n");

print("---\n");

  print(db.execute("select count(*) from users").integer(0), "\n");
}
*/

auto nall::main(lstring args) -> void {
  { auto output = DML().parse("document.dml");
    print(output, "\n\n");
  }
  { auto output = CML().parse("document.cml");
    print(output, "\n\n");
  }
}

/*
auto nall::main(lstring args) -> void {
  std::atomic<unsigned> threads{0};
  std::atomic<uint64_t> counter{0};
  for(unsigned t = 0; t < 16; t++) {
    thread::create([&](uintptr_t) {
      ++threads;
      for(unsigned n = 0; n < 1000000; n++) {
        string s{n};
        counter += decimal(s);
      }
      --threads;
    });
  }
  while(threads > 0) usleep(20 * 1000);
  print(counter == 0x746a4ae6e00ull, ": ", hex(counter), "\n");
}
*/

/*
auto increment(const string& s) -> string {
  return {decimal(s) + 1};
}

auto nall::main(lstring args) -> void {
  volatile signed c = 0;
  clock_t a = clock();
  string s = "0";
  for(unsigned n = 0; n < 10000000; n++) {
    s = increment(s);
  }
  clock_t b = clock();
  print((double)(b - a) / CLOCKS_PER_SEC, "s\n");
}
*/
