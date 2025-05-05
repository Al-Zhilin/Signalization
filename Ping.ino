bool Pingyem() {
  bool inter = Ping.ping("www.yandex.ru", 3);
  
  if (inter) {
    average_ping = Ping.averageTime();
    return inter;
  }

  inter = Ping.ping("www.google.com", 3);
  average_ping = Ping.averageTime();
  return inter;
}