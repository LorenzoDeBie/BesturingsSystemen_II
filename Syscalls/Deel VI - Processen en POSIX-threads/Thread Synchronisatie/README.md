# Thread Synchronisatie

1. Mutexen
  * [Waarom hebben we mutexen nodig?](no_mutex1.c):  
    Voorbeeld van een programma dat niet *thread-safe* is. Het script [test.sh](test.sh) toont dit aan.
  * [Oplossing voor dit probleem](mutex1.c)
  * Tickets
    * [Aantonen dat mutexen nodig zijn](tickets_no_lock.c)
    * [Oplossing opdracht 1](tickets_lock.c)

2. Semaforen
   * [Oplossing opdracht 2](tickets_sem.c)

3. Extra's
   * Hoe kan ik snel de laatste tien bytes van een bestand lezen zonder het volledig te overlopen? [lseek](lseek.c)
   * lseek in combinatie met read of write is niet *thread-safe*. [pread & pwrite](pread.c) zijn dat wel.