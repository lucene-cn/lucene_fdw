export PATH=/usr/local/pgsql/bin:/opt/software/jdk/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
 make clean
  make 
  make install
rm  /usr/pgsql-13/lib/kafka_fdw.so -f
   cp  /usr/local/pgsql/lib/kafka_fdw.so /usr/pgsql-13/lib/
service postgresql-13 restart

