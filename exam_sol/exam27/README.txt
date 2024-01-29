questo file si riferisce a pack_gen.c e pack_recv.c

per vedere in funzionamento il programma lanciare pack_gen, pack_recv sarà lanciato 
automaticamente da pack_gen come child.

FUNZIONAMENTO:
pack gen genera pacchetti contenenti un numero casuale di bytes che va da MIN_PLENGTH a 
MAX_PLENGTH, i bytes che formano il pacchetto sono a loro volta generati casualmente.
il pacchetto è stato dichiarato come unsigned char:
char in quanto ogni unità del pacchetto deve essere un byte e unsigned in modo tale da poterlo
rendere piu comprensibile durante la stampa tramite %d.
una volta generato il pacchetto lo invia a pack_recv tramite una pipe.
pack recv a sua volta riceve il pacchetto tramite una thread, e in base al numero di bytes che contiene 
(che viene preso dai primi 4 bytes del pacchetto stesso) alloca la memoria necessaria per
inserirlo in una coda fifo. Altre 2 threads si occupano inoltre costamente di rimuovere 
pacchetti dalla fifo. il tutto viene sincronizzato da un mutex.
Si noti che la fifo viene stampata su terminale ogni volta che un pacchetto vine inserito o tolto.
quando viene inserito il coloro di stampa è quello di default, quando viene tolto la fifo viene stampata in rosso.
